// @(#)root/html:$Id: T33CMatrix.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMessErr.h"
#include "T33CMatrix.h"

ClassImp(T33CMatrix)
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
//

T33CMatrix::T33CMatrix(TSComplex m00,TSComplex m10,TSComplex m20,
                       TSComplex m01,TSComplex m11,TSComplex m21,
                       TSComplex m02,TSComplex m12,TSComplex m22) {
                         //constructor with 9 components given
                         fM00 = m00;   fM10 = m10;   fM20 = m20;
                         fM01 = m01;   fM11 = m11;   fM21 = m21;
                         fM02 = m02;   fM12 = m12;   fM22 = m22;
}
T33CMatrix::T33CMatrix(const T33CMatrix &m) {
  //constructor with other T33CMatrix given given
  fM00 = m.fM00;   fM10 = m.fM10;   fM20 = m.fM20;
  fM01 = m.fM01;   fM11 = m.fM11;   fM21 = m.fM21;
  fM02 = m.fM02;   fM12 = m.fM12;   fM22 = m.fM22;
}
TSComplex T33CMatrix::Det() const {
  //Returns the determinant of this matrix
  TSComplex d;
  d = fM00*fM11*fM22  +  fM01*fM12*fM20  +  fM02*fM10*fM21  -
    fM02*fM11*fM20  -  fM00*fM12*fM21  -  fM01*fM10*fM22;
  return d;
}
void T33CMatrix::Diagonal(TSComplex a, TSComplex b, TSComplex c) {
  //Defines this matrix as being a diagonal matrix with diagonal elements a,b,c
  const Double_t zero = 0.0;
  fM00 = a;     fM01 = zero;  fM02 = zero;
  fM10 = zero;  fM11 = b;     fM12 = zero;
  fM20 = zero;  fM21 = zero;  fM22 = c;
}
void T33CMatrix::GetColumn(Int_t i,TSComplex &a,TSComplex &b,TSComplex &c) const {
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
    gMes->SetTitle("T33CMatrix");
    gMes->M(Error_M,1,"argument outside bounds");
    break;
  }
}
void T33CMatrix::GetLine(Int_t i,TSComplex &a,TSComplex &b,TSComplex &c) const {
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
    gMes->SetTitle("T33CMatrix");
    gMes->M(Error_M,1,"argument outside bounds");
    break;
  }
}
void T33CMatrix::Set(T33CMatrix &m) {
  //replacement for =
  fM00 = m.fM00; fM10 = m.fM10; fM20 = m.fM20; 
  fM01 = m.fM01; fM11 = m.fM11; fM21 = m.fM21; 
  fM02 = m.fM02; fM12 = m.fM12; fM22 = m.fM22; 
}
void T33CMatrix::SetColumn(Int_t i,TSComplex a,TSComplex b,TSComplex c) {
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
    gMes->SetTitle("T33CMatrix");
    gMes->M(Error_M,1,"argument outside bounds");
    break;
  }
}
void T33CMatrix::SetLine(Int_t i,TSComplex a,TSComplex b,TSComplex c) {
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
    gMes->SetTitle("T33CMatrix");
    gMes->M(Error_M,1,"argument outside bounds");
    break;
  }
}
T33CMatrix& T33CMatrix::operator=(const T33CMatrix &m) {
  //defines = for complex matrices
  if (this != &m) {
    fM00 = m.fM00; fM10 = m.fM10; fM20 = m.fM20; 
    fM01 = m.fM01; fM11 = m.fM11; fM21 = m.fM21; 
    fM02 = m.fM02; fM12 = m.fM12; fM22 = m.fM22; 
  }
  return *this;
}
TSComplex &T33CMatrix::operator()(Int_t i,Int_t j) const {
  //access to components
  TSComplex *p;
  switch (i) {
    case 1:
      switch (j) {
    case 1:
      p = (TSComplex *)&fM11;
      break;
    case 2:
      p = (TSComplex *)&fM12;
      break;
    default:
      p = (TSComplex *)&fM10;
      break;
      }
      break;
    case 2:
      switch (j) {
    case 1:
      p = (TSComplex *)&fM21;
      break;
    case 2:
      p = (TSComplex *)&fM22;
      break;
    default:
      p = (TSComplex *)&fM20;
      break;
      }
      break;
    default:
      switch (j) {
    case 1:
      p = (TSComplex *)&fM01;
      break;
    case 2:
      p = (TSComplex *)&fM02;
      break;
    default:
      p = (TSComplex *)&fM00;
      break;
      }
      break;
  }
  return *p;
}
TSComplex &T33CMatrix::operator()(Int_t i,Int_t j) {
  //access to components
  return (TSComplex&)((*(const T33CMatrix *)this)(i,j));
}
T33CMatrix operator*(const T33CMatrix &m1, const T33CMatrix &m2) {
  //Product of 2 complex matrices  M1 X M2
  T33CMatrix m;
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
T33CMatrix operator*(const T33CMatrix &m1, const T33Matrix &m2) {
  //Product of one complex M1 with a real matrix M2
  T33CMatrix m;
  m.fM00 = m2.fM00*m1.fM00 + m2.fM10*m1.fM01 + m2.fM20*m1.fM02;
  m.fM01 = m2.fM01*m1.fM00 + m2.fM11*m1.fM01 + m2.fM21*m1.fM02;
  m.fM02 = m2.fM02*m1.fM00 + m2.fM12*m1.fM01 + m2.fM22*m1.fM02;
  m.fM10 = m2.fM00*m1.fM10 + m2.fM10*m1.fM11 + m2.fM20*m1.fM12;
  m.fM11 = m2.fM01*m1.fM10 + m2.fM11*m1.fM11 + m2.fM21*m1.fM12;
  m.fM12 = m2.fM02*m1.fM10 + m2.fM12*m1.fM11 + m2.fM22*m1.fM12;
  m.fM20 = m2.fM00*m1.fM20 + m2.fM10*m1.fM21 + m2.fM20*m1.fM22;
  m.fM21 = m2.fM01*m1.fM20 + m2.fM11*m1.fM21 + m2.fM21*m1.fM22;
  m.fM22 = m2.fM02*m1.fM20 + m2.fM12*m1.fM21 + m2.fM22*m1.fM22;
  return m;
}
T33CMatrix operator*(const T33Matrix &m1, const T33CMatrix &m2) {
  //Product of one real matrix M1 with a complex matrix M2
  T33CMatrix m;
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
