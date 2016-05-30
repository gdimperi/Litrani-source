// @(#)root/html:$Id: TResults.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TResults
#define LITR_TResults
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TResults    Summary of all statistics and histograms for one run   //
//               or for all runs                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObjArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TPaveLabel.h"
#include "TPaveText.h"

class TStatSurfD;
class TStatVolD;
class TStatAPD;

class TResults : public TNamed {

protected:

  Short_t     fNfluo;          //Nb of components of fluorescence for material histogrammed
  TPaveLabel *fPavel;          //! Pave Label for the summary plot
  TPaveText  *fPavet;          //! Pave Text for the summary plot

  Short_t     AtLeastOne();    //Chooses at least one fluorescent material to histogram
  void        InitP();         //Put pointers to 0

public:

  static Double_t fgEnergyOfGamma; //Maximal energy of gammas defined in TPhotoElecCompton;

  //General
  Int_t       fRun;            //run number. If 0 : global statistics
  Bool_t      fConclusion;     //true only when Conclusion() has been called
  Double_t    fXparam;         //parameter to take as abscissa for TPublication
  Int_t       fK;              //histo to be plotted
  Int_t       fKdet;           //detector to be plotted
  Int_t       fKSurfDet;       //Surface detector to be plotted
  Int_t       fKVolDet;        //Volume detector to be plotted
  Int_t       fKAPD;           //APD to be plotted
  Int_t       fKDH;            //histo of detector to be plotted
  Int_t       fNpGener;        //Nb. of photons generated
  TH1F       *fHPoints;        //Nb. of points on photon trajectory
  //Detection of photons
  Int_t       fNpSeen;         //Nb. of photons seen
  Double_t    fPhweigth;       //(Photon arrived at detector with QE)*(Quantum Efficiency)
  TH1F       *fHTimeSeen;      //10 time of arrival of photons on detector
  TH1F       *fHDistSeen;      //11 distance travelled by photons seen in detector
  TH1F       *fHWvlgthSeen;    //31 Wavelength of photons seen
  //Special detectors statistics
  Int_t       fNSurfDet;       //Nb. of surface detectors
  TObjArray   fSurfDet;        //list of all surface detectors
  Int_t       fNVolDet;        //Nb. of volume detectors
  TObjArray   fVolDet;         //list of all volume detectors
  Int_t       fNAPD;           //Nb. of APDs
  TObjArray   fAPD;            //list of all APD
  //Loss of photons
  Int_t       fNpLossAny;      //Nb. of photons lost for ANY reason
  Int_t       fNpAbsMat;       //Nb. of photons lost inside non-sensitive material
  Int_t       fNpAbsBef;       //Nb. of photons absorbed before revetment (supplementary absorption)
  Int_t       fNpAbsRvt;       //Nb. of photons absorbed by revetment
  Int_t       fNpOutSide;      //Nb. of photons lost because they leave the setup
  Int_t       fNpAbnorm;       //Nb. of photons lost for abnormal reasons
  Int_t       fNpTooLate;      //Nb. of photons lost because seen too late
  Int_t       fNpTooFew;       //photons lost because of too few electrons in APD
  Int_t       fNpLossAng;      //Lost because of acceptance angle [NPABSDET]
  Int_t       fNpLossQE;       //Lost because of quantum efficiency
  Int_t       fNpAbsL0;        //Lost because user put a value of 0 for absorption length
  TH1F       *fHAbsAny;        //distance travelled by photons lost anywhere
  TH1F       *fHAbsMat;        //distance travelled by photons absorbed in material
  TH1F       *fHAbsBef;        //distance of photons absorbed before revetment
  TH1F       *fHAbsRvt;        //distance of photons absorbed by revetment
  TH1F       *fHOutSide;       //distance travelled by photons leaving the setup
  TH1F       *fHAbnLost;       //distance travelled by photons abnormally lost
  TH1F       *fHTooLate;       //Wavelength of photons seen, but too late
  //Diffusion of photons
  TH1F       *fHDiffusion;     //Number of diffusions done by photon
  //Shape and face statistics
  Int_t       fNbOfShapes;     //Number of shapes in setup
  Int_t       fMaxNbOfFaces;   //Maximum Nb. of faces for a shape in this setup.
  TH1F       *fHStopShape;     //Nb. of photons having stopped inside shape i, (not on its faces)
  TH2F       *fHStopFace;      //Nb. of photons that died on face i of shape k
  TH2F       *fHHitRvt;        //Nb. of hits on revetment of face i of shape k
  TH2F       *fHHitFace;       //Nb. of hits of photons on shape k, face i
  TH2F       *fHCrossFace;     //Nb. of crossing forward through shape k face i, leaving shape k.
  TH2F       *fHDontCross;     //Nb. of not crossing forward through shape k face i, leaving shape k.
  //Time and Wavelength
  TH1F       *fHTimeAll;       //generated time of emission, all components of fluorescence
  TH2S       *fHTimeEach;      //generated time of emission, each component of fluorescence
  TH1F       *fHWvlgthAll;     //generated wavelength all components of fluorescence
  TH2S       *fHWvlgthEach;    //generated wavelength each component of fluorescence
  TH1F       *fHTimeCer;       //generated time of emission for Cerenkov photons
  TH1F       *fHWvlgthCer;     //generated wavelength for Cerenkov photons
  //Particle generation
  Int_t       fNbPart;         //number of particles generated
  TH1F       *fHPMomentum;     //momentum of generated particles
  TH1F       *fHPTheta;        //theta distribution of generated particles
  TH2D       *fHEnergyInShape; //Energy deposited in shape k by gamma

  TResults() { InitP();}
  TResults(const char*, const char*, Int_t, Double_t = -2.0e+20);
  TResults(const TString, const TString, Int_t, Double_t = -2.0e+20);
  virtual    ~TResults();
  Bool_t      AddToList() const;
  Bool_t      AddToList(const char *) const;
  void        CInit();
  void        CInitStatDet();
  Int_t       Compare(const TObject*) const;
  void        Conclusion();
  void        D();
  void        D1();
  void        DelHistos();
  void        DelPaveT();
  void        DoStat();
  TStatAPD*   GetAPD(const char*) const;
  Int_t       GetRunNumber() const { return fRun; }
  TStatSurfD* GetSurfDet(const char*) const;
  TStatVolD*  GetVolDet(const char*) const;
  void        GInit();
  Bool_t      HasFluo() const { return fNfluo>0; }
  Bool_t      IsEqual(const TObject*) const;
  Bool_t      IsSortable() const { return kTRUE; }
  void        N();
  void        N1();  //display next histogram fK
  void        RefreshDet() const;
  void        ResetCounters();
  ClassDef(TResults,1) //Summary of all statistics and histograms for one run
};
R__EXTERN TResults *gGs;
R__EXTERN TResults *gCs;
#endif
