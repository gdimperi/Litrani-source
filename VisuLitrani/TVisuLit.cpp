// @(#)root/html:$Id: TVisuLit.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TApplication.h"
#include "TGClient.h"
#include "TRint.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TStyle.h"
#include "TwoPadDisplay.h"
#include "TSplineFit.h"
#include "TLitPhys.h"
#include "TLitSpectrum.h"
#include "TPublication.h"
#include "TLitGlob.h"
#include "TVisuLit.h"

R__EXTERN TResults     *gGs;
R__EXTERN TResults     *gCs;
R__EXTERN TPublication *gGp;

const char *filetypes[] = { "All files",     "*",
"ROOT files",    "*.root",
"ROOT macros",   "*.C",
"Text files",    "*.[tT][xX][tT]",
0,               0 };

TVisuLit* gVisuLit = 0;

ClassImp(TVisuLit)
//______________________________________________________________________________
//
/* BEGIN_HTML
<div align="center">
<h2>Visualize the ROOT files produced by Litrani.</h2>
</div>
<p>After having launched <b><code>VisuLitrani</code></b>, please open a <b><code>Litrani</code></b> 
<b><code>.root</code></b> file using the file menu. In the <b><code><a href="http://root.cern.ch/root/html/TGListBox.html">TGListBox</a></code></b> 
appearing on the left of the main window, you will then see the list of all runs 
contained in the <b><code>Litrani</code></b> file. The first item of the <b><code><a href="http://root.cern.ch/root/html/TGListBox.html">TGListBox</a></code></b> 
selects the histograms for the sum of <b><i>all</i></b> runs. It is selected by default. 
If you want to see a particular run, select it. There are 5 kinds of histograms that 
you can display, according to enum <b><code>ToBeDrawn</code></b>. You select the 
kind you want to see using the 5 radio buttons:</p>
<ol>
<li>[<b><code>ResultsVsPar</code></b>] Histograms showing quantities as a function 
of a run dependent parameter. Examples:
<ul>
<li>Efficiency as a function of tilt angle of the fibre
<li>Efficiency as a function of the wavelength of the light of the fibre
<li>Lost in material as a function of the wavelength of the light
<li>and so on ...
</ul>
<li>[<b><code>RunsResults</code></b>] Run specific histograms (or the sum of these 
run specific histograms for all runs) Examples :
<ul>
<li><b><code>fHPoints</code></b>   : Nb. of interactions on the trajectory of 
each photons
<li><b><code>fHTimeSeen</code></b> : Time of arrival for each photon seen
<li><b><code>fHDistSeen</code></b> : Distance travelled by photons seen in detector
<li>and so on ...
</ul>
<li>[<b><code>DetResults</code></b>] Detector specific histograms for each run, 
or for the sum of all runs. (A setup can contain many detectors. Examples : 
<ul>
<li>Nb. of photons seen by a specific detector
<li>Nb. of photons lost because of quantum efficiency for a specific detector
<li>and so on ...
</ul>
<li>[<b><code>FitsUsed</code></b>] Display of all the fits of type <b><code><a href="../SplineFitCode/TSplineFit.html">TSplineFit</a></code></b> 
used for the generation of the photons of this <b><code>Litrani</code></b> <b><code>.root</code></b> 
file.
<li>[<b><code>SpectraUsed</code></b>] Display of all the emission spectra of type 
<b><code><a href="../LitraniCode/TLitSpectrum.html">TLitSpectrum</a></code></b> used 
for the generation of the photons of this <b><code>Litrani</code></b> <b><code>.root</code></b> 
file.
</ol>
<p>The following buttons are available:</p>
<ol>
<li>&quot;<b><code>DrawNext</code></b>&quot; Each time you click the button &quot;<b><code>DrawNext</code></b>&quot; 
the next histogram in the category you have chosen is plotted.
<li>&quot;<b><code>PlotStat</code></b>&quot; Clicking the button &quot;<b><code>PlotStat</code></b>&quot;, 
you get a plot of the statistics for the chosen run, in particular the most important 
result: the efficiency.
<li>&quot;<b><code>SelState</code></b>&quot; This button allows to choose between 
4 states:
<ol>
<li type="a">plotting alternatively in pad1 and pad2 [default]
<li type="a">plotting only onto pad1
<li type="a">plotting only onto pad2
<li type="a">using only one big pad
<ul>
<li>Options (b) and (c) are useful if you want to bring together 2 histograms 
which are not following each other in the list.
<li>Option (d) is useful if your screen lacks of pixels
</ul>
</ol>
<li>&quot;<b><code>Lin/log</code></b>&quot; allows to select between linear or 
logarithmic scale.
<li>&quot;<b><code>SelColor</code></b>&quot; Allows to select either a color display 
or an all grey display.
</ol>
<p>Notice that the canvas used by <b><code>TVisuLit</code></b> [ the canvas contained 
by the <b><code><a href="http://root.cern.ch/root/html/TRootEmbeddedCanvas.html">TRootEmbeddedCanvas</a> 
*fEcanvas</code></b> ] is handled by the class <b><code><a href="../TwoPadCode/TwoPadDisplay.html">TwoPadDisplay</a></code></b>. 
Look at this class to know its options. You have at hand the global pointer <b><code>gTwoPad</code></b> 
to access all the methods of <b><code><a href="../TwoPadCode/TwoPadDisplay.html">TwoPadDisplay</a></code></b>. 
You can change everything in the appearance of the display accessing the methods 
of <b><code><a href="../TwoPadCode/TwoPadDisplay.html">TwoPadDisplay</a></code></b> 
with the global pointer <b><code>gTwoPad</code></b>, or using the <b><code><a href="http://root.cern.ch/">ROOT</a></code></b> 
pointer <b><code>gStyle</code></b> pointing towards the <b><code><a href="http://root.cern.ch/">ROOT</a></code></b> 
class <b><code><a href="http://root.cern.ch/root/html/TStyle.html">TStyle</a></code></b>. 
You can also call all the methods of <b><code>TVisuLit</code></b> from the command 
prompt window, using the global pointer <b><code>gVisuLit</code></b>.</p>
<p>The 4 comments appearing on the 4 corners of the canvas can be changed using:</p>
<ul>
<li><b><code>gTwoPad-&gt;CommentTL(&quot;...&quot;);</code></b>
<li><b><code>gTwoPad-&gt;CommentBL(&quot;...&quot;);</code></b>
<li><b><code>gTwoPad-&gt;CommentBR(&quot;...&quot;);</code></b>
<li><b><code>gTwoPad-&gt;CommentTR(&quot;...&quot;);</code></b>
</ul>
<p>You can return to the defaults comments using:</p>
<ul>
<li><b><code>gTwoPad-&gt;DefCommentTL();</code></b>
<li><b><code>gTwoPad-&gt;DefCommentBL();</code></b>
<li><b><code>gTwoPad-&gt;DefCommentBR();</code></b>
<li><b><code>gTwoPad-&gt;DefCommentTR();</code></b>
</ul>
<p>The global size of the window used by <b><code>TVisuLit</code></b> is set in 
the main program, file <b><code>main.cpp</code></b>. The way it is done takes into 
account the size available in pixels of your screen. The width of the window is chosen 
as the width of your screen minus 200: line   <b><code>w -= 200</code></b>; and the 
height is tried using the &quot;golden ratio&quot; of the greek temples if possible. 
You can adjust the size of the window of <b><code>TVisuLit</code></b> changing this 
value of 200, but you have the to recompile and relink <b><code>TVisuLit</code></b>.</p>
<p></p>
END_HTML */
//
TVisuLit::TVisuLit(const TGWindow *p,UInt_t w, UInt_t h):
TGMainFrame(p,w,h) {
  //constructor creating a main frame with widgets. It is called by main.cpp
  TString s2;
  Init();
  if (gLit) gLit = 0;
  TLitPhys::Book();
  InitRef();
  fWidthGen  = w;
  fHeigthGen = h;
  fTextBR          = "Please File/Open a Litrani file";
  PrepareMenus();
  fL1 = new TGLayoutHints(kLHintsRight | kLHintsExpandX | kLHintsExpandY,10,10,10,10);
  fL2 = new TGLayoutHints(kLHintsCenterX,2,2,2,2);
  fL3 = new TGLayoutHints(kLHintsRight | kLHintsCenterY,40,40,10,10);
  fL4 = new TGLayoutHints(kLHintsRight | kLHintsCenterY,40,40,10,10);
  fL5 = new TGLayoutHints(kLHintsCenterX,2,2,2,2);
  fL6 = new TGLayoutHints(kLHintsLeft | kLHintsExpandY,10,10,10,10);
  fL7 = new TGLayoutHints(kLHintsLeft | kLHintsCenterY);
  fL8 = new TGLayoutHints(kLHintsTop | kLHintsLeft,5,5,5,5);
  //  fHFrame1=new TGHorizontalFrame(this,1000,660);
  fHFrame1=new TGHorizontalFrame(this,w,h-100);
  fListBox = new TGListBox(fHFrame1,89);
  // OLD
//  fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fHFrame1,w-180,h-100);
  // NEW
  fEcanvas = new TRootEmbeddedCanvas(0,fHFrame1,w-180,h-100);
  // END NEW
  fHFrame1->AddFrame(fListBox,fL6);
  s2 = "AllRuns";
  fListBox->AddEntry(s2.Data(),1);
  fListBox->Select(1);
  fListBox->Resize(180,500);
  fHFrame1->AddFrame(fEcanvas,fL1);
  AddFrame(fHFrame1,fL2);
  //  fHFrame2=new TGHorizontalFrame(this,1000,100);
  fHFrame2=new TGHorizontalFrame(this,w,100);
  fButtonGroup = new TGGroupFrame(fHFrame2,"To be displayed",kHorizontalFrame | kRaisedFrame);
  fRadioPar = new TGRadioButton(fButtonGroup,"VsParameter",21);
  fRadioPar->SetToolTipText("Histograms of run results versus parameter");
  fRadioRun = new TGRadioButton(fButtonGroup,"RunSpecific",22);
  fRadioRun->SetToolTipText("Runs specific histograms or sum of all runs");
  fRadioDet = new TGRadioButton(fButtonGroup,"Detectors",23);
  fRadioDet->SetToolTipText("Histograms specific to detectors");
  fRadioFit = new TGRadioButton(fButtonGroup,"Fits",24);
  fRadioFit->SetToolTipText("Display of fits used");
  fRadioSpec = new TGRadioButton(fButtonGroup,"Spectra",25);
  fRadioSpec->SetToolTipText("Display of spectra used");
  fButtonGroup->AddFrame(fRadioPar,fL8);
  fRadioPar->Connect("Pressed()","TVisuLit",this,"DoRadio()");
  fButtonGroup->AddFrame(fRadioRun,fL8);
  fRadioRun->Connect("Pressed()","TVisuLit",this,"DoRadio()");
  fButtonGroup->AddFrame(fRadioDet,fL8);
  fRadioDet->Connect("Pressed()","TVisuLit",this,"DoRadio()");
  fButtonGroup->AddFrame(fRadioFit,fL8);
  fRadioFit->Connect("Pressed()","TVisuLit",this,"DoRadio()");
  fButtonGroup->AddFrame(fRadioSpec,fL8);
  fRadioSpec->Connect("Pressed()","TVisuLit",this,"DoRadio()");
  fRadioPar->SetState(kButtonDown);
  GContext_t tggc;
  tggc = TGTextButton::GetDefaultGC()();
  FontStruct_t font;
  font = TGTextButton::GetDefaultFontStruct();
  fDrawButton = new TGTextButton(fHFrame2,"&DrawNext",-1,tggc,font,kRaisedFrame|kDoubleBorder);
  fDrawButton->SetToolTipText("draws next histogram");
  fDrawButton->Connect("Clicked()","TVisuLit",this,"DoDraw()");
  fHFrame2->AddFrame(fButtonGroup,fL7);
  fStatButton = new TGTextButton(fHFrame2,"PlotStat",-1,tggc,font,kRaisedFrame|kDoubleBorder);
  fStatButton->Connect("Clicked()","TVisuLit",this,"DoStat()");
  fStatButton->SetToolTipText("shows statistics of photons");
  fOneTwoButton = new TGTextButton(fHFrame2,"SelState",-1,tggc,font,kRaisedFrame|kDoubleBorder);
  fOneTwoButton->Connect("Clicked()","TVisuLit",this,"ToggleState()");
  fOneTwoButton->SetToolTipText("{Pad1AndPad2,Pad1Only,Pad2Only,OnePad}");
  fLinLogButton = new TGTextButton(fHFrame2,"Lin/Log",-1,tggc,font,kRaisedFrame|kDoubleBorder);
  fLinLogButton->Connect("Clicked()","TVisuLit",this,"ChangePadLogY()");
  fLinLogButton->SetToolTipText("Switch from linear Y log scale to logarithmic or vice-versa");
  fColorButton = new TGTextButton(fHFrame2,"SelColor",-1,tggc,font,kRaisedFrame|kDoubleBorder);
  fColorButton->Connect("Clicked()","TVisuLit",this,"ToggleColor()");
  fColorButton->SetToolTipText("Display in color or in grey");
  fPrintButton = new TGTextButton(fHFrame2,"Print",-1,tggc,font,kRaisedFrame|kDoubleBorder);
  fPrintButton->Connect("Clicked()","TVisuLit",this,"PrintCanvas()");
  fPrintButton->SetToolTipText("Save canvas as a file [default jpg]");
  fHFrame2->AddFrame(fPrintButton,fL4);
  fHFrame2->AddFrame(fColorButton,fL4);
  fHFrame2->AddFrame(fLinLogButton,fL4);
  fHFrame2->AddFrame(fOneTwoButton,fL4);
  fHFrame2->AddFrame(fStatButton,fL4);
  fHFrame2->AddFrame(fDrawButton, fL3);
  fButtonGroup->Resize(300,100);
  AddFrame(fHFrame2, fL5);
  //  fHFrame2->Resize(1000,100);
  fHFrame2->Resize(w,100);
  s2 = fRef->GetName();
  s2.Append("    ");
  s2.Append(fRef->GetWebSite());
  SetWindowName(s2.Data());
  MapSubwindows();
  Resize(GetDefaultSize());
  fDrawButton->Resize(70,fDrawButton->GetDefaultHeight());
  fStatButton->Resize(70,fStatButton->GetDefaultHeight());
  fOneTwoButton->Resize(70,fOneTwoButton->GetDefaultHeight());
  fLinLogButton->Resize(70,fLinLogButton->GetDefaultHeight());
  fColorButton->Resize(70,fColorButton->GetDefaultHeight());
  fPrintButton->Resize(70,fPrintButton->GetDefaultHeight());
  SetWMPosition(2,2);
  MapWindow();
  BookCanvas();
  gVisuLit = this;
}
TVisuLit::~TVisuLit() {
  //destructor
  if (fRef) {
    delete fRef;
    fRef = 0;
  }
  if (!fFileClosed) CloseFile();
  delete gTwoPad;
  gTwoPad = 0;
  Cleanup();
  gVisuLit = 0;
}
void TVisuLit::BookCanvas() {
  //Prepare a nice canvas
  // OLD
//  fCanvas = fEcanvas->GetCanvas();
//  gTwoPad = new TwoPadDisplay(fRef,fCanvas,0);
  // NEW
  UInt_t w,h;
  Int_t wid = fEcanvas->GetCanvasWindowId();
  w = fWidthGen-180;
  h = fHeigthGen-100;
  gTwoPad = new TwoPadDisplay("embedded",w,h,wid,fRef,0);
  fEcanvas->AdoptCanvas(gTwoPad);
  fCanvas = gTwoPad;
  // END NEW
  gTwoPad->CommentBR(fTextBR.Data());
}
void TVisuLit::ChangePadLogX() {
  // Go from lin to log or vice-versa in x
  gTwoPad->ChangePadLogX();
}
void TVisuLit::ChangePadLogY() {
  // Go from lin to log or vice-versa in y
  gTwoPad->ChangePadLogY();
}
void TVisuLit::CloseFile(Bool_t AlsoEntries) {
  //To close the Litrani .root file
  if (!fFileClosed) {
    Int_t nlb;
    fLitFile->Close();
    delete fLitFile;
    fLitFile    = 0;
    fLitNb      = 0;
    fLitTree    = 0;
    fLitBranch  = 0;
    gGs         = 0;
    gCs         = 0;
    gGp         = 0;
    fFitColl    = 0;
    fSpecColl   = 0;
    fNFits      = 0;
    fKFit       = 0;
    fNSpecs     = 0;
    fKSpec      = 0;
    if (AlsoEntries) {
      nlb         = fListBox->GetNumberOfEntries();
      if (nlb>1) fListBox->RemoveEntries(2,nlb);
      fListRuns.Delete();
    }
    fFileClosed = kTRUE;
  }
}
void TVisuLit::DoDraw() {
  // Draws the next histogram in the chosen category. When the last histogram is
  //reached, continues with the first.
  TResults *p = 0;
  if (fBTRchanged) {
    gTwoPad->DefCommentTR();
    gTwoPad->CommentBR(fFileName.Data());
    fBTRchanged = kFALSE;
  }
  if (fDrawChoice != ResultsVsPar) p = SelectRun();
  switch (fDrawChoice) {
    case ResultsVsPar:
      if (gGp) gGp->N1();
      break;
    case RunsResults:
      if (p) p->N1();
      break;
    case DetResults:
      if (p) p->D1();
      break;
    case FitsUsed:
      DrawNextFit();
      break;
    case SpectraUsed:
      DrawNextSpectrum();
      break;
  }
}
void TVisuLit::DoRadio() {
  // Response to radio buttons.
  Int_t id1,id2,id3,id4,id5;
  TGButton *btn = (TGButton *) gTQSender;
  Int_t id = btn->WidgetId();
  if (id >= 21 && id <= 25) {
    id1 = fRadioPar->WidgetId();
    id2 = fRadioRun->WidgetId();
    id3 = fRadioDet->WidgetId();
    id4 = fRadioFit->WidgetId();
    id5 = fRadioSpec->WidgetId();
    if (id1 != id) fRadioPar->SetState(kButtonUp);
    else fDrawChoice = ResultsVsPar;
    if (id2 != id) fRadioRun->SetState(kButtonUp);
    else fDrawChoice = RunsResults;
    if (id3 != id) fRadioDet->SetState(kButtonUp);
    else fDrawChoice = DetResults;
    if (id4 != id) fRadioFit->SetState(kButtonUp);
    else fDrawChoice = FitsUsed;
    if (id5 != id) fRadioSpec->SetState(kButtonUp);
    else fDrawChoice = SpectraUsed;
  }
}
void TVisuLit::DoStat() {
  //Shows photon statistics
  if (!fFileClosed) {
    TResults *p;
    if (fBTRchanged) {
      gTwoPad->DefCommentTR();
      gTwoPad->CommentBR(fFileName.Data());
      fBTRchanged = kFALSE;
    }
    p = SelectRun();
    if (fListBoxItem == 1) gGs->DoStat();
    else                   gCs->DoStat();
    gTwoPad->ChangePad();
  }
  else cout << "TVisuLit::DoStat ERROR : Please open a Litrani file" << endl;
}
void TVisuLit::DrawNextFit() {
  // Next fit in fFitColl is drawn
  Bool_t nofit = kTRUE;
  if (gTwoPad) {
    if (fFitColl) {
      if (fNFits) {
        TSplineFit *fit;
        gTwoPad->SetStateOfPads(OnePad);
        fit = (TSplineFit *)(*fFitColl)[fKFit];
        fit->SetDefaultLabels();
        fit->DrawFit();
        fKFit++;
        if (fKFit>=fNFits) fKFit = 0;
        nofit = kFALSE;
        fBTRchanged = kTRUE;
      }
    }
  }
  if (nofit) cout << "No fit to show" << endl;
}
void TVisuLit::DrawNextSpectrum() {
  // Next fit in fFitColl is drawn
  Bool_t nospec = kTRUE;
  if (gTwoPad) {
    if (fSpecColl) {
      if (fNSpecs) {
        TLitSpectrum *spec;
        gTwoPad->SetStateOfPads(Pad1AndPad2);
        spec = (TLitSpectrum *)(*fSpecColl)[fKSpec];
        spec->SetDefaultLabels();
        spec->DrawDecayTimes();
        spec->DrawSpectrum();
        fKFit++;
        if (fKSpec>=fNSpecs) fKSpec = 0;
        nospec = kFALSE;
        fBTRchanged = kTRUE;
      }
    }
  }
  if (nospec) cout << "No spectrum to show" << endl;
}
void TVisuLit::FirstRead() {
  // First read, to have an idea of the content in run numbers. The file is then
  //closed and reopened.
  TString s;
  TObjString *sb;
  Int_t k,krun,nentries;
  nentries = (Int_t)fLitTree->GetEntries(); 
  k = 0;
  fLitNb = 0;
  if ((!fFileClosed) && (fLitTree)) {
    while (k<nentries) {
      fLitNb += fLitTree->GetEntry(k,1);
      krun = gCs->fRun;
      s    = gCs->GetName();
      sb = new TObjString(s.Data());
      fListRuns.Add(sb);
      fListBox->AddEntry(s.Data(),k+2);
      k++;
    }
  }
  fListBox->MapSubwindows();
  fListBox->Layout();
  CloseFile(kFALSE);
  OpenFileR();
}
void TVisuLit::HandleMenu(Int_t id) {
  //Response to the activation of a menu item
  switch (id) {
    case M_FILE_OPEN:
      {
        if (!fFileClosed) CloseFile();
        static TString dir(".");
        fFi->fFileTypes = filetypes;
        fFi->fIniDir    = StrDup(dir.Data());
        new TGFileDialog(gClient->GetRoot(), this, kFDOpen, fFi);
        dir = fFi->fIniDir;
        fFileName = fFi->fFilename;
        OpenFileR();
        FirstRead();
      }
      break;
    case M_FILE_CLOSE:
      CloseFile();
      break;
    case M_FILE_EXIT:
      if (!fFileClosed) CloseFile();
      CloseWindow();   // terminate theApp no need to use SendCloseMessage()
      gApplication->Terminate(0);
      break;
    default:
      break;
  }
}
void TVisuLit::Init() {
  //Set default values in some variables
  InitP();
  ((TRint*)gROOT->GetApplication())->SetPrompt("VisuLitrani [%d] ");
  //other
  fNFits       = 0;
  fKFit        = 0;
  fNSpecs      = 0;
  fKSpec       = 0;
  fDrawChoice  = ResultsVsPar;
  fListBoxItem = -1;
  fFileName    = "";
  //Litrani file
  fFileClosed  = kTRUE;
  fLitBufSize  = 64000;
  fLitNb       = 0;
  fBTRchanged  = kFALSE;
  fInColor     = kTRUE;
  gStyle->SetHistFillColor(15);
  /*
  gStyle->SetTitleColor(1);
  gStyle->SetTitleFillColor(191);
  gStyle->SetTitleTextColor(1);
  gStyle->SetLabelSize(0.03,"xyz");
  */
}
void TVisuLit::InitP() {
  // All pointers to 0
  fFitColl     = 0;
  fSpecColl    = 0;
  fRef         = 0;
  fCanvas      = 0;
  fLitFile     = 0;
  fLitTree     = 0;
  fLitBranch   = 0;
  fLMenu       = 0;
  fLMenuItem   = 0;
  fFi          = 0;
  fMenuBar     = 0;
  fMenuFile    = 0;
  fL1          = 0;
  fL2          = 0;
  fL3          = 0;
  fL4          = 0;
  fL5          = 0;
  fL6          = 0;
  fL7          = 0;
  fL8          = 0;
  fHFrame1     = 0;
  fHFrame2     = 0;
  fEcanvas     = 0;
  fDrawButton  = 0;
  fStatButton  = 0;
  fOneTwoButton= 0;
  fLinLogButton= 0;
  fColorButton = 0;
  fPrintButton = 0;
  fListBox     = 0;
  fButtonGroup = 0;
  fRadioPar    = 0;
  fRadioRun    = 0;
  fRadioDet    = 0;
  fRadioFit    = 0;
  fRadioSpec   = 0;
  gGs          = 0;
  gCs          = 0;
  gGp          = 0;
}
void TVisuLit::InitRef() {
  // Set references of program
  TAuthor *a;
  fRef = new TReference("VisuLitrani","Visualization of ROOT files produced by Litrani","3_7",
    "http://gentit.home.cern.ch/gentit/litrani/visucl/USER_Index.html");
  a = new TAuthor("Gentit","François-Xavier");
  a->AddAddress("IRFU/SPP CEA Saclay");
  a->AddMail("gentit@hep.saclay.cea.fr");
  a->AddWebSite("http://gentit.home.cern.ch/gentit/");
  a->AddCountry("France");
  fRef->AddAuthor(a);
}
void TVisuLit::OpenFileR() {
  //Open Litrani file for read
  fCanvas->cd();
  fTextBR = fFileName.Data();
  gTwoPad->CommentBR(fTextBR.Data());
  gTwoPad->SelectPad1();
  fLitFile   = new TFile(fFileName.Data(),"READ");
  gGs = (TResults *)fLitFile->Get("GlobStat;1");
  gGp = (TPublication *)fLitFile->Get("Summary;1");
  fNFits     = 0;
  fKFit      = 0;
  fNSpecs    = 0;
  fKSpec     = 0;
  fFitColl   = 0;
  fFitColl   = (TObjArray *)fLitFile->Get("AllFits");
  if (fFitColl) fNFits     = fFitColl->GetEntries();
  fSpecColl  = 0;
  fSpecColl  = (TObjArray *)fLitFile->Get("AllSpectra");
  if (fSpecColl) fNSpecs    = fSpecColl->GetEntries();
  fLitTree   = (TTree *)fLitFile->Get("T");
  if (gCs) {
    delete gCs;
    gCs = 0;
  }
  if (fLitTree) {
    gCs = new TResults();
    fLitBranch = fLitTree->GetBranch("runstat");
    fLitBranch->SetAddress(&gCs);
  }
  fFileClosed = kFALSE;
}
void TVisuLit::PrepareMenus() {
  //Creation of the menus
  fFi    = new TGFileInfo();
  fLMenu = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX,0, 0, 1, 1);
  fLMenuItem = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
  fMenuFile = new TGPopupMenu(gClient->GetRoot());
  fMenuFile->AddEntry("&Open...", M_FILE_OPEN);
  fMenuFile->AddEntry("&Close",M_FILE_CLOSE);
  fMenuFile->AddSeparator();
  fMenuFile->AddEntry("E&xit", M_FILE_EXIT);
  fMenuFile->Connect("Activated(Int_t)", "TVisuLit", this,"HandleMenu(Int_t)");
  fMenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
  fMenuBar->AddPopup("&File", fMenuFile, fLMenuItem);
  AddFrame(fMenuBar, fLMenu);
}
void TVisuLit::PrintCanvas() {
  // Save canvas as a file. Default jpg
  gTwoPad->SaveCanvas();
}
TResults *TVisuLit::SelectRun() {
  // Select the run wanted, or the sum of all runs
  TResults *p = 0;
  if (!fFileClosed) {
    Int_t k,krun,selected;
    TString s;
    selected = fListBox->GetSelected();
    if (selected==1) p = gGs;
    else {
      if ((!gCs) || (fListBoxItem != selected)) {
        k = selected - 2;
        fLitNb += fLitTree->GetEntry(k,1);
        krun = gCs->fRun;
        s    = gCs->GetName();
      }//end if ((!gCs) || (fListBoxItem != selected))
      p = gCs;
    }//end else if (selected==1)
    fListBoxItem = selected;
  }//end if (!fFileClosed)
  else cout << "TVisuLit::SelectRun ERROR : file has been closed" << endl;
  return p;
}
void TVisuLit::ToggleColor() {
  // Switch from color to grey or vice-versa
  if (fInColor) {
    gTwoPad->SetAllGrey();
    fInColor = kFALSE;
  }
  else {
    gTwoPad->SetAllColor();
    fInColor = kTRUE;
  }
}
void TVisuLit::ToggleState() {
  //  Go from one pad to two pads or vice-versa
  StateOfPads statepad;
  gTwoPad->ToggleState();
  statepad = gTwoPad->GetStateOfPads();
  switch (statepad) {
    case Pad1AndPad2:
      fOneTwoButton->SetTitle("Pad1And2");
      break;
    case Pad1Only:
      fOneTwoButton->SetTitle("Pad1Only");
      break;
    case Pad2Only:
      fOneTwoButton->SetTitle("Pad2Only");
      break;
    case OnePad:
      fOneTwoButton->SetTitle("OnePad");
      break;
  }
}
