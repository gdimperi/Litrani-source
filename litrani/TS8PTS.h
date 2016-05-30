// @(#)root/html:$Id: TS8PTS.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TS8PTS
#define LITR_TS8PTS
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TS8PTS   general form with 8 points and 6 plane faces                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TBRIK.h"
#include "TObjArray.h"

class TSupplShape;
class T3Vector;

class TS8PTS : public TBRIK {

protected:

  T3Vector *fV0;      //point 0 of the TS8PTS
  T3Vector *fV1;      //point 1 of the TS8PTS
  T3Vector *fV2;      //point 2 of the TS8PTS
  T3Vector *fV3;      //point 3 of the TS8PTS
  T3Vector *fV4;      //point 4 of the TS8PTS
  T3Vector *fV5;      //point 5 of the TS8PTS
  T3Vector *fV6;      //point 6 of the TS8PTS
  T3Vector *fV7;      //point 7 of the TS8PTS

  void      DelPoints();
  void      InitP();
  Double_t  MakePlanar();


public:

  TSupplShape *fSuppl;       //! suppl. information added to each TSSHAPE

  TS8PTS() { InitP(); }
  TS8PTS(const char*,const char*,const char*,const char*,Double_t,
    Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
    Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
    Bool_t = kFALSE);
  virtual  ~TS8PTS();
  void      SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const;
  void      SetPoints(Float_t* buff) const;
  ClassDef(TS8PTS,0) //Most general shape of Litrani
};
#endif
