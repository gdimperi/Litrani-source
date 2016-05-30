// @(#)root/html:$Id: TOpticMaterial.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>, D. Wahl <mailto:david.n.wahl@gmail.com>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TOpticMaterial
#define LITR_TOpticMaterial
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOpticMaterial   a material, with its optical characteristics        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TMaterial.h"
#include "TSplineFit.h"
#include "TLitSpectrum.h"
#include "TSellmeier.h"

using namespace std;

enum MethodUsed {undefined,constant,byfit,sellmeier};
enum dEdxMethod {nodEdx,WithoutLandau,WithLandau};

class TOpticMaterial : public TMaterial {

protected:

  static Double_t fMinimalGap;      //minimal difference between epsa,epsb,epsc
  Bool_t          fInUse;           //if in use, an optical material cannot be deleted
  Bool_t          fChecked;         //material is correctly defined (method CheckIt has been called)
  Bool_t          fSensible;        //true if the material is able to detect photons

  //Absorption lengths

  Short_t         fAniAbsL;         //0 ==> abs. L. isotropic   1 ==> La != Lb  = Lc   2 ==> La  = Lb != Lc   3 ==> la != Lb != Lc
  Double_t        fFacAbsL;         //Factor by which all absorption length are multiplied after having been obtained from fit (default=1.0)
  Double_t        fAbsLa;           //value of absorption length associated with fEpsa or isotropic absorption length when fAniAbsL = 0
  Double_t        fPhimua;          //imaginary part of fEpsa multiplied by fMu
  TString         fNameAbsLa;       //name of the fit giving fAbsLa as a function of wavelength
  TSplineFit     *fFitAbsLa;        //pointer to the fit for fAbsLa
  Double_t        fAbsLb;           //value of absorption length associated with fEpsb
  Double_t        fPhimub;          //imaginary part of fEpsb multiplied by fMu
  TString         fNameAbsLb;       //name of the fit giving fAbsLb as a function of wavelength
  TSplineFit     *fFitAbsLb;        //pointer to the fit for fAbsLb
  Double_t        fAbsLc;           //value of absorption length associated with fEpsc
  Double_t        fPhimuc;          //imaginary part of fEpsc multiplied by fMu
  TString         fNameAbsLc;       //name of the fit giving fAbsLc as a function of wavelength
  TSplineFit     *fFitAbsLc;        //pointer to the fit for fAbsLc
  Bool_t          fHasAbsorption;   //True if material has absorption
  Bool_t          fHasWLS;          //True if material is wavelength shifting

  //Diffusion lengths

  Bool_t          fHasDiffusion;    //True if material has istropic scattering defined
  Bool_t          fHasRayleigh;     //True if material has Rayleigh scattering defined
  Double_t        fFacDiffus;       //if >0, factor which gives Diffusion length when multiplied by absorption length
  Double_t        fDiffusLength;    //Actual value of diffusion length
  TString         fNameDiffusL;     //name of the fit giving fDiffusLength as a function of wavelength
  TSplineFit     *fFitDiffusL;      //pointer to the fit giving fDiffusLength as a function of wavelength
  Double_t        fRayleighLength;  //Actual value of Rayleigh scattering length
  TString         fNameRayleighL;   //name of the fit giving fRayleighLength as a function of wavelength
  TSplineFit     *fFitRayleighL;    //pointer to the fit giving fRayleighLength as a function of wavelength

  //Index of refraction

  Short_t         fAnisotropic;     //0 isotrop 1 neg bir 2 pos bir 3 birefringent
  //-1 ==> optical property not yet defined !
  // 0 ==> material is optically isotropic
  // 1 ==> material is "Uniaxial negative birefringent"
  //        ( epsa < epsb = epsc )
  // 2 ==> material is "Uniaxial positiv birefringent"
  //        ( epsa = epsb < epsc )
  // 3 ==> material is "biaxial birefringent" or "totally anisotropic"
  //        ( epsa < epsb < epsc )
  Double_t        fIndex;           //Index of refraction
  MethodUsed      fIndexM;          //method used for calculating the index of refraction
  TSellmeier     *fIndexS;          //pointer to Sellmeier formula for index
  TString         fNameIndex;       //name of the fit giving the index of refraction as a function of wavelength
  TSplineFit     *fFitIndex;        //pointer to the fit for index of refraction

