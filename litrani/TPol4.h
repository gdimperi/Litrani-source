// @(#)root/html:$Id: TPol4.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TPol4
#define LITR_TPol4
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPol4         z^4 + fA3*z^3 + fA2*z^2 + fA1*z + fA0 == 0             //
//                                                                      //
// Solution of polynom of 4th order with real coefficients by Super-    //
//Newton                                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TSComplex.h"
#include "TPol3.h"

class TPol4 : public TObject {

protected:

  Bool_t    fOk;      //if true, a good solution is available
  Double_t  fLimConv; //convergence criterium
  Double_t  fA3;      //coefficient of z^3 of pol4
  Double_t  fA2;      //coefficient of z^2 of pol4
  Double_t  fA1;      //coefficient of z of pol4
  Double_t  fA0;      //constant term of pol4
  Double_t  fB2;      //coefficient of z^2 of pol3
  Double_t  fB1;      //coefficient of z of pol3
  Double_t  fB0;      //constant term of pol3
  TSComplex fZ0;      //1st solution
  TSComplex fZ1;      //2nd solution
  TSComplex fZ2;      //3rd solution
  TSComplex fZ3;      //4th solution
  Double_t  fEps0;    //quality of solution 0
  Double_t  fEps1;    //quality of solution 1
  Double_t  fEps2;    //quality of solution 2
  Double_t  fEps3;    //quality of solution 3
  Bool_t    fC0;      //true if fZ0 not real
  Bool_t    fC1;      //true if fZ0 not real
  Bool_t    fC2;      //true if fZ0 not real
  Bool_t    fC3;      //true if fZ0 not real
  TPol3    *fP3;      //pointer to TPol3

  TSComplex F0(TSComplex) const;
  TSComplex F1(TSComplex) const;
  TSComplex F2(TSComplex) const;
  Double_t  FR0(Double_t) const;
  Double_t  FR1(Double_t) const;
  Double_t  FR2(Double_t) const;
  void      LastSolutions();
  Double_t  RSolution(Double_t,Bool_t&,Short_t);
  Double_t  RStep4(Double_t,Bool_t&) const;
  Double_t  SecondSolution(Bool_t&);
  Bool_t    SolDeriv(Double_t&,Double_t&);
  Double_t  Solution(TSComplex,Bool_t&,Short_t);
  TSComplex Step4(TSComplex,Bool_t&) const;

public:

  TPol4() { fP3 = 0; }
  TPol4(Double_t);
  TPol4(Double_t,Double_t,Double_t,Double_t);
  virtual  ~TPol4() { if (fP3) delete fP3; }
  Bool_t    Get(TSComplex&,TSComplex&,TSComplex&,TSComplex&,
    Bool_t&,Bool_t&,Bool_t&,Bool_t&) const;
  void      Set(Double_t,Double_t,Double_t,Double_t);
  void      SetLimConv(Double_t);
  Bool_t    Solve(Double_t&);
  ClassDef(TPol4,1)   //Solution of polynom of 4th order with real coeff.
};
#endif
