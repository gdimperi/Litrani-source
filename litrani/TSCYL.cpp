// @(#)root/html:$Id: TSCYL.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "T3Vector.h"
#include "TFace.h"
#include "TSupplShape.h"
#include "TSCYL.h"

ClassImp(TSCYL)
//______________________________________________________________________________
//
// TSCYL inherits from TTUBE and adds supplementary information required for
//        Litrani, in particular the default revetment of all faces.
//
//   RECALL THAT ALL DIMENSIONS OF SHAPES ARE IN cm IN LITRANI !
//
//
// TSCYL is a cylinder, without hole : it is not a tube. Its axis is the Oz axis in local
//coordinate. The center of the cylinder is the center of the coordinate system.
//
//It has 6 parameters:
//
//    const char   *name       name of the shape
//    const char   *title      shape's title
//    const char   *namemat    name of the optical material of the shape
//OR  const char   *nameshape  name of the other shape having same material and
//    const char   *namerev    name of the default revetment for the 6 faces
//    Double_t     r           radius of the cylinder in cm
//    Double_t     dz          half-length of the cylinder along the z-axis in cm
//    Bool_t       FromOther   true if material and optical axis have to be
//                              taken from an other shape.
//
// We define arbitrarily 6 points to help the construction of the faces of the cylinder :
//
//  point[0] = (  0,  0,-dz)     center of the lower circular basis
//  point[1] = (  r,  0,-dz)     one point on the circle of the lower basis
//  point[2] = (  0,  r,-dz)     an other point on the circle of the lower basis
//  point[3] = (  0,  0,+dz)     center of the upper circular basis
//  point[4] = (  r,  0,+dz)     one point on the circle of the upper basis
//  point[5] = (  0,  r,+dz)     an other point on the circle of the upper basis
//
// The 3 faces of the TSCYL are built from these points in the following way :
//
//  face 0  : defined by points 0 1 2     lower circular basis (plane )
//  face 1  : defined by points 3 4 5     upper circular basis (plane )
//  face 2  : defined by points 0 1 3     cylindrical face ( not plane )
//
// Look at the description of the class TFACE to understand these definitions of faces
//on the basis of points.
//
//
// For each shape of Litrani {tsbrik,tstrd1,tstrd2,tspara,tstrap,tsgtra,
//ts8pts,tscyl,tstube,tscone} there is an associated "friend" class
//TSupplShape, which contains everything which is common to all kind of
//shapes. This associated class may be reached from TSCYL (or from any
//other shape) like this : (supposing that pcyl is a pointer on a TSCYL)
//
//     TSupplShape *suppl = pcyl->fSuppl;
//
// All shapes in Litrani may at the same time be a detector of photon. For
//instance, a shape may represent a phototube or an APD. See the description
//how to define a shape as a detector in the presentation of the TSupplShape
//class. It is clear that TSCYL is the shape most suited to represent a
//phototube !
//
//  If FromOther is true, it means that the material of the shape and the
//optical axis are taken from an other shape. If this is the case, it means
//that in case of transition between this shape and the other shape having
//same material and same optical axis, the photon simply continues from one
//shape to the other, without any transition calculation.
//
TSCYL::TSCYL(const char *name,const char *title,const char *namemat,const char *namerev,
  Double_t r, Double_t dz,Bool_t FromOther):TTUBE(name,title,namemat,Float_t(0.0),
  Float_t(r),Float_t(dz)) {
  //constructor
  const Double_t zero = 0.0;
  TString nameshape;
  T3Vector *v;
  TFace *face;
  fLr = r;
  fLz = dz;
  if (FromOther) {
    nameshape = namemat;
    fSuppl = new TSupplShape(name,title,this,tscyl,nameshape,namerev);
  }
  else {
    fSuppl = new TSupplShape(name,title,this,tscyl,namemat,namerev);
  }
  fSuppl->fGCenter.Set(zero,zero,zero);
  fSuppl->fNbSubPoints = 0;
  fSuppl->fNbPoints = 6;
  fSuppl->fTotalNbPoints = fSuppl->fNbPoints;
  fSuppl->fPoints = new TObjArray();
  v = new T3Vector(zero,zero,-dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(   r,zero,-dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(zero,   r,-dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(zero,zero, dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(   r,zero, dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(zero,   r, dz);
  fSuppl->fPoints->Add(v);
  fSuppl->fNbMasterFaces = 3;
  fSuppl->fNbSubFaces = 0;
  face = new TFace(this,0,-1,circular,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,0,1,2);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,1,-1,circular,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,3,4,5);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,2,-1,cylindrical,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,0,1,3);
  fSuppl->fFaces.Add(face);
  fSuppl->fFaces.Sort();
}
TSCYL::~TSCYL() {
  //Destructor
  if (fSuppl) {
    delete fSuppl;
    fSuppl = 0;
  }
}
void TSCYL::SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const {
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
