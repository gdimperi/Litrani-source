// @(#)root/html:$Id: TSupplShape.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSupplShape
#define LITR_TSupplShape
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSupplShape   Add information necessary for all types of shape       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TH2D.h"
#include "TShape.h"
#include "TLitPhys.h"
#include "T33CMatrix.h"
#include "T3Vector.h"
#include "TSNode.h"
#include "TFace.h"

enum KindOfShape {tsbrik,tstrd1,tstrd2,tspara,tstrap,tsgtra,ts8pts,tscyl,tstube,tscone};

class TOpticMaterial;
class TRevetment;

class TIntercept : public TObject {

protected:

  Double_t fT;    //distance from point x0
  T3Vector fX;    //point of intercept

public:

  TIntercept() { ;}
  TIntercept(Double_t t, T3Vector x) { fT=t; fX=x; }
  Int_t    Compare(const TObject *b) const;
  Double_t GetT() const { return fT; }
  T3Vector GetX() const { return fX; }
  Bool_t   IsEqual(const TObject*) const;
  Bool_t   IsSortable() const { return kTRUE; }
};

class TSupplShape : public TNamed {

  friend class TSNode;
  friend class TSBRIK;
  friend class TSTRD1;
  friend class TSTRD2;
  friend class TSPARA;
  friend class TSTRAP;
  friend class TSGTRA;
  friend class TS8PTS;
  friend class TSCYL;
  friend class TSTUBE;
  friend class TSCONE;

protected:

  Bool_t          fFromOther;       //material and optical axis to be taken from other shape
  TSupplShape    *fOther;           //other shape to look for material and optical axis
  KindOfShape     fKind;            //brik, cone, cylinder, etc
  TSNode         *fSNode;           //TSNode associated with this TSupplShape.
  TShape         *fSShape;          //TSShape associated with this TSupplShape.
  Int_t           fNbPoints;        //number of points for this shape, not including supplementary points
  Int_t           fNbSubPoints;     //number of supplementary points for this shape
  Int_t           fTotalNbPoints;   //number of points for this shape, with supplementary points
  Int_t           fNbMasterFaces;   //number of master faces
  Int_t           fNbSubFaces;      //number of subfaces
  TObjArray       fFaces;           //list of all master faces
  TObjArray       fSubFaces;        //list of all subfaces
  TOpticMaterial *fOptMat;          //optical material for this shape
  Short_t         fAnisotropic;     //anisotropic characteristics of material
  Short_t         fAniAbsL;         //0 ==> abs. L. isotropic 1 ==> La != Lb = Lc  2 ==> La = Lb != Lc  3 ==> la != Lb != Lc
  T3Vector        fAniOx;           //x axis of dielectric tensor in case fOptMat anisotropic
  T3Vector        fAniOy;           //y axis of dielectric tensor in case fOptMat anisotropic
  T3Vector        fAniOz;           //z axis of dielectric tensor in case fOptMat anisotropic
  T33Matrix       fM;               //Rot. matrix which transforms Ox(LCS) to Ox'(DTS). DTS = Dielectric tensor system
  T33Matrix       fMt;              //Rot. matrix which transforms E(LCS) to E'(DTS). DTS = Dielectric tensor system. fMt is the transpose matrix of fM.
  Bool_t          fDielChange;      //true if dielectric tensor depends upon wavelength.
  TRevetment     *fDefRev;          //default revetment for this shape
  // Bevelling (chanfreins)
  Bool_t          fChf;             //If true, the shape has bevellings
  Double_t        fChfDist;         //Distance [cm] from edge below which photon on bevelling
  TRevetment     *fChfRev;          //Revetment near edges
  Bool_t          fChfIsDepolished; //True if bevellings depolished.
  Double_t        fChfCosDpAngle;   //Cos of depolish angle of bevellings
  // Detector characteristics
  Int_t           fNDet;            //detector number assigned by TResults
  KindOfDetector  fDet;             //{none,surfdet,phototube,volumedet,apd}
  Int_t           fNFaceA;          //surfdet   : Face Nb. of sensitive face or subface. phototube : Face Nb. of face representing the photocathode. volumedet : irrelevant. apd : Face Nb. of face acting as anode (back face)
  Int_t           fNFaceC;          //apd       : Face Nb. of face acting as cathode (entrance). all other : irrelevant
  Bool_t          fIsQE;            //true if there is a quantum efficiency
  TString         fNameQE;          //name of the fit (TSplineFit) giving quantum efficiency of face fNFaceA as a function of wavelength. For volume detectors and APD only
  TSplineFit     *fQE;              //pointer to this fit for volume detectors and APD only
  Double_t        fVDrifte;         //drift velocity of electrons inside APD in cm/s
  TString         fNameGP;          //name of the fit giving gain profile of the APD
  TSplineFit     *fGP;              //pointer to this fit
  Double_t        fDistCatAn;       //distance from cathode to anode in nm.
  // Damages due to radiation. Variables for both methods
  Bool_t          fDamage;          //true if depth dependent damage
  T3Vector        fDamAxis;         //axis of damage
  T3Vector        fDamStart;        //starting point of damage
  Double_t        fDamKimax;        //Kimax 1/Li(z,r,l) at the point where Li(z,r,l) is minimal, i.e. where damages are maximal
  TSplineFit     *fDamFitKimax;     //Fit giving Kimax as a function of wavelength
  Double_t        fDamFmax;         //fmax: value of damage distribution f(z,r) where it is maximal
  Double_t        fDamStep;         //steps [cm] for calculating absorption
  Double_t        fDamSupFac;       //supplementary constant factor by which Ki(lambda) can be multiplied [default 1]
  TH2D           *fHDamage;         //histogram recording 1 - damage factor.
  // Damage method of "RadiationDamage"
  Double_t        fDamZm;           //mean of gaussian  in z representing damages f(z,r)
  Double_t        fDamZs;           //sigma of gaussian in z representing damages f(z,r)
  Double_t        fDamRs;           //sigma of gaussian in r representing damages f(z,r)
  Double_t        fDamGz;           //value at a given depth of g(z): the z distribution provided by the user.
  Double_t        fDamGzMax;        //maximum value taken by g(z)
  TSplineFit     *fDamFitGz;        //fit giving g(z)
  // Damage method of "ShowerDamage"
  Bool_t          fDamByShower;     //damages have been caused by showers. ShowerDamage() has been called, not RadiationDamage()
  Double_t        fDamE0;           //energy in Mev of the showers having caused damage
  Bool_t          fDamGamma;        //true if damages caused by gammas. If false, damages caused by electrons
  Double_t        fDamA;            //a of ShowerDamage
  Double_t        fRk2;             //K^2 of the r distribution
  Double_t        fDamFac;          //constant factor in front of dependent factor in f(z,r)

