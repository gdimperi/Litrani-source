// @(#)root/html:$Id: TRevetment.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TRevetment
#define LITR_TRevetment
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRevetment   Description of the wrapping of a face of a shape, with  //
//              its optical properties                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TSplineFit.h"
#include "TSComplex.h"
#include "TOpticMaterial.h"

class TRevetment : public TNamed {

protected:

  Bool_t          fIsSlice;        //true if there is a slice of material between the face of the shape and the revetment
  TOpticMaterial *fSlice;          //pointer to material in the slice between the face of the shape and the revetment
  Double_t        fDiffus;         //proportion of diffusion when fixed (not varying with wavelength)
  TString         fNameDiffus;     //name of the fit giving the proportion of light diffused as a function of wavelength
  TSplineFit     *fFitDiffus;      //pointer to the fit giving the proportion of light diffused for revetment
  Double_t        fSinThetaMax;    //sinus of the 9th argument of the constructors.
  Double_t        fRindex;         //real part of index of refraction when fixed (not varying with wavelength)
  TString         fNameRindex;     //name of the fit giving the real part of the index of refraction of the revetment as a function of wavelength
  TSplineFit     *fFitRindex;      //pointer to the fit giving the real part of index of refraction of revetment
  Double_t        fIindex;         //imaginary part of index of refraction when fixed (not varying with wavelength)
  TString         fNameIindex;     //name of the fit giving the imaginary part of the index of refraction of the revetment as a function of wavelength
  TSplineFit     *fFitIindex;      //pointer to the fit giving the imaginary part of index of refraction of revetment
  Double_t        fMu;             //magnetic permeability mu when fixed (not varying with wavelength)
  TString         fNameMu;         //name of the fit giving the magnetic permeability mu of the revetment as a function of wavelength
  TSplineFit     *fFitMu;          //pointer to the fit giving the magnetic permeability mu of revetment
  Short_t         fAbsorbing;      //o : no supplementary absorption.  1 : some suppl. abs.   2 : revetment 100% absorbing
  Double_t        fSupplAbs;       //[0..1] supplementary absorption, (f.e. : dust). Default 0
  Bool_t          fInUse;          //true if this revetment is used by a face of a shape
  Bool_t          fChecked;        //true if definition of revetment correct : CheckIt() has been called

  Bool_t   AddToList() const;
  Bool_t   AddToList(const char*) const;
  void     FindDiffus(const TString&);   //Find % of diffusion for revetment.
  void     FindIindex(const TString&);   //Find imaginary part of index of refraction of revetment
  void     FindMu(const TString&);       //Find fit for magnetic permeability mu of revetment.
  void     FindRindex(const TString&);   //Find real part of index of refraction of revetment
  void     FindSliceMat(const TString&); //Find optic material of slice.
  void     Init(Double_t);               //basic initialization
  void     MoreAbsorption(Double_t);     //Defines supplementary absorption, if any.
  void     PrintProblem() const;         //Header for signaling a problem
  void     PrintTitle() const;           //prints the name and the title of the material
  void     ProblemHeader() const;        //header before reporting a problem

public:

  //Default constructor
  TRevetment() { Init(90.0); }
  //constructor when diffusion variable and index of refraction variable and magnetic permeability variable
  TRevetment(const char*, const char*, const char*, const char*,   const char*,   const char*,   const char*,      Double_t=0.0,  Double_t=90.0);
  //           name     title    nameMat  nameDiffus nameRindex nameIindex nameMu        supplAbs       ThetaMax
  //constructor when diffusion variable and index of refraction variable and magnetic permeability fixed
  TRevetment(const char*, const char*, const char*, const char*,   const char*,   const char*,   Double_t=1.0, Double_t=0.0,  Double_t=90.0);
  //           name     title    nameMat  nameDiffus nameRindex nameIindex Mu            supplAbs       ThetaMax
  //constructor when diffusion variable and index of refraction fixed    and magnetic permeability variable
  TRevetment(const char*, const char*, const char*, const char*,   Double_t,  Double_t,  const char*,      Double_t=0.0,  Double_t=90.0);
  //           name     title    nameMat  nameDiffus Rindex     Iindex     nameMu        supplAbs       ThetaMax
  //constructor when diffusion fixed    and index of refraction variable and magnetic permeability variable
  TRevetment(const char*, const char*, const char*, Double_t,  const char*,   const char*,   const char*,      Double_t=0.0,  Double_t=90.0);
  //           name     title    nameMat  diffus     nameRindex nameIindex nameMu        supplAbs       ThetaMax
  //constructor when diffusion variable and index of refraction fixed    and magnetic permeability fixed
  TRevetment(const char*, const char*, const char*, const char*,   Double_t,  Double_t,  Double_t=1.0, Double_t=0.0,  Double_t=90.0);
  //           name     title    nameMat  nameDiffus Rindex     Iindex     Mu            supplAbs       ThetaMax
  //constructor when diffusion fixed    and index of refraction variable and magnetic permeability fixed
  TRevetment(const char*, const char*, const char*, Double_t,  const char*,   const char*,   Double_t=1.0, Double_t=0.0,  Double_t=90.0);
  //           name     title    nameMat  diffus     nameRindex nameIindex Mu            supplAbs       ThetaMax
  //constructor when diffusion fixed    and index of refraction fixed    and magnetic permeability variable
  TRevetment(const char*, const char*, const char*, Double_t,  Double_t,  Double_t,  const char*,      Double_t=0.0,  Double_t=90.0);
  //           name     title    nameMat  diffus     Rindex     Iindex     nameMu        supplAbs       ThetaMax
  //constructor when diffusion fixed    and index of refraction fixed    and magnetic permeability fixed
  TRevetment(const char*, const char*, const char *nameMat="none", Double_t=0.0,  Double_t=2.0,  Double_t=6.0,  Double_t=1.0, Double_t=1.0,  Double_t=90.0);
  //           name     title    nameMat                 diffus         Rindex         Iindex         Mu            supplAbs       ThetaMax
  virtual        ~TRevetment();
  Bool_t          CheckIt();
  Double_t        Diffus() const         { return fDiffus;      }
  TSComplex       GetIndex() const       { TSComplex n(fRindex,fIindex); return n; }
  Bool_t          GetInUse() const       { return fInUse;       }
  Double_t        GetMu() const          { return fMu;          }
  Double_t        GetSinThetaMax() const { return fSinThetaMax; }
  TOpticMaterial *GetSlice() const       { return fSlice;       }
  Double_t        GetSupplAbs() const    { return fSupplAbs;    }
  Bool_t          IsSlice() const        { return fIsSlice;     }
  Short_t         IsSupplAbs() const     { return fAbsorbing;   }
  void            NewWavelength(Double_t);
  void            Print() const;
  void            SetInUse()             { fInUse = kTRUE;      }
  void            SetNotUsed()           { fInUse = kFALSE;     }
  ClassDef(TRevetment,1)  //Revetment covering a face of a shape, with optical properties.
};
#endif
