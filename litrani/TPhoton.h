// @(#)root/html:$Id: TPhoton.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TPhoton
#define LITR_TPhoton
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TPhoton    Full description of a photon inside the set-up          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TShape.h"
#include "TSComplex.h"
#include "T3Vector.h"
#include "TFace.h"

class TRevetment;
class TContact;
class ThinSlice;
class TSupplShape;
class TOpticMaterial;
class TTWave;

class TPhoton:public TObject {

protected:

  Int_t           fRun;          //run number
  Int_t           fKphot;        //photon number
  //location
  KindOfShape     fKind;         //kind of shape in which photon is
  TShape         *fShape;        //! shape in which photon is
  TSupplShape    *fS;            //! TSupplShapehape in which photon is
  Int_t           fNShape;       //Number of this shape
  Axis_t          fANShape;      //Idem as Axis_t
  T3Vector        fX0;           //Location of photon in World Coordinates
  Double_t        fSl0;          //space travelled by photon
  Double_t        fDistRidge;    //Distance of photon to nearest ridge of face (when on a face )
  //time
  Double_t        fT0;           //time elapsed since birth of photon or excitation in [ps]
  //status
  Int_t           fNPoints;      //(NPTPHOT) Number of points on the trajectory of the photon
  Bool_t          fPhtLive;      //true if photon not yet absorbed or detected
  Bool_t          fPhtSeen;      //true if photon seen by detector
  Bool_t          fToOther;      //true if photon towards other convex polyhedron
  Bool_t          fBetween;      //true if photon between 2 convex polyhedrons
  Bool_t          fOutPoly;      //VERIFIER QUE PAS INUTILE !true if photon out of shape
  Bool_t          fPhtOut;       //true if photon is transmitted or absorbed, false if reflected.
  Bool_t          fPhtAbs;       //true if photon absorbed during transition process for instance on reflection on metal.
  Bool_t          fAnomaly;      //photon to be killed for abnormal reasons. Should never happen
  Bool_t          fFromani;      //true if incident wave in anisotropic medium
  Short_t         fChooseIndex;  //choice between n1 and n2 {0->random,1->biggest,2->smallest}
  //physical values
  Double_t        fSkl0;         //Sum over all k*l
  Double_t        fWvlgth;       //wavelength of photon in nanometers
  Double_t        fW;            //frequency in ps-1
  Double_t        fEphot;        //energy of photon in joule [mksa]
  //The quotients of indices and magnetic permeability :
  TSComplex       fBI;           // (ni*mut)/(nt*mui)
  TSComplex       fBR1;          // (nr1*mut)/(nt*mui)
  TSComplex       fBR2;          // (nr2*mut)/(nt*mui)
  TSComplex       fBT1;          // (nt1*mui)/(ni*mut)
  TSComplex       fBT2;          // (nt2*mui)/(ni*mut)
  //k vector
  T3Vector        fVk0;          //k vector in WCS
  Double_t        fAk0;          //absolute value (module) of vector k in [cm-1]
  T3Vector        fUk0;          //unit k vector in WCS
  TSComplex       fNc;           //complex index of refraction, including absorption
  //Absorption length and diffusion length
  Double_t        fAbsl0;        //abs. length for this photon.
  Bool_t          fIsDiffus;     //true if there is diffusion inside this material
  Double_t        fDiffusL;      //diffusion length
  Int_t           fNbOfDiffusion;//number of diffusions that occured for this photon
  Int_t           fNbOfRayleigh; //number of diffusions that occured for this photon
  Int_t           fNbOfWLS;      //number of wavelength shifts that occured for this photon
  //WLS and Rayleigh and absorption
  Bool_t          fIsRayleigh;   //true if there is Rayleigh scattering inside this material
  Bool_t          fIsWLS;        //true if the material is wavelength shifting
  Bool_t          fIsAbsorb;     //true if there is absorption inside this material
  Double_t        fRayleighL;    //Rayleigh Scattering length
  Double_t        fWLSL;         //Wavelength shifting length
  //Electric Field
  Double_t        fAef0;         //absolute value (module) of electric field E
  T3Vector        fEf0;          //electric field [MKSA] in WCS
  Double_t        fBeta;         //phase of linearly polarized photon (irrelevant)
  //Magnetic Field
  T3Vector        fBf0;          //magnetic field [MKSA] in WCS
  //Poyinting Vector
  T3Vector        fPtg;          //Pyinting vector [MKSA] in WCS
  T3Vector        fUpg;          //Unit Poyinting vector in WCS
  Double_t        fCospk;        //cosine of angle between Poyinting and k vector
  //characteristics of the face where the photon is
  TFace          *fFace;         //! master face on which photon is, 0 if not on a face
  Int_t           fNFace;        //face number of the master face fFace
  Int_t           fNbSubFaces;   //number of subfaces on this face
  TFace          *fSubFace;      //! subface on which the photon is, if any
  TFace          *fUsedFace;     //! = fFace if (fSubFace==0), = fSubFace otherwise.
  Int_t           fNSubFace;     //face number of the sub face fSubFace, if any
  Int_t           fKFace;        //= fNSubFace if (fSubFace), else = fNFace
  Bool_t          fPlanar;       //true if fFace is planar
  TRevetment     *fRevetment;    //! revetment of this face
  TOpticMaterial *fMatSl;        //! material between face and revetment
  Double_t        fSinThetaMax;  //Max. value for angle theta in diffusion
  T3Vector        fFN;           //normal to the face at the point fX0
  T3Vector        fFNd;          //normal to the face, tilted by depolish
  Int_t           fNDet;         //detector number if fFace or fSubFace is a detector
  KindOfDetector  fDetKind;      // detector type
  Double_t        fAccept;       //acceptance angle of detector
  TSplineFit     *fQE;           //! quantum efficiency of detector in function of wavelength
  Bool_t          fHasContact;   //true if face in contact with an other face
  Int_t           fNContact;     //number of contacts
  KindOfContact   fContactType;  //type of contact
  TContact       *fContact;      //! contact
  ThinSlice      *fThinSlice;    //! if the contact has a thin slice
  //characteristics of the face in contact
  TFace          *fContactFace;  //! master face in contact with fFace, if any
  //characteristics of material in which the photon is
  TOpticMaterial *fMat0;         //! material of fShape
  Short_t         fAnis0;        //anisotropic optical characteristics of material in which photon is
  Short_t         fAnia0;        //anisotropic absorption characteristics of material in which photon is
  Double_t        fPmu0;         //magnetic permeability of material in which photon is
  TSComplex       fRfIndex0;     //refraction index, complex for convenience. Im always 0
  Double_t        fDiel0;        //diel. constant eps in case of isotropic material
  Bool_t          fIsDiag;       //the dielectric tensor in use is diagonal in ICS
  T33Matrix       fDzt0;         //dielectric tensor epsilon multiplied by mu in WCS
  T33CMatrix      fCDzt0;        //complex fDzt0, including absorption
  //characteristics of material associated with the incident wave
  Short_t         fAnisi;        //anisotropic characteristics of material associated with the incident wave
  Short_t         fAniai;        //anisotropic absorption characteristics of material associated with the incident wave
  Double_t        fPmui;         //magnetic permeability mu of material associated with the incident wave
  TSComplex       fRfIndexi;     //refraction index of material associated with the incident wave
  Bool_t          fIsDiagi;      //the dielectric tensor of the shape containing the incident wave is diagonal in ICS
  T33Matrix       fDzti;         //dielectric tensor epsilon multiplied by mu in WCS, ass. with the incident wave
  //identical with fDzt0 when photon forward and with fDzt1 when photon backward
  T33Matrix       fDztii;        //idem fDzti in coordinate system of incidence (ICS)
  //characteristics of material into which the photon may go
  TShape         *fContactShape; //! shape in contact with face fFace
  TSupplShape    *fContactSuppl; //! TSupplShape associated with fContactShape
  TOpticMaterial *fMat1;         //! material of the shape in contact
  Short_t         fAnis1;        //anisotropic optical characteristics of destination material
  Short_t         fAnia1;        //anisotropic absorption characteristics of destination material
  Double_t        fPmu1;         //magnetic permeability of destination material
  TSComplex       fRfIndex1;     //complex refraction index  of destination material
  Double_t        fDiel1;        //diel. constant eps in case of isotropic destination material
  T33Matrix       fDzt1;         //dielectric tensor epsilon multiplied by mu in WCS
  //characteristics of material associated with the transmitted wave
  Short_t         fAnist;        //anisotropic characteristics of material associated with the transmitted wave
  Short_t         fAniat;        //anisotropic absorption characteristics of material associated with the transmitted wave
  Double_t        fPmut;         //magnetic permeability of  of material associated with the transmitted wave
  TSComplex       fRfIndext;     //refraction index of material associated with the transmitted wave
  Bool_t          fIsDiagt;      //the dielectric tensor of the shape containing the transmitted wave is diagonal in ICS
  T33Matrix       fDztt;         //dielectric tensor epsilon multiplied by mu in WCS, ass. with the transmitted wave
  //identical with fDzt1 when photon forward and with fDzt0 when photon backward
  T33Matrix       fDztti;        //idem fDztt in coordinate system of incidence (ICS)
  //Definition of the incident plane
  T3Vector        fOx;           //Ox axis of the coordinate system of incidence
  T3Vector        fOy;           //Oy axis of the coordinate system of incidence
  T3Vector        fOz;           //Oz axis of the coordinate system of incidence
  T33Matrix       fMn;           //matrix transforms a vector vi in ICS into a vector v in WCS
  T33Matrix       fMt;           //matrix transforms a vector v in WCS into a vector vi in ICS. fMt is the transposed of fMn
  //5 possible waves at the intersection :
  TTWave         *fI;            //incident wave
  TTWave         *fR1;           //1st reflected wave
  TTWave         *fR2;           //2nd reflected wave. Does not exist in isotropic material.
  TTWave         *fT1;           //1st transmitted wave
  TTWave         *fT2;           //2nd transmitted wave. Does not exist in isotropic material.
  //Weight of each possible choice for a wave
  Double_t        fWeigth[8];    //8 possible weigths
  OpticalProcess  fProcess;      // Optical process the photon will undergo next

