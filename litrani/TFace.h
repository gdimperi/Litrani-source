// @(#)root/html:$Id: TFace.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TFace
#define LITR_TFace
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TFace       Description of a face of a SHAPE                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TShape.h"
#include "TArrayI.h"
#include "TObjArray.h"
#include "TSplineFit.h"
#include "T3Vector.h"
#include "TContact.h"

enum KindOfFace     {polygonal, circular, ring, cylindrical, conical, tronconical};
enum KindOfDetector {none,surfdet,phototube,volumedet,apd};

class TRevetment;
class TSupplShape;
class TOtherSide;

class TFace : public TObject {

  friend class TSNode;

protected:

  Int_t          fNumber;        //Face number
  TShape        *fShape;         //pointer to shape to which this Face belongs
  TSupplShape   *fSuppl;         //pointer to TSupplShape belonging to fShape
  Int_t          fMasterFace;    //If this face is a subdivision of a master face, face number of master face
  Int_t          fNbSubFaces;    //Number of subfaces belonging to this face
  TObjArray      fSubFaces;      //List of all subfaces belonging to this face
  TObjArray     *fPts;           //List of points of shape to which this face is associated
  KindOfFace     fKind;          //Face polygonal, circular, ring, cylindrical, conical, tronconical
  Bool_t         fPlanar;        //True if plane surface
  Int_t          fNbPoints;      //Number of points of face
  TArrayI        fListPts;       //List of points of this face from associated shape
  TRevetment    *fRevetment;     //Pointer to revetment covering this face
  T3Vector       fGc;            //Center of gravity of all points of shape
  Bool_t         fReverse;       //True if normal has to be reversed. See comment of method Normal().
  T3Vector       fN;             //Normal to the surface when plane surface
  TObjArray      fPtsFace;       //List of all points of Face (subset of fPts)
  TObjArray      fRidge;         //List of all ridges
  TObjArray      fOutRidge;      //List of all vectors in face, normal to ridges, pointing outside
  Double_t       fRadiusBase1;   //radius of large basis or basis 1 or large radius for ring
  Double_t       fRadiusBase2;   //radius of small basis or basis 2 or small radius for ring
  Double_t       fConeHeight;    //height of not truncated cone
  Double_t       fHeight;        //height of cylinder or truncated cone
  T3Vector       fAxis;          //unit vector of axis in case of cylinder or cone
  Double_t       fSurface;       //surface of the face
  Double_t       fCharLength;    //square root of fSurface, characteristic length to decide about intersection
  // Contacts
  Bool_t         fHasContact;    //true if this face is in contact with an other face of an other shape
  Int_t          fNContact;      //number of contacts
  TObjArray      fContactSide;   //number of sides in contact with this face
  //characteristics of depolishing
  Bool_t         fIsDepolished;  //if the surface is depolished
  Double_t       fThetaDepls;    //angle of depolish (roughness of surface)
  Double_t       fCosThetaDepls; //cos(fThetaDepls)
  //detector characteristics
  Int_t          fNDet;          //detector number assigned by TResults
  KindOfDetector fDet;           //If this surface is also a detector
  Double_t       fAccept;        //acceptance angle in degree for surface detector
  TSplineFit    *fQE;            //pointer to the fit giving Quantum Efficiency

  void           InitP();
  void           InitTFace(Int_t,Int_t);
  void           OutRidges();
  void           Parameters(Bool_t);
  void           Ridges();
  Double_t       Surface() const;

public:

