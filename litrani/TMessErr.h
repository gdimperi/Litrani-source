// @(#)root/html:$Id: TMessErr.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TMessErr
#define LITR_TMessErr
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMessErr  Class handling warnings or error messages  going to output //
//            and listing                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"

enum KindOfMes {Info_M,Warning_M,Error_M,kill_this,kill_prog};

class TString;
class TSComplex;
class T3Vector;

class TMessErr : public TNamed {

protected:

  char     fC;        //1st character of line: I/W/E/K/F
  Bool_t   fTestCalc; //if true, many checks to calculations

  void     Init();
  void     KillProg() const;
  void     PrintCplx(const TSComplex&) const;
  void     PrintCR() const;
  void     PrintHeader(KindOfMes,Int_t);
  void     PrintInt(Int_t) const;
  void     PrintReal(Double_t) const;
  void     PrintSpace() const;
  void     PrintStr(const TString&) const;
  void     PrintText(const char*) const;
  void     PrintVal() const;
  void     PrintVec(const T3Vector&) const;
  void     SetfC(KindOfMes);

public:

  ofstream *fListing;

  TMessErr() { Init(); }
  TMessErr(const char*, const char*, const char*);
  virtual ~TMessErr();
  void     M(KindOfMes,Int_t,const char*);
  void     W(const char*) const;
  void     M(KindOfMes,Int_t,const char*,Int_t);
  void     W(const char*,Int_t) const;
  void     M(KindOfMes,Int_t,const char*,Double_t);
  void     W(const char*,Double_t) const;
  void     M(KindOfMes,Int_t,const char*,Double_t,Double_t);
  void     W(const char*,Double_t,Double_t) const;
  void     M(KindOfMes,Int_t,const char*,const TSComplex&);
  void     W(const char*,const TSComplex&) const;
  void     M(KindOfMes,Int_t,const char*,const T3Vector&);
  void     W(const char*,const T3Vector&) const;
  void     M(KindOfMes,Int_t,const char*,Int_t,Int_t);
  void     W(const char*,Int_t,Int_t) const;
  void     M(KindOfMes,Int_t,const char*,Int_t,Int_t,Int_t);
  void     W(const char*,Int_t,Int_t,Int_t) const;
  void     M(KindOfMes,Int_t,const char*,Int_t,Double_t);
  void     W(const char*,Int_t,Double_t) const;
  void     M(KindOfMes,Int_t,const char*,const TString&);
  void     W(const char*,const TString&) const;
  void     M(KindOfMes,Int_t,const char*,const TString&,const TString&);
  void     W(const char*,const TString&,const TString&) const;
  void     SetTestCalc(Bool_t test) { fTestCalc = test; }
  Bool_t   TestCalc() const         { return fTestCalc; }
  ClassDef(TMessErr,0)   //Class handling warnings or error messages
};

R__EXTERN TMessErr *gMes;

#endif
