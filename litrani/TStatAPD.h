// @(#)root/html:$Id: TStatAPD.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TStatAPD
#define LITR_TStatAPD
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TStatAPD    Statistics and histograms for an APD                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TH1.h"

class TSupplShape;

class TStatAPD : public TNamed {

protected:

  Bool_t   fGlob;        //True if global [all runs] statistics. False if per run statistics
  Int_t    fN;           //APD number

  void     InitP();

public:

  Int_t    fNpSeen;      //Number of photons seen by this APD
  Int_t    fNpNotSeen;   //Photons not seen because to few e- emitted or too late
  Int_t    fNelectrons;  //Number of electrons emitted by this APD
  TH1F    *fHTimeSeen;   //time of arrival of photons accepted by this APD
  TH1F    *fHWvlgthSeen; //wave length of photons seen by this APD
  TH1F    *fHWvlgthNot;  //wave length of photons not seen by this APD
  TH1F    *fHeVersusT;   //Number of e- emitted versus time
  TH1F    *fHeVersusW;   //Number of e- emitted versus wavelength
  TH1F    *fHdCathode;   //Distance of photon stop from cathode in nm
  TH1F    *fHQEff;       //Quantum efficiency of APD versus wavelength
  TH1F    *fHInside;     //Nb. of photons inside APD versus wavelength
  TH1F    *fHEfficiency; //Efficiency as a function of run. Only if fGlob.

  TStatAPD() { InitP(); }
  TStatAPD(const char*,const char*,Int_t,Double_t, Bool_t = kFALSE);
  virtual ~TStatAPD();
  void     ClearHistos();
  void     Conclusion();
  ClassDef(TStatAPD,1) //Statistics and histograms for an APD.
};
#endif
