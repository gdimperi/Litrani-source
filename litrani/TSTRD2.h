// @(#)root/html:$Id: TSTRD2.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSTRD2
#define LITR_TSTRD2
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSTRD2 Inherits from TTRD2 and add all informations asked by Litrani //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TTRD2.h"
#include "TObjArray.h"

class TSupplShape;

class TSTRD2 : public TTRD2 {

protected:

  Double_t fLx1;  //half length in x, at basis at -z, in double precision.
  Double_t fLx2;  //half length in x, at basis at +z, in double precision.
  Double_t fLy1;  //half length in y, at basis at -z, in double precision.
  Double_t fLy2;  //half length in y, at basis at +z, in double precision.
  Double_t fLz;   //half length in z in double precision.

public:

  TSupplShape *fSuppl; //! suppl. information added to each TSSHAPE

  TSTRD2() { fSuppl = 0; }
  TSTRD2(const char*,const char*,const char*,const char*,Double_t,Double_t,Double_t,
    Double_t dy2, Double_t dz,Bool_t = kFALSE);
  virtual ~TSTRD2();
  void     SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay,Double_t phiy) const;
  ClassDef(TSTRD2,0) //TTRD2 with more information
};
#endif
