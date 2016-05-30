// @(#)root/html:$Id: TSTRD1.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSTRD1
#define LITR_TSTRD1
///////////////////////////////////////////////////////////////////////////
//                                                                       //
// TSTRD1  Inherits from TTRD1 and add all informations asked by Litrani //
//                                                                       //
///////////////////////////////////////////////////////////////////////////
#include "TTRD1.h"
#include "TObjArray.h"

class TSupplShape;

class TSTRD1 : public TTRD1 {

protected:

  Double_t fLx1;       //half length in x, at basis at -z, in double precision.
  Double_t fLx2;       //half length in x, at basis at +z, in double precision.
  Double_t fLy;        //half length in y in double precision.
  Double_t fLz;        //half length in z in double precision.

public:

  TSupplShape *fSuppl; //! suppl. information added to each TSSHAPE

  TSTRD1() { fSuppl = 0; }
  TSTRD1(const char*,const char*,const char*,const char*,Double_t,Double_t,Double_t,
    Double_t dz,Bool_t = kFALSE);
  virtual ~TSTRD1();
  void     SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const;
  ClassDef(TSTRD1,0) //TTRD1 with more information
};
#endif
