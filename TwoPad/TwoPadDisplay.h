// @(#)root/html:$Id: TwoPadDisplay.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TwoPadDisplay
#define LIT_TwoPadDisplay
///////////////////////////////////////////////////////////////////////////
//                                                                       //
// TwoPadDisplay    Useful to show drawings or histograms into 2 pads    //
//                                                                       //
///////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "TObjString.h"
#include "TLatex.h"
#include "TObjArray.h"
#include "TCanvas.h"
#include "TButton.h"
#include "TReference.h"

#undef OLDROOT

enum StateOfPads {Pad1AndPad2,Pad1Only,Pad2Only,OnePad};

class TwoPadDisplay : public TCanvas {

protected:

  Bool_t         fCanDelete;       //Allows to prevent delete of TwoPadDisplay if not wanted
  TReference    *fRef;             //References concerning program and authors
  Bool_t         fIsOwnerOfRef;    //True if fRef was not given by the user and was created as the default of TwoPadDisplay
  TString        fNameForSave;     //Name to be given to fCanvas when it is saved
  TString        fExtForSave;      //Type of file used when saving canvas [ps, eps, pdf,...]
  Short_t        fSize;            //Size of canvas, [ 1 <= size <= 8 ]
  TPad          *fPad1;            //First pad
  TPad          *fPad2;            //Second pad
  Bool_t         fUsePad2;         //Change used pad
  StateOfPads    fStateOfPads;     //4 different ways to use the 2 pads
  Bool_t         fGrey;            //True if all grey, no color [default false]
  Int_t          fPadLogX;         //log display in x
  Int_t          fPadLogY;         //log display in y
  TString        fDate;            //Date when program runs
  TString        fTime;            //Time when program runs
  TLatex        *fTexTL;           //User updatable label 1 on top    left  of main canvas
  TString        fStringTL;        //Used to store title of fTexTL
  Bool_t         fDefTexTL;        //True if default label TL used
  Double_t       fXTexTL;          //x position of label 1
  Double_t       fYTexTL;          //y position of label 1
  TLatex        *fTexBL;           //user updatable label 2 on bottom left  of main canvas
  TString        fStringBL;        //Used to store title of fTexBL
  Bool_t         fDefTexBL;        //True if default label BL used
  Double_t       fXTexBL;          //x position of label 2
  Double_t       fYTexBL;          //y position of label 2
  TLatex        *fTexBR;           //fixed          label 3 on bottom right of main canvas
  TString        fStringBR;        //Used to store title of fTexBR
  Bool_t         fDefTexBR;        //True if default label BR used
  Double_t       fXTexBR;          //x position of label 3
  Double_t       fYTexBR;          //y position of label 3
  TLatex        *fTexTR;           //fixed          label 4 on top    right of main canvas
  TString        fStringTR;        //Used to store title of fTexTR
  Bool_t         fDefTexTR;        //True if default label TR used
  Double_t       fXTexTR;          //x position of label 4
  Double_t       fYTexTR;          //y position of label 4
  Font_t         fFontTex;         //font used for label
  Float_t        fSizeTex;         //size of label font
  Width_t        fWidthTex;        //line width for label text
  Color_t        fColCan;          //Color of main canvas
  Style_t        fStyleCan;        //Fill style of main canvas
  Int_t          fStyleStat;       //What to plot in stat
  Color_t        fStyleHistColor;  //fill color of histograms
  Color_t        fStyleTitleColor; //Color of title
  Color_t        fStyleTFColor;    //Fill color for title
  Color_t        fStyleTTextColor; //Color text of title
  Short_t        fBszCan;          //Border size of main canvas
  Double_t       fPlaceLeft;       //Place at left of pad 1, to put buttons, in case fNbButtons != 0
  Double_t       fEmptyX;          //Empty space between pads in x
  Double_t       fEmptyY;          //Empty space up and down pads
  Int_t          fNbButtons;       //Number of buttons wanted
  TObjArray      fCollBut;         //Collection of TButton
  TObjArray      fButNames;        //Collection of TButton names
  TObjArray      fButActions;      //Collection of TButton actions
  Double_t       fHeightBut;       //Height of one button
  Double_t       fFreeYBut;        //Space in y between buttons
  Double_t       fXLowButton;      //XLow of all buttons
  Double_t       fXUpButton;       //Xup of all buttons
  Font_t         fFontButton;      //Font used in buttons
  Color_t        fColorBut;        //Color of button
  Short_t        fBszBut;          //Border size of buttons
  Float_t        fTextSBut;        //Size of text in button
  Double_t       fXLowPad1;        //XLow of fPad1 in fCanvas
  Double_t       fYLowPad;         //YLow of fPad1 and fPad2 in fCanvas
  Double_t       fXUpPad1;         //XUp  of fPad1 in fCanvas
  Double_t       fYUpPad;          //YUp  of fPad1 in fCanvas
  Color_t        fColPad1;         //Color of fPad1
  Style_t        fStylePad1;       //Fill style of pad1
  Double_t       fXLowPad2;        //XLow of fPad2 in fCanvas
  Double_t       fXUpPad2;         //XUp  of fPad2 in fCanvas
  Color_t        fColPad2;         //Color of fPad2
  Style_t        fStylePad2;       //Fill style of pad2
  Short_t        fBszPad;          //Border size of both pads
  Color_t        fColFrame;        //Color of frame of pad 1 or 2

