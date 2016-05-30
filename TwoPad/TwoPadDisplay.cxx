// @(#)root/html:$Id: TwoPadDisplay.cxx 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TSystem.h"
#include "Riostream.h"
#include "TStyle.h"
#include "TGClient.h"
#include "TDatime.h"
#include "TPad.h"
#include "TGRootIDE.h"
#include "TwoPadDisplay.h"

TwoPadDisplay* gTwoPad = 0;

ClassImp(TwoPadDisplay)
//______________________________________________________________________________
//
//  Useful to show drawings or histograms into 2 pads. The references
// concerning the running program must have been established before booking
// TwoPadDisplay. See Class TReference.
//  There are 4 modes of using the 2 pads fPad1 and fPad2, according to the
// class variable fStateOfPads, of type enum StateOfPads:
//
// (1) - Pad1AndPad2   In this mode the histograms are plotted alternatively
//                   on fPad1 and fPad2 when you go from one histogram to
//                   the next. It is the default mode
// (2) - Pad1Only      In this mode the histograms are plotted only on fPad1.
//                   fPad2 is left untouched with its content when you go from
//                   one histogram to the next.
// (3) - Pad2Only      In this mode the histograms are plotted only on fPad2.
//                   fPad1 is left untouched with its content when you go from
//                   one histogram to the next.
// (4) - OnePad        In this mode, there is only one pad: fPad1. You then
//                   have twice the number of pixels in x. Useful if small
//                   screen
//  Modes Pad1Only and Pad2Only are useful if one wants to bring side by
// side two histograms which are not displayed one after the other in the
// sequence of plots of histograms. You change the state by using method
// SetStateOfPads() or ToggleState() or simpler by hitting TButton 2.
//  You can change almost everything in the appearance of the canvas using
// the Set... methods and using all the TSyle methods available with
// gStyle->Set... Look at all these Set... methods.
//  To switch from one pad to the other if you are in mode Pad1AndPad2, use
// method ChangePad(). If not in mode Pad1AndPad2, ChangePad() simply selects
// the required pad.
//  Four comments appear at the (top,left), (bottom,left), (bottom,right)
// and (top,right) of the main canvas. You can change the content of these
// four comments using the CommentXX(const char *) methods, or return to the
// default values of these four comments using the DefCommentXX() methods.
// The default values of these comments are filled with the information
// contained in the TReference class, that you have to define first before
// booking TwoPadDisplay.
//  The title of the main canvas is also established using informations
// found in class TReference:
// - name of program running
// - short description of program running
// - web site describing the program running
//
//  Calling any constructor of TwoPadDisplay [ except the default constructor ]
// define the global variable TwoPadDisplay *gTwoPad that can be used also
// within a CINT macro.
//
//  At the left of the left pad fPad1 is a list of programmable buttons.
// The number of such buttons is given in the constructors. It has to be
// greater than or equal to 5 and smaller than 48. The default value is 24.
// This number can be changed later on by calling method SetNbButtons(). As
// created by the method BookButtons(), these buttons are doing nothing useful,
// simply writing their name if clicked, or saving the canvas in various modes,
// except for the first 5 buttons:
//
//  - button0 exit from program
//  - button1 change the y scale from linear to logarithmic or vice-versa
//  - button2 go through the 4 modes of enum StateOfPads described above
//  - button3 change all colors to grey or vice-versa
//  - button4 invokes the Root editor for macros. A 2nd click cancels.
//
//  For the other buttons, it is up to the user from his CINT macro, or to
// the program using TwoPadDisplay to put a useful action inside some or all
// of the remaining buttons. For this, use method SetButton(). For instance,
// calling:
//
// - gTwoPad->SetButton(7,"demos",".x demos.C");
//
// will change the title of the button to "demos", and will activate the
// ROOT CINT macro demos.C when the button is clicked.
//  It is also allowed to have 0 buttons. In that case, the place occupied by
// buttons is taken back by the pads. You can change the number of buttons
// by calling method SetNbButtons().
//  TwoPadDisplay inherits from TCanvas, so that it was possible, by appending
// comment // *MENU* to some of the methods to have them executed from the
// context menu of the canvas TwoPadDisplay. It is the case for:
//
//  ChangePadLogY()
//  SetAllColor()
//  SetAllGrey()
//  ToggleButtons()    [which allow to switch from a state xith or without buttons]
//  ToggleState()
//
//  It is particularly useful when you are in the state "without" buttons: you
// regain here the possibilities offered by the buttons.
//
//  Here is a short CINT macro allowing to check TwoPadDisplay:
//
/*
{
TwoPadDisplay *d;
TReference *ref;
TAuthor *a, *b;
ref = new TReference("Litrani","Monte-Carlo of optical photons","3_9",
"http://gentit.home.cern.ch/gentit/litrani/");
ref->SetLanguage("C++");
ref->SetFramework("ROOT");
a = new TAuthor("Gentit","François-Xavier");
a->AddAddress("IRFU/SPP CEA Saclay");
a->AddMail("gentit@hep.saclay.cea.fr");
a->AddWebSite("http://gentit.home.cern.ch/gentit/");
a->AddCountry("France");
a->AddPhone("0169083038");
ref->AddAuthor(a);
b = new TAuthor("Wahl","David");
b->AddAddress("University Lima");
b->AddMail("david.n.wahl@gmail.com");
b->AddCountry("Peru");
ref->AddAuthor(b);
d = new TwoPadDisplay(ref,5);
TF1 *f1 = new TF1("f1","sin(x)/x",0,10);
f1->Draw();
gTwoPad->ChangePad();
TF1 *f;
Double_t content = 10000;
Double_t mean    = 50.0;
Double_t sigma   = 10.0;
TH1F *fH     = new TH1F("histo1","histo1",100,0.,100.);
f = new TF1("gg","gaus");
f->SetParameters(content,mean,sigma);
fH->FillRandom("gg",10000);
delete f;
fH->Draw("");
gTwoPad->ChangePad();
}
*/
//
TwoPadDisplay::TwoPadDisplay(TReference *ref,Short_t size,Int_t nbbut,Int_t xminC,
                             Int_t yminC):TCanvas(kTRUE) {
  //
  // Constructor with arguments:
  //
  //    ref     : references concerning running program. See class TReference
  //              if ref == 0, a dummy reference will be produced
  //    size    : size of canvas, [ 1 <= size <= 8 ] Default 4
  //    nbbut   : number of programmable buttons wanted [ 5 <= nbbut <= 48 ]
  //              [ Default 24 ] if nbbut == 0, no button created
  //    xminC   : x coordinate, in pixel, of top left corner of main canvas
  //              [ Default 2 ]
  //    yminC   : y coordinate, in pixel, of top left corner of main canvas
  //              [ Default 2 ]
  //
  //  This constructor chooses cleverly width [widthC] and height [heigthC]
  // of main canvas, taking into account the size of your screen in pixel and
  // the size specified.
  //
  const Int_t safwidth  = 10;
  const Int_t safheight = 100;
  UInt_t widthC;  //Width  of this canvas
  UInt_t heigthC; //Heigth of this canvas
  Int_t w,h,wmax,hmax;
  Double_t secor = (TMath::Sqrt(5.0) - 1.0)/2.0;
  nbbut = TMath::Abs(nbbut);
  if (nbbut) {
    if (nbbut <  5) nbbut = 5;
    if (nbbut > 48) nbbut = 48;
    fNbButtons = nbbut;
  }
  else fNbButtons = -24;
  Init();
  if (ref) fRef = ref;
  else     DefRef();
  SetWindowPosition(xminC,yminC);
  w = gClient->GetDisplayWidth();
  h = gClient->GetDisplayHeight();
  wmax = w - xminC - safwidth;
  hmax = h - yminC - safheight;
  fSize = size;
  if (fSize<1) fSize = 1;
  if (fSize>8) fSize = 8;
  switch (fSize) {
    case 1:
      //trying 600
      widthC = 600;
      break;
    case 2:
      //trying 800
      widthC = 800;
      break;
    case 3:
      //trying 1000
      widthC = 1000;
      break;
    case 4:
      //trying 1200
      widthC = 1200;
      break;
    case 5:
      //trying 1400
      widthC = 1400;
      break;
    case 6:
      //trying 1600
      widthC = 1600;
      break;
    case 7:
      //trying 1800
      widthC = 1800;
      break;
    case 8:
      //trying 2000
      widthC = 2000;
      break;
  }
  heigthC = (Int_t)(secor*widthC);
  widthC  = TMath::Min((Int_t)widthC,wmax);
  heigthC = TMath::Min((Int_t)heigthC,hmax);
  SetWindowSize(widthC,heigthC);
  gSystem->ProcessEvents();
  CreateContent();
  Modified();
  Update();
  gTwoPad = this;
}
TwoPadDisplay::TwoPadDisplay(const char *name,Int_t ww,Int_t wh,Int_t winid,
  TReference *ref,Int_t nbbut):TCanvas(name,ww,wh,winid) {
  // Constructor to create an embedded canvas
  nbbut = TMath::Abs(nbbut);
  if (nbbut) {
    if (nbbut <  5) nbbut = 5;
    if (nbbut > 48) nbbut = 48;
    fNbButtons = nbbut;
  }
  else fNbButtons = -24;
  Init();
  if (ref) fRef = ref;
  else     DefRef();
  fSize = 0;
  CreateContent();
  gTwoPad = this;
}
TwoPadDisplay::TwoPadDisplay(TReference *ref,Int_t nbbut,Int_t xminC, Int_t yminC,
  UInt_t widthC,UInt_t heigthC):TCanvas("TwoPad","TwoPad",xminC,yminC,widthC,heigthC) {
  //
  // Constructor with arguments:
  //
  //    ref     : references concerning running program. See class TReference
  //              if ref == 0, a dummy reference will be produced
  //    nbbut   : number of programmable buttons wanted [ 5 <= nbbut <= 48 ]
  //              if nbbut == 0, no button created
  //    xminC   : x coordinate, in pixel, of top left corner of main canvas
  //    yminC   : y coordinate, in pixel, of top left corner of main canvas
  //    widthC  : horizontal width, in pixel, along x, of main Canvas
  //    heigthC : vertical  heigth, in pixel, along y, of main Canvas
  //
  // You have to specify all arguments: there is no default value in this constructor   
  //
  nbbut = TMath::Abs(nbbut);
  if (nbbut) {
    if (nbbut <  5) nbbut = 5;
    if (nbbut > 48) nbbut = 48;
    fNbButtons = nbbut;
  }
  else fNbButtons = -24;
  Init();
  if (ref) fRef = ref;
  else     DefRef();
//  SetWindowPosition(xminC,yminC);
//  SetWindowSize(widthC,heigthC);
  fSize = 0;
  CreateContent();
  gTwoPad = this;
}
TwoPadDisplay::TwoPadDisplay(TReference *ref,TCanvas *canvas,Int_t nbbut,Bool_t deleteit):TCanvas(kTRUE) {
  //  Constructor to be used when one wants to copy and may be delete an already
  // existing canvas. This constructor can also be called with canvas = 0. In that
  // case, everything is booked with default values.
  //
  // Arguments:
  //
  //    ref      : references concerning running program. See class TReference
  //                if ref == 0, a dummy reference will be produced
  //    canvas   : provided canvas if != 0
  //    nbbut    : number of programmable buttons wanted [ 5 <= nbbut <= 48 ]
  //              [ Default 24 ]
  //    deleteit : true if canvas has to be deleted after the copy
  //              [ Default false ]
  Int_t  xminC;   //X position of canvas
  Int_t  yminC;   //Y position of canvas
  UInt_t widthC;  //Width  of canvas
  UInt_t heigthC; //Heigth of canvas
  nbbut = TMath::Abs(nbbut);
  if (nbbut) {
    if (nbbut <  5) nbbut = 5;
    if (nbbut > 48) nbbut = 48;
    fNbButtons = nbbut;
  }
  else fNbButtons = -24;
  Init();
  if (ref) fRef = ref;
  else     DefRef();
  if (canvas) {
    xminC   = canvas->GetWindowTopX();
    yminC   = canvas->GetWindowTopY();
    widthC  = canvas->GetWindowWidth();
    heigthC = canvas->GetWindowHeight();
    if (deleteit) delete canvas;
  }
  else {
    const Int_t safwidth  = 10;
    const Int_t safheight = 100;
    UInt_t w,h,wmax,hmax;
    Double_t secor = (TMath::Sqrt(5.0) - 1.0)/2.0;
    xminC = 2;
    yminC = 2;
    w = gClient->GetDisplayWidth();
    h = gClient->GetDisplayHeight();
    wmax = TMath::Abs(Int_t(w - xminC - safwidth));
    hmax = TMath::Abs(Int_t(h - yminC - safheight));
    fSize   = 3;
    widthC  = 1000;
    heigthC = (Int_t)(secor*widthC);
    widthC  = TMath::Min(widthC,wmax);
    heigthC = TMath::Min(heigthC,hmax);
  }
  SetWindowPosition(xminC,yminC);
  SetWindowSize(widthC,heigthC);
  gSystem->ProcessEvents();
  CreateContent();
  gTwoPad = this;
}
TwoPadDisplay::~TwoPadDisplay() {
  //  Destructor. fRef has not been booked by TwoPadDisplay, which is not owner of
  // fRef.
  if (!fCanDelete) cout << "Error: TwoPadDisplay should not be deleted" << endl;
  if (fIsOwnerOfRef) {
    delete fRef;
    fRef = 0;
  }
  DeleteContent();
  gTwoPad = 0;
}
void TwoPadDisplay::BookButtons() {
  // Book all buttons at the left of pad1
  if (fNbButtons>0) {
    Int_t N1,N2,k;
    Double_t ylow,yup,d;
    TObjString *name,*action;
    TButton *B;
    N1 = fButNames.GetEntries();
    N2 = fButActions.GetEntries();
    if (N1!=N2) {
      cout << "TwoPadDisplay::BookButtons ERROR: N button names != N button actions" << endl;
      fButNames.Delete();
      fButActions.Delete();
      N1 = 0;
    }
    if ((N1!=fNbButtons) && (N1)) {
      fButNames.Delete();
      fButActions.Delete();
      N1 = 0;
    }
    if (N1==fNbButtons) {
      cd();
      d = (fYUpPad - fYLowPad)/fNbButtons;
      fHeightBut = d - fFreeYBut;
      ylow = fYLowPad + 0.5*fFreeYBut;
      yup  = ylow + fHeightBut;
      for (k=0;k<fNbButtons;k++) {
        name   = (TObjString*)fButNames[k];
        action = (TObjString*)fButActions[k];
        B = new TButton(name->GetString().Data(),action->GetString().Data(),fXLowButton,ylow,fXUpButton,yup);
        B->SetTextFont(fFontButton);
        B->SetTextSize(fTextSBut);
        B->SetFillColor(fColorBut);
        B->SetBorderSize(fBszBut);
        B->Draw();
        fCollBut.Add(B);
        ylow += d;
        yup  += d;
      }
    }
    else {
      TString name1 = "button";
      TString action1 = ".p ";
      cd();
      d = (fYUpPad - fYLowPad)/fNbButtons;
      fHeightBut = d - fFreeYBut;
      ylow = fYLowPad + 0.5*fFreeYBut;
      yup  = ylow + fHeightBut;
      for (k=0;k<fNbButtons;k++) {
        name = new TObjString(name1.Data());
        if (k<10) name->GetString().Append('0');
        name->GetString() += k;
        action = new TObjString(action1.Data());
        action->GetString().Append('"');
        action->GetString().Append(name->GetString());
        action->GetString().Append('"');
        B = new TButton(name->GetString().Data(),action->GetString().Data(),fXLowButton,ylow,fXUpButton,yup);
        B->SetTextFont(fFontButton);
        B->SetTextSize(fTextSBut);
        B->SetFillColor(fColorBut);
        B->SetBorderSize(fBszBut);
        B->Draw();
        fCollBut.Add(B);
        fButNames.Add(name);
        fButActions.Add(action);
        ylow += d;
        yup  += d;
      }
      SetButtonFast(0,"Exit","gApplication->Terminate(0);");
      SetButtonFast(1,"Lin | Log","gTwoPad->ChangePadLogY();");
      SetButtonFast(2,"StateOfPad","gTwoPad->ToggleState();");
      SetButtonFast(3,"Grey | Color","gTwoPad->GreyOrColor();");
      //  SetButtonFast(4,"============",";");
#ifdef WIN32
      SetButtonFast(4,"?Help/Edit","gTwoPad->Editor();");
#else
      SetButtonFast(4,"============",";");
#endif
      k = fNbButtons-1;
      if (k>4)
        SetButtonFast(k,"ps","gTwoPad->SetExtForSave(\"ps\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"Portrait","gTwoPad->SetExtForSave(\"Portrait\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"Landscape","gTwoPad->SetExtForSave(\"Landscape\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"eps","gTwoPad->SetExtForSave(\"eps\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"Preview","gTwoPad->SetExtForSave(\"Preview\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"pdf","gTwoPad->SetExtForSave(\"pdf\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"svg","gTwoPad->SetExtForSave(\"svg\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"gif","gTwoPad->SetExtForSave(\"gif\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"xpm","gTwoPad->SetExtForSave(\"xpm\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"png","gTwoPad->SetExtForSave(\"png\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"jpg","gTwoPad->SetExtForSave(\"jpg\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"tiff","gTwoPad->SetExtForSave(\"tiff\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"cxx","gTwoPad->SetExtForSave(\"cxx\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"xml","gTwoPad->SetExtForSave(\"xml\");gTwoPad->SaveCanvas();");
      k--;
      if (k>4)
        SetButtonFast(k,"root","gTwoPad->SetExtForSave(\"root\");gTwoPad->SaveCanvas();");
    }
    UpdateCanvas();
    fPad1->cd();
    fUsePad2 = kFALSE;
  }
}
void TwoPadDisplay::BookCanvas() {
  // Booking of main Canvas
  // Finds the date
  TString s = fRef->GetName();
  s.Append(" : ");
  s.Append(fRef->GetTitle());
  s.Append("    ");
  s.Append(fRef->GetWebSite());
  SetName(fRef->GetName());
  SetTitle(s.Data());
  fUsePad2 = kFALSE;
  SetEditable();
  Range(0,0,1,1);
  SetFillColor(fColCan);
  SetFillStyle(fStyleCan);
  SetBorderSize(fBszCan);
  gStyle->SetOptStat(1111);
  gStyle->SetStatFont(22);
  gStyle->SetStatColor(19);
  if (GetWindowHeight()>=700) {
    gStyle->SetStatH(0.1);
    gStyle->SetTitleSize(0.01);
  }
  else {
    gStyle->SetStatH(0.15);
    gStyle->SetTitleSize(0.015);
  }
  gStyle->SetTitleFillColor(fStyleTFColor);
  gStyle->SetTitleTextColor(fStyleTTextColor);
  gStyle->SetTitleW(0.76);
  gStyle->SetHistFillColor(fStyleHistColor);
  gStyle->SetTitleFont(22);
  gStyle->SetTitleColor(fStyleTitleColor);
  gStyle->SetLabelSize(0.02,"XYZ");
  DrawLabels();
  BookPad1();
  if (fStateOfPads != OnePad) BookPad2();
  if (fNbButtons>0) BookButtons();
  fPad1->cd();
}
void TwoPadDisplay::BookLabels() {
  // Booking of the 3 comments
  if (fDefTexTL) DefCommentTL();
  else {
    if (fStringTL.Length()) CommentTL();
    else                    DefCommentTL();
  }
  if (fDefTexBL) DefCommentBL();
  else {
    if (fStringBL.Length()) CommentBL();
    else                    DefCommentBL();
  }
  if (fDefTexBR) DefCommentBR();
  else {
    if (fStringBR.Length()) CommentBR();
    else                    DefCommentBR();
  }
  if (fDefTexTR) DefCommentTR();
  else {
    if (fStringTR.Length()) CommentTR();
    else                    DefCommentTR();
  }
}
void TwoPadDisplay::BookPad1() {
  cd();
  if (fStateOfPads != OnePad)
    fPad1 = new TPad("LeftPad","Left pad",fXLowPad1,fYLowPad,fXUpPad1,fYUpPad);
  else
    fPad1 = new TPad("LeftPad","Left pad",fXLowPad1,fYLowPad,fXUpPad2,fYUpPad);
  fPad1->SetNumber(1);
  fPad1->SetFillColor(fColPad1);
  fPad1->SetFillStyle(fStylePad1);
  fPad1->SetBorderSize(fBszPad);
  fPad1->SetGridx();
  fPad1->SetGridy();
  fPad1->SetLogx(fPadLogX);
  fPad1->SetLogy(fPadLogY);
  fPad1->SetFrameFillColor(fColFrame);
  fPad1->SetFillStyle(fStylePad1);
  fPad1->Draw();
  fPad1->cd();
  fUsePad2 = kFALSE;
}
void TwoPadDisplay::BookPad2() {
  if (fStateOfPads != OnePad) {
    cd();
    fPad2 = new TPad("RightPad","Right pad",fXLowPad2,fYLowPad,fXUpPad2,fYUpPad);
    fPad2->SetNumber(2);
    fPad2->SetFillColor(fColPad2);
    fPad2->SetFillStyle(fStylePad2);
    fPad2->SetBorderSize(fBszPad);
    fPad2->SetGridx();
    fPad2->SetGridy();
    fPad2->SetLogx(fPadLogX);
    fPad2->SetLogy(fPadLogY);
    fPad2->SetFrameFillColor(fColFrame);
    fPad2->SetFillStyle(fStylePad2);
    fPad2->Draw();
    fPad1->cd();
    fUsePad2 = kFALSE;
  }
}
void TwoPadDisplay::ChangePad() {
  // Activate the other Pad if it is activable
  switch (fStateOfPads) {
    case Pad1AndPad2:
      if (fUsePad2) {
        fUsePad2 = kFALSE;
        fPad1->cd();
      }
      else {
        fUsePad2 = kTRUE;
        fPad2->cd();
      }
      break;
    case Pad1Only:
      fUsePad2 = kFALSE;
      fPad1->cd();
      break;
    case Pad2Only:
      fUsePad2 = kTRUE;
      fPad2->cd();
      break;
    case OnePad:
      fUsePad2 = kFALSE;
      fPad1->cd();
      break;
  }
}
void TwoPadDisplay::ChangePadLogX() {
  // Go from lin to log or vice-versa
  if (fPadLogX) SetPadLogX(0);
  else          SetPadLogX(1);
}
void TwoPadDisplay::ChangePadLogY() {
  // Go from lin to log or vice-versa
  if (fPadLogY) SetPadLogY(0);
  else          SetPadLogY(1);
}
void TwoPadDisplay::CommentTL() {
  // Add comment appearing on top left of the main canvas
  cd();
  if (fTexTL) fTexTL->SetTitle(fStringTL.Data());
  else        fTexTL = new TLatex(fXTexTL,fYTexTL,fStringTL.Data());
  UpdateCanvas();
  fDefTexTL = kFALSE;
}
void TwoPadDisplay::CommentTL(const char *t) {
  // Add comment appearing on top left of the main canvas
  cd();
  if (fTexTL) fTexTL->SetTitle(t);
  else        fTexTL = new TLatex(fXTexTL,fYTexTL,t);
  UpdateCanvas();
  fDefTexTL = kFALSE;
  fStringTL = t;
}
void TwoPadDisplay::CommentBL() {
  // Add comment appearing at the bottom left of the main canvas
  cd();
  if (fTexBL) fTexBL->SetTitle(fStringBL.Data());
  else        fTexBL = new TLatex(fXTexBL,fYTexBL,fStringBL.Data());
  fDefTexBL = kFALSE;
  if (fDefTexBR) DefCommentBR();
  UpdateCanvas();
}
void TwoPadDisplay::CommentBL(const char *t) {
  // Add comment appearing at the bottom left of the main canvas
  cd();
  if (fTexBL) fTexBL->SetTitle(t);
  else        fTexBL = new TLatex(fXTexBL,fYTexBL,t);
  fDefTexBL = kFALSE;
  if (fDefTexBR) DefCommentBR();
  UpdateCanvas();
  fStringBL = t;
}
void TwoPadDisplay::CommentBR() {
  // Add comment appearing at the bottom left of the main canvas
  cd();
  if (fTexBR) fTexBR->SetTitle(fStringBR.Data());
  else        fTexBR = new TLatex(fXTexBR,fYTexBR,fStringBR.Data());
  UpdateCanvas();
  fDefTexBR = kFALSE;
}
void TwoPadDisplay::CommentBR(const char *t) {
  // Add comment appearing at the bottom left of the main canvas
  cd();
  if (fTexBR) fTexBR->SetTitle(t);
  else        fTexBR = new TLatex(fXTexBR,fYTexBR,t);
  UpdateCanvas();
  fDefTexBR = kFALSE;
  fStringBR = t;
}
void TwoPadDisplay::CommentTR() {
  // Add comment appearing on top right of the main canvas
  cd();
  if (fTexTR) fTexTR->SetTitle(fStringTR.Data());
  else        fTexTR = new TLatex(fXTexTR,fYTexTR,fStringTR.Data());
  UpdateCanvas();
  fDefTexTR = kFALSE;
}
void TwoPadDisplay::CommentTR(const char *t) {
  // Add comment appearing on top right of the main canvas
  cd();
  if (fTexTR) fTexTR->SetTitle(t);
  else        fTexTR = new TLatex(fXTexTR,fYTexTR,t);
  UpdateCanvas();
  fDefTexTR = kFALSE;
  fStringTR = t;
}
void TwoPadDisplay::CreateContent() {
  // Recreate content of TwoPadDisplay
  SetDate();
  BookLabels();
  BookCanvas();
}
void TwoPadDisplay::DefCommentTL() {
  // SetDefault values for the Top Left comment: info on program
  TString c1;
  c1 = fRef->GetName();
  c1.Append(" : ");
  c1.Append(fRef->GetTitle());
  if (!fTexTL) fTexTL = new TLatex(fXTexTL,fYTexTL,c1.Data());
  else         fTexTL->SetTitle(c1.Data());
  UpdateCanvas();
  fDefTexTL = kTRUE;
}
void TwoPadDisplay::DefCommentBL() {
  // SetDefault values for the Bottom Left comment: info on 1st author
  Int_t N;
  TString c2;
  TAuthor *au;
  N = fRef->GetNbOfAuthor();
  if (N) {
    au = fRef->GetAuthor(0);
    c2 = au->GetInitials();
    c2.Append(au->GetName());
    c2.Append("  ");
    c2.Append(au->GetAddress());
    c2.Append("  ");
    c2.Append(au->GetMail());
  }
  else c2 = "No author specified";
  if (!fTexBL) fTexBL = new TLatex(fXTexBL,fYTexBL,c2.Data());
  else         fTexBL->SetTitle(c2.Data());
  fDefTexBL = kTRUE;
  if (fDefTexBR) DefCommentBR();
  UpdateCanvas();
}
void TwoPadDisplay::DefCommentBR() {
  //  SetDefault values for the Bottom Right comment: info on 2nd author
  // if first author quoted. Else info on first author
  Int_t N;
  TString c3;
  TAuthor *au;
  N = fRef->GetNbOfAuthor();
  if (fDefTexBL) {
    if (N>1) {
      au = fRef->GetAuthor(1);
      c3 = au->GetInitials();
      c3.Append(au->GetName());
      c3.Append("  ");
      c3.Append(au->GetAddress());
      c3.Append("  ");
      c3.Append(au->GetMail());
    }
    else c3 = "No 2nd author";
    if (!fTexBR) fTexBR = new TLatex(fXTexBR,fYTexBR,c3.Data());
    else         fTexBR->SetTitle(c3.Data());
  }
  else {
    if (N) {
      au = fRef->GetAuthor(0);
      c3 = au->GetInitials();
      c3.Append(au->GetName());
      c3.Append("  ");
      c3.Append(au->GetAddress());
      c3.Append("  ");
      c3.Append(au->GetMail());
    }
    else c3 = "No author specified";
    if (!fTexBR) fTexBR = new TLatex(fXTexBR,fYTexBR,c3.Data());
    else         fTexBR->SetTitle(c3.Data());
  }
  fDefTexBR = kTRUE;
  UpdateCanvas();
}
void TwoPadDisplay::DefCommentTR() {
  //  SetDefault values for the Top Left comment: info on program version,
  // date and time
  TString c4;
  c4 = fRef->GetName();
  c4.Append(' ');
  c4.Append(fRef->GetVersion());
  c4.Append("  ");
  c4.Append(fDate);
  c4.Append("  ");
  c4.Append("Language: ");
  c4.Append(fRef->GetLanguage());
  c4.Append("  ");
  c4.Append("Framework: ");
  c4.Append(fRef->GetFramework());
  if (!fTexTR) fTexTR = new TLatex(fXTexTR,fYTexTR,c4.Data());
  else         fTexTR->SetTitle(c4.Data());
  fDefTexTR = kTRUE;
  UpdateCanvas();
}
void TwoPadDisplay::DefRef() {
  // Defines a dummy fRef in case the user has not provided it
  TAuthor *a;
  fIsOwnerOfRef = kTRUE;
  fRef = new TReference("TwoPad","Default canvas","1_0",
    "http://gentit.home.cern.ch/gentit/litrani/AllModules/TwoPadCode/TwoPadDisplay.html");
  fRef->SetFramework("ROOT");
  fRef->SetLanguage("C++");
  a = new TAuthor("Gentit","François-Xavier");
  a->AddAddress("IRFU/SPP");
  a->AddMail("gentit@hep.saclay.cea.fr");
  a->AddWebSite("http://gentit.home.cern.ch/gentit/");
  a->AddCountry("France");
  fRef->AddAuthor(a);
}
void TwoPadDisplay::DeleteContent() {
  // Delete Canvas
  DeleteLabels();
  fCollBut.Delete();
  fButNames.Delete();
  fButActions.Delete();
  DeletePads();
}
void TwoPadDisplay::DeleteLabels() {
  // Delete the 4 comments
  if (fTexTL)  {
    delete fTexTL;
    fTexTL = 0;
  }
  if (fTexBL)  {
    delete fTexBL;
    fTexBL = 0;
  }
  if (fTexBR)  {
    delete fTexBR;
    fTexBR = 0;
  }
  if (fTexTR)  {
    delete fTexTR;
    fTexTR = 0;
  }
}
void TwoPadDisplay::DeletePads() {
  // Delete the 2 pads
  if (fPad1) {
    delete fPad1;
    fPad1 = 0;
  }
  if (fPad2) {
    delete fPad2;
    fPad2 = 0;
  }
}
void TwoPadDisplay::DrawLabels() {
  // Draws the 4 labels on the Canvas
  fTexTL->SetTextFont(fFontTex);
  fTexTL->SetTextSize(fSizeTex);
  fTexTL->SetLineWidth(fWidthTex);
  fTexBL->SetTextFont(fFontTex);
  fTexBL->SetTextSize(fSizeTex);
  fTexBL->SetLineWidth(fWidthTex);
  fTexBR->SetTextFont(fFontTex);
  fTexBR->SetTextSize(fSizeTex);
  fTexBR->SetLineWidth(fWidthTex);
  fTexTR->SetTextFont(fFontTex);
  fTexTR->SetTextSize(fSizeTex);
  fTexTR->SetLineWidth(fWidthTex);
  cd();
  fTexTL->Draw();
  fTexBL->Draw();
  fTexBR->Draw();
  fTexTR->Draw();
}
void TwoPadDisplay::Editor(const char *name) {
  if (gRootIDE) {
    delete gRootIDE;
    gRootIDE = 0;
  }
  else {
    Int_t L;
    TString s = name;
    L = s.Length();
    if (L) gRootIDE = new TGRootIDE(name);
    else   gRootIDE = new TGRootIDE();
  }
}
TButton *TwoPadDisplay::GetButton(Int_t k) {
  // Gives pointer to button k
  TButton *B = 0;
  if (fNbButtons>0) {
    k  = TMath::Abs(k);
    k %= fNbButtons;
    B = (TButton*)fCollBut[k];
  }
  return B;
}
void TwoPadDisplay::GreyOrColor() {
  // Switch from color to grey or vice-versa
  if (fGrey) SetAllColor();
  else       SetAllGrey();
}
void TwoPadDisplay::Init() {
  // Initialization
  InitP();
  fIsOwnerOfRef = kFALSE;
  fStateOfPads  = Pad1AndPad2;
  fUsePad2      = kFALSE;
  fGrey         = kFALSE;
  fStringTL     = "";
  fStringBL     = "";
  fStringBR     = "";
  fStringTR     = "";
  fXTexTL       = 0.02;
  fYTexTL       = 0.975;
  fXTexBL       = 0.02;
  fYTexBL       = 0.01;
  fXTexBR       = 0.55;
  fYTexBR       = 0.01;
  fXTexTR       = 0.55;
  fYTexTR       = 0.975;
  fDefTexTL     = kTRUE;
  fDefTexBL     = kTRUE;
  fDefTexBR     = kTRUE;
  fDefTexTR     = kTRUE;
  fFontTex      = 132;
  fSizeTex      = 0.02;
  fWidthTex     = 2;
  // Various settings
  fStyleHistColor  = 15;
  fColCan       = kGray+1;
  fStyleCan     = 1001;
  fPlaceLeft    = 0.12;
  fEmptyX       = 0.005;
  fEmptyY       = 0.03;
  RecalcX();
  RecalcY();
  fFreeYBut     = 0.18/TMath::Abs(fNbButtons);
  fFontButton   = 72;
  fColorBut     = 30;
  fBszBut       = 4;
  if (TMath::Abs(fNbButtons)>=8) fTextSBut = 0.025*TMath::Abs(fNbButtons);
  else                           fTextSBut = 0.18;
  fStylePad1    = 1001;
  fPadLogX      = 0;
  fPadLogY      = 0;
#ifdef OLDROOT
  fColFrame     = 18;
  fColPad1      = 30;
  fColPad2      = 30;
#else
  fColPad1      = kYellow-8;
  fColPad2      = kYellow-8;
  fColFrame     = kYellow-10;
#endif
  fStylePad2    = 1001;
  fBszCan       = 4;
  fBszPad       = 6;
#ifdef OLDROOT
  fStyleHistColor  = 45;
#else
  fStyleHistColor  = 15;
#endif
  fStyleTitleColor = kBlack;
  fStyleTFColor    = kYellow-10;
  fStyleTTextColor = 1;
  fNameForSave = "TwoPad";
  fExtForSave  = "jpg";
}
void TwoPadDisplay::InitP() {
  // Pointers to 0
  fIsOwnerOfRef = kTRUE;
  fRef          = 0;
  fPad1         = 0;
  fPad2         = 0;
  fTexTL        = 0;
  fTexBL        = 0;
  fTexBR        = 0;
  fTexTR        = 0;
}
void TwoPadDisplay::RecalcX() {
  //  Something has changed in x dimensions. All dimensions have to be
  // recalculated and the pad re-booked
  Double_t wpad, wbutton, placeleft;
  if (fNbButtons>0) placeleft = fPlaceLeft;
  else              placeleft = 0.0;
  wbutton     = placeleft - 2*fEmptyX;
  wpad        = 0.5*(1.0 - placeleft - 4*fEmptyX);
  fXLowButton = 2*fEmptyX;
  fXUpButton  = 2*fEmptyX + wbutton;
  fXLowPad1   = placeleft + fEmptyX;
  fXUpPad1    = fXLowPad1 + wpad;
  fXLowPad2   = fXUpPad1 + 2*fEmptyX;
  fXUpPad2    = fXLowPad2 + wpad;
  if (fPad1) {
    DeletePads();
    fCollBut.Delete();
    BookPad1();
    BookPad2();
    if (fNbButtons>0) BookButtons();
  }
}
void TwoPadDisplay::RecalcY() {
  //  Something has changed in y dimensions. All dimensions have to be
  // recalculated and the pad re-booked
  fYLowPad    = fEmptyY;
  fYUpPad     = 1.0 - fEmptyY;
  if (fPad1) {
    DeletePads();
    BookPad1();
    BookPad2();
  }
}
void TwoPadDisplay::SaveCanvas() {
  // Save canvas of TwoPaddisplay as a fExtForSave file
  TString s = fNameForSave.Data();
  if (!s.Contains('.')) {
    if (!fExtForSave.CompareTo("ps")) {
      s.Append(".ps");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("Portrait")) {
      s.Append(".ps");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("Landscape")) {
      s.Append(".ps");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("eps")) {
      s.Append(".eps");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("Preview")) {
      s.Append(".Preview");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("pdf")) {
      s.Append(".pdf");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("svg")) {
      s.Append(".svg");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("gif")) {
      s.Append(".gif");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("xpm")) {
      s.Append(".xpm");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("png")) {
      s.Append(".png");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("jpg")) {
      s.Append(".jpg");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("tiff")) {
      s.Append(".tiff");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("cxx")) {
      s.Append(".cxx");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("xml")) {
      s.Append(".xml");
      Print(s.Data(),fExtForSave.Data()); return;
    }
    if (!fExtForSave.CompareTo("root")) {
      s.Append(".root");
      Print(s.Data(),fExtForSave.Data()); return;
    }
  }
  Print(s.Data(),fExtForSave.Data());
}
void TwoPadDisplay::SelectCanvas() {
  // Canvas is selected
  cd();
}
void TwoPadDisplay::SelectPad1() {
  // fPad1 is selected
  if (fPad1) {
    fPad1->cd();
    fUsePad2 = kFALSE;
  }
}
void TwoPadDisplay::SelectPad2() {
  // fPad2 is selected
  if (fPad2) {
    fPad2->cd();
    fUsePad2 = kTRUE;
  }
}
void TwoPadDisplay::SetAllColor() {
  // Only use grey: better for black and white print
  Int_t k,N;
  TButton *B;
  fGrey            = kFALSE;
  fColCan          = kGray+1;
#ifdef OLDROOT
  fColFrame        = 18;
  fStyleHistColor  = 45;
  fColPad1         = 30;
  fColPad2         = 30;
#else
  fColFrame        = kYellow-10;
  fStyleHistColor  = 15;
  fColPad1         = kYellow-8;
  fColPad2         = kYellow-8;
#endif
  fStyleTitleColor = 1;
  fStyleTFColor    = kYellow-10;
  fStyleTTextColor = 1;
  fColorBut        = 30;
  N = fCollBut.GetEntries();
  for (k=0;k<N;k++) {
    B = (TButton*)fCollBut[k];
    B->SetFillColor(fColorBut);
    B->Modified();
    B->Update();
  }
  gStyle->SetHistFillColor(fStyleHistColor);
  gStyle->SetTitleColor(fStyleTitleColor);
  gStyle->SetTitleFillColor(fStyleTFColor);
  gStyle->SetTitleTextColor(fStyleTTextColor);
  SetFillColor(fColCan);
  if (fPad1) {
    fPad1->SetFillColor(fColPad1);
    fPad1->SetFrameFillColor(fColFrame);
  }
  if (fPad2) {
    fPad2->SetFillColor(fColPad2);
    fPad2->SetFrameFillColor(fColFrame);
  }
  UpdateCanvas();
  UpdatePad1();
  UpdatePad2();
}
void TwoPadDisplay::SetAllGrey() {
  // Only use grey: better for black and white print
  Int_t k,N;
  TButton *B;
  fGrey            = kTRUE;
  fColCan          = 16;
  fColPad1         = 18;
  fColPad2         = 18;
  fColFrame        = 10;
  fStyleHistColor  = 15;
  fStyleTitleColor = 1;
  fStyleTFColor    = 19;
  fStyleTTextColor = 1;
  fColorBut        = 17;
  N = fCollBut.GetEntries();
  for (k=0;k<N;k++) {
    B = (TButton*)fCollBut[k];
    B->SetFillColor(fColorBut);
    B->Modified();
    B->Update();
  }
  gStyle->SetHistFillColor(fStyleHistColor);
  gStyle->SetTitleColor(fStyleTitleColor);
  gStyle->SetTitleFillColor(fStyleTFColor);
  gStyle->SetTitleTextColor(fStyleTTextColor);
  SetFillColor(fColCan);
  if (fPad1) {
    fPad1->SetFillColor(fColPad1);
    fPad1->SetFrameFillColor(fColFrame);
  }
  if (fPad2) {
    fPad2->SetFillColor(fColPad2);
    fPad2->SetFrameFillColor(fColFrame);
  }
  UpdateCanvas();
  UpdatePad1();
  UpdatePad2();
}
void TwoPadDisplay::SetBorderCan(Short_t b) {
  // Set border size of main canvas
  fBszCan = b;
  SetBorderSize(fBszCan);
  UpdateCanvas();
}
void TwoPadDisplay::SetBorderPad(Short_t b) {
  // Set border size of both pads
  fBszPad = b;
  if (fPad1) {
    fPad1->SetBorderSize(fBszPad);
    fPad1->Modified();
    fPad1->Update();
    if (fPad2) {
      fPad2->SetBorderSize(fBszPad);
      fPad2->Modified();
      fPad2->Update();
      if (fUsePad2) fPad2->cd();
      else          fPad1->cd();
    }
    else fPad1->cd();
  }//end if (fPad1)
}
void TwoPadDisplay::SetButton(Int_t k,const char *title, const char *action) {
  // Put a more useful action to button k.
  //  - title is the new name of button k
  //  - action is the new CINT command for button k
  Int_t N1,N2;
  N1 = fButNames.GetEntries();
  N2 = fButActions.GetEntries();
  if ((N1!=N2) || (N1!=fNbButtons))
    cout << "TwoPadDisplay::SetButton ERROR: mismatch in collections" << endl;
  TButton *B;
  TObjString *objname, *objaction;
  k  = TMath::Abs(k);
  if (k<fNbButtons) {
    cd();
    B = (TButton*)fCollBut[k];
    B->SetTitle(title);
    B->SetMethod(action);
    objname    = (TObjString*)fButNames[k];
    objaction  = (TObjString*)fButActions[k];
    objname->String()   = title;
    objaction->String() = action;
    B->Modified();
    B->Update();
    UpdateCanvas();
  }
}
void TwoPadDisplay::SetButtonFast(Int_t k,const char *title, const char *action) {
  //  Put a more useful action to button k.
  //  - title is the new name of button k
  //  - action is the new CINT command for button k
  //  Identical to SetButton, except that no update is done. Use it when you have
  // many buttons to define, but do not forget to do gTwoPad->UpdateCanvas() at
  // the end!
  Int_t N1,N2;
  N1 = fButNames.GetEntries();
  N2 = fButActions.GetEntries();
  if ((N1!=N2) || (N1!=fNbButtons))
    cout << "TwoPadDisplay::SetButtonFast ERROR: mismatch in collections" << endl;
  TButton *B;
  TObjString *objname, *objaction;
  k  = TMath::Abs(k);
  if (k<fNbButtons) {
    cd();
    B = (TButton*)fCollBut[k];
    B->SetTitle(title);
    B->SetMethod(action);
    objname    = (TObjString*)fButNames[k];
    objaction  = (TObjString*)fButActions[k];
    objname->String()   = title;
    objaction->String() = action;
    B->Modified();
    B->Update();
  }
}
void TwoPadDisplay::SetColCan(Color_t col) {
  // Set color of main canvas
  fColCan = col;
  SetFillColor(fColCan);
  UpdateCanvas();
}
void TwoPadDisplay::SetColFrame(Color_t col) {
  // Set color of frame
  fColFrame = col;
  if (fPad1) {
    fPad1->SetFrameFillColor(fColFrame);
    fPad1->Modified();
    fPad1->Update();
    if (fPad2) {
      fPad2->SetFrameFillColor(fColFrame);
      fPad2->Modified();
      fPad2->Update();
      if (fUsePad2) fPad2->cd();
      else          fPad1->cd();
    }
    else fPad1->cd();
  }
}
void TwoPadDisplay::SetColPad1(Color_t col) {
  // Set color of main canvas
  fColPad1 = col;
  if (fPad1) {
    fPad1->SetFillColor(fColPad1);
    fPad1->Modified();
    fPad1->Update();
    fPad1->cd();
    fUsePad2 = kFALSE;
  }
}
void TwoPadDisplay::SetColPad2(Color_t col) {
  // Set color of main canvas
  fColPad2 = col;
  if (fPad2) {
    fPad2->SetFillColor(fColPad2);
    fPad2->Modified();
    fPad2->Update();
    fPad2->cd();
    fUsePad2 = kTRUE;
  }
}
void TwoPadDisplay::SetDate() {
  // Finds the date and time of to-day
  const Int_t cent = 100;
  Int_t date,time;
  Int_t day,month,year;
  Int_t hour,minute,second;
  TDatime td;
  date   = td.GetDate();
  time   = td.GetTime();
  //
  day    = date % cent;
  date  /= cent;
  month  = date % cent;
  date  /= cent;
  year   = date;
  second = time % cent;
  time  /= cent;
  minute = time % cent;
  time  /= cent;
  hour   = time;
  //
  fDate  = "  ";
  fDate += day;
  fDate.Append(" / ");
  fDate += month;
  fDate.Append(" / ");
  fDate += year;
  fDate.Append("  ");
  //
  fTime  = "";
  fTime += hour;
  fTime.Append('_');
  fTime += minute;
}
void TwoPadDisplay::SetEmptyX(Double_t d) {
  // Empty space between pads in x is changed
  fEmptyX = d;
  RecalcX();
}
void TwoPadDisplay::SetEmptyY(Double_t d) {
  // Empty space up and down pads is changed
  fEmptyY = d;
  RecalcY();
}
void TwoPadDisplay::SetFontTex(Font_t f) {
  // Change the font used for labels
  fFontTex = f;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetExtForSave(const char *ext) {
  // Decides type of file to be used when saving canvas
  TString s = ext;
  if (!s.CompareTo("ps")) {
    fExtForSave = "ps";
    return;
  }
  if (!s.CompareTo("Portrait")) {
    fExtForSave = "Portrait";
    return;
  }
  if (!s.CompareTo("Landscape")) {
    fExtForSave = "Landscape";
    return;
  }
  if (!s.CompareTo("eps")) {
    fExtForSave = "eps";
    return;
  }
  if (!s.CompareTo("Preview")) {
    fExtForSave = "Preview";
    return;
  }
  if (!s.CompareTo("pdf")) {
    fExtForSave = "pdf";
    return;
  }
  if (!s.CompareTo("svg")) {
    fExtForSave = "svg";
    return;
  }
  if (!s.CompareTo("gif")) {
    fExtForSave = "gif";
    return;
  }
  if (!s.CompareTo("xpm")) {
    fExtForSave = "xpm";
    return;
  }
  if (!s.CompareTo("png")) {
    fExtForSave = "png";
    return;
  }
  if (!s.CompareTo("jpg")) {
    fExtForSave = "jpg";
    return;
  }
  if (!s.CompareTo("tiff")) {
    fExtForSave = "tiff";
    return;
  }
  if (!s.CompareTo("cxx")) {
    fExtForSave = "cxx";
    return;
  }
  if (!s.CompareTo("xml")) {
    fExtForSave = "xml";
    return;
  }
  if (!s.CompareTo("root")) {
    fExtForSave = "root";
    return;
  }
}
void TwoPadDisplay::SetNbButtons(Int_t nbbut) {
  // Change the number of available buttons
  nbbut = TMath::Abs(nbbut);
  if (nbbut) {
    if (nbbut <  5) nbbut = 5;
    if (nbbut > 48) nbbut = 48;
    fNbButtons = nbbut;
  }
  else fNbButtons = -TMath::Abs(fNbButtons);
  fFreeYBut   = 0.18/TMath::Abs(fNbButtons);
  if (TMath::Abs(fNbButtons)>=8) fTextSBut = 0.025*TMath::Abs(fNbButtons);
  else                           fTextSBut = 0.18;
  fCollBut.Delete();
  DeletePads();
  RecalcX();
  BookPad1();
  if (fStateOfPads != OnePad) BookPad2();
  if (fNbButtons>0) BookButtons();
  fPad1->cd();
  UpdateCanvas();
}
void TwoPadDisplay::SetPadLogX(Int_t L) {
  // Set logarithmic x scale (L=1) or linear x scale (L=0)
  fPadLogX = L;
  if (fPad1) fPad1->SetLogx(fPadLogX);
  if (fPad2) fPad2->SetLogx(fPadLogX);
  UpdatePad1();
  UpdatePad2();
}
void TwoPadDisplay::SetPadLogY(Int_t L) {
  // Set logarithmic x scale (L=1) or linear x scale (L=0)
  fPadLogY = L;
  if (fPad1) fPad1->SetLogy(fPadLogY);
  if (fPad2) fPad2->SetLogy(fPadLogY);
  UpdatePad1();
  UpdatePad2();
}
void TwoPadDisplay::SetPlaceLeft(Double_t d) {
  // Change the size of the place in x for the buttons, at left of pad 1
  fPlaceLeft = d;
  RecalcX();
}
void TwoPadDisplay::SetRef(TReference *ref,Bool_t deleteold) {
  //  Replaces reference fRef by a new one. If deleteold is true [default false]
  // then the old reference is deleted
  if (deleteold) {
    if (fRef) delete fRef;
  }
  fRef = ref;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetSizeTex(Float_t s) {
  // Change size of text of comments
  fSizeTex = s;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetStateOfPads(StateOfPads state) {
  // Select one of the 4 different ways to look at histograms on the 2 pads
  TButton *but;
  StateOfPads oldstate = fStateOfPads;
  if (fNbButtons>2) but = (TButton*)fCollBut[2];
  else              but = 0;
  fStateOfPads = state;
  switch (state) {
    case Pad1AndPad2:
      if (oldstate != Pad1AndPad2) {
        if (oldstate == OnePad) {
          DeletePads();
          BookPad1();
          BookPad2();
        }
        UpdatePad1();
        UpdatePad2();
        fPad1->cd();
        fUsePad2 = kFALSE;
        if (but) but->SetTitle("Pad1AndPad2");
      }
      break;
    case Pad1Only:
      if (oldstate != Pad1Only) {
        if (oldstate == OnePad) {
          DeletePads();
          BookPad1();
          BookPad2();
        }
        UpdatePad1();
        fPad1->cd();
        fUsePad2 = kFALSE;
        if (but) but->SetTitle("Pad1Only");
      }
      break;
    case Pad2Only:
      if (oldstate != Pad2Only) {
        if (oldstate == OnePad) {
          DeletePads();
          BookPad1();
          BookPad2();
        }
        UpdatePad2();
        fPad2->cd();
        fUsePad2 = kTRUE;
        if (but) but->SetTitle("Pad2Only");
      }
      break;
    case OnePad:
      if (oldstate != OnePad) {
        DeletePads();
        BookPad1();
        fUsePad2 = kFALSE;
        if (but) but->SetTitle("OnePad");
      }
      break;
  }
  if (but) {
    but->Modified();
    but->Update();
  }
  UpdateCanvas();
}
void TwoPadDisplay::SetStyleCan(Style_t style) {
  // Set fill style of main canvas
  fStyleCan = style;
  SetFillStyle(fStyleCan);
  Modified();
  Update();
  fPad1->cd();
  fUsePad2 = kFALSE;
}
void TwoPadDisplay::SetStylePad1(Style_t style) {
  // Set fill style of pad 1
  fStylePad1 = style;
  if (fPad1) {
    fPad1->SetFillStyle(fStylePad1);
    fPad1->Modified();
    fPad1->Update();
    fPad1->cd();
    fUsePad2 = kFALSE;
  }
}
void TwoPadDisplay::SetStylePad2(Style_t style) {
  // Set fill style of pad2
  fStylePad2 = style;
  if (fPad2) {
    fPad2->SetFillStyle(fStylePad2);
    fPad2->Modified();
    fPad2->Update();
    fPad2->cd();
    fUsePad2 = kTRUE;
  }
}
void TwoPadDisplay::SetStyleStat(Int_t style) {
  // Set level of print in stat box
  fStyleStat = style;
  gStyle->SetOptStat(fStyleStat);
}
void TwoPadDisplay::SetWidthTex(Width_t w) {
  // Change width of text of comments
  fWidthTex = w;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetXTexTL(Double_t x) {
  // Change x position of label Top Left
  fXTexTL = x;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetYTexTL(Double_t y) {
  // Change y position of label Top Left
  fYTexTL = y;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetXTexBL(Double_t x) {
  // Change x position of label Bottom Left
  fXTexBL = x;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetYTexBL(Double_t y) {
  // Change y position of label Bottom Left
  fYTexBL = y;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetXTexBR(Double_t x) {
  // Change x position of label Bottom Right
  fXTexBR = x;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetYTexBR(Double_t y) {
  // Change y position of label Bottom Right
  fYTexBR = y;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetXTexTR(Double_t x) {
  // Change x position of label Top Right
  fXTexTR = x;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::SetYTexTR(Double_t y) {
  // Change y position of label Top Right
  fYTexTR = y;
  DeleteLabels();
  BookLabels();
  DrawLabels();
  UpdateCanvas();
}
void TwoPadDisplay::ToggleButtons() {
  // Switch between state with or without buttons
  if (fNbButtons>0) SetNbButtons(0);
  else SetNbButtons(-fNbButtons);
}
void TwoPadDisplay::ToggleState() {
  // Go to next state. Used by TButton.
  switch (fStateOfPads) {
    case Pad1AndPad2:
      SetStateOfPads(Pad1Only);
      break;
    case Pad1Only:
      SetStateOfPads(Pad2Only);
      break;
    case Pad2Only:
      SetStateOfPads(OnePad);
      break;
    case OnePad:
      SetStateOfPads(Pad1AndPad2);
      break;
  }
}
void TwoPadDisplay::UpdateCanvas() {
  // Changes have been made to main canvas. Must be updated
  Modified();
  Update();
  if (fPad1) {
    switch (fStateOfPads) {
      case Pad1AndPad2:
        if (fUsePad2) fPad2->cd();
        else          fPad1->cd();
        break;
      case Pad1Only:
        fPad1->cd();
        break;
      case Pad2Only:
        fPad2->cd();
        break;
      case OnePad:
        fPad1->cd();
        break;
    }
    UpdateIt();
  }
}
void TwoPadDisplay::UpdateIt() {
  // Update the selected pad
  switch (fStateOfPads) {
    case Pad1AndPad2:
      if (fUsePad2) {
        fPad2->Modified();
        fPad2->Update();
      }
      else {
        fPad1->Modified();
        fPad1->Update();
      }
      break;
    case Pad1Only:
      fPad1->Modified();
      fPad1->Update();
      break;
    case Pad2Only:
      fPad2->Modified();
      fPad2->Update();
      break;
    case OnePad:
      fPad1->Modified();
      fPad1->Update();
      break;
  }
}
void TwoPadDisplay::UpdatePad1() {
  // fPad1 has been modified
  if (fPad1) {
    fPad1->Modified();
    fPad1->Update();
  }
}
void TwoPadDisplay::UpdatePad2() {
  // fPad2 has been modified
  if (fPad2) {
    fPad2->Modified();
    fPad2->Update();
  }
}
