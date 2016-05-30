// @(#)root/html:$Id: TDetector.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TDetector
#define LITR_TDetector
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDetector    Minimal information required for a detector             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TSupplShape.h"
#include "TFace.h"

class TDetector : public TNamed {

protected:

  KindOfDetector  fDet;         // {none,surfdet,phototube,volumedet,apd}
  TSupplShape    *fSupplShape;  //TSupplShape to which this detector is attached
  TFace          *fFace;        //Face to which this detector is attached

  Bool_t          AddToList() const;
  Bool_t          AddToList(const char*) const;
  void            InitP();

public:

  TDetector() { InitP(); }
  TDetector(const char*,const char*,KindOfDetector,TSupplShape*,TFace* = 0);
  virtual        ~TDetector();
  TFace          *GetFace() const  { return fFace;       }
  KindOfDetector  GetKind() const  { return fDet;        }
  TSupplShape    *GetShape() const { return fSupplShape; }
  void            Print() const;
  ClassDef(TDetector,1) //Minimal information required for a detector
};
#endif
