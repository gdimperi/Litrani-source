// @(#)root/html:$Id: TSBRIK.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSBRIK
#define LITR_TSBRIK
///////////////////////////////////////////////////////////////////////////
//                                                                       //
// TSBRIK  Inherits from TBRIK and add all informations asked by Litrani //
//                                                                       //
///////////////////////////////////////////////////////////////////////////
#include "TBRIK.h"
#include "TObjArray.h"

class TSupplShape;

class TSBRIK : public TBRIK {

protected:

  Double_t fLx;              //half length in x but in double precision.
  Double_t fLy;              //half length in y but in double precision.
  Double_t fLz;              //half length in z but in double precision.

public:

  TSupplShape *fSuppl;       //! suppl. information added to each TSSHAPE

  TSBRIK() { fSuppl = 0; }
  TSBRIK(const char*,const char*,const char*,const char*,Double_t,Double_t,Double_t,
    Bool_t = kFALSE);
  virtual ~TSBRIK();
  void     SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const;
  ClassDef(TSBRIK,0) //TBRIK with more information
};
#endif
