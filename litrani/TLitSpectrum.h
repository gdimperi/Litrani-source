// @(#)root/html:$Id: TLitSpectrum.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LITR_TLitSpectrum
#define LITR_TLitSpectrum
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitSpectrum   Emission spectrum of an optical material              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TObjArray.h"
#include "TH1.h"
#include "TGraph.h"
#include "TFile.h"

Double_t SumOfGaussians(Double_t*,Double_t *);

class TPolyMarker;
class TBranch;
class TTree;
class TSplineFit;
class TLitSpectrumCp;

class TLitSpectrum : public TNamed {

protected:

  Bool_t               fNormalized;      //True if sum of amplitudes of components, TLitSpectrumCp::fGaussAmp, is 1
  Bool_t               fInSplineFitDB;   //The spectrum to be decomposed into gaussians exists in SplineFitDB.rdb
  Int_t                fNbComponents;    //Nb. of TLitSpectrumCp components in fComponents
  Double_t            *fArea;            //[fNbComponents] Area of values of normalized amplitudes of components, helping in the random choice of which component will emit photon
  TObjArray            fComponents;      //Collection of gaussian components
  TString              fDate;            //Date of creation of this spectrum
  TString              fSource;          //Source of the measurements
  TString              fMacro;           //Name of CINT macro producing this spectrum
  TString              fXLabel;          //Label of the x axis
  TString              fVLabel;          //Label of the value axis
  TSplineFit          *fSplineFit;       //! Pointer to the SplineFit showing the spectrum if it exists
//
  static Int_t         fgNextDraw;       //Next fit to be drawn
  static TFile        *fgSpectrumFile;   //"Database" file containing optical emission spectra
  static TTree        *fgSpectrumTree;   //Tree of spectra
  static TBranch      *fgSpectrumBranch; //Branch of spectra
  static TString      *fgFileName;       //Name or TreeName of "database" file containing spectra

  void                 FindDate();
  void                 Init();

public:

  TString              fComment[10];     //A comment of maximum 10 lines can be stored with this spectrum
  TGraph              *fFitGraph;        //! Graph of the fitted spectrum
  TH1F                *fH;               //! Histo to show spectrum and gaussian fit on spectrum
  TPolyMarker         *fPM;              //! Polymarker to show where the peaks are

  static TObjArray    *fgSpectra;        //Collection of all TLitSpectrum


  TLitSpectrum();
  TLitSpectrum(const char*,const char*,Bool_t = kFALSE);
  TLitSpectrum(const TLitSpectrum&);
  virtual             ~TLitSpectrum();
  void                 AddOneComponent(Double_t,Double_t,Double_t);
  void                 AddOneComponent(Double_t,Double_t,Double_t,Double_t,Double_t=-1.0,Double_t=1.0);
  void                 AddOneComponent(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  void                 AddOneComponent(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  void                 AddOneComponent(Double_t,Double_t,Double_t,Int_t,Double_t*,Double_t*,Double_t*);
  static void          AddSpectrum(TLitSpectrum*);
  void                 AddThisSpectrum();
  void                 AddTimeComponent(Double_t,Double_t,Double_t,Double_t);
  void                 AddTimeComponent(TLitSpectrumCp*,Double_t,Double_t,Double_t);
  Bool_t               BetterPeaks(Int_t,Double_t*,Double_t*,Double_t*,Short_t*,Bool_t= kTRUE);
  void                 Clear();
  void                 ClearGraphs();
  Int_t                Compare(const TObject*) const;
  void                 DrawDecayTimes(Int_t=500000);
  static void          DrawNextInCollection();
  static void          DrawSpectraInCollection();
  static void          DrawSpectraInFile();
  void                 DrawSpectrum(Int_t=501);
  TLitSpectrumCp      *FindComponent(Double_t) const;
  void                 FindFit(Bool_t = kFALSE);
  Bool_t               FindPeaks(Int_t=80,Double_t=0.002,Double_t = 14.0,Double_t tooclose=16.0,Bool_t = kFALSE,Bool_t = kTRUE);
  static TLitSpectrum *FindSpectrum(const char*,Bool_t = kTRUE);
  Int_t                GenWaveLAndDecayT(Double_t&,Double_t&);
  Bool_t               GetInSplineFitDB() const  { return fInSplineFitDB;               }
  const char          *GetMacro() const          { return fMacro.Data();                }
  Int_t                GetNbOfComponents() const { return fNbComponents;                }
  Bool_t               GetNormalized() const     { return fNormalized;                  }
  TSplineFit          *GetSplineFit() const      { return fSplineFit;                   }
  static void          InitStatic();
  Bool_t               IsEqual(const TObject*) const;
  Bool_t               IsInCollection() const;
  static Bool_t        IsInCollection(TLitSpectrum*);
  Bool_t               IsSortable() const        { return kTRUE;                        }
  static void          NameFile(const char* = "SpectraDB.rdb");
  void                 Normalize();
  static Bool_t        OrderFile(Bool_t = kTRUE);
  void                 Print() const;
  void                 PrintComment() const;
  void                 PrintSpectrum2File(const char*, Int_t, Int_t=0, Double_t cutoff = 1.E-3) const;
  static void          Purge();
  static void          PurgeStatic();
  static Bool_t        RemoveSpectrumFromFile(const char*,Bool_t=kTRUE);
  void                 SetDefaultLabels();
  void                 SetMacro(const char*);
  void                 SetSource(const char*);
  void                 SetVLabel(const char*);
  void                 SetXLabel(const char*);
  static void          ShowSpectraInFile();
  Bool_t               UpdateFile(Bool_t=kFALSE);
  Bool_t               VerifyNotInFile() const;
  ClassDef(TLitSpectrum,1)  //Emission spectrum of an optical material
};
R__EXTERN Int_t        gLitSpecNb;
#endif
