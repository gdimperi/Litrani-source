// @(#)root/html:$Id: TPol4.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TPol4.h"

ClassImp(TPol4)
//______________________________________________________________________________
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
//  General solution for the polynom of 4th order with real coefficients.
//
TPol4::TPol4(Double_t a3) {
  //Constructor
  const Double_t LimConv = 1.0e-10;
  fLimConv = LimConv;
  fA3=a3; fA2=0; fA1=0; fA0=0;
  fP3 = new TPol3();
  fP3->SetLimConv(fLimConv);
}
TPol4::TPol4(Double_t a3,Double_t a2,Double_t a1,Double_t a0) {
  //Constructor
  const Double_t LimConv = 1.0e-10;
  fLimConv = LimConv;
  fA3 = a3;  fA2 = a2;  fA1 = a1;  fA0 = a0; fOk = kFALSE;
  fP3 = new TPol3();
  fP3->SetLimConv(fLimConv);
}
inline TSComplex TPol4::F0(TSComplex z) const {
  //Calculation of the polynom
  TSComplex poly;
  poly = fA0 + z*(fA1 + z*(fA2 + z*(fA3 + z)));
  return poly;
}
inline TSComplex TPol4::F1(TSComplex z) const {
  //Calculation of the derivative of the polynom
  const Double_t deux   = 2.0;
  const Double_t trois  = 3.0;
  const Double_t quatre = 4.0;
  TSComplex poly;
  poly = fA1 + z*(deux*fA2 + z*(trois*fA3 + quatre*z));
  return poly;
}
inline TSComplex TPol4::F2(TSComplex z) const {
  //Calculation of the derivative of the polynom
  const Double_t deux   = 2.0;
  const Double_t six    = 6.0;
  const Double_t douze  = 12.0;
  TSComplex poly;
  poly  = deux*fA2 + z*(six*fA3 + douze*z);
  return poly;
}
inline Double_t TPol4::FR0(Double_t x) const {
  //Calculation of the polynom for x real
  Double_t poly;
  poly = fA0 + x*(fA1 + x*(fA2 + x*(fA3 + x)));
  return poly;
}
inline Double_t TPol4::FR1(Double_t x) const {
  //Calculation of the derivative of the polynom for x real
  const Double_t deux   = 2.0;
  const Double_t trois  = 3.0;
  const Double_t quatre = 4.0;
  Double_t poly;
  poly = fA1 + x*(deux*fA2 + x*(trois*fA3 + quatre*x));
  return poly;
}
inline Double_t TPol4::FR2(Double_t x) const {
  //Calculation of the second derivative of the polynom for x real
  const Double_t deux   = 2.0;
  const Double_t six    = 6.0;
  const Double_t douze  = 12.0;
  Double_t poly;
  poly  = deux*fA2 + x*(six*fA3 + douze*x);
  return poly;
}
Bool_t TPol4::Get(TSComplex &z0,TSComplex &z1,TSComplex &z2,TSComplex &z3,
  Bool_t &b0,Bool_t &b1,Bool_t &b2,Bool_t &b3) const {
  //Returns the solution if available
  const Double_t zero = 0.0;
  if (fOk) {
    z0 = fZ0; z1 = fZ1; z2 = fZ2; z3 = fZ3;
    b0 = fC0; b1 = fC1; b2 = fC2; b3 = fC3;
  }
  else {
    z0 = zero; z1 = zero; z2 = zero; z3 = zero;
    b0 = kFALSE; b1 = kFALSE; b2 = kFALSE; b3 = kFALSE;
  }
  return fOk;
}
void TPol4::LastSolutions() {
  //Finds the 2 last solutions, fZ0 and fZ1 being found
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  Bool_t bb,br;
  Double_t y2,y3,b,c,d,S,P;
  TSComplex z,zs;
  TSComplex I(0,1);
  z = fZ0 + fZ1;
  S = z.Re();
  z = fZ0*fZ1;
  P = z.Re();
  c = fA0/P;
  b = fA3 + S;
  d = b*b - quatre*c;
  if (d>=zero) {
    d   = TMath::Sqrt(d);
    y2  = (-b + d)/deux;
    y3  = (-b - d)/deux;
    fZ2 = y2;
    fZ3 = y3;
    fC2 = kFALSE;
    fC3 = kFALSE;
  }
  else {
    z   = I*TMath::Sqrt(-d);
    fZ2 = (-b + z)/deux;
    fZ3 = (-b - z)/deux;
    fC2 = kTRUE;
    fC3 = kTRUE;
  }
  //Check these solutions
  zs = fZ2;
  br = fC2;
  z  = fZ2;
  fEps2 = Solution(z,bb,2);
  if (!bb) {
    fZ2 = zs;
    fC2 = br;
  }
  zs = fZ3;
  br = fC3;
  z  = fZ3;
  fEps3 = Solution(z,bb,3);
  if (!bb) {
    fZ3 = zs;
    fC3 = br;
  }
}
Double_t TPol4::RSolution(Double_t x,Bool_t &b,Short_t kr) {
  //Finds the 1st solution of the polynom by SuperNewton, starting the itera-
  //tions with the value x
  const Int_t MaxStep    = 40;
  Bool_t br;
  Double_t eps1,eps2;
  Double_t xn,xs,f0;
  Int_t kstep = 0;
  f0 = FR0(x);
  eps1 = TMath::Abs(f0);
  b = kTRUE;
  while ((eps1>fLimConv) && (kstep<MaxStep) && (b)) {
    xn   = RStep4(x,b);
    x    = xn;
    f0   = FR0(x);
    eps1 = TMath::Abs(f0);
    kstep++;
  }
  if ((eps1<=fLimConv) && b) {
    //Do a last iteration
    xs   = x;
    eps2 = eps1;
    xn   = RStep4(x,b);
    x    = xn;
    f0   = FR0(x);
    eps1 = TMath::Abs(f0);
    if ((!b) || (eps1>eps2)) {
      x    = xs;
      eps1 = eps2;
    }
    //Store the result
    br = kFALSE;
    switch (kr) {
    case 0:
      fZ0 = x;
      fC0 = br;
      break;
    case 1:
      fZ1 = x;
      fC1 = br;
      break;
    case 2:
      fZ2 = x;
      fC2 = br;
      break;
    default:
      fZ3 = x;
      fC3 = br;
      break;
    }
  }
  else  b = kFALSE;
  return eps1;
}
Double_t TPol4::RStep4(Double_t x,Bool_t &bb) const {
  //Step for pol3
  const Double_t zero = 0.0;
  const Double_t deux = 2.0;
  const Double_t z01     = 0.1;
  const Double_t VerySmall = 1.0e-300;
  Double_t xn1,xn2;
  Double_t h0,h1,h2,ah1,ah2,ahs,dd;
  Double_t eps1,eps2;
  xn1 = x;
  h0  = FR0(x);
  h1  = FR1(x);
  h2  = FR2(x);
  bb   = kTRUE;
  ah1 = TMath::Abs(h1);
  ahs = z01*ah1;
  ah2 = TMath::Abs(h2);
  dd  = h1*h1 - deux*h0*h2;
  if ((dd>=zero) && (ah2>ahs)) {
    dd  = TMath::Sqrt(dd);
    xn1 = x + (-h1 + dd)/h2;
    xn2 = x - ( h1 + dd)/h2;
    eps1 = TMath::Abs(xn1-x);
    eps2 = TMath::Abs(xn2-x);
    if (eps2<eps1) xn1 = xn2;
  }
  else {
    if (ah1>VerySmall) {
      xn1 = x - h0/h1;
    }
    else bb = kFALSE;
  }
  return xn1;
}
Double_t TPol4::SecondSolution(Bool_t &bb) {
  //In case 1st solution real, finds second real solution.
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  Bool_t bcplx;
  Double_t x1,t1,t2,eps0,xs;
  TSComplex I(zero,un);
  xs = fZ0.Re();
  fB0 = -fA0/xs;
  fB1 = (fB0-fA1)/xs;
  fB2 = (fB1-fA2)/xs;
  fP3->Set(fB2,fB1,fB0);
  bcplx = fP3->Solve(x1,t1,t2,eps0);
  fZ1 = x1;
  fC1 = kFALSE;
  if (bcplx) {
    fZ2 = t1 + t2*I;
    fC2 = kTRUE;
    fZ3 = t1 - t2*I;
    fC3 = kTRUE;
  }
  else {
    fZ2 = t1;
    fC2 = kFALSE;
    fZ3 = t2;
    fC3 = kFALSE;
  }
  bb = (eps0<fLimConv);
  return eps0;
}
void TPol4::Set(Double_t a3,Double_t a2,Double_t a1,Double_t a0) {
  //new problem
  fA3 = a3;  fA2 = a2;  fA1 = a1;  fA0 = a0;  fOk = kFALSE;
}
void TPol4::SetLimConv(Double_t s) {
  fLimConv = s;
  fP3->SetLimConv(fLimConv);
}
Bool_t TPol4::SolDeriv(Double_t &xm,Double_t &R) {
  // Finds the biggest zero of the derivative of the 4th order polynom, giving
  //the rightmost extremum. Add to this value the radius of curvature at this
  //extremum and take this value as starting point for the iterative solution
  //of the 4th order polynom.
  const Double_t un      = 1.0;
  const Double_t deux    = 2.0;
  const Double_t trois   = 3.0;
  const Double_t quatre  = 4.0;
  Bool_t bb;
  Double_t w2,w3,eps0;
  fB0 = fA1/quatre;
  fB1 = fA2/deux;
  fB2 = (trois*fA3)/quatre;
  fP3->Set(fB2,fB1,fB0);
  bb  = fP3->Solve(xm,w2,w3,eps0);
  bb  = (eps0<fLimConv);
  R   = un/TMath::Abs(FR2(xm));
  return bb;
}
Double_t TPol4::Solution(TSComplex z,Bool_t &b,Short_t kr) {
  //Finds the 1st solution of the polynom by SuperNewton, starting the itera-
  //tions with the value z
  const Int_t MaxStep    = 40;
  const Double_t LimReal = 1.0e-7;
  Bool_t br;
  Double_t eps1,eps2;
  TSComplex zn,zs,f0;
  Int_t kstep = 0;
  f0 = F0(z);
  eps1 = Abs(f0);
  b = kTRUE;
  while ((eps1>fLimConv) && (kstep<MaxStep) && (b)) {
    zn   = Step4(z,b);
    z    = zn;
    f0   = F0(z);
    eps1 = Abs(f0);
    kstep++;
  }
  if ((eps1<=fLimConv) && b) {
    //Do a last iteration
    zs   = z;
    eps2 = eps1;
    zn   = Step4(z,b);
    z    = zn;
    f0   = F0(z);
    eps1 = Abs(f0);
    if ((!b) || (eps1>eps2)) {
      z    = zs;
      eps1 = eps2;
    }
    //Store the result
    if (TMath::Abs(z.Im())<LimReal) {
      br = kFALSE;
      z.PureReal();
    }
    else br = kTRUE;
    switch (kr) {
    case 0:
      fZ0 = z;
      fC0 = br;
      break;
    case 1:
      fZ1 = z;
      fC1 = br;
      break;
    case 2:
      fZ2 = z;
      fC2 = br;
      break;
    default:
      fZ3 = z;
      fC3 = br;
      break;
    }
  }
  else  b = kFALSE;
  return eps1;
}
Bool_t TPol4::Solve(Double_t &epsg) {
  //Solve the equation by SuperNewton. Try to avoid the "middle" region, whe-
  //re the Newton method is bad by finding a smart starting value for the
  //iterations. This "smart" value" is right of the rightmost extremum, at a
  //distance R of it, R being the radius of curvature at the rightmost extre-
  //mum. Doing that, we are almost sure that the SuperNewton method will con-
  //verge. Having by that the 1st root of the polynomial equation of the 4th
  //order, finding the second root is a matter of solving a 3rd order poly-
  //nomial equation, which is done by "SecondSolutio". Finding the 2 last
  //roots is trivial and is done by "LastSolutions".
  const Double_t z001  = 0.01;
  Bool_t bb,bb0,bb1,bb2,bb3;
  Double_t xstart,eps,x,xm,R;
  TSComplex zs1,zs2;
  bb0 = SolDeriv(xm,R);
  xstart = xm + R;
  x = xstart;
  epsg = 1.0;
  fEps0 = RSolution(x,bb0,0);
  if (!bb0) {
    zs1.Set(xstart,z001*xstart);
    fEps0 = Solution(zs1,bb0,0);
    if (!bb0) {
      zs2.Set(xstart,xstart);
      fEps0 = Solution(zs2,bb0,0);
    }
  }
  bb = bb0;
  if (bb0) {
    if (!fC0) eps = SecondSolution(bb1);
    else {
      fZ1 = fZ0;
      fZ1.C();
      fC1 = kTRUE;
      LastSolutions();
    }
    //Verify quality of 3 last solutions
    fEps1 = Abs(F0(fZ1));
    bb1   = fEps1<=fLimConv;
    if (!bb1) {
      if (fC1) {
        zs1   = fZ1;
        fEps1 = Solution(zs1,bb1,1);
        if (fC1) {
          fC0 = kTRUE;
          fZ0.BetterConj(fZ1);
        }
      }
      else {
        x     = fZ1.Re();
        fEps1 = RSolution(x,bb1,1);
      }
      fEps0 = Abs(F0(fZ0));
      fEps1 = Abs(F0(fZ1));
    }
    fEps2 = Abs(F0(fZ2));
    bb2 = fEps2<=fLimConv;
    if (!bb2) {
      if (fC2) {
        zs1   = fZ2;
        fEps2 = Solution(zs1,bb2,2);
      }
      else {
        x     = fZ2.Re();
        fEps2 = RSolution(x,bb2,2);
      }
      fEps2 = Abs(F0(fZ2));
    }
    fEps3 = Abs(F0(fZ3));
    bb3   = fEps3<=fLimConv;
    if (!bb3) {
      if (fC3) {
        zs1   = fZ3;
        fEps3 = Solution(zs1,bb3,3);
        if (fC3) {
          fC2 = kTRUE;
          fZ2.BetterConj(fZ3);
        }
      }
      else {
        x     = fZ3.Re();
        fEps3 = RSolution(x,bb3,3);
      }
      fEps2 = Abs(F0(fZ2));
      fEps3 = Abs(F0(fZ3));
    }
    epsg = fEps0 + fEps1 + fEps2 + fEps3;
    bb   = bb0 && bb1 && bb2 && bb3;
    bb   = bb && (epsg<=4*fLimConv);
  }
  fOk = bb;
  return bb;
}
TSComplex TPol4::Step4(TSComplex z,Bool_t &b) const {
  //1 step of SuperNewton
  const Double_t zero = 0.0;
  const Double_t deux = 2.0;
  const Double_t VerySmall = 1.0e-306;
  TSComplex zz;
  TSComplex f0,f1,f2,g0,g1,g2,y;
  Double_t a01,a02,a11,a22,a12,dd,fg1,fg2;
  Double_t dx = zero;
  Double_t dy = zero;
  TSComplex I(0.0,1.0);
  b = kTRUE;
  f0  = F0(z);
  f1  = F1(z);
  f2  = F2(z);
  g0  = f0;
  g0.C();
  g1  = f1;
  g1.C();
  g2  = f2;
  g2.C();
  y   = f0*g1 + g0*f1;
  a01 = - y.Re()/deux;
  y   = I*(g0*f1 - f0*g1);
  a02 = - y.Re()/deux;
  y   = f1*g1;
  fg1 = y.Re();
  y   = g0*f2 + f0*g2;
  fg2 = y.Re()/deux;
  a11 = fg1 + fg2;
  a22 = fg1 - fg2;
  y   = I*(g0*f2 - f0*g2);
  a12 = y.Re()/deux;
  dd  = a11*a22 -a12*a12;
  if (TMath::Abs(dd) > VerySmall) {
    dx = (a01*a22 - a02*a12)/dd;
    dy = (a02*a11 - a01*a12)/dd;
  }
  else b = kFALSE;
  zz.Set(dx,dy);
  zz  += z;
  return zz;
}
