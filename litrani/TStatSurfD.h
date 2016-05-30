// @(#)root/html:$Id: TStatSurfD.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TStatSurfD
#define LITR_TStatSurfD
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TStatSurfD   Statistics and histograms for a surface detector        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TH1.h"

class TStatSurfD : public TNamed {

protected:

  Bool_t   fGlob;       //true if concerns global statistics
  Int_t    fN;          //Number of this surface detector
  Bool_t   fPm;         //true if surface detector is a phototube

  void     InitP();


public:

  Int_t    fNpSeen;      //Number of photons seen by this surface detector
  Int_t    fNpNotSeen;   //Number of photons lost by this detector because of quantum efficiency or angle
  TH1F    *fHTimeSeen;   //time of arrival of photons accepted by this surface detector
  TH1F    *fHWvlgthSeen; //wave length of photons seen by this volume detector
  TH1F    *fHWvlgthNot;  //wave length of photons not seen by this volume detector
  TH1F    *fHQEff;       //Quantum efficiency of surface detector
  TH1F    *fHInside;     //Nb. of photons inside surface detector versus wavelength
  TH1F    *fHAngleAcc;   //Angle of incidence of accepted photons
  TH1F    *fHAngleAll;   //Angle of incidence of all photons reaching this detector
  TH1F    *fHEfficiency; //Efficiency as a function of run. Only if fGlob.

  TStatSurfD() { InitP(); }
  TStatSurfD(const char*,const char*,Int_t,Bool_t = kFALSE,Bool_t = kFALSE);
  virtual ~TStatSurfD();
  void     ClearHistos();
  void     Conclusion();
  Bool_t   IsPhototube() const { return fPm; }
  ClassDef(TStatSurfD,1) //Statistics and histograms for a surface detector
};
#endif