  TFace() { InitP(); }
  TFace(TShape*, Int_t, Int_t, KindOfFace, TObjArray*, T3Vector&, const char*,     Int_t, Int_t, Int_t, Bool_t = kFALSE);
  TFace(TShape*, Int_t, Int_t, KindOfFace, TObjArray*, T3Vector&, TRevetment*, Int_t, Int_t, Int_t, Bool_t = kFALSE);
  TFace(TShape*, Int_t, Int_t, KindOfFace, TObjArray*, T3Vector&, const char*,     Int_t, Int_t, Int_t,Int_t, Bool_t = kFALSE);
  TFace(TShape*, Int_t, Int_t, KindOfFace, TObjArray*, T3Vector&, TRevetment*, Int_t, Int_t, Int_t,Int_t, Bool_t = kFALSE);
  TFace(TShape*, Int_t, Int_t, KindOfFace, TObjArray*, T3Vector&, const char*,     Int_t, Int_t, Int_t,Int_t, Int_t, Bool_t = kFALSE);
  TFace(TShape*, Int_t, Int_t, KindOfFace, TObjArray*, T3Vector&, TRevetment*, Int_t, Int_t, Int_t,Int_t, Int_t, Bool_t = kFALSE);
  TFace(TShape*, Int_t, Int_t, KindOfFace, TObjArray*, T3Vector&, const char*,     Int_t, Int_t, Int_t,Int_t, Int_t, Int_t, Bool_t = kFALSE);
  TFace(TShape*, Int_t, Int_t, KindOfFace, TObjArray*, T3Vector&, TRevetment*, Int_t, Int_t, Int_t,Int_t, Int_t, Int_t, Bool_t = kFALSE);
  TFace(TShape*, Int_t, Int_t, KindOfFace, TObjArray*, T3Vector&, const char*,     TArrayI&, Bool_t = kFALSE);
  TFace(TShape*, Int_t, Int_t, KindOfFace, TObjArray*, T3Vector&, TRevetment*, TArrayI&, Bool_t = kFALSE);
  virtual ~TFace();
  void           ChangeRevetment(const char*);
  void           ChangeRevetment(TString&);
  Int_t          Compare(const TObject *obj) const;
  TContact      *Contact(Int_t i) const;
  TFace         *ContactFace(Int_t i) const;
  TShape        *ContactShape(Int_t i) const;
  TSupplShape   *ContactSuppl(Int_t i) const;
  KindOfContact  ContactType(Int_t i) const;
  Bool_t         Contained(const T3Vector &v);
  Double_t       CosDepolishAngle() const  { return fCosThetaDepls;          }
  Double_t       CosDepolishAngle(Double_t) const;
  Double_t       CrossFace(const T3Vector&, const T3Vector&, Double_t&, T3Vector&, Int_t&,
    Int_t&,Double_t&,T3Vector&,Double_t&,Double_t&,Double_t=-1.0e+300) const;
  Double_t       GetAccAngle() const       { return fAccept;                 }
  KindOfDetector GetDetKind() const        { return fDet;                    }
  Int_t          GetDetNumber() const      { return fNDet;                   }
  KindOfFace     GetKind() const           { return fKind;                   }
  Int_t          GetNumber() const         { return fNumber;                 }
  Bool_t         GetPlanar() const         { return fPlanar;                 }
  Int_t          GetPoint(Int_t i) const   { return fListPts[i];             }
  TSplineFit    *GetQuantumEff() const     { return fQE;                     }
  Double_t       GetRadius1() const        { return fRadiusBase1;            }
  TRevetment    *GetRevetment() const      { return fRevetment;              }
  TRevetment    *GetRevetment(Double_t) const;
  TShape        *GetShape() const          { return fShape;                  }
  TFace         *GetSubFace(Int_t i) const { return (TFace *)fSubFaces[i];   }
  Double_t       GetSurface() const        { return fSurface;                }
  Bool_t         HasContact() const        { return fHasContact;             }
  Bool_t         IsDepolished() const      { return fIsDepolished;           }
  Bool_t         IsDepolished(Double_t) const;
  Bool_t         IsEqual(const TObject *obj) const;
  Bool_t         IsInside(const T3Vector &v) const;
  Bool_t         IsSortable() const        { return kTRUE;                   }
  Int_t          NbOfContacts() const      { return fNContact;               }
  Int_t          NbSubFaces() const        { return fNbSubFaces;             }
  T3Vector&      Normal()                  { return fN;                      }
  T3Vector&      Normal(const T3Vector &v);
  Int_t          NumberOfPoints() const    { return fNbPoints;               }
  Bool_t         Parallelogram(T3Vector&,T3Vector&,Double_t&,T3Vector&,Double_t&) const;
  T3Vector      *Point(Int_t i) const      { return (T3Vector *)fPtsFace[i]; }
  void           Print() const;
  void           RemoveContact(TOtherSide *ct);
  void           SetAsDetector(KindOfDetector, const char*,Double_t);
  void           SetAsDetector(KindOfDetector, TSplineFit*,Double_t);
  void           SetContact(TOtherSide*);
  void           SetDepolished(Double_t ThetaDepls = 60.0);
  void           SetDetNumber(Int_t n)     { fNDet = n;                      }
  void           ToWCS();
  ClassDef(TFace,0)//Face of a shape
};
#endif
