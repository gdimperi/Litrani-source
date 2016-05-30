// @(#)root/html:$Id: TSCONE.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSCONE
#define LITR_TSCONE
////////////////////////////////////////////////////////////////////////////
//                                                                        //
// TSCONE   Inherits from TCONE and add all informations asked by Litrani //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
#include "TCONE.h"
#include "TObjArray.h"

class TSupplShape;

class TSCONE : public TCONE {

protected:

  Double_t fLRlow;           //radius of the lower (-dz) base of the truncated cone
  Double_t fLRup;            //radius of the upper (+dz) base of the truncated cone
  Double_t fLz;              //half height of truncated cone

public:

  TSupplShape *fSuppl;       //! suppl. information added to each TSSHAPE

  TSCONE() { fSuppl = 0; }
  TSCONE(const char*,const char*,const char*,const char*,Double_t,Double_t,Double_t,Bool_t = kFALSE);
  virtual ~TSCONE();
  void     SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const;
  ClassDef(TSCONE,0) //TCONE with more information
};
#endif
