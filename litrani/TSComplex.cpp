// @(#)root/html:$Id: TSComplex.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TSComplex.h"
ClassImp(TSComplex)

//______________________________________________________________________________
//
// Implement complex numbers in Litrani, with many operations using complex
//numbers. Look at all the methods for documentation. Done before complex
//numbers introduced into ROOT. Name changed from TComplex to TSComplex
//to not interfere with the ROOT class
//
void TSComplex::BetterConj(TSComplex &z2) {
  //Make this and z2 better complex conjugate
  const Double_t deux = 2.0;
  Double_t a,b;
  a = (fRe + z2.fRe)/deux;
  b = (fIm - z2.fIm)/deux;
  z2.fRe =  a;
  z2.fIm = -b;
  fRe    = a;
  fIm    = b;
}
void TSComplex::CosFromSin() {
  //  Given a complex number representing the sinus of a complex angle, returns
  //the complex value of the cosinus of the same complex angle. The value chosen
  //is the one with a positive real part, if the imaginary part is 0.
  //If the imaginary part is non-zero, the value chosen is the one with a
  //negative imaginary part. THE IMAGINARY PART OF THE RESULT IS ALWAYS CHOSEN
  //NEGATIVE IF IT IS NON-ZERO. This is an arbitrary choice motivated by the
  //fact that in Litrani, the phases of waves have to be negative when a wave
  //is absorbed, never positive which would correspond to an unphysical explosion
  //of the wave.
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t vsmall = 1.0e-12;
  TSComplex z;
  z = *this;
  z = Sqrt(un - z*z);
  if (z.fIm>vsmall) z = zero - z;
  if (TMath::Abs(z.fIm)<=vsmall) z.fIm = zero;
  *this = z;
}
void TSComplex::FromRhoPhi(const TCplxRhoPhi &r) {
  //returns from phase representation
  Double_t rho;
  Double_t phi;
  rho = r.GetRho();
  phi = r.GetPhi();
  fRe = rho*TMath::Cos(phi);
  fIm = rho*TMath::Sin(phi);
}
Bool_t TSComplex::IsComplex(Double_t limreal) {
  //if the abs. value of the imaginary part is smaller than limreal, make the
  //number real and returns kFALSE else returns kTRUE.
  const Double_t zero = 0.0;
  Bool_t cplx = kTRUE;
  if (TMath::Abs(fIm)<TMath::Abs(limreal)) {
    fIm = zero;
    cplx = kFALSE;
  }
  return cplx;
}
void TSComplex::Print() const {
  //Prints the complex number
  cout << *this << endl;;
}
void TSComplex::RPhi(Double_t &r, Double_t &phi) const {
  //  Calculates module (with sign !) and phase of a complex number.
  //If the real part of the complex number is negativ, returns a negative module.
  //Advantage : in case of a negative real number, the module stays negative and
  //the phase 0. A negative real number is not considered as a non-real, complex
  //number with a phase of pi.
  const Double_t zero   = 0.0;
  const Double_t wsmall = 1.0e-300;
  Double_t axr,axi;
  axr = TMath::Abs(fRe);
  axi = TMath::Abs(fIm);
  if ((axr<wsmall) && (axi<wsmall)) {
    r   = zero;
    phi = zero;
  }
  else {
    r   = TMath::Sqrt(fRe*fRe+fIm*fIm);
    if (fRe<zero) r = -r;
    if (axi<wsmall) {
      phi = zero;
    }
    else {
      phi = atan2(fIm,fRe);
      if (fRe<zero) {
        if (fIm>=zero) phi = phi - TMath::Pi();
        else phi = phi + TMath::Pi();
      }
    }
  }
}
TSComplex TSComplex::operator+(const TSComplex &z) const {
  //+ with other complex number
  TSComplex z4;
  z4.fRe = fRe + z.fRe;
  z4.fIm = fIm + z.fIm;
  return z4;
}
TSComplex TSComplex::operator+(const Double_t x) const {
  //+ with other complex number
  TSComplex z4;
  z4.fRe = fRe + x;
  z4.fIm = fIm;
  return z4;
}
TSComplex TSComplex::operator-(const TSComplex &z) const {
  //- with other complex number
  TSComplex z4;
  z4.fRe = fRe - z.fRe;
  z4.fIm = fIm - z.fIm;
  return z4;
}
TSComplex TSComplex::operator-(const Double_t x) const {
  //- with other complex number
  TSComplex z4;
  z4.fRe = fRe - x;
  z4.fIm = fIm;
  return z4;
}
TSComplex TSComplex::operator*(const Double_t x) const {
  //* with real number
  TSComplex z4;
  z4.fRe = fRe*x;
  z4.fIm = fIm*x;
  return z4;
}
TSComplex TSComplex::operator/(const TSComplex &z) const {
  //division with other complex number
  TSComplex z4;
  Double_t d;
  d = z.fRe*z.fRe + z.fIm*z.fIm;
  z4.fRe = (fRe*z.fRe + fIm*z.fIm)/d;
  z4.fIm = (fIm*z.fRe - fRe*z.fIm)/d;
  return z4;
}
TSComplex TSComplex::operator/(const Double_t x) const {
  //division with real number
  TSComplex z4;
  z4.fRe = fRe/x;
  z4.fIm = fIm/x;
  return z4;
}
TSComplex& TSComplex::operator+=(const TSComplex &z) {
  //Add itself with a complex numbers
  fRe += z.fRe;
  fIm += z.fIm;
  return *this;
}
TSComplex& TSComplex::operator+=(const Double_t &x) {
  //Add itself with a real number
  fRe += x;
  return *this;
}
TSComplex& TSComplex::operator=(const TSComplex &z) {
  //defines = for complex
  if (this != &z) {
    fRe = z.fRe;
    fIm = z.fIm;
  }
  return *this;
}
TSComplex& TSComplex::operator=(const Double_t x) {
  //defines = for complex
  fRe = x;
  fIm = 0.0;
  return *this;
}
TSComplex operator + (const Double_t x, const TSComplex &z) {
  //addition of a real and a complex
  return TSComplex(x + z.fRe,z.fIm);
}
TSComplex operator - (const Double_t x, const TSComplex &z) {
  //substraction of a complex from a real
  return TSComplex(x - z.fRe,-z.fIm);
}
TSComplex operator * (const Double_t x, const TSComplex &z) {
  //multiplication of a real by a complex
  return TSComplex(x*z.fRe,x*z.fIm);
}
TSComplex operator * (const TSComplex &x, const TSComplex &y) {
  //multiplication of a real by a complex
  return TSComplex(x.fRe*y.fRe - x.fIm*y.fIm,x.fRe*y.fIm + x.fIm*y.fRe);
}
TSComplex operator / (const Double_t x, const TSComplex &z) {
  //division of a real by a complex
  TSComplex z4;
  Double_t d;
  d = z.fRe*z.fRe + z.fIm*z.fIm;
  z4.fRe =  (x*z.fRe)/d;
  z4.fIm = -(x*z.fIm)/d;
  return z4;
}
Double_t Abs(const TSComplex &z) {
  //Calculates the absolute value of a complex number
  return TMath::Sqrt(z.fRe*z.fRe + z.fIm*z.fIm);
}
TSComplex Sqrt(const TSComplex &z) {
  //Calculates the square root of a complex number
  const Double_t zero = 0.0;
  TSComplex zz;
  if (z.fRe >= zero) {
    TCplxRhoPhi r(z.fRe,z.fIm);
    r.Sqrt();
    zz.FromRhoPhi(r);
  }
  else {
    TCplxRhoPhi r(-z.fRe,-z.fIm);
    r.Sqrt();
    zz.FromRhoPhi(r);
    TSComplex j(0,1);
    zz = zz*j;
  }
  return zz;
}
TSComplex Sqrt3(const TSComplex &z,Int_t k) {
  //
  //  Calculates the cubic root of a complex number.
  //  If the real part is negative, calculates the cubic root of minus the complex
  //number and then changes the sign of the solution.
  //  There are 3 possible solutions. k decides which one is taken :
  //    - k = 0  ==>  phi --> phi/3
  //    - k = 1  ==>  phi --> (phi+2pi)/3
  //    - k = 2  ==>  phi --> (phi-2pi)/3
  //
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t trois  = 3.0;
  const Double_t small  = 1.0e-7;
  const Double_t wsmall = 1.0e-300;
  Double_t rho,phi;
  Bool_t isreal,isneg;
  TSComplex z1,zz;
  isreal = (TMath::Abs(z.fIm)<wsmall);
  isneg  = (z.fRe<zero);
  if (isneg) z1 = zero - z;
  else       z1 = z;
  k      = k % 3;
  TCplxRhoPhi r(z1.fRe,z1.fIm);
  rho = r.GetRho();
  phi = r.GetPhi();
  rho = TMath::Exp(TMath::Log(rho)/trois);
  switch (k) {
  case 0:
    phi = phi/trois;
    break;
  case 1:
    phi = (phi + deux*TMath::Pi())/trois;
    break;
  case 2:
    phi = (phi - deux*TMath::Pi())/trois;
    break;
  }
  zz.fRe = rho*TMath::Cos(phi);
  zz.fIm = rho*TMath::Sin(phi);
  if (isreal && (TMath::Abs(zz.fIm)<small)) zz.fIm = zero;
  if (isneg) zz = zero - zz;
  return zz;
}
TSComplex Exp(const TSComplex &z) {
  //Calculates the exponential of a complex number
  register Double_t ex;
  TSComplex zz;
  ex = TMath::Exp(z.fRe);
  zz.fRe = ex*TMath::Cos(z.fIm);
  zz.fIm = ex*TMath::Sin(z.fIm);
  return zz;
}
TSComplex Log(const TSComplex &z) {
  //Calculates the logarithm of a complex number
  TSComplex zz;
  TCplxRhoPhi r(z.fRe,z.fIm);
  zz.fRe = TMath::Log(r.GetRho());
  zz.fIm = r.GetPhi();
  return zz;
}
TSComplex Sin(const TSComplex &z) {
  //Calculates the sinus of a complex number
  const Double_t un   = 1.0;
  const Double_t deux = 2.0;
  TSComplex zz;
  Double_t a,b;
  a = TMath::Exp(z.fIm);
  b = un/a;
  zz.fRe = ((a+b)*TMath::Sin(z.fRe))/deux;
  zz.fIm = ((a-b)*TMath::Cos(z.fRe))/deux;
  return zz;
}
TSComplex Cos(const TSComplex &z) {
  //Calculates the cosinus of a complex number
  const Double_t un   = 1.0;
  const Double_t deux = 2.0;
  TSComplex zz;
  Double_t a,b;
  a = TMath::Exp(z.fIm);
  b = un/a;
  zz.fRe = ((a+b)*TMath::Cos(z.fRe))/deux;
  zz.fIm = ((b-a)*TMath::Sin(z.fRe))/deux;
  return zz;
}
istream &operator>>(istream &is,TSComplex &z) {
  //Allows to input a complex number
  char c;
  cin >> c >> z.fRe;
  cin >> c >> z.fIm;
  cin >> c;
  return ( is );
}
ostream &operator<<(ostream &os,const TSComplex &z) {
  //Allows to output a complex number
  os << '(' << z.fRe << ',' << z.fIm << ")  ";
  return ( os );
}
