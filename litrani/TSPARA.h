// @(#)root/html:$Id: TSPARA.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSPARA
#define LITR_TSPARA
///////////////////////////////////////////////////////////////////////////
//                                                                       //
// TSPARA  Inherits from TPARA and add all informations asked by Litrani //
//                                                                       //
///////////////////////////////////////////////////////////////////////////
#include "TPARA.h"
#include "TObjArray.h"

class TSupplShape;

class TSPARA : public TPARA {

protected:

  Double_t fLx;        //half length in x but in double precision.
  Double_t fLy;        //half length in y but in double precision.
  Double_t fLz;        //half length in z but in double precision.
  Double_t fLAlpha;    //angle w.r.t. the y axis from the centre of the low y to the high y. double prec.
  Double_t fLTheta;    //polar angle from the centre of the low z to the high z. double prec.
  Double_t fLPhi;      //polar angle from the centre of the low z to the high z

public:

  TSupplShape *fSuppl; //! suppl. information added to each TSSHAPE

  TSPARA() { fSuppl = 0; }
  TSPARA(const char*, const char*, const char*, const char*, Double_t, Double_t, Double_t,
    Double_t, Double_t, Double_t, Bool_t=kFALSE, Bool_t=kFALSE);
  virtual     ~TSPARA();
  void         SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const;
  ClassDef(TSPARA,0) //TPARA with more information
};
#endif