  void           BookButtons();
  void           BookCanvas();
  void           BookLabels();
  void           BookPad1();
  void           BookPad2();
  void           CommentTL();
  void           CommentBL();
  void           CommentBR();
  void           CommentTR();
  void           CreateContent();
  void           DefRef();
  void           DeleteContent();
  void           DeleteLabels();
  void           DeletePads();
  void           Init();
  void           InitP();
  void           RecalcX();
  void           RecalcY();
  void           SetDate();

public:


  TwoPadDisplay()                      { InitP();             }
  TwoPadDisplay(TReference*,Short_t = 4,Int_t = 24,Int_t = 2,Int_t = 2);
  TwoPadDisplay(TReference*,Int_t,Int_t,Int_t,UInt_t,UInt_t);
  TwoPadDisplay(TReference*,TCanvas*,Int_t = 24,Bool_t = kFALSE);
  TwoPadDisplay(const char*,Int_t,Int_t,Int_t,TReference*,Int_t=24);
  virtual    ~TwoPadDisplay();
  Bool_t      CanDelete() const        { return fCanDelete;   }
  void        CanDelete(Bool_t b)      { fCanDelete = b;      }
  void        ChangePad();
  void        ChangePadLogX();
  void        ChangePadLogY();   // *MENU*
  void        CommentTL(const char*);
  void        CommentBL(const char*);
  void        CommentBR(const char*);
  void        CommentTR(const char*);
  void        DefCommentTL();
  void        DefCommentBL();
  void        DefCommentBR();
  void        DefCommentTR();
  void        DrawLabels();
  void        Editor(const char * = "");
  Short_t     GetBorderCan() const     { return fBszCan;      }
  Short_t     GetBorderPad() const     { return fBszPad;      }
  TButton    *GetButton(Int_t);
  TCanvas    *GetCanvas() const        { return fCanvas;      }
  Color_t     GetColCan() const        { return fColCan;      }
  Color_t     GetColFrame() const      { return fColFrame;    }
  Color_t     GetColPad1() const       { return fColPad1;     }
  Color_t     GetColPad2() const       { return fColPad2;     }
  Double_t    GetEmptyX() const        { return fEmptyX;      }
  Double_t    GetEmptyY() const        { return fEmptyY;      }
  Font_t      GetFontTex() const       { return fFontTex;     }
  Int_t       GetNbButtons() const     { return fNbButtons;   }
  TPad       *GetPad1() const          { return fPad1;        }
  TPad       *GetPad2() const          { return fPad2;        }
  Int_t       GetPadLogX() const       { return fPadLogX;     }
  Int_t       GetPadLogY() const       { return fPadLogY;     }
  Double_t    GetPlaceLeft() const     { return fPlaceLeft;   }
  TReference *GetRef() const           { return fRef;         }
  Float_t     GetSizeTex() const       { return fSizeTex;     }
  StateOfPads GetStateOfPads() const   { return fStateOfPads; }
  Style_t     GetStyleCan() const      { return fStyleCan;    }
  Style_t     GetStylePad1() const     { return fStylePad1;   }
  Style_t     GetStylePad2() const     { return fStylePad2;   }
  Int_t       GetStyleStat() const     { return fStyleStat;   }
  Bool_t      GetUsePad2() const       { return fUsePad2;     }
  Width_t     GetWidthTex() const      { return fWidthTex;    }
  Double_t    GetXLowPad1() const      { return fXLowPad1;    }
  Double_t    GetXLowPad2() const      { return fXLowPad2;    }
  Double_t    GetXUpPad1() const       { return fXUpPad1;     }
  Double_t    GetXUpPad2() const       { return fXUpPad2;     }
  Double_t    GetYLowPad() const       { return fYLowPad;     }
  Double_t    GetYUpPad() const        { return fYUpPad;      }
  Double_t    GetXTexTL() const        { return fXTexTL;      }
  Double_t    GetXTexBL() const        { return fXTexBL;      }
  Double_t    GetXTexBR() const        { return fXTexBR;      }
  Double_t    GetXTexTR() const        { return fXTexTR;      }
  Double_t    GetYTexTL() const        { return fYTexTL;      }
  Double_t    GetYTexBL() const        { return fYTexBL;      }
  Double_t    GetYTexBR() const        { return fYTexBR;      }
  Double_t    GetYTexTR() const        { return fYTexTR;      }
  void        GreyOrColor();
  void        SaveCanvas();
  void        SelectCanvas();
  void        SelectPad1();
  void        SelectPad2();
  void        SetAllColor();   // *MENU*
  void        SetAllGrey();    // *MENU*
  void        SetBorderCan(Short_t);
  void        SetBorderPad(Short_t);
  void        SetButton(Int_t,const char *,const char *);
  void        SetButtonFast(Int_t,const char *,const char *);
  void        SetColCan(Color_t);
  void        SetColFrame(Color_t);
  void        SetColPad1(Color_t);
  void        SetColPad2(Color_t);
  void        SetEmptyX(Double_t);
  void        SetEmptyY(Double_t);
  void        SetExtForSave(const char *);
  void        SetFontTex(Font_t);
  void        SetNameForSave(const char* n) { fNameForSave = n; }
  void        SetNbButtons(Int_t);
  void        SetPadLogX(Int_t L);
  void        SetPadLogY(Int_t L);
  void        SetPlaceLeft(Double_t d);
  void        SetRef(TReference*,Bool_t = kFALSE);
  void        SetSizeTex(Float_t);
  void        SetStateOfPads(StateOfPads);
  void        SetStyleCan(Style_t s);
  void        SetStylePad1(Style_t s);
  void        SetStylePad2(Style_t s);
  void        SetStyleStat(Int_t k);
  void        SetWidthTex(Width_t);
  void        SetXTexTL(Double_t);
  void        SetYTexTL(Double_t);
  void        SetXTexBL(Double_t);
  void        SetYTexBL(Double_t);
  void        SetXTexBR(Double_t);
  void        SetYTexBR(Double_t);
  void        SetXTexTR(Double_t);
  void        SetYTexTR(Double_t);
  void        ToggleButtons(); // *MENU*
  void        ToggleState();   // *MENU*
  void        UpdateCanvas();
  void        UpdateIt();
  void        UpdatePad1();
  void        UpdatePad2();
  ClassDef(TwoPadDisplay,1) //Useful to show drawings or histograms into 2 pads
};
R__EXTERN TwoPadDisplay *gTwoPad;
#endif
