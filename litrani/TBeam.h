// @(#)root/html:$Id: TBeam.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TBeam
#define LITR_TBeam
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TBeam    Generation of a beam of particles                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TString.h"
#include "TSplineFit.h"
#include "T3Vector.h"
#include "TPhotonCradle.h"


class TOpticMaterial;
class TSupplShape;
class TSNode;
class TPhoton;
class TSParticle;

class TBeam : public TNamed {

protected:

  Int_t           fRun;           //run number
  TString         fFullName;      //name + run number. Name of histos file created
  Bool_t          fRunStat;       //ask for separate statistics and histograms for this run
  TSParticle     *fParticle;      //particle of the beam, with energy distribution
  Double_t        fStep;          //step along particle trajectory to emit light.
  Bool_t          fEmitCerenkov;  //if true (default) particle will also emit Cerenkov light !
  //characteristics of the beam cradle
  TSupplShape    *fSuppl;         //TSupplShape with respect to which the beam cradle is defined
  TSNode         *fSNode;         //node of fSuppl.
  KindOfCradle    fKindCdl;       //does the beam originates {point,disk,ellipse,parallelogram} ?
  T3Vector        fCenter;        //center of the cradle in WCS
  Double_t        fA1;            //radius of disk or half-length of 1st axis of {ellipse,parallelogram}
  Double_t        fA2;            //half-length of 2nd axis of {ellipse,parallelogram}
  T3Vector        fAxis1;         //Direction of 1st axis of {ellipse,parallelogram}
  T3Vector        fAxis2;         //2nd axis of parallelogram or parallelipiped
  T3Vector        fAxis3;         //normal to {disk,ellipse,parallelogram}
  //characteristics of the beam of photons
  T3Vector        fAxisb1;        //complement fAxisb3 to have full coordinate system
  T3Vector        fAxisb2;        //complement fAxisb3 to have full coordinate system
  T3Vector        fAxisb3;        //axis of the beam in WCS (not necessary parallel to fAxis3 !)
  Double_t        fAperture;      //aperture of the beam around fAxisb3 in radian (theta max)
  Double_t        fApercos;       //cosine of fAperture
  Double_t        fApersin;       //sine of fAperture
  KindOfDist      fKindDist;      //kind of theta distribution for the beam of particles
  TString         fDistName;      //name of the given distribution for theta of the beam
  TSplineFit     *fFitDist;       //pointer to the fit for theta around fAxisb3.
  //Results for one particle generated
  T3Vector        fX;             //starting coordinates of particle in WCS
  T3Vector        fP;             //direction of particle in WCS
  Double_t        fTheta;         //generated theta for the emission of the particle or of the photon
  Double_t        fTime;          //time taken by particle to go from fX to fX0
  Double_t        fSpeed;         //speed of particle in [cm/ps]
  Double_t        fMomentum;      //momentum of particle in [Gev/c]
  Double_t        fEnergy;        //energy of particle in [Gev]
  TSupplShape    *fSup;           //shape in which particle is
  TOpticMaterial *fMat;           //material in which particle is
  Double_t        fnCerenkov;     //index of refraction to be used in Cerenkov formulae
  //Results for one photon generated
  TPhoton        *fPhot;          //pointer to generated photons
  Double_t        fWvlgth;        //wavelength of this photon in [nm]
  T3Vector        fX0;            //starting point of photon;
  Double_t        fT0;            //time of emission of this photon in [ps]

  Bool_t    AddToList() const;
  Bool_t    AddToList(const char*) const;
  void      AxisBeam();
  void      DefaultInit(T3Vector&);
  void      EmitLight(Double_t&,T3Vector&,Double_t&,T3Vector&);
  void      FindDist(const char*);
  void      FindShape(TString&);
  void      Gen4pi(Double_t&);
  void      GenE(const T3Vector&,T3Vector&) const;
  void      GenFlat(Double_t&);
  void      GenK(T3Vector&);
  void      GenKECer(T3Vector&,T3Vector&) const;
  void      GenP();
  void      GenProvided(Double_t&);
  void      GenSinCos(Double_t&);
  void      GenX();
  void      InitP();
  void      NameFile();
  void      OutsideOk() const;
  void      StartOneParticle();
  void      WandLCerenkov(Double_t&,Double_t&) const;

public:

  TBeam() { InitP(); }
  TBeam(const char*,const char*,TSParticle*,const char*, T3Vector&);
  TBeam(const char*,const char*,TSParticle*,const char*,T3Vector&,KindOfCradle,
    Double_t,Double_t=0,Double_t=180,Double_t=0,Double_t=90,Double_t=180);
  TBeam(const char*,const char*,TSParticle*,T3Vector&,const char*,KindOfCradle,
    Double_t,Double_t=0,
    Double_t=90,Double_t=180,Double_t=90,Double_t=90);
  virtual  ~TBeam();
  void      Beam(KindOfDist,Double_t,Double_t,Double_t,const char *distrib="");
  void      Beam(KindOfDist,Double_t,const char *distrib="");
  void      Gen(Int_t, Int_t = 1, Double_t = -2.0e+20, Bool_t = kTRUE,
    Bool_t = kFALSE, Double_t = 0.5);
  T3Vector &GetCenter() { return fCenter; }
  void      Print() const;
  void      SetCerenkov(Bool_t);
  ClassDef(TBeam,0) //Defines a beam of particles
};
#endif
