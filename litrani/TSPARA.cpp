// @(#)root/html:$Id: TSPARA.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "T3Vector.h"
#include "TMessErr.h"
#include "TFace.h"
#include "TSupplShape.h"
#include "TSPARA.h"

ClassImp(TSPARA)
//______________________________________________________________________________
//
// TSPARA inherits from TPARA and adds supplementary information required for
//        Litrani, in particular the default revetment of all faces.
//
//   RECALL THAT ALL DIMENSIONS OF SHAPES ARE IN cm IN LITRANI !
//
// TSPARA is a parallelepiped. It has 11 parameters:
//
//     const char *name       name of the shape
//     const char *title      shape's title
//     const char *namemat    optical material of the shape
//OR   const char *nameshape  name of the other shape having same material and
//                            optical axis
//     const char *namerev    name of the default revetment for the 6 faces
//     Double_t   dx          half-length in x, or -x coord of point 0 in cm
//     Double_t   dy          half-length in y, or -y coord of point 0 in cm
//     Double_t   dz          half-length in z, or -z coord of point 0 in cm
//     Double_t   alpha or x2 angle formed by the y axis and by the plane joining the centre of the
//                            faces parallel to the z-x plane at -DY and +DY. Or x coord. of point 2
//                            if bypoints true.
//     Double_t   theta or x4 polar angle of the line joining the centres of the faces at -DZ and +DZ
//                            in z. Or x coordinate of point 4 if bypoints true.
//     Double_t   phi   or y4 azimuthal angle of the line joining the centres of the faces at -DZ and
//                            +DZ in z. Or y coord. of point 4 if bypoints true.
//                            ALL ANGLES IN DEGREE !!
//     Bool_t     bypoints    if true, x2, x4, y4 are given instead of alpha, theta, phi !
//     Bool_t     FromOther   true if material and optical axis have to be
//                            taken from an other shape.
//
//  For those for which these definitions are not very clear, here is the definition of the 8 points
//of the parallellepiped from dx, dy, dz, alpha, theta and phi :
//
//  dxx = 2*dz*TMath::Sin(theta)*TMath::Cos(phi);
//  dyy = 2*dz*TMath::Sin(theta)*TMath::Sin(phi);
//  ddx = 2*dy*TMath::Tan(alpha);
//  ddy = 2*dy*(1.0/TMath::Cos(alpha) - 1);
//
//  point[0] = ( -dx,         -dy,         -dz )
//  point[1] = (  dx,         -dy,         -dz )
//  point[2] = (  dx+ddx,      dy+ddy,     -dz )
//  point[3] = ( -dx+ddx,      dy+ddy,     -dz)
//  point[4] = ( -dx+dxx,     -dy+dyy,      dz )
//  point[5] = (  dx+dxx,     -dy+dyy,      dz )
//  point[6] = (  dx+dxx+ddx,  dy+dyy+ddy,  dz )
//  point[7] = ( -dx+dxx+ddx,  dy+dyy+ddy,  dz )
//
//    The 6 faces of the TSPARA are built from these points in the following way :
//
//  face 0  : defined by points 0 1 5 4
//  face 1  : defined by points 1 2 6 5
//  face 2  : defined by points 2 3 7 6
//  face 3  : defined by points 3 0 4 7
//  face 4  : defined by points 1 0 3 2
//  face 5  : defined by points 4 5 6 7
//
//
//  You can see that giving :
//
//              - point 0  minus the 3 coord : -x0, -y0, -z0
//              - point 2  the x coordinnate : x2 
//              - point 4  the x and y coord : x4, y4
//
//is equivalent to giving dx, dy,dz, alpha, theta, phi. If you prefer this second way of defining
//a parallelepiped, put bypoints to kTRUE in the constructor.
//
//
// For each shape of Litrani {tsbrik,tstrd1,tstrd2,tspara,tstrap,tsgtra,
//ts8pts,tscyl,tstube,tscone} there is an associated "friend" class
//TSupplShape, which contains everything which is common to all kind of
//shapes. This associated class may be reached from TSPARA (or from any
//other shape) like this : (supposing that ppara is a pointer on a TSPARA)
//
//     TSupplShape *suppl = ppara->fSuppl;
//
// All shapes in Litrani may at the same time be a detector of photon. For
//instance, a shape may represent a phototube or an APD. See the description
//how to define a shape as a detector in the presentation of the TSupplShape
//class.
//
TSPARA::TSPARA(const char *name,const char *title,const char *namemat,
  const char *namerev,Double_t dx,Double_t dy,Double_t dz,Double_t alpha,
  Double_t theta,Double_t phi, Bool_t bypoints, Bool_t FromOther):TPARA(name,
  title,namemat,Float_t(dx),Float_t(dy),Float_t(dz),Float_t(alpha),Float_t(theta),
  Float_t(phi)) {
  //constructor
  const Double_t pi    = TMath::Pi();
  const Double_t pideg = 180.0;
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  const Double_t deux  = 2.0;
  const Double_t small = 1.0e-20;
  TString nameshape;
  Double_t x2,x4,y4,ddx,ddy,dxx,dyy;
  Double_t d2,sc,ss;
  T3Vector *v;
  TFace *face;
  fLx = dx;
  fLy = dy;
  fLz = dz;
  if (bypoints) {
    x2  = alpha;
    x4  = theta;
    y4  = phi;
    ddx = x2 - dx;
    dxx = x4 + dx;
    dyy = y4 - dy;
    d2  = deux*dz;
    sc  = dxx/d2;
    ss  = dyy/d2;
    d2  = sc*sc + ss*ss;
    if (d2>un) {
      gMes->SetName("TSPARA");
      gMes->SetTitle("TSPARA");
      gMes->M(kill_prog,1,"point 4 is wrong");
    }
    else {
      d2 = TMath::Sqrt(d2);
      theta = TLitPhys::ASin(d2);
      if (d2<small) {
        theta = zero;
        phi = zero;
      }
      else {
        phi   = TLitPhys::ASin(TMath::Abs(ss/d2));
        if (ss>=zero) {
          if (sc<zero) phi = pi - phi;
        }
        else {
          if (sc>=zero) {
            phi = - phi;
          }
          else {
            phi = phi - pi;
          }
        }
      }
    }
    d2 = ddx/(deux*dy);
    alpha  = TMath::ATan(d2);
    fAlpha = Float_t(alpha);
    fTheta = Float_t(theta);
    fPhi   = Float_t(phi);
  }
  else {
    Double_t torad = pi/pideg;
    alpha *= torad;
    theta *= torad;
    phi   *= torad;
    ss  = TMath::Sin(theta);
    d2  = deux*dz*ss;
    dxx = d2*TMath::Cos(phi);
    dyy = d2*TMath::Sin(phi);
    ddx = deux*dy*TMath::Tan(alpha);
  }
  ddy = deux*dy*(un/TMath::Cos(alpha) - un);
  fLAlpha = alpha;
  fLTheta = theta;
  fLPhi   = phi;
  if (FromOther) {
    nameshape = namemat;
    fSuppl = new TSupplShape(name,title,this,tspara,nameshape,namerev);
  }
  else {
    fSuppl = new TSupplShape(name,title,this,tspara,namemat,namerev);
  }
  fSuppl->fGCenter.Set(zero,zero,zero);
  fSuppl->fNbSubPoints = 0;
  fSuppl->fNbPoints = 8;
  fSuppl->fTotalNbPoints = fSuppl->fNbPoints;
  fSuppl->fPoints = new TObjArray();
  v = new T3Vector(-dx,-dy,-dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector( dx,-dy,-dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector( dx+ddx, dy+ddy,-dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(-dx+ddx, dy+ddy,-dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(-dx+dxx,-dy+dyy, dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector( dx+dxx,-dy+dyy, dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector( dx+dxx+ddx, dy+dyy+ddy, dz);
  fSuppl->fPoints->Add(v);
  v = new T3Vector(-dx+dxx+ddx, dy+dyy+ddy, dz);
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
TSPARA::~TSPARA() {
  //Destructor
  if (fSuppl) {
    delete fSuppl;
    fSuppl = 0;
  }
}
void TSPARA::SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const {
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
