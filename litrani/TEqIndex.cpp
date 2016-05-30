// @(#)root/html:$Id: TEqIndex.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TMath.h"
#include "TwoPadDisplay.h"
#include "TMessErr.h"
#include "TLitGlob.h"
#include "TEqIndex.h"

TEqIndex* gIdx = 0;

ClassImp(TEqIndex)
//______________________________________________________________________________
//
//
//        --------------------------------------------------------------
//        |                                                             |
//        |  This class is an internal class of Litrani. It is not ne-  |
//        | cessary for a user of Litrani to know anything about it.    |
//        | The documentation about it is of interest only for those    |
//        | interested in the inner working of Litrani.                 |
//        |                                                             |
//         -------------------------------------------------------------
//
// TEqIndex handles the equation :
//
//    z^2  + a*z + (b*z+c)*Sqrt(z-g) + d == 0     (E1)
//
//    where a,b,c,d,g are always real, but the 4 solutions z may be complex.
//
//  This is the equation which gives the 2 indices in case of anisotropy.
//It has the strange property of having 4 (not 2 !) solutions. Only 2 of
//the 4 solutions correspond to a wave having a Poyinting vector going in
//the right direction. But we cannot know in advance which 2 among the 4
//are the right one! We have to go until the calculation of the Poyinting
//vector to know which 2 solutions have to be kept.
//
TEqIndex::TEqIndex(const char *name) {
  fName = name;
  InitP();
  a=-10.15; b=0.18; c=-0.93; d=25.75;g=4.9;
  fDebug = kFALSE;
  fResidus0 = 0;
  fResidus1 = 0;
  fP4 = new TPol4(0.0);
  Init();
}
TEqIndex::TEqIndex(const char *name, Int_t i) {
  fName = name;
  InitP();
  fDebug = kFALSE;
  fResidus0 = 0;
  fResidus1 = 0;
  fP4 = new TPol4(0.0);
  Difficult(i);
}
TEqIndex::TEqIndex(const char *name, Double_t a0,Double_t b0,Double_t c0,Double_t d0,
  Double_t g0) {
  fName = name;
  InitP();
  a=a0; b=b0;c=c0;d=d0;g=g0;
  fDebug = kFALSE;
  fResidus0 = 0;
  fResidus1 = 0;
  fP4 = new TPol4(0.0);
  Init();
}
TEqIndex::~TEqIndex() {
  if (fP4)       delete fP4;
  if (fGraphP)   delete fGraphP;
  if (fGraphM)   delete fGraphM;
  if (fResidus0) delete fResidus0;
  if (fResidus1) delete fResidus1;
}
void TEqIndex::Better(TSComplex &z,Bool_t &bs,Double_t &ps) {
  //Make solution better by Super Newton iteration.
  //But, before, we look whether we are in the case of "conspiracy" between the
  //coefficients of the equation, such that equation :
  //
  //  z^2  + a*z + (b*z+c)*Sqrt(z-g) + d == 0     (E1)
  //
  //degenerates into 2 equations, which have the SAME z solutions :
  //
  //  z^2  + a*z + d == 0                         (E1a)
  //  b*z+c == 0                                  (E1b)
  //
  //In this case of conspiracy, the solution is real, although Sqrt(z-g)
  //may very well be imaginary : the index is real, but the wave is evanescent
  //
  //We also consider the special case where the solution z is in the neighborhood
  //of g. In that case, the square root is almost zero, so that all the derivatives
  //of the SuperNewton method are almost infinite, which is quite bad. In that
  //case, we call Better4, which tries to solve the equation (E1) squared, so
  //that Sqrt(z-g) has disappeared.
  //
  const Double_t small  = 1.0e-6;
  const Double_t vsmall = 1.0e-9;
  const Double_t nearg  = 0.002;
  const Double_t neargs = 1.0e-5;
  Double_t aa,bb,cc;
  Double_t eps1,eps2,eps3,eps;
  Double_t x;
  TSComplex zl;
  Bool_t HasConverged;
  Bool_t bl;
  Double_t pl;
  x    = -c/b;
  zl   = z - x;
  eps3 = Abs(zl);
  if ((TMath::Abs(z.Im())<small) && (eps3<small)) {
    aa = (3*a)/2;
    bb = (a*a + b*b + 2*d)/2;
    cc = (a*d + b*c)/2;
    Troisieme(x,aa,bb,cc);
    eps1 = TMath::Abs((b*x+c)*TMath::Sqrt(TMath::Abs(x-g)));
    eps2 = TMath::Abs(x*x + a*x + d);
    zl   = z - x;
    eps3 = Abs(zl);
    eps  = eps1 + eps2;
    if ((eps3<small) && (eps<vsmall)) {
      z  = x;
      ps = eps;
      return;
    }
  }
  //
  //special case neighborhood of g
  //
  zl = z - g;
  eps3 = Abs(zl);
  if (eps3<nearg) {
    zl = z;
    pl = ps;
    //    HasConverged = SuperNewt4(zl,pl);
    HasConverged = FZpSol(zl,bs,pl);
    if (HasConverged) {
      z    = zl;
      //      fRac = Rac(z,bs);
      ps   = pl;
      //      zl   = F0(z);
      //      ps   = Abs(zl);
      return;
    }
    else
      if (eps3<neargs) return;
  }
  //
  //normal case
  //
  zl = z;
  pl = ps;
  HasConverged = SuperNewton(zl,bs,pl);
  if (HasConverged) {
    z  = zl;
    ps = pl;
  }
  else {
    bl = !bs;
    HasConverged = SuperNewton(zl,bl,pl);
    if (HasConverged) {
      z  = zl;
      bs = bl;
      ps = pl;
    }
  }
}
Short_t TEqIndex::ClosestFrom(Double_t ni,Bool_t bi) {
  //  Find which among the 4 solution is closest to ni, ni being real. So we look
  //only among the real solutions. If there is no real solution, returns -1.
  //The reason of this routine is that when calculating the transition from
  //anisotropic to isotropic medium, the index of refraction of the incident
  //wave is found among the 4 solutions. But this solution must be avoided !
  //  If the same index (or almost the same) appear twice, we kill the one
  //having a cos(theta) different from the incident wave
  const Double_t eps = 1.0e-6;
  Short_t k = -1;
  Double_t ni2,d2;
  Double_t d1 = -1.0;
  Double_t x0,x1,x2,x3;
  ni2 = ni*ni;
  x0  = fZ0.Re();
  x1  = fZ1.Re();
  x2  = fZ2.Re();
  x3  = fZ3.Re();
  if (!fR0) {
    d1 = TMath::Abs(x0 - ni2);
    k  = 0;
    if (fB0==bi) d1 += eps;
  }
  if (!fR1) {
    d2 = TMath::Abs(x1 - ni2);
    if (d2<d1) {
      d1 = d2;
      k  = 1;
      if (fB1==bi) d1 += eps;
    }
  }
  if (!fR2) {
    d2 = TMath::Abs(x2 - ni2);
    if (d2<d1) {
      d1 = d2;
      k  = 2;
      if (fB2==bi) d1 += eps;
    }
  }
  if (!fR3) {
    d2 = TMath::Abs(x3 - ni2);
    if (d2<d1) k = 3;
  }
  return k;
}
void TEqIndex::Difficult(Short_t i) {
  //tests a certain number of difficult cases
  const Short_t n = 17;
  Double_t aa[n],bb[n],cc[n],dd[n],gg[n];

  aa[0] = -10.3504799049;
  bb[0] = 0;
  cc[0] = 0;
  dd[0] = 26.7831085656;
  gg[0] = 4.94963995708e-032;
  //gd : 1.59940959851e-065

  aa[1] =  -8.178867598;
  bb[1] =  0.100537665;
  cc[1] =  -0.410329585;
  dd[1] =  16.72340342;
  gg[1] =  4.056168105;

  aa[2] =  -9.92656197;
  bb[2] =  -0.050933878;
  cc[2] =  0.236894902;
  dd[2] =  24.53312898;
  gg[2] =  5.642503553;

  aa[3] = -8.181866693;
  bb[3] = 0.100398773;
  cc[3] = -0.409762717;
  dd[3] = 16.73564384;
  gg[3] = 4.045059063;

  aa[4] = -8.936733016;
  bb[4] = -0.123588686;
  cc[4] = 0.568464495;
  dd[4] = 19.94906434;
  gg[4] = 2.616004553;

  aa[5] = -9.142767119;
  bb[5] = 0.171529796;
  cc[5] = -0.923607897;
  dd[5] = 20.66146166;
  gg[5] = 5.05766914;

  aa[6] = -8.55266665;
  bb[6] = 0.473426746;
  cc[6] = -1.857679281;
  dd[6] = 18.16281757;
  gg[6] = 4.684890774;

  aa[7] = -10.15001701;
  bb[7] = 0.181357029;
  cc[7] = -0.927470126;
  dd[7] = 25.75418659;
  gg[7] = 4.928623823;

  aa[8] = -10.08767843;
  bb[8] = -0.006808947;
  cc[8] = 0.031900746;
  dd[8] = 25.31163552;
  gg[8] = 5.538174615;

  aa[9] = -7.903871398;
  bb[9] = 0.007578013;
  cc[9] = -0.029455271;
  dd[9] = 15.61357111;
  gg[9] = 4.01693301;

  aa[10] = -8.259262806;
  bb[10] = -0.113456731;
  cc[10] = 0.463056825;
  dd[10] = 17.05152456;
  gg[10] = 4.1787326;

  aa[11] = -9.9455033681907;
  bb[11] = 0.13395666428851;
  cc[11] = -0.67481069492000;
  dd[11] = 24.725064307763;
  gg[11] = 4.9164625172000;

  aa[12] = -9.9909361989730;
  bb[12] = 0.086507298251473;
  cc[12] = -0.43534503197112;
  dd[12] = 24.954074234765;
  gg[12] = 4.9723218636147;

  aa[13] = -10.072670538675;
  bb[13] = -6.115646185501e-7;
  cc[13] = 3.2117853133233e-6;
  dd[13] = 25.318268855649;
  gg[13] = 5.1734532854320;

  aa[14] = -9.5952649378212;
  bb[14] = -3.0347345970167e-7;
  cc[14] = 1.5001682392998e-6;
  dd[14] = 22.996050705858;
  gg[14] = 4.6631865442249;

  aa[15] = -9.4980772729929;
  bb[15] = 0.073830803411596;
  cc[15] = -0.35073079100128;
  dd[15] = 22.553111378568;
  gg[15] = 2.4256796562818;

  aa[16] = -10.529263729689;
  bb[16] = 0.11156676113325;
  cc[16] = -0.59571593153035;
  dd[16] = 27.714501928828;
  gg[16] = 5.2281248129843;

  if (!gLit) gLit = new TLitGlob(5);
  if (!gIdx) {
    gIdx = new TEqIndex("TEqIndex");
    if (gMes->TestCalc()) gIdx->SetDebug();
  }
  Set(aa[i],bb[i],cc[i],dd[i],gg[i]);
  fGd = a*a-4.0*d;
  Solve(3,kTRUE);
  PutIn();
  ShowEqu();
  PrintSolution();
}
void TEqIndex::Exchange(Short_t i, Short_t j) {
  //Exchange solution i with solution j
  TSComplex z;
  Double_t s,c;
  Bool_t b,r;

  z       = fZc[j];
  fZc[j]  = fZc[i];
  fZc[i]  = z;

  b       = fBc[j];
  fBc[j]  = fBc[i];
  fBc[i]  = b;

  r       = fRc[j];
  fRc[j]  = fRc[i];
  fRc[i]  = r;

  s       = fQsc[j];
  fQsc[j] = fQsc[i];
  fQsc[i] = s;

  c       = fQjc[j];
  fQjc[j] = fQjc[i];
  fQjc[i] = c;
}
TSComplex TEqIndex::F0(const TSComplex &z) const {
  //Calculates F = z^2  + a*z + (b*z+c)*Sqrt(z-g) + d
  //Sqrt(z-g) must have been calculated by a call to Rac() and its result put
  //into fRac before calling F !
  return z*z  + a*z + (b*z+c)*fRac + d;
}
TSComplex TEqIndex::F40(const TSComplex &z) const {
  //F40 = z^4 + fA*z^3 + fB*z^2 + fC*z + fD
  TSComplex z2,z3,z4,zz;
  z2 = z*z;
  z3 = z*z2;
  z4 = z*z3;
  zz = z4 + fA*z3 + fB*z2 + fC*z + fD;
  return zz;
}
TSComplex TEqIndex::F4p(const TSComplex &z) const {
  //first derivative of F40 = z^4 + fA*z^3 + fB*z^2 + fC*z + fD
  const Double_t deux   = 2.0;
  const Double_t trois  = 3.0;
  const Double_t quatre = 4.0;
  TSComplex z2,z3,zz;
  z2 = z*z;
  z3 = z*z2;
  zz = quatre*z3 + trois*fA*z2 + deux*fB*z + fC;
  return zz;
}
TSComplex TEqIndex::F4s(const TSComplex &z) const {
  //second derivative of F40 = z^4 + fA*z^3 + fB*z^2 + fC*z + fD
  const Double_t deux   = 2.0;
  const Double_t six    = 6.0;
  const Double_t douze  = 12.0;
  TSComplex z2,zz;
  z2 = z*z;
  zz = douze*z2 + six*fA*z + deux*fB;
  return zz;
}
void TEqIndex::FillResidu(Double_t eps,Bool_t b) {
  //Fill residus of guess
  const Double_t ln10 = 2.30258509299404;
  Int_t bin;
  Double_t x;
  x = TMath::Log(eps)/ln10;
  if (b) bin = fResidus1->Fill(x);
  else   bin = fResidus0->Fill(x);
}
TSComplex TEqIndex::Fp(const TSComplex &z) const {
  //Calculates the 1st derivative of F0()
  //Sqrt(z-g) must have been calculated by a call to Rac() and its result put
  //into fRac before calling Fp !
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t wsmall = 1.0e-50;
  Double_t anum,aden;
  TSComplex num,den;
  TSComplex zz;
  num  = (b*z+c);
  anum = Abs(num);
  den  = deux*fRac;
  aden = Abs(den);
  if (aden<wsmall) {
    den = wsmall;
    if (anum<wsmall) num = zero;
  }
  zz = deux*z + a + b*fRac + num/den;
  return zz;
}
TSComplex TEqIndex::Fs(const TSComplex &z) const {
  //Calculates the 2nd derivative of F0()
  //Sqrt(z-g) must have been calculated by a call to Rac() and its result put
  //into fRac before calling Fs !
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  const Double_t wsmall = 1.0e-50;
  TSComplex num,den,den2,zz;
  Double_t anum,aden,aden2;
  num   = b*(z-deux*g) - c;
  anum  = Abs(num);
  den   = deux*fRac;
  aden  = Abs(den);
  den2  = quatre*fRac*fRac*fRac;
  aden2 = Abs(den2);
  if (aden2<wsmall) {
    den2 = wsmall;
    if (anum<wsmall) num = zero;
  }
  if (aden<wsmall) {
    den = wsmall;
  }
  zz = deux + b/den + num/den2;
  return zz;
}
TSComplex TEqIndex::FZp0(const TSComplex &y) const {
  //For solution near g
  TSComplex y2,y3,y4,z;
  y2 = y*y;
  y3 = y*y2;
  y4 = y*y3;
  z  = y4 + fZpa3*y3 + fZpa2*y2 + fZpa1*y + fZpa0;
  return z;
}
TSComplex TEqIndex::FZp1(const TSComplex &y) const {
  //For solution near g
  const Double_t deux   = 2.0;
  const Double_t trois  = 3.0;
  const Double_t quatre = 4.0;
  TSComplex y2,y3,z;
  y2 = y*y;
  y3 = y*y2;
  z = quatre*y3 + trois*fZpa3*y2 + deux*fZpa2*y + fZpa1;
  return z;
}
TSComplex TEqIndex::FZp2(const TSComplex &y) const {
  //For solution near g
  const Double_t deux   = 2.0;
  const Double_t douze  = 12.0;
  const Double_t six    = 6.0;
  TSComplex y2,z;
  y2 = y*y;
  z = douze*y2 + six*fZpa3*y + deux*fZpa2;
  return z;
}
TSComplex TEqIndex::FZpIter(const TSComplex &y0) const {
  TSComplex z,y1;
  TSComplex G0,G1,G2,H1,H2;
  Double_t a01,a11,a02,b01,b02;
  Double_t c11,c12,c22,d1,d2,dd,dx,dy;
  G0  = FZp0(y0);
  G1  = FZp1(y0);
  G2  = FZp2(y0);
  H1  = G1;
  H1.C();
  H2  = G2;
  H2.C();
  z   = G0*H1;
  a01 = z.Re();
  b01 = z.Im();
  z   = G0*H2;
  a02 = z.Re();
  b02 = z.Im();
  z   = G1*H1;
  a11 = z.Re();
  c11 = a02 + a11;
  c12 = b02;
  c22 = a11 - a02;
  d1  = -a01;
  d2  = -b01;
  dd  = c11*c22 - c12*c12;
  dx  = (d1*c22 - c12*d2)/dd;
  dy  = (c11*d2 - d1*c12)/dd;
  z.Set(dx,dy);
  y1  = y0 + z;
  return y1;
}
Bool_t TEqIndex::FZpSol(TSComplex &z,Bool_t &bs,Double_t &eps) {
  //Better z in case z close from g
  const Int_t MaxIter=40;
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t wsmall = 1.0e-10;
  Bool_t HasConverged;
  Double_t eps1,eps2;
  TSComplex rac1;
  Double_t epsy = un;
  TSComplex y0,y1;
  Int_t k=0;
  y0 = Rac(z,bs);
  while ((k<MaxIter) && (epsy>wsmall)) {
    y1 = FZpIter(y0);
    epsy = Abs(FZp0(y1));
    y0 = y1;
    k++;
  }
  HasConverged = epsy<wsmall;
  if (!HasConverged) {
    y0 = zero;
    k = 0;
    epsy = un;
    while ((k<MaxIter) && (epsy>wsmall)) {
      y1 = FZpIter(y0);
      epsy = Abs(FZp0(y1));
      y0 = y1;
      k++;
    }
    HasConverged = epsy<wsmall;
  }
  if (HasConverged) {
    //Solution for rac positiv :
    z = y1*y1 + g;
    fRac = Rac(z,kTRUE);
    rac1 = fRac;
    eps1 = Abs(F0(z));
    //Solution for rac negativ
    fRac = Rac(z,kFALSE);
    eps2 = Abs(F0(z));
    if (eps1<=eps2) {
      eps  = eps1;
      fRac = rac1;
      bs   = kTRUE;
    }
    else {
      eps  = eps2;
      bs   = kFALSE;
    }
  }
  return HasConverged;
}
Bool_t TEqIndex::GetfB(Short_t i) const {
  //Get solution number i, i=0..3
  Bool_t b = kFALSE;
  i %= 4;
  switch (i) {
  case 0:
    b = fB0;
    break;
  case 1:
    b = fB1;
    break;
  case 2:
    b = fB2;
    break;
  case 3:
    b = fB3;
    break;
  }
  return b;
}
TSComplex TEqIndex::GetfD(Short_t i) const {
  //Get solution number i, i=0..3
  TSComplex z;
  i %= 4;
  switch (i) {
  case 0:
    z = fD0;
    break;
  case 1:
    z = fD1;
    break;
  case 2:
    z = fD2;
    break;
  case 3:
    z = fD3;
    break;
  }
  return z;
}
Bool_t TEqIndex::GetfR(Short_t i) const {
  //Get solution number i, i=0..3
  Bool_t b = kFALSE;
  i %= 4;
  switch (i) {
  case 0:
    b = fR0;
    break;
  case 1:
    b = fR1;
    break;
  case 2:
    b = fR2;
    break;
  case 3:
    b = fR3;
    break;
  }
  return b;
}
TSComplex TEqIndex::GetfZ(Short_t i) const {
  //Get solution number i, i=0..3
  TSComplex z;
  i %= 4;
  switch (i) {
  case 0:
    z = fZ0;
    break;
  case 1:
    z = fZ1;
    break;
  case 2:
    z = fZ2;
    break;
  case 3:
    z = fZ3;
    break;
  }
  return z;
}
Short_t TEqIndex::Identical(Bool_t twice,Bool_t &changed,Bool_t *zsame) {
  //We cannot accept without further investigation that 2 solutions are identical
  const Double_t identlim = 1.0e-11;
  const Double_t LimConv  = 1.0e-10;
  const Double_t limreal2 = 1.0e-7;
  Bool_t bbb,HasConverged;
  Short_t kret = 0;
  Double_t eps,ddd;
  TSComplex zz;
  Short_t i,j;
  changed = kFALSE;
  for (i=0;i<4;i++) zsame[i] = kFALSE;
  for (i=0;i<3;i++)
    for (j=i+1;j<4;j++) {
      if (fBc[i] == fBc[j]) {
        zz  = fZc[i] - fZc[j];
        eps = Abs(zz);
        if (eps<identlim) {
          //Here 2 solutions are identical. We try to change the sign of Sqrt(z-g) of the
          //second solution.
          if (twice) zz = -fA -fZc[0] -fZc[1] -fZc[2] -fZc[3] + fZc[j];
          else zz = fZc[j];
          bbb  = !fBc[j];
          ddd  = LimConv;
          HasConverged = SuperNewton(zz,bbb,ddd);
          if (HasConverged) {
            changed = kTRUE;
            if (TMath::Abs(zz.Im())<limreal2) {
              zz.PureReal();
              fRc[j] = kFALSE;
            }
            else {
              fRc[j] = kTRUE;
            }
            fZc[j]  = zz;
            fBc[j]  = bbb;
            fQsc[j] = ddd;
          }
          else if (!zsame[j]) {
            kret += 1;
            zsame[j] = kTRUE;
          }
        }
      }
    }
    return kret;
}
Short_t TEqIndex::IdenticalF(Bool_t *zsame) const {
  //We cannot accept without further investigation that 2 solutions are identical
  const Double_t identlim = 1.0e-11;
  Short_t kret = 0;
  Double_t eps;
  TSComplex zz;
  Short_t i,j;
  for (i=0;i<4;i++) zsame[i] = kFALSE;
  for (i=0;i<3;i++)
    for (j=i+1;j<4;j++) {
      if (fBc[i] == fBc[j]) {
        zz  = fZc[i] - fZc[j];
        eps = Abs(zz);
        if (eps<identlim) {
          //Here 2 solutions are identical.
          kret += 1;
          zsame[j] = kTRUE;
        }
      }
    }
    return kret;
}
void TEqIndex::Ini4() {
  const Double_t deux = 2.0;
  fA = deux*a - b*b;
  fB = a*a + deux*d + b*b*g - deux*b*c;
  fC = deux*a*d - c*c + deux*b*c*g;
  fD = d*d + c*c*g;
  fZpa3 = b;
  fZpa2 = a + deux*g;
  fZpa1 = b*g + c;
  fZpa0 = g*g + a*g + d;
}
void TEqIndex::InifZ() {
  //Put an initial value into everything
  const Double_t zero = 0.0;
  const Double_t cent = 100.0;
  fZ0  = zero;
  fZ1  = zero;
  fZ2  = zero;
  fZ3  = zero;;
  fB0  = kTRUE;;
  fB1  = kTRUE;
  fB2  = kTRUE;
  fB3  = kTRUE;
  fR0  = kFALSE;
  fR1  = kFALSE;
  fR2  = kFALSE;
  fR3  = kFALSE;
  fQs0 = cent;
  fQs1 = cent;
  fQs2 = cent;
  fQs3 = cent;
  fQj0 = cent;
  fQj1 = cent;
  fQj2 = cent;
  fQj3 = cent;
}
void TEqIndex::Init() {
  const Double_t zero   = 0.0;
  const Double_t vsmall = 1.0e-8;
  Double_t ab,ac,y1;
  fSolved  = kFALSE;
  fTypeEq  = general;
  fNr      = 0;
  f0Chosen = 0.0; f1Chosen = 0.0; f2Chosen = 0.0;
  fNbin    = 200;
  fXlow    = 0.0;
  fXup     = 6.0;
  //If b AND c very small, we put them to 0
  ab = TMath::Abs(b);
  ac = TMath::Abs(c);
  if ((ab<vsmall) && (ac<vsmall)) {
    fTypeEq = bczero;
    fEnongen = ab + ac;
    b = zero;
    c = zero;
  }
  else {
    y1 = -c/b;
    ab = TMath::Abs(y1*y1 + a*y1 + d);
    if (ab<vsmall) {
      fTypeEq = degenerate;
      fEnongen = ab;
    }
  }
  Ini4();
  InifZ();
}
void TEqIndex::InitP() {
  fP4       = 0;
  fGraphP   = 0;
  fGraphM   = 0;
  fResidus0 = 0;
  fResidus1 = 0;
}
void TEqIndex::Order1() {
  //Put the solution under evaluation, fZc, in increasing order of absolute
  //value of imaginary part
  Bool_t ok = kFALSE;
  while (!ok) {
    ok = kTRUE;
    if (TMath::Abs(fZc[1].Im())<TMath::Abs(fZc[0].Im())) {
      ok       = kFALSE;
      Exchange(0,1);
    }
    if (TMath::Abs(fZc[2].Im())<TMath::Abs(fZc[1].Im())) {
      ok       = kFALSE;
      Exchange(1,2);
    }
    if (TMath::Abs(fZc[3].Im())<TMath::Abs(fZc[2].Im())) {
      ok       = kFALSE;
      Exchange(2,3);
    }
  }
}
Double_t TEqIndex::Order2() {
  //Group complex solutions into complex conjugate pairs
  const Double_t zero = 0.0;
  Double_t dps,dps2;
  Double_t dpsi[3];
  Double_t x1,x2;
  switch (fNr) {
  case 0:
    dpsi[0] = TMath::Abs(fZc[0].Im() + fZc[1].Im());
    dpsi[1] = TMath::Abs(fZc[0].Im() + fZc[2].Im());
    dpsi[2] = TMath::Abs(fZc[0].Im() + fZc[3].Im());
    if (dpsi[0]<=dpsi[1]) {
      if (dpsi[0]<=dpsi[2]) {
        //      ii = 0; jj = 1; ll = 2;  mm = 3;
        dps     = dpsi[0];
        dps2    = TMath::Abs(fZc[2].Im() + fZc[3].Im());
        fQjc[0] = dps;
        fQjc[1] = dps;
        fQjc[2] = dps2;
        fQjc[3] = dps2;
      }//if (dpsi[0]<=dpsi[2])
      else {
        //      ii = 0; jj = 3;  ll = 1; mm = 2;
        dps  = dpsi[2];
        dps2 = TMath::Abs(fZc[1].Im() + fZc[2].Im());
        fQjc[0] = dps;
        fQjc[3] = dps;
        fQjc[1] = dps2;
        fQjc[2] = dps2;
        Exchange(1,3);  // (0,3,1,2) -> (0,1,3,2)
        Exchange(2,3);  // (0,1,3,2) -> (0,1,2,3)
      }//end else if (dpsi[0]<=dpsi[2])
    }//(dpsi[0]<=dpsi[1])
    else {
      if (dpsi[1]<=dpsi[2]) {
        //      ii = 0; jj = 2;  ll = 1; mm = 3;
        dps  = dpsi[1];
        dps2 = TMath::Abs(fZc[1].Im() + fZc[3].Im());
        fQjc[0] = dps;
        fQjc[2] = dps;
        fQjc[1] = dps2;
        fQjc[3] = dps2;
        Exchange(1,2);  // (0,2,1,3) -> (0,1,2,3)
      }//if (dpsi[0]<=dpsi[2])
      else {
        //      ii = 0; jj = 3;  ll = 1; mm = 2;
        dps  = dpsi[2];
        dps2 = TMath::Abs(fZc[1].Im() + fZc[2].Im());
        fQjc[0] = dps;
        fQjc[3] = dps;
        fQjc[1] = dps2;
        fQjc[2] = dps2;
        Exchange(1,3);  // (0,3,1,2) -> (0,1,3,2)
        Exchange(2,3);  // (0,1,3,2) -> (0,1,2,3)
      }//end else if (dpsi[0]<=dpsi[2])
    }//end else (dpsi[0]<=dpsi[1])
    dps = TMath::Max(dps,dps2);
    break;
  case 2:
    dps = TMath::Abs(fZc[2].Im() + fZc[3].Im());
    fQjc[0] = zero;
    fQjc[1] = zero;
    fQjc[2] = dps;
    fQjc[3] = dps;
    break;
  default:
    dps = zero;
    fQjc[0] = zero;
    fQjc[1] = zero;
    fQjc[2] = zero;
    fQjc[3] = zero;
    break;
  }
  //Now we group the solutions into pairs of complex conjugate, putting the
  //less imaginary first. We also put the negative imaginary of a complex
  //conjugate pair before the positive imaginary.
  x1 = TMath::Abs(fZc[0].Im());
  x2 = TMath::Abs(fZc[2].Im());
  if (fZc[0].Im()>fZc[1].Im()) Exchange(0,1);
  if (fZc[2].Im()>fZc[3].Im()) Exchange(2,3);
  if (x1>x2) {
    Exchange(0,2);
    Exchange(1,3);
  }
  return dps;
}
void TEqIndex::Order3() {
  //Put real solutions with +Sqrt(z-g) before real solutions with -Sqrt(z-g)
  //If 2 solutions are with +Sqrt(z-g), put the biggest one first.
  Short_t is,is1;
  Bool_t bad = kTRUE;
  Bool_t cd1,cd2,cd3;
  switch (fNr) {
  case 2:
    cd1 = fZc[1].Re() > fZc[0].Re();
    cd2 = !(fBc[0] && (!fBc[1]));
    cd3 =  (((!fBc[0]) && fBc[1]) || (cd2 && cd1));
    if (cd3) Exchange(0,1);
    break;
  case 4:
    while (bad) {
      bad = kFALSE;
      for (is=0;is<3;is++) {
        is1 = is + 1;
        cd1 = fZc[is1].Re() > fZc[is].Re();
        cd2 = !(fBc[is] && (!fBc[is1]));
        cd3 =  (((!fBc[is]) && fBc[is1]) || (cd2 && cd1));
        if (cd3) {
          bad = kTRUE;
          Exchange(is,is1);
        }
      }
    }
    break;
  default:
    break;
  }
}
void TEqIndex::PrintSolution() const {
  // Print the 4 roots
  Short_t k;
  TString s;
  s = "Final result.  Type: ";
  switch (fTypeEq) {
    case bczero:
      s.Append("bczero");
      break;
    case degenerate:
      s.Append("degenerate");
      break;
    case general:
      s.Append("general");
      break;
  }
  cout << endl;
  cout << s.Data() << "    g = ";
  cout.width(20);
  cout.precision(12);
  cout << g << endl;
  cout << endl;
  for (k=0;k<4;k++) {
    cout << "fZc[" << k << "] = " << fZc[k] << endl;
    cout << "fBc[" << k << "] = " << fBc[k] << endl;
    cout << "fRc[" << k << "] = " << fRc[k] << endl;
    cout << endl;
  }
}
void TEqIndex::PutIn() {
  //fZi into fZc
  fZc[0]  = fZ0;
  fZc[1]  = fZ1;
  fZc[2]  = fZ2;
  fZc[3]  = fZ3;
  fBc[0]  = fB0;
  fBc[1]  = fB1;
  fBc[2]  = fB2;
  fBc[3]  = fB3;
  fRc[0]  = fR0;
  fRc[1]  = fR1;
  fRc[2]  = fR2;
  fRc[3]  = fR3;
  fGok[0] = kFALSE;
  fGok[1] = kFALSE;
  fGok[2] = kFALSE;
  fGok[3] = kFALSE;
  fQsc[0] = fQs0;
  fQsc[1] = fQs1;
  fQsc[2] = fQs2;
  fQsc[3] = fQs3;
  fQjc[0] = fQj0;
  fQjc[1] = fQj1;
  fQjc[2] = fQj2;
  fQjc[3] = fQj3;
}
void TEqIndex::PutOut() {
  //fZc into fZi
  fZ0  = fZc[0];
  fZ1  = fZc[1];
  fZ2  = fZc[2];
  fZ3  = fZc[3];
  fB0  = fBc[0];
  fB1  = fBc[1];
  fB2  = fBc[2];
  fB3  = fBc[3];
  fR0  = fRc[0];
  fR1  = fRc[1];
  fR2  = fRc[2];
  fR3  = fRc[3];
  fQs0 = fQsc[0];
  fQs1 = fQsc[1];
  fQs2 = fQsc[2];
  fQs3 = fQsc[3];
  fQj0 = fQjc[0];
  fQj1 = fQjc[1];
  fQj2 = fQjc[2];
  fQj3 = fQjc[3];
}
TSComplex TEqIndex::Rac(const TSComplex &z,Bool_t b) const {
  //Calculation of Sqrt(z-g)
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t wsmall = 1.0e-300;
  Double_t x,y,r;
  TSComplex zz;
  TSComplex I(zero,un);
  x = z.Re();
  y = TMath::Abs(z.Im());
  if (y<wsmall) {
    //Quantity under Sqrt is real
    r = x - g;
    if (r>=zero) {
      //Quantity under Sqrt is real positiv
      r  = TMath::Sqrt(r);
      zz = r;
    }
    else {
      //Quantity under Sqrt is real negativ
      r  = TMath::Sqrt(-r);
      zz = r*I;
    }
  }
  else {
    //Quantity under Sqrt is complex
    zz = Sqrt(z-g);
  }
  if (!b) zz = zero - zz;
  return zz;
}
void TEqIndex::Set(Double_t a0,Double_t b0,Double_t c0,Double_t d0,
                   Double_t g0, Double_t gd) {
                     //new equation to solve
                     a=a0; b=b0; c=c0; d=d0; g=g0; fGd = gd;
                     Init();
}
void TEqIndex::SetAxis(Int_t n,Axis_t xl,Axis_t xu) {
  //Change axis values
  fNbin = n;
  fXlow = xl;
  fXup  = xu;
}
void TEqIndex::SetDebug() {
  //Set debug on
  fDebug = kTRUE;
  fResidus0 = new TH1F("Residus0","Residus of guess of method 0",
    50,-17.0,-2.0);
  fResidus1 = new TH1F("Residus1","Residus of guess of method 1",
    50,-17.0,-2.0);
}
void TEqIndex::ShowEqu(Int_t nbin,Double_t xlow,Double_t xup,Double_t ylow,Double_t yup) {
  // Show the shape of the absolute value of the equation (E1) for real z values
  //between xlow and xup, both with +Sqrt(z-g) and -Sqrt(z-g). If z>=g, the
  //equation y is real. If z<g, y is complex and |y| is drawn.
  // nbin is the number of points in the graph. [Default 1000]
  // If xlow>=xup [default] then suitable value for xlow and xup are calculated
  //by ShowEqu for the abscissa. Else xlow and xup are taken as given.
  // If ylow>=yup [default] then suitable value for ylow and yup are calculated
  //by ShowEqu for the ordinate. Else ylow and yup are taken as given.
  const Int_t    decim  = 3;
  const Double_t eps    = 1.0e-15;
  const Double_t big    = 1.0e+99;
  const Double_t dix    = 10.0;
  const Double_t margex = 0.5;
  const Double_t margey = 0.2;
  Int_t k,k1,k2;
  TString s1,s2;
  Double_t x,y1,y2,dx,dy,ymin,ymax,l1,sign;
  TSComplex zp,zm;
  TArrayD xa(nbin),yp(nbin),ym(nbin);
  s1 = "Equation  with +Sqrt(z-g) type: ";
  s2 = "Equation  with -Sqrt(z-g) type: ";
  switch (fTypeEq) {
    case bczero:
      s1.Append("bczero");
      s2.Append("bczero");
      break;
    case degenerate:
      s1.Append("degenerate");
      s2.Append("degenerate");
      break;
    case general:
      s1.Append("general");
      s2.Append("general");
      break;
  }
  if (xlow>=xup) {
    xlow = fZc[0].Re();
    xup  = xlow;
    for (k=1;k<4;k++) {
      x = fZc[k].Re();
      if (x<xlow) xlow = x;
      if (x>xup)  xup  = x;
    }
    dx    = (xup-xlow)*margex;
    xlow -= dx;
    xup  += dx;
    sign  = xlow/TMath::Abs(xlow);
    l1    = TMath::Log10(TMath::Abs(xlow));
    k1    = (Int_t)(-l1 + decim);
    dx    = TMath::Abs(xlow)*TMath::Exp(k1*TMath::Log(dix));
    k2    = (Int_t)dx;
    if (sign>0.0) dx = k2;
    else          dx = k2+1;
    xlow  = sign*dx*TMath::Exp(-k1*TMath::Log(dix));
    //
    sign  = xup/TMath::Abs(xup);
    l1    = TMath::Log10(TMath::Abs(xup));
    k1    = (Int_t)(-l1 + decim);
    dx    = TMath::Abs(xup)*TMath::Exp(k1*TMath::Log(dix));
    k2    = (Int_t)dx;
    if (sign>0.0) dx = k2+1;
    else          dx = k2;
    xup   = sign*dx*TMath::Exp(-k1*TMath::Log(dix));
  }
  dx = (xup-xlow)/nbin;
  x  = xlow + 0.5*dx;
  ymin =  big;
  ymax = -big;
  for (k=0;k<nbin;k++) {
    xa[k] = x;
    zp    = Y(x,kTRUE);
    zm    = Y(x,kFALSE);
    if (TMath::Abs(zp.Im())<eps) y1 = zp.Re();
    else                         y1 = Abs(zp);
    if (TMath::Abs(zm.Im())<eps) y2 = zm.Re();
    else                         y2 = Abs(zm);
    yp[k] = y1;
    ym[k] = y2;
    if (y1<ymin) ymin = y1;
    if (y1>ymax) ymax = y1;
    if (y2<ymin) ymin = y2;
    if (y2>ymax) ymax = y2;
    x += dx;
  }
  if (ymin>0.0) ymin = 0.0;
  dy    = (ymax-ymin)*margey;
  ymin -= dy;
  ymax += dy;
  sign = ymin/TMath::Abs(ymin);
  l1   = TMath::Log10(TMath::Abs(ymin));
  k1   = (Int_t)(-l1 + decim);
  dy   = TMath::Abs(ymin)*TMath::Exp(k1*TMath::Log(dix));
  k2   = (Int_t)dy;
  if (sign>0.0) dy = k2;
  else          dy = k2+1;
  ymin = sign*dy*TMath::Exp(-k1*TMath::Log(dix));
  //
  sign = ymax/TMath::Abs(ymax);
  l1   = TMath::Log10(TMath::Abs(ymax));
  k1   = (Int_t)(-l1 + decim);
  dy   = TMath::Abs(ymax)*TMath::Exp(k1*TMath::Log(dix));
  k2   = (Int_t)dy;
  if (sign>0.0) dy = k2+1;
  else          dy = k2;
  ymax  = sign*dy*TMath::Exp(-k1*TMath::Log(dix));
  if (!fGraphP) {
    fGraphP = new TGraph(nbin,xa.fArray,yp.fArray);
    fGraphP->SetName("Equation_E1+");
    fGraphP->SetTitle(s1.Data());
    if (ylow>=yup) {
      fGraphP->SetMinimum(ymin);
      fGraphP->SetMaximum(ymax);
    }
    else {
      fGraphP->SetMinimum(ylow);
      fGraphP->SetMaximum(yup);
    }
  }
  if (!fGraphM) {
    fGraphM = new TGraph(nbin,xa.fArray,ym.fArray);
    fGraphM->SetName("Equation_E1-");
    fGraphM->SetTitle(s2.Data());
    if (ylow>=yup) {
      fGraphM->SetMinimum(ymin);
      fGraphM->SetMaximum(ymax);
    }
    else {
      fGraphM->SetMinimum(ylow);
      fGraphM->SetMaximum(yup);
    }
  }
  if (!gLit) gLit = new TLitGlob(5);
  if (!gTwoPad) {
    gLit->BookCanvas();
  }
  fGraphP->Draw("AP");
  gTwoPad->ChangePad();
  fGraphM->Draw("AP");
}
void TEqIndex::ShowResidus() const {
  //Draws histograms fResidus0/1
  fResidus0->Draw();
  gTwoPad->ChangePad();
  fResidus1->Draw();
}
Double_t TEqIndex::Solve(Short_t ani, Bool_t OnlyOne) {
  //Finds the 3 families of 4 solutions and chooses among the 3 families the
  //best one. See comments about TSComplex& Solve(Int_t).
  //
  //  BE CAREFUL !!!
  //
  //It seems in fact that only the 1st family gives complex conjugate solutions.
  //In the future, we will have to simply take the 1st family each time. We keep
  //this choice among 3 families as long as we want to verify this.
  //
  const Double_t zero       = 0.0;
  const Double_t un         = 1.0;
  const Double_t deux       = 2.0;
  const Double_t WeightEqua = 1.0;  //weight for quality "Equation well solved"
  const Double_t WeightConj = 10.0; //more weight to the quality "complex conjugate"
  const Double_t small      = 1.0e-6;
  const Double_t limidentic = 1.0e-12;
  const Double_t big        = 1.0e+300;
  Bool_t ok;
  Double_t Q;
  Double_t Quality = -100.0;
  Short_t kmax;
  if (OnlyOne) kmax = 1;
  else kmax = 3;
  switch (fTypeEq) {
  case bczero:
    Quality = SolveSimple();
    break;
  case degenerate:
    ok = SolveDegen();
    if (ok) break;
    else fTypeEq = general;
  case general:
    Short_t k;
    Short_t kbest = 0;
    TSComplex w;
    k  = 0;
    w.Set(zero,zero);
    ok = Solve1();
    if (!ok) {
      for (k=0;k<kmax;k++) {
        w  = Solve(k);
        Q  = WeightEqua*w.Re() + WeightConj*w.Im();
        if (!k) {
          kbest = k;
          Quality = Q;
        }
        else {
          if (Q<Quality) {
            kbest = k;
            Quality = Q;
          }
        }
      }
      switch (kbest) {
  case 0:
    f0Chosen += un;
    break;
  case 1:
    f1Chosen += un;
    break;
  case 2:
    f2Chosen += un;
    break;
      }
    }
    fZ0  = fY0[kbest];
    fZ1  = fY1[kbest];
    fZ2  = fY2[kbest];
    fZ3  = fY3[kbest];
    fB0  = fC0[kbest];
    fB1  = fC1[kbest];
    fB2  = fC2[kbest];
    fB3  = fC3[kbest];
    fR0  = fS0[kbest];
    fR1  = fS1[kbest];
    fR2  = fS2[kbest];
    fR3  = fS3[kbest];
    fQs0 = fWs0[kbest];
    fQs1 = fWs1[kbest];
    fQs2 = fWs2[kbest];
    fQs3 = fWs3[kbest];
    fQj0 = fWj0[kbest];
    fQj1 = fWj1[kbest];
    fQj2 = fWj2[kbest];
    fQj3 = fWj3[kbest];
    //Make the solutions better complex conjugate
    Double_t y,y1,y2;
    if (fR0) {
      y1 = -fZ0.Im();
      y2 =  fZ1.Im();
      y  = (y1+y2)/deux;
      fZ0.SetIm(-y);
      fZ1.SetIm(y);
    }
    if (fR2) {
      y1 = -fZ2.Im();
      y2 =  fZ3.Im();
      y  = (y1+y2)/deux;
      fZ2.SetIm(-y);
      fZ3.SetIm(y);
    }
    break;
  }
  //Refuses that 2 solutions be identical
  Short_t i = -1;
  Short_t j = -1;
  Short_t jsave;
  Bool_t identical = kFALSE;
  TSComplex z1,z2;
  Bool_t   b1,b2;
  Double_t dd;
  while ((!identical) && (i<2)) {
    i += 1;
    j  = i;
    z1 = GetfZ(i);
    b1 = GetfB(i);
    while ((!identical) && (j<3)) {
      j += 1;
      z2 = GetfZ(j);
      b2 = GetfB(j);
      //  correction : was b1=b2 !
      if (b1==b2) {
        dd = Abs(z2-z1);
        if (dd<limidentic) {
          identical = kTRUE;
          jsave = j;
        }
      }
    }
  }
  //Calculates the cosines
  if (fR0) fD0 = Rac(fZ0,fB0)/Sqrt(fZ0);
  else fD0 = Rac(fZ0,fB0)/TMath::Sqrt(fZ0.Re());
  if (fR1) fD1 = Rac(fZ1,fB1)/Sqrt(fZ1);
  else fD1 = Rac(fZ1,fB1)/TMath::Sqrt(fZ1.Re());
  if (fR2) fD2 = Rac(fZ2,fB2)/Sqrt(fZ2);
  else fD2 = Rac(fZ2,fB2)/TMath::Sqrt(fZ2.Re());
  if (fR3) fD3 = Rac(fZ3,fB3)/Sqrt(fZ3);
  else fD3 = Rac(fZ3,fB3)/TMath::Sqrt(fZ3.Re());
  if (identical) {
    gMes->SetName("Solve");
    gMes->SetTitle("TEqIndex");
    gMes->M(Error_M,1,"these 2 solutions are identical",i,j);
    Quality = deux;
  }
  else Quality = fQs0 + fQs1 + fQs2 + fQs3;
  //In the case of "uniaxial negativ birefringence", if the 4 solutions are
  //real, then the 2 highest solutions must be equal.
  //In the case of "uniaxial positiv birefringence", if the 4 solutions are
  //real, then the 2 smallest solutions must be equal.
  //We verify that !
  Bool_t buni = kFALSE;
  switch (ani) {
  case 1:
  case 2:
    buni = (!fR0) && (!fR1) && (!fR2) && (!fR3);
    if (buni) PutIn();
    break;
  default:
    break;
  }
  if ((buni) && (fTypeEq == general)) {
    Double_t diff;
    Double_t amax1 = -un;
    Double_t amax2 = -un;
    Double_t amin1 = big;
    Double_t amin2 = big;
    Short_t k;
    Short_t i1 = 0;
    Short_t i2 = 0;
    switch (ani) {
    case 1:
      for (k=0;k<4;k++)
        if (fZc[k].Re()>amax1) {
          i1 = k;
          amax1 = fZc[k].Re();
        }
        for (k=0;k<4;k++)
          if ((k != i1) && (fZc[k].Re()>amax2)) {
            i2 = k;
            amax2 = fZc[k].Re();
          }
          diff = TMath::Abs(amax1-amax2);
          if (diff>small) {
            gMes->SetName("Solve");
            gMes->SetTitle("TEqIndex");
            gMes->M(Error_M,2,"Uniaxial neg. birefringence bad",diff);
          }
          else {
            diff = (amax1 + amax2)/deux;
            fZc[i1] = diff;
            fZc[i2] = diff;
          }
          PutOut();
          break;
    case 2:
      for (k=0;k<4;k++)
        if (fZc[k].Re()<amin1) {
          i1 = k;
          amin1 = fZc[k].Re();
        }
        for (k=0;k<4;k++)
          if ((k != i1) && (fZc[k].Re()<amin2)) {
            i2 = k;
            amin2 = fZc[k].Re();
          }
          diff = TMath::Abs(amin1-amin2);
          if (diff>small) {
            gMes->SetName("Solve");
            gMes->SetTitle("TEqIndex");
            gMes->M(Error_M,3,"Uniaxial pos. birefringence bad",diff);
          }
          else {
            diff = (amin1 + amin2)/deux;
            fZc[i1] = diff;
            fZc[i2] = diff;
          }
          break;
    default:
      break;
    }//end switch (ani)
  }//end if (buni)
  fSolved = kTRUE;
  return Quality;
}
TSComplex TEqIndex::Solve(Int_t k) {
  //Finds the solution by brute force, taking the formula given by Mathematica.
  //Notice that at some time in these solution, the cubic root of a complex
  //number (variable M) has to be taken. As there are 3 solutions for the cubic
  //root of a complex number, this gives rise to 3 possibilities of 4 solutions.
  //This is why Solve is called 3 times, with parameter k = 1,2,3. At the end
  //we have to choose the best among these 3 lot of solution. We have at hand
  //2 criteria to decide of the quality of the solution :
  //  (1) - how well the equation is solved. This 1st criterium is returned in
  //          the real part of the returned complex number !
  //  (2) - are the 4 solutions groupable in pair of complex conjugate complex
  //          numbers ? They have to ! It can be shown that if z is solution of
  //          the equation, the complex conjugate of z is also solution of the
  //          equation. This is because a,b,c,d,g are real. How well solutions
  //          are complex conjugate is returned in the imaginary part of the
  //          returned complex number !
  //It may happen that the solutions found by Solve are very bad. This is because
  //equation (E1) behaves often very badly. For instance, it happens often that
  //variable F be infinitesimal (~1.0e-7), although it is calculated from the
  //variables A,B,C,D which have the order of magnitude 1.0e+2 . So we face the
  //problem of "decimal extinctions".
  //
  const Double_t k0  = 0.0;
  const Double_t k1  = 1.0;
  const Double_t k2  = 2.0;
  const Double_t k3  = 3.0;
  const Double_t k4  = 4.0;
  const Double_t k8  = 8.0;
  const Double_t k9  = 9.0;
  const Double_t k12 = 12.0;
  const Double_t k27 = 27.0;
  const Double_t k72 = 72.0;
  const Double_t k03 = k1/k3;
  const Double_t precision  = 1.0e-12;
  const Double_t limreal1 = 1.0e-6;
  const Double_t limreal2 = 5.0e-7;
  const Double_t gok = 1.0e-7;
  Short_t nIdentical;
  Bool_t  zsame[4];
  Short_t Ntimes = 0;
  Short_t nBad;
  Bool_t  zbad[4];
  Short_t Ntimesb = 0;
  Double_t dps1,dps2,dps3,dps4;
  TSComplex zz1,zz2,hbb,hcc,hdd;
  Bool_t bb1,bb2,zchanged;
  Short_t is,isok;
  TSComplex I(k0,k1);
  Double_t eps = k1;
  Double_t dps,df0,df0s,df0min;
  TSComplex zz,f0,zeps;
  TSComplex eps1,eps2,eps3,eps4;
  TSComplex eps5,eps6,eps7,eps8;
  Double_t kk;
  Double_t x,y,x1,x2,x3;
  TSComplex w,w1,w2,w3;
  Double_t a2,b2,c2,d2;
  Double_t A,B,C,D,F,G,H,P;
  Double_t B2,B3,C2,C3,D2,F3,G2;
  TSComplex z,M,M1s3,N,SN;

  InifZ();
  kk = TMath::Exp(k03*TMath::Log(k2));
  a2 = a*a;
  b2 = b*b;
  c2 = c*c;
  d2 = d*d;

  A  = d2 + c2*g;
  B  = k2*a - b2;
  C  = a2 - k2*b*c + k2*d + b2*g;
  D  = -c2 + k2*a*d + k2*b*c*g;
  B2 = B*B;
  B3 = B*B2;
  C2 = C*C;
  C3 = C*C2;
  D2 = D*D;
  F  = k12*A + C2 - k3*B*D;
  F3 = F*F*F;
  G  = k27*A*B2 - k72*A*C + k2*C3 - k9*B*C*D + k27*D2;
  G2 = G*G;
  H  = -k4*F3 + G2;
  if (H>=k0) M = G + TMath::Sqrt(H);
  else       M = G + I*TMath::Sqrt(-H);
  M1s3= Sqrt3(M,k);
  P  = -B3 + k4*B*C - k8*D;

  //x1 1st term of zi : real     +B/k4
  x1 = B/k4;
  //x2 3rd term of zi : real     +B2/k2 - (k4*C)/k3
  x2 =  B2/k2 - (k4*C)/k3;
  w2 =  M1s3/(k3*kk);
  x  =  (kk*F)/k3;
  w.Set(k1,k0);
  z  =  w/M1s3;
  //w1 4th term of zi : complex  +(kk*F)/(k3*M1s3)
  w1 =  x*z;
  x  =  k1/(k3*kk);
  //w2 5th term of zi : complex  +M1s3/(k3*kk)
  w2 =  x*M1s3;
  //x3 1th term of N  : real
  x3 = B2/k4 - (k2*C)/k3;
  N  = x3 + w1 + w2;
  SN = Sqrt(N);
  x  = P/k4;
  z  = w/SN;
  //w3 6th term of zi : complex  +P/(k4*SN)
  w3 = x*z;

  z  = Sqrt(x2 - w1 - w2 - w3);
  fZ0 = -x1 - (SN + z)/k2;
  fZ1 = -x1 - (SN - z)/k2;
  w  = Sqrt(x2 - w1 - w2 + w3);
  fZ2 = -x1 + (SN - w)/k2;
  fZ3 = -x1 + (SN + w)/k2;
  //
  //Verifications
  //
  Double_t epsn;
  //
  // fZ0
  //
  fRac = Rac(fZ0,kTRUE);
  eps1 = F0(fZ0);
  fRac.SetNeg();
  eps5 = F0(fZ0);
  x    = Abs(eps1);
  y    = Abs(eps5);
  if (x<=y) {
    eps  = x;
    fQs0 = x;
    fB0  = kTRUE;
  }
  else {
    eps  = y;
    fQs0 = y;
    fB0  = kFALSE;
  }
  //
  // fZ1
  //
  fRac = Rac(fZ1,kTRUE);
  eps2 = F0(fZ1);
  fRac.SetNeg();
  eps6 = F0(fZ1);
  x    = Abs(eps2);
  y    = Abs(eps6);
  if (x<=y) {
    epsn = x;
    fQs1 = x;
    fB1  = kTRUE;
  }
  else {
    epsn = y;
    fQs1 = y;
    fB1  = kFALSE;
  }
  if (epsn>eps) eps = epsn;
  //
  // fZ2
  //
  fRac = Rac(fZ2,kTRUE);
  eps3 = F0(fZ2);
  fRac.SetNeg();
  eps7 = F0(fZ2);
  x    = Abs(eps3);
  y    = Abs(eps7);
  if (x<=y) {
    epsn = x;
    fQs2 = x;
    fB2  = kTRUE;
  }
  else {
    epsn = y;
    fQs2 = y;
    fB2  = kFALSE;
  }
  if (epsn>eps) eps = epsn;
  //
  // fZ3
  //
  fRac = Rac(fZ3,kTRUE);
  eps4 = F0(fZ3);
  fRac.SetNeg();
  eps8 = F0(fZ3);
  x    = Abs(eps4);
  y    = Abs(eps8);
  if (x<=y) {
    epsn = x;
    fQs3 = x;
    fB3  = kTRUE;
  }
  else {
    epsn = y;
    fQs3 = y;
    fB3  = kFALSE;
  }
  if (fDebug) {
    FillResidu(fQs0,kFALSE);
    FillResidu(fQs1,kFALSE);
    FillResidu(fQs2,kFALSE);
    FillResidu(fQs3,kFALSE);
  }
  if (epsn>eps) eps = epsn;
  PutIn();
  //
  //Look 1st for the special case where z = g is a solution. A solution at this
  //place is absolutly not manageable by SuperNewton because all derivatives
  //are infinite !
  //
  zz   = g;
  fRac = Rac(zz,kTRUE);
  f0   = F0(zz);
  df0  = Abs(f0);
  df0s = df0;
  if (df0<gok) {
    //Here z = g is a good solution. We find which among the 4 found solution is
    //closest to g
    isok   = 0;
    zz     = fZc[0] - g;
    df0min = Abs(zz);
    for (is=1;is<4;is++) {
      zz   = fZc[is] - g;
      df0  = Abs(zz);
      if (df0<df0min) {
        isok   = is;
        df0min = df0;
      }
    }
    fZc[isok]  = g;
    fBc[isok]  = kTRUE;
    fRc[isok]  = kFALSE;
    fGok[isok] = kTRUE;
    fQsc[isok] = df0s;
    fQjc[isok] = k0;
  }
  //
  //Ordering
  //
bigloop:
  Order1();
  WhichIsReal(limreal1,kTRUE);
  //Now we look how well the solutions are complex conjugate, if complex !
  dps = Order2();
  //
  //Solutions which are "almost real" are made totally real
  //and solution is made better by Super Newton when poor.
  //
  for (is=0;is<4;is++)
    if ((!fGok[is]) && (fQsc[is]>precision)) Better(fZc[is],fBc[is],fQsc[is]);
  //We consider as suspect the case where 2 solutions are identical
  nIdentical = Identical(kFALSE,zchanged,zsame);
  //We have to order again because Better() may have change things.
  Order1();
  WhichIsReal(limreal2,kFALSE);
  epsn = Order2();
  zeps.Set(eps,dps);
  Order3();
  nIdentical = Identical(kTRUE,zchanged,zsame);
  if (zchanged) nIdentical = Identical(kTRUE,zchanged,zsame);
  nIdentical = IdenticalF(zsame);
  Short_t kktimes = 0;
  switch (nIdentical) {
  case 0:
    break;
  case 1:
    Ntimes += 1;
    zz1 = -fA;
    for (is=0;is<4;is++)
      if (!zsame[is]) zz1 = zz1 - fZc[is];
    fRac = Rac(zz1,kTRUE);
    dps1 = Abs(F0(zz1));
    fRac.SetNeg();
    dps2 = Abs(F0(zz1));
    bb1  = (dps1<=dps2);
    for (is=0;is<4;is++)
      if (zsame[is]) {
        fZc[is] = zz1;
        fBc[is] = bb1;
        if (bb1) fQsc[is] = dps1;
        else     fQsc[is] = dps2;
      }
      if (Ntimes==1) goto bigloop;
      break;
  case 2:
    Ntimes += 1;
    hbb = fA;
    hcc = fD;
    for (is=0;is<4;is++)
      if (!zsame[is]) {
        hbb = hbb + fZc[is];
        hcc = hcc/fZc[is];
      }
      hdd  = Sqrt(hbb*hbb - k4*hcc);
      zz1  = (hdd -hbb)/k2;
      zz2  = (k0 -hbb - hdd)/k2;
      fRac = Rac(zz1,kTRUE);
      dps1 = Abs(F0(zz1));
      fRac.SetNeg();
      dps2 = Abs(F0(zz1));
      bb1  = (dps1<=dps2);
      fRac = Rac(zz2,kTRUE);
      dps3 = Abs(F0(zz2));
      fRac.SetNeg();
      dps4 = Abs(F0(zz2));
      bb2  = (dps3<=dps4);
      for (is=0;is<4;is++)
        if (zsame[is]) {
          kktimes += 1;
          if (kktimes == 1) {
            fZc[is] = zz1;
            fBc[is] = bb1;
            if (bb1) fQsc[is] = dps1;
            else     fQsc[is] = dps2;
          }
          else {
            fZc[is] = zz2;
            fBc[is] = bb2;
            if (bb2) fQsc[is] = dps3;
            else     fQsc[is] = dps4;
          }
        }
        if (Ntimes==1) goto bigloop;
        break;
  default:
    gMes->SetName("Solve");
    gMes->SetTitle("TEqIndex");
    gMes->M(Error_M,1,"3 solutions are identical !");
    break;
  }
  //How many bad solutions ?
  nBad = 0;
  for (is=0;is<4;is++) {
    zbad[is] = fQsc[is]>gok;
    if (zbad[is]) nBad += 1;
  }
  kktimes = 0;
  switch (nBad) {
  case 0:
    break;
  case 1:
    Ntimesb += 1;
    zz1 = -fA;
    for (is=0;is<4;is++)
      if (!zbad[is]) zz1 = zz1 - fZc[is];
    fRac = Rac(zz1,kTRUE);
    dps1 = Abs(F0(zz1));
    fRac.SetNeg();
    dps2 = Abs(F0(zz1));
    bb1  = (dps1<=dps2);
    for (is=0;is<4;is++)
      if (zbad[is]) {
        fZc[is] = zz1;
        fBc[is] = bb1;
        if (bb1) fQsc[is] = dps1;
        else     fQsc[is] = dps2;
      }
      if (Ntimesb==1) goto bigloop;
      break;
  case 2:
    Ntimesb += 1;
    hbb = fA;
    hcc = fD;
    for (is=0;is<4;is++)
      if (!zbad[is]) {
        hbb = hbb + fZc[is];
        hcc = hcc/fZc[is];
      }
      hdd  = Sqrt(hbb*hbb - k4*hcc);
      zz1  = (hdd -hbb)/k2;
      zz2  = (k0 -hbb - hdd)/k2;
      fRac = Rac(zz1,kTRUE);
      dps1 = Abs(F0(zz1));
      fRac.SetNeg();
      dps2 = Abs(F0(zz1));
      bb1  = (dps1<=dps2);
      fRac = Rac(zz2,kTRUE);
      dps3 = Abs(F0(zz2));
      fRac.SetNeg();
      dps4 = Abs(F0(zz2));
      bb2  = (dps3<=dps4);
      for (is=0;is<4;is++)
        if (zbad[is]) {
          kktimes += 1;
          if (kktimes == 1) {
            fZc[is] = zz1;
            fBc[is] = bb1;
            if (bb1) fQsc[is] = dps1;
            else     fQsc[is] = dps2;
          }
          else {
            fZc[is] = zz2;
            fBc[is] = bb2;
            if (bb2) fQsc[is] = dps3;
            else     fQsc[is] = dps4;
          }
        }
        if (Ntimesb==1) goto bigloop;
        break;
  default:
    gMes->SetName("Solve");
    gMes->SetTitle("TEqIndex");
    gMes->M(Error_M,2,"Too many bad solutions");
    break;
  }
  fY0[k]  = fZc[0];
  fY1[k]  = fZc[1];
  fY2[k]  = fZc[2];
  fY3[k]  = fZc[3];
  fC0[k]  = fBc[0];
  fC1[k]  = fBc[1];
  fC2[k]  = fBc[2];
  fC3[k]  = fBc[3];
  fS0[k]  = fRc[0];
  fS1[k]  = fRc[1];
  fS2[k]  = fRc[2];
  fS3[k]  = fRc[3];
  fWs0[k] = fQsc[0];
  fWs1[k] = fQsc[1];
  fWs2[k] = fQsc[2];
  fWs3[k] = fQsc[3];
  fWj0[k] = fQjc[0];
  fWj1[k] = fQjc[1];
  fWj2[k] = fQjc[2];
  fWj3[k] = fQjc[3];
  return zeps;
}
Bool_t TEqIndex::Solve1() {
  //Solution of z^2  + a*z + (b*z+c)*Sqrt(z-g) + d == 0 by mean of a change
  //of variable y = Sqrt(z-g) and call to the class TPol4.
  const Double_t zero = 0.0;
  const Double_t deux = 2.0;
  const Double_t k4   = 4.0;
  const Double_t limreal2 = 1.0e-7;
  const Double_t precision  = 1.0e-12;
  const Double_t gok = 1.0e-7;
  Bool_t bb,zchanged;
  Bool_t  zsame[4];
  Short_t nBad;
  Bool_t  zbad[4];
  Short_t nIdentical;
  Short_t is,isok;
  Double_t aa3,aa2,aa1,aa0,eps1,eps2,epsn;
  Double_t dps;
  TSComplex z0,z1,z2,z3;
  TSComplex zz1,zz2,hbb,hcc,hdd;
  Double_t dps1,dps2,dps3,dps4;
  Bool_t b0,b1,b2,b3,bb1,bb2;
  Short_t Ntimes = 0;
  Short_t Ntimesb = 0;
  aa3 = b;
  aa2 = a + deux*g;
  aa1 = b*g + c;
  aa0 = d + a*g + g*g;
  fP4->Set(aa3,aa2,aa1,aa0);
  bb = fP4->Solve(eps1);
  if (!bb) return kFALSE;
  bb = fP4->Get(z0,z1,z2,z3,b0,b1,b2,b3);
  if (!bb) return kFALSE;
  fZ0  = z0*z0 + g;
  fZ1  = z1*z1 + g;
  fZ2  = z2*z2 + g;
  fZ3  = z3*z3 + g;
  fR0  = fZ0.IsComplex(limreal2);
  fR1  = fZ1.IsComplex(limreal2);
  if (fR0 && !fR1) {
    fZ0.PureReal();
    fR0 = kFALSE;
  }
  if (!fR0 && fR1) {
    fZ1.PureReal();
    fR1 = kFALSE;
  }
  if (fR0 && fR1) fZ0.BetterConj(fZ1);
  fR2  = fZ2.IsComplex(limreal2);
  fR3  = fZ3.IsComplex(limreal2);
  if (fR2 && !fR3) {
    fZ2.PureReal();
    fR2 = kFALSE;
  }
  if (!fR2 && fR3) {
    fZ3.PureReal();
    fR3 = kFALSE;
  }
  if (fR2 && fR3) fZ2.BetterConj(fZ3);
  fRac = Rac(fZ0,kTRUE);
  eps1 = Abs(F0(fZ0));
  fRac.SetNeg();
  eps2 = Abs(F0(fZ0));
  if (eps1<=eps2) {
    fB0 = kTRUE;
    fQs0 = eps1;
  }
  else {
    fB0 = kFALSE;
    fQs0 = eps2;
  }
  fRac = Rac(fZ1,kTRUE);
  eps1 = Abs(F0(fZ1));
  fRac.SetNeg();
  eps2 = Abs(F0(fZ1));
  if (eps1<=eps2) {
    fB1 = kTRUE;
    fQs1 = eps1;
  }
  else {
    fB1 = kFALSE;
    fQs1 = eps2;
  }
  fRac = Rac(fZ2,kTRUE);
  eps1 = Abs(F0(fZ2));
  fRac.SetNeg();
  eps2 = Abs(F0(fZ2));
  if (eps1<=eps2) {
    fB2 = kTRUE;
    fQs2 = eps1;
  }
  else {
    fB2 = kFALSE;
    fQs2 = eps2;
  }
  fRac = Rac(fZ3,kTRUE);
  eps1 = Abs(F0(fZ3));
  fRac.SetNeg();
  eps2 = Abs(F0(fZ3));
  if (eps1<=eps2) {
    fB3 = kTRUE;
    fQs3 = eps1;
  }
  else {
    fB3 = kFALSE;
    fQs3 = eps2;
  }
  if (fDebug) {
    const Double_t limbad = 1.0e-5;
    eps1 = fQs0;
    FillResidu(fQs0,kTRUE);
    if (fQs1>eps1) eps1 = fQs1;
    FillResidu(fQs1,kTRUE);
    if (fQs2>eps1) eps1 = fQs2;
    FillResidu(fQs2,kTRUE);
    if (fQs3>eps1) eps1 = fQs3;
    FillResidu(fQs3,kTRUE);
    if (eps1>limbad) {
      bb = kFALSE;
      return bb;
    }
  }
  fQj0 = zero;
  fQj1 = zero;
  fQj2 = zero;
  fQj3 = zero;
  PutIn();
  //
  //Look 1st for the special case where z = g is a solution. A solution at
  //this place is absolutly not manageable by SuperNewton because all
  //derivatives are infinite !
  //
  TSComplex zz,f0;
  Double_t df0,df0s,df0min;
  zz   = g;
  fRac = Rac(zz,kTRUE);
  f0   = F0(zz);
  df0  = Abs(f0);
  df0s = df0;
  if (df0<gok) {
    //Here z = g is a good solution. We find which among the 4 found solution is
    //closest to g
    isok   = 0;
    zz     = fZc[0] - g;
    df0min = Abs(zz);
    for (is=1;is<4;is++) {
      zz   = fZc[is] - g;
      df0  = Abs(zz);
      if (df0<df0min) {
        isok   = is;
        df0min = df0;
      }
    }
    fZc[isok]  = g;
    fBc[isok]  = kTRUE;
    fRc[isok]  = kFALSE;
    fGok[isok] = kTRUE;
    fQsc[isok] = df0s;
    fQjc[isok] = zero;
  }
  //
  //Ordering
  //
  fNr = 4;
  for (is=0;is<4;is++) if (fRc[is]) fNr--;
bigloop:
  if (fRc[0] || fRc[2]) Order1();
  if ((Ntimes) || (Ntimesb)) {
    WhichIsReal(limreal2,kTRUE);
    //Now we look how well the solutions are complex conjugate, if complex !
    dps = Order2();
  }
  //
  //Solutions which are "almost real" are made totally real
  //and solution is made better by Super Newton when poor.
  //
  for (is=0;is<4;is++)
    if ((!fGok[is]) && (fQsc[is]>precision)) Better(fZc[is],fBc[is],fQsc[is]);
  //We consider as suspect the case where 2 solutions are identical
  nIdentical = Identical(kFALSE,zchanged,zsame);
  //We have to order again because Better() may have change things.
  Order1();
  WhichIsReal(limreal2,kFALSE);
  epsn = Order2();
  Order3();
  nIdentical = Identical(kTRUE,zchanged,zsame);
  if (zchanged) nIdentical = Identical(kTRUE,zchanged,zsame);
  nIdentical = IdenticalF(zsame);
  Short_t kktimes = 0;
  switch (nIdentical) {
  case 0:
    break;
  case 1:
    Ntimes += 1;
    zz1 = -fA;
    for (is=0;is<4;is++)
      if (!zsame[is]) zz1 = zz1 - fZc[is];
    fRac = Rac(zz1,kTRUE);
    dps1 = Abs(F0(zz1));
    fRac.SetNeg();
    dps2 = Abs(F0(zz1));
    bb1  = (dps1<=dps2);
    for (is=0;is<4;is++)
      if (zsame[is]) {
        fZc[is] = zz1;
        fBc[is] = bb1;
        if (bb1) fQsc[is] = dps1;
        else     fQsc[is] = dps2;
      }
      if (Ntimes==1) goto bigloop;
      break;
  case 2:
    Ntimes += 1;
    hbb = fA;
    hcc = fD;
    for (is=0;is<4;is++)
      if (!zsame[is]) {
        hbb = hbb + fZc[is];
        hcc = hcc/fZc[is];
      }
      hdd  = Sqrt(hbb*hbb - k4*hcc);
      zz1  = (hdd -hbb)/deux;
      zz2  = (zero -hbb - hdd)/deux;
      fRac = Rac(zz1,kTRUE);
      dps1 = Abs(F0(zz1));
      fRac.SetNeg();
      dps2 = Abs(F0(zz1));
      bb1  = (dps1<=dps2);
      fRac = Rac(zz2,kTRUE);
      dps3 = Abs(F0(zz2));
      fRac.SetNeg();
      dps4 = Abs(F0(zz2));
      bb2  = (dps3<=dps4);
      for (is=0;is<4;is++)
        if (zsame[is]) {
          kktimes += 1;
          if (kktimes == 1) {
            fZc[is] = zz1;
            fBc[is] = bb1;
            if (bb1) fQsc[is] = dps1;
            else     fQsc[is] = dps2;
          }
          else {
            fZc[is] = zz2;
            fBc[is] = bb2;
            if (bb2) fQsc[is] = dps3;
            else     fQsc[is] = dps4;
          }
        }
        if (Ntimes==1) goto bigloop;
        break;
  default:
    gMes->SetName("Solve");
    gMes->SetTitle("TEqIndex");
    gMes->M(Error_M,1,"3 solutions are identical !");
    break;
  }
  //How many bad solutions ?
  nBad = 0;
  for (is=0;is<4;is++) {
    zbad[is] = fQsc[is]>gok;
    if (zbad[is]) nBad += 1;
  }
  kktimes = 0;
  switch (nBad) {
  case 0:
    break;
  case 1:
    Ntimesb += 1;
    zz1 = -fA;
    for (is=0;is<4;is++)
      if (!zbad[is]) zz1 = zz1 - fZc[is];
    fRac = Rac(zz1,kTRUE);
    dps1 = Abs(F0(zz1));
    fRac.SetNeg();
    dps2 = Abs(F0(zz1));
    bb1  = (dps1<=dps2);
    for (is=0;is<4;is++)
      if (zbad[is]) {
        fZc[is] = zz1;
        fBc[is] = bb1;
        if (bb1) fQsc[is] = dps1;
        else     fQsc[is] = dps2;
      }
      if (Ntimesb==1) goto bigloop;
      break;
  case 2:
    Ntimesb += 1;
    hbb = fA;
    hcc = fD;
    for (is=0;is<4;is++)
      if (!zbad[is]) {
        hbb = hbb + fZc[is];
        hcc = hcc/fZc[is];
      }
      hdd  = Sqrt(hbb*hbb - k4*hcc);
      zz1  = (hdd -hbb)/deux;
      zz2  = (zero -hbb - hdd)/deux;
      fRac = Rac(zz1,kTRUE);
      dps1 = Abs(F0(zz1));
      fRac.SetNeg();
      dps2 = Abs(F0(zz1));
      bb1  = (dps1<=dps2);
      fRac = Rac(zz2,kTRUE);
      dps3 = Abs(F0(zz2));
      fRac.SetNeg();
      dps4 = Abs(F0(zz2));
      bb2  = (dps3<=dps4);
      for (is=0;is<4;is++)
        if (zbad[is]) {
          kktimes += 1;
          if (kktimes == 1) {
            fZc[is] = zz1;
            fBc[is] = bb1;
            if (bb1) fQsc[is] = dps1;
            else     fQsc[is] = dps2;
          }
          else {
            fZc[is] = zz2;
            fBc[is] = bb2;
            if (bb2) fQsc[is] = dps3;
            else     fQsc[is] = dps4;
          }
        }
        if (Ntimesb==1) goto bigloop;
        break;
  default:
    gMes->SetName("Solve");
    gMes->SetTitle("TEqIndex");
    gMes->M(Error_M,2,"Too many bad solutions");
    break;
  }
  fY0[0]  = fZc[0];
  fY1[0]  = fZc[1];
  fY2[0]  = fZc[2];
  fY3[0]  = fZc[3];
  fC0[0]  = fBc[0];
  fC1[0]  = fBc[1];
  fC2[0]  = fBc[2];
  fC3[0]  = fBc[3];
  fS0[0]  = fRc[0];
  fS1[0]  = fRc[1];
  fS2[0]  = fRc[2];
  fS3[0]  = fRc[3];
  fWs0[0] = fQsc[0];
  fWs1[0] = fQsc[1];
  fWs2[0] = fQsc[2];
  fWs3[0] = fQsc[3];
  fWj0[0] = fQjc[0];
  fWj1[0] = fQjc[1];
  fWj2[0] = fQjc[2];
  fWj3[0] = fQjc[3];
  return bb;
}
Bool_t TEqIndex::SolveDegen() {
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  const Double_t vsmall = 1.0e-6;
  TSComplex I(zero,un);
  TSComplex rz2,rz3;
  Bool_t ok = kTRUE;
  Double_t x0,x2,x3,y1,r1,r2,r3,bb,dd,eps1,eps2,eps3,eps4;
  Double_t x0a,x0b,x0c,w0a,w0b,w0c,ddd;
  ddd  = a*a - quatre*d;
  ddd  = TMath::Sqrt(TMath::Abs(ddd));
  x0a  = (-a + ddd)/deux;
  x0b  = (-a - ddd)/deux;
  x0c  = -c/b;
  eps1 = TMath::Abs(x0a-x0c);
  eps2 = TMath::Abs(x0b-x0c);
  if (eps1<=eps2) {
    eps3 = (un - a/ddd)/deux;
    eps4 = un/ddd;
    w0a  = un/(eps3*eps3 + eps4*eps4);
    w0c  = un/(x0c*x0c*(un/(c*c) + un/(b*b)));
    eps3 = w0a + w0c;
    w0a  = w0a/eps3;
    w0c  = w0c/eps3;
    x0   = w0a*x0a + w0c*x0c;
    y1   = x0b;
  }
  else {
    eps3 = (un + a/ddd)/deux;
    eps4 = un/ddd;
    w0b  = un/(eps3*eps3 + eps4*eps4);
    w0c  = un/(x0c*x0c*(un/(c*c) + un/(b*b)));
    eps3 = w0b + w0c;
    w0b  = w0b/eps3;
    w0c  = w0c/eps3;
    x0   = w0b*x0b + w0c*x0c;
    y1   = x0a;
  }
  r1   = TMath::Sqrt(TMath::Abs(x0-g));
  eps1 = TMath::Abs(x0*x0 + a*x0 + d + r1*(b*x0+c));
  eps2 = TMath::Abs(x0*x0 + a*x0 + d - r1*(b*x0+c));
  fZ0  = x0;
  fB0  = kTRUE;
  fR0  = kFALSE;
  fQs0 = eps1;
  fZ3  = x0;
  fB3  = kFALSE;
  fR3  = kFALSE;
  fQs3 = eps2;
  r1   = b*b;
  bb   = - r1 - deux*y1;
  dd   = r1*(r1 + quatre*(y1-g));
  if (dd>=zero) {
    fR1 = kFALSE;
    fR2 = kFALSE;
    dd  = TMath::Sqrt(dd);
    x2  = (-bb+dd)/deux;
    fZ1 = x2;
    r2  = x2 - g;
    if (r2>=zero) r2   = TMath::Sqrt(r2);
    else {
      gMes->SetName("SolveDegen");
      gMes->SetTitle("TEqIndex");
      gMes->M(Error_M,1,"No solution for x2");
      ok = kFALSE;
      r2 = zero;
    }
    eps1 = TMath::Abs(x2-y1 + b*r2);
    eps2 = TMath::Abs(x2-y1 - b*r2);
    if (eps1<=eps2) {
      fB1  = kTRUE;
      fQs1 = eps1;
    }
    else {
      fB1  = kFALSE;
      fQs1 = eps2;
    }
    x3  = (-bb-dd)/deux;
    fZ2 = x3;
    r3  = x3 - g;
    if (r3>=zero) r3   = TMath::Sqrt(r3);
    else {
      gMes->SetName("SolveDegen");
      gMes->SetTitle("TEqIndex");
      gMes->M(Error_M,2,"No solution for x3");
      ok = kFALSE;
      r3 = zero;
    }
    eps3 = TMath::Abs(x3-y1 + b*r3);
    eps4 = TMath::Abs(x3-y1 - b*r3);
    if (eps3<=eps4) {
      fB2  = kTRUE;
      fQs2 = eps3;
    }
    else {
      fB2  = kFALSE;
      fQs2 = eps4;
    }
    if (dd<vsmall) {
      if (fB1 && fB2) {
        fB2  = kFALSE;
        fQs2 = eps4;
      }
      if ((!fB1) && (!fB2)) {
        fB1  = kTRUE;
        fQs1 = eps1;
      }
    }
  }
  else {
    fR1  = kTRUE;
    fR2  = kTRUE;
    dd   = TMath::Sqrt(-dd);
    fZ1  = (-bb + I*dd)/deux;
    rz2  = Sqrt(fZ1-g);
    eps1 = Abs(fZ1*fZ1 + a*fZ1 + d + rz2*(b*fZ1+c));
    eps2 = Abs(fZ1*fZ1 + a*fZ1 + d - rz2*(b*fZ1+c));
    if (eps1<=eps2) {
      fB1  = kTRUE;
      fQs1 = eps1;
    }
    else {
      fB1  = kFALSE;
      fQs1 = eps2;
    }
    fZ2  = (-bb - I*dd)/deux;
    rz3  = Sqrt(fZ2-g);
    eps3 = Abs(fZ2*fZ2 + a*fZ2 + d + rz3*(b*fZ2+c));
    eps4 = Abs(fZ2*fZ2 + a*fZ2 + d - rz3*(b*fZ2+c));
    if (eps3<=eps4) {
      fB2  = kTRUE;
      fQs2 = eps3;
    }
    else {
      fB2  = kFALSE;
      fQs2 = eps4;
    }
    if (dd<vsmall) {
      if (fB1 && fB2) {
        fB2  = kFALSE;
        fQs2 = eps4;
      }
      if ((!fB1) && (!fB2)) {
        fB1  = kTRUE;
        fQs1 = eps1;
      }
    }
    //Solution which are almost real are made real
    if (dd<vsmall) {
      fR1 = kFALSE;
      fR2 = kFALSE;
      fZ1.PureReal();
      fZ2.PureReal();
    }
  }
  if ((fB2) && (!fB1)) {
    TSComplex zz4;
    Bool_t b4,r4;
    Double_t q4;
    zz4  = fZ1;
    b4   = fB1;
    r4   = fR1;
    q4   = fQs1;
    fZ1  = fZ2;
    fB1  = fB2;
    fR1  = fR2;
    fQs1 = fQs2;
    fZ2  = zz4;
    fB2  = b4;
    fR2  = r4;
    fQs2 = q4;
  }
  fQj0 = zero;
  fQj1 = zero;
  fQj2 = zero;
  fQj3 = zero;
  return ok;
}
Double_t TEqIndex::SolveSimple() {
  //Idem Solve in case b == c == 0 !
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  TSComplex I(zero,un);
  Bool_t cplx = kFALSE;
  TSComplex cdd;
  Double_t dd;
  dd = fGd;
  if (dd>=zero) {
    dd  = TMath::Sqrt(dd);
    cdd = dd;
  }
  else {
    cplx = kTRUE;
    dd  = TMath::Sqrt(-dd);
    cdd = I*dd;
  }
  fZ0  = (-a + cdd)/deux;
  fZ1  = (-a - cdd)/deux;
  fZ2  = fZ0;
  fZ3  = fZ1;
  fB0  = kTRUE;
  fB1  = kTRUE;
  fB2  = kFALSE;
  fB3  = kFALSE;
  fR0  = cplx;
  fR1  = cplx;
  fR2  = cplx;
  fR3  = cplx;
  fQs0 = fEnongen;
  fQs1 = fEnongen;
  fQs2 = fEnongen;
  fQs3 = fEnongen;
  fQj0 = zero;
  fQj1 = zero;
  fQj2 = zero;
  fQj3 = zero;
  return fEnongen;
}
void TEqIndex::StepSN(TSComplex &z,Bool_t bs,Double_t smooth) {
  //Step in Super Newton method
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  const Double_t deux = 2.0;
  Double_t x,y;
  Double_t a11,a12,a22,b1,b2,dd;
  TSComplex z1,z2,z3,z4,z5;
  TSComplex za,zz,f0,f1,f2,g0,g1,g2;
  TSComplex I(zero,un);
  za = z;
  fRac = Rac(za,bs);
  f0 = F0(za);
  f1 = Fp(za);
  f2 = Fs(za);
  g0 = f0;
  g0.C();
  g1 = f1;
  g1.C();
  g2 = f2;
  g2.C();
  z1  = g0*f2;
  z2  = g2*f0;
  z3  = deux*g1*f1;
  z4  = g1*f0;
  z5  = g0*f1;
  zz  = z3 + z1 + z2;
  a11 = zz.Re();
  zz  = I*(z1 - z2);
  a12 = zz.Re();
  zz  = z3 - z1 - z2;
  a22 = zz.Re();
  zz  = zero - z4 - z5;
  b1  = zz.Re();
  zz  = I*(z4 - z5);
  b2  = zz.Re();
  dd  = a11*a22 - a12*a12;
  x   = (b1*a22 - b2*a12)/dd;
  y   = (b2*a11 - b1*a12)/dd;
  x   = x*smooth;
  y   = y*smooth;
  zz.Set(x,y);
  z   = zz + z;
}
void TEqIndex::StepSN4(TSComplex &z) const {
  //Step in Super Newton method
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  const Double_t deux = 2.0;
  Double_t x,y;
  Double_t a11,a12,a22,b1,b2,dd;
  TSComplex z1,z2,z3,z4,z5;
  TSComplex za,zz,f0,f1,f2,g0,g1,g2;
  TSComplex I(zero,un);
  za = z;
  f0 = F40(za);
  f1 = F4p(za);
  f2 = F4s(za);
  g0 = f0;
  g0.C();
  g1 = f1;
  g1.C();
  g2 = f2;
  g2.C();
  z1  = g0*f2;
  z2  = g2*f0;
  z3  = deux*g1*f1;
  z4  = g1*f0;
  z5  = g0*f1;
  zz  = z3 + z1 + z2;
  a11 = zz.Re();
  zz  = I*(z1 - z2);
  a12 = zz.Re();
  zz  = z3 - z1 - z2;
  a22 = zz.Re();
  zz  = zero - z4 - z5;
  b1  = zz.Re();
  zz  = I*(z4 - z5);
  b2  = zz.Re();
  dd  = a11*a22 - a12*a12;
  x   = (b1*a22 - b2*a12)/dd;
  y   = (b2*a11 - b1*a12)/dd;
  zz.Set(x,y);
  z   = zz + z;
}
Bool_t TEqIndex::SuperNewton(TSComplex &z,Bool_t sg,Double_t &prec) {
  //Solve z^2  + a*z + (b*z+c)*Sqrt(z-g) + d == 0  by iteration
  const Int_t MaxStep    = 40;
  const Double_t LimConv = 1.0e-10;
  const Double_t smooth  = 0.5;
  Bool_t conv;
  Int_t k = 0;
  TSComplex zz,ceps,zzold;
  Double_t eps,epsold;
  Double_t lim;
  lim = TMath::Min(prec,LimConv);
  zz   = z;
  fRac = Rac(zz,sg);
  ceps = F0(zz);
  eps  = Abs(ceps);
  while ((eps>lim) && (k<MaxStep)) {
    k++;
    StepSN(zz,sg);
    fRac = Rac(zz,sg);
    ceps = F0(zz);
    eps  = Abs(ceps);
  }
  conv = k<MaxStep;
  if (conv) {
    //if process has converged, we execute one cycle more and keep next result
    //only if better than old
    epsold = eps;
    zzold  = zz;
    StepSN(zz,sg);
    fRac = Rac(zz,sg);
    ceps = F0(zz);
    eps  = Abs(ceps);
    if (eps<epsold) {
      z    = zz;
      prec = eps;
    }
    else {
      z    = zzold;
      prec = epsold;
    }
  }
  else {
    //if process has not converged, we try an other time, using smoother convergence
    k = 0;
    zz   = z;
    fRac = Rac(zz,sg);
    ceps = F0(zz);
    eps  = Abs(ceps);
    while ((eps>lim) && (k<MaxStep)) {
      k++;
      StepSN(zz,sg,smooth);
      fRac = Rac(zz,sg);
      ceps = F0(zz);
      eps  = Abs(ceps);
    }
    conv = k<MaxStep;
    if (conv) {
      //if process has converged, we execute one cycle more and keep next result
      //only if better than old
      epsold = eps;
      zzold  = zz;
      StepSN(zz,sg);
      fRac = Rac(zz,sg);
      ceps = F0(zz);
      eps  = Abs(ceps);
      if (eps<epsold) {
        z    = zz;
        prec = eps;
      }
      else {
        z    = zzold;
        prec = epsold;
      }
    }
  }
  return conv;
}
Bool_t TEqIndex::SuperNewt4(TSComplex &z,Double_t &prec) const {
  //Solve z^4  + fA*z^3 + fB*z^2 + fC*z + fD  by iteration
  const Int_t MaxStep    = 40;
  const Double_t LimConv = 1.0e-10;
  Bool_t conv;
  Int_t k = 0;
  TSComplex zz,ceps,zzold;
  Double_t eps,epsold;
  Double_t lim;
  lim = TMath::Min(prec,LimConv);
  zz   = z;
  ceps = F40(zz);
  eps  = Abs(ceps);
  while ((eps>lim) && (k<MaxStep)) {
    k++;
    StepSN4(zz);
    ceps = F40(zz);
    eps  = Abs(ceps);
  }
  conv = k<MaxStep;
  if (conv) {
    //if process has converged, we execute one cycle more and keep next result
    //only if better than old
    epsold = eps;
    zzold  = zz;
    StepSN4(zz);
    ceps = F40(zz);
    eps  = Abs(ceps);
    if (eps<epsold) {
      z    = zz;
      prec = eps;
    }
    else {
      z    = zzold;
      prec = epsold;
    }
  }
  return conv;
}
void TEqIndex::Troisieme(const Double_t &x,Double_t aa, Double_t bb, Double_t cc) const {
  //Solve 3rd order equation when real
  const Double_t lim = 1.0e-12;
  const Int_t max    = 40;
  Int_t k=0;
  Double_t x0,y0,x1,yp;
  x0 = x;
  y0 = x0*x0*x0 + aa*x0*x0 + bb*x0 + cc;
  while ((y0>lim) && (k<max)) {
    k++;
    yp = 3.0*x0*x0 + 2.0*aa*x0 + bb;
    x1 = x0 - y0/yp;
    x0 = x1;
    y0 = x0*x0*x0 + aa*x0*x0 + bb*x0 + cc;
  }
}
void TEqIndex::WhichIsReal(Double_t lreal,Bool_t redochoice) {
  //The family under evaluation being fZc, look which solution is real. We put a
  //limit of lreal on the imaginary part under which the solution is considered
  //real and the imaginary part is cleared. But we must be careful that the
  //number of real solutions has to be 0, 2 or 4, never 1 or 3 !
  const Double_t LimConv  = 1.0e-10;
  Short_t is;
  Bool_t b,HasConverged;
  TSComplex zz,eps1,eps2;
  Double_t x,y,d;
  fNr = 0;
  for (is=0;is<4;is++)
    if (TMath::Abs(fZc[is].Im())<lreal) fNr += 1;
  switch (fNr) {
  case 1:
    fNr = 0;
    break;
  case 3:
    fNr = 2;
    break;
  default:
    break;
  }
  switch (fNr) {
  case 2:
    for (is=0;is<2;is++) {
      if (!redochoice) {
        zz = fZc[is];
        if (TMath::Abs(zz.Im())<LimConv) {
          fZc[is].PureReal();
          fRc[is] = kFALSE;
        }
        else {
          zz.PureReal();
          b  = fBc[is];
          d  = LimConv;
          HasConverged = SuperNewton(zz,b,d);
          if ((HasConverged) && (TMath::Abs(zz.Im())<LimConv)) {
            zz.PureReal();
            fZc[is]  = zz;
            fRc[is]  = kFALSE;
            fQsc[is] = d;
          }
        }
      }
      //Redo Choice of sign of Sqrt(z-g)
      else {
        fZc[is].PureReal();
        fRc[is] = kFALSE;
        zz   = fZc[is];
        fRac = Rac(zz,kTRUE);
        eps1 = F0(zz);
        fRac.SetNeg();
        eps2 = F0(zz);
        x    = Abs(eps1);
        y    = Abs(eps2);
        if (x<=y) {
          fQsc[is] = x;
          fBc[is]  = kTRUE;
        }
        else {
          fQsc[is] = y;
          fBc[is]  = kFALSE;
        }
      }
    }
    for (is=2;is<4;is++) fRc[is] = kTRUE;
    break;
  case 4:
    for (is=0;is<4;is++) {
      if (!redochoice) {
        zz = fZc[is];
        if (TMath::Abs(zz.Im())<LimConv) {
          fZc[is].PureReal();
          fRc[is] = kFALSE;
        }
        else {
          zz.PureReal();
          b  = fBc[is];
          d  = LimConv;
          HasConverged = SuperNewton(zz,b,d);
          if ((HasConverged) && (TMath::Abs(zz.Im())<LimConv)) {
            zz.PureReal();
            fZc[is]  = zz;
            fRc[is]  = kFALSE;
            fQsc[is] = d;
          }
        }
      }
      else {
        //Redo Choice of sign of Sqrt(z-g)
        fZc[is].PureReal();
        fRc[is] = kFALSE;
        zz   = fZc[is];
        fRac = Rac(zz,kTRUE);
        eps1 = F0(zz);
        fRac.SetNeg();
        eps2 = F0(zz);
        x    = Abs(eps1);
        y    = Abs(eps2);
        if (x<=y) {
          fQsc[is] = x;
          fBc[is]  = kTRUE;
        }
        else {
          fQsc[is] = y;
          fBc[is]  = kFALSE;
        }
      }
    }
    break;
  default:
    break;
  }
}
TSComplex TEqIndex::Y(TSComplex z,Bool_t ssqr) const {
  // Returns the value of the equation at z
  //  - with positive Sqrt(z-g) if ssqr true
  //  - with negative Sqrt(z-g) if ssqr false
  TSComplex y,rac;
  rac = Sqrt(z-g);
  if (!ssqr) rac = 0.0 - rac;
  y = z*z+a*z+d+(b*z+c)*rac;
  return y;
}
TSComplex TEqIndex::Y(Double_t x,Bool_t ssqr) const {
  // Returns the may be complex value of the equation at real x
  //  - with positive Sqrt(z-g) if ssqr true
  //  - with negative Sqrt(z-g) if ssqr false
  TSComplex y,zg,rac;
  zg = x - g;
  rac = Sqrt(zg);
  if (!ssqr) rac = 0.0 - rac;
  y = x*x+a*x+d+(b*x+c)*rac;
  return y;
}
TSComplex TEqIndex::Y(Short_t k,TSComplex z,Bool_t ssqr) const {
  // If k==0 returns the value of the equation at z
  // If k==1 returns the value of the 1st derivative of the equation at z
  // If k==2 returns the value of the 2nd derivative of the equation at z
  //  - with positive Sqrt(z-g) if ssqr true
  //  - with negative Sqrt(z-g) if ssqr false
  TSComplex y(0.0,0.0);
  TSComplex rac;
  rac = Sqrt(z-g);
  if (!ssqr) rac = 0.0 - rac;
  switch (k) {
    case 0:
      y = z*z+a*z+d+(b*z+c)*rac;
      break;
    case 1:
      y = 2.0*z+a+b*rac+(b*z+c)/(2.0*rac);
      break;
    case 2:
      y = 2.0 + b/(2.0*rac) + (b*z-2*b*g-c)/(4.0*rac*rac*rac);
      break;
    default:
      break;
  }
  return y;
}
TSComplex TEqIndex::Y(Short_t k,Double_t x,Bool_t ssqr) const {
  // If k==0 returns the value of the equation at z
  // If k==1 returns the value of the 1st derivative of the equation at z
  // If k==2 returns the value of the 2nd derivative of the equation at z
  //  - with positive Sqrt(z-g) if ssqr true
  //  - with negative Sqrt(z-g) if ssqr false
  TSComplex y(0.0,0.0);
  TSComplex zg,rac;
  zg = x - g;
  rac = Sqrt(zg);
  if (!ssqr) rac = 0.0 - rac;
  switch (k) {
    case 0:
      y = x*x+a*x+d+(b*x+c)*rac;
      break;
    case 1:
      y = 2*x+a+b*rac+(b*x+c)/(2.0*rac);
      break;
    case 2:
      y = 2.0 + b/(2.0*rac) + (b*x-2*b*g-c)/(4.0*rac*rac*rac);
      break;
    default:
      break;
  }
  return y;
}
Double_t TEqIndex::YAbs(TSComplex z,Bool_t ssqr) const {
  // Returns the absolute value of the equation at z. If z is a
  //root, it is a measure of the quality of the root
  Double_t rho;
  TSComplex y;
  y = Y(z,ssqr);
  rho = Abs(y);
  return rho;
}
Double_t TEqIndex::YAbs(Double_t x,Bool_t ssqr) const {
  // Returns the absolute value of the equation at x. If x is a
  //root, it is a measure of the quality of the root
  Double_t rho;
  TSComplex y;
  y = Y(x,ssqr);
  rho = Abs(y);
  return rho;
}