  //Magnetic permeability

  Double_t        fMu;              //Magnetic permeability mu
  TString         fNameMu;          //name of the fit giving the magnetic permeability mu as a function of wavelength
  TSplineFit     *fFitMu;           //pointer to the fit for magnetic permeability mu
  Bool_t          fMuFound;         //Fit for Mu or value for mu found;

  //Epsa : 1st diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc

  Double_t        fEpsa;            //Epsa : 1st diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc
  MethodUsed      fEpsaM;           //method used for calculating fEpsa
  TSellmeier     *fEpsaS;           //pointer to Sellmeier formula for Epsa
  TString         fNameEpsa;        //name of the fit giving the 1st diagonal element Epsa as a function of wavelength
  TSplineFit     *fFitEpsa;         //pointer to the fit for Epsa

  //Epsb : 2nd diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc

  Double_t        fEpsb;            //Epsb : 2nd diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc
  MethodUsed      fEpsbM;           //method used for calculating fEpsb
  TSellmeier     *fEpsbS;           //pointer to Sellmeier formula for Epsb
  TString         fNameEpsb;        //name of the fit giving the 2nd diagonal element Epsb as a function of wavelength
  TSplineFit     *fFitEpsb;         //pointer to the fit for Epsb

  //Epsc : 3rd diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc

  Double_t        fEpsc;            //Epsc : 3rd diagonal element of dielectric tensor in the system where it is diagonal. Epsa <= Epsb <= Epsc
  MethodUsed      fEpscM;           //method used for calculating fEpsc
  TSellmeier     *fEpscS;           //pointer to Sellmeier formula for Epsc
  TString         fNameEpsc;        //name of the fit giving the 3rd diagonal element Epsc as a function of wavelength
  TSplineFit     *fFitEpsc;         //pointer to the fit for Epsc

  //dE/dx

  dEdxMethod      fdEdxUsed;        //this material is optically activ when crossed by particle
  TString         fNamedEdx;        //name of the family of fits containing all distributions
  Int_t           fNbdEdx;          //Nb. of elements in the family fDeposi, if fDeposit is a family
  TSplineFit     *fDeposit;         //pointer to fit giving deposited energy in 1 cm versus energy of particle when no Landau fluctuations
  Double_t        fPhotMev;         //Number of photons emitted per deposited Mev of energy
  Double_t        fResolutionScale; // Scaling of resolution on fPhotMev to allow for effects due to the Fano factor and intrinsic broadening of resolution due to non-homogeneity.

  //Fluorescence

  TLitSpectrum   *fLitSpectrum;     //Emission spectrum [wavelength and times] of this material
  Bool_t          fHistFluo;        //histogrammation required for fluorescence of this material

  //Electromagnetic cascades

  Double_t        fX0;              //Radiation length in cm
  Double_t        fRm;              //Moliere's radius in cm
  Double_t        fEc;              //Critical energy  in Mev

  //Non-proportionality of light yield

  TSplineFit     *fFitNPLY;         //SplineFit for the data on the non-proportionality of the light yield
  Bool_t          fHasNPLY;         //Is the material defined as being non-proportional
  TString         fNameNPLY;        //Name of the fit

  //Photo Electric effect

  TSplineFit     *fFitPE;           // SplineFit with the cross section for photo-electric absorption
  Bool_t          fPEIsInitialised; // Has the PE been initialised
  TString         fNamePE;          // Name of the PE fit

  //Wavelength shifting

  TLitSpectrum   *fWLSSpectrum;     // Wavelength shifted spectrum
  Double_t        fWLSL;            // WLS Absorption length (not wavelength dependant)
  TString         fNameWLSL;        //name of the fit giving fAbsLb as a function of wavelength
  TSplineFit     *fFitWLSL;         //pointer to the fit for fAbsLb
  Bool_t          fCanStopElectron; // Compton electron properties
  Double_t        fEP[6];           //Material properties necessary to calculate electron ionisation dE/dX