  void     AgainstRev();
  void     AgainstShape();
  void     AniToAni() const;
  void     AniToIso();
  void     APDDet();
  void     BuildBPUP(const T3Vector&,const T3Vector&,T3Vector&,T3Vector&,T3Vector&,Double_t) const;
  //
  void     ch1111();
  void     ch1112();
  void     ch1121();
  void     ch1113();
  void     ch1114();
  void     ch1122();
  void     ch1123();
  void     ch1124();
  void     ch1131();
  void     ch1132();
  void     ch1133();
  void     ch1141();
  void     ch1142();
  void     ch1144();
  void     ch1211();
  void     ch1212();
  void     ch1213();
  void     ch1214();
  void     ch1221();
  void     ch1222();
  void     ch1223();
  void     ch1224();
  void     ch1231();
  void     ch1232();
  void     ch1233();
  void     ch1241();
  void     ch1242();
  void     ch1244();
  void     ch1311();
  void     ch1312();
  void     ch1313();
  void     ch1321();
  void     ch1322();
  void     ch1323();
  void     ch1331();
  void     ch1332();
  void     ch1333();
  //
  void     ch2111();
  void     ch2112();
  void     ch2121();
  void     ch2113();
  void     ch2114();
  void     ch2122();
  void     ch2123();
  void     ch2124();
  void     ch2131();
  void     ch2132();
  void     ch2133();
  void     ch2141();
  void     ch2142();
  void     ch2144();
  void     ch2211();
  void     ch2212();
  void     ch2213();
  void     ch2214();
  void     ch2221();
  void     ch2222();
  void     ch2223();
  void     ch2224();
  void     ch2231();
  void     ch2232();
  void     ch2233();
  void     ch2241();
  void     ch2242();
  void     ch2244();
  void     ch2311();
  void     ch2312();
  void     ch2313();
  void     ch2321();
  void     ch2322();
  void     ch2323();
  void     ch2331();
  void     ch2332();
  void     ch2333();
  //
  void     Choose();
  void     Depolish();
  void     Diffused();
  void     FindAbsLength();
  void     FindDielAni();
  void     FindDielIso();
  Double_t FindProcessAndLength(T3Vector);
  void     GetImin12(Short_t,Short_t&,Short_t&,Short_t = -1);
  void     GetShapeAndMat(TSupplShape*);
  void     ICStoWCS();
  void     Init();
  void     InitP();
  void     IsoToAni();
  void     IsoToIso();
  void     IsoTsIso();
  Short_t  K(Short_t i, Short_t j) const { return 3*j + i; }
  void     PrintWhich() const;
  void     PrintWhich(Int_t ifreq) const;
  void     Regenerate(OpticalProcess);
  void     SliceToRev();
  void     SliceToShape();
  void     StartPhoton();
  void     SurfDet();
  void     ToICS();
  Short_t  Toss(Short_t);
  void     VerifyAtoI();
  void     VerifyItoA();
  void     VolDet();

public:
  TPhoton() { InitP(); Init();}
  TPhoton(Int_t,Int_t,const T3Vector&,TSupplShape*,Double_t,Double_t,
    T3Vector&,T3Vector&,Short_t=0);
  virtual ~TPhoton();
  void     ExitFace();
  Bool_t   GetAnomaly() { return fAnomaly; }
  void     Jump();
  void     Move(Int_t=-1);
  void     OnFace();
  void     Transition();
  ClassDef(TPhoton,0) //Full description of a photon inside the set-up
};
#endif
