// @(#)root/html:$Id: TPol3.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TPol3
#define LITR_TPol3
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPol3         z^3 + fA2*z^2 + fA1*z + fA0 == 0                       //
//                                                                      //
// Solution of polynom of 3trd order with real coefficients             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TSComplex.h"

class TPol3 : public TObject {

protected:

  Double_t  fA2;      //coefficient of z^2 of pol3
  Double_t  fA1;      //coefficient of z of pol3
  Double_t  fA0;      //constant term of pol3
  Double_t  fLimConv; //convergence criterium
  TSComplex fZ1;      //1st solution
  TSComplex fZ2;      //2nd solution
  TSComplex fZ3;      //3rd solution

  TSComplex F0(TSComplex) const;
  Double_t  RF0(Double_t) const;
  Double_t  RF1(Double_t) const;
  Double_t  RF2(Double_t) const;
  Double_t  Step3(Double_t,Bool_t&) const;

public:

  TPol3();
  TPol3(Double_t,Double_t,Double_t);
  Double_t  MakeBetter(Double_t&,Bool_t&) const;
  void      Set(Double_t,Double_t,Double_t);
  void      SetLimConv(Double_t s) { fLimConv = s; }
  Bool_t    Solve(Double_t&,Double_t&,Double_t&,Double_t&);
  ClassDef(TPol3,1)   //Solution of polynom of 3rd order with real coeff.
};
#endif
