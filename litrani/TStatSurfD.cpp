// @(#)root/html:$Id: TStatSurfD.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TLitPhys.h"
#include "TStatSurfD.h"

ClassImp(TStatSurfD)
//______________________________________________________________________________
//
//  Contains all the statistics and histograms for a surface detector,
//including phototubes. The arguments are the following :
//
//  name   : name of the detector
//  title  : title of the detector
//  n      : number of this surface detector
//  b      : true for global statistics of all runs, false for statistics
//           per run.
//  pm     : this surface detector is a phototube
//
TStatSurfD::TStatSurfD(const char *name,const char *title, Int_t n,Bool_t b,
  Bool_t pm) : TNamed(name,title) {
  //
  //    Arguments
  //
  //  name   : name of the detector
  //  title  : title of the detector
  //  n      : number of this surface detector
  //  b      : true for global statistics of all runs, false for statistics
  //           per run.
  //  pm     : this surface detector is a phototube
  //
  const Axis_t zero    = 0.0;
  const Axis_t nonante = 90.0;
  TString s,st;
  TString sn = "";
  TString ssd;
  InitP();
  fPm = pm;
  if (pm) ssd = "phototube ";
  else    ssd = "surface detector ";
  TString sp = "";
  sn += n;
  fN = n;
  fGlob = b;
  fNpSeen = 0;
  fNpNotSeen = 0;
  if (fGlob) sp = "G_";
  s = "TimeSeen_SD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "time [ps] of photons seen by ";
  st.Append(ssd);
  st.Append(name);
  fHTimeSeen = new TH1F(s.Data(),st.Data(),
    100,zero,TLitPhys::Get()->TooLate());
  s = "WvlgthSeen_SD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Wavelength [nm] of photons seen by ";
  st.Append(ssd);
  st.Append(name);
  fHWvlgthSeen = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "WvlgthNot_SD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Wavelength [nm] of photons NOT seen by ";
  st.Append(ssd);
  st.Append(name);
  fHWvlgthNot = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "QEff_SD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Quantum efficiency versus wavelength (nm) of ";
  st.Append(ssd);
  st.Append(name);
  fHQEff     = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "Inside_SD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Nb. of photons versus wavelength (nm) of ";
  st.Append(ssd);
  st.Append(name);
  fHInside   = new TH1F(s.Data(),st.Data(),
    100,TLitPhys::Get()->MinWaveL(),TLitPhys::Get()->MaxWaveL());
  s = "AngleAcc_SD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Incident angle of accepted photons of ";
  st.Append(ssd);
  st.Append(name);
  fHAngleAcc = new TH1F(s.Data(),st.Data(),100,zero,nonante);
  s = "AngleAll_SD";
  if (fGlob) s.Prepend(sp);
  s.Append(sn);
  st = "Incident angle of all photons of ";
  st.Append(ssd);
  st.Append(name);
  fHAngleAll = new TH1F(s.Data(),st.Data(),100,zero,nonante);
  fHEfficiency = 0;
}
TStatSurfD::~TStatSurfD() {
  ClearHistos();
}
void TStatSurfD::ClearHistos() {
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
  if (fHAngleAcc) {
    delete fHAngleAcc;
    fHAngleAcc = 0;
  }
  if (fHAngleAll) {
    delete fHAngleAll;
    fHAngleAll = 0;
  }
  if (fHEfficiency) {
    delete fHEfficiency;
    fHEfficiency = 0;
  }
}
void TStatSurfD::Conclusion() {
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
void TStatSurfD::InitP() {
  // Pointers to 0
  fHTimeSeen    = 0;
  fHWvlgthSeen  = 0;
  fHWvlgthNot   = 0;
  fHQEff        = 0;
  fHInside      = 0;
  fHAngleAcc    = 0;
  fHAngleAll    = 0;
  fHEfficiency  = 0;
}