  Bool_t          AddToList() const;
  Bool_t          AddToList(const char*) const;
  void            Init();
  void            InitP();

public:

  TObjArray      *fPoints;          //list of all points.
  T3Vector        fGCenter;         //center of Shape
  Int_t           fShapeNumber;     //will be assigned at end of defining setup
  T3Vector        fAxisAPD;         //axis (direction)along which gain profile is given inside APD
  T3Vector        fx0APD;           //start point of fAxisAPD, on the cathode
  T3Vector        fx1APD;           //End point of fAxisAPD, on the anode
  T33Matrix       fDTmu;            //dielectric tensor MULTIPLIED BY MAGNETIC PERMEABILITY
  T33CMatrix      fCDTmu;           //complex fDTmu to calculate complex index of wave

  TSupplShape() { InitP(); }
  TSupplShape(const char*,const char*,TShape *,const KindOfShape,const char*,const char*);
  TSupplShape(const char*,const char*,TShape *,const KindOfShape,TString,const char*);
  virtual        ~TSupplShape();
  void            AddSubFace(TFace*);
  void            AddSubPoint(T3Vector*);
  Short_t         AllCrossingPoints(const T3Vector&,const T3Vector&,Double_t&,Double_t&,Double_t&,Double_t&,
    T3Vector&,T3Vector&,T3Vector&,T3Vector&) const;
  Double_t        CosDplAngle() const          { return fChfCosDpAngle;   }
  Bool_t          DplBevellings() const        { return fChfIsDepolished; }
  Bool_t          FindContact(TSupplShape*, Int_t&, TFace*&, Int_t&, TFace*&);
  Double_t        GetDamage(const T3Vector*,Double_t);
  Double_t        GetDamStep() const           { return fDamStep;         }
  KindOfDetector  GetDetector() const          { return fDet;             }
  Int_t           GetDetNumber() const         { return fNDet;            }
  Double_t        GetDistCatAn() const         { return fDistCatAn;       }
  TFace          *GetFace(Int_t) const;
  T33Matrix      *GetfM()                      { return &fM;              }
  TSplineFit     *GetGainProfile() const       { return fGP;              }
  KindOfShape     GetKind() const              { return fKind;            }
  TObjArray      *GetListOfFaces()             { return &fFaces;          }
  const char     *GetName() const              {return fSShape->GetName();}
  Int_t           GetNbOfFaces() const         { return fNbMasterFaces + fNbSubFaces; }
  TSNode         *GetNode() const              { return fSNode;           }
  TOpticMaterial *GetOptMat() const            { return fOptMat;          }
  TSupplShape    *GetOther() const             { return fOther;           }
  T3Vector       *GetPoint(Int_t i) const      { return (T3Vector *)(*fPoints)[i]; }
  TSplineFit     *GetQuantumEfficiency() const { return fQE;              }
  TShape*         GetShape() const             { return fSShape;          }
  Double_t        GetVDrifte() const           { return fVDrifte;         }
  Bool_t          HasBevellings() const        { return fChf;             }
  Bool_t          IsDamage() const             { return fDamage;          }
  Bool_t          IsFluorescent() const;
  Bool_t          IsFromOther() const          { return fFromOther;       }
  Bool_t          IsInside(Double_t, Double_t, Double_t) const;
  Bool_t          IsInside(const T3Vector&) const;
  Bool_t          IsQE() const                 { return fIsQE;            }
  void            NewWavelength(Double_t);
  TFace          *NextFace(const TFace*,const T3Vector&,const T3Vector&,T3Vector&,Double_t&,Double_t) const;
  void            Print() const;
  void            PrintDiel() const;
  void            PrintFace(Int_t i) const;
  void            PrintPoints() const;
  Bool_t          RadiationDamage(const T3Vector*,const T3Vector*,Double_t,Double_t,Double_t,Double_t,Double_t = 0.4,Double_t = 1.0);
  Bool_t          RadiationDamage(const T3Vector*,const T3Vector*,const char*,Double_t,Double_t,Double_t,Double_t = 0.4,Double_t = 1.0);
  Bool_t          RadiationDamage(const T3Vector*,const T3Vector*,Double_t,const char *,Double_t,Double_t = 0.4,Double_t = 1.0);
  Bool_t          RadiationDamage(const T3Vector*,const T3Vector*,const char*,const char *,Double_t,Double_t = 0.4,Double_t = 1.0);
  void            RecordDamages(Int_t,Double_t,Int_t,Double_t);
  TRevetment     *RevBevellings() const        { return fChfRev;          }
  Bool_t          SameMedium(TSupplShape*) const;
  void            SetAPD(const char*,const char*,Int_t,Int_t,const char*,Double_t = -1.0,Double_t = 1.0e+5);
  void            SetBevellings(Bool_t,Double_t=0.0, const char* ="none", Double_t=0.0);
  void            SetDetNumber(Int_t n) { fNDet = n; }
  void            SetDielTensor();
  void            SetM();
  void            SetNoDamage();
  void            SetOpticalAxis(Double_t, Double_t, Double_t, Double_t);
  void            SetPhotoTube(const char*,const char*,Int_t,const char*);
  void            SetSurfDet(const char*,const char*,Int_t,const char* = "",Double_t = 91.0,KindOfDetector=surfdet);
  void            SetVolumeDet(const char*,const char*,const char* = "");
  void            ShowDamage() const;
  Bool_t          ShowerDamage(const T3Vector*,const T3Vector*,Double_t,Double_t,Bool_t=kTRUE,Double_t=0.4,Double_t = 1.0);
  Bool_t          ShowerDamage(const T3Vector*,const T3Vector*,const char*,Double_t,Bool_t=kTRUE,Double_t=0.4,Double_t = 1.0);
  Double_t        SizeBevellings() const       { return fChfDist;         }
  ClassDef(TSupplShape,0) //more information for all shapes
};
#endif
