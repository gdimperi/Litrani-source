// @(#)root/html:$Id: TOtherSide.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TOtherSide
#define LITR_TOtherSide
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOtherSide      Describes one side of a contact                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TShape.h"

class TFace;
class TSupplShape;
class TContact;

enum KindOfContact {nocontact,containing,identical,contained,overlapping};

class TOtherSide : public TObject {

protected:

  KindOfContact  fKind;    //kind of contact as seen from this side
  TContact      *fContact; //contact to which this side belongs
  TFace         *fFace;    //pointer towards the face of this side of the contact
  TShape        *fShape;   //shape to which this side belongs
  TSupplShape   *fSuppl;   //TSupplShape to which this side belongs

  void InitP();


public:

  TOtherSide()                      { InitP();         }
  TOtherSide(KindOfContact,TContact*,TFace*,TShape*,TSupplShape*);
  virtual       ~TOtherSide();
  Int_t          Compare(const TObject*) const;
  TContact      *GetContact() const { return fContact; }
  TFace         *GetFace() const    { return fFace;    }
  KindOfContact  GetKind() const    { return fKind;    }
  TShape        *GetShape() const   { return fShape;   }
  TSupplShape   *GetSuppl() const   { return fSuppl;   }
  Bool_t         IsEqual(const TObject*) const;
  Bool_t         IsSortable() const { return kTRUE;    }
  ClassDef(TOtherSide,1)   //Describes one side of a contact
};
#endif
