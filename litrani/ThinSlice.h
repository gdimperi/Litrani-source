// @(#)root/html:$Id: ThinSlice.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_ThinSlice
#define LITR_ThinSlice
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// ThinSlice   Description of a thin slice between 2 contiguous faces   //
//              of two shapes                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TSplineFit.h"

class ThinSlice : public TNamed {

protected:

  Double_t    fWidth;       //width of thin slice [cm]. Was given in [nm] to ctor.
  Double_t    fIndex;       //index of refraction of thin slice if fixed, always real.
  TString     fNameIndex;   //name of the fit giving the index of refraction as a function of wavelength
  TSplineFit *fFitIndex;    //pointer to the fit giving the index of refraction as a function of wavelength
  Double_t    fMu;          //magnetic permeability mu of the thin slice when fixed (not varying with wavelength)
  TString     fNameMu;      //name of the fit giving the magnetic permeability mu of the thin slice as a function of wavelength
  TSplineFit *fFitMu;       //pointer to the fit giving the magnetic permeability mu of the thin slice
  Bool_t      fInUse;       //true if this ThinSlice is used by a face of a shape
  Bool_t      fChecked;     //true if definition of ThinSlice correct : CheckIt() has been called

  Bool_t      AddToList() const;
  Bool_t      AddToList(const char*) const;
  void        FindIndex(const TString&);
  void        FindMu(const TString&);
  void        Init();
  void        PrintProblem() const;
  void        PrintTitle() const;
  void        ProblemHeader() const;

public:

  //Default constructor
  ThinSlice() { Init(); }
  //constructor when index of refraction variable and magnetic permeability variable
  ThinSlice(const char*, const char*, Double_t, const char*,  const char*);
  //          name     title    width     nameIndex nameMu
  //constructor when index of refraction variable and magnetic permeability fixed
  ThinSlice(const char*, const char*, Double_t, const char*,  Double_t=1.0);
  //          name     title    width     nameIndex Mu
  //constructor when index of refraction fixed    and magnetic permeability variable
  ThinSlice(const char*, const char*, Double_t, Double_t, const char*);
  //          name     title    width     Index     nameMu
  //constructor when index of refraction fixed    and magnetic permeability fixed
  ThinSlice(const char*, const char*, Double_t, Double_t, Double_t=1.0);
  //          name     title    width     Index     Mu
  virtual    ~ThinSlice();
  Bool_t      CheckIt();
  Double_t    GetIndex() const  { return fIndex;   }
  Bool_t      GetInUse() const  { return fInUse;   }
  Double_t    GetMu() const     { return fMu;      }
  Double_t    GetWidth() const  { return fWidth;   }
  void        NewWavelength(Double_t);
  void        Print() const;
  void        SetInUse()   { fInUse = kTRUE;  }
  void        SetNotUsed() { fInUse = kFALSE; }
  ClassDef(ThinSlice,0) //Thin slice between 2 contiguous faces of 2 shapes
};
#endif
