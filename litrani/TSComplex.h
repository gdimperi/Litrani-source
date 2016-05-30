// @(#)root/html:$Id: TSComplex.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSComplex
#define LITR_TSComplex
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSComplex     Implement complex numbers for Litrani                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "Riostream.h"
#include "TObject.h"
#include "TMath.h"

using namespace std;

class TCplxRhoPhi {

protected:
  Double_t fRho;    //module of complex number
  Double_t fPhi;    //phase of complex number

public:
  TCplxRhoPhi(Double_t x,Double_t y) { fRho = TMath::Sqrt(x*x+y*y);
  fPhi = atan2(y,x); }
  Double_t GetRho() const { return fRho; }
  Double_t GetPhi() const { return fPhi; }
  void Sqrt() { fRho = TMath::Sqrt(fRho); fPhi = fPhi/2.0; }
};

class TSComplex {

protected:

  Double_t fRe;     //real part
  Double_t fIm;     //imaginary part

  friend TSComplex  operator + (const Double_t,const TSComplex&);
  friend TSComplex  operator - (const Double_t,const TSComplex&);
  friend TSComplex  operator * (const Double_t,const TSComplex&);
  friend TSComplex  operator * (const TSComplex&,const TSComplex&);
  friend TSComplex  operator / (const Double_t,const TSComplex&);
  friend Double_t   Abs(const TSComplex&);
  friend TSComplex  Sqrt(const TSComplex&);
  friend TSComplex  Sqrt3(const TSComplex&, Int_t);
  friend TSComplex  Exp(const TSComplex&);
  friend TSComplex  Log(const TSComplex&);
  friend TSComplex  Sin(const TSComplex&);
  friend TSComplex  Cos(const TSComplex&);
  friend istream   &operator>>(istream&,TSComplex&);
  friend ostream   &operator<<(ostream&,const TSComplex&);

public:

  TSComplex()                           { fRe=0.0; fIm =0.0;        }
  TSComplex(Double_t x,Double_t y)      { fRe = x; fIm = y;         }
  TSComplex(const TSComplex &z)         { fRe = z.fRe; fIm = z.fIm; }
  virtual  ~TSComplex() {;}
  void       BetterConj(TSComplex&);
  void       C()                        { fIm = - fIm;              }
  void       CosFromSin();
  void       FromRhoPhi(const TCplxRhoPhi&);
  Double_t   Im() const                 { return fIm;               }
  Bool_t     IsComplex(Double_t c);
  Double_t   Re() const                 { return fRe;               }
  void       Print() const;
  void       PureIm()                   { fRe = 0.0;                }
  void       PureReal()                 { fIm = 0.0;                }
  void       RPhi(Double_t&, Double_t&) const;
  void       Set(Double_t x,Double_t y) { fRe = x; fIm = y;         }
  void       SetIm(Double_t y)          { fIm = y;                  }
  void       SetNeg()                   { fRe = -fRe; fIm = -fIm;   }
  void       SetReal(Double_t x)        { fRe = x;                  }
  TSComplex  operator+(const TSComplex&) const;
  TSComplex  operator+(const Double_t) const;
  TSComplex  operator-(const TSComplex&) const;
  TSComplex  operator-(const Double_t) const;
  TSComplex  operator*(const Double_t) const;
  TSComplex  operator/(const TSComplex&) const;
  TSComplex  operator/(const Double_t) const;
  TSComplex& operator+=(const TSComplex&);
  TSComplex& operator+=(const Double_t&);
  TSComplex& operator=(const TSComplex&);
  TSComplex& operator=(const Double_t);
  ClassDef(TSComplex,1)   //Implement complex numbers for Litrani
};
#endif
