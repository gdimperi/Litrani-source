// @(#)root/html:$Id: TPol3.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TLitPhys.h"
#include "TPol3.h"

ClassImp(TPol3)
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
//  General solution for the polynom of 3rd order with real coefficients.
//
TPol3::TPol3() {
  const Double_t LimConv = 1.0e-10;
  fLimConv = LimConv;
  fA2 =  0;
  fA1 = -1;
  fA0 = -6;
}
TPol3::TPol3(Double_t a2,Double_t a1,Double_t a0) {
  //Constructor
  const Double_t LimConv = 1.0e-10;
  fLimConv = LimConv;
  fA2 = a2;  fA1 = a1;  fA0 = a0;
}
TSComplex TPol3::F0(TSComplex z) const {
  //calculates the polynom
  TSComplex zz;
  zz = fA0 + z*(fA1 + z*(fA2 + z));
  return zz;
}
Double_t TPol3::MakeBetter(Double_t &x,Bool_t &bb) const {
  const Int_t    MaxStep = 40;
  Int_t kstep = 0;
  Double_t xn,xs,eps0,eps1;
  bb = kTRUE;
  eps0 = 100.0;
  while ((eps0>fLimConv) && (kstep<MaxStep) && (bb)) {
    xn = Step3(x,bb);
    x  = xn;
    eps0 = TMath::Abs(RF0(x));
    kstep++;
  }
  if ((eps0>fLimConv) || (!bb)) bb = kFALSE;
  else {
    bb = kTRUE;
    xs = x;
    eps1 = eps0;
    xn = Step3(x,bb);
    x  = xn;
    eps0 = TMath::Abs(RF0(x));
    if ((!bb) || (eps1<eps0)) {
      x = xs;
      eps0 = eps1;
      bb = kTRUE;
    }
  }
  return eps0;
}
inline Double_t TPol3::RF0(Double_t x) const {
  Double_t poly;
  poly = fA0 + x*(fA1 + x*(fA2 + x));
  return poly;
}
inline Double_t TPol3::RF1(Double_t x) const {
  const Double_t deux  = 2.0;
  const Double_t trois = 3.0;
  Double_t poly;
  poly = fA1 + x*(deux*fA2 + trois*x);
  return poly;
}
inline Double_t TPol3::RF2(Double_t x) const {
  const Double_t deux   = 2.0;
  const Double_t six    = 6.0;
  Double_t poly;
  poly = deux*fA2 + six*x;
  return poly;
}
void TPol3::Set(Double_t a2,Double_t a1,Double_t a0) {
  //new problem
  fA2 = a2;  fA1 = a1;  fA0 = a0;
}
Bool_t TPol3::Solve(Double_t &x1,Double_t &t1,Double_t &t2,Double_t &eps) {
  //If there are 3 real solutions,
  //   returns kFALSE and
  //           these 3 real solutions in x1, t1, t2.
  //If there are one real solution and a pair of complex conjugate solution,
  //   returns kTRUE and
  //           the real solution in x1, 
  //           the real part of the complex solution in t1
  //           the positive imaginary part in t2
  //eps is the precision of the solution.
  //
  const Double_t v0  = 0.0;
  const Double_t v1  = 1.0;
  const Double_t v2  = 2.0;
  const Double_t v3  = 3.0;
  const Double_t v4  = 4.0;
  const Double_t v27 = 27.0;
  TSComplex I(v0,v1);
  Double_t p,q,d,u,v,su,sv,ps3,drp3,p3,qs2,uv,as3,sq3,pis3;
  Double_t phi,cphi,bt1,cbt1,bt2,cbt2,bt3,cbt3;
  Double_t eps1,eps2,eps3;
  Double_t b,c;
  Bool_t bb,order;
  sq3  = TMath::Sqrt(v3);
  pis3 = TMath::Pi()/v3;
  as3  = fA2/v3;
  p    = fA1 - (fA2*fA2)/v3;
  q    = fA0 - (fA2*fA1)/v3 + (v2*fA2*fA2*fA2)/v27;
  ps3  = p/v3;
  qs2  = q/v2;
  p3   = ps3*ps3*ps3;
  d    = qs2*qs2 + p3;
  if (d>=v0) {
    //only one real solution
    bb = kTRUE;
    d   = TMath::Sqrt(d);
    u   = -qs2 + d;
    su  = TMath::Sign(v1,u);
    u   = TMath::Abs(u);
    u   = su*TMath::Exp(TMath::Log(u)/v3);
    v   = -qs2 - d;
    sv  = TMath::Sign(v1,v);
    v   = TMath::Abs(v);
    v   = sv*TMath::Exp(TMath::Log(v)/v3);
    uv  = u + v;
    x1  = uv - as3;
    t1  = - uv/v2 - as3;
    t2  = TMath::Abs(((u-v)*sq3)/v2);
    fZ1 = x1;
    fZ2 = t1 + t2*I;
    fZ3 = t1 - t2*I;
  }
  else {
    //3 real solutions
    bb   = kFALSE;
    cphi = -qs2/TMath::Sqrt(-p3);
    phi  = TLitPhys::ACos(cphi);
    bt1  = phi/v3;
    cbt1 = TMath::Cos(bt1);
    bt2  = pis3 + bt1;
    cbt2 = TMath::Cos(bt2);
    bt3  = pis3 - bt1;
    cbt3 = TMath::Cos(bt3);
    drp3 = v2*TMath::Sqrt(-ps3);
    x1   = drp3*cbt1 - as3;
    t1   = -drp3*cbt2 - as3;
    t2   = -drp3*cbt3 - as3;
    fZ1  = x1;
    fZ2  = t1;
    fZ3  = t2;
  }
  //Verification
  eps1 = Abs(F0(fZ1));
  eps2 = Abs(F0(fZ2));
  eps3 = Abs(F0(fZ3));
  eps  = eps1 + eps2 + eps3;
  if (eps>fLimConv) {
    Bool_t bbb;
    eps1 = MakeBetter(x1,bbb);
    if ((eps1<fLimConv) && (bbb)) {
      c = - fA0/x1;
      b = fA2 + x1;
      d = b*b - v4*c;
      if (d>=v0) {
        bb = kFALSE;
        d  = TMath::Sqrt(d);
        t1 = (-b + d)/v2;
        t2 = (-b - d)/v2;
        fZ1  = x1;
        fZ2  = t1;
        fZ3  = t2;
      }
      else {
        bb = kTRUE;
        d  = TMath::Sqrt(-d);
        t1 = -b/v2;
        t2 =  d/v2;
        fZ1 = x1;
        fZ2 = t1 + t2*I;
        fZ3 = t1 - t2*I;
      }
    }
  }
  if (!bb) {
    order = kFALSE;
    while (!order) {
      order = kTRUE;
      if (t1>x1) {
        p  = t1;
        t1 = x1;
        x1 = p;
        order = kFALSE;
      }
      if ( t2>t1) {
        p  = t2;
        t2 = t1;
        t1 = p;
        order = kFALSE;
      }
    }
  }
  return bb;
}
Double_t TPol3::Step3(Double_t x,Bool_t &bb) const {
  //Step for pol3
  const Double_t zero = 0.0;
  const Double_t deux = 2.0;
  const Double_t z01     = 0.1;
  const Double_t VerySmall = 1.0e-300;
  Double_t xn2;
  Double_t xn1 = 0.0;
  Double_t h0,h1,h2,ah1,ah2,ahs,dd;
  Double_t eps1,eps2;
  h0  = RF0(x);
  h1  = RF1(x);
  h2  = RF2(x);
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
