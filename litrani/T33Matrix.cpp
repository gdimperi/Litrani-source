// @(#)root/html:$Id: T33Matrix.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TMessErr.h"
#include "T33Matrix.h"

ClassImp(T33Matrix)
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
// Implement 3X3 real matrices in Litrani
//
//______________________________________________________________________________

T33Matrix::T33Matrix(Double_t m00,Double_t m10,Double_t m20,
                     Double_t m01,Double_t m11,Double_t m21,
                     Double_t m02,Double_t m12,Double_t m22) {
                       //constructor with 9 components given
                       fM00 = m00;   fM10 = m10;   fM20 = m20;
                       fM01 = m01;   fM11 = m11;   fM21 = m21;
                       fM02 = m02;   fM12 = m12;   fM22 = m22;
}
T33Matrix::T33Matrix(const T33Matrix &m) {
  //constructor with other T33Matrix given given
  fM00 = m.fM00;   fM10 = m.fM10;   fM20 = m.fM20;
  fM01 = m.fM01;   fM11 = m.fM11;   fM21 = m.fM21;
  fM02 = m.fM02;   fM12 = m.fM12;   fM22 = m.fM22;
}
Double_t T33Matrix::CloseUnit() const {
  //Tests how close from the unit matrix
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Double_t d=zero;
  d  = TMath::Abs(fM00-un);
  d += TMath::Abs(fM01);
  d += TMath::Abs(fM02);

  d += TMath::Abs(fM10);
  d += TMath::Abs(fM11-un);
  d += TMath::Abs(fM12);

  d += TMath::Abs(fM20);
  d += TMath::Abs(fM21);
  d += TMath::Abs(fM22-un);
  return d;
}
Double_t T33Matrix::Det() const {
  //Returns the determinant of this matrix
  Double_t d;
  d = fM00*fM11*fM22  +  fM01*fM12*fM20  +  fM02*fM10*fM21  -
    fM02*fM11*fM20  -  fM00*fM12*fM21  -  fM01*fM10*fM22;
  return d;
}
void T33Matrix::Diagonal(Double_t a, Double_t b, Double_t c) {
  //Defines this matrix as being a diagonal matrix with diagonal elements a,b,c
  const Double_t zero = 0.0;
  fM00 = a;     fM01 = zero;  fM02 = zero;
  fM10 = zero;  fM11 = b;     fM12 = zero;
  fM20 = zero;  fM21 = zero;  fM22 = c;
}
void T33Matrix::Diagonalize() {
  //Clears the non diagonal elements
  const Double_t zero = 0.0;
  fM10 = zero; fM20 = zero; fM21 = zero;
  fM01 = zero; fM02 = zero; fM12 = zero;
}
void T33Matrix::GetColumn(Int_t i,Double_t &a,Double_t &b,Double_t &c) const {
  //Defines column i as being the vector v = (a,b,c)
  switch (i) {
  case 0:
    a = fM00;  b = fM10;  c = fM20;
    break;
  case 1:
    a = fM01;  b = fM11;  c = fM21;
    break;
  case 2:
    a = fM02;  b = fM12;  c = fM22;
    break;
  default:
    gMes->SetName("GetColumn");
    gMes->SetTitle("T33Matrix");
    gMes->M(Error_M,1,"argument outside bounds");
    break;
  }
}
void T33Matrix::GetLine(Int_t i,Double_t &a,Double_t &b,Double_t &c) const {
  //Defines line i as being the vector v = (a,b,c)
  switch (i) {
  case 0:
    a = fM00;  b = fM01;  c = fM02;
    break;
  case 1:
    a = fM10;  b = fM11;  c = fM12;
    break;
  case 2:
    a = fM20;  b = fM21;  c = fM22;
    break;
  default:
    gMes->SetName("GetLine");
    gMes->SetTitle("T33Matrix");
    gMes->M(Error_M,1,"argument outside bounds");
    break;
  }
}
void T33Matrix::SetColumn(Int_t i,Double_t a,Double_t b,Double_t c) {
  //Defines column i as being the vector v = (a,b,c)
  switch (i) {
  case 0:
    fM00 = a;  fM10 = b;  fM20 = c;
    break;
  case 1:
    fM01 = a;  fM11 = b;  fM21 = c;
    break;
  case 2:
    fM02 = a;  fM12 = b;  fM22 = c;
    break;
  default:
    gMes->SetName("SetColumn");
    gMes->SetTitle("T33Matrix");
    gMes->M(Error_M,1,"argument outside bounds");
    break;
  }
}
void T33Matrix::SetLine(Int_t i,Double_t a,Double_t b,Double_t c) {
  //Defines line i as being the vector v = (a,b,c)
  switch (i) {
  case 0:
    fM00 = a;  fM01 = b;  fM02 = c;
    break;
  case 1:
    fM10 = a;  fM11 = b;  fM12 = c;
    break;
  case 2:
    fM20 = a;  fM21 = b;  fM22 = c;
    break;
  default:
    gMes->SetName("SetLine");
    gMes->SetTitle("T33Matrix");
    gMes->M(Error_M,1,"argument outside bounds");
    break;
  }
}
Double_t T33Matrix::Symmetrize() {
  //Make the matrix symmetric. Returns the sum of the absolute values of the
  //non diagonal elements of the matrix. This is useful to decide whether may
  //be the matrix is diagonal, up to numerical imprecision.
  const Double_t deux = 2.0;
  Double_t d;
  fM10 = (fM10+fM01)/deux;   fM01 = fM10;
  fM20 = (fM20+fM02)/deux;   fM02 = fM20;
  fM21 = (fM21+fM12)/deux;   fM12 = fM21;
  d = deux*(TMath::Abs(fM10) + TMath::Abs(fM20) + TMath::Abs(fM21));
  return d;
}
void T33Matrix::T() {
  //Transposes the matrix
  Double_t y;
  y = fM10;  fM10 = fM01;  fM01 = y;
  y = fM20;  fM20 = fM02;  fM02 = y;
  y = fM12;  fM12 = fM21;  fM21 = y;
}
void T33Matrix::TransposedOf(T33Matrix &m) {
  //"this" becomes the transposed of m
  fM00 = m.fM00;  fM01 = m.fM10;  fM02 = m.fM20;
  fM10 = m.fM01;  fM11 = m.fM11;  fM12 = m.fM21;
  fM20 = m.fM02;  fM21 = m.fM12;  fM22 = m.fM22;
}
void T33Matrix::Unit() {
  //Defines this matrix as being the unit matrix
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  fM00 = un;    fM01 = zero;  fM02 = zero;
  fM10 = zero;  fM11 = un;    fM12 = zero;
  fM20 = zero;  fM21 = zero;  fM22 = un;
}
void T33Matrix::Zero() {
  //Defines this matrix as being the null matrix
  const Double_t zero = 0.0;
  fM00 = zero;  fM01 = zero;  fM02 = zero;
  fM10 = zero;  fM11 = zero;  fM12 = zero;
  fM20 = zero;  fM21 = zero;  fM22 = zero;

}
T33Matrix T33Matrix::operator+(const T33Matrix &m) {
  //+ with other 3X3 matrix
  T33Matrix m4;

  m4.fM00 = fM00 + m.fM00;
  m4.fM10 = fM10 + m.fM10;
  m4.fM20 = fM20 + m.fM20;

  m4.fM01 = fM01 + m.fM01;
  m4.fM11 = fM11 + m.fM11;
  m4.fM21 = fM21 + m.fM21;

  m4.fM02 = fM02 + m.fM02;
  m4.fM12 = fM12 + m.fM12;
  m4.fM22 = fM22 + m.fM22;

  return m4;
}
T33Matrix T33Matrix::operator-(const T33Matrix &m) {
  //- with other 3X3 matrix
  T33Matrix m4;

  m4.fM00 = fM00 - m.fM00;
  m4.fM10 = fM10 - m.fM10;
  m4.fM20 = fM20 - m.fM20;

  m4.fM01 = fM01 - m.fM01;
  m4.fM11 = fM11 - m.fM11;
  m4.fM21 = fM21 - m.fM21;

  m4.fM02 = fM02 - m.fM02;
  m4.fM12 = fM12 - m.fM12;
  m4.fM22 = fM22 - m.fM22;

  return m4;
}
T33Matrix& T33Matrix::operator=(const T33Matrix &m) {
  //defines = for real matrices
  if (this != &m) {
    fM00 = m.fM00; fM10 = m.fM10; fM20 = m.fM20; 
    fM01 = m.fM01; fM11 = m.fM11; fM21 = m.fM21; 
    fM02 = m.fM02; fM12 = m.fM12; fM22 = m.fM22; 
  }
  return *this;
}
Double_t &T33Matrix::operator()(Int_t i,Int_t j) const {
  //access to components
  Double_t *p;
  switch (i) {
    case 1:
      switch (j) {
    case 1:
      p = (Double_t *)&fM11;
      break;
    case 2:
      p = (Double_t *)&fM12;
      break;
    default:
      p = (Double_t *)&fM10;
      break;
      }
      break;
    case 2:
      switch (j) {
    case 1:
      p = (Double_t *)&fM21;
      break;
    case 2:
      p = (Double_t *)&fM22;
      break;
    default:
      p = (Double_t *)&fM20;
      break;
      }
      break;
    default:
      switch (j) {
    case 1:
      p = (Double_t *)&fM01;
      break;
    case 2:
      p = (Double_t *)&fM02;
      break;
    default:
      p = (Double_t *)&fM00;
      break;
      }
      break;
  }
  return *p;
}
Double_t &T33Matrix::operator()(Int_t i,Int_t j) {
  //access to components
  return (Double_t&)((*(const T33Matrix *)this)(i,j));
}
T33Matrix operator*(const Double_t x, const T33Matrix &m) {
  //Multiplication of a matrix by a real number
  T33Matrix p;
  p.fM00 = x*m.fM00;   p.fM10 = x*m.fM10;   p.fM20 = x*m.fM20;
  p.fM01 = x*m.fM01;   p.fM11 = x*m.fM11;   p.fM21 = x*m.fM21;
  p.fM02 = x*m.fM02;   p.fM12 = x*m.fM12;   p.fM22 = x*m.fM22;
  return p;
}
T33Matrix operator*(const T33Matrix &m, const Double_t x) {
  //Multiplication of a matrix by a real number
  T33Matrix p;
  p.fM00 = x*m.fM00;   p.fM10 = x*m.fM10;   p.fM20 = x*m.fM20;
  p.fM01 = x*m.fM01;   p.fM11 = x*m.fM11;   p.fM21 = x*m.fM21;
  p.fM02 = x*m.fM02;   p.fM12 = x*m.fM12;   p.fM22 = x*m.fM22;
  return p;
}
T33Matrix operator*(const T33Matrix &m1, const T33Matrix &m2) {
  //Product of 2 matrices  M1 X M2
  T33Matrix m;
  m.fM00 = m1.fM00*m2.fM00 + m1.fM01*m2.fM10 + m1.fM02*m2.fM20;
  m.fM01 = m1.fM00*m2.fM01 + m1.fM01*m2.fM11 + m1.fM02*m2.fM21;
  m.fM02 = m1.fM00*m2.fM02 + m1.fM01*m2.fM12 + m1.fM02*m2.fM22;
  m.fM10 = m1.fM10*m2.fM00 + m1.fM11*m2.fM10 + m1.fM12*m2.fM20;
  m.fM11 = m1.fM10*m2.fM01 + m1.fM11*m2.fM11 + m1.fM12*m2.fM21;
  m.fM12 = m1.fM10*m2.fM02 + m1.fM11*m2.fM12 + m1.fM12*m2.fM22;
  m.fM20 = m1.fM20*m2.fM00 + m1.fM21*m2.fM10 + m1.fM22*m2.fM20;
  m.fM21 = m1.fM20*m2.fM01 + m1.fM21*m2.fM11 + m1.fM22*m2.fM21;
  m.fM22 = m1.fM20*m2.fM02 + m1.fM21*m2.fM12 + m1.fM22*m2.fM22;
  return m;
}
T33Matrix operator&(const T33Matrix &m1, const T33Matrix &m2) {
  //Product of 2 matrices, the 1st one being transposed M1T X M2
  T33Matrix m;
  m.fM00 = m1.fM00*m2.fM00 + m1.fM10*m2.fM10 + m1.fM20*m2.fM20;
  m.fM01 = m1.fM00*m2.fM01 + m1.fM10*m2.fM11 + m1.fM20*m2.fM21;
  m.fM02 = m1.fM00*m2.fM02 + m1.fM10*m2.fM12 + m1.fM20*m2.fM22;
  m.fM10 = m1.fM01*m2.fM00 + m1.fM11*m2.fM10 + m1.fM21*m2.fM20;
  m.fM11 = m1.fM01*m2.fM01 + m1.fM11*m2.fM11 + m1.fM21*m2.fM21;
  m.fM12 = m1.fM01*m2.fM02 + m1.fM11*m2.fM12 + m1.fM21*m2.fM22;
  m.fM20 = m1.fM02*m2.fM00 + m1.fM12*m2.fM10 + m1.fM22*m2.fM20;
  m.fM21 = m1.fM02*m2.fM01 + m1.fM12*m2.fM11 + m1.fM22*m2.fM21;
  m.fM22 = m1.fM02*m2.fM02 + m1.fM12*m2.fM12 + m1.fM22*m2.fM22;
  return m;
}
