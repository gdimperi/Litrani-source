// @(#)root/html:$Id: TContact.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMessErr.h"
#include "TLitGlob.h"
#include "TSupplShape.h"
#include "ThinSlice.h"
#include "TFace.h"
#include "TContact.h"

ClassImp(TContact)
//______________________________________________________________________________
//
//  TContact declares that there is a contact between 2 different faces of 2
//different shapes. It is not necessary to specify which face of the first
//shape is in contact with which face of the second shape. It is only
//necessary to give the names of the 2 shapes in contact and to specify the
//type of contact :
//  Let us suppose that face F1 of shape 1 is in contact with face F2 of
//shape 2. Then the type of contacts are the following :
//
//   - nocontact   : there is no contact (meaningless to define a contact
//                    with nocontact !)
//   - containing  : face F1 of shape 1 contains entirely face F2 of shape 2
//   - identical   : face F1 of shape 1 is identical with face F2 of shape 2
//   - contained   : face F1 of shape 1 is contained inside face F2 of shape 2
//   - overlapping : face F1 of shape 1 is overlapping face F2 of shape 2
//
//  These characteristics of contact concerns only masterfaces. F1 and F2
//are masterfaces. Litrani understands that if 2 masterfaces F1 and F2 are
//in contact, and if F1 or F2 or both have subfaces, it has also to look at
//possible contact between subfaces. Look at the description of the class
//TFace to know what masterfaces and subfaces are.
//  The notion of contact is only relevant for plane faces. For non planar
//faces, the surface of the contact would be 0. So do not declare a contact
//if this contact is a point or a line because one of the surface in contact
//is non planar.
//  There is an exception to this rule. A cylinder or a tube may be inserted
//inside an other tube and there may be a contact between the outer cylin-
//drical surface of the inner shape and the inner cylindrical surface of
//the outer tube.
//  Notice that if, AT THE SAME TIME, a face is declared to have a revetment
//AND is declared to be in contact with an other face of an other shape,
//the part of the face which is in contact with the other shape is obviously
//NOT considered as covered by the revetment : if a contact is declared
//between 2 shapes , it means that a photon may go from one to the other
//shape, which would not be possible if there was a revetment !
//
//  The parameters of the constructor are the followings :
//
//   - name       : name of the contact
//   - title      : title of the contact
//   - NameShape1 : name of first shape in contact
//   - NameShape2 : name of 2nd shape in contact
//   - kind       : kind of contact as expained above
//   - nameTS     : name of thin slice if any, "" or "none" otherwise.
//
//  If nameTS is not absent and is neither "" nor "none", there is a thin
//slice of name nameTS inbetween the 2 faces. It is the case for instance
//between the shape made of glue ( or grease) and the shape made of silicium
//for the APD. An APD has an entrance window which is a thin slice and which
//enhances the collection of light because of the characteristics of the
//thin slice. See the definition of the class ThinSlice.
//
TContact::TContact(const char *name,const char *title,const char *NameShape1,
  const char *NameShape2,KindOfContact kind1, const char *nameTS):TNamed(name,title) {
  //  Constructor
  //  The parameters of the constructor are the following :
  //   - name       : name of the contact
  //   - title      : title of the contact
  //   - NameShape1 : name of first shape in contact
  //   - NameShape2 : name of 2nd shape in contact
  //   - kind1       : kind of contact
  //   - nameTS     : name of thin slice if any, "" or "none" otherwise.
  TSupplShape *lSuppl1=0;  //pointer to shape1 (TSupplShape)
  TSupplShape *lSuppl2=0;  //pointer to shape2 (TSupplShape)
  TFace *lFace1;           //pointer to face 1 in contact
  TFace *lFace2;           //pointer to face 2 in contact
  Int_t lNFace1;           //number of face 1 in contact
  Int_t lNFace2;           //number of face 2 in contact
  TShape *lShape1=0;       //pointer to shape 1 in contact
  TShape *lShape2=0;       //pointer to shape 2 in contact
  TString s,s1,s2,s3;
  TSupplShape *p;
  Bool_t found1 = kFALSE;
  Bool_t found2 = kFALSE;
  Bool_t contact = kFALSE;
  InitP();
  gMes->SetName("TContact");
  gMes->SetTitle("TContact");
  if (kind1==nocontact) {
    gMes->M(kill_prog,1,"Contact has not to be nocontact");
  }
  s1 = NameShape1;
  s2 = NameShape2;
  TIter next(&gLit->fSupplShape);
  while (((!found1) || (!found2)) && (p = (TSupplShape *)next())) {
    s = p->GetName();
    if (!s.CompareTo(s1)) {
      found1  = kTRUE;
      lSuppl1 = p;
      lShape1 = p->GetShape();
    }
    if (!s.CompareTo(s2)) {
      found2  = kTRUE;
      lSuppl2 = p;
      lShape2 = p->GetShape();
    }
  }
  if (!found1)
    gMes->M(kill_prog,1,"shape not found",s1);
  if (!found2)
    gMes->M(kill_prog,1,"shape not found",s2);
  contact = lSuppl1->FindContact(lSuppl2,lNFace1,lFace1,lNFace2,lFace2);
  if (contact) {
    fKind = kind1;
    KindOfContact kind2;
    switch (kind1) {
    case containing:
      kind2 = contained;
      break;
    case contained:
      kind2 = containing;
      break;
    default:
      kind2 = kind1;
      break;
    };
    fSide1 = new TOtherSide(kind1,this,lFace2,lShape2,lSuppl2);
    fSide2 = new TOtherSide(kind2,this,lFace1,lShape1,lSuppl1);
    lFace1->SetContact(fSide1);
    lFace2->SetContact(fSide2);
    s3 = nameTS;
    found1 = kFALSE;
    fThinSlice = 0;
    if ((!s3.CompareTo("")) || (!s3.CompareTo("none")))
      found1 = kTRUE;
    else {
      ThinSlice *pts;
      TIter next1(&gLit->fThinSlice);
      while ((!found1) && (pts = (ThinSlice *)next1())) {
        s = pts->GetName();
        if (!s.CompareTo(s3)) {
          found1  = kTRUE;
          fThinSlice = pts;
          fThinSlice->SetInUse();
        }
      }
    }
    if (!found1) {
      gMes->M(Error_M,2,"thin slice not found");
      gMes->M(Warning_M,2,"No thin slice assumed");
    }
    AddToList(name);
  }
  else {
    fKind = nocontact;
    gMes->M(kill_prog,1,"no contact found between those 2 shapes",s1,s2);
  }
}
TContact::~TContact() {
  // Destructor. TContact is not owner of fThinSlice. Do not delete it.
  TFace *lFace1;         //pointer to face 1 in contact
  TFace *lFace2;         //pointer to face 2 in contact
  lFace2 = fSide1->GetFace();
  lFace1 = fSide2->GetFace();
  lFace1->RemoveContact(fSide1);
  lFace2->RemoveContact(fSide2);
  if (fSide1) {
    delete fSide1;
    fSide1 = 0;
  }
  if (fSide2) {
    delete fSide2;
    fSide2 = 0;
  }
  gLit->fContact.Remove(this);
  gLit->fNContact--;
}
void TContact::InitP() {
  // Pointers to 0
  fThinSlice = 0;
  fSide1     = 0;
  fSide2     = 0;
}
Bool_t TContact::AddToList() const {
  //record the new contact into gLit->fContact
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TContact");
    gMes->M(kill_prog,1,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneContact(this,fName);
}
Bool_t TContact::AddToList(const char *name) const {
  //record the new contact into gLit->fContact
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TContact");
    gMes->M(kill_prog,2,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneContact(this,name);
}
void TContact::Print() const {
  //Prints everything about contact
  TFace *lFace1;         //pointer to face 1 in contact
  Int_t lNFace1;         //number of face 1 in contact
  TFace *lFace2;         //pointer to face 2 in contact
  Int_t lNFace2;         //number of face 2 in contact
  lFace1  = fSide1->GetFace();
  lNFace1 = lFace1->GetNumber();
  lFace2  = fSide2->GetFace();
  lNFace2 = lFace2->GetNumber();
  if (fKind != nocontact) {
    //output
    cout << endl;
    cout << "    Contact       : " << fName.Data() << endl;
    cout << "    Title         : " << fTitle.Data() << endl;
    cout << endl;
    cout << "Type of contact   : ";
    cout << "Face " << lNFace1 << " of shape " << 
      fSide1->GetSuppl()->GetName();
    switch (fKind) {
    case containing:
      cout << " contains          ";
      break;
    case identical:
      cout << " is identical with ";
      break;
    case contained:
      cout << " is contained in   ";
      break;
    case overlapping:
      cout << " is overlapping    ";
      break;
    default:
      cout << " ???? ";
      break;
    };
    cout << "Face " << lNFace2 << " of shape " << fSide2->GetSuppl()->GetName() << endl;
    if (fThinSlice)
      cout << "There is this thin slice inside the contact : " << fThinSlice->GetName() << endl;
    else
      cout << "No thin slice" << endl;
    //listing
    *gMes->fListing << endl;
    *gMes->fListing << "    Contact       : " << fName.Data() << endl;
    *gMes->fListing << "    Title         : " << fTitle.Data() << endl;
    *gMes->fListing << endl;
    *gMes->fListing << "Type of contact   : ";
    *gMes->fListing << "Face " << lNFace1 << " of shape " <<
      fSide1->GetSuppl()->GetName();
    switch (fKind) {
    case containing:
      *gMes->fListing << " contains          ";
      break;
    case identical:
      *gMes->fListing << " is identical with ";
      break;
    case contained:
      *gMes->fListing << " is contained in   ";
      break;
    case overlapping:
      *gMes->fListing << " is overlapping    ";
      break;
    default:
      *gMes->fListing << " ???? ";
      break;
    };
    *gMes->fListing << "Face " << lNFace2 << " of shape " << 
      fSide2->GetSuppl()->GetName() << endl;
    if (fThinSlice)
      *gMes->fListing << "There is this thin slice inside the contact : " << fThinSlice->GetName() << endl;
    else
      *gMes->fListing << "No thin slice" << endl;
  }
}
