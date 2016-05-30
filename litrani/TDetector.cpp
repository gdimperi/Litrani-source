// @(#)root/html:$Id: TDetector.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMessErr.h"
#include "TLitGlob.h"
#include "TDetector.h"

ClassImp(TDetector)
//______________________________________________________________________________
//
//        --------------------------------------------------------------
//        |                                                             |
//        |  This class is an internal class of Litrani. It is not ne-  |
//        | cessary for a user of Litrani to know anything about it.    |
//        | The documentation about it is of interest only for those    |
//        | interested in the inner working of Litrani.                 |
//        |                                                             |
//         -------------------------------------------------------------
//
// Minimal information required for a detector. Notice that the user has not
//to define himself a class TDetector to define a detector. All definitions
//of detectors in Litrani are done by calling methods of TSupplShape. See
//the class description of TSupplShape to know how to define detectors.
//
TDetector::TDetector(const char *name,const char *title,KindOfDetector kind,
  TSupplShape *shape,TFace *face) : TNamed(name,title) {
  //Constructor
  Bool_t b;
  fDet        = kind;
  fSupplShape = shape;
  fFace       = face;
  b           = AddToList(name);
}
TDetector::~TDetector() {
  //Destructor. Is not owner of fFace, nor of fSuulShape. Do not delete them.
  if (gLit) {
    gLit->fDetector.Remove(this);
    gLit->fNDetector--;
  }
}
Bool_t TDetector::AddToList() const {
  return gLit->AddOneDetector(this,fName);
}
Bool_t TDetector::AddToList(const char*name) const {
  return gLit->AddOneDetector(this,name);
}
void TDetector::InitP() {
  // Pointers to 0
  fSupplShape = 0;
  fFace       = 0;
}
void TDetector::Print() const {
  //output
  cout << endl;
  cout << "    Detector      : " << fName.Data() << endl;
  cout << "    Title         : " << fTitle.Data() << endl;
  cout << "kind of detector  : " << (long unsigned int)fDet << endl;
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "    Detector      : " << fName.Data() << endl;
  *gMes->fListing << "    Title         : " << fTitle.Data() << endl;
  *gMes->fListing << "kind of detector  : " << (long unsigned int)fDet << endl;
  *gMes->fListing << endl;
}
