// @(#)root/html:$Id: TPhotonCradle.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TPhotonCradle
#define LITR_TPhotonCradle
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TPhotonCradle    Determines position, direction and polarization   //
//                    of the photon to be generated                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TString.h"
#include "TSplineFit.h"

enum KindOfCradle  {point,disk,ellipse,parallelogram,parallelipiped};

class TSupplShape;
class TSNode;

class TPhotonCradle {

protected:

  //characteristics of the cradle
  TSupplShape *fSuppl;     //TSupplShape in which the cradle of photons is.
  TSNode      *fSNode;     //node of fSuppl.
  KindOfCradle fKindCdl;   //does the photon originates from {point,disk,ellipse,parallelogram,parallelipiped} ?
  T3Vector     fCenter;    //center of the cradle in WCS
  Bool_t       fFromFace;  //true if the beam cradle is a face of the shape
  Double_t     fA1;        //radius of disk or half-length of 1st axis of {ellipse,parallelogram,parallelipiped}
  Double_t     fA2;        //half-length of 2nd axis of {ellipse,parallelogram,parallelipiped}
  Double_t     fA3;        //half-length of 3rd axis of {parallelipiped}
  T3Vector     fAxis1;     //direction of 1st axis of {ellipse,parallelogram,parallelipiped}
  T3Vector     fAxis2;     //direction of 2nd axis of {ellipse,parallelogram,parallelipiped}
  T3Vector     fAxis3;     //normal to {disk,ellipse,parallelogram} or 3rd axis of parallelipiped
  Short_t      fAni;       //anisotropic characteristics of material of fSuppl
  //characteristics of the beam of photons
  T3Vector     fAxisb1;    //complement fAxisb3 to have full coordinate system
  T3Vector     fAxisb2;    //complement fAxisb3 to have full coordinate system
  T3Vector     fAxisb3;    //axis of the beam in WCS
  Double_t     fAperture;  //aperture of the beam around fAxisb3 in radian (theta max)
  Double_t     fApercos;   //cosine of fAperture
  Double_t     fApersin;   //sine of fAperture
  KindOfDist   fKindDist;  //kind of theta distribution for the beam of photons
  TString      fDistName;  //name of the given distribution for theta
  TSplineFit  *fFitDist;   //pointer to the fit for theta around fAxisb3.
  Bool_t       fPolarized; //true if the beam of photons is polarized
  T3Vector     fPolar;     //direction of polarization for the E field
  //Results for one photon generated
  T3Vector     fX;         //point where the photon is generated
  T3Vector     fK;         //k vector generated
  T3Vector     fE;         //E field generated

  void      AxisBeam();
  void      FindDist(const char*);
  void      Gen4pi(Double_t&,Double_t&) const;
  void      GenE();
  void      GenFlat(Double_t&,Double_t&) const;
  void      GenK();
  void      GenProvided(Double_t&,Double_t&) const;
  void      GenSinCos(Double_t&,Double_t&) const;
  void      GenX();
  void      InitP();
  Bool_t    IsInside() const;
  void      SetPolar(Double_t,Double_t);

public:

  TPhotonCradle() { InitP(); }
  TPhotonCradle(TSupplShape*,const T3Vector&,Bool_t = kFALSE);
  TPhotonCradle(TSupplShape*,Int_t);
  TPhotonCradle(TSupplShape*,const T3Vector&,KindOfCradle,
    Double_t=1.0,Double_t=1.0,Double_t=1.0,
    Double_t=180,Double_t=0,Double_t=90,Double_t=180,Bool_t = kFALSE);
  TPhotonCradle(const T3Vector&,TSupplShape*,KindOfCradle,
    Double_t=1.0,Double_t=1.0,Double_t=1.0,
    Double_t=90,Double_t=180,Double_t=90,Double_t=90,Double_t=180,Double_t=0,Bool_t = kFALSE);
  virtual  ~TPhotonCradle() {;}
  void      Beam(KindOfDist,Double_t,Double_t,Double_t,const char *distrib="",
    Bool_t=kFALSE,Double_t=90,Double_t=0);
  void      Beam(KindOfDist,Double_t,const char *distrib="",
    Bool_t=kFALSE,Double_t=90,Double_t=0);
  void      DefaultInit(TSupplShape*,const T3Vector&,Bool_t = kFALSE);
  void      Generate(T3Vector&,T3Vector&,T3Vector&);
  T3Vector &GetCenter() { return fCenter; }
  ClassDef(TPhotonCradle,1) //Determines position, direction and polarization
};
#endif
