// @(#)root/html:$Id: TVisuLit.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef ROOT_TVisuLit
#define ROOT_TVisuLit

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TVisuLit                                                             //
//                                                                      //
// Visualize the ROOT files produced by Litrani                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TFile.h"
#include "TTree.h"
#include "TGMenu.h"
#include "TRootEmbeddedCanvas.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGListBox.h"
#include "TGFileDialog.h"
#include "TReference.h"
#include "TResults.h"

enum ToBeDrawn {ResultsVsPar,RunsResults,DetResults,FitsUsed,SpectraUsed};
enum AllMenusID { M_FILE_OPEN,M_FILE_CLOSE,M_FILE_EXIT};

class TVisuLit : public TGMainFrame {

protected:

  //Canvas
  UInt_t               fWidthGen;        //Width  of general canvas fCanvas
  UInt_t               fHeigthGen;       //Heigth of general canvas fCanvas
  Int_t                fNFits;           //Number of fits in fFitColl
  Int_t                fKFit;            //Counter of fits drawn
  TObjArray           *fFitColl;         //Collection of fits
  Int_t                fNSpecs;          //Number of spectra in fSpecColl
  Int_t                fKSpec;           //Counter of spectra drawn
  TObjArray           *fSpecColl;        //Collection of spectra
  TReference          *fRef;             //References concerning program and authors
  TCanvas             *fCanvas;          //Canvas of fECanvas
  TString              fDate;            //date of to-day put in 3rd label
  TString              fTime;            //Time when program runs
  Bool_t               fBTRchanged;      //True if comments BR and TR have changed
  TString              fTextBR;          //Initial bottom right label
  Bool_t               fInColor;         //True if display in color, not in grey
  //File
  Bool_t               fFileClosed;      //True if Litrani file has been closed
  TString              fFileName;        //Name of file to open
  TFile               *fLitFile;         //Litrani file to be read
  Int_t                fLitBufSize;      //buffer size of file
  Int_t                fLitNb;           //pointer on actual size of file
  TTree               *fLitTree;         //tree of histos and statistics for each run.
  TBranch             *fLitBranch;       //branch of tree
  TObjArray            fListRuns;        //collection of runs
  //Widgets
  ToBeDrawn            fDrawChoice;      //Kind of histos to be plotted
  TGLayoutHints       *fLMenu;           //Layout of menu bar
  TGLayoutHints       *fLMenuItem;       //Layout of left menus in menu bar
  TGFileInfo          *fFi;              //Used in meu File/Open
  TGMenuBar           *fMenuBar;         //Bar holding menus
  TGPopupMenu         *fMenuFile;        //File menu
  TGLayoutHints       *fL1;              //Layout of fEcanvas in fHFrame1
  TGLayoutHints       *fL2;              //Layout of fHFrame1 in this
  TGLayoutHints       *fL3;              //Layout of fDrawButton in fHFrame2
  TGLayoutHints       *fL4;              //Layout of fStatButton in fHFrame2
  TGLayoutHints       *fL5;              //Layout of fHFrame2 in this
  TGLayoutHints       *fL6;              //Layout of fListBox in fHFrame1
  TGLayoutHints       *fL7;              //Layout of fButtonGroup in fHFrame2
  TGLayoutHints       *fL8;              //Layout of radio buttons in fButtonGroup
  TGHorizontalFrame   *fHFrame1;         //Horizontal frame up
  TGHorizontalFrame   *fHFrame2;         //Horizontal frame down
  TRootEmbeddedCanvas *fEcanvas;         //RootEmbeddedCanvas in fHFrame1, at right
  TGTextButton        *fDrawButton;      //Draw Button
  TGTextButton        *fStatButton;      //Plot statistics Button
  TGTextButton        *fOneTwoButton;    //Change from 2 pads to 1 pad or vice-versa
  TGTextButton        *fLinLogButton;    //Switch from linear Y log scale to logarithmic or vice-versa
  TGTextButton        *fColorButton;     //Change display from color to grey or vice-versa
  TGTextButton        *fPrintButton;     //Save canvas of VisuLitrani
  TGListBox           *fListBox;         //List Box showing all runs in file
  Int_t                fListBoxItem;     //Item selected in fListBox
  TGGroupFrame        *fButtonGroup;     //Contains list of radio buttons
  TGRadioButton       *fRadioPar;        //Choosing display of results versus run-dependent parameter
  TGRadioButton       *fRadioRun;        //Choosing display of results for a specific run or sum of all runs
  TGRadioButton       *fRadioDet;        //Choosing display of results of detectors for a specific run or sum of all runs
  TGRadioButton       *fRadioFit;        //Choosing display of fits of type TSplineFit used for this generation of photons
  TGRadioButton       *fRadioSpec;       //Choosing display of spectra of type TLitSpectrum used for this generation of photons

  void      BookCanvas();
  void      CloseFile(Bool_t = kTRUE);
  void      FirstRead();
  void      Init();
  void      InitP();
  void      InitRef();
  void      OpenFileR();
  void      PrepareMenus();
  TResults *SelectRun();

public:

  TVisuLit()                           { InitP();           }
  TVisuLit(const TGWindow *p,UInt_t w, UInt_t h);
  virtual    ~TVisuLit();
  void        ChangePadLogX();
  void        ChangePadLogY();
  void        DoDraw();
  void        DoRadio();
  void        DoStat();
  void        DrawNextFit();
  void        DrawNextSpectrum();
  TCanvas    *GetCanvas() const        { return fCanvas;    }
  UInt_t      GetHeigthGen() const     { return fHeigthGen; }
  TReference *GetRef() const           { return fRef;       }
  UInt_t      GetWidthGen() const      { return fWidthGen;  }
  void        HandleMenu(Int_t);
  void        PrintCanvas();
  void        ToggleColor();
  void        ToggleState();
  ClassDef(TVisuLit,0)  //Visualize the ROOT files produced by Litrani
};
R__EXTERN TVisuLit *gVisuLit;
#endif
