// @(#)root/html:$Id: TStatVolD.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TLitPhys.h"
#include "TStatVolD.h"

ClassImp(TStatVolD)
//______________________________________________________________________________
//
//  Contains all the statistics and histograms for a volume detector.
//

TStatVolD::TStatVolD(const char *name,const char *title,Int_t n,Bool_t b):TNamed(name,title) {
  //
  //      Arguments
  //
  //  n      : Detector number
  //  b      : True if global [all runs] statistics. False if per run statistics
  //
  const Axis_t zero = 0.0;
  TString s,st;
  TString sn = "";
  TString sp = "";
  InitP();
  sn += n;
  fGlob = b;
  fN = n;
  fNpSeen = 0;
  fNpNotSeen = 0;
  if (fGlob) sp = "G_";
  s = "TimeSeen_VD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "time of arrival (ps) of photons accepted by vol. det. ";
  st.Append(name);
  fHTimeSeen = new TH1F(s.Data(),st.Data(),100,zero,TLitPhys::Get()->TooLate());
  s = "WvlgthSeen_VD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Wavelength (nm) of photons seen by vol. det. ";
  st.Append(name);
  fHWvlgthSeen = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "WvlgthNot_VD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Wavelength (nm) of photons not seen by vol. det. ";
  st.Append(name);
  fHWvlgthNot = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "QEff_VD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Quantum efficiency versus wavelength (nm) of vol. det. ";
  st.Append(name);
  fHQEff     = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "Inside_VD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Nb. of photons versus wavelength (nm) of vol. det. ";
  st.Append(name);
  fHInside   = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  fHEfficiency = 0;
}
TStatVolD::~TStatVolD() {
  ClearHistos();
}
void TStatVolD::ClearHistos() {
  // Clear all histograms
  if (fHTimeSeen) {
    delete fHTimeSeen;
    fHTimeSeen = 0;
  }
  if (fHWvlgthSeen) {
    delete fHWvlgthSeen;
    fHWvlgthSeen = 0;
  }
  if (fHWvlgthNot) {
    delete fHWvlgthNot;
    fHWvlgthNot = 0;
  }
  if (fHQEff) {
    delete fHQEff;
    fHQEff = 0;
  }
  if (fHInside) {
    delete fHInside;
    fHInside = 0;
  }
  if (fHEfficiency) {
    delete fHEfficiency;
    fHEfficiency = 0;
  }
}
void TStatVolD::Conclusion() {
  //Last calculations before using the class
  const Double_t un  = 1.0;
  const Int_t NbChan = 100;
  Int_t i;
  Axis_t a,num,den;
  for (i=1;i<=NbChan;i++) {
    num = fHQEff->GetBinContent(i);
    den = fHInside->GetBinContent(i);
    if (den<un) den = un;
    a   = num/den;
    fHQEff->SetBinContent(i,a);
  }
}
void TStatVolD::InitP() {
  // Pointers to 0
  fHTimeSeen   = 0;
  fHWvlgthSeen = 0;
  fHWvlgthNot  = 0;
  fHQEff       = 0;
  fHInside     = 0;
  fHEfficiency = 0;
}