  Bool_t          AddToList() const;
  Bool_t          AddToList(const char*) const;
  void            BasicInit();
  void            BasicInit1();
  void            NewPhiMu(Double_t);
  Int_t           NPhotInteger(Double_t nphot) const;
  void            PrintProblem() const;
  void            PrintTitle() const;
  void            ProblemHeader() const;

public:

  static Double_t fCerWaveL;        //wavelength to use for n used in Cerenkov formulae

  //Default constructor
  TOpticMaterial() { BasicInit1(); }
  //constructor when abs. length variable and magnetic permeability variable
  TOpticMaterial(const char*,     const char*,     Bool_t,   const char*,  const char*,
    const char* ="", const char* ="", Float_t=1,Float_t=0,Float_t=1);
  //               Name         Title        Sensible  NameMu    NameAbsLa
  //               NameAbsLb    NameAbsLc    A         Z         Density
  //constructor when abs. length fixed    and magnetic permeability variable
  TOpticMaterial(const char*,     const char*,     Bool_t,   const char*,  Double_t,     
    Double_t=-1, Double_t=-1, Float_t=1,Float_t=0,Float_t=1);
  //               Name         Title        Sensible  NameMu    AbsLa
  //               AbsLb        AbsLc        A         Z         Density
  //constructor when abs. length variable and magnetic permeability fixed
  TOpticMaterial(const char*,     const char*,     Bool_t,   Double_t, const char*, 
    const char* ="", const char* ="", Float_t=1,Float_t=0,Float_t=1);
  //               Name         Title        Sensible  Mu        NameAbsLa
  //               NameAbsLb    NameAbsLc    A         Z         Density
  //constructor when abs. length fixed and magnetic permeability fixed
  TOpticMaterial(const char*,     const char*,     Bool_t,   Double_t, Double_t,
    Double_t=-1, Double_t=-1, Float_t=1,Float_t=0,Float_t=1);
  //               Name         Title        Sensible  Mu        AbsLa
  //               AbsLb        AbsLc        A         Z         Density
  //constructor where all optical properties need to be fixed elsewhere
  TOpticMaterial(const char*, Float_t,  Float_t,  Float_t,   Double_t = 1.0, Bool_t=kFALSE);
  //               Name         A         Z         Density    mu              Sensible
  //Care: Not like other constructors!
  virtual        ~TOpticMaterial();
  Bool_t          CheckIt();
  void            dEdx(const char*,Double_t,Bool_t = kTRUE);
  dEdxMethod      dEdxActiv() const               { return fdEdxUsed;            }
  void            DoWLS(Double_t &,Double_t &,Int_t &);
  void            FindSpectrum(const char*);
  void            FluoComponent(Double_t,Double_t,Double_t,Double_t);
  Double_t        GetAbsL() const                 { return fAbsLa;               }
  Double_t        GetAbsLb() const                { return fAbsLb;               }
  Double_t        GetAbsLc() const                { return fAbsLc;               }
  Short_t         GetAniAbsL() const              { return fAniAbsL;             }
  Short_t         GetAnisotropy() const           { return fAnisotropic;         }
  Bool_t          GetCanStopElectron() const      { return fCanStopElectron;     }
  Double_t        GetdEdx(Double_t) const;
  Double_t        GetDiffLength() const           { return fDiffusLength;        }
  Double_t        GetEc() const                   { return fEc;                  }
  Double_t*       GetElectronProperties()         { return &(fEP[0]);            }
  Double_t        GetEpsa() const                 { return fEpsa;                }
  Double_t        GetEpsb() const                 { return fEpsb;                }
  Double_t        GetEpsc() const                 { return fEpsc;                }
  Short_t         GetFluorescent() const;
  Bool_t          GetHasAbsorption() const        { return fHasAbsorption;       }
  Bool_t          GetHasDiffusion() const         { return fHasDiffusion;        }
  Bool_t          GetHasRayleigh() const          { return fHasRayleigh;         }
  Bool_t          GetHasWLS() const               { return fHasWLS;              }
  Bool_t          GetHistFluo() const             { return fHistFluo;            }
  Bool_t          GetIChangeWithWvl() const;
  Double_t        GetIndex() const                { return fIndex;               }
  Bool_t          GetInUse() const                { return fInUse;               }
  TLitSpectrum   *GetLitSpectrum() const          { return fLitSpectrum;         }
  Double_t        GetMu() const                   { return fMu;                  }
  Double_t        GetnCerenkov() const;
  Int_t           GetNPhot(Double_t Edep) const;  
  Double_t        GetPhimua() const               { return fPhimua;              }
  Double_t        GetPhimub() const               { return fPhimub;              }
  Double_t        GetPhimuc() const               { return fPhimuc;              }
  Double_t        GetPhotMev() const;
  Double_t        GetRayleighLength() const       { return fRayleighLength;      }
  Double_t        GetRm() const                   { return fRm;                  }
  Double_t        GetWLSL() const                 { return fWLSL;                }
  Double_t        GetX0() const                   { return fX0;                  }
  Double_t        GetXSectnPE(Double_t MeV) const;
  Bool_t          GoodForCPE() const;
  void            IsBirefr(const char*,const char*,const char*);
  void            IsBirefr(Double_t,Double_t,Double_t);
  void            IsBirefr(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  void            IsBirefr(TSellmeier*,TSellmeier*, TSellmeier*);
  Bool_t          IsFluorescent() const           { return (GetFluorescent()>0); }
  void            IsIsotropic(const char*);
  void            IsIsotropic(Double_t);
  void            IsIsotropic(Int_t,Double_t,Double_t,Double_t,Double_t=0.0,Double_t=0.0,Double_t=0.0,Double_t=0.0);
  void            IsIsotropic(TSellmeier*);
  Bool_t          IsNonProportional() const       { return fHasNPLY;             }
  Bool_t          IsSensible() const              { return fSensible;            }
  void            IsUniAxialNegBirefr(const char*,const char*);
  void            IsUniAxialNegBirefr(Double_t,Double_t);
  void            IsUniAxialNegBirefr(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  void            IsUniAxialNegBirefr(TSellmeier*,TSellmeier*);
  void            IsUniAxialPosBirefr(const char*,const char*);
  void            IsUniAxialPosBirefr(Double_t,Double_t);
  void            IsUniAxialPosBirefr(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  void            IsUniAxialPosBirefr(TSellmeier*,TSellmeier*);
  void            NewWavelength(Double_t);
  void            Print() const;
  void            SetAbsLa(Double_t,Bool_t = kFALSE);
  void            SetAbsLa(const char *,Bool_t = kFALSE);
  void            SetAbsLb(Double_t,Bool_t = kFALSE);
  void            SetAbsLb(const char *,Bool_t = kFALSE);
  void            SetAbsLc(Double_t,Bool_t = kFALSE);
  void            SetAbsLc(const char *,Bool_t = kFALSE);
  void            SetCanStopElectron(Double_t, Double_t, Double_t,Double_t, Double_t, Double_t);
  void            SetCascades(Double_t, Double_t, Double_t = -1.0);
  void            SetDiffusionLength(Double_t,Bool_t=kTRUE);
  void            SetDiffusionLength(const char* ="");
  void            SetEc(Double_t ec)     { fEc = ec;             }
  void            SetFactorOnAbsL(Double_t);
  void            SetHistFluo(Bool_t);
  void            SetInUse()                      { fInUse = kTRUE;       }
  void            SetLitSpectrum(TLitSpectrum *s) { fLitSpectrum = s;     }
  void            SetNonPropLightYield(Text_t* ="");
  void            SetNotUsed()                    { fInUse = kFALSE;      }
  void            SetPhotMev(Double_t photmev, Double_t resScale = 1.0);
  void            SetRayleighLength(Double_t,Bool_t = kFALSE);
  void            SetRayleighLength(const char*);
  void            SetRm(Double_t rm)              { fRm = rm;             }
  void            SetWLS(TLitSpectrum*, Double_t wlsl);
  void            SetWLS(TLitSpectrum*, Text_t* wlslFile);
  void            SetX0(Double_t x0)              { fX0 = x0;             }
  void            SetXSectnPE(Text_t* name);
  void            WaveAndLife(Double_t&,Double_t&,Int_t&);
  ClassDef(TOpticMaterial,1)  //A material, with its optical properties.
};
R__EXTERN TOpticMaterial *gOutside;
#endif
