// @(#)root/html:$Id: T3CVector.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_T3CVector
#define LITR_T3CVector
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// T3CVector    Implement complex 3vectors in Litrani                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "T33CMatrix.h"

class T3CVector : public TObject {


protected:

  TSComplex fV0; //1st element
  TSComplex fV1; //2nd element
  TSComplex fV2; //3rd element

  friend TSComplex operator*(const T3CVector &v1, const T3CVector &v2);
  friend T3CVector operator*(const TSComplex x,    const T3CVector &v);
  friend T3CVector operator*(const T3CVector &v,  const TSComplex x);
  friend T3CVector operator*(const Double_t x,    const T3CVector &v);
  friend T3CVector operator*(const T3CVector &v,  const Double_t x);
  friend T3CVector operator*(const T33CMatrix &m, const T3CVector &v);

public:

  T3CVector() { fV0 = 0.0; fV1 = 0.0; fV2 = 0.0; }
  T3CVector(TSComplex);
  T3CVector(TSComplex,TSComplex,TSComplex);
  T3CVector(const T3CVector &v);
  Bool_t     AlfaDelta(TSComplex&, TSComplex&, TSComplex&, TSComplex&, TSComplex&, TSComplex&) const;
  Short_t    DegMatrix(T33CMatrix&, T3CVector&,Bool_t = kFALSE);
  void       Get(TSComplex&,TSComplex&,TSComplex&) const;
  Double_t   Module() const;
  Double_t   Norm2Sqr() const;
  void       RatherPositiv();
  void       Set(TSComplex a0,TSComplex a1,TSComplex a2) { fV0=a0; fV1=a1; fV2=a2; }
  void       Unit();
  T3CVector  operator+(const T3CVector&);
  T3CVector  operator-(const T3CVector&);
  T3CVector  operator^(const T3CVector&);
  T3CVector &operator=(const T3CVector&);
  TSComplex &operator()(Int_t index) const;
  TSComplex &operator()(Int_t index);
  ClassDef(T3CVector,1)   //Implements 3-vectors
};
#endif
