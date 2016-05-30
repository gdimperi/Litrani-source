// @(#)root/html:$Id: TPhotoElecCompton.h 2008-06-11
// Author: D. Wahl <mailto:david.n.wahl@gmail.com>,F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TPhotoElecCompton
#define LITR_TPhotoElecCompton
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPhotoElecCompton                                                    //
//                                                                      //
// Generation of gamma rays of energy approx 0.1-1 Mev                  //
// You can only use a material with defines A,Z, rho for this           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#undef OLDMETHOD
#include "TNamed.h"

class T3Vector;
class TOpticMaterial;
class TSupplShape;
class TSNode;
class TPhoton;
class TPhotonCradle;

class TPhotoElecCompton : public TNamed {

protected:

  Double_t        fE0;             //Energy of the incoming Gamma in Mev
  TString         fFullName;       //name + run number. Name of histos file created
  // Pointers not to delete (external objects)
  TSupplShape    *fRefShape;       //! Reference shape for setting the coordinates of the generated Gamma
  TString         fRefName;        //! Name of the reference shape fRefShape
  TSNode         *fSNode;          //! Node to which fRefShape belongs
  //Pointers to delete (photon generation)
  TPhoton        *fPhot;           //Pointer to generated photons
  // Photon Generation
  T3Vector        fSourcePosition; //Gamma Source initial Position
  T3Vector        fGammaDirection; //Gamma initial direction
  Bool_t          fCollimated;     //Is gamma beam collimated
  Int_t           fRun;            //Run number
  T3Vector        fPosGamma;       //! Current position of Gamma in WCS
  T3Vector        fDirGamma;       //! Current direction of Gamma
  // Tracking variables
  Bool_t          fFollowGamma;    //! Propagate Gamma through non fluorescent materials
  Double_t        fCurrentMeV;     //! Current energy of gamma     
  Int_t           fNphot;          //Number of photons generated by this gamma
  Double_t        fTotEdeposit;    //Total energy deposited in shape
  Int_t           fCompton;        //Compton interaction counter
  Double_t        fTime;           //! Time of travel of compton 
  Bool_t          fGammaDead;      //! Did gamma gave all its energy, for instance by photo-electric cross-section
  Bool_t          fIsNextShape;    //! True as long as fluorescent shape found on the way of the gamma

  Double_t       *fShEdeposit;     //[fNbOfShape] fShEdeposit[k] is energy deposited in shape k for ALL runs Gentit
  Double_t       *fShEdepositR;    //[fNbOfShape] fShEdeposit[k] is energy deposited in shape k for this run Gentit
  Int_t           fNbOfShape;      //Number of shapes in setup Gentit
  // Fill Death members
  Bool_t          fFillDeath;      //! true if FillDeath has to be called
  TH1F           *fHX0;            //! hist of x coord of death point of seen photons
  TH1F           *fHY0;            //! hist of y coord of death point of seen photons
  TH1F           *fHZ0;            //! hist of z coord of death point of seen photons

  Bool_t          fFollowElectron; // Do we attempt to follow the electron from Compton scattering.
  Double_t        fStep; // Step length for energy deposition of electron

  Bool_t          AcceptDirection() const;
  void            Clear();
  void            EmitLight(Double_t, TSupplShape*, EDepProcess, T3Vector&);
  void            FillDeath() const;
  T3Vector        DepositEnergy(TSupplShape*, Double_t);
  Double_t        Get_dSigma_dOmega(Double_t, Double_t) const;
  Double_t        GetElectrondEdx(Double_t) const;
  Double_t        GetPathlength(Double_t, TOpticMaterial*);
  Double_t        GetXSectnKN(Double_t, TOpticMaterial*) const;// Get Klein Nishina  X-Section
  Double_t        GetXSectnPE(Double_t, TOpticMaterial*) const;// Get Photo-electric X-Section
  TSupplShape*    GotoNextShape(TSupplShape*);
  void            InitAxis();
  void            InitP();
  void            InitTV();
  void            NameFile();
  void            OutsideOk();

public:

  TPhotoElecCompton()                  { InitP();           }
  TPhotoElecCompton(Text_t *name, Text_t *title, Text_t *namesh,
    Double_t E0, T3Vector sourceposition);
  TPhotoElecCompton(Text_t *name, Text_t *title, Text_t *namesh,
    Double_t E0, T3Vector sourceposition, 
    T3Vector gammadirection);
  virtual        ~TPhotoElecCompton();
  Int_t           CheckCompton() const { return fCompton;   }
  Bool_t          CheckDead() const    { return fGammaDead; }
  void            Gen(Int_t,Int_t = 1,Double_t = -2.0e+20,Bool_t = kTRUE,Bool_t = kFALSE);  
  void            SetFillDeath(TH1F*, TH1F*, TH1F*);
  void            SetFollowElectron(Bool_t, Double_t);
  void            SetFollowGamma(Bool_t) { fFollowGamma = kTRUE;}
  void            SetE0(Double_t E0)   { fE0 = E0;          }//Gentit
  void            SetGammaDirection(T3Vector);//Gentit
  void            SetSourcePosition(T3Vector);//Gentit

  ClassDef(TPhotoElecCompton,0) //Generation of gamma rays of energy approx 0.1-1 Mev
};
#endif