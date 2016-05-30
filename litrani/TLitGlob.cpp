// @(#)root/html:$Id: TLitGlob.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TDatime.h"
#include "TObjArray.h"
#include "TwoPadDisplay.h"
#include "TSplineFit.h"
#include "TMessErr.h"
#include "TLitPhys.h"
#include "TEqIndex.h"
#include "TOpticMaterial.h"
#include "TRevetment.h"
#include "ThinSlice.h"
#include "TSupplShape.h"
#include "TContact.h"
#include "TDetector.h"
#include "TResults.h"
#include "TPublication.h"
#include "TSParticle.h"
#include "TBeam.h"
#include "TSpontan.h"
#include "TLitGlob.h"

using namespace std;

TLitGlob*       gLit = 0;

ClassImp(TLitGlob)

//______________________________________________________________________________
//
//  Defines a "global" class TLitGlob whose incarnation gLit must be
//available during the whole execution of Litrani. This class defines
//the canvas and the pads used by Litrani by calling TwoPadDisplay, with
//the method TLitGlob::BookCanvas().
//  It also contains the lists of all kind of objects created by Litrani.
//  Look at class TwoPadDisplay to have an idea of the drawing possibilities
//offered by Litrani. Look at class TResult and TPublication to have an
//idea of the available histograms. The size of the main canvas is selected
//by the argument "size" in the constructor. size 1 is extremely small and
//size 8 extremely big. But in any case, TwoPadDisplay take care of the
//number of pixels of your screen. The main canvas drawn by TwoPadDisplay
//contains buttons that allow to look at all available histograms, in turn.
//  Apart of this, the role of TLitGlob is to contain the lists of all kind
//of objects used by Litrani. When the user specifies an object by its name
//Litrani always looks inside the relevant list to see whether there exists
//an object of this name.
//  An other important method of TLitGlob is NewWavelength(), which is the
//steering routine called when the wavelength has changed, for instance
//because the birth of a new photon. NewWavelength() will update ALL para-
//meters depending upon wavelength in ALL classes in use.
//  
TLitGlob::TLitGlob(Short_t size,Int_t nbbut,Int_t xminC, Int_t yminC,
  const char *namerf,const char *namelis,Bool_t WithDate) {
  // Constructor of the class TLitGlob
  //
  //  The 7 arguments of the constructor are the following :
  //
  //    size    : Size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
  //    nbbut   : Number of buttons wanted in TwoPadDisplay. Bigger than 10.
  //              [default 20]
  //    xminC   : x coordinate, in pixel, of top left corner of main canvas
  //              [default 2]
  //    yminC   : y coordinate, in pixel, of top left corner of main canvas
  //              [default 2]
  //    namerf  : the name of the .root file that Litrani produces, containing
  //              statistics and histograms of all runs. Do not give the suffix .root.
  //              It will be appended.
  //              [default "litrani"]
  //    namelis : the name of the .lis file that Litrani produces, containing
  //              messages issued by litrani. Do not give the suffix .lis . It will
  //              be appended.
  //              [default "litrani"] 
  //    WithDate: if true, date and time are appended to the name of the ROOT file
  //              and of the listing, in order not to loose them when starting again
  //              the job.
  //              [default false]     
  //
  //  Notice method SaveFitSpec() handling class variable fSaveFitSpec. If
  //fSaveFitSpec is true [default], then the fits of type TSplineFit and the
  //spectra of type TLitSpectrum used are put on the ROOT file produced. If
  //you do not want it, call SaveFitSpec(kFALSE) after the call to the constructor
  //of TLitGlob.
  //
  TString sdate;
  fSize = size;
  if (fSize<1) fSize = 1;
  if (fSize>8) fSize = 8;
  nbbut = TMath::Abs(nbbut);
  if (nbbut < 12) nbbut = 12;
  if (nbbut > 48) nbbut = 48;
  fNbButtons = nbbut;
  fTopXGen   = xminC;
  fTopYGen   = yminC;
  TSplineFit::InitStatic();
  InitP();
  InitRef();
  PublishVersion();
  SetDate();
  if (WithDate) {
    sdate = fDate;
    sdate.ReplaceAll("  ","");
    sdate.ReplaceAll(' ',"");
    sdate.ReplaceAll('/','_');
    sdate.Append('_');
    sdate.Append(fTime);
    sdate.Prepend('_');
  }
  gStyle->SetLabelSize(0.03,"xyz");
  //
  //file initialization
  //
  fLitFileName = namerf;
  if (WithDate) fLitFileName.Append(sdate);
  fLitFileName.Append(".root");
  fLitBufSize  = 64000;
  fLitFile     = 0;
  fLitTree     = 0;
  fLitBranch   = 0;
  fFilesClosed = kTRUE;
  //
  fNCallEL = 0;
  TString litlisting = namelis;
  if (WithDate) litlisting.Append(sdate);
  gMes  = new TMessErr("notSet","notSet",(char *)litlisting.Data());
  //First initializes all physical constants used by Litrani
  TLitPhys::Book();
  //initializes all lists
  fShapeNumbered = kFALSE;
  fOldWaveLength = 0;
  fNfamily     = 0;
  fNOptMat     = 0;
  fNRevetment  = 0;
  fNThinSlice  = 0;
  fNSupplShape = 0;
  fNContact    = 0;
  fNDetector   = 0;
  fNResults    = 0;
  fNParticle   = 0;
  fNBeam       = 0;
  fNSpontan    = 0;
  TLitPhys::Get()->SetMinMaxWL(300.0,800.0);
}
TLitGlob::~TLitGlob() {
  //destructor of class TLitGlob
  if (!fFilesClosed) CloseFiles();
  TSplineFit::PurgeStatic();
  TLitSpectrum::PurgeStatic();
  DeleteCanvas();
  if (gGp) {
    delete gGp;
    gGp = 0;
  }
  if (gGs) {
    delete gGs;
    gGs = 0;
  }
  if (gCs) {
    delete gCs;
    gCs = 0;
  }
  if (gOutside) {
    fOptMat.Remove(gOutside);
    delete gOutside;
    gOutside = 0;
  }
  if (gIdx) {
    delete gIdx;
    gIdx = 0;
  }
  if (gMes) {
    delete gMes;
    gMes = 0;
  }
  TLitPhys::Kill();
  TOpticMaterial *pmat;
  TIter next2(&fOptMat);
  while ((pmat = (TOpticMaterial *)next2())) pmat->SetNotUsed();
  TRevetment *prev;
  TIter next3(&fRevetment);
  while ((prev = (TRevetment *)next3())) prev->SetNotUsed();
  ThinSlice *ptsl;
  TIter next4(&fThinSlice);
  while ((ptsl = (ThinSlice *)next4())) ptsl->SetNotUsed();
  if (fNfamily) {
    fFamily.Delete();
    fNfamily = 0;
  }
  if (fNOptMat) {
    fOptMat.Delete();
    fNOptMat = 0;
  }
  if (fNRevetment) {
    fRevetment.Delete();
    fNRevetment = 0;
  }
  if (fNThinSlice) {
    fThinSlice.Delete();
    fNThinSlice = 0;
  }
  if (fNContact) {
    fContact.Delete();
    fNContact = 0;
  }
  if (fNSupplShape) {
    fSupplShape.Delete();
    fNSupplShape = 0;
  }
  if (fNDetector) {
    fDetector.Delete();
    fNDetector = 0;
  }
  if (fNResults) {
    fResults.Delete();
    fNResults = 0;
  }
  if (fNParticle) {
    fParticle.Delete();
    fNParticle = 0;
  }
  if (fNBeam) {
    fBeam.Delete();
    fNBeam = 0;
  }
  if (fNSpontan) {
    fSpontan.Delete();
    fNSpontan = 0;
  }
  gLit = 0;
}
Bool_t TLitGlob::AddOneBeam(const TObject *Beam,const char *name) {
  //Add one Beam to the list fBeam.
  Int_t i;
  TString s1,s2;
  TBeam *p;
  TIter next(&fBeam);
  s1 = name;
  while ((p = (TBeam *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fBeam.Remove(p);
      gMes->SetName("AddOneBeam");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for Beam");
    }
  }
  fBeam.Add((TBeam *)Beam);
  fNBeam += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneBeam(const TObject *Beam,const TString &s1) {
  //Add one Beam to the list fBeam.
  Int_t i;
  TString s2;
  TBeam *p;
  TIter next(&fBeam);
  while ((p = (TBeam *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fBeam.Remove(p);
      gMes->SetName("AddOneBeam");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for Beam");
    }
  }
  fBeam.Add((TBeam *)Beam);
  fNBeam += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneContact(const TObject *Contact,const char *name) {
  //Add one Contact to the list fContact.
  Int_t i;
  TString s1,s2;
  TContact *p;
  TIter next(&fContact);
  s1 = name;
  while ((p = (TContact *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fContact.Remove(p);
      gMes->SetName("AddOneContact");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for contacts");
    }
  }
  fContact.Add((TContact *)Contact);
  fNContact += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneContact(const TObject *Contact,const TString &s1) {
  //Add one Contact to the list fContact.
  Int_t i;
  TString s2;
  TContact *p;
  TIter next(&fContact);
  while ((p = (TContact *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fContact.Remove(p);
      gMes->SetName("AddOneContact");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for contacts");
    }
  }
  fContact.Add((TContact *)Contact);
  fNContact += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneDetector(const TObject *det,const char *name) {
  //Add one detector to the list fDetector.
  Int_t i;
  TString s1,s2;
  TDetector *p;
  TIter next(&fDetector);
  s1 = name;
  while ((p = (TDetector *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fDetector.Remove(p);
      gMes->SetName("AddOneDetector");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for detectors");
    }
  }
  fDetector.Add((TDetector *)det);
  fNDetector += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneDetector(const TObject *det,const TString &s1) {
  //Add one Detector to the list fDetector.
  Int_t i;
  TString s2;
  TDetector *p;
  TIter next(&fDetector);
  while ((p = (TDetector *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fDetector.Remove(p);
      gMes->SetName("AddOneDetector");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for detectors");
    }
  }
  fDetector.Add((TDetector *)det);
  fNDetector += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneOptMat(const TObject *optmat,const char *name) {
  //Add one optical material to the list fOptMat.
  Bool_t success = kTRUE;
  Int_t i;
  TString s1,s2;
  TOpticMaterial *p;
  TOpticMaterial *psave=0;
  TIter next(&fOptMat);
  s1 = name;
  while ((p = (TOpticMaterial *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      gMes->SetName("AddOneOptMat");
      gMes->SetTitle("TLitGlob");
      if ( p->GetInUse() ) {
        gMes->M(Error_M,1,"Already existing material in use with same name");
        gMes->M(Error_M,1,"New material refused. Delete it : risk of memory leak");
        success = kFALSE;
      }
      else {
        fOptMat.Remove(p);
        gMes->M(Warning_M,2,"Already existing material with same name deleted");
        psave = p;
      }
    }
  }
  if (psave) delete psave;
  if (success) {
    fOptMat.Add((TOpticMaterial *)optmat);
    fNOptMat += 1;
  }
  return success;
}
Bool_t TLitGlob::AddOneOptMat(const TObject *optmat,const TString &s1) {
  //Add one optical material to the list fOptMat.
  Bool_t success = kTRUE;
  Int_t i;
  TString s2;
  TOpticMaterial *p;
  TOpticMaterial *psave=0;
  TIter next(&fOptMat);
  while ((p = (TOpticMaterial *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      gMes->SetName("AddOneOptMat");
      gMes->SetTitle("TLitGlob");
      if ( p->GetInUse() ) {
        gMes->M(Error_M,1,"Already existing material in use with same name");
        gMes->M(Error_M,1,"New material refused. Delete it : risk of memory leak");
        success = kFALSE;
      }
      else {
        fOptMat.Remove(p);
        gMes->M(Warning_M,2,"Already existing material with same name deleted");
        psave = p;
      }
    }
  }
  if (psave) delete psave;
  if (success) {
    fOptMat.Add((TOpticMaterial *)optmat);
    fNOptMat += 1;
  }
  return success;
}
Bool_t TLitGlob::AddOneParticle(const TObject *Particle,const char *name) {
  //Add one particle to the list fParticle.
  Int_t i;
  TString s1,s2;
  TSParticle *p;
  TIter next(&fParticle);
  s1 = name;
  while ((p = (TSParticle *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fParticle.Remove(p);
      gMes->SetName("AddOneParticle");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for Particle");
    }
  }
  fParticle.Add((TSParticle *)Particle);
  fNParticle += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneParticle(const TObject *Particle,const TString &s1) {
  //Add one Particle to the list fParticle.
  Int_t i;
  TString s2;
  TSParticle *p;
  TIter next(&fParticle);
  while ((p = (TSParticle *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fParticle.Remove(p);
      gMes->SetName("AddOneParticle");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for Particle");
    }
  }
  fParticle.Add((TSParticle *)Particle);
  fNParticle += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneResult(const TObject *Results,const char *name) {
  //Add one Results to the list fResults.
  Int_t i;
  TString s1,s2;
  TResults *p;
  TIter next(&fResults);
  s1 = name;
  while ((p = (TResults *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fResults.Remove(p);
      gMes->SetName("AddOneResult");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for results");
    }
  }
  fResults.Add((TResults *)Results);
  fNResults += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneResult(const TObject *Results,const TString &s1) {
  //Add one Results to the list fResults.
  Int_t i;
  TString s2;
  TResults *p;
  TIter next(&fResults);
  while ((p = (TResults *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fResults.Remove(p);
      gMes->SetName("AddOneResult");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for results");
    }
  }
  fResults.Add((TResults *)Results);
  fNResults += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneRevetment(const TObject *revetment,const char *name) {
  //Add one revetment to the list fRevetment.
  Bool_t success = kTRUE;
  Int_t i;
  TString s1,s2;
  TRevetment *p;
  TRevetment *psave=0;
  TIter next(&fRevetment);
  s1 = name;
  while ((p = (TRevetment *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      gMes->SetName("AddOneRevetment");
      gMes->SetTitle("TLitGlob");
      if ( p->GetInUse() ) {
        gMes->M(Error_M,1,"Already existing revetment in use with same name");
        gMes->M(Error_M,1,"New revetment refused. Delete it : risk of memory leak");
        success = kFALSE;
      }
      else {
        fRevetment.Remove(p);
        gMes->M(Warning_M,2,"Already existing revetment with same name deleted");
        psave = p;
      }
    }
  }
  if (psave) delete psave;
  if (success) {
    fRevetment.Add((TRevetment *)revetment);
    fNRevetment += 1;
  }
  return success;
}
Bool_t TLitGlob::AddOneRevetment(const TObject *revetment,const TString &s1) {
  //Add one revetment to the list fRevetment.
  Bool_t success = kTRUE;
  Int_t i;
  TString s2;
  TRevetment *p;
  TRevetment *psave=0;
  TIter next(&fRevetment);
  while ((p = (TRevetment *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      gMes->SetName("AddOneRevetment");
      gMes->SetTitle("TLitGlob");
      if ( p->GetInUse() ) {
        gMes->M(Error_M,1,"Already existing revetment in use with same name");
        gMes->M(Error_M,1,"New revetment refused. Delete it : risk of memory leak");
        success = kFALSE;
      }
      else {
        fRevetment.Remove(p);
        gMes->M(Warning_M,2,"Already existing revetment with same name deleted");
        psave = p;
      }
    }
  }
  if (psave) delete psave;
  if (success) {
    fRevetment.Add((TRevetment *)revetment);
    fNRevetment += 1;
  }
  return success;
}
Bool_t TLitGlob::AddOneSpontan(const TObject *Spontan,const char *name) {
  //Add one spontaneous source to the list fSpontan.
  Int_t i;
  TString s1,s2;
  TSpontan *p;
  TIter next(&fSpontan);
  s1 = name;
  while ((p = (TSpontan *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fSpontan.Remove(p);
      gMes->SetName("AddOneSpontan");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for Spontan");
    }
  }
  fSpontan.Add((TSpontan *)Spontan);
  fNSpontan += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneSpontan(const TObject *Spontan,const TString &s1) {
  //Add one spontaneous source to the list fSpontan.
  Int_t i;
  TString s2;
  TSpontan *p;
  TIter next(&fSpontan);
  while ((p = (TSpontan *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fSpontan.Remove(p);
      gMes->SetName("AddOneSpontan");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for Spontan");
    }
  }
  fSpontan.Add((TSpontan *)Spontan);
  fNSpontan += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneSupplShape(const TObject *SupplShape,const char *name) {
  //Add one SupplShape to the list fSupplShape.
  Int_t i;
  TString s1,s2;
  TSupplShape *p;
  TIter next(&fSupplShape);
  s1 = name;
  while ((p = (TSupplShape *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fSupplShape.Remove(p);
      gMes->SetName("AddOneSupplShape");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for shapes");
    }
  }
  fSupplShape.Add((TSupplShape *)SupplShape);
  fNSupplShape += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneSupplShape(const TObject *SupplShape,const TString &s1) {
  //Add one SupplShape to the list fSupplShape.
  Int_t i;
  TString s2;
  TSupplShape *p;
  TIter next(&fSupplShape);
  while ((p = (TSupplShape *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      fSupplShape.Remove(p);
      gMes->SetName("AddOneSupplShape");
      gMes->SetTitle("TLitGlob");
      gMes->M(Error_M,1,"twice same name for shapes");
    }
  }
  fSupplShape.Add((TSupplShape *)SupplShape);
  fNSupplShape += 1;
  return kTRUE;
}
Bool_t TLitGlob::AddOneThinSlice(const TObject *thinslice,const char *name) {
  //Add one thinslice to the list fThinSlice.
  Bool_t success = kTRUE;
  Int_t i;
  TString s1,s2;
  ThinSlice *p;
  ThinSlice *psave=0;
  TIter next(&fThinSlice);
  s1 = name;
  while ((p = (ThinSlice *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      gMes->SetName("AddOneThinSlice");
      gMes->SetTitle("TLitGlob");
      if ( p->GetInUse() ) {
        gMes->M(Error_M,1,"Already existing thin slice in use with same name");
        gMes->M(Error_M,1,"New thin slice refused. Delete it : risk of memory leak");
        success = kFALSE;
      }
      else {
        fThinSlice.Remove(p);
        gMes->M(Warning_M,2,"Already existing thin slice with same name deleted");
        psave = p;
      }
    }
  }
  if (psave) delete psave;
  if (success) {
    fThinSlice.Add((ThinSlice *)thinslice);
    fNThinSlice += 1;
  }
  return success;
}
Bool_t TLitGlob::AddOneThinSlice(const TObject *thinslice,const TString &s1) {
  //Add one thinslice to the list fThinSlice.
  Bool_t success = kTRUE;
  Int_t i;
  TString s2;
  ThinSlice *p;
  ThinSlice *psave=0;
  TIter next(&fThinSlice);
  while ((p = (ThinSlice *)next())) {
    s2 = p->GetName();
    i = s1.CompareTo(s2);
    if ( i==0 ) {
      gMes->SetName("AddOneThinSlice");
      gMes->SetTitle("TLitGlob");
      if ( p->GetInUse() ) {
        gMes->M(Error_M,1,"Already existing thin slice in use with same name");
        gMes->M(Error_M,1,"New thin slice refused. Delete it : risk of memory leak");
        success = kFALSE;
      }
      else {
        fThinSlice.Remove(p);
        gMes->M(Warning_M,2,"Already existing thin slice with same name deleted");
        psave = p;
      }
    }
  }
  if (psave) delete psave;
  if (success) {
    fThinSlice.Add((ThinSlice *)thinslice);
    fNThinSlice += 1;
  }
  return success;
}
void TLitGlob::BookCanvas() {
  // Book main canvas of Litrani
  if (!gTwoPad) {
    if (!fRef) InitRef();
    new TwoPadDisplay(fRef,fSize,fNbButtons,fTopXGen,fTopYGen);
    gTwoPad->SetButtonFast(5,"Next Fit","gLit->fBTRchanged=kTRUE;TSplineFit::DrawNextInCollection();");
    gTwoPad->SetButtonFast(6,"============",";");
    gTwoPad->SetButtonFast(7,"Next Spectrum","TLitSpectrum::DrawNextInCollection();");
    gTwoPad->SetButtonFast(8,"RunDependent","gGp->N1();");
    gTwoPad->SetButtonFast(9,"Detectors","gGs->D1();");
    gTwoPad->SetButtonFast(10,"SumOfRuns","gGs->N1();");
    gTwoPad->SetButtonFast(11,"============",";");
    gTwoPad->UpdateCanvas();
  }
  if (fTRComment.Length()>0) gTwoPad->CommentTR(fTRComment.Data());
  if (fBRComment.Length()>0) gTwoPad->CommentBR(fBRComment.Data());
}
void TLitGlob::ChangePad() {
  // Only there for compatibility reasons with old CINT macros
  // Please use gTwoPad->ChangePad() instead!
  if (!gTwoPad) BookCanvas();
  gTwoPad->ChangePad();
}
Bool_t TLitGlob::CheckContacts() {
  // Useful method to call at the end of the geometry building, to check
  //whether contacts have been correctly defined
  // All shapes should be in WCS. Added by David Wahl.
  gMes->SetName("CheckContacts");
  gMes->SetTitle("TLitGlob");
  Bool_t missingcontact = kFALSE;
  Int_t iface1, iface2;
  TFace* face1=0;
  TFace* face2=0;
  TSupplShape *pshape;
  TSupplShape *pshapetwo;
  TIter next(&fSupplShape);
  TIter next2(&fSupplShape);
  while ((pshape = (TSupplShape *)next())) {
    next2.Reset();
    missingcontact = kFALSE;
    while ((pshapetwo = (TSupplShape*)next2())) {      
      if (!(pshape==pshapetwo)) {
        // Check all contacts have been defined
        if (pshape->FindContact(pshapetwo,iface1,face1,iface2,face2)
          && !face1->HasContact()) {
            gMes->M(Error_M,1,"---- No Contact ---");
            gMes->M(Error_M,1,"When checking shape",pshape->GetName());
            gMes->M(Error_M,1,"Contact expected with", pshapetwo->GetName());
            missingcontact = kTRUE;}
      }
    }
  }
  return !missingcontact;
}
Bool_t TLitGlob::CheckOverlaps() {
  // Useful method to call at the end of the geometry building, to check
  //absence of overlaps
  //All shapes should be in WCS. Added by David Wahl.
  gMes->SetName("CheckOverlaps");
  gMes->SetTitle("TLitGlob");
  Bool_t hasoverlap = kFALSE;
  Int_t iface1, iface2;
  TFace* face1=0;
  TFace* face2=0;
  TSupplShape *pshape;
  TSupplShape *pshapetwo;
  // Loop over shapes
  // Take pshape and loop over all pshapetwo to look for overlaps
  TIter next(&fSupplShape);
  TIter next2(&fSupplShape);
  while ((pshape = (TSupplShape *)next())) {
    next2.Reset();
    while ((pshapetwo = (TSupplShape*)next2())) {      
      if (!(pshape==pshapetwo)) {
        // Loop over all the faces of pshapetwo
        hasoverlap = kFALSE;
        TFace* pface;
        TIter faces(pshapetwo->GetListOfFaces());
        while ((pface = (TFace*)faces())) {
          // Loop over all the points on the current face
          for (Int_t i = 0; i<pface->NumberOfPoints(); ++i) {
            Double_t x,y,z = 0.;
            pface->Point(i)->Get(x,y,z);
            if (pshape->IsInside(x,y,z) && pshapetwo->IsInside(x,y,z) &&
              !pshape->FindContact(pshapetwo,iface1,face1,iface2,face2)) {
                gMes->M(Error_M,1,"---- Overlap ---");
                gMes->M(Error_M,1,"When checking shape",pshape->GetName());
                gMes->M(Error_M,1,"Overlap detected with", pshapetwo->GetName());
                hasoverlap = kTRUE;}
          } // End of loop over points on face
        } // End of loop over faces of shapetwo
      }
    } // End of loop over second shapes
  } // End of loop over first shapes
  // No overlaps were found
  return !hasoverlap;
}
void TLitGlob::CloseFiles() {
  // To be called after all runs of generation of photons. Close files open
  //for writing. Do not forget this important call at the end of your macros.
  //
  if (!fFilesClosed) {
    Int_t Ns;
    gGs->Conclusion();
    fLitFile->cd();
    if (fLitTree) fLitTree->Write();
    gGs->Write();
    if (gGp) gGp->Write();
    if (fSaveFitSpec) {
      TSplineFit::WriteCollection();
      if (TLitSpectrum::fgSpectra) {
        Ns = TLitSpectrum::fgSpectra->GetEntries();
        if (Ns) {
          TLitSpectrum::fgSpectra->Write("AllSpectra",1);
        }
      }
    }
    fLitFile->Close();
    delete fLitFile;
    fLitFile     = 0;
    fLitTree     = 0;
    fLitBranch   = 0;
    fLitNb       = 0;
    fFilesClosed = kTRUE;
    gROOT->cd();
  }
}
void TLitGlob::CloseFilesR() {
  // To close the .root when it was opened for reading.
  //
  if (!fFilesClosed) {
    fLitFile->cd();
    fLitFile->Close();
    delete fLitFile;
    fLitFile     = 0;
    fLitTree     = 0;
    fLitBranch   = 0;
    fLitNb       = 0;
    fNFits       = 0;
    fKFit        = 0;
    fFitColl     = 0;
    fNSpecs      = 0;
    fKSpec       = 0;
    fSpecColl    = 0;
    fFilesClosed = kTRUE;
    gROOT->cd();
  }
}
void TLitGlob::DeleteCanvas() {
  //Delete Canvas
  if (fRef) {
    delete fRef;
    fRef = 0;
  }
  if (gTwoPad) {
    if (gTwoPad->CanDelete()) {
      delete gTwoPad;
      gTwoPad = 0;
    }
  }
}
void TLitGlob::DrawNextFit() {
  // Next fit in fFitColl is drawn
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
      }
    }
  }
}
void TLitGlob::DrawNextSpectrum() {
  // Next fit in fFitColl is drawn
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
      }
    }
  }
}
void TLitGlob::F() {
  //
  //Draws in turn all fit histograms
  //
  //    HISTOGRAMS ASSOCIATED WITH FITS AND INTERPOLATION.  Each time
  //one defines a fit or an interpolation in Litrani ( with the class TSplineFit)
  //histograms are created to show the results of the fit or interpolation.
  //If you want to see ALL histograms of fits or interpolation, do :
  //
  //  gLit->F();
  //
  TSplineFit::DrawFitsInCollection();
  gTwoPad->SetStateOfPads(Pad1AndPad2);
}
Int_t TLitGlob::GiveNbToShape() {
  //Assign a number to each shape, to help making statistics on shapes, like
  //number of photons absorbed in each shape.
  Int_t k=0;
  Int_t f;
  TSupplShape *p;
  fMaxNbOfFaces = 0;
  TIter next(&fSupplShape);
  for (k=0;k<fNSupplShape;k++) {
    p = (TSupplShape *)next();
    f = p->GetNbOfFaces();
    if (f>fMaxNbOfFaces) fMaxNbOfFaces = f;
    p->fShapeNumber = k;
  }
  fShapeNumbered = kTRUE;
  return fNSupplShape;
}
void TLitGlob::HistosManager() {
  // Do nothing. Still there to avoid crash of old CINT Macros
}
void TLitGlob::InitP() {
  // Pointers to 0 and default options
  fRef         = 0;
  fLitFile     = 0;
  fLitTree     = 0;
  fLitBranch   = 0;
  fSaveFitSpec = kTRUE;
  fPrintFreq   = 5000;
  fBTRchanged  = kFALSE;
  fTRComment   = "";
  fBRComment   = "";
}
void TLitGlob::InitRef() {
  // Enter references of program
  TAuthor *a, *b;
  if (fRef) delete fRef;
  fRef = new TReference("litrani","Monte-Carlo of optical photons");
  fRef->SetVersion("3_14");
  fRef->SetWebSite("http://gentit.home.cern.ch/gentit/litrani/");
  fRef->SetLanguage("C++");
  fRef->SetFramework("ROOT");
  a = new TAuthor("Gentit","François-Xavier");
  a->AddAddress("IRFU/SPP CEA Saclay");
  a->AddMail("gentit@hep.saclay.cea.fr");
  a->AddWebSite("http://gentit.home.cern.ch/gentit/");
  a->AddCountry("France");
  fRef->AddAuthor(a);
  b = new TAuthor("Wahl","David");
  b->AddAddress("London");
  b->AddMail("david.n.wahl@gmail.com");
  b->AddCountry("England");
  fRef->AddAuthor(b);
}
void TLitGlob::NameIOFile(const char *name) {
  //  Allow to change the name of the I/O file [default litrani.root] used by
  //Litrani. Do not give the suffix ".root", it will be appended. Do not
  //change the name of the I/O file between runs ! Do it before the generation
  //of the first photon.
  fLitFileName = name;
  fLitFileName.Append(".root");
}
void TLitGlob::NewWavelength(Double_t wavelength,Bool_t ForceIt) {
  //recalculates everything depending upon wavelength
  const Double_t eps = 1.0e-10;
  if ((TMath::Abs(fOldWaveLength-wavelength) > eps) || ForceIt) {
    fOldWaveLength = wavelength;
    //all materials
    TOpticMaterial *pmat;
    TIter next1(&fOptMat);
    while ((pmat = (TOpticMaterial *)next1())) {
      pmat->NewWavelength(wavelength);
    }
    //all revetments
    TRevetment *prev;
    TIter next2(&fRevetment);
    while ((prev = (TRevetment *)next2())) {
      prev->NewWavelength(wavelength);
    }
    //all thin slices
    ThinSlice *pth;
    TIter next3(&fThinSlice);
    while ((pth = (ThinSlice *)next3())) {
      pth->NewWavelength(wavelength);
    }
    //all detectors
    //all SupplShape to recalculate dielectric tensor
    TSupplShape *psup;
    TIter next5(&fSupplShape);
    while ((psup = (TSupplShape *)next5())) {
      psup->NewWavelength(wavelength);
      pmat = psup->GetOptMat();
      if ((pmat->GetAnisotropy()) && (pmat->GetIChangeWithWvl())) psup->SetDielTensor();
    }
  }
}
void TLitGlob::NF() const {
  // Draw next fit used
  TSplineFit::DrawNextInCollection();
}
void TLitGlob::OpenFilesR() {
  // Open for read the .root file that Litrani has created and whose name is
  //in fLitFileName.
  fLitNb = 0;
  fLitFile   = new TFile(fLitFileName.Data(),"READ");
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
  fFilesClosed = kFALSE;
}
void TLitGlob::OpenFilesR(const char *name) {
  //  Open for read the .root file that Litrani has created. "name" is the
  //name of the file, without the suffix ".root". Default "litrani".
  NameIOFile(name);
  OpenFilesR();
}
void TLitGlob::OpenFilesW(Bool_t withTree) {
  //  Open for writing the .root file that Litrani will create. "name" is the
  //name of the file, without the suffix ".root". Default "litrani".
  fLitNb = 0;
  fLitFile   = new TFile(fLitFileName.Data(),"RECREATE");
  if (withTree) {
    fLitTree   = new TTree("T","Statistics and Histos for each run");
    if (!gCs) gCs = new TResults();
    fLitBranch = fLitTree->Branch("runstat","TResults",&gCs,fLitBufSize,0);
    //    fNResults++;
  }
  fNResults = fResults.GetEntries();
  fFilesClosed = kFALSE;
}
void TLitGlob::PrintAllBeam() const {
  //Prints characteristics of all Beams.
  cout << endl;
  cout << "Number of Beams :" << fNBeam << endl;
  cout << endl;
  TIter next(&fBeam);
  TBeam *pc;
  while (( pc = (TBeam *)next())) {
    pc->Print();
  }
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "Number of Beams :" << fNBeam << endl;
  *gMes->fListing << endl;
  TIter next1(&fBeam);
  while (( pc = (TBeam *)next1())) {
    pc->Print();
  }
  *gMes->fListing << endl;
}
void TLitGlob::PrintAllContact() const {
  //Prints characteristics of all TContacts.
  cout << endl;
  cout << "Number of contacts :" << fNContact << endl;
  cout << endl;
  TIter next(&fContact);
  TContact *pc;
  while (( pc = (TContact *)next())) {
    pc->Print();
  }
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "Number of contacts :" << fNContact << endl;
  *gMes->fListing << endl;
  TIter next1(&fContact);
  while (( pc = (TContact *)next1())) {
    pc->Print();
  }
  *gMes->fListing << endl;
}
void TLitGlob::PrintAllDetector() const {
  //Prints characteristics of all detectors.
  cout << endl;
  cout << "Number of Detectors :" << fNDetector << endl;
  cout << endl;
  TIter next(&fDetector);
  TDetector *pc;
  while (( pc = (TDetector *)next())) {
    pc->Print();
  }
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "Number of Detectors :" << fNDetector << endl;
  *gMes->fListing << endl;
  TIter next1(&fDetector);
  while (( pc = (TDetector *)next1())) {
    pc->Print();
  }
  *gMes->fListing << endl;
}
void TLitGlob::PrintAllOptMat() const {
  //Prints characteristics of all optical materials.
  cout << endl;
  cout << "Number of optical materials :" << fNOptMat << endl;
  cout << endl;
  TIter next(&fOptMat);
  TOpticMaterial *pc;
  while (( pc = (TOpticMaterial *)next())) {
    pc->Print();
  }
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "Number of optical materials :" << fNOptMat << endl;
  *gMes->fListing << endl;
  TIter next1(&fOptMat);
  while (( pc = (TOpticMaterial *)next1())) {
    pc->Print();
  }
  *gMes->fListing << endl;
}
void TLitGlob::PrintAllParticle() const {
  //Prints characteristics of all particles.
  cout << endl;
  cout << "Number of Particles :" << fNParticle << endl;
  cout << endl;
  TIter next(&fParticle);
  TSParticle *pc;
  while (( pc = (TSParticle *)next())) {
    pc->Print();
  }
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "Number of Particles :" << fNParticle << endl;
  *gMes->fListing << endl;
  TIter next1(&fParticle);
  while (( pc = (TSParticle *)next1())) {
    pc->Print();
  }
  *gMes->fListing << endl;
}
void TLitGlob::PrintAllRevetment() const {
  //Prints characteristics of all revetments.
  cout << endl;
  cout << "Number of revetments :" << fNRevetment << endl;
  cout << endl;
  TIter next(&fRevetment);
  TRevetment *pc;
  while (( pc = (TRevetment *)next())) {
    pc->Print();
  }
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "Number of revetments :" << fNRevetment << endl;
  *gMes->fListing << endl;
  TIter next1(&fRevetment);
  while (( pc = (TRevetment *)next1())) {
    pc->Print();
  }
  *gMes->fListing << endl;
}
void TLitGlob::PrintAllSpontan() const {
  //Prints characteristics of all spontaneous sources.
  cout << endl;
  cout << "Nb. of spontaneous sources :" << fNSpontan << endl;
  cout << endl;
  TIter next(&fSpontan);
  TSpontan *pc;
  while (( pc = (TSpontan *)next())) {
    pc->Print();
  }
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "Nb. of spontaneous sources :" << fNSpontan << endl;
  *gMes->fListing << endl;
  TIter next1(&fSpontan);
  while (( pc = (TSpontan *)next1())) {
    pc->Print();
  }
  *gMes->fListing << endl;
}
void TLitGlob::PrintAllSupplShape() const {
  //Prints characteristics of all TSupplShapes.
  cout << endl;
  cout << "Number of suppl. shape :" << fNSupplShape << endl;
  cout << endl;
  TIter next(&fSupplShape);
  TSupplShape *pc;
  while (( pc = (TSupplShape *)next())) {
    pc->Print();
  }
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "Number of suppl. shape :" << fNSupplShape << endl;
  *gMes->fListing << endl;
  TIter next1(&fSupplShape);
  while (( pc = (TSupplShape *)next1())) {
    pc->Print();
  }
  *gMes->fListing << endl;
}
void TLitGlob::PrintAllThinSlice() const {
  //Prints characteristics of all thinslices.
  cout << endl;
  cout << "Number of thin slices :" << fNThinSlice << endl;
  cout << endl;
  TIter next(&fThinSlice);
  ThinSlice *pc;
  while (( pc = (ThinSlice *)next())) {
    pc->Print();
  }
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "Number of thin slices :" << fNThinSlice << endl;
  *gMes->fListing << endl;
  TIter next1(&fThinSlice);
  while (( pc = (ThinSlice *)next1())) {
    pc->Print();
  }
  *gMes->fListing << endl;
}
void TLitGlob::PrintGlobalResults() const {
  //Prints global statistics of all runs
  TResults *pc;
  pc = (TResults *)fResults[0];
  pc->Print();
}
void TLitGlob::PublishVersion() const {
  // Initial Welcome and version number
  cout << endl;
  cout << "  *******************************************" << endl;
  cout << "  *                                         *" << endl;
  cout << "  *      W E L CO M E to L I T R A N I      *" << endl;
  cout << "  *                                         *" << endl;
  cout << "  *            Version ";
  cout << fRef->GetVersion();
  cout << "                 *" << endl;
  cout << "  *                                         *" << endl;
  cout << "  *******************************************" << endl;
  cout << endl;
}
void TLitGlob::RestoreComments() {
  // Restore default comments if fBTRchanged is true
  if (gTwoPad && fBTRchanged) {
    gTwoPad->CommentBR(fBRComment.Data());
    gTwoPad->CommentTR(fTRComment.Data());
    fBTRchanged = kFALSE;
  }
}
Bool_t TLitGlob::SelectRun(Int_t run) {
  //  Given the run number, make the current statistics pointer gCs point
  //towards this run. If there are many different runs with different
  //names but identical run number, use the other version of SelectRun !
  //  This method is NOT intended to be used during the generation of photons,
  //between 2 runs. It is intended to be used :
  //    (1) - after all runs have been produced,
  //    (2) - after gLit->CloseFiles() has been called,
  //    (3) - and before calling it, you must have opened the .root file for
  //           reading by a call to gLit->OpenFilesR().
  //
  Int_t k,nentries,krun;
  krun = 0;
  if (fFilesClosed) {
    gMes->SetName("SelectRun");
    gMes->SetTitle("TLitGlob");
    gMes->M(Error_M,1,"You must first open the .root file calling OpenFilesR");
    gMes->M(Warning_M,1,"We try to call OpenFilesR with argument <litrani>");
    OpenFilesR();
  }
  if (fLitTree) {
    nentries = (Int_t)fLitTree->GetEntries(); 
    k = 0;
    fLitNb = 0;
    while ((krun != run) && (k<nentries)) {
      fLitNb += fLitTree->GetEntry(k,1);
      krun = gCs->fRun;
      k++;
    }
  }
  else {
    gMes->SetName("SelectRun");
    gMes->SetTitle("TLitGlob");
    gMes->M(Warning_M,2,"Tree of individual runs not on .root file");
  }
  return (krun == run);
}
Bool_t TLitGlob::SelectRun(TString name, Int_t run) {
  //  Given the name of the run and the run number, make the current
  //statistics pointer gCs point towards the statistics and histograms of
  //the corresponding run. If you have many different sources of photons,
  //having each a different name, it is quite possible that you have twice
  //the same run number, each belonging to a different source of photon.
  //If it is the case in your set-up, call this instance of SelectRun,
  //not the preceeding one.
  //  Notice that name is the name of the source of photons having generated
  //this run.
  //  This method is NOT intended to be used during the generation of photons,
  //between 2 runs. It is intended to be used :
  //    (1) - after all runs have been produced,
  //    (2) - after gLit->CloseFiles() has been called,
  //    (3) - and before calling it, you must have opened the .root file for
  //           reading by a call to gLit->OpenFilesR().
  //
  const Int_t kBufferSize = 64;
  Int_t k,nentries,krun;
  TString s = "";
  krun = 0;
  if (fFilesClosed) {
    gMes->SetName("SelectRun");
    gMes->SetTitle("TLitGlob");
    gMes->M(Error_M,2,"You must first open the .root file calling gLit->OpenFilesR");
    gMes->M(Warning_M,2,"We try to call OpenFilesR with argument <litrani>");
    OpenFilesR();
  }
  //construct the full name with run number
  Int_t l,l1;
  l1 = kBufferSize - 10;
  l = name.Length();
  if (l>l1) name.Remove(l1);
  TString fullname;
  fullname = name;
  fullname.Append('_');
  fullname += run;
  if (fLitTree) {
    nentries = (Int_t)fLitTree->GetEntries(); 
    k = 0;
    fLitNb = 0;
    while (((krun != run) || (s.CompareTo(fullname))) && (k<nentries)) {
      fLitNb += fLitTree->GetEntry(k,1);
      krun = gCs->fRun;
      s    = gCs->GetName();
      k++;
    }
  }
  else {
    gMes->SetName("SelectRun");
    gMes->SetTitle("TLitGlob");
    gMes->M(Warning_M,3,"Tree of individual runs not on .root file");
  }
  return (krun == run);
}
void TLitGlob::SetDate() {
  //Finds the date of to-day
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
void TLitGlob::VerifyOutside() const {
  //verifies that gOutside still points to the material "Outside". This is
  //necessary since the user may have redefined the material "Outside" !
  TOpticMaterial *pmat;
  TIter next(&fOptMat);
  Bool_t found = kFALSE;
  TString s;
  TString s1 = "Outside";
  while ((!found) && (pmat = (TOpticMaterial *)next())) {
    s = pmat->GetName();
    if (!s.CompareTo(s1)) {
      found = kTRUE;
      gOutside = pmat;
    }
  }
}
