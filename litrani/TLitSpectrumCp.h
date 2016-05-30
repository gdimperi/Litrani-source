// @(#)root/html:$Id: TLitSpectrumCp.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LITR_TLitSpectrumCp
#define LITR_TLitSpectrumCp
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLitSpectrumCp  One gaussian component of the emission spectrum      //
//                 TLitSpectrum of an optical material                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"

class TLitSpectrumCp : public TObject {

protected:

  Double_t  fGaussAmpNN;   //Un-normalized amplitude of this gaussian component as issued by fit
  Double_t  fGaussAmp;     //Normalized amplitude of this gaussian component [Sum of all fGaussAmp of all components is 1]
  Double_t  fGaussMean;    //Mean value of this gaussian component
  Double_t  fGaussSigma;   //Sigma value of this gaussian component
  Bool_t    fNormalized;   //True if time components are normalized [fTimeWeight and fArea defined]
  Int_t     fNbTimes;      //Number of time components associated with this gaussian component
  Double_t *fTimeWeightNN; //[fNbTimes] not normalized weight of this time component
  Double_t *fTimeWeight;   //[fNbTimes] normalized weight of this time component
  Double_t *fDecayTime;    //[fNbTimes] Decay time of this component in nanosecond
  Double_t *fRiseTime;     //[fNbTimes] Rise time of this component in nanosecond, if any. If no rise time == -1.0
  Double_t *fArea;         //[fNbTimes] Area from 0 to 1 to help to select randomly which time component

  void      Init();

public:

  TLitSpectrumCp()                          { Init();                  }
  TLitSpectrumCp(Double_t,Double_t,Double_t);
  TLitSpectrumCp(Double_t,Double_t,Double_t,Double_t,Double_t= -1.0,Double_t = 1.0);
  TLitSpectrumCp(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,
                 Double_t,Double_t,Double_t);
  TLitSpectrumCp(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,
                 Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  TLitSpectrumCp(Double_t,Double_t,Double_t,Int_t,Double_t*,Double_t*,Double_t*);
  TLitSpectrumCp(const TLitSpectrumCp&);
  virtual ~TLitSpectrumCp();
  void     AddTimeComponent(Double_t,Double_t,Double_t);
  void     AddToGaussAmpNN(Double_t);
  void     Clear();
  Int_t    Compare(const TObject*) const;
  Double_t GenDecayTime();
  Double_t GenWaveLength(Bool_t&) const;
  Double_t GetArea(Short_t k) const         { return fArea[k];         }
  Double_t GetDecayTime(Short_t k) const    { return fDecayTime[k];    }
  Double_t GetGaussAmp() const              { return fGaussAmp;        }
  Double_t GetGaussAmpNN() const            { return fGaussAmpNN;      }
  Double_t GetGaussMean() const             { return fGaussMean;       }
  Double_t GetGaussSigma() const            { return fGaussSigma;      }
  Int_t    GetNbTimes() const               { return fNbTimes;         }
  Bool_t   GetNormalized() const            { return fNormalized;      }
  Double_t GetRiseTime(Short_t k) const     { return fRiseTime[k];     }
  Double_t GetTimeWeight(Short_t k) const   { return fTimeWeight[k];   }
  Double_t GetTimeWeightNN(Short_t k) const { return fTimeWeightNN[k]; }
  Bool_t   IsEqual(const TObject*) const;
  Bool_t   IsSortable() const               { return kTRUE;            }
  void     Normalize();
  void     Print() const;
  void     SetGaussAmp(Double_t A)          { fGaussAmp = A;           }
  void     SetGaussAmpNN(Double_t A)        { fGaussAmpNN = A;         }
  ClassDef(TLitSpectrumCp,1)  //One gaussian component of the emission spectrum TLitSpectrum of an optical material
};
#endif
