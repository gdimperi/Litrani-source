// @(#)root/html:$Id: TStatVolD.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TStatVolD
#define LITR_TStatVolD
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TStatVolD   Statistics and histograms for a volume detector          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TH1.h"

class TStatVolD : public TNamed {

protected:

  Bool_t   fGlob;       //True if global [all runs] statistics. False if per run statistics
  Int_t    fN;          //Detector number

  void     InitP();

public:

  Int_t    fNpSeen;      //Nb. of photons seen by this volume detector
  Int_t    fNpNotSeen;   //Nb. of photons not seen by this detector because of quantum efficiency
  TH1F    *fHTimeSeen;   //time of arrival of photons accepted by this Volume detector
  TH1F    *fHWvlgthSeen; //wave length of photons seen by this volume detector
  TH1F    *fHWvlgthNot;  //wave length of photons not seen by this volume detector
  TH1F    *fHQEff;       //Quantum efficiency of volume detector
  TH1F    *fHInside;     //Nb. of photons inside Volume detector versus wavelength

  //SUMMARY HISTOGRAMS

  TH1F    *fHEfficiency; //Efficiency as a function of run. Only if fGlob.

  TStatVolD() { InitP(); }
  TStatVolD(const char*, const char*, Int_t, Bool_t = kFALSE);
  virtual ~TStatVolD();
  void     ClearHistos();
  void     Conclusion();
  ClassDef(TStatVolD,1)//Statistics and histograms for a volume detector
};
#endif
