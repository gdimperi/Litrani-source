// @(#)root/html:$Id: ThinSlice.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TMessErr.h"
#include "TLitGlob.h"
#include "ThinSlice.h"

ClassImp(ThinSlice)
//______________________________________________________________________________
//
// ThinSlice  defines a thin slice between 2 contiguous faces of 2 shapes.
//
//BEGIN_HTML <!--
/* -->
</pre>
<P>
The existence of a thin slice between 2 media can change the optical properties
dramatically. The most important case where one meets a thin slice is for
the entrance window of an APD, which is covered by a thin slice which greatly
enhances the light collection inside the APD. litrani &nbsp;allows the
insertion of thin slices between 2 contiguous faces of 2 different shapes.
The parameters of the constructor of the class ThinSlice are name, title,
width, index of refraction and magnetic permeability. The last 2 parameters
may be dependent upon wavelength, in which case you have to give the name
of the fit or interpolation, of type TSplineFit, which describes this dependency.
If you do not give the last parameter the magnetic permeability mu will be
taken as equal to 1, independent of wavelength.
<P>
<CENTER>
<TABLE BORDER="6" CELLSPACING="3" CELLPADDING="6">
<CAPTION>
Parameters of the constructor of class ThinSlice
</CAPTION>
<TR>
<TH>name</TH>
<TH>type</TH>
<TH>alternative type</TH>
<TH>explanation.</TH>
</TR>
<TR>
<TD>name</TD>
<TD>const char*</TD>
<TD>-</TD>
<TD>name of this thin slice.</TD>
</TR>
<TR>
<TD>title</TD>
<TD>const char*</TD>
<TD>-</TD>
<TD>title of this thin slice.</TD>
</TR>
<TR>
<TD>width</TD>
<TD>Double_t</TD>
<TD>-</TD>
<TD>width of the thin slice in nanometres.</TD>
</TR>
<TR>
<TD>nameIndex
<P>
or
<P>
index</TD>
<TD>const char*</TD>
<TD>Double_t</TD>
<TD>If const char* : name of the fit ( of type TSplineFit ) giving index of refraction
of
<P>
thin slice as a function of wavelength. If Double_t, fixed value of index
of 
<P>
refraction of thin slice.</TD>
</TR>
<TR>
<TD>nameMu
<P>
or
<P>
mu</TD>
<TD>const char*</TD>
<TD>Double_t</TD>
<TD>If const char* : name of the fit ( of type TSplineFit ) giving magnetic
permeability mu of
<P>
thin slice as a function of wavelength. If Double_t, fixed value of magnetic
<P>
permeability mu of thin slice.</TD>
</TR>
</TABLE>
</CENTER>
<P>
<P>
Example ( with a magnetic permeability being constant at 1.0 ) :
<P>
<P>
<TT>ROOT&gt; ThinSlice *ts;</TT>
<P>
<TT>ROOT&gt; ts = new ThinSlice("nitrure","nitrure",125.0,"Index_nitrure");</TT>
<pre>
<!--*/
// -->END_HTML
//
//  Notice that you are not permitted to introduce a thin slice between 2
//shapes, one or both of which being made of an anisotropic material. In fact,
//this is not really a limitation, since for instance concerning the thin
//slice of the window of the APD, it appears between for instance glue and
//silicium, both of which being isotropic.
//
ThinSlice::ThinSlice(const char *name,const char *title,Double_t width,const char *nameIndex,const char *nameMu)
:TNamed(name,title) {
  //constructor when index of refraction variable and magnetic permeability variable
  const Double_t dixm7 = 1.0e-7;
  TString s;
  Init();
  fWidth = dixm7*TMath::Abs(width);
  s = nameIndex;
  FindIndex(s);
  s = nameMu;
  FindMu(s);
  AddToList(name);
}
ThinSlice::ThinSlice(const char *name,const char *title,Double_t width,const char *nameIndex,Double_t mu)
:TNamed(name,title) {
  //constructor when index of refraction variable and magnetic permeability fixed
  const Double_t dixm7 = 1.0e-7;
  TString s;
  TString snone = "none";
  Init();
  fWidth = dixm7*TMath::Abs(width);
  s = nameIndex;
  FindIndex(s);
  fMu = TMath::Abs(mu);
  fNameMu = snone;
  fFitMu  = 0;
  AddToList(name);
}
ThinSlice::ThinSlice(const char *name,const char *title,Double_t width,Double_t index,const char *nameMu)
:TNamed(name,title) {
  //constructor when index of refraction fixed and magnetic permeability variable
  const Double_t un = 1.0;
  const Double_t dixm7 = 1.0e-7;
  TString s;
  TString snone = "none";
  Init();
  fWidth = dixm7*TMath::Abs(width);
  fIndex     = TMath::Abs(index);
  if (fIndex<un) fIndex=un;
  fNameIndex = snone;
  fFitIndex  = 0;
  s = nameMu;
  FindMu(s);
  AddToList(name);
}
ThinSlice::ThinSlice(const char *name,const char *title,Double_t width,Double_t index,Double_t mu)
:TNamed(name,title) {
  //constructor when index of refraction fixed and magnetic permeability fixed
  const Double_t un = 1.0;
  const Double_t dixm7 = 1.0e-7;
  TString s;
  TString snone = "none";
  Init();
  fWidth = dixm7*TMath::Abs(width);
  fIndex     = TMath::Abs(index);
  if (fIndex<un) fIndex=un;
  fNameIndex = snone;
  fFitIndex  = 0;
  fMu = TMath::Abs(mu);
  fNameMu = snone;
  fFitMu  = 0;
  AddToList(name);
}
ThinSlice::~ThinSlice() {
  // Destructor. ThinSlice is not owner of fFitIndex nor fFitMu. Do not delete.
  if (gLit) {
    gLit->fThinSlice.Remove(this);
    gLit->fNThinSlice--;
  }
}
Bool_t ThinSlice::AddToList() const {
  //record this thin slice into gLit->fThinSlice
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("ThinSlice");
    gMes->M(kill_prog,1,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneThinSlice(this,fName);
}
Bool_t ThinSlice::AddToList(const char *name) const {
  //record this thin slice into gLit->fThinSlice
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("ThinSlice");
    gMes->M(kill_prog,2,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneThinSlice(this,name);
}
Bool_t ThinSlice::CheckIt() {
  //Check that the thin slice has been defined correctly and is usable.
  const Double_t minus05 = -0.5;
  gMes->SetName("CheckIt");
  gMes->SetTitle("ThinSlice");
  if (fChecked)
    return fChecked;
  if (fIndex<minus05) {
    ProblemHeader();
    gMes->M(Error_M,1,"Fit index of thin slice not found");
    return fChecked;
  }
  if (fMu<minus05) {
    ProblemHeader();
    gMes->M(Error_M,1,"Fit for mu of thin slice not found");
    return fChecked;
  }
  fChecked = kTRUE;
  return fChecked;
}
void ThinSlice::FindIndex(const TString &s1) {
  //Finds the fit or interpolation giving the index of refraction of thin slice
  fFitIndex = TSplineFit::FindFit(s1.Data(),1);
  if (fFitIndex) fNameIndex = s1;
  else {
    gMes->SetName("FindIndex");
    gMes->SetTitle("ThinSlice");
    gMes->M(Error_M,1,"Fit for index of thin slice not found : ",s1);
    gMes->M(kill_prog,1,"Please define this fit before defining thin slice");
  }
}
void ThinSlice::FindMu(const TString &s1) {
  //Finds the fit giving the magnetic permeability mu of thin slice
  fFitMu = TSplineFit::FindFit(s1.Data(),14);
  if (fFitMu) fNameMu = s1;
  else {
    gMes->SetName("FindMu");
    gMes->SetTitle("ThinSlice");
    gMes->M(Error_M,1,"Fit for magnetic permeability mu not found",s1);
    gMes->M(kill_prog,1,"Please define this fit before defining thin slice");
  }
}
void ThinSlice::Init() {
  //basic default initialization
  const Double_t zero = 0.0;
  fFitIndex  = 0;
  fFitMu     = 0;
  fInUse     = kFALSE;
  fChecked   = kFALSE;
  fWidth     = zero;
  fIndex     = zero;
  fMu        = zero;
}
void ThinSlice::NewWavelength(Double_t wavelength) {
  //Recalculates all parameters depending upon wavelength.
  const Double_t un = 1.0;
  if (fFitIndex) {
    fIndex = TMath::Abs(fFitIndex->V(wavelength));
    if (fIndex<un) fIndex = un;
  }
  if (fFitMu)
    fMu = TMath::Abs(fFitMu->V(wavelength));
}
void ThinSlice::Print() const {
  //Print all characteristics of thin slice
  PrintTitle();
  cout << "width of thinslice [cm] : " << fWidth << endl;
  if (fFitIndex) {
    cout << "index varies with wavelength" << endl;
    cout << "name of fit for index : " << fNameIndex.Data() << endl;
  }
  else
    cout << "index fixed at        : " << fIndex << endl;
  if (fFitMu) {
    cout << "mu varies with wavelength" << endl;
    cout << "name of fit for mu    : " << fNameMu.Data() << endl;
  }
  else
    cout << "mu fixed at           : " << fMu << endl;
  if (fInUse)
    cout << "in use" << endl;
  else
    cout << "not in use" << endl;
  //listing
  *gMes->fListing << "width of thinslice [cm] : " << fWidth << endl;
  if (fFitIndex) {
    *gMes->fListing << "index varies with wavelength" << endl;
    *gMes->fListing << "name of fit for index : " << fNameIndex.Data() << endl;
  }
  else
    *gMes->fListing << "index fixed at        : " << fIndex << endl;
  if (fFitMu) {
    *gMes->fListing << "mu varies with wavelength" << endl;
    *gMes->fListing << "name of fit for mu    : " << fNameMu.Data() << endl;
  }
  else
    *gMes->fListing << "mu fixed at           : " << fMu << endl;
  if (fInUse)
    *gMes->fListing << "in use" << endl;
  else
    *gMes->fListing << "not in use" << endl;
}
void ThinSlice::PrintProblem() const {
  //Header to point out a problem
  cout << endl;
  cout << "THERE IS A PROBLEM IN THE FOLLOWING THIN SLICE :" << endl;
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "THERE IS A PROBLEM IN THE FOLLOWING THIN SLICE :" << endl;
  *gMes->fListing << endl;
}
void ThinSlice::PrintTitle() const {
  //Prints name and title of the thin slice
  cout << endl;
  cout << "    Thin Slice    : " << fName.Data() << endl;
  cout << "    Title         : " << fTitle.Data() << endl;
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "    Thin Slice    : " << fName.Data() << endl;
  *gMes->fListing << "    Title         : " << fTitle.Data() << endl;
  *gMes->fListing << endl;
}
void ThinSlice::ProblemHeader() const {
  //header before reporting a problem
  PrintProblem();
  PrintTitle();
  return;
}
