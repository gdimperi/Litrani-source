// @(#)root/html:$Id: TResults.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TwoPadDisplay.h"
#include "TMessErr.h"
#include "TLitGlob.h"
#include "TOpticMaterial.h"
#include "TStatSurfD.h"
#include "TStatVolD.h"
#include "TStatAPD.h"
#include "TDetector.h"
#include "TSupplShape.h"
#include "TFace.h"
#include "TResults.h"

TResults* gGs = 0;
TResults* gCs = 0;
Double_t TResults::fgEnergyOfGamma = 2.0;

ClassImp(TResults)
//______________________________________________________________________________
//
//
//  GENERAL PRESENTATION OF THE HISTOGRAMS AND STATISTICS AVAILABLE
//                      IN LITRANI
//
//  There are 4 kinds of histograms in Litrani :
//
//    (1) - HISTOGRAMS ASSOCIATED WITH FITS AND INTERPOLATION.  Each time
//one defines a fit or an interpolation in Litrani ( with the class TSplineFit)
//histograms are created to show the results of the fit or interpolation.
//The access to these histograms is extremely simple. For instance, if you
//have defined the absorption length of the Meltmount glue by :
//
// -> const Int_t M = 34;
// -> const Int_t m = 5;
// -> Int_t i;
// -> TSplineFit *AbsLMeltmount;
// -> Double_t x[M]= { 320,   340,   360,   380,   400,   420,   440,   460,   480,   500,
// ->                  520,   540,   560,   580,   600,   620,   640,   660,   680,   700,
// ->                  720,   740,   760,   780,   800,   820,   840,   860,   880,   900,
// ->                  920,   940,   960,   980 };
// -> Double_t y[M]= { 0.000527, 0.000830, 0.001958, 0.011993, 0.041164, 0.061999, 0.076422, 0.082832, 0.086455, 0.090408,
// ->                  0.094737, 0.099499, 0.102063, 0.104762, 0.110610, 0.113784, 0.117146, 0.120711, 0.124499, 0.128532,
// ->                  0.132832, 0.137430, 0.142356, 0.147647, 0.153345, 0.159499, 0.166166, 0.173412, 0.181318, 0.189975,
// ->                  0.199499, 0.210025, 0.221722, 0.234794 };
// -> Double_t s[M]= { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
// ->                  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
// ->                  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
// ->                  0.001, 0.001, 0.001, 0.001 };
// -> AbsLMeltmount = new TSplineFit("AbsorptionLength_Meltmount1704","Absorption Length | Glue Meltmount 1.704",
// ->                                5,M,m,x,y,s,kTRUE,0.0,kTRUE,0.24,300.0,1000.0,kFALSE);
// -> AbsLMeltmount->SetSource("Meltmount"); 
// -> AbsLMeltmount->SetMacro("AbsorptionLength_Meltmount1704.C");
// -> AbsLMeltmount->SetXLabel("wavelength [nm]");
// -> AbsLMeltmount->SetVLabel("absorption length");
//
//  Then you have only to do :
//
// -> AbsLMeltmount->SetDefaultLabels();
// -> AbsLMeltmount->DrawFit();
//
//to see the histogram containing data and fit. If you want to see ALL histo-
//grams of fits used in your application, do :
//
// -> TSplineFit::DrawFitsInCollection();  // or simpler:
// -> gLit->F();
//
//  See the description of class TSplineFit for more details.
//
//    (2) - HISTOGRAMS RELATED TO ONE SPECIFIC RUN.  Each time you call the
//method Gen()  [ TSpontan::Gen() or TBeam::Gen() or TElecCascade::Gen()],
//you have the possibility to ask for histograms specific to the run you are
//launching. These histograms are defined in the class TResults (this
//one !). For a list of the histograms available, look at the public
//declarations of class TResults. These histograms are pointed to by the
//global pointer gCs. How to make gCs point towards the run you want to see
//will be explained below.
//
//    (3) - HISTOGRAMS SUM OF ALL RUNS.   These histograms are the same as
//the one defined in (2), except that they contain the sum of all runs. They
//are also defined in the class TResults, with a run number fRun = 0. They
//are accessed using the pointer gGs.
//
//    (4) - HISTOGRAMS SHOWING QUANTITIES AS A FUNCTION OF A RUN DEPENDENT
//          PARAMETER.  For instance :
//           - Efficiency of detection of photons as a function of position
//             of the beam (if beam position is what you have varied from
//             run to run ).
//           - Loss in material as a function of tilt angle of the fibre ( if
//             tilt angle of fibre is what you have varied from run to run ).
//           - etc
//These histograms are defined in the class TPublication. See this class for
//more details. They are pointed to by the global pointer gGp.
//
//  Here, in the class TResults, we are interested only in the histograms
//of kind (2) and (3). Let us turn now to the question of finding the value
//of the pointers gGs and gCs.
//
//    FINDING THE WANTED POINTER TOWARDS STATISTICS AND HISTOGRAMS
//
//    (a) - If you are interested in the global statistics for all runs,
//histograms of kind (3), you have the pointer gGs at hand. No search is
//necessary. For instance, you can look at the histogram giving the distri-
//bution in time of all photons seen in all runs by :
//  gGs->fHTimeSeen->Draw();
//or look simply at all these histograms by :
//  gGs->N();
//  gGs->D();
//
//    (b) - If you are interested in the statistics of the current run
//( the last run achieved at this time ), you have the pointer gCs at hand.
//No search necessary. For instance, you can look at the histogram giving
//the distribution in time of all photons seen in this last run by :
//  gCs->fHTimeSeen->Draw();
//or look simply at all the histograms of this last run by :
//  gCs->N();
//  gCs->D();
//
//    (c) - if you are interested neither in (a) nor in (b), but you wish
//to access, after the generation of all runs, after having closed the .root
//file by a call to CloseFiles(), the statistics and histograms of a given
//run of run number = irun, proceed like this :
//      (c0) => Bool_t b;
//      (c1) => gLit->OpenFilesR("litrani"); //Open the .root file for read
//      (c2) => b = gLit->SelectRun(irun);   //now gCs points towards
//                                           //run irun if b true.
//You can now for instance look at all histograms of run irun by :
//      (c3) - gCs->N();
//
//The method SelectRun(irun) works only in the case where you have given a
//different run number for each of your run.
//
//    (d) - In the case where you have many different sources of photons,
//each with a different name, the possibility arises that you have many
//different runs, with DIFFERENT names but IDENTICAL run number. In that
//case, use the method gLit->SelectRun(name, irun), name being the name of
//the source of photons having generated the run you are interested in.
//  Notice however that it is not a good idea to give twice the same run
//number even in the case where the name of the source of photons has
//changed : you would have problems using the class TPublication, except in
//the case where you have defined the parameter "xparam" of the method
//Gen(), where everything is ok. Look at the method Gen() [ TSpontan::Gen()
//or TBeam::Gen() or TElecCascade::GEn() ].
//
//  The macro xreadfile.C is an example on how to read the histograms
//from the file produced by Litrani.
//
//
//    DETECTOR HISTOGRAMS
//
//  Histograms of kind (2) or (3) have sub-histograms not appearing in the
//class declaration of TResults. These are the histograms relative to de-
//tectors, which are contained in the classes TStatSurfD, TStatVolD and
//TStatAPD. They are stored by TResults in the TObjArray fSurfDet, fVolDet
//and fAPD. You must realize that in the setup used by Litrani, you may
//have ANY number of detectors, and that these detectors may be of 4 kinds:
//    (a) - General surface detectors, contained in the class TStatSurfD
//    (b) - Phototubes, also contained in the class TStatSurfD
//    (c) - general volume detectors, contained in the class TStatVolD
//    (d) - APD contained in the class TStatAPD.
//  For more informations on detectors, look at the class TSupplShape.
//  TResults provide you with 3 methods to reach the histograms of some
//detector : GetSurfDet(), GetVolDet() and GetAPD(). The use of these methods
//implies that 
//              (1) you know the type (a)...(d) of the wanted detector [ you
//have to call GetSurfDet() for type (a) or (b), GetVolDet() for type (c),
//and GetAPD() for type (d) ]
//              (2) you know the name of the wanted detector [ each detec-
//tor in Litrani must have a different name ]
//  Suppose you want to look at the histogram fHeVersusT, the histogram
//giving the number of e- emitted versus time for the APD named "hamamatsu",
//in the run 103. Proceed like this :
//
//  => Bool_t b;
//  => b = gLit->SelectRun(103)    //make gCs point towards run 103
//  => TStatAPD *papd;
//  => papd = gCs->GetAPD("hamamatsu");
//  => papd->fHeVersusT->Draw();
//
//
//    LOOKING AT ALL HISTOGRAMS OF KIND (2) or (3)
//
//  Very often, at the end of a production, one does not want to see
//particular histograms, but simply ALL histograms, in order to fully check
//that everything was ok. For this, there exists the extremely simple
//methods 
//
//      N() : for looking at all histograms of a run (without the detector
//            histograms).
//      D() : for looking at all detector histograms of a run
//
//  For instance, let us suppose that you want to see all histograms of run
//103. Proceed like this :
//
//  => Bool_t b;
//  => b = gLit->SelectRun(103)  //make gCs point towards run 103 if b true.
//  => gCs->N();                 //You will then see the 1st histogram. To
//                               //see the next one, simply type n<CR>. To
//                               //stop the show, type any key, for instance
//                               //q<CR>.
//  Proceed exactly the same to see all detector histograms of run 103, but
//use D() instead of N().
//
//    ROOT FILE CONTAINING ALL STATISTICS AND HISTOGRAMS
//
//  This file has the default name "litrani.root". You can change this 
//default name
// - either when calling the constructor of TLItGlob, when defining the
//     global pointer gLit at the beginning of your macro. It is the 5th
//     argument :
//      ==>  gLit = new TLitGlob(85,2,1515,810,"newname");
//  - or by calling :
//      ==>  gLit->NameIOFile("newname");
//  Change this name BEFORE generating any photon !
//
//  At the end of Litrani, if you open the .root file and list it by:
//
//     ==> TFile *f = new TFile("litrani.root","READ");
//     ==> f->ls();
//
//  you will get :
//
//  =>TFile*         litrani.root
//  => KEY: TTree    T;1     Statistics and Histos for each run
//  => KEY: TResults GlobStat;1      Global statistics of all runs
//  => KEY: TPublication     Summary;1       tilt angle of the fibre
//
//  You see that the file is composed of :
//
//  (1) "T"        : a TTree containing statistics and histograms of each
//individual runs, under the form of objects of class TResults. You look at
//the content of this tree using gLit->SelectRun() and the pointer gCs.
//  (2) "GlobStat" : an object of class TResults containing the global
//sum of the statistics and histograms of all runs. You look at it using
//the pointer gGs.
//  (3) "Summary"  : an object of class TPublication containing histograms
//of quantities varying as a function of a run parameter. You look at it
//using the pointer gGp.
//  Look at the macro xreadfile.C to have an example on how to read the
//histograms from the file produced by Litrani.
//
TResults::TResults(const char *name, const char *title, Int_t run,Double_t xparam):TNamed(name,title) {
  //constructor for this class of statistics and summary for run "name" of run
  //number run. If run == 0, this is the global statistics of all runs.
  const Double_t xparamlim = -1.0e+20;
  Bool_t b;
  fConclusion = kFALSE;
  fRun  = run;
  InitP();
  if ((xparam>xparamlim) || (!fRun)) fXparam = xparam;
  else                  fXparam = fRun;
  fK    = 0;
  fKdet = 0;
  fKDH  = 0;
  fKSurfDet = 0;
  fKVolDet = 0;
  fKAPD = 0;
  if (!fRun) GInit();
  else CInit();
  CInitStatDet();
  b = AddToList(name);
}
TResults::TResults(const TString name, const TString title,Int_t run,Double_t xparam):TNamed(name,title) {
  //constructor for this class of statistics and summary for run "name" of run
  //number run. If run == 0, this is the global statistics of all runs.
  const Double_t xparamlim = -1.0e+20;
  Bool_t b;
  fConclusion = kFALSE;
  fRun = run;
  InitP();
  if (xparam>xparamlim) fXparam = xparam;
  else                  fXparam = fRun;
  fK   = 0;
  fKdet = 0;
  fKDH  = 0;
  fKSurfDet = 0;
  fKVolDet = 0;
  fKAPD = 0;
  if (!fRun) GInit();
  else CInit();
  CInitStatDet();
  b = AddToList();
}
TResults::~TResults() {
  DelPaveT();
  DelHistos();
  if (fNSurfDet) {
    fSurfDet.Delete();
    fNSurfDet = 0;
  }
  if (fNVolDet) {
    fVolDet.Delete();
    fNVolDet = 0;
  }
  if (fNAPD) {
    fAPD.Delete();
    fNAPD = 0;
  }
  if (gLit) {
    gLit->fResults.Remove(this);
    gLit->fNResults--;
  }
}
Bool_t TResults::AddToList() const {
  return gLit->AddOneResult(this,fName);
}
Bool_t TResults::AddToList(const char *name) const {
  return gLit->AddOneResult(this,name);
}
Short_t TResults::AtLeastOne() {
  //if there is fluorescent materials in the setup, verifies that at least one
  //is set so that its fluorescent characteristics be histogrammed
  //fNfluo is the number of fluorescent components
  Bool_t ThereIs = kFALSE;
  Bool_t found = kFALSE;
  Int_t khist = 0;
  TOpticMaterial *pmat;
  TOpticMaterial *pmats = 0;
  Short_t fNfluo = 0;
  TIter next(&gLit->fOptMat);
  while ((!ThereIs) && (pmat = (TOpticMaterial *)next()))
    if (pmat->IsFluorescent()) ThereIs = kTRUE;
  if (ThereIs) {
    next.Reset();
    while ((pmat = (TOpticMaterial *)next()))
      if ((pmat->IsFluorescent()) && (pmat->GetHistFluo())) {
        khist += 1;
        if (khist == 1) pmats = pmat;
        if (khist>1) {
          pmat->SetHistFluo(kFALSE);
          khist -= 1;
        }
        //end if ((pmat->IsFluorescent() && (pmat->GetHistFluo()))
      }
      if (!khist) {
        next.Reset();
        while ((!found) && (pmat = (TOpticMaterial *)next())) {
          if (pmat->IsFluorescent()) {
            found = kTRUE;
            pmat->SetHistFluo(kTRUE);
            pmats = pmat;
            //end if (pmat->IsFluorescent())
          }
          //end while ((!found) && (pmat = (TOpticMaterial *)next()))
        }
        //end if (!khist)
      }
      if (pmats) fNfluo = pmats->GetFluorescent();
      //end if (ThereIs)
  }
  return fNfluo;
}
void TResults::CInit() {
  //Groups everything common to all constructors
  Axis_t late,MinW,MaxW;
  late = TLitPhys::Get()->TooLate();
  MinW = TLitPhys::Get()->MinWaveL();
  MaxW = TLitPhys::Get()->MaxWaveL();
  fNpGener = 0;
  //Detection of photons
  fHPoints   = new TH1F("Points",
    "Number of points on photon trajectory",250,0.0,250.0);
  fNpSeen  = 0;
  fPhweigth = 0.0;
  fHTimeSeen = new TH1F("TimeSeen",
    "time of arrival (ps) of photons on detector",100,0.0,late);
  fHDistSeen = new TH1F("DistSeen",
    "distance (cm) travelled by photons seen in detector",100,0.0,400.0);
  fHWvlgthSeen = new TH1F("WvlgthSeen","Wavelength (nm) of photons seen",
    100,MinW,MaxW);
  //Special detectors statistics
  fNSurfDet = 0;
  fNVolDet = 0;
  fNAPD = 0;
  //Loss of photons
  fNpLossAny = 0;
  fNpAbsMat  = 0;
  fNpAbsBef  = 0;
  fNpAbsRvt  = 0;
  fNpOutSide = 0;
  fNpAbnorm  = 0;
  fNpTooLate = 0;
  fNpTooFew  = 0;
  fNpLossAng = 0;
  fNpLossQE  = 0;
  fNpAbsL0   = 0;
  fHAbsAny  = new TH1F("AbsAny",
    "distance (cm) travelled by photons absorbed anywhere",100,0.0,400.0);
  fHAbsMat  = new TH1F("AbsMat",
    "distance (cm) travelled by photons absorbed in material",100,0.0,400.0);
  fHAbsBef  = new TH1F("AbsBef",
    "distance (cm) of photons absorbed before revetment",100,0.0,400.0);
  fHAbsRvt  = new TH1F("AbsRvt",
    "distance (cm) of photons absorbed by revetment",100,0.0,400.0);
  fHOutSide = new TH1F("OutSide",
    "distance (cm) travelled by photons leaving the setup",100,0.0,400.0);
  fHAbnLost = new TH1F("AbnLost",
    "distance (cm) travelled by photons abnormally lost",100,0.0,400.0);
  fHTooLate = new TH1F("TooLate",
    "Wavelength of photons seen, but too late",
    100,MinW,MaxW);
  //Diffusion of photons
  fHDiffusion = new TH1F("Diffusion",
    "Number of diffusions per photon",16,0.0,16.0);
  //Shape and face statistics statistics
  Axis_t max,maxf;
  if (!gLit->fShapeNumbered)
    fNbOfShapes = gLit->GiveNbToShape();
  else
    fNbOfShapes = gLit->fNSupplShape;
  fMaxNbOfFaces = gLit->fMaxNbOfFaces;
  max = fNbOfShapes;
  fHStopShape = new TH1F("StopShape","Number of photons stopped in shape k",
    fNbOfShapes,0.0,max);
  maxf = fMaxNbOfFaces;
  fHStopFace  = new TH2F("StopFace",
    "Nb. of photons that stopped on shape k, face i",
    fNbOfShapes,0.0,max,fMaxNbOfFaces,0.0,maxf);
  fHHitRvt  = new TH2F("HitRvt",
    "Nb. of hits on revetment of face i of shape k",
    fNbOfShapes,0.0,max,fMaxNbOfFaces,0.0,maxf);
  fHHitFace   = new TH2F("HitFace",
    "Nb. of hits of photons on shape k, face i",
    fNbOfShapes,0.0,max,fMaxNbOfFaces,0.0,maxf);
  fHCrossFace   = new TH2F("CrossFace",
    "Nb. of crossing forward through shape k face i",
    fNbOfShapes,0.0,max,fMaxNbOfFaces,0.0,maxf);
  fHDontCross   = new TH2F("DontCross",
    "Nb. of not crossing forward through shape k face i",
    fNbOfShapes,0.0,max,fMaxNbOfFaces,0.0,maxf);
  //Time and Wavelength
  fHTimeAll = new TH1F("TimeAll","Generated time (ps) for all components",
    100,0.0,late);
  fHWvlgthAll = new TH1F("WvlgthAll","Generated wavelength (nm) for all components",
    100,MinW,MaxW);
  fHTimeCer = new TH1F("TimeCer","Generated time (ps) for Cerenkov photons",
    100,0.0,late);
  fHWvlgthCer = new TH1F("WvlgthCer","Generated wavelength (nm) for Cerenkov photons",
    100,MinW,MaxW);
  fNfluo = AtLeastOne();
  fHTimeEach   = 0;
  fHWvlgthEach = 0;
  if (fNfluo) {
    Axis_t afluo = fNfluo;
    fHTimeEach = new TH2S("TimeEach","Generated time for each component",
      64,0.0,late,fNfluo,0.0,afluo);
    fHWvlgthEach = new TH2S("WvlgthEach","Generated wavelength (nm) for each component",
      64,MinW,MaxW,fNfluo,0.0,afluo);
  }
  //Particle generation
  fNbPart = 0;
  fHPMomentum = new TH1F("Momentum","Momentum [Gev] of generated particles",
    100,0.0,100.0);
  fHPTheta = new TH1F("Theta","Theta distribution of generated particles",80,0.0,3.2);
  if (!gLit->fShapeNumbered) gLit->GiveNbToShape();
  fHEnergyInShape = new TH2D("EinShape","Energy [Mev] deposited by gamma in all shapes",
    gLit->fNSupplShape,0.0,(Double_t)gLit->fNSupplShape,50,0.0,fgEnergyOfGamma);
}
void TResults::CInitStatDet() {
  //Defines all histograms attached to detectors
  Bool_t pm;
  Int_t n;
  n = gLit->fNDetector;
  if (n) {
    Int_t i;
    Bool_t g = !fRun;
    Double_t distCA;
    const char *name;
    const char *title;
    KindOfDetector kind;
    TSupplShape *shape;
    TFace *face;
    TDetector *pdet;
    TStatSurfD *psurf;
    TStatVolD *pvol;
    TStatAPD *papd;
    TIter next(&gLit->fDetector);
    for (i=0;i<n;i++) {
      pdet  = (TDetector *)next();
      name  = pdet->GetName();
      title = pdet->GetTitle();
      kind  = pdet->GetKind();
      shape = pdet->GetShape();
      face  = pdet->GetFace();
      pm    = kind==phototube;
      switch (kind) {
      case none:
        break;
      case surfdet:
      case phototube:
        psurf = new TStatSurfD((char *)name,(char *)title,fNSurfDet,g,pm);
        fSurfDet.Add(psurf);
        if (fRun == 0) face->SetDetNumber(fNSurfDet);
        fNSurfDet += 1;
        break;
      case volumedet:
        pvol  = new TStatVolD((char *)name,(char *)title,fNVolDet,g);
        fVolDet.Add(pvol);
        if (fRun == 0) shape->SetDetNumber(fNVolDet);
        fNVolDet += 1;
        break;
      case apd:
        distCA = shape->GetDistCatAn();
        papd  = new TStatAPD((char *)name,(char *)title,fNAPD,distCA,g);
        fAPD.Add(papd);
        if (fRun == 0) {
          shape->SetDetNumber(fNAPD);
        }
        fNAPD += 1;
        break;
      }
    }
  }
}
Int_t TResults::Compare(const TObject *obj) const {
  if (fXparam>((TResults *)obj)->fXparam)
    return 1;
  else if (fXparam<((TResults *)obj)->fXparam)
    return -1;
  else return 0; 
}
void TResults::Conclusion() {
  //Last calculations before using the class
  if (!fConclusion) {
    Int_t n;
    n = gLit->fNDetector;
    if (n) {
      Int_t i;
      const char *name;
      KindOfDetector kind;
      TDetector *pdet;
      TStatSurfD *psurf;
      TStatVolD *pvol;
      TStatAPD *papd;
      TIter next(&gLit->fDetector);
      for (i=0;i<n;i++) {
        pdet  = (TDetector *)next();
        name  = pdet->GetName();
        kind  = pdet->GetKind();
        switch (kind) {
        case none:
          break;
        case surfdet:
        case phototube:
          psurf = GetSurfDet((char *)name);
          psurf->Conclusion();
          break;
        case volumedet:
          pvol = GetVolDet((char *)name);
          pvol->Conclusion();
          break;
        case apd:
          papd = GetAPD((char *)name);
          papd->Conclusion();
          break;
        }
      }
    }
    fConclusion = kTRUE;
  }
}
void TResults::D() {
  //displays next detector histogram if the user type "n", exit otherwise
  char c='n';
  gTwoPad->SelectPad1();
  while (c == 'n' ) {
    D1();
    cout << "Press n to see next histo, any other key to quit" << endl;
    cin >> c;
  }
}
void TResults::D1() {
  //Display all histograms of detectors, in turn
  Int_t NbOfDetectors;
  TStatSurfD *psurf;
  TStatVolD *pvol;
  TStatAPD *papd;
  if (!gTwoPad) gLit->BookCanvas();
  gTwoPad->ChangePad();
  if (gLit) gLit->RestoreComments();
  NbOfDetectors = fNSurfDet + fNVolDet + fNAPD;
  if (NbOfDetectors>0) {
    if (fNSurfDet>0) {
      if (fKSurfDet<fNSurfDet) {
        psurf = (TStatSurfD *)fSurfDet[fKSurfDet];
        switch (fKDH) {
        case 0:
          gTwoPad->SetNameForSave(psurf->fHTimeSeen->GetName());
          psurf->fHTimeSeen->Draw();
          break;
        case 1:
          gTwoPad->SetNameForSave(psurf->fHWvlgthSeen->GetName());
          psurf->fHWvlgthSeen->Draw();
          break;
        case 2:
          gTwoPad->SetNameForSave(psurf->fHWvlgthNot->GetName());
          psurf->fHWvlgthNot->Draw();
          break;
        case 3:
          gTwoPad->SetNameForSave(psurf->fHQEff->GetName());
          psurf->fHQEff->Draw();
          break;
        case 4:
          gTwoPad->SetNameForSave(psurf->fHAngleAcc->GetName());
          psurf->fHAngleAcc->Draw();
          break;
        case 5:
          gTwoPad->SetNameForSave(psurf->fHAngleAll->GetName());
          psurf->fHAngleAll->Draw();
          break;
        }//end switch (fKDH)
        fKDH += 1;
        if (fKDH == 6) {
          fKDH       = 0;
          fKSurfDet += 1;
          fKdet     += 1;
          if (fKdet >= NbOfDetectors) {
            fKdet = 0;
            fKSurfDet = 0;
            fKVolDet  = 0;
            fKAPD     = 0;
          }
        }//end if (fKDH == 6)
      }//end if (fKSurfDet<fNSurfDet)
    }//end if (fNSurfDet>0)
    if (fNVolDet>0) {
      if ((fKSurfDet==fNSurfDet) && (fKVolDet<fNVolDet)) {
        pvol  = (TStatVolD *)fVolDet[fKVolDet];
        switch (fKDH) {
        case 0:
          gTwoPad->SetNameForSave(pvol->fHTimeSeen->GetName());
          pvol->fHTimeSeen->Draw();
          break;
        case 1:
          gTwoPad->SetNameForSave(pvol->fHWvlgthSeen->GetName());
          pvol->fHWvlgthSeen->Draw();
          break;
        case 2:
          gTwoPad->SetNameForSave(pvol->fHWvlgthNot->GetName());
          pvol->fHWvlgthNot->Draw();
          break;
        case 3:
          gTwoPad->SetNameForSave(pvol->fHQEff->GetName());
          pvol->fHQEff->Draw();
          break;
        }//end switch (fKDH)
        fKDH += 1;
        if (fKDH == 4) {
          fKDH      = 0;
          fKVolDet += 1;
          fKdet    += 1;
          if (fKdet >= NbOfDetectors) {
            fKdet = 0;
            fKSurfDet = 0;
            fKVolDet  = 0;
            fKAPD     = 0;
          }
        }//end if (fKDH == 4)
      }//end if ((fkSurfDet==fNSurfDet) && (fKVoldet<fNVoldet))
    }//end if (fNVolDet>0)
    if (fNAPD>0) {
      if ((fKSurfDet==fNSurfDet) && (fKVolDet==fNVolDet) && (fKAPD<fNAPD)) {
        papd  = (TStatAPD *)fAPD[fKAPD];
        switch (fKDH) {
        case 0:
          gTwoPad->SetNameForSave(papd->fHTimeSeen->GetName());
          papd->fHTimeSeen->Draw();
          break;
        case 1:
          gTwoPad->SetNameForSave(papd->fHWvlgthSeen->GetName());
          papd->fHWvlgthSeen->Draw();
          break;
        case 2:
          gTwoPad->SetNameForSave(papd->fHWvlgthNot->GetName());
          papd->fHWvlgthNot->Draw();
          break;
        case 3:
          gTwoPad->SetNameForSave(papd->fHeVersusT->GetName());
          papd->fHeVersusT->Draw();
          break;
        case 4:
          gTwoPad->SetNameForSave(papd->fHeVersusW->GetName());
          papd->fHeVersusW->Draw();
          break;
        case 5:
          gTwoPad->SetNameForSave(papd->fHdCathode->GetName());
          papd->fHdCathode->Draw();
          break;
        case 6:
          gTwoPad->SetNameForSave(papd->fHQEff->GetName());
          papd->fHQEff->Draw();
          break;
        }//end switch (fKDH)
        fKDH += 1;
        if (fKDH == 7) {
          fKAPD += 1;
          fKdet += 1;
          fKDH   = 0;
          if (fKAPD==fNAPD) {
            fKdet     = 0;
            fKSurfDet = 0;
            fKVolDet  = 0;
            fKAPD     = 0;
            if (fKdet >= NbOfDetectors) {
              fKdet = 0;
              fKSurfDet = 0;
              fKVolDet  = 0;
              fKAPD     = 0;
            }
          }//end if (fKAPD==fNAPD)
        }//end if (fKDH == 7)
      }//end if ((fkSurfDet==fNSurfDet) && (fKVoldet==fNVoldet) && (fKAPD<fNAPD))
    }//end if (fNAPD>0)
  }//end if (NbOfDetectors>0)
  else {
    gMes->SetName("D");
    gMes->SetTitle("TResults");
    gMes->M(Warning_M,2,"No detector in setup");
  }
  gTwoPad->UpdateIt();
}
void TResults::DelHistos() {
  //Delete all histograms
  if (fHPoints) {
    delete fHPoints;
    fHPoints = 0;
  }
  if (fHTimeSeen) {
    delete fHTimeSeen;
    fHTimeSeen = 0;
  }
  if (fHDistSeen) {
    delete fHDistSeen;
    fHDistSeen = 0;
  }
  if (fHWvlgthSeen) {
    delete fHWvlgthSeen;
    fHWvlgthSeen = 0;
  }
  if (fHAbsAny) {
    delete fHAbsAny;
    fHAbsAny = 0;
  }
  if (fHAbsMat) {
    delete fHAbsMat;
    fHAbsMat = 0;
  }
  if (fHAbsBef) {
    delete fHAbsBef;
    fHAbsBef = 0;
  }
  if (fHAbsRvt) {
    delete fHAbsRvt;
    fHAbsRvt = 0;
  }
  if (fHOutSide) {
    delete fHOutSide;
    fHOutSide = 0;
  }
  if (fHAbnLost) {
    delete fHAbnLost;
    fHAbnLost = 0;
  }
  if (fHTooLate) {
    delete fHTooLate;
    fHTooLate = 0;
  }
  if (fHDiffusion) {
    delete fHDiffusion;
    fHDiffusion = 0;
  }
  if (fHStopShape) {
    delete fHStopShape;
    fHStopShape = 0;
  }
  if (fHStopFace) {
    delete fHStopFace;
    fHStopFace = 0;
  }
  if (fHHitRvt) {
    delete fHHitRvt;
    fHHitRvt = 0;
  }
  if (fHHitFace) {
    delete fHHitFace;
    fHHitFace = 0;
  }
  if (fHCrossFace) {
    delete fHCrossFace;
    fHCrossFace = 0;
  }
  if (fHDontCross) {
    delete fHDontCross;
    fHDontCross = 0;
  }
  if (fHTimeAll) {
    delete fHTimeAll;
    fHTimeAll = 0;
  }
  if (fHTimeEach) {
    delete fHTimeEach;
    fHTimeEach = 0;
  }
  if (fHWvlgthAll) {
    delete fHWvlgthAll;
    fHWvlgthAll = 0;
  }
  if (fHWvlgthEach) {
    delete fHWvlgthEach;
    fHWvlgthEach = 0;
  }
  if (fHTimeCer) {
    delete fHTimeCer;
    fHTimeCer = 0;
  }
  if (fHWvlgthCer) {
    delete fHWvlgthCer;
    fHWvlgthCer = 0;
  }
  if (fHPMomentum) {
    delete fHPMomentum;
    fHPMomentum = 0;
  }
  if (fHPTheta) {
    delete fHPTheta;
    fHPTheta = 0;
  }
  if (fHEnergyInShape) {
    delete fHEnergyInShape;
    fHEnergyInShape = 0;
  }
  //Detector histograms
  if (fNSurfDet) {
    TStatSurfD *psurf;
    TIter next1(&fSurfDet);
    while ((psurf = (TStatSurfD *)next1())) {
      if (psurf->fHTimeSeen) {
        delete psurf->fHTimeSeen;
        psurf->fHTimeSeen = 0;
      }
      if (psurf->fHWvlgthSeen) {
        delete psurf->fHWvlgthSeen;
        psurf->fHWvlgthSeen = 0;
      }
      if (psurf->fHWvlgthNot) {
        delete psurf->fHWvlgthNot;
        psurf->fHWvlgthNot = 0;
      }
      if (psurf->fHQEff) {
        delete psurf->fHQEff;
        psurf->fHQEff = 0;
      }
      if (psurf->fHInside) {
        delete psurf->fHInside;
        psurf->fHInside = 0;
      }
      if (psurf->fHAngleAcc) {
        delete psurf->fHAngleAcc;
        psurf->fHAngleAcc = 0;
      }
      if (psurf->fHAngleAll) {
        delete psurf->fHAngleAll;
        psurf->fHAngleAll = 0;
      }
    }
  }
  if (fNVolDet) {
    TStatVolD *pvol;
    TIter next2(&fVolDet);
    while ((pvol = (TStatVolD *)next2())) {
      if (pvol->fHTimeSeen) {
        delete pvol->fHTimeSeen;
        pvol->fHTimeSeen = 0;
      }
      if (pvol->fHWvlgthSeen) {
        delete pvol->fHWvlgthSeen;
        pvol->fHWvlgthSeen = 0;
      }
      if (pvol->fHWvlgthNot) {
        delete pvol->fHWvlgthNot;
        pvol->fHWvlgthNot = 0;
      }
      if (pvol->fHQEff) {
        delete pvol->fHQEff;
        pvol->fHQEff = 0;
      }
      if (pvol->fHInside) {
        delete pvol->fHInside;
        pvol->fHInside = 0;
      }
    }
  }
  if (fNAPD) {
    TStatAPD *papd;
    TIter next3(&fAPD);
    while ((papd = (TStatAPD *)next3())) {
      if (papd->fHTimeSeen) {
        delete papd->fHTimeSeen;
        papd->fHTimeSeen = 0;
      }
      if (papd->fHWvlgthSeen) {
        delete papd->fHWvlgthSeen;
        papd->fHWvlgthSeen = 0;
      }
      if (papd->fHWvlgthNot) {
        delete papd->fHWvlgthNot;
        papd->fHWvlgthNot = 0;
      }
      if (papd->fHeVersusT) {
        delete papd->fHeVersusT;
        papd->fHeVersusT = 0;
      }
      if (papd->fHeVersusW) {
        delete papd->fHeVersusW;
        papd->fHeVersusW = 0;
      }
      if (papd->fHdCathode) {
        delete papd->fHdCathode;
        papd->fHdCathode = 0;
      }
      if (papd->fHQEff) {
        delete papd->fHQEff;
        papd->fHQEff = 0;
      }
      if (papd->fHInside) {
        delete papd->fHInside;
        papd->fHInside = 0;
      }
    }
  }
}
void TResults::DelPaveT() {
  //Delete the TPaveLabel and the TPaveStat of DoStat()
  if (fPavel) {
    delete fPavel;
    fPavel = 0;
  }
  if (fPavet) {
    delete fPavet;
    fPavet = 0;
  }
}
void TResults::DoStat() {
  //Shows photon statistics
  const Double_t un   = 1.0;
  const Double_t x1pl = 0.1;
  const Double_t y1pl = 0.9;
  const Double_t x2pl = un - x1pl;
  const Double_t y2pl = 0.98;
  const Double_t x1pt = 0.05;
  const Double_t y1pt = 0.05;
  const Double_t x2pt = un - x1pt;
  const Double_t y2pt = 0.85;
  TString s,sf;
  TString stitle;
  Double_t num,den,eff,def;
  stitle = GetName();
  DelPaveT();
  fPavel = new TPaveLabel(x1pl,y1pl,x2pl,y2pl,stitle.Data(),"BRNDCARC");
  fPavel->SetBorderSize(5);
  fPavel->SetFillColor(41);
  fPavel->SetTextColor(2);
  fPavet = new TPaveText(x1pt,y1pt,x2pt,y2pt,"BRNDCARC");
  fPavet->SetBorderSize(8);
  fPavet->SetFillColor(21);
  fPavet->SetTextAlign(12);
  fPavet->SetTextFont(101);
  fPavet->SetTextSize(0.03);
  s  = "Nb. of photons generated   : ";
  s += fNpGener;
  fPavet->AddText(s.Data());
  s  = "Lost for abnormal reasons  : ";
  s += fNpAbnorm;
  fPavet->AddText(s.Data());
  s  = "Lost because abs. length 0 : ";
  s += fNpAbsL0;
  fPavet->AddText(s.Data());
  s  = "Eff. nb. of gen. photons   : ";
  s += fNpGener  - fNpAbnorm - fNpAbsL0;
  fPavet->AddText(s.Data());
  s  = "Nb. of photons seen        : ";
  s += fNpSeen;
  fPavet->AddText(s.Data());
  s  = "Efficiency                 : ";
  num = fNpSeen;
  den = fNpGener  - fNpAbnorm - fNpAbsL0;
  eff = num/den;
  def = TMath::Sqrt(num)/den;
  sf = Form("%10.6g",eff);
  s.Append(sf);
  fPavet->AddText(s.Data());
  s  = "   error                   :  +/-";
  sf = Form("%10.6g",def);
  s.Append(sf);
  fPavet->AddText(s.Data());
  s  = "Lost for any reason        : ";
  s += fNpLossAny;
  fPavet->AddText(s.Data());
  s  = "Lost in materials          : ";
  s += fNpAbsMat;
  fPavet->AddText(s.Data());
  s  = "Lost before wrapping       : ";
  s += fNpAbsBef;
  fPavet->AddText(s.Data());
  s  = "Lost in wrapping           : ";
  s += fNpAbsRvt;
  fPavet->AddText(s.Data());
  s  = "Lost leaving setup         : ";
  s += fNpOutSide;
  fPavet->AddText(s.Data());
  s  = "Lost because seen too late : ";
  s += fNpTooLate;
  fPavet->AddText(s.Data());
  s  = "Lost b. too few e- in APD  : ";
  s += fNpTooFew;
  fPavet->AddText(s.Data());
  s  = "Lost b. acceptance angle   : ";
  s += fNpLossAng;
  fPavet->AddText(s.Data());
  s  = "Lost b. quantum efficiency : ";
  s += fNpLossQE;
  fPavet->AddText(s.Data());
  gPad->Clear();
  fPavel->Draw();
  fPavet->Draw();
  gPad->Modified();
  gPad->Update();
  if (gTwoPad) gTwoPad->ChangePad();
}
TStatAPD* TResults::GetAPD(const char *name) const {
  //  Given the name of a volume detector, GetVolDet() returns the pointer to
  //this detector, which allows for instance to reach the histograms of this
  //detector. This implies that you have given different names to different
  //detectors, which you should always do.
  Bool_t found = kFALSE;
  TString s;
  TStatAPD *p;
  TStatAPD *papd=0;
  TIter next(&fAPD);
  while ((!found) && (p = (TStatAPD *)next())) {
    s = p->GetName();
    if (!s.CompareTo(name)) {
      found = kTRUE;
      papd = p;
    } 
  }
  return papd;
}
TStatSurfD* TResults::GetSurfDet(const char *name) const {
  //  Given the name of a surface detector or of a phototube, GetSurfDet()
  //returns the pointer to this detector, which allows for instance to reach
  //the histograms of this detector. This implies that you have given diffe-
  //rent names to different detectors, which you should always do.
  Bool_t found = kFALSE;
  TString s;
  TStatSurfD *p;
  TStatSurfD *psurf=0;
  TIter next(&fSurfDet);
  while ((!found) && (p = (TStatSurfD *)next())) {
    s = p->GetName();
    if (!s.CompareTo(name)) {
      found = kTRUE;
      psurf = p;
    } 
  }
  return psurf;
}
TStatVolD* TResults::GetVolDet(const char *name) const {
  //  Given the name of a volume detector, GetVolDet() returns the pointer to
  //this detector, which allows for instance to reach the histograms of this
  //detector. This implies that you have given different names to different
  //detectors, which you should always do.
  Bool_t found = kFALSE;
  TString s;
  TStatVolD *p;
  TStatVolD *pvol=0;
  TIter next(&fVolDet);
  while ((!found) && (p = (TStatVolD *)next())) {
    s = p->GetName();
    if (!s.CompareTo(name)) {
      found = kTRUE;
      pvol = p;
    } 
  }
  return pvol;
}
void TResults::GInit() {
  //Groups everything common to all constructors
  //General
  Axis_t late,MinW,MaxW;
  late = TLitPhys::Get()->TooLate();
  MinW = TLitPhys::Get()->MinWaveL();
  MaxW = TLitPhys::Get()->MaxWaveL();
  fNpGener = 0;
  //Detection of photons
  fHPoints   = new TH1F("G_Points",
    "Number of points on photon trajectory",250,0.0,250.0);
  //  fHPoints->SetFillColor(kOrange+1);
  fNpSeen  = 0;
  fPhweigth = 0.0;
  fHTimeSeen = new TH1F("G_TimeSeen",
    "time of arrival (ps) of photons on detector",100,0.0,late);
  fHDistSeen = new TH1F("G_DistSeen",
    "distance (cm) travelled by photons seen in detector",100,0.0,400.0);
  fHWvlgthSeen = new TH1F("G_WvlgthSeen","Wavelength (nm) of photons seen",
    100,MinW,MaxW);
  //Special detectors statistics
  fNSurfDet = 0;
  fNVolDet = 0;
  fNAPD = 0;
  //Loss of photons
  fNpLossAny = 0;
  fNpAbsMat  = 0;
  fNpAbsBef  = 0;
  fNpAbsRvt  = 0;
  fNpOutSide = 0;
  fNpAbnorm  = 0;
  fNpTooLate = 0;
  fNpTooFew  = 0;
  fNpLossAng = 0;
  fNpLossQE  = 0;
  fNpAbsL0   = 0;
  fHAbsAny  = new TH1F("G_AbsAny",
    "distance (cm) travelled by photons absorbed anywhere",100,0.0,400.0);
  fHAbsMat  = new TH1F("G_AbsMat",
    "distance (cm) travelled by photons absorbed in material",100,0.0,400.0);
  fHAbsBef  = new TH1F("G_AbsBef",
    "distance (cm) of photons absorbed before revetment",100,0.0,400.0);
  fHAbsRvt  = new TH1F("G_AbsRvt",
    "distance (cm) of photons absorbed by revetment",100,0.0,400.0);
  fHOutSide = new TH1F("G_OutSide",
    "distance (cm) travelled by photons leaving the setup",100,0.0,400.0);
  fHAbnLost = new TH1F("G_AbnLost",
    "distance (cm) travelled by photons abnormally lost",100,0.0,400.0);
  fHTooLate = new TH1F("G_TooLate",
    "Wavelength of photons seen, but too late",
    100,MinW,MaxW);
  //Diffusion of photons
  fHDiffusion = new TH1F("G_Diffusion",
    "Number of diffusions per photon",16,0.0,16.0);
  //Shape and face statistics statistics
  Axis_t max,maxf;
  if (!gLit->fShapeNumbered)
    fNbOfShapes = gLit->GiveNbToShape();
  else
    fNbOfShapes = gLit->fNSupplShape;
  fMaxNbOfFaces = gLit->fMaxNbOfFaces;
  max = fNbOfShapes;
  fHStopShape = new TH1F("G_StopShape","Number of photons stopped in shape k",
    fNbOfShapes,0.0,max);
  maxf = fMaxNbOfFaces;
  fHStopFace  = new TH2F("G_StopFace",
    "Nb. of photons that stopped on shape k, face i",
    fNbOfShapes,0.0,max,fMaxNbOfFaces,0.0,maxf);
  fHHitRvt  = new TH2F("G_HitRvt",
    "Nb. of hits on revetment of face i of shape k",
    fNbOfShapes,0.0,max,fMaxNbOfFaces,0.0,maxf);
  fHHitFace   = new TH2F("G_HitFace",
    "Nb. of hits of photons on shape k, face i",
    fNbOfShapes,0.0,max,fMaxNbOfFaces,0.0,maxf);
  fHCrossFace   = new TH2F("G_CrossFace",
    "Nb. of crossing forward through shape k face i",
    fNbOfShapes,0.0,max,fMaxNbOfFaces,0.0,maxf);
  fHDontCross   = new TH2F("G_DontCross",
    "Nb. of not crossing forward through shape k face i",
    fNbOfShapes,0.0,max,fMaxNbOfFaces,0.0,maxf);
  //Time and Wavelength
  fHTimeAll = new TH1F("G_TimeAll","Generated time (ps) for all components",
    100,0.0,late);
  fHWvlgthAll = new TH1F("G_WvlgthAll","Generated wavelength (nm) for all components",
    100,MinW,MaxW);
  fHTimeCer = new TH1F("G_TimeCer","Generated time (ps) for Cerenkov photons",
    100,0.0,late);
  fHWvlgthCer = new TH1F("G_WvlgthCer","Generated wavelength (nm) for Cerenkov photons",
    100,MinW,MaxW);
  fNfluo = AtLeastOne();
  fHTimeEach   = 0;
  fHWvlgthEach = 0;
  if (fNfluo) {
    Axis_t afluo = fNfluo;
    fHTimeEach = new TH2S("G_TimeEach","Generated time for each component",
      100,0.0,late,fNfluo,0.0,afluo);
    fHWvlgthEach = new TH2S("G_WvlgthEach","Generated wavelength (nm) for each component",
      100,MinW,MaxW,fNfluo,0.0,afluo);
  }
  //Particle generation
  fNbPart = 0;
  fHPMomentum = new TH1F("G_Momentum","Momentum [Gev] of generated particles",
    100,0.0,100.0);
  fHPTheta = new TH1F("G_Theta","Theta distribution of generated particles",
    80,0.0,3.2);
  if (!gLit->fShapeNumbered) gLit->GiveNbToShape();
  fHEnergyInShape = new TH2D("G_EinShape","Energy [Mev] deposited by gamma in all shapes",
    gLit->fNSupplShape,0.0,(Double_t)gLit->fNSupplShape,50,0.0,fgEnergyOfGamma);
}
void TResults::InitP() {
  //Put all pointers to 0
  fPavel          = 0;
  fPavet          = 0;
  fHPoints        = 0;
  fHTimeSeen      = 0;
  fHDistSeen      = 0;
  fHWvlgthSeen    = 0;
  fHAbsAny        = 0;
  fHAbsMat        = 0;
  fHAbsBef        = 0;
  fHAbsRvt        = 0;
  fHOutSide       = 0;
  fHAbnLost       = 0;
  fHTooLate       = 0;
  fHDiffusion     = 0;
  fHStopShape     = 0;
  fHStopFace      = 0;
  fHHitRvt        = 0;
  fHHitFace       = 0;
  fHCrossFace     = 0;
  fHDontCross     = 0;
  fHTimeAll       = 0;
  fHTimeEach      = 0;
  fHWvlgthAll     = 0;
  fHWvlgthEach    = 0;
  fHTimeCer       = 0;
  fHWvlgthCer     = 0;
  fHPMomentum     = 0;
  fHPTheta        = 0;
  fHEnergyInShape = 0;
}
Bool_t TResults::IsEqual(const TObject *obj) const {
  return (fXparam==((TResults *)obj)->fXparam);
}
void TResults::N() {
  //displays next histogram if the user type "n", exit otherwise
  char c='n';
  gTwoPad->SelectPad1();
  while (c == 'n' ) {
    N1();
    cout << "Press n to see next histo, any other key to quit" << endl;
    cin >> c;
  }
}
void TResults::N1() {
  //For printing each histogram, one after the other
  if (!gTwoPad) gLit->BookCanvas();
  gTwoPad->ChangePad();
  if (gLit) gLit->RestoreComments();
  switch (fK) {
  case 0:
    gTwoPad->SetNameForSave(fHPoints->GetName());
    fHPoints->Draw();
    break;
  case 1:
    gTwoPad->SetNameForSave(fHTimeSeen->GetName());
    fHTimeSeen->Draw();
    break;
  case 2:
    gTwoPad->SetNameForSave(fHDistSeen->GetName());
    fHDistSeen->Draw();
    break;
  case 3:
    gTwoPad->SetNameForSave(fHWvlgthSeen->GetName());
    fHWvlgthSeen->Draw();
    break;
  case 4:
    gTwoPad->SetNameForSave(fHAbsAny->GetName());
    fHAbsAny->Draw();
    break;
  case 5:
    gTwoPad->SetNameForSave(fHAbsMat->GetName());
    fHAbsMat->Draw();
    break;
  case 6:
    gTwoPad->SetNameForSave(fHAbsBef->GetName());
    fHAbsBef->Draw();
    break;
  case 7:
    gTwoPad->SetNameForSave(fHAbsRvt->GetName());
    fHAbsRvt->Draw();
    break;
  case 8:
    gTwoPad->SetNameForSave(fHOutSide->GetName());
    fHOutSide->Draw();
    break;
  case 9:
    gTwoPad->SetNameForSave(fHAbnLost->GetName());
    fHAbnLost->Draw();
    break;
  case 10:
    gTwoPad->SetNameForSave(fHTooLate->GetName());
    fHTooLate->Draw();
    break;
  case 11:
    gTwoPad->SetNameForSave(fHDiffusion->GetName());
    fHDiffusion->Draw();
    break;
  case 12:
    gTwoPad->SetNameForSave(fHStopShape->GetName());
    fHStopShape->Draw();
    break;
  case 13:
    gTwoPad->SetNameForSave(fHStopFace->GetName());
    fHStopFace->Draw("LEGO2Z");
    break;
  case 14:
    gTwoPad->SetNameForSave(fHHitRvt->GetName());
    fHHitRvt->Draw("LEGO2Z");
    break;
  case 15:
    gTwoPad->SetNameForSave(fHHitFace->GetName());
    fHHitFace->Draw("LEGO2Z");
    break;
  case 16:
    gTwoPad->SetNameForSave(fHCrossFace->GetName());
    fHCrossFace->Draw("LEGO2Z");
    break;
  case 17:
    gTwoPad->SetNameForSave(fHDontCross->GetName());
    fHDontCross->Draw("LEGO2Z");
    break;
  case 18:
    gTwoPad->SetNameForSave(fHTimeAll->GetName());
    fHTimeAll->Draw();
    break;
  case 19:
    if (fHTimeEach) {
      gTwoPad->SetNameForSave(fHTimeEach->GetName());
      fHTimeEach->Draw("LEGO2Z");
    }
    break;
  case 20:
    gTwoPad->SetNameForSave(fHWvlgthAll->GetName());
    fHWvlgthAll->Draw();
    break;
  case 21:
    if (fHWvlgthEach) {
      gTwoPad->SetNameForSave(fHWvlgthEach->GetName());
      fHWvlgthEach->Draw("LEGO2Z");
    }
    break;
  case 22:
    gTwoPad->SetNameForSave(fHTimeCer->GetName());
    fHTimeCer->Draw();
    break;
  case 23:
    gTwoPad->SetNameForSave(fHWvlgthCer->GetName());
    fHWvlgthCer->Draw();
    break;
  case 24:
    gTwoPad->SetNameForSave(fHPMomentum->GetName());
    fHPMomentum->Draw();
    break;
  case 25:
    gTwoPad->SetNameForSave(fHPTheta->GetName());
    fHPTheta->Draw();
    break;
  case 26:
    gTwoPad->SetNameForSave(fHEnergyInShape->GetName());
    fHEnergyInShape->Draw("LEGO2Z");
    break;
  default:
    fK = 0;
    gTwoPad->SetNameForSave(fHPoints->GetName());
    fHPoints->Draw();
    break;
  }
  fK += 1;
  fK %= 27;
  gTwoPad->UpdateIt();
}
void TResults::RefreshDet() const {
  Int_t n;
  n = gLit->fNDetector;
  if (n) {
    Int_t i;
    Int_t nsurfdet = 0;
    Int_t nvoldet = 0;
    Int_t napd = 0;
    TDetector *pdet;
    KindOfDetector kind;
    TFace *face;
    TSupplShape *shape;
    TIter next(&gLit->fDetector);
    for (i=0;i<n;i++) {
      pdet  = (TDetector *)next();
      kind  = pdet->GetKind();
      shape = pdet->GetShape();
      face  = pdet->GetFace();
      switch (kind) {
      case none:
        break;
      case surfdet:
      case phototube:
        if (fRun == 0) face->SetDetNumber(nsurfdet);
        nsurfdet++;
        break;
      case volumedet:
        if (fRun == 0) shape->SetDetNumber(nvoldet);
        nvoldet++;
        break;
      case apd:
        if (fRun == 0) {
          shape->SetDetNumber(napd);
        }
        napd++;
        break;
      }
    }
  }
}
void TResults::ResetCounters() {
  //counters to 0
  fK    = 0;
  fKdet = 0;
  fKDH  = 0;
}
