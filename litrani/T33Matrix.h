// @(#)root/html:$Id: T33Matrix.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_T33Matrix
#define LITR_T33Matrix
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// T33Matrix       Implement 3X3 complex Matrices in Litrani            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"

class T33Matrix {

public:

  Double_t fM00,fM10,fM20,fM01,fM11,fM21,fM02,fM12,fM22; //elements of matrix

  friend T33Matrix operator*(const Double_t x,    const T33Matrix &m1);
  friend T33Matrix operator*(const T33Matrix &m1, const Double_t x);
  friend T33Matrix operator*(const T33Matrix &m1, const T33Matrix &m2);
  friend T33Matrix operator&(const T33Matrix &m1, const T33Matrix &m2);

  T33Matrix() { fM00=0.0;fM10=0.0;fM20=0.0;
  fM01=0.0;fM11=0.0;fM21=0.0;
  fM02=0.0;fM12=0.0;fM22=0.0; }
  T33Matrix(Double_t,Double_t,Double_t,
    Double_t,Double_t,Double_t,
    Double_t,Double_t,Double_t);
  T33Matrix(const T33Matrix &m);
  virtual  ~T33Matrix() {;}
  Double_t   CloseUnit() const;
  Double_t   Det() const;
  void       Diagonal(Double_t,Double_t,Double_t);
  void       Diagonalize();
  void       GetColumn(Int_t i,Double_t &a,Double_t &b,Double_t &c) const;
  void       GetLine(Int_t i,Double_t &a,Double_t &b,Double_t &c) const;
  void       SetColumn(Int_t i,Double_t a,Double_t b,Double_t c);
  void       SetLine(Int_t i,Double_t a,Double_t b,Double_t c);
  Double_t   Symmetrize();
  void       T();
  void       TransposedOf(T33Matrix &m);
  void       Unit();
  void       Zero();
  T33Matrix  operator+(const T33Matrix&);
  T33Matrix  operator-(const T33Matrix&);
  T33Matrix &operator=(const T33Matrix&);
  Double_t  &operator()(Int_t i,Int_t j) const;
  Double_t  &operator()(Int_t i,Int_t j);
  ClassDef(T33Matrix,1)   //implements 3X3 real matrices
};
#endif
