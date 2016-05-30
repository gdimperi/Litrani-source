// @(#)root/html:$Id: TSParticle.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TMath.h"
#include "TLitPhys.h"
#include "TMessErr.h"
#include "TLitGlob.h"
#include "TSParticle.h"

ClassImp(TSParticle)
//______________________________________________________________________________
//
//  IMPORTANT NOTICE
//
//  Sorry, but the name of this class has been changed from TParticle to
//TSParticle. You have to modify all your macros accordingly ! I have been
//obliged to this incompatible change because a class TParticle has appeared
//inside ROOT, which would have been in conflict with this class of Litrani.
// TSParticle  Defines particles with their momentum distribution. These
//particles will generate photons when crossing fluorescent materials.
//The momentum distribution can be :
//
// - singlevalued : particles have always the same momentum
// - straigth     : flat between fPmin and fPmax
// - gaussian     : gaussian distributed, with mean fPmean ans sigma fSig.
// - exponential  : exp(-b*p), with b = fSlope.
//
// Notice that by default particles also emit Cerenkov light. If you do not
//want Cerenkov light, look at the method TBeam::SetCerenkov().
//
TSParticle::TSParticle(const char *name,const char *title,PDistribution dist,
  Float_t Pmean,Float_t Pmin,Float_t Pmax,Float_t sig,Float_t b,Double_t mass):TNamed(name,title) {
  //Constructor, with the following arguments :
  //
  //  name    : name of the particle
  //             mandatory
  //  title   : title for the particle
  //             default ""
  //  dist    : distribution in momentum : {singlevalued,straigth,gaussian,exponential}
  //             default : singlevalued  
  //  Pmean   : mean momentum in Gev, valid for {singlevalued,gaussian}
  //             default : 1.0
  //  Pmin    : minimum momentum, valid for {straigth,gaussian,exponential}
  //             default : 1.0
  //  Pmax    : maximum momentum, valid for {straigth,gaussian,exponential}
  //             default : 10.0
  //  sig     : standard deviation of distribution, valid for {gaussian}
  //             default : 0.5
  //  b       : slope of exponential distribution in [Gev]-1
  //             default : 1.0
  //  mass    : mass of the particle in Gev
  //             default : mass of the muon
  //
  Float_t Exp2;    //2nd constant in case {exponential}
  fMass  = mass;
  fPdist = dist;
  fPmean = Pmean;
  fPmin  = Pmin;
  fPmax  = Pmax;
  fSig   = sig;
  fSlope = b;
  fExp1  = TMath::Exp(-fSlope*fPmin);
  Exp2   = TMath::Exp(-fSlope*fPmax);
  fD     = fExp1 - Exp2;
  fCerNphot = 0;
  AddToList(name);
}
TSParticle::~TSParticle() {
  gLit->fParticle.Remove(this);
  gLit->fNParticle--;
}
Bool_t TSParticle::AddToList() const {
  //record the new Particle into gLit->fParticle
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TSParticle");
    gMes->M(kill_prog,1,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneParticle(this,fName);
}
Bool_t TSParticle::AddToList(const char *name) const {
  //record the new Particle into gLit->fParticle
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TSParticle");
    gMes->M(kill_prog,2,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneParticle(this,name);
}
void TSParticle::Cerenkov(Double_t n,Double_t thickness) {
  //  Providing index of refraction n and thickness of material in [cm] in
  //the call to Cerenkov(), you will fix all parameters affecting
  //the generation of Cerenkov light ( theta Cerenkov and number of Cerenkov
  //photons emitted ). As soon as you have called Cerenkov once, the
  //particle is able to emit Cerenkov light.
  //
  const Double_t un     = 1.0;
  const Double_t nmtocm = 1.0e-7;
  Double_t WaveLmin,WaveLmax; //min and max wavelengths used in [cm]
  Double_t fac;
  fCerCos   = un/(fBeta*n);
  if (fCerCos<un) {
    fCerSin   = TMath::Sqrt(un - fCerCos*fCerCos);
    WaveLmin  = TLitPhys::Get()->MinWaveL()*nmtocm;
    WaveLmax  = TLitPhys::Get()->MaxWaveL()*nmtocm;
    fac       = thickness*(un/WaveLmin - un/WaveLmax);
    fCerNphot = Int_t(TMath::TwoPi()*TLitPhys::Get()->Alpha()*fac*fCerSin*fCerSin);
  }
  else {
    fCerNphot = 0;
  }
}
Double_t TSParticle::GenP() {
  //Generates the momentum of the particle and returns its speed in [cm/ps]
  Float_t y;
  switch (fPdist) {
  case singlevalued:
    fP = fPmean;
    break;
  case straigth:
    y = gRandom3->Rndm();
    fP = fPmin + (fPmax-fPmin)*y;
    break;
  case gaussian:
    fP = gRandom3->Gaus(fPmean,fSig);
    break;
  case exponential:
    y = gRandom3->Rndm();
    fP = - TMath::Log(fExp1 - y*fD)/fSlope;
    break;
  }
  fE = TMath::Sqrt(fP*fP + fMass*fMass);
  fBeta  = fP/fE;
  fSpeed = fBeta*TLitPhys::Get()->C();
  return fSpeed;
}
void TSParticle::Print() const {
  //Prints everything about particle
  //
  cout << endl;
  cout << "    Particle       : " << fName.Data() << endl;
  cout << "    Title          : " << fTitle.Data() << endl;
  cout << endl;
  cout << "    mass           : " << fMass << endl;
  cout << "Type of momentum distribution  : ";
  switch (fPdist) {
    case singlevalued:
      cout << "singlevalued";
      break;
    case straigth:
      cout << "straigth";
      break;
    case gaussian:
      cout << "gaussian";
      break;
    case exponential:
      cout << "exponential";
      break;
  }
  cout << endl;
  cout << "mean momentum [Gev]: " << fPmean << endl;
  cout << "min. momentum [Gev]: " << fPmin  << endl;
  cout << "max. momentum [Gev]: " << fPmax  << endl;
  cout << "sigma of gaussian  : " << fSig   << endl;
  cout << "slope of exp.      : " << fSlope << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "    Particle       : " << fName.Data() << endl;
  *gMes->fListing << "    Title          : " << fTitle.Data() << endl;
  *gMes->fListing << endl;
  *gMes->fListing << "    mass           : " << fMass << endl;
  *gMes->fListing << "Type of momentum distribution  : ";
  switch (fPdist) {
    case singlevalued:
      *gMes->fListing << "singlevalued";
      break;
    case straigth:
      *gMes->fListing << "straigth";
      break;
    case gaussian:
      *gMes->fListing << "gaussian";
      break;
    case exponential:
      *gMes->fListing << "exponential";
      break;
  }
  *gMes->fListing << endl;
  *gMes->fListing << "mean momentum [Gev]: " << fPmean << endl;
  *gMes->fListing << "min. momentum [Gev]: " << fPmin  << endl;
  *gMes->fListing << "max. momentum [Gev]: " << fPmax  << endl;
  *gMes->fListing << "sigma of gaussian  : " << fSig   << endl;
  *gMes->fListing << "slope of exp.      : " << fSlope << endl;
}
