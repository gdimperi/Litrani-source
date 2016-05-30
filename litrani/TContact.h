// @(#)root/html:$Id: TContact.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TContact
#define LITR_TContact
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TContact  Declares that there is a contact between 2 TSShape, i-e    //
//           a photon can pass from shape 1 to shape 2 or vice-versa    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TShape.h"
#include "TOtherSide.h"

class ThinSlice;
class TSupplShape;

class TContact : public TNamed {

protected:

  KindOfContact  fKind;      //type of contact between the 2 faces
  ThinSlice     *fThinSlice; //pointer to thin slice if any
  TOtherSide    *fSide1;     //side 1 of contact
  TOtherSide    *fSide2;     //side 2 of contact

  Bool_t         AddToList() const;
  Bool_t         AddToList(const char*) const;
  void           InitP();

public:

  TContact() { InitP(); }
  TContact(const char*, const char*, const char*, const char*, KindOfContact,
    const char* = "none");
  virtual       ~TContact();
  TShape        *GetShape1()    const { return fSide1->GetShape(); }
  TShape        *GetShape2()    const { return fSide2->GetShape(); }
  TSupplShape   *GetSuppl1()    const { return fSide1->GetSuppl(); }
  TSupplShape   *GetSuppl2()    const { return fSide2->GetSuppl(); }
  ThinSlice     *GetThinSlice() const { return fThinSlice;         }
  void           Print() const;
  ClassDef(TContact,0)//Declares a contact between 2 shapes
};
#endif
