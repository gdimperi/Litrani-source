// @(#)root/html:$Id: TLitSpectrumCp.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TCleanOut.h"
#include "TLitPhys.h"
#include "TLitSpectrumCp.h"

ClassImp(TLitSpectrumCp)
//______________________________________________________________________________
//
//    LitSpectrumCp
//
//  One gaussian component of the emission spectrum TLitSpectrum of an optical
// material. This gaussian component may have ANY number of decay times and
// rise times.
//
//  TLitSpectrum and TLitSpectrumCp are used both in Litrani and SLitrani.
//
//  The gaussian which has been fitted is
//
//            (A/(Sqrt(2*pi)*s))*exp(-(x-m)^2/(2*s^2))
//
//  where A == fGaussAmpNN, m == fGaussMean, s == fGaussSigma. The sum of all
// A [fGaussAmpNN] of all components is equal to the surface under the curve of
// the fitted distribution. Since in [S]Litrani we have decided that the emission
// spectra entered in SplineFit.rdb will be normalized to 10^6 photons, the surface
// under the curve will be 10^6 and the sum of all A == fGaussAmpNN of all components
// will be ~10^6 [~because the spectrum in SplineFit.rdb does not go from -infinity
// to + infinity]. The reason to select a normalization of 10^6 and not 1 for the
// spectra in SplineFit.rdb is that I have found that TSpectrum of ROOT is working
// better with histograms of bin contents much larger than 1. And TSpectrum will
// be used by [S]Litrani for the construction of spectra of emission, see the class
// description of class TLitSpectrum.
//  For the generation of photons, we need that the sum of all A == fGaussAmp be 1,
// and components are said to be normalized when the sum of all A == fGaussAmp is 1.
// It is necessary to keep both fGaussAmpNN and fGaussAmp, since if a new component
// is added [or removed] afterwards, we want to remain able to normalize again.
//  If fGaussAmp is found to be negativ [-1.0], it means that the distribution
// has not yet been normalized. The normalization we speak of here is the normalization
// of all gaussian peaks inside the collection TLitSpectrum::fComponents, not the
// normalization referred by TLitSpectrumCp::fNormalized, which is the normalization
// of all time components inside one gaussian peak.
//
//  Time components
//
//
//  [S]Litrani allows 2 kind of decay for a time component:
//
//  (1) - Standard decay with 1 decay time td [ fDecayTime[k] ]. The normalized decay
// distribution is then:
//
//           f(t) == (1/td)*exp(-(t/td))
//
//  (2) - Decay with rise time tr and decay time td, with distribution:
//
//           f(t) == ((tr+td)/(td*td))*(1-exp(-(t/tr)))*exp(-(t/td))
//
//  Both distributions are normalized such that the probability of a decay at any
// time is 1. Integral over f(t) from 0 to infinity is 1.
//  fDecayTime[k] is td, fRiseTime[k] is tr. If fRiseTime[k] is negativ [-1.0],
// it means that there is no rise time and that distribution (1) must be used.
//  If x is generated flat between 0 and 1, and if we define t as:
//
//           t = -td*ln(1-x)
//
// then it is easy to show that t will be generated according to distribution (1)
//
//  To generate t according to distribution (2), it is necessary to solve with
// respect to t the transcendental equation:
//
//           (1/td)*exp(-(t/td))*(tr*exp(-t/tr) - tr - td) = x - 1
//
// which is done by method GenDecay().
//
//  It is perfectly allowed for a gaussian component to have many decay times (td,tr).
// The number of possible pair of decay times (td,tr) is fNbTimes. The unnormalized
// weight for each (td,tr) given by the user are in fTimeWeightNN[k]. The user has
// not to bother that the sum of the weights he gives be 1. A call to the method
// Normalize(), after all time components have been entered, will normalize the sum
// of the weights to 1, in fTimeWeight. The array fArea allows a quick random selection
// of the time component pondered by the weights in fTimeWeight.
//
TLitSpectrumCp::TLitSpectrumCp(Double_t m,Double_t s,Double_t A) {
  //Constructor initializing only the gaussian, not the time components
  //
  //  Arguments :
  //
  //    m  : mean value of this gaussian component as found by gaussian fit on spectrum
  //    s  : Sigma value of this gaussian component as found by gaussian fit on spectrum
  //    A  : amplitude as found by gaussian fit on spectrum
  //
  Init();
  fGaussMean  = m;
  fGaussSigma = s;
  fGaussAmpNN = A;
}
TLitSpectrumCp::TLitSpectrumCp(Double_t m,Double_t s,Double_t A,
                               Double_t td,Double_t tr,Double_t w) {
                                 //  Constructor initializing the gaussian and 1 component of time decay component
                                 //
                                 //  Arguments :
                                 //
                                 //    m  : mean value of this gaussian component as found by gaussian fit on spectrum
                                 //    s  : Sigma value of this gaussian component as found by gaussian fit on spectrum
                                 //    A  : amplitude as found by gaussian fit on spectrum
                                 //    td : decay time for first time component (td,tr)
                                 //    tr : rise time  for first time component (td,tr). If negativ [-1.0] no rise time
                                 //    w  : weight of the time component (td,tr). Irrelevant if we remain with only 1
                                 //         time component, but relevant if other components are added afterwards by
                                 //         calls to method AddTimeComponent().
                                 const Double_t m1 = -1.0;
                                 Init();
                                 fGaussMean       = m;
                                 fGaussSigma      = s;
                                 fGaussAmpNN      = A;
                                 fNbTimes         = 1;
                                 fTimeWeightNN    = new Double_t [fNbTimes];
                                 fTimeWeight      = new Double_t [fNbTimes];
                                 fDecayTime       = new Double_t [fNbTimes];
                                 fRiseTime        = new Double_t [fNbTimes];
                                 fArea            = new Double_t [fNbTimes];
                                 fTimeWeightNN[0] = w;
                                 fTimeWeight[0]   = m1;
                                 fDecayTime[0]    = td;
                                 fRiseTime[0]     = tr;
                                 fArea[0]         = m1;
}
TLitSpectrumCp::TLitSpectrumCp(Double_t m,Double_t s,Double_t A,
                               Double_t td1,Double_t tr1,Double_t w1,Double_t td2,Double_t tr2,Double_t w2) {
                                 //  Constructor initializing the gaussian and 2 components of time decay component
                                 //
                                 //  Arguments :
                                 //
                                 //    m   : mean value of this gaussian component as found by gaussian fit on spectrum
                                 //    s   : Sigma value of this gaussian component as found by gaussian fit on spectrum
                                 //    A   : amplitude as found by gaussian fit on spectrum
                                 //    td1 : decay time for 1st time component (td1,tr1)
                                 //    tr1 : rise time  for 1st time component (td1,tr1). If negativ [-1.0] no rise time
                                 //    w1  : weight  of the 1st time component (td1,tr1)
                                 //    td2 : decay time for 2nd time component (td2,tr2)
                                 //    tr2 : rise time  for 2nd time component (td2,tr2). If negativ [-1.0] no rise time
                                 //    w2  : weight  of the 2nd time component (td2,tr2)
                                 //
                                 const Double_t m1 = -1.0;
                                 Init();
                                 fGaussMean       = m;
                                 fGaussSigma      = s;
                                 fGaussAmpNN      = A;
                                 fNbTimes         = 2;
                                 fTimeWeightNN    = new Double_t [fNbTimes];
                                 fTimeWeight      = new Double_t [fNbTimes];
                                 fDecayTime       = new Double_t [fNbTimes];
                                 fRiseTime        = new Double_t [fNbTimes];
                                 fArea            = new Double_t [fNbTimes];
                                 fTimeWeightNN[0] = w1;
                                 fTimeWeight[0]   = m1;
                                 fDecayTime[0]    = td1;
                                 fRiseTime[0]     = tr1;
                                 fArea[0]         = m1;
                                 fTimeWeightNN[1] = w2;
                                 fTimeWeight[1]   = m1;
                                 fDecayTime[1]    = td2;
                                 fRiseTime[1]     = tr2;
                                 fArea[1]         = m1;
}
TLitSpectrumCp::TLitSpectrumCp(Double_t m,Double_t s,Double_t A,
                               Double_t td1,Double_t tr1,Double_t w1,Double_t td2,Double_t tr2,Double_t w2,
                               Double_t td3,Double_t tr3,Double_t w3) {
                                 //  Constructor initializing the gaussian and 3 components of time decay component
                                 //
                                 //  Arguments :
                                 //
                                 //    m   : mean value of this gaussian component as found by gaussian fit on spectrum
                                 //    s   : Sigma value of this gaussian component as found by gaussian fit on spectrum
                                 //    A   : amplitude as found by gaussian fit on spectrum
                                 //    td1 : decay time for 1st time component (td1,tr1)
                                 //    tr1 : rise time  for 1st time component (td1,tr1). If negativ [-1.0] no rise time
                                 //    w1  : weight  of the 1st time component (td1,tr1)
                                 //    td2 : decay time for 2nd time component (td2,tr2)
                                 //    tr2 : rise time  for 2nd time component (td2,tr2). If negativ [-1.0] no rise time
                                 //    w2  : weight  of the 2nd time component (td2,tr2)
                                 //    td3 : decay time for 3rd time component (td3,tr3)
                                 //    tr3 : rise time  for 3rd time component (td3,tr3). If negativ [-1.0] no rise time
                                 //    w3  : weight  of the 3rd time component (td3,tr3)
                                 //
                                 const Double_t m1 = -1.0;
                                 Init();
                                 fGaussMean       = m;
                                 fGaussSigma      = s;
                                 fGaussAmpNN      = A;
                                 fNbTimes         = 3;
                                 fTimeWeightNN    = new Double_t [fNbTimes];
                                 fTimeWeight      = new Double_t [fNbTimes];
                                 fDecayTime       = new Double_t [fNbTimes];
                                 fRiseTime        = new Double_t [fNbTimes];
                                 fArea            = new Double_t [fNbTimes];
                                 fTimeWeightNN[0] = w1;
                                 fTimeWeight[0]   = m1;
                                 fDecayTime[0]    = td1;
                                 fRiseTime[0]     = tr1;
                                 fArea[0]         = m1;
                                 fTimeWeightNN[1] = w2;
                                 fTimeWeight[1]   = m1;
                                 fDecayTime[1]    = td2;
                                 fRiseTime[1]     = tr2;
                                 fArea[1]         = m1;
                                 fTimeWeightNN[2] = w2;
                                 fTimeWeight[2]   = m1;
                                 fDecayTime[2]    = td2;
                                 fRiseTime[2]     = tr2;
                                 fArea[2]         = m1;
}
TLitSpectrumCp::TLitSpectrumCp(Double_t m,Double_t s,Double_t A,Int_t N,
                               Double_t *td,Double_t *tr,Double_t *w) {
                                 //  Constructor initializing the gaussian and N components of time decay component
                                 //
                                 //  Arguments :
                                 //
                                 //    m   : mean value of this gaussian component as found by gaussian fit on spectrum
                                 //    s   : sigma value of this gaussian component as found by gaussian fit on spectrum
                                 //    A   : amplitude as found by gaussian fit on spectrum
                                 //    N   : number of time pair (tdi,tri)
                                 //    *td : array of decay times
                                 //    *tr : array of rise times. If tri negativ [-1.0] no rise time for component i
                                 //    *w  : array of weights  of the N components
                                 //
                                 const Double_t m1 = -1.0;
                                 Short_t k;
                                 Init();
                                 fGaussAmpNN = A;
                                 fGaussMean  = m;
                                 fGaussSigma = s;
                                 fNbTimes    = TMath::Abs(N);
                                 if (N>0) {
                                   fTimeWeightNN = new Double_t [fNbTimes];
                                   fTimeWeight   = new Double_t [fNbTimes];
                                   fDecayTime    = new Double_t [fNbTimes];
                                   fRiseTime     = new Double_t [fNbTimes];
                                   fArea         = new Double_t [fNbTimes];
                                   for (k=0;k<fNbTimes;k++) {
                                     fTimeWeightNN[k] = w[k];
                                     fTimeWeight[k]   = m1;
                                     fDecayTime[k]    = td[k];
                                     fRiseTime[k]     = tr[k];
                                     fArea[k]         = m1;
                                   }
                                 }
}
TLitSpectrumCp::TLitSpectrumCp(const TLitSpectrumCp &cp) {
  // Copy constructor
  Short_t k;
  fGaussAmpNN = cp.fGaussAmpNN;
  fGaussAmp   = cp.fGaussAmp;
  fGaussMean  = cp.fGaussMean;
  fGaussSigma = cp.fGaussSigma;
  fNormalized = cp.fNormalized;
  fNbTimes    = cp.fNbTimes;
  fTimeWeightNN = new Double_t [fNbTimes];
  fTimeWeight   = new Double_t [fNbTimes];
  fDecayTime    = new Double_t [fNbTimes];
  fRiseTime     = new Double_t [fNbTimes];
  fArea         = new Double_t [fNbTimes];
  for (k=0;k<fNbTimes;k++) {
    fTimeWeightNN[k] = cp.fTimeWeightNN[k];
    fTimeWeight[k]   = cp.fTimeWeight[k];
    fDecayTime[k]    = cp.fDecayTime[k];
    fRiseTime[k]     = cp.fRiseTime[k];
    fArea[k]         = cp.fArea[k];
  }
}
TLitSpectrumCp::~TLitSpectrumCp() {
  //destructor
  Clear();
}
void TLitSpectrumCp::AddTimeComponent(Double_t td,Double_t tr,Double_t w) {
  //Add one time component more
  const Double_t m1 = -1.0;
  Short_t k;
  Double_t *oldwNN;
  Double_t *oldtd;
  Double_t *oldtr;
  fNormalized = kFALSE;
  fNbTimes   += 1;
  oldwNN      = fTimeWeightNN;
  oldtd       = fDecayTime;
  oldtr       = fRiseTime;
  if (fTimeWeight) {
    delete [] fTimeWeight;
    fTimeWeight = 0;
  }
  if (fArea) {
    delete [] fArea;
    fArea = 0;
  }
  fTimeWeightNN = new Double_t [fNbTimes];
  fTimeWeight   = new Double_t [fNbTimes];
  fDecayTime    = new Double_t [fNbTimes];
  fRiseTime     = new Double_t [fNbTimes];
  fArea         = new Double_t [fNbTimes];
  for (k=0;k<fNbTimes-1;k++) {
    fTimeWeightNN[k] = oldwNN[k];
    fTimeWeight[k]   = m1;
    fDecayTime[k]    = oldtd[k];
    fRiseTime[k]     = oldtr[k];
    fArea[k]         = m1;
  }
  k = fNbTimes-1;
  fTimeWeightNN[k] = w;
  fTimeWeight[k]   = m1;
  fDecayTime[k]    = td;
  fRiseTime[k]     = tr;
  fArea[k]         = m1;
  delete [] oldwNN;
  delete [] oldtd;
  delete [] oldtr;
}
void TLitSpectrumCp::AddToGaussAmpNN(Double_t A) {
  // Add the weight w to the unnormalized gaussian amplitude
  fGaussAmpNN += A;
  fGaussAmp    = -1.0;
}
void TLitSpectrumCp::Clear() {
  // Delete all the time components. Do not touch at the gaussian
  if (fTimeWeightNN) {
    delete fTimeWeightNN;
    fTimeWeightNN = 0;
  }
  if (fTimeWeight) {
    delete fTimeWeight;
    fTimeWeight = 0;
  }
  if (fDecayTime) {
    delete fDecayTime;
    fDecayTime = 0;
  }
  if (fRiseTime) {
    delete fRiseTime;
    fRiseTime = 0;
  }
  if (fArea) {
    delete fArea;
    fArea = 0;
  }
  fNbTimes = 0;
}
Int_t TLitSpectrumCp::Compare(const TObject *obj) const {
  // We order components according to the value of their mean wavelength of emission
  TLitSpectrumCp *p = (TLitSpectrumCp*)obj;
  if (fGaussMean < p->fGaussMean) return -1;
  else {
    if (fGaussMean > p->fGaussMean) return 1;
    else return 0;
  }
}
Double_t TLitSpectrumCp::GenWaveLength(Bool_t &ok) const {
  // Generates wavelength in nm. Generated wavelength must be greater than
  //TLitPhys::Get()->fMinWaveL and smaller than TLitPhys::Get()->fMaxWavel
  Double_t WaveL;
  WaveL = gRandom3->Gaus(fGaussMean,fGaussSigma);
  ok = ((WaveL>TLitPhys::Get()->MinWaveL()) && (WaveL<TLitPhys::Get()->MaxWaveL()));
  return WaveL;
}
Double_t TLitSpectrumCp::GenDecayTime() {
  // Generates time of decay for this component in nanosecond. If more than 1
  //time component, first select randomly, according to weights in fTimeWeight,
  //which time component is activ
  const Short_t  Nmax = 1000;
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Short_t k,TimeComponent = 0;
  Double_t x,t,td,tr;
  if (!fNormalized) Normalize();
  if (fNbTimes>1) {
    Bool_t found = kFALSE;
    x = gRandom3->Rndm();
    k = 0;
    while ((!found) && (k<fNbTimes)) {
      if (x<=fArea[k]) {
        found = kTRUE;
        TimeComponent = k;
      }
      k++;
    }//end while ((!found) && (k<fNbTimes))
    if (!found) {
      Error("TLitSpectrumCp::GenDecay","Time component not found");
      TimeComponent = fNbTimes - 1;
    }
  }//end if (fNbTimes>1)
  x = gRandom3->Rndm();
  td = fDecayTime[TimeComponent];
  tr = fRiseTime[TimeComponent];
  t = -td*TMath::Log(un - x);
  if (tr>zero) {
    const Double_t z05  = 0.5;
    const Double_t eps  = 1.0e-8;
    const Double_t tabs = 1.0e+2;
    Double_t a,b,t1,t2,x0,x1,x2,tw,test;
    Double_t tmax;
    tmax = tabs*(td+tr);
    a  = un/td;
    b  = tr + td;
    t2 = tmax;
    t1 = t;
    if (t1>t2) return tmax;
    x1 = a*TMath::Exp(-t1/td)*(tr*TMath::Exp(-t1/tr) - b) + 1;
    x2 = a*TMath::Exp(-t2/td)*(tr*TMath::Exp(-t2/tr) - b) + 1;
    k = 0;
    do {
      tw = t2 - t1;
      t  = t1 + z05*tw;
      x0 = a*TMath::Exp(-t/td)*(tr*TMath::Exp(-t/tr) - b) + 1;
      if (x>x0) {
        t1 = t;
        x1 = a*TMath::Exp(-t1/td)*(tr*TMath::Exp(-t1/tr) - b) + 1;
      }
      else {
        t2 = t;
        x2 = a*TMath::Exp(-t2/td)*(tr*TMath::Exp(-t2/tr) - b) + 1;
      }
      test = TMath::Abs(x2-x1);
      k++;
    } while ((test > eps) && (k<Nmax));
    if (k>=Nmax) Error("TLitSpectrumCp::GenDecay","Infinite loop");
  }
  return t;
}
void TLitSpectrumCp::Init() {
  //Default values for variables
  const Double_t m1 = -1.0;
  fGaussAmpNN  = m1;
  fGaussAmp     = m1;
  fGaussMean    = m1;
  fGaussSigma   = m1;
  fNormalized   = kFALSE;
  fNbTimes      = 0;
  fTimeWeightNN = 0;
  fTimeWeight   = 0;
  fDecayTime    = 0;
  fRiseTime     = 0;
  fArea         = 0;
}
Bool_t TLitSpectrumCp::IsEqual(const TObject* obj) const {
  // 2 components are considered as equal if the 2 fGaussMean are equal
  TLitSpectrumCp *p = (TLitSpectrumCp*)obj;
  return fGaussMean == p->fGaussMean;
}
void TLitSpectrumCp::Print() const {
  // Print all class variables
  const char *met = "Print";
  Short_t k;
  gCleanOut->MMR(info,met,"Un-normalized amplitude","fGaussAmpNN",fGaussAmpNN,ClassName());
  gCleanOut->MMR(info,met,"Normalized amplitude","fGaussAmp",fGaussAmp,ClassName());
  gCleanOut->MMR(info,met,"Mean value","fGaussMean",fGaussMean,ClassName());
  gCleanOut->MMR(info,met,"MSigma value","fGaussSigma",fGaussSigma,ClassName());
  gCleanOut->MMB(info,met,"Time components normalized ?","fNormalized",fNormalized,ClassName());
  gCleanOut->MMI(info,met,"Nb. of time components","fNbTimes",fNbTimes,ClassName());
  if (fNbTimes>0) {
    for (k=0;k<fNbTimes;k++) {
      gCleanOut->M(info,"");
      gCleanOut->MMI(info,met,"Time component","k+1",k+1,ClassName());
      gCleanOut->MMR(info,met,"Weight UnNorm","fTimeWeightNN",fTimeWeightNN[k],ClassName());
      gCleanOut->MMR(info,met,"Weight","fTimeWeight",fTimeWeight[k],ClassName());
      gCleanOut->MMR(info,met,"Decay time","fDecayTime",fDecayTime[k],ClassName());
      gCleanOut->MMR(info,met,"Rise time","fRiseTime",fRiseTime[k],ClassName());
    }
  }
}
void TLitSpectrumCp::Normalize() {
  // This method has to be called when all time components have been entered
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Short_t k;
  Double_t S = zero;
  for (k=0;k<fNbTimes;k++) S += fTimeWeightNN[k];
  for (k=0;k<fNbTimes;k++) fTimeWeight[k] = fTimeWeightNN[k]/S;
  S = zero;
  for (k=0;k<fNbTimes;k++) {
    S += fTimeWeight[k];
    fArea[k] = S;
  }
  fArea[fNbTimes-1] = un;
  fNormalized = kTRUE;
}
