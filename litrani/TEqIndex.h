// @(#)root/html:$Id: TEqIndex.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TEqIndex
#define LITR_TEqIndex
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TEqIndex       handles the Equation giving the 2 indices in case of  //
//                anisotropy :                                          //
//                                                                      //
//                z^2  + a*z + (b*z+c)*Sqrt(z-g) + d == 0     (E1)      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TGraph.h"
#include "TH1.h"
#include "TSComplex.h"
#include "TPol4.h"

enum TypeOfEquation {bczero,degenerate,general};

class TEqIndex : public TObject {

protected:

  TString   fName;            //Stores the name of the class creating it
  Double_t  a,b,c,d,g;        //coefficients of the equation (E1)
  Double_t  fGd;              //determinant for SolveSimple
  Bool_t    fDebug;           //if true, study the residues of guess
  TPol4    *fP4;              //towards solution of 4th order polynom
  Double_t  fA,fB,fC,fD;      //coefficients of equation (E1) squared
  Double_t  fZpa3,fZpa2,fZpa1,fZpa0; //for solution near g
  Bool_t    fSolved;          //Solve() has been called
  Short_t   fNr;              //Number of solutions which are real
  TSComplex fRac;             //the value of Sqrt(z-g)
  TypeOfEquation fTypeEq;     //true if b and c zero
  Double_t  fEnongen;         //error in case of fTypeEq != general

  TSComplex fY0[3],fY1[3],fY2[3],fY3[3]; //The 3 families of solutions
  Bool_t    fC0[3],fC1[3],fC2[3],fC3[3];
  Bool_t    fS0[3],fS1[3],fS2[3],fS3[3];
  Double_t  fWs0[3],fWs1[3],fWs2[3],fWs3[3];
  Double_t  fWj0[3],fWj1[3],fWj2[3],fWj3[3];

  TSComplex fZc[4];           //the family under evaluation
  Bool_t    fBc[4];           //the sign of Sqrt(z-g) for each solution
  Bool_t    fRc[4];           //whether solution is real or complex
  Bool_t    fGok[4];          //if fZc = g is a good solution
  Double_t  fQsc[4];          //the quality of the solution
  Double_t  fQjc[4];          //the quality of complex conjugation
  TGraph   *fGraphP;          //! Graph of eq. (E1) with + sign for Sqrt(z-g)
  TGraph   *fGraphM;          //! Graph of eq. (E1) with - sign for Sqrt(z-g)
  TH1F     *fResidus0;        //quality of first guess, method 0 (Solve°
  TH1F     *fResidus1;        //quality of first guess, method Pol4 (Solve1)
  Int_t     fNbin;            //Number of bins for histogram fHequx
  Axis_t    fXlow;            //low x in histo
  Axis_t    fXup;             //upper x in histo

  TSComplex fZ0,fZ1,fZ2,fZ3;  //the 4 solutions
  TSComplex fD0,fD1,fD2,fD3;  //the 4 associated cosines
  Bool_t    fB0,fB1,fB2,fB3;  //the sign of Sqrt(z-g) for each solution
  Bool_t    fR0,fR1,fR2,fR3;  //if kTRUE solution fZi is complex
  Double_t  fQs0,fQs1;        //the quality of each solution
  Double_t  fQs2,fQs3;        //the quality of each solution
  Double_t  fQj0,fQj1;        //the quality of complex conjugation
  Double_t  fQj2,fQj3;        //the quality of complex conjugation

  void      Better(TSComplex&,Bool_t&,Double_t&);
  void      Exchange(Short_t,Short_t);
  TSComplex F0(const TSComplex&) const;   //F0 = z^2  + a*z + (b*z+c)*Sqrt(z-g) + d
  TSComplex F40(const TSComplex&) const;  //F40 = z^4 + fA*z^3 + fB*z^2 + fC*z + fD
  TSComplex F4p(const TSComplex&) const;  //first derivative of F4
  TSComplex F4s(const TSComplex&) const;  //second derivative of F4
  void      FillResidu(Double_t,Bool_t);
  TSComplex Fp(const TSComplex&) const;   //first derivative of F
  TSComplex Fs(const TSComplex&) const;   //second derivative of F
  TSComplex FZp0(const TSComplex&) const; //for solution near g
  TSComplex FZp1(const TSComplex&) const; //for solution near g
  TSComplex FZp2(const TSComplex&) const; //for solution near g
  TSComplex FZpIter(const TSComplex&) const;
  Bool_t    FZpSol(TSComplex&,Bool_t&,Double_t&);
  Short_t   Identical(Bool_t,Bool_t&,Bool_t*);
  Short_t   IdenticalF(Bool_t*) const;
  void      Ini4();
  void      InifZ();
  void      Init();
  void      InitP();
  void      Order1();
  Double_t  Order2();
  void      Order3();
  void      PutIn();
  void      PutOut();
  TSComplex Rac(const TSComplex&,Bool_t) const;
  TSComplex Solve(Int_t);
  Bool_t    Solve1();
  Bool_t    SolveDegen();
  Double_t  SolveSimple();
  void      StepSN(TSComplex&,Bool_t,Double_t = 1.0);
  void      StepSN4(TSComplex&) const;
  Bool_t    SuperNewton(TSComplex&,Bool_t,Double_t&);
  Bool_t    SuperNewt4(TSComplex&,Double_t&) const;
  void      Troisieme(const Double_t&,Double_t,Double_t,Double_t) const;
  void      WhichIsReal(Double_t,Bool_t);

public:



  Double_t  f0Chosen;        //how many times family 0 was chosen.
  Double_t  f1Chosen;        //how many times family 1 was chosen.
  Double_t  f2Chosen;        //how many times family 2 was chosen.


  TEqIndex() { InitP(); }
  TEqIndex(const char*);
  TEqIndex(const char*,Int_t i);
  TEqIndex(const char*, Double_t,Double_t,Double_t,Double_t,Double_t);
  virtual  ~TEqIndex();
  Short_t   ClosestFrom(Double_t,Bool_t);
  void      Difficult(Short_t i);
  Bool_t    GetfB(Short_t) const;
  TSComplex GetfD(Short_t) const;
  Bool_t    GetfR(Short_t) const;
  TSComplex GetfZ(Short_t) const;
  void      PrintSolution() const;
  void      Set(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t=-1.0);
  void      SetAxis(Int_t,Axis_t,Axis_t);
  void      SetDebug();
  void      ShowEqu(Int_t=1000,Double_t=0.0,Double_t=-1.0,Double_t=0.0,Double_t=-1.0);
  void      ShowResidus() const;
  Double_t  Solve(Short_t, Bool_t);
  TSComplex Y(TSComplex,Bool_t=kTRUE) const;
  TSComplex Y(Double_t,Bool_t=kTRUE) const;
  TSComplex Y(Short_t,TSComplex,Bool_t=kTRUE) const;
  TSComplex Y(Short_t,Double_t,Bool_t=kTRUE) const;
  Double_t  YAbs(TSComplex,Bool_t=kTRUE) const;
  Double_t  YAbs(Double_t,Bool_t=kTRUE) const;
  ClassDef(TEqIndex,0) //Equation giving the 2 indices in case of anisotropy
};
R__EXTERN TEqIndex *gIdx;
#endif
