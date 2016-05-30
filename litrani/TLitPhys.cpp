// @(#)root/html:$Id: TLitPhys.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TGeoMedium.h"
#include "TCleanOut.h"
#include "TLitPhys.h"

TLitPhys* TLitPhys::fgLitPhys  = 0;
TRandom3* gRandom3 = 0;

ClassImp(TLitPhys)
//______________________________________________________________________________
//
//  TLitPhys groups all physical constants and global settings of Litrani or
// SLitrani. The same TLitPhys is used in both Litrani and SLitrani. The 2 methods
// GetA() and GetZ() are used only by SLitrani. It is the reason for the
// #include "TGeoMedium.h". TGeoMedium is never used in Litrani.
//  There must be only one instance of this class. Therefore, this class is
// constructed as a singleton: constructor is private and only access through
// static method Get() and Kill().
//
//  The physical constants cannot be changed.
//
//  The following settings can be changed:
//
//  Global Setting  Default  Method to change        Comments
//
//  fTooLate        100000ps SetLate(Double_t late)  Time (ns) after which photon is considered too late
//                                                   fTooLate in ps, late of SetLate in ns !
//  fThreshe        5.0      SetThreshe(Double_t t)  minimal number of e- emitted by APD to accept photon
//                                                   (see description of APD in class TLitVolume)
//  fMaxAnomal      1000     SetAnomal(Int_t k)      maximal number of abnormal photons accepted
//                                                   program killed if reached
//
TLitPhys::TLitPhys() {
  const Double_t un         = 1.0;
  const Double_t DenomAlpha = 137.0359895;

  //Physical constants

  fC         = 0.0299792458;
  fCcmps     = 29979245800.0;
  fCmks      = 2.99793e+8;
  fE         = 4.80320441e-10;
  fMe        = 0.51099891844;
  fMeg       = 9.1093826e-28;
  fHbar      = 1.0544e-34;
  fAlpha     = un/DenomAlpha;
  fEps0      = 8.859e-12;
  fAvoDivE26 = 6.025;
  fAvogadro  = 6.025e+26;
  fVphot     = 1.0e-9;
  fR02       = (fE*fE)/(fMeg*fCcmps*fCcmps);
  fR02      *= fR02;
  fR02      *= 1.0e+24;

  //Global Settings

  fTooLate     = 200000.0;
  fThreshe     = 5.0;
  fMaxAnomal   = 1000;
  fMinWaveL    = 200.0;
  fMaxWaveL    = 700.0;
  fBiggestStep = 1.0e30;
  gRandom3     = new TRandom3();
  gRandom      = gRandom3;
}
Double_t TLitPhys::ASin(Double_t cs) {
  //TMath::ASin with protections
  const Double_t un = 1.0;
  const Double_t uneps = 1.000001;
  if (cs>un) {
    cs = un;
    if (cs>uneps) gCleanOut->MM(error,"ASin","Argument of ASin bigger than 1");
  }
  if (cs<-un) {
    cs = -un;
    if (cs<-uneps) gCleanOut->MM(error,"ASin","Argument of ASin smaller than -1");
  }
  return TMath::ASin(cs);
}
Double_t TLitPhys::ACos(Double_t cs) {
  //TMath::ACos with protections
  const Double_t un = 1.0;
  const Double_t uneps = 1.000001;
  if (cs>un) {
    cs = un;
    if (cs>uneps) gCleanOut->MM(error,"ACos","Argument of ACos bigger than 1");
  }
  if (cs<-un) {
    cs = -un;
    if (cs<-uneps) gCleanOut->MM(error,"ACos","Argument of ACos smaller than -1");
  }
  return TMath::ACos(cs);
}
void TLitPhys::Book() {
  // Booking of fgLitPhys
  if (!fgLitPhys) fgLitPhys = new TLitPhys();
}
// GetA() and GetZ() only in use in SLitrani !
Double_t TLitPhys::GetA(TGeoMedium *medium) const {
  //  In case material of "medium" is an element, like "Pb", return atomic weight A
  //  In case material of "medium" is a compound, like "PbWO4",returns the sum of
  // the Ai of all elements of the compound. When one calls TGeoMixture::GetA(), it
  // is NOT the sum of the Ai which is returned, but a kind of mean <A> which is
  // returned, suitable for instance in the calculation of the radiation length,
  // but not when what one needs is the sum of all Ai.
  Double_t A=0.0;
  if (medium) {
    Int_t k,N;
    TGeoMaterial *mat;
    TGeoMixture  *mix;
    Double_t *amix;
    mat = medium->GetMaterial();
    if (mat->InheritsFrom("TGeoMixture")) {
      mix  = (TGeoMixture*)mat;
      N    = mix->GetNelements();
      amix = mix->GetAmixt();
      for (k=0;k<N;k++) A += amix[k];
    }
    else A = mat->GetA();
  }
  return A;
}
Double_t TLitPhys::GetZ(TGeoMedium *medium) const {
  //  In case material of "medium" is an element, like "Pb", return atomic number Z
  //  In case material of "medium" is a compound, like "PbWO4",returns the sum of
  // the Zi of all elements of the compound. This sum of the Zi is required in the
  // calculation of the Compton effect cross section, and unfortunately, when one
  // calls TGeoMixture::GetZ(), it is NOT the sum of the Zi which is returned, but
  // a kind of mean <Z> which is returned, suitable for instance in the calculation
  // of the radiation length, but not suitable for the calculation of the Compton
  // effect cross-section!
  Double_t Z=0.0;
  if (medium) {
    Int_t k,N;
    TGeoMaterial *mat;
    TGeoMixture  *mix;
    Double_t *zmix;
    mat = medium->GetMaterial();
    if (mat->InheritsFrom("TGeoMixture")) {
      mix  = (TGeoMixture*)mat;
      N    = mix->GetNelements();
      zmix = mix->GetZmixt();
      for (k=0;k<N;k++) Z += zmix[k];
    }
    else Z = mat->GetZ();
  }
  return Z;
}
void TLitPhys::Kill() {
  // Only way to delete fgLitPhys! This, combined with the fact that constructor
  //and destructor are private, makes of TLitPhys a singleton
  delete fgLitPhys;
  fgLitPhys = 0;
}
void TLitPhys::SetMinMaxWL(Double_t min,Double_t max) {
  // Minimum and maximum wavelength for an optical photon in nm
  fMinWaveL = min;
  fMaxWaveL = max;
}
