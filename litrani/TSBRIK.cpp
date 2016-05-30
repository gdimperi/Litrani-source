// @(#)root/html:$Id: TSBRIK.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "T3Vector.h"
#include "TSupplShape.h"
#include "TFace.h"
#include "TSBRIK.h"
ClassImp(TSBRIK)
//______________________________________________________________________________
//
//
//   RECALL THAT ALL DIMENSIONS OF SHAPES ARE IN cm IN LITRANI !
//
// TSBRIK inherits from TBRIK and adds supplementary information required for
//        Litrani, in particular the default revetment of all faces.
//
// TSBRIK is a box with faces perpendicular to the axes. It has 7 parameters:
//
//    const char   *name       name of the shape
//    const char   *title      shape's title
//    const char   *namemat    name of the optical material of the shape
//OR  const char   *nameshape  name of the other shape having same material and
//                              optical axis
//    const char   *namerev    name of the default revetment for the 6 faces
//    Double_t     dx          half-length of the box along the x-axis in cm
//    Double_t     dy          half-length of the box along the y-axis in cm
//    Double_t     dz          half-length of the box along the z-axis in cm
//    Bool_t       FromOther   true if material and optical axis have to be
//                              taken from an other shape.
//
// The 8 points of a TSBRIK are defined in the following way :
//
//  point[0] = (-dx,-dy,-dz)
//  point[1] = (+dx,-dy,-dz)
//  point[2] = (+dx,+dy,-dz)
//  point[3] = (-dx,+dy,-dz)
//  point[4] = (-dx,-dy,+dz)
//  point[5] = (+dx,-dy,+dz)
//  point[6] = (+dx,+dy,+dz)
//  point[7] = (-dx,+dy,+dz)
//
// The 6 faces of the TSBRIK are built from these points in the following way :
//
//  face 0  : defined by points 0 1 5 4
//  face 1  : defined by points 1 2 6 5
//  face 2  : defined by points 2 3 7 6
//  face 3  : defined by points 3 0 4 7
//  face 4  : defined by points 1 0 3 2
//  face 5  : defined by points 4 5 6 7
//
//
// For each shape of Litrani {tsbrik,tstrd1,tstrd2,tspara,tstrap,tsgtra,
//ts8pts,tscyl,tstube,tscone} there is an associated "friend" class
//TSupplShape, which contains everything which is common to all kind of
//shapes. This associated class may be reached from TSBRIK (or from any
//other shape) like this : (supposing that pbrik is a pointer on a TSBRIK)
//
//     TSupplShape *suppl = pbrik->fSuppl;
//
// All shapes in Litrani may at the same time be a detector of photon. For
//instance, a shape may represent a phototube or an APD. See the description
//how to define a shape as a detector in the presentation of the TSupplShape
//class.
//
//  If FromOther is true, it means that the material of the shape and the
//optical axis are taken from an other shape. If this is the case, it means
//that in case of transition between this shape and the other shape having
//same material and same optical axis, the photon simply continues from one
//shape to the other, without any transition calculation.
//
TSBRIK::TSBRIK(const char *name,const char *title,const char *namemat,
  const char *namerev,Double_t dx, Double_t dy, Double_t dz,Bool_t FromOther):TBRIK(name,title,namemat,Float_t(dx),Float_t(dy),Float_t(dz)) {
  //constructor
  const Double_t zero = 0.0;
  TString nameshape;
  T3Vector *v;
  TFace *face;
  fLx = dx;
  fLy = dy;
  fLz = dz;
  fSuppl = 0;
  if (FromOther) {
    nameshape = namemat;
    fSuppl = new TSupplShape(name,title,this,tsbrik,nameshape,namerev);
  }
  else {
    fSuppl = new TSupplShape(name,title,this,tsbrik,namemat,namerev);
  }
  fSuppl->fGCenter.Set(zero,zero,zero);
  fSuppl->fNbSubPoints = 0;
  fSuppl->fNbPoints = 8;
  fSuppl->fTotalNbPoints = fSuppl->fNbPoints;
  fSuppl->fPoints = new TObjArray();
  v = new T3Vector(-fLx,-fLy,-fLz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(+fLx,-fLy,-fLz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(+fLx,+fLy,-fLz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(-fLx,+fLy,-fLz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(-fLx,-fLy,+fLz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(+fLx,-fLy,+fLz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(+fLx,+fLy,+fLz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(-fLx,+fLy,+fLz);
  fSuppl->fPoints->Add(v);
  fSuppl->fNbMasterFaces = 6;
  fSuppl->fNbSubFaces = 0;
  face = new TFace(this,0,-1,polygonal,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,0,1,5,4);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,1,-1,polygonal,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,1,2,6,5);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,2,-1,polygonal,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,2,3,7,6);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,3,-1,polygonal,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,3,0,4,7);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,4,-1,polygonal,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,1,0,3,2);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,5,-1,polygonal,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,4,5,6,7);
  fSuppl->fFaces.Add(face);
  fSuppl->fFaces.Sort();
}
TSBRIK::~TSBRIK() {
  //Destructor
  if (fSuppl) {
    delete fSuppl;
    fSuppl = 0;
  }
}
void TSBRIK::SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const {
  //  Sets the 3 optical axis fAniOx, fAniOy, fAniOz. The 3 optical axis are the 3 axis of the
  //coordinate system in which the dielectric tensor is diagonal, with eigenvalues
  //
  //   ea   along the Ox axis
  //   eb   along the Oy axis
  //   ec   along the Oz axis
  //   ea <= eb <= ec
  //   ea <  eb =  ec  for "uniaxial negativ birefringent" crystals
  //   ea =  eb <  ec  for "uniaxial positiv birefringent" crystals
  //
  //  These 3 axis are given "a la GEANT" : give 
  //   theta and phi of fAniOx axis   (thetax,phix)
  //   theta and phi of fAniOy axis   (thetay,phiy)
  //  It is not necessary to give theta and phi of fAniOz : fAniOz can be determined with fAniOx
  //and fAniOy. This way of giving a coordinate system inside the local coordinate system is intuitive,
  //but inelegant, because it is redundant. If you give an fAniOy axis which is not orthogonal to the
  //fAniOx axis, you will be warned and your fAniOy axis will be corrected.
  //
  //  GIVE ALL ANGLES IN DEGREE !
  //
  //  To understand the construction of the dielectric tensor both in Local Coordinate System and
  //in the World Coordinate system, read the general description of the class TSupplShape.
  //
  fSuppl->SetOpticalAxis(thetax,phix,thetay,phiy);
}
