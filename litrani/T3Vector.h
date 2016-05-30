// @(#)root/html:$Id: T3Vector.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_T3Vector
#define LITR_T3Vector
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// T3Vector   Implement real 3vectors in Litrani                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "Riostream.h"
#include "TObjArray.h"
#include "T33Matrix.h"

class T3Vector : public TObject {


protected:

  Double_t fV0; //1st component
  Double_t fV1; //2nd component
  Double_t fV2; //3rd component

  friend Double_t operator*(const T3Vector &v1, const T3Vector &v2);
  friend T3Vector operator*(const Double_t x,   const T3Vector &v);
  friend T3Vector operator*(const T3Vector &v,  const Double_t x);
  friend T3Vector operator*(const T33Matrix &m, const T3Vector &v);
  friend T3Vector operator*(const T3Vector &v,  const T33Matrix &m);
  friend istream &operator>>(istream&,T3Vector&);
  friend ostream &operator<<(ostream&,const T3Vector&);

public:

  T3Vector() { fV0 = 0.0; fV1 = 0.0; fV2 = 0.0; }
  T3Vector(Double_t);
  T3Vector(Double_t,Double_t,Double_t);
  T3Vector(const T3Vector &v);
  Bool_t    AlfaDelta(Double_t, Double_t, Double_t&, Double_t&, Double_t&, Double_t&) const;
  Short_t   Biggest() const;
  void      ChangeSign() { fV0=-fV0; fV1=-fV1; fV2=-fV2; }
  Double_t  CrossCylinder(const T3Vector&, Int_t&, Int_t&, Double_t&, Double_t&,
    T3Vector&, T3Vector&, const T3Vector&, const T3Vector&, Double_t, Double_t,
    Double_t&, Double_t&, Double_t = -1.0e+300) const;
  Double_t  CrossDisk(const T3Vector&, Double_t&,T3Vector&, Int_t&, const T3Vector&,
    const T3Vector&, Double_t, Double_t = -1.0e+300) const;
  Double_t  CrossPolygon(const T3Vector&,Double_t&,T3Vector&,Int_t&,Int_t,
    const T3Vector&,const TObjArray&,const TObjArray&,Double_t = -1.0e+300, Double_t = 1.0) const;
  Double_t  CrossRing(const T3Vector&, Double_t&,T3Vector&, Int_t&, const T3Vector&,
    const T3Vector&, Double_t,Double_t,Double_t = -1.0e+300) const;
  Double_t  CrossTCone(const T3Vector&, Int_t&, Int_t&, Double_t&, Double_t&,
    T3Vector&, T3Vector&, const T3Vector&, const T3Vector&, Double_t,Double_t,Double_t,
    Double_t&, Double_t&, Double_t = -1.0e+300) const;
  Short_t   DegMatrix(T33Matrix&, T3Vector&, Bool_t = kFALSE);
  void      Get(Double_t&,Double_t&,Double_t&) const;
  void      MinOrthoN(const T3Vector&, Double_t&, Double_t&);
  Double_t  Module() const;
  Double_t  Norm2Sqr() const;
  void      Print() const;
  void      RatherPositiv();
  void      Set(Double_t a0,Double_t a1,Double_t a2) { fV0=a0; fV1=a1; fV2=a2; }
  Short_t   Smallest() const;
  Double_t  TriangleSurface(const T3Vector&,const T3Vector&) const;
  void      Unit();
  Double_t  UnitVCos(const T3Vector &v) const;
  Double_t  UnitVSin(const T3Vector &v) const;
  Double_t  VCos(const T3Vector &) const;
  Double_t  VSin(const T3Vector &) const;
  T3Vector  operator+(const T3Vector&) const;
  T3Vector  operator-(const T3Vector&) const;
  T3Vector  operator^(const T3Vector&) const;
  T3Vector &operator+=(const T3Vector&);
  T3Vector &operator-=(const T3Vector&);
  T3Vector &operator=(const T3Vector&);
  Double_t &operator()(Int_t index) const;
  Double_t &operator()(Int_t index);
  ClassDef(T3Vector,1) //Implements real 3-vectors in Litrani
};
#endif
