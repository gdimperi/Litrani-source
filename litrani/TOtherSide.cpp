// @(#)root/html:$Id: TOtherSide.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TFace.h"
#include "TContact.h"
#include "TOtherSide.h"

ClassImp(TOtherSide)
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
//  Describes one side of a contact
//
TOtherSide::TOtherSide(KindOfContact k,TContact *c,TFace *f,TShape *s,TSupplShape *su) {
  //Constructor
  fKind    = k;
  fContact = c;
  fFace    = f;
  fShape   = s;
  fSuppl   = su;
}
TOtherSide::~TOtherSide() {
  //Destructor
}
Int_t TOtherSide::Compare(const TObject *obj) const {
  Int_t kret;
  Double_t surf1,surf2;
  TOtherSide *o2;
  TFace *f2;
  o2 = (TOtherSide *)obj;
  f2 = o2->fFace;
  surf1 = fFace->GetSurface();
  surf2 = f2->GetSurface();
  if (surf1 > surf2)
    kret = -1;
  else if (surf1 < surf2)
    kret = 1;
  else
    kret = 0;
  return kret;
}
void TOtherSide::InitP() {
  // Pointers to 0
  fContact = 0;
  fFace    = 0;
  fShape   = 0;
  fSuppl   = 0;
}
Bool_t TOtherSide::IsEqual(const TObject *obj) const {
  Double_t surf1,surf2;
  TOtherSide *o2;
  TFace *f2;
  o2 = (TOtherSide *)obj;
  f2 = o2->fFace;
  surf1 = fFace->GetSurface();
  surf2 = f2->GetSurface();
  return (surf1 == surf2);
}
