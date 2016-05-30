#include "Riostream.h"
int InitLitrani(Short_t size=5,char *name = "",char *listing = "",char *upcom = "",
 char *downcom = "",Bool_t otherseq = kFALSE,Bool_t indebug = kFALSE,Bool_t WithDate = kFALSE)
{
//
// arguments of InitLitrani:
//
// size     size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
// name     name of the produced ROOT file (without ".root")
// listing  name of the listing file (without ".lis")
// upcom    name of the comment appearing on top    left of canvas
// downcom  name of the comment appearing on bottom left of canvas
// otherseq true if one wants an other sequence of random numbers at each new launching
// indebug  true if one wants to run Litrani in debug mode
// WithDate if true, appends date and time to name of root file and name of listing
//
//  All macros using Litrani must begin with the call to this macro InitLitrani.C,
//which among other things defines the coordinates of the main canvas used by
//Litrani.
  const Bool_t litdebug = kTRUE;
  Int_t kClass,kLoad;
  ((TRint*)gROOT->GetApplication())->SetPrompt("litrani [%d] ");
  if (litdebug) {
    cout << endl;
    cout << "size     : " << size << endl;
    cout << "name     : " << name << endl;
    cout << "listing  : " << listing << endl;
    cout << "upcom    : " << upcom << endl;
    cout << "downcom  : " << downcom << endl;
    cout << "otherseq : " << (Int_t)otherseq << endl;
    cout << "indebug  : " << (Int_t)indebug << endl;
    cout << "WithDate : " << (Int_t)WithDate << endl;
    cout << endl;
  }
//
//  This first part is to cope with the fact that the user either has launched
//Litrani, or has launched ROOT. In the second case, the shared libraries of Litrani
//have to be loaded. So we detect whether shared library libLitrani has to be loaded
//in bit 16 of gROOT.
//
  kClass = TClassTable::GetID("TLitGlob");
  if (kClass<0) {
    kLoad = gSystem->Load("libTwoPad","",kTRUE);
    kLoad = gSystem->Load("libSplineFit","",kTRUE);
    kLoad = gSystem->Load("libLitrani","",kTRUE);
    if (litdebug) cout << "Start from ROOT, libLitrani to be loaded" << endl;
    switch (kLoad) {
    case -1:
      cout << "ERROR: Shared library libLitrani not found" << endl;
      break;
    case 0:
      if (litdebug) cout << "libLitrani correctly loaded" << endl;
      break;
    case 1:
      cout << "ERROR: Contradictory results from TClassTable::GetID()" << endl;
      cout << "                         and from gSystem->Load()" << endl;
      break;
    }
  }
  else {
    if (litdebug) cout << "Start from Litrani, libLitrani is already loaded" << endl;
    if (gLit) {
      delete gLit;
      gLit = 0;
      cout << "gLit deleted" << endl;
    }
  }
  if (name    == "") name    = "litrani";
  if (listing == "") listing = "litrani";
//
//  gLit is the main global pointer of Litrani. Look at the class TLitGlob to have
//a description of its features.
//
//  The 7 arguments of the constructor are the following :
//
//    size    : size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
//    nbbut   : Number of buttons wanted in TwoPadDisplay. Bigger than 10.
//              [default 20]
//    xminC   : x coordinate, in pixel, of top left corner of main canvas
//              [default 150]
//    yminC   : y coordinate, in pixel, of top left corner of main canvas
//              [default 2]
//    name    : the name of the .root file that Litrani produces, containing
//              statistics and histograms of all runs. Do not give the suffix .root.
//              It will be appended.
//              [default "litrani"]
//    listing : the name of the .lis file that Litrani produces, containing
//              messages issued by litrani. Do not give the suffix .lis . It will
//              be appended.
//              [default "litrani"] 
//    WithDate: if true, date and time are appended to the name of the ROOT file
//              and of the listing, in order not to loose them when starting again
//              the job.
//              [default false]     
//
  cout << "calling TLitGlob" << endl;
  gLit = new TLitGlob(size,24,2,2,name,listing,WithDate);
//  TLitPhys::Get()->SetLate(100.0);
//
// if otherseq true, other sequence of random number when restarting run. Litrani uses
//gRandom3, Root uses gRandom. But sometimes Litrani asks Root for random numbers, for
//instance when calling TH1::GetRandom(). So both have to be called with SetSeed().
  if (otherseq) {
    gRandom->SetSeed(0);
    gRandom3->SetSeed(0);
  }
//  gMes->SetTesTCalc(kTRUE) asks for many many checks in the computations.
  gMes->SetTestCalc(indebug);
  if (upcom   != "") gLit->fTRComment = upcom;
  if (downcom != "") gLit->fBRComment = downcom;
  return 0;
}
