// @(#)root/html:$Id: TLitGlob.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TLitGlob
#define LITR_TLitGlob
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitGlob                                                             //
//                                                                      //
// Defines a "global" class TLitGlob whose incarnation gLit must be     //
// available during the whole execution of Litrani                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TString.h"
#include "TLatex.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TReference.h"

class TLitGlob : public TObject {

protected:

  void         InitP();
  void         InitRef();
  void         SetDate();

public:

  TReference  *fRef;           //Reference of program
  Int_t        fTopXGen;       //top x  of general canvas fCanvas
  Int_t        fTopYGen;       //top y  of general canvas fCanvas
  Short_t      fSize;          //Size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
  Int_t        fNbButtons;     //Number of buttons wanted in TwoPadDisplay
  Int_t        fWidthGen;      //width  of general canvas fCanvas
  Int_t        fHeigthGen;     //heigth of general canvas fCanvas
  TString      fDate;          //Date when program runs
  TString      fTime;          //Time when program runs
  Bool_t       fBTRchanged;    //True if default comments have been changed
  TString      fTRComment;     //Comment appearing at top right of main canvas
  TString      fBRComment;     //Comment appearing at bottom right of main canvas
  Bool_t       fSaveFitSpec;   //If true, TSplinefit fits used and TLitSpectrum spectra used are saved on root file. Default
  Bool_t       fSeen;          //true if photon seen
  Double_t     fX0;            //x coordinate of death of seen photons
  Double_t     fY0;            //y coordinate of death of seen photons
  Double_t     fZ0;            //z coordinate of death of seen photons
  Double_t     fOldWaveLength; //old value of wavelength in nanometers
  Int_t        fNfamily;       //Number of families of fits
  TObjArray    fFamily;        //List of all families of fits
  Int_t        fNOptMat;       //Number of optical materials
  TObjArray    fOptMat;        //List of all optical materials
  Int_t        fNRevetment;    //Number of revetments
  TObjArray    fRevetment;     //List of all revetments
  Int_t        fNThinSlice;    //Number of thin slices
  TObjArray    fThinSlice;     //List of all thin slices
  Int_t        fNContact;      //Number of contacts
  TObjArray    fContact;       //List of all contacts
  Bool_t       fShapeNumbered; //All Shapes have been numbered
  Int_t        fMaxNbOfFaces;  //Maximum number of faces for a shape in this setup
  Int_t        fNSupplShape;   //Number of TSupplShape
  TObjArray    fSupplShape;    //List of all TSupplShape
  Int_t        fNDetector;     //Number of detectors
  TObjArray    fDetector;      //List of all detectors
  Int_t        fNResults;      //Number of run statistics stored
  TObjArray    fResults;       //List of all statistics of run
  Int_t        fNParticle;     //Number of particles
  TObjArray    fParticle;      //List of all particles
  Int_t        fNBeam;         //Number of beams
  TObjArray    fBeam;          //List of all beams
  Int_t        fNSpontan;      //Number of spontaneous sources
  TObjArray    fSpontan;       //List of all spontaneous sources
  Int_t        fNCallEL;       //number of times EmitLight is called
  //
  //  Info_M on file
  //
  TString      fLitFileName;   //name of .root file created by Litrani
  TFile       *fLitFile;       //.root file created by Litrani
  Int_t        fLitBufSize;    //buffer size of file
  Int_t        fLitNb;         //pointer on actual size of file
  TTree       *fLitTree;       //tree of histos and statistics for each run.
  TBranch     *fLitBranch;     //branch of tree
  Int_t        fNFits;         //Number of fits in fFitColl
  Int_t        fKFit;          //Counter of fits drawn
  TObjArray   *fFitColl;       //Collection of fits
  Int_t        fNSpecs;        //Number of spectra in fSpecColl
  Int_t        fKSpec;         //Counter of spectra drawn
  TObjArray   *fSpecColl;      //Collection of spectra
  Bool_t       fFilesClosed;   //True if summary files have been closed
  //
  //  Printing
  //
  Int_t     fPrintFreq;        //Frequency of print for creation of new photons

  TLitGlob() { InitP(); }
  TLitGlob(Short_t, Int_t = 24,Int_t = 2, Int_t = 2,const char* = "litrani",
    const char* = "litrani",Bool_t WithDate = kFALSE);
  virtual ~TLitGlob();
  Bool_t AddOneBeam(const TObject *,const char *);
  Bool_t AddOneBeam(const TObject *,const TString&);
  Bool_t AddOneContact(const TObject *,const char *);
  Bool_t AddOneContact(const TObject *,const TString&);
  Bool_t AddOneDetector(const TObject *,const char *);
  Bool_t AddOneDetector(const TObject *,const TString&);
  Bool_t AddOneOptMat(const TObject *,const char *);
  Bool_t AddOneOptMat(const TObject *,const TString&);
  Bool_t AddOneParticle(const TObject *,const char *);
  Bool_t AddOneParticle(const TObject *,const TString&);
  Bool_t AddOneResult(const TObject *,const char *);
  Bool_t AddOneResult(const TObject *,const TString&);
  Bool_t AddOneRevetment(const TObject *,const char *);
  Bool_t AddOneRevetment(const TObject *,const TString&);
  Bool_t AddOneSpontan(const TObject *,const char *);
  Bool_t AddOneSpontan(const TObject *,const TString&);
  Bool_t AddOneSupplShape(const TObject *,const char *);
  Bool_t AddOneSupplShape(const TObject *,const TString&);
  Bool_t AddOneThinSlice(const TObject *,const char *);
  Bool_t AddOneThinSlice(const TObject *,const TString&);
  void   BookCanvas();
  void   ChangePad();
  Bool_t CheckContacts();
  Bool_t CheckOverlaps();
  void   CloseFiles();
  void   CloseFilesR();
  void   DeleteCanvas();
  void   DrawNextFit();
  void   DrawNextSpectrum();
  void   F();
  Int_t  GiveNbToShape();
  void   HistosManager();
  void   NameIOFile(const char *);
  void   NewWavelength(Double_t wavelength,Bool_t = kFALSE);
  void   NF() const;
  void   OpenFilesR();
  void   OpenFilesR(const char*);
  void   OpenFilesW(Bool_t = kTRUE);
  void   PrintAllBeam() const;
  void   PrintAllContact() const;
  void   PrintAllDetector() const;
  void   PrintAllOptMat() const;
  void   PrintAllParticle() const;
  void   PrintAllRevetment() const;
  void   PrintAllSpontan() const;
  void   PrintAllSupplShape() const;
  void   PrintAllThinSlice() const;
  void   PrintGlobalResults() const;
  void   PublishVersion() const;
  void   RestoreComments();
  Bool_t SaveFitSpec() const    { return fSaveFitSpec; }
  void   SaveFitSpec( Bool_t b) { fSaveFitSpec = b;    }
  Bool_t SelectRun(Int_t);
  Bool_t SelectRun(TString, Int_t);
  void   SetPrintFreq(Int_t f)  { fPrintFreq = f;      }
  void   VerifyOutside() const;
  ClassDef(TLitGlob,0)   //global variables for litrani
};
R__EXTERN TLitGlob *gLit;
#endif
