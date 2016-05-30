// @(#)root/html:$Id: TLitPhys.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LIT_TLitPhys
#define LIT_TLitPhys

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TLitPhys    All physical constants and global settings of SLitrani  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TRandom3.h"

enum KindOfDist {on4pi,flat,sinuscosinus,provided};
enum EDepProcess {kPhotoElectric,kComptonScattering,kCascade,kBeam};
enum OpticalProcess {kNoOpt=0,kAbsorption,kIsotropicScattering,kRayleighScattering,
kWLS,kSurfaceProcess};

class TGeoMedium;

class TLitPhys {

private:

  //Physical constants

  Double_t fC;           //speed of light in cm/picoseconds
  Double_t fCcmps;       //speed of light in cm/seconds
  Double_t fCmks;        //speed of light in m/s
  Double_t fE;           //electron charge in esu
  Double_t fMe;          //Mass of electron in Mev/c2
  Double_t fMeg;         //Mass of electron in gram
  Double_t fR02;         //= 1.0e+24*((fE*fE)/(fMeg*fCcmps*fCcmps))^2
  Double_t fHbar;        //Planck hbar in Joule*s
  Double_t fAlpha;       //fine structure constant
  Double_t fEps0;        //epsilon0 "influence constant" [ A s ] / [ V M ]
  Double_t fAvogadro;    //Avogadro's number in molecules/kmole
  Double_t fAvoDivE26;   //Avogadro's number in molecules/kmole divided by 1.0e+26
  Double_t fVphot;       //arbitrary volume [m3 ] in which we choose that the energy of the photon is hbar*w. We take 1mm3

  //Global Settings

  Double_t fTooLate;     //Time (ps) after which photon is considered too late
  Double_t fThreshe;     //minimal number of e- emitted by APD to accept photon
  Int_t    fMaxAnomal;   //maximal number of abnormal photons accepted
  Double_t fMinWaveL;    //Minimum possible value for wavelength
  Double_t fMaxWaveL;    //Maximum possible value for wavelength
  Double_t fBiggestStep; //Biggest possible step in tracking. Set it to the biggest distance in your geometry.

  static TLitPhys *fgLitPhys;

  TLitPhys();            //Private constructor: TLitPhys is a singleton!
  virtual ~TLitPhys() {;}//Private destructor:  TLitPhys is a singleton

public:

  Double_t Alpha() const              { return fAlpha;          }
  Int_t    Anomalies() const          { return fMaxAnomal;      }
  Double_t AvoDivE26() const          { return fAvoDivE26;      }
  Double_t Avogadro() const           { return fAvogadro;       }
  Double_t BiggestStep() const        { return fBiggestStep;    }
  Double_t C() const                  { return fC;              }
  Double_t Ccmps() const              { return fCcmps;          }
  Double_t Cmks() const               { return fCmks;           }
  Double_t E() const                  { return fE;              }
  Double_t Eps0() const               { return fEps0;           }
// GetA() and GetZ() only in use in SLitrani !
  Double_t GetA(TGeoMedium*) const;
  Double_t GetZ(TGeoMedium*) const;
  Double_t Hbar() const               { return fHbar;           }
  Double_t MaxWaveL() const           { return fMaxWaveL;       }
  Double_t Me() const                 { return fMe;             }
  Double_t Meg() const                { return fMeg;            }
  Double_t MinWaveL() const           { return fMinWaveL;       }
  Double_t R02() const                { return fR02;            }
  void     SetAnomal(Int_t k)         { fMaxAnomal = k;         }
  void     SetBiggestStep(Double_t b) { fBiggestStep = b;       }
  void     SetLate(Double_t late)     { fTooLate = 1000.0*late; }
  void     SetMaxWaveL(Double_t m)    { fMaxWaveL = m;          }
  void     SetMinMaxWL(Double_t,Double_t);
  void     SetMinWaveL(Double_t m)    { fMinWaveL = m;          }
  void     SetThreshe(Double_t t)     { fThreshe = t;           }
  Double_t Threshe() const            { return fThreshe;        }
  Double_t TooLate() const            { return fTooLate;        }
  Double_t Vphot() const              { return fVphot;          }

  static   Double_t  ASin(Double_t);
  static   Double_t  ACos(Double_t);
  static   void      Book();
  static   TLitPhys *Get();
  static   void      Kill();

  ClassDef(TLitPhys,0) //All physical constants used by SLitrani.
};
inline TLitPhys* TLitPhys::Get() {
  // Only way to access fgLitPhys! Prevents to create more then one instance of
  //TLitPhys. This, combined with the fact that constructor and destructor are
  //private, makes of TLitPhys a singleton
  if (!fgLitPhys) fgLitPhys = new TLitPhys();
  return fgLitPhys;
}
R__EXTERN TRandom3 *gRandom3;

#endif
