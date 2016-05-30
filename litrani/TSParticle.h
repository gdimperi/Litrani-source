// @(#)root/html:$Id: TSParticle.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSParticle
#define LITR_TSParticle
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TSParticle   Defines a particle which will generate photons when   //
//                 crossing fluorescent materials                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"

enum PDistribution {singlevalued,straigth,gaussian,exponential};

class TSParticle  : public TNamed {

protected:

  Double_t      fMass;     //mass of the particle in Gev
  PDistribution fPdist;    //type of momentum distribution
  Float_t       fPmean;    //mean momentum in Gev in case singlevalued or gaussian
  Float_t       fPmin;     //min. momentum, case straigth, gaussian or exponential
  Float_t       fPmax;     //max. momentum, case straigth, gaussian or exponential
  Float_t       fSig;      //sigma of gaussian distribution
  Float_t       fSlope;    //slope of exponential
  Float_t       fExp1;     //1st constant in case {exponential}
  Float_t       fD;        //difference fExp1 - fExp2
  Double_t      fP;        //generated momentum [Gev*c]
  Double_t      fE;        //generated energy [Gev]
  Double_t      fBeta;     //beta = fP/fE of particle
  Double_t      fSpeed;    //speed of particle  [cm/picoseconds]
  Double_t      fCerCos;   //cosine of Cerenkov angle
  Double_t      fCerSin;   //sine   of Cerenkov angle
  Int_t         fCerNphot; //number of cerenkov photons emitted

  Bool_t        AddToList() const;
  Bool_t        AddToList(const char*) const;

public:

  TSParticle() { ; }
  TSParticle(const char*,const char* = "",PDistribution = singlevalued,Float_t=1.0,
    Float_t=1.0,Float_t=10.0,Float_t=0.5,Float_t=1.0,Double_t = 0.105658389);
  virtual      ~TSParticle();
  Double_t      CerCos() const         { return fCerCos;   }
  void          Cerenkov(Double_t,Double_t=1.0);
  Double_t      CerSin() const         { return fCerSin;   }
  Double_t      GenP();
  Double_t      GetEnergy() const      { return fE;        }
  Double_t      GetMomentum() const    { return fP;        }
  Double_t      Mass() const           { return fMass;     }
  Int_t         NbCerPhot() const      { return fCerNphot; }
  void          Print() const;
  void          SetMass(Double_t mass) { fMass = mass;     }
  ClassDef(TSParticle,1) //Define a particle which will generate photons
};
#endif
