// @(#)root/html:$Id: TStatAPD.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TLitPhys.h"
#include "TStatAPD.h"

ClassImp(TStatAPD)
//______________________________________________________________________________
//
//  Contains all the statistics and histograms for an APD.
//

TStatAPD::TStatAPD(const char *name,const char *title,Int_t n,Double_t distCA,
  Bool_t b) : TNamed(name,title) {
  // Constructor.
  //  n      : APD number
  //  distCA : distance between cathode and anode
  //  b      : true if global [all runs] statistics
  //           false if per run statistics
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
  fNelectrons = 0;
  if (fGlob) sp = "G_";
  s = "TimeSeen_APD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "time of arrival (ps) of photons accepted by APD ";
  st.Append(name);
  fHTimeSeen = new TH1F(s.Data(),st.Data(),100,zero,TLitPhys::Get()->TooLate());
  s = "WvlgthSeen_APD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Wavelength (nm) of photons seen by APD ";
  st.Append(name);
  fHWvlgthSeen = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "WvlgthNot_APD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Wavelength (nm) of photons not seen by APD ";
  st.Append(name);
  fHWvlgthNot = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "eVersusT_APD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Number of e- emitted versus time for APD ";
  st.Append(name);
  fHeVersusT = new TH1F(s.Data(),st.Data(),
    100,zero,TLitPhys::Get()->TooLate());
  s = "eVersusW_APD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Number of e- emitted versus wavelength (nm) for APD ";
  st.Append(name);
  fHeVersusW = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "dCathode_APD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Distance of photon stop from cathode in nm for APD ";
  st.Append(name);
  fHdCathode = new TH1F(s.Data(),st.Data(),100,zero,distCA);
  s = "QEff_APD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Quantum efficiency versus Wavelength (nm) of APD ";
  st.Append(name);
  fHQEff     = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "Inside_APD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Nb. of photons versus wavelength (nm) of APD ";
  st.Append(name);
  fHInside   = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  fHEfficiency = 0;
}
TStatAPD::~TStatAPD() {
  ClearHistos();
}
void TStatAPD::ClearHistos() {
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
  if (fHeVersusT) {
    delete fHeVersusT;
    fHeVersusT = 0;
  }
  if (fHeVersusW) {
    delete fHeVersusW;
    fHeVersusW = 0;
  }
  if (fHdCathode) {
    delete fHdCathode;
    fHdCathode = 0;
  }
  if (fHQEff) {
    delete fHQEff;
    fHQEff = 0;
  }
  if (fHInside) {
    delete fHInside;
    fHInside = 0;
  }
  if (fHEfficiency)	{
    delete fHEfficiency;
    fHEfficiency = 0;
  }
}
void TStatAPD::Conclusion() {
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
void TStatAPD::InitP() {
  // Initializes pointers to 0
  fHTimeSeen   = 0;
  fHWvlgthSeen = 0;
  fHWvlgthNot  = 0;
  fHeVersusT   = 0;
  fHeVersusW   = 0;
  fHdCathode   = 0;
  fHQEff       = 0;
  fHInside     = 0;
  fHEfficiency = 0;
}
