// @(#)root/html:$Id: TSCYL.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSCYL
#define LITR_TSCYL
///////////////////////////////////////////////////////////////////////////
//                                                                       //
//  TSCYL  Inherits from TTUBE and add all informations asked by Litrani //
//                                                                       //
///////////////////////////////////////////////////////////////////////////
#include "TTUBE.h"
#include "TObjArray.h"

class TSupplShape;

class TSCYL : public TTUBE {

protected:

  Double_t fLr;        //Radius of the cylinder
  Double_t fLz;        //Half height of cylinder

public:

  TSupplShape *fSuppl; //! suppl. information added to each TSSHAPE

  TSCYL() { fSuppl = 0; }
  TSCYL(const char*,const char*,const char*,const char*,Double_t,Double_t,Bool_t = kFALSE);
  virtual ~TSCYL();
  void SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const;
  ClassDef(TSCYL,0) //TTUBE with more information
};
#endif
