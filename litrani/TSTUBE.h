// @(#)root/html:$Id: TSTUBE.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSTUBE
#define LITR_TSTUBE
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSTUBE Inherits from TTUBE and add all informations asked by Litrani //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TTUBE.h"
#include "TObjArray.h"

class TSupplShape;

class TSTUBE : public TTUBE {

protected:

  Double_t fLmin;      //inner radius of the tube
  Double_t fLmax;      //outer radius of the tube
  Double_t fLz;        //half height of tube

public:

  TSupplShape *fSuppl; //! suppl. information added to each TSSHAPE

  TSTUBE() { fSuppl = 0; }
  TSTUBE(const char*,const char*,const char*,const char*,Double_t,Double_t,Double_t,
    Bool_t = kFALSE);
  virtual ~TSTUBE();
  void     SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const;
  ClassDef(TSTUBE,0) //TTUBE with more information
};
#endif
