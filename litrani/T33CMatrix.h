// @(#)root/html:$Id: T33CMatrix.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_T33CMatrix
#define LITR_T33CMatrix
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// T33CMatrix    Implement 3X3 complex Matrices in Litrani              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "TSComplex.h"
#include "T33Matrix.h"

class T33CMatrix  {

public:

  TSComplex fM00,fM10,fM20,fM01,fM11,fM21,fM02,fM12,fM22; //elements of matrix

  friend T33CMatrix operator*(const T33CMatrix &m1, const T33CMatrix &m2);
  friend T33CMatrix operator*(const T33CMatrix &m1, const T33Matrix &m2);
  friend T33CMatrix operator*(const T33Matrix &m1,  const T33CMatrix &m2);

  T33CMatrix() { fM00=0.0;fM10=0.0;fM20=0.0;
  fM01=0.0;fM11=0.0;fM21=0.0;
  fM02=0.0;fM12=0.0;fM22=0.0; }
  T33CMatrix(TSComplex,TSComplex,TSComplex,
    TSComplex,TSComplex,TSComplex,
    TSComplex,TSComplex,TSComplex);
  T33CMatrix(const T33CMatrix &m);
  virtual  ~T33CMatrix() {;}
  TSComplex   Det() const;
  void        Diagonal(TSComplex,TSComplex,TSComplex);
  void        GetColumn(Int_t i,TSComplex &a,TSComplex &b,TSComplex &c) const;
  void        GetLine(Int_t i,TSComplex &a,TSComplex &b,TSComplex &c) const;
  void        Set(T33CMatrix &m);
  void        SetColumn(Int_t i,TSComplex a,TSComplex b,TSComplex c);
  void        SetLine(Int_t i,TSComplex a,TSComplex b,TSComplex c);
  T33CMatrix &operator=(const T33CMatrix&);
  TSComplex  &operator()(Int_t i,Int_t j) const;
  TSComplex  &operator()(Int_t i,Int_t j);
  ClassDef(T33CMatrix,1)   //implements 3X3 complex Matrices
};
#endif
