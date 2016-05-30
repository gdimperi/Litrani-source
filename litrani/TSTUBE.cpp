// @(#)root/html:$Id: TSTUBE.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "T3Vector.h"
#include "TFace.h"
#include "TSupplShape.h"
#include "TSTUBE.h"

ClassImp(TSTUBE)
//______________________________________________________________________________
//
// TSTUBE inherits from TTUBE and adds supplementary information required for
//        Litrani, in particular the default revetment of all faces.
//
//   RECALL THAT ALL DIMENSIONS OF SHAPES ARE IN cm IN LITRANI !
//
// TSTUBE is a tube, with an inner and an outer cylindrical face. Its axis is the Oz axis in local
//coordinate. The center of the tube is the center of the coordinate system. Notice that a tube
//is not a convex volume. It is why the normal to the inner surface of face 3 has to be reversed.
//( fReverse = KTRUE).
//
//It has 6 parameters:
//
//    const char *name       name of the shape
//    const char *title      shape's title
//    const char *namemat    name of the optical material of the shape
//OR  const char *nameshape  name of the other shape having same material and
//    const char *namerev    name of the default revetment for the 6 faces
//    Double_t   rmin        inner radius of the tube in cm
//    Double_t   rmax        outer radius of the tube in cm
//    Double_t   dz          half-length of the tube along the z-axis in cm
//    Bool_t     FromOther   true if material and optical axis have to be
//                           taken from an other shape.
//
// We define arbitrarily 6 points to help the construction of the faces of the tube :
//
//  point[0] = (  0 ,   0, -dz)     center of the lower ring basis
//  point[1] = (rmax,   0, -dz)     one point on the lower basis large circle
//  point[2] = (   0,rmin, -dz)     one point on the lower basis small circle
//  point[3] = (   0,   0, +dz)     center of the upper ring basis
//  point[4] = (rmax,   0, +dz)     one point on the upper basis large circle
//  point[5] = (   0,rmin, +dz)     one point on the upper basis small circle
//
// The 4 faces of the TSTUBE are built from these points in the following way :
//
//  face 0  : defined by points 0 1 2     lower basis of type ring (plane )
//  face 1  : defined by points 3 4 5     upper basis of type ring (plane )
//  face 2  : defined by points 0 1 3     outer cylindrical face ( not plane )
//  face 3  : defined by points 0 2 3     inner cylindrical face ( not plane ) (normal reversed)
//
// Look at the description of the class TFACE to understand these definitions of faces
//on the basis of points.
//
// For each shape of Litrani {tsbrik,tstrd1,tstrd2,tspara,tstrap,tsgtra,
//ts8pts,tscyl,tstube,tscone} there is an associated "friend" class
//TSupplShape, which contains everything which is common to all kind of
//shapes. This associated class may be reached from TSTUBE (or from any
//other shape) like this : (supposing that ptube is a pointer on a TSTUBE)
//
//     TSupplShape *suppl = ptube->fSuppl;
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
TSTUBE::TSTUBE(const char *name,const char *title,const char *namemat,
  const char *namerev,Double_t rmin,Double_t rmax,Double_t dz,
  Bool_t FromOther):TTUBE(name,title,namemat,Float_t(rmin),Float_t(rmax),Float_t(dz)) {
  //constructor
  const Double_t zero = 0.0;
  TString nameshape;
  T3Vector *v;
  TFace *face;
  fLmin = rmin;
  fLmax = rmax;
  fLz   = dz;
  fSuppl = 0;
  if (FromOther) {
    nameshape = namemat;
    fSuppl = new TSupplShape(name,title,this,tstube,nameshape,namerev);
  }
  else {
    fSuppl = new TSupplShape(name,title,this,tstube,namemat,namerev);
  }
  fSuppl->fGCenter.Set(zero,zero,zero);
  fSuppl->fNbSubPoints = 0;
  fSuppl->fNbPoints = 6;
  fSuppl->fTotalNbPoints = fSuppl->fNbPoints;
  fSuppl->fPoints = new TObjArray();
  v = new T3Vector(zero,zero,-dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(rmax,zero,-dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(zero,rmin,-dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(zero,zero, dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(rmax,zero, dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(zero,rmin, dz);
  fSuppl->fPoints->Add(v);
  fSuppl->fNbMasterFaces = 4;
  fSuppl->fNbSubFaces = 0;
  face = new TFace(this,0,-1,ring,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,0,1,2);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,1,-1,ring,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,3,4,5);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,2,-1,cylindrical,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,0,1,3);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,3,-1,cylindrical,fSuppl->fPoints,fSuppl->fGCenter,fSuppl->fDefRev,0,2,3,kTRUE);
  fSuppl->fFaces.Add(face);
  fSuppl->fFaces.Sort();
}
TSTUBE::~TSTUBE() {
  //Destructor
  if (fSuppl) {
    delete fSuppl;
    fSuppl = 0;
  }
}
void TSTUBE::SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const {
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
