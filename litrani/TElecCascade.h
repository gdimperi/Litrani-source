// @(#)root/html:$Id: TElecCascade.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TElecCascade
#define LITR_TElecCascade
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TElecCascade     Generation of electromagnetic showers               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TString.h"
#include "T3Vector.h"

class TOpticMaterial;
class TSupplShape;
class TSNode;
class TPhoton;
class TPhotonCradle;

class TElecCascade : public TNamed {

protected:

  Int_t           fRun;      //run number
  Int_t           fNphot;    //photon number
  TPhoton        *fPhot;     //pointer to generated photons
  TString         fFullName; //name + run number. Name of histos file created
  Double_t        fWvlgth;   //value of wavelength
  Double_t        fT0;       //start time of photon in picoseconds
  TPhotonCradle  *fBeam;     //!all characteristics of the beam of photons
  Double_t        fE0;       //Energy of the cascade in Mev
  Double_t        fCj;       //parameter Cj "Review of Particle Physics" (23.26) p 148
  Double_t        fA;        //parameter  a "Review of Particle Physics" (23.25) p 148
  Double_t        fB;        //parameter  b "Review of Particle Physics" (23.25) p 148
  Double_t        fFacdEdz;  //factor in front of (23.25)
  Double_t        fFacdEdr;  //factor in front of (23.27)
  Double_t        fK;        //parameter  R "Review of Particle Physics" (23.27) p 148
  Double_t        fZmax;     //maximal length of cascade in cm. Beyond no energy deposit
  Double_t        fRmax;     //maximal radius of cascade in cm. Beyond no energy deposit
  T3Vector        fStart;    //starting point of the cascade, in LCS of fRefShape
  T3Vector        fZaxis;    //axis of the cascade, pointing from start to end
  T3Vector        fXaxis;    //complement fZaxis to full coordinnate system
  T3Vector        fYaxis;    //complement fZaxis to full coordinnate system
  TOpticMaterial *fMat;      //material in which cascade develops itself
  Double_t        fX0;       //radiation length of material fMat
  Double_t        fRm;       //Moliere's radius of material fMat
  Double_t        fEc;       //critical energy of material fMat
  TSNode         *fSNode;    //node to which fRefShape belongs
  TSupplShape    *fRefShape; //Reference shape for positionning the cascade
  Double_t        fPhotFac;  //Factor to diminish the number of produced photons
  Double_t        fEdeposit; //energy deposited in shape
  Double_t        fVdeposit; //Volume into which energy deposited
  Double_t        fX;        //x coord. of volume element in WCS
  Double_t        fY;        //y coord. of volume element in WCS
  Double_t        fZ;        //z coord. of volume element in WCS
  Double_t        fZcasc;    //z coord. of volume element in cascade coord. system
  Double_t        fRcasc;    //r coord. of volume element in cascade coord. system
  Double_t        fFcasc;    //phi coord. of volume element in cascade coord. system
  Double_t        fDStepL;   //longitudinal step increment
  Double_t        fDStepR;   //radial step increment
  Double_t        fDStepF;   //angular step increment
  Int_t           fNStepL;   //number of longitudinal steps
  Int_t           fNStepR;   //number of radial steps
  Int_t           fNStepF;   //number of steps in phi
  Int_t           fIStepL;   //actual longitudinal step
  Int_t           fIStepR;   //actual radial step
  Int_t           fIStepF;   //actual step in phi

  void     CascCoordSys();
  void     EmitLight(Double_t,TSupplShape*);
  void     FirstStep();
  void     InitP();
  void     NameFile();
  Bool_t   NextStep();
  void     OutsideOk() const;


public:

  static Double_t fZmaxLR;  //maximal length of cascade in rad. length. Beyond no energy deposit
  static Double_t fRmaxMR;  //maximal radius of cascade in Moliere radius. Beyond no energy deposit

  TElecCascade() { InitP(); }
  TElecCascade(const char*, const char*, Double_t, T3Vector, T3Vector, const char*,
    const char*, Bool_t=kTRUE, Double_t=0.05, Int_t=100, Int_t=35,
    Int_t=120);
  virtual ~TElecCascade();
  void     CoordStep();
  Double_t DeltaE(Double_t&, Double_t&) const;
  void     Gen(Int_t,Double_t = -2.0e+20,Bool_t = kTRUE,Bool_t = kFALSE);
  ClassDef(TElecCascade,0) //Generation of electromagnetic showers
};
#endif
