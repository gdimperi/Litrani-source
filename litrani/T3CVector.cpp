// @(#)root/html:$Id: T3CVector.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TMessErr.h"
#include "T3Vector.h"
#include "T3CVector.h"

ClassImp(T3CVector)

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
// Implement complex 3-vectors in Litrani
//
//______________________________________________________________________________

T3CVector::T3CVector(TSComplex a) {
  //constructor with all elements initialized to same value
  fV0 = a;
  fV1 = a;
  fV2 = a;
}
T3CVector::T3CVector(TSComplex v1,TSComplex v2,TSComplex v3) {
  //constructor with 3 components given
  fV0 = v1;
  fV1 = v2;
  fV2 = v3;
}
T3CVector::T3CVector(const T3CVector &v) {
  //constructor with vector given
  fV0 = v.fV0;
  fV1 = v.fV1;
  fV2 = v.fV2;
}
Bool_t T3CVector::AlfaDelta(TSComplex &stheta, TSComplex &ctheta,TSComplex &salfa,
  TSComplex &calfa,TSComplex &sdelta, TSComplex &cdelta) const {
  //From the unit T3CVector "this" calculates alfa and delta for the decomposition :
  //
  //       x[0] = sin(delta)*cos(alfa)
  //       x[1] = cos(delta)*cos(alfa)
  //       x[2] = sin(alfa)
  //
  //AlfaDelta return true, except in the case where cos(alfa) is 0. In that
  //case, delta is undefined, AlfaDelta returns false and arbitrarily puts
  //sin(delta) = stheta, cos(delta) = ctheta.
  //
  //The problem is undefined in the sense that there are 2 possible solutions :
  //we can change the signs of calfa,sdelta and cdelta : we still have a solution.
  //So we choose the solution which is closest stheta and ctheta.
  //
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t wsmall = 1.0e-300;
  TSComplex calf2,sdelt2,cdelt2;
  Double_t dd1,dd2;
  Bool_t b;
  Double_t r;
  salfa = fV2;
  calfa = Sqrt(un - salfa*salfa);
  r     = Abs(calfa);
  if (r>wsmall) {
    b      = kTRUE;
    sdelta = fV0/calfa;
    cdelta = fV1/calfa;
    calf2  = zero - calfa;
    sdelt2 = zero - sdelta;
    cdelt2 = zero - cdelta;
    dd1    = Abs(sdelta - stheta) + Abs(cdelta - ctheta);
    dd2    = Abs(sdelt2 - stheta) + Abs(cdelt2 - ctheta);
    if (dd2<dd1) {
      calfa  = calf2;
      sdelta = sdelt2;
      cdelta = cdelt2;
    }
  }
  else {
    b      = kFALSE;
    salfa  = un;
    calfa  = zero;
    sdelta = stheta;
    cdelta = ctheta;
  }
  return b;
}
Short_t T3CVector::DegMatrix(T33CMatrix &M, T3CVector &y, Bool_t test) {
  //Solve the equation A*x = 0 assuming that matrix A is degenerate. x is the
  //vector "this". There are 3 ways to get vector x. We use the 3, with a
  //ponderate method. The weights used are the absolute sin^2 of the angle
  //between 2 vectors. It is natural, since when this sinus is 0, the solution is
  //undefined.
  //
  //Put test to kTRUE if you want result to be checked.
  //
  //If 0 returned, it means that matrix M is not degenerate, so that the
  //               problem A*x = 0 has no solution other than x = 0.
  //               x = y = 0 is returned
  //If 1 returned, matrix M is once degenerate, normal case. There is
  //               only 1 vector as solution x. Vector y is 0.
  //If 2 returned, matrix M twice degenerate. All vectors in a certain plane are
  //               solutions. We arbitrary return 2 orthogonal vectors in this
  //               plane as solution : vector x and y. Remember in that case that
  //               any linear combination of these 2 vectors is also a solution.
  //If 3 returned, matrix M is the 0 matrix. Any vector is solution, so that it
  //               does not make sense to return something in x or y. We return
  //               x=0 and y=0.
  //
  //When not 0, the return vectors are always normalized to 1
  //
  const Double_t small  = 1.0e-5;
  const Double_t vsmall = 1.0e-15;
  const Double_t ltest  = 5.0*small;
  T3CVector x;
  TSComplex zero(0.0,0.0);
  TSComplex   un(1.0,0.0);
  Short_t deg = 1;
  Double_t a,b,c,d;
  TSComplex ca,cb,cc;
  Bool_t   b0,b1,b2;
  Bool_t   gen = kFALSE;
  Double_t w0,w1,w2,w;
  T3CVector x0,x1,x2,p;
  d = Abs(M.Det());
  x.Set(zero,zero,zero);
  y.Set(zero,zero,zero);
  if (d>small) {
    deg = 0;
    return deg;
  }
  M.GetLine(0,ca,cb,cc);
  T3CVector m0(ca,cb,cc);
  M.GetLine(1,ca,cb,cc);
  T3CVector m1(ca,cb,cc);
  M.GetLine(2,ca,cb,cc);
  T3CVector m2(ca,cb,cc);
  a = m0.Norm2Sqr();
  b = m1.Norm2Sqr();
  c = m2.Norm2Sqr();
  T3Vector vz(a,b,c);
  vz.Unit();
  a = vz(0);
  b = vz(1);
  c = vz(2);
  if (a<vsmall) {
    b0 = kFALSE;
    w0 = 0.0;
  }
  else {
    b0 = kTRUE;
    m0.Unit();
  }
  if (b<vsmall) {
    b1 = kFALSE;
    w1 = 0.0;
  }
  else {
    b1 = kTRUE;
    m1.Unit();
  }
  if (c<vsmall) {
    b2 = kFALSE;
    w2 = 0.0;
  }
  else {
    b2 = kTRUE;
    m2.Unit();
  }
  if (b0) {
    //m0 usable
    if (b1) {
      //m0 && m1 usable
      if (b2) {
        //m0 && m1 && m2 usable          : +0 +1 +2
        x0 = m0^m1;
        x1 = m0^m2;
        x2 = m1^m2;
        w0 = a*b*x0.Norm2Sqr();
        w1 = a*c*x1.Norm2Sqr();
        w2 = b*c*x2.Norm2Sqr();
        w  = w0 + w1 + w2;
        w0 /= w;
        w1 /= w;
        w2 /= w;
        x0.Unit();
        x1.Unit();
        x2.Unit();
        x0.RatherPositiv();
        x1.RatherPositiv();
        x2.RatherPositiv();
        x  = w0*x0 + w1*x1 + w2*x2;
      }
      else {
        //m0 && m1 usable, but not m2    : +0 +1 -2
        x0 = m0^m1;
        w0 = x0.Norm2Sqr();
        if (w0<vsmall) {
          deg = 2;
          p   = m0;
          gen = kTRUE;
        }
        else {
          x0.Unit();
          x0.RatherPositiv();
          x = x0;
        }
      }
    }
    else {
      //m0 usable, but not m1
      if (b2) {
        //m0 && m2 usable, but not m1    : +0 -1 +2
        x1 = m0^m2;
        w1 = x1.Norm2Sqr();
        if (w1<vsmall) {
          deg = 2;
          p   = m0;
          gen = kTRUE;
        }
        else {
          x1.Unit();
          x1.RatherPositiv();
          x = x1;
        }
      }
      else {
        //m0 usable but not m1, not m2   : +0 -1 -2
        deg = 2;
        p   = m0;
        gen = kTRUE;
      }
    }
  }
  else {
    //m0 unusable
    if (b1) {
      //m1 usable but not m0
      if (b2) {
        //m1 && m2 usable, not m0        : -0 +1 +2
        x2 = m1^m2;
        w2 = x2.Norm2Sqr();
        if (w2<vsmall) {
          deg = 2;
          p   = m1;
          gen = kTRUE;
        }
        else {
          x2.Unit();
          x2.RatherPositiv();
          x = x2;
        }
      }
      else {
        //m1 usable, not m0, m2          : -0 +1 -2
        deg = 2;
        p = m1;
        gen = kTRUE;
      }
    }
    else {
      //m0, m1 unusable
      if (b2) {
        //m2 usable, not m0,m1           : -0 -1 +2
        deg = 2;
        p = m2;
        gen = kTRUE;
      }
      else {
        //m0, m1, m2 unusable            : -0 -1 -2
        deg = 3;
      }
    }
  }
  if (gen) {
    x0.Set(un,zero,zero);
    x1.Set(zero,un,zero);
    x2.Set(zero,zero,un);
    a  = Abs(p*x0);
    b  = Abs(p*x1);
    c  = Abs(p*x2);
    if (a<=b) {
      if (a<=c) {
        x = p^x0;
        x.Unit();
        y = p^x;
        y.Unit();
      }
      else {
        x = p^x2;
        x.Unit();
        y = p^x;
        y.Unit();
      }
    }
    else {
      if (b<=c) {
        x = p^x1;
        x.Unit();
        y = p^x;
        y.Unit();
      }
      else {
        x = p^x2;
        x.Unit();
        y = p^x;
        y.Unit();
      }
    }
  }
  if (test) {
    switch (deg) {
    case 1:
      x0 = M*x;
      w0 = x0.Module();
      break;
    case 2:
      x0 = M*x;
      x1 = M*y;
      w0 = x0.Module() + x1.Module();
      break;
    default:
      w0 = 0.0;
      break;
    }
    if (w0>ltest) {
      gMes->SetName("Unit");
      gMes->SetTitle("T3CVector");
      gMes->M(Error_M,1,"module of vector is 0",w0);
    }
  }
  *this = x;
  return deg;
}
void T3CVector::Get(TSComplex &a0,TSComplex &a1,TSComplex &a2) const {
  // Returns 3 elements into a0,a1,a2
  a0 = fV0;
  a1 = fV1;
  a2 = fV2;
}
Double_t T3CVector::Module() const {
  //Calculates the module of the 3-vector
  Double_t sum;
  sum = TMath::Sqrt(Norm2Sqr());
  return sum;
}
Double_t T3CVector::Norm2Sqr() const {
  //Calculates the scalar product of the vector with its complex conjugate
  TSComplex sum;
  TSComplex a(fV0);
  TSComplex b(fV1);
  TSComplex c(fV2);
  a.C();
  b.C();
  c.C();
  sum = a*fV0 + b*fV1 +c*fV2;
  return sum.Re();
}
void T3CVector::RatherPositiv() {
  //If the direction of a vector is free, chooses it rather positiv and real
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Double_t d,d2;
  d  = TMath::Abs(fV0.Re() + fV1.Re() + fV2.Re());
  d2 = TMath::Abs(fV0.Im() + fV1.Im() + fV2.Im());
  if (d2>d) {
    TSComplex I(zero,un);
    fV0 = I*fV0;
    fV1 = I*fV1;
    fV2 = I*fV2;
  }
  d = fV0.Re() + fV0.Im() + fV1.Re() + fV1.Im() + fV2.Re() + fV2.Im();
  if (d<zero) {
    fV0 = zero - fV0;
    fV1 = zero - fV1;
    fV2 = zero - fV2;
  }
}
void T3CVector::Unit() {
  //Makes the length of the vector unity
  const Double_t zero      = 0.0;
  const Double_t VerySmall = 1.0e-306;
  Double_t l;
  l = Module();
  if (l>VerySmall) {
    fV0 = fV0/l;
    fV1 = fV1/l;
    fV2 = fV2/l;
  }
  else {
    const Double_t un   = 1.0;
    Double_t d0,d1,d2,phi;
    fV0.RPhi(d0,phi);
    fV1.RPhi(d1,phi);
    fV2.RPhi(d2,phi);
    if (d0>=d1)
      if (d0>=d2) {
        fV0 = TMath::Sign(un,d0);
        fV1 = zero;
        fV2 = zero;
      }
      else {
        fV0 = zero;
        fV1 = zero;
        fV2 = TMath::Sign(un,d2);
      }
    else
      if (d1>=d2) {
        fV0 = zero;
        fV1 = TMath::Sign(un,d1);
        fV2 = zero;
      }
      else {
        fV0 = zero;
        fV1 = zero;
        fV2 = TMath::Sign(un,d2);
      }
  }
}
T3CVector T3CVector::operator+(const T3CVector &v) {
  //+ with other 3_vector
  T3CVector v4;
  v4.fV0 = fV0 + v.fV0;
  v4.fV1 = fV1 + v.fV1;
  v4.fV2 = fV2 + v.fV2;
  return v4;
}
T3CVector T3CVector::operator-(const T3CVector &v) {
  //- with other 3_vector
  T3CVector v4;
  v4.fV0 = fV0 - v.fV0;
  v4.fV1 = fV1 - v.fV1;
  v4.fV2 = fV2 - v.fV2;
  return v4;
}
TSComplex operator*(const T3CVector &v1, const T3CVector &v2) {
  //Scalar product.
  TSComplex sum;
  sum = v1.fV0*v2.fV0 + v1.fV1*v2.fV1 + v1.fV2*v2.fV2;
  return sum;
}
T3CVector operator*(const TSComplex x, const T3CVector &v) {
  //Multiplication by a complex number
  TSComplex xx;
  T3CVector v1;
  xx = x;
  v1.Set(xx*v.fV0,xx*v.fV1,xx*v.fV2);
  return v1;
}
T3CVector operator*(const T3CVector&v, const TSComplex x) {
  //Multiplication by a complex number
  TSComplex xx;
  T3CVector w;
  xx = x;
  w.Set(xx*v.fV0,xx*v.fV1,xx*v.fV2);
  return w;
}
T3CVector operator*(const Double_t x, const T3CVector &v) {
  //Multiplication by a real number
  return T3CVector(x*v.fV0,x*v.fV1,x*v.fV2);
}
T3CVector operator*(const T3CVector&v, const Double_t x) {
  //Multiplication by a real number
  return T3CVector(x*v.fV0,x*v.fV1,x*v.fV2);
}
T3CVector operator*(const T33CMatrix &m, const T3CVector &v) {
  //Multiplication of a matrix by a column vector
  T3CVector w;
  w.fV0 = m.fM00*v.fV0  +  m.fM01*v.fV1  +  m.fM02*v.fV2;
  w.fV1 = m.fM10*v.fV0  +  m.fM11*v.fV1  +  m.fM12*v.fV2;
  w.fV2 = m.fM20*v.fV0  +  m.fM21*v.fV1  +  m.fM22*v.fV2;
  return w;
}
T3CVector T3CVector::operator^(const T3CVector &v) {
  //Compute the vector product
  T3CVector v4;
  TSComplex a1,a2,a3,b1,b2,b3;
  a1 = fV0;
  a2 = fV1;
  a3 = fV2;
  b1 = v.fV0;
  b2 = v.fV1;
  b3 = v.fV2;
  v4.fV0 = a2*b3 - a3*b2;
  v4.fV1 = a3*b1 - a1*b3;
  v4.fV2 = a1*b2 - a2*b1;
  return v4;
}
T3CVector& T3CVector::operator=(const T3CVector &v) {
  //defines = for 3-vectors
  if (this != &v) {
    fV0 = v.fV0;
    fV1 = v.fV1;
    fV2 = v.fV2;
  }
  return *this;
}
TSComplex &T3CVector::operator()(Int_t ind) const {
  //access to components
  TSComplex *p;
  switch (ind) {
    case 0:
      p = (TSComplex *)&fV0;
      break;
    case 1:
      p = (TSComplex *)&fV1;
      break;
    case 2:
      p = (TSComplex *)&fV2;
      break;
    default:
      p = (TSComplex *)&fV0;
      break;
  }
  return *p;
}
TSComplex &T3CVector::operator()(Int_t index) {
  //access to components
  return (TSComplex&)((*(const T3CVector *)this)(index));
}
