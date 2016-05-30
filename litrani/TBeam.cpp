// @(#)root/html:$Id: TBeam.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TROOT.h"
#include "TLitPhys.h"
#include "TMessErr.h"
#include "TOpticMaterial.h"
#include "TSNode.h"
#include "TSupplShape.h"
#include "TEqIndex.h"
#include "TPhoton.h"
#include "TSParticle.h"
#include "TResults.h"
#include "TPublication.h"
#include "TLitGlob.h"
#include "TBeam.h"

ClassImp(TBeam)
//______________________________________________________________________________
//
//  TBeam defines a beam of particles. When asked to generate a particle,
//it determines start position and direction of emission of the particle.
//Every positions and directions are given in the Local Coordinate System of
//a shape which the user is free to select among all shapes of his setup.
//This is much easier than asking the user to define all positions and
//directions in the World Coordinate System, independently to any reference
//to a shape. The fact that it is asked to the user to give positions and
//directions with respect to a shape does not at all mean that the beam has
//to be emitted from within the reference shape, although it is not forbid-
//den. Notice however that it is forbidden to emit the beam from within a
//shape which emits light when crossed by particles! So if your reference
//shape is made of a material for which you have called the method
//TOpticMaterial::dEdx(), DO NOT place the beam cradle inside this shape
//nor on one of its face, but strictly outside. In general, the surface of
//emission of the beam will be outside the reference shape.
//
//  The start position may be, according to fKindCdl :
//
//  point          --> at a given point fCenter
//  disk           --> on a given disk, centered at fCenter,
//                       with a radius fA1, normal to fAxis3.
//  ellipse        --> on a given ellipse, centered at fCenter. The axis
//                       of the ellipse are along fAxis1 and fAxis2 and
//                       have half-lengths fA1 and fA2.
//  parallelogram  --> on a given parallelogram, centered at fCenter,
//                       normal to fAxis3,
//                       with half-length of 1st side of +-fA1 along fAxis1
//                       with half-length of 2nd side of +-fA2 along fAxis2
//  parallelipiped --> forbidden ! A beam cannot issue from a volume
//
//  fAxis2 and fAxis1 MAY not be orthogonal. fAxis3 is always orthogonal to
//fAxis1 and fAxis2.
//  Notice that before defining a beam, you have to define the particles
//constituting the beam. See the class TSParticle.
//  Use the first version of the constructor for the simplest case where the
//cradle is a point. Use the 2nd version of the constructor when fAxis1 is
//orthogonal to fAxis2, i.e. for disk, ellipse or parallelogram if the paral-
//lelogram is a rectangle (so that fAxis2 is orthogonal to fAxis1 and fAxis1,
//fAxis2 and fAxis3 build an orthonormal system). Use the 3rd version of the
//constructor in the most general case when fAxis1 is not necessarily
//orthogonal to fAxis2.
//
//  Notice that the direction of emission of the beam of particles may have
//the following shapes, according to the value of fKindDist : 
//( t == angle theta, f == angle phi )
//
//  on4pi          --> the direction is generated isotropically on 4 pi
//                with the distribution sin(t)dtdf.
//  flat           --> the direction is generated with t <= fAperture,
//                with the distribution sin(t)dtdf around the direction
//                given by the vector fAxisb3.
//  sinuscosinus   --> the direction is generated with t <= fAperture,
//                with the distribution sin(t)cos(t)dtdf around the
//                direction given by the vector fAxisb3. (With an extra
//                cos(t) factor to slightly favour the forward direction
//                with respect to a flat distribution sin(t)dtdf.) 
//  provided       --> the direction is generated according to a distribution
//                given by the user under the form of a fit or interpolation
//                of type TSplineFit around the direction given by the vector
//                fAxisb3.
//
//  In the case you provide yourself the distribution fDistName, please be
//careful about the following non trivial issues :
//    (0) - To know how to define yourself a distribution of type TSplineFit
//to be used to generate random numbers according to a distribution, please
//read the documentation about TSplineFit and in particular the documentation
//about the 2 methods TSplineFit::UseForRandom() and TSplineFit::GetRandom().
//You have to call UseForRandom() !
//    (1) - your distribution has to generate theta in radians, not degrees.
//    (2) - you have to give a 2-dimensional distribution, not a 1-dimensional
//distribution. If you have measured yourself the distribution, your measurement
//was very likely a 1-dimensional distribution, (you have displaced your instru-
//ment inside a plane and you have assumed isotropy in phi) so you have to
//multiply each measurement by sin(t).
//    (3) - Even if you have provided yourself the distribution, the value for
//fAperture still matters : if a t angle is generated from your distribution
//which is bigger than fAperture, then we start again until a value smaller
//than fAperture is found.
//
//  The characteristics of angular distribution of the beam are not given in
//the constructor of TBeam. They have to be provided by a call to Beam(). If
//you do not call Beam(), default characteristics for the beam will be used
//which are the following :
//
//            fKindDist  = flat
//            fAperture  = 10°
//            fAxisb3    = (0,0,-1) in LCS of choosen shape
//
//  Notice that there are 2 instances of the Beam() method, depending whether
//the axis of the beam, fAxisb3, is parallel or not to fAxis3, i.e. whether
//the axis of the beam is orthogonal or not to the generating surface.
//
TBeam::TBeam(const char *name,const char *title,TSParticle *part,const char *shname,
  T3Vector &center): TNamed(name,title) {
  //  Constructor in the case one wants that the beam originates from a
  //point. The center of emission "center" is given in the Local Coordinate
  //System of a shape named "shname" which the user is free to select among
  //all shapes of his setup. This is much easier than asking the user to
  //define all positions and directions in the World Coordinate System,
  //independently to any reference to a shape. The fact that it is asked to
  //the user to give positions and directions with respect to a shape does not
  //at all mean that the beam has to be emitted from within the reference
  //shape (although it is not forbidden). In general, the surface of emission
  //of the beam will be outside the reference shape. 
  //
  //
  //  The parameters are the following :
  //
  //  name   : name of this beam of particles.
  //  title  : title of this beam of particles.
  //  part   : particle constituting the beam, with its energy distribution.
  //            see class TSParticle.
  //  shname : name of the shape chosen as reference shape. The coordinates
  //            of "center" have to be provided in the Local Coordinate
  //            System of this shape.
  //  center : point of emission of the beam in LCS of the shape "shname".
  //
  //  The characteristics of angular distribution of the beam are not given
  //in the constructor of TBeam. They have to be provided by a call to Beam().
  //If you do not call Beam(), default characteristics for the angular
  //distribution of the beam will be used. which are the following, IN THE
  //LOCAL COORDINATE OF THE SHAPE "shname".
  //
  //            fKindDist  = flat;
  //            fAperture  = 10°;
  //            fAxisb3    = (0,0,-1) in LCS of choosen shape
  //
  //  The default beam is dropping vertically and has an aperture of 10°
  //
  InitP();
  if (part==0) {
    gMes->SetName("TBeam");
    gMes->SetTitle("TBeam");
    gMes->M(kill_prog,0,"Define particle before defining beam");
  }
  fParticle  = part;
  TString s1 = shname;
  FindShape(s1);
  DefaultInit(center);
  AddToList(name);
}
TBeam::TBeam(const char *name,const char *title,TSParticle *part,const char *shname,
  T3Vector &center,KindOfCradle kind,Double_t a1,Double_t a2,Double_t theta3,
  Double_t phi3,Double_t theta1,Double_t phi1):TNamed(name,title) {
  //
  //  Constructor in the general case, BUT with the restriction that (fAxis1,
  //fAxis2 and fAxis3 build an orthonormal system, which is always the case,
  //except when kind == parallelogram and the parallelogram is not a
  //rectangle ! So use this constructor if the emitting beam surface is a disk
  //or an ellipse or a rectangle.
  //
  //  Every positions and directions are given in the Local Coordinate System
  //of a shape named "shname" which the user is free to select among all
  //shapes of his setup. This is much easier than asking the user to define
  //all positions and directions in the World Coordinate System, independently
  //to any reference to a shape. The fact that the user is asked to give
  //positions and directions with respect to a shape does not at all mean that
  //the beam has to be emitted from within the chosen shape (although it is
  //not forbidden). In general, the surface of emission of the beam will be
  //outside the chosen shape. All coordinates or directions given by the
  //user will be transformed into World Coordinate.
  //
  //  The parameters are the following :
  //
  //  name   : name of this beam of particles.
  //  title  : title of this beam of particles.
  //  part   : particle constituting the beam, with its energy distribution.
  //              see class TSParticle.
  //  shname : name of the shape chosen as reference shape. All coordinates
  //            and directions have to be provided in the Local Coordinate
  //            System of this shape.
  //  center : center of the emitting surface of the beam in LCS of the
  //              shape "shname".
  //  kind   : kind of the emitting zone :
  //            {point,disk,ellipse,parallelogram}. parallelipiped forbidden.
  //            if {point}, the use of the preceeding constructor would have
  //            been simpler.
  //  a1     : - radius of disk or
  //           - half-length of 1st axis of ellipse  along fAxis1 or 
  //           - half-length of rectangle along fAxis1.
  //            This parameter is irrelevant in case kind == {point}
  //  a2     : - half-length of 2nd axis of ellipse along fAxis2 or 
  //           - half-length of 2nd side of rectangle along fAxis2.
  //            This parameter is irrelevant in case kind == {point,disk}
  //            Default 0
  //  theta3 : angle theta (degree, in LCS) of the axis fAxis3, normal to
  //            disk, ellipse or rectangle. Default 180.
  //  phi3   : angle phi   (degree, in LCS) of the axis fAxis3, normal to
  //            disk, ellipse or rectangle. Default 0.
  //            If you do not give these 2 parameters, fAxis3 will be along
  //             -Oz in LCS of shape "shname".
  //            These 2 parameters are irrelevant in case kind == point and
  //            you call Beam() afterwards. But if you do not call Beam()
  //            afterwards, the axis of emission of the beam, fAxisb3 will
  //            be set equal to fAxis3.
  //  theta1 : angle theta (degree, in LCS) of the axis fAxis1, 1st axis of
  //            ellipse or 1st axis of rectangle. Default 90.
  //  phi1   : angle phi   (degree, in LCS) of the axis fAxis1, 1st axis of
  //            ellipse or 1st axis of rectangle. Default 180.
  //            If you do not give these 2 parameters, fAxis1 will be along
  //            -Ox in LCS. These 2 parameters are irrelevant in case
  //            kind == point or disk.
  //            Not giving the 4 last parameters will result in the following
  //            in the LCS of shape "shname" :
  //            fAxis1 = -Ox   fAxis2 = +Oy   fAxis3 = -Oz
  //
  //  Notice that fAxis2 needs not be given. It can be obtained from fAxis1
  //and fAxis3, because it is assumed in this constructor that fAxis1, fAxis2
  //and fAxis3 build an orthonormal system.
  //
  //  The characteristics of angular distribution of the beam are not given
  //in the constructor of TBeam. They have to be provided by a call to Beam().
  //If you do not call Beam(), default characteristics for the angular
  //distribution of the beam will be used. which are the following :
  //
  //            fKindDist  = flat;
  //            fAperture  = 10°;
  //            fAxisb3    = (0,0,-1) in LCS of choosen shape
  //
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  const Double_t small = 1.0e-3;
  const Double_t torad = TMath::Pi()/180.0;
  T3Vector v,axis1,axis2,axis3;
  Double_t s,c,sf,cf,test;
  InitP();
  if (part==0) {
    gMes->SetName("TBeam");
    gMes->SetTitle("TBeam");
    gMes->M(kill_prog,0,"Define particle before defining beam");
  }
  fParticle  = part;
  TString s1 = shname;
  FindShape(s1);
  DefaultInit(center);
  theta3 *= torad;
  phi3   *= torad;
  theta1 *= torad;
  phi1   *= torad;
  fA1 = TMath::Abs(a1);
  fA2 = TMath::Abs(a2);
  fKindCdl = kind;
  s  = TMath::Sin(theta3);
  c  = TMath::Cos(theta3);
  sf = TMath::Sin(phi3);
  cf = TMath::Cos(phi3);
  axis3.Set(s*cf,s*sf,c);
  fSNode->AxisToWorld(axis3,fAxis3);
  fAxis3.Unit();
  switch (kind) {
  case point:
  case disk:
    //Finds dummy fAxis1 and fAxis2
    Short_t k;
    k = fAxis3.Smallest();
    switch (k) {
  case 0:
    v.Set(un,zero,zero);
    break;
  case 1:
    v.Set(zero,un,zero);
    break;
  case 2:
    v.Set(zero,zero,un);
    break;
    }
    fAxis1 = v^fAxisb3;
    fAxis1.Unit();
    fAxis2 = fAxis3^fAxis1;
    fAxis2.Unit();
    break;
  case ellipse:
  case parallelogram:
    s  = TMath::Sin(theta1);
    c  = TMath::Cos(theta1);
    sf = TMath::Sin(phi1);
    cf = TMath::Cos(phi1);
    axis1.Set(s*cf,s*sf,c);
    test = TMath::Abs(axis1*axis3);
    if (test>small) {
      gMes->SetName("TBeam");
      gMes->SetTitle("TBeam");
      gMes->M(Error_M,2,"axis1 not orthogonal to axis3");
      gMes->M(Error_M,2,"axis1 changed to be orthogonal to axis3");
    }
    axis1.MinOrthoN(axis3,s,c);
    fSNode->AxisToWorld(axis1,fAxis1);
    fAxis1.Unit();
    fAxis2 = fAxis3^fAxis1;
    break;
  case parallelipiped:
    gMes->SetName("FindShape");
    gMes->SetTitle("TBeam");
    gMes->M(kill_prog,3,"Parallelipiped not valid as beam source");
    break;
  }
  //We have to provide default values for the 3 beam axis
  fAxisb1 = fAxis1;
  fAxisb2 = fAxis2;
  fAxisb3 = fAxis3;
  AddToList(name);
}
TBeam::TBeam(const char *name,const char *title,TSParticle *part,T3Vector &center,
  const char *shname,KindOfCradle kind,Double_t a1,Double_t a2,Double_t theta1,
  Double_t phi1,Double_t theta2,Double_t phi2): TNamed(name,title) {
  //  Constructor in the MOST general case, without any restriction. fAxis1
  //and fAxis2 are not necessarily orthogonal ! This constructor can be used
  //for any situation ( kind == {point,disk,ellipse,parallelogram} ), but if
  //kind == point, it is much easier to use the 1st constructor, and if
  //kind = {disk,ellipse} or kind == parallelogram, the parallelogram being
  //a rectangle, it is easier to use the 2nd constructor. So, use this
  //constructor only if kind == parallelogram, and the parallelogram is not
  //rectangle.
  //
  //  Every positions and directions are given in the Local Coordinate System
  //of a shape named "shname" which the user is free to select among all
  //shapes of his setup. This is much easier than asking the user to define
  //all positions and directions in the World Coordinate System, independently
  //to any reference to a shape. The fact that the user is asked to give
  //positions and directions with respect to a shape does not at all mean that
  //the beam has to be emitted from within the chosen shape (although it is
  //not forbidden). In general, the surface of emission of the beam will be
  //outside the chosen shape. All coordinates or directions given by the
  //user will be transformed into World Coordinate.
  //
  //  The parameters are the following :
  //
  //  name   : name of this beam of particles.
  //  title  : title of this beam of particles.
  //  part   : particle constituting the beam, with its energy distribution.
  //            see class TSParticle.
  //  center : center of the emitting surface of the beam in LCS of the
  //            shape "shname".
  //  shname : name of the shape chosen as reference shape. All coordinates
  //            and directions have to be provided in the Local Coordinate
  //            System of this shape.
  //  kind   : kind of the emitting zone :
  //            {point,disk,ellipse,parallelogram}. parallelipiped forbidden.
  //            if {point}, the use of the preceeding constructor would have
  //            been simpler.
  //  a1     : - radius of disk or
  //           - half-length of 1st axis of ellipse  along fAxis1 or 
  //           - half-length of rectangle along fAxis1.
  //            This parameter is irrelevant in case kind == {point}
  //  a2     : - half-length of 2nd axis of ellipse along fAxis2 or 
  //           - half-length of 2nd side of rectangle along fAxis2.
  //            This parameter is irrelevant in case kind == {point,disk}
  //            Default 0
  //  theta1 : angle theta (degree, in LCS) of the axis fAxis1, 1st axis of
  //            ellipse or 1st axis of rectangle. Default 90.
  //  phi1   : angle phi   (degree, in LCS) of the axis fAxis1, 1st axis of
  //            ellipse or 1st axis of rectangle. Default 0.
  //            (if you do not give these 2 parameters, fAxis1 will be along
  //            +Ox in LCS).(These 2 parameters are irrelevant in case
  //            kind == point or disk)
  //  theta2 : angle theta (degree, in LCS) of the axis fAxis2, 2nd axis of
  //            ellipse or 2nd axis of parallelogram. Default 90.
  //  phi2   : angle phi   (degree, in LCS) of the axis fAxis2, 2nd axis of
  //            ellipse or 2nd axis of parallelogram. Default 90.
  //            (if you do not give these 2 parameters, fAxis2 will be along
  //            +Oy in LCS).(These 2 parameters are irrelevant in case
  //            kind == point,disk,ellipse)
  //
  //  Notice the inversion of the parameters "center" and "shname", as
  //compared with the preceeding constructor : this inversion is there only to
  //allow the compiler not to confuse the two ! Notice also that it is not
  //necessary to give theta3, phi3 of fAxis3, because fAxis3 can be obtained
  //from fAxis1 and fAxis2 by fAxis3 = fAxis1^fAxis2 ! But this implies that
  //even in case of {point, disk, ellipse } you carefully define fAxis1 AND
  //fAxis2 so that fAxis3 be correctly calculated. This shows once more that
  //this constructor is best suited to define parallelograms and that you
  //should use the other constructors for the other cases.
  //
  //  The characteristics of angular distribution of the beam are not given
  //in the constructor of TBeam. They have to be provided by a call to Beam().
  //If you do not call Beam(), default characteristics for the angular
  //distribution of the beam will be used. which are the following :
  //
  //            fKindDist  = flat;
  //            fAperture  = 10°;
  //            fAxisb3    = (0,0,-1) in LCS of choosen shape
  //
  const Double_t small = 1.0e-3;
  const Double_t torad = TMath::Pi()/180.0;
  T3Vector v,axis1,axis2,axis3;
  Double_t s,c,sf,cf,test;
  InitP();
  if (part==0) {
    gMes->SetName("TBeam");
    gMes->SetTitle("TBeam");
    gMes->M(kill_prog,0,"Define particle before defining beam");
  }
  fParticle  = part;
  TString s1 = shname;
  FindShape(s1);
  DefaultInit(center);
  theta1 *= torad;
  phi1   *= torad;
  theta2 *= torad;
  phi2   *= torad;
  fA1 = TMath::Abs(a1);
  fA2 = TMath::Abs(a2);
  fKindCdl = kind;
  s  = TMath::Sin(theta1);
  c  = TMath::Cos(theta1);
  sf = TMath::Sin(phi1);
  cf = TMath::Cos(phi1);
  axis1.Set(s*cf,s*sf,c);
  s  = TMath::Sin(theta2);
  c  = TMath::Cos(theta2);
  sf = TMath::Sin(phi2);
  cf = TMath::Cos(phi2);
  axis2.Set(s*cf,s*sf,c);
  switch (kind) {
  case point:
  case disk:
  case ellipse:
    test = TMath::Abs(axis1*axis2);
    if (test>small) {
      gMes->SetName("TBeam");
      gMes->SetTitle("TBeam");
      gMes->M(Error_M,5,"axis2 not orthogonal to axis1");
      gMes->M(Error_M,5,"axis2 changed to be orthogonal to axis1");
    }
    axis2.MinOrthoN(axis1,s,c);
    break;
  case parallelogram:
    break;
  case parallelipiped:
    gMes->SetName("TBeam");
    gMes->SetTitle("TBeam");
    gMes->M(kill_prog,6,"Parallelipiped forbidden as beam cradle");
    break;
  }
  fSNode->AxisToWorld(axis1,fAxis1);
  fAxis1.Unit();
  fSNode->AxisToWorld(axis2,fAxis2);
  fAxis2.Unit();
  fAxis3 = fAxis1^fAxis2;
  fAxis3.Unit();
  //We have to provide default values for the 3 beam axis
  fAxisb3 = fAxis3;
  fAxisb1 = fAxis1;
  fAxisb2 = fAxis3^fAxis1;
  AddToList(name);
}
TBeam::~TBeam() {
  // Destructor
  // TBeam is not owner of fParticle, fSuppl, fSNode, fFitDist, fSup, fMat. New for
  //these pointers have not been done inside TBeam, and TBeam is not allowed to
  //delete elements of the geometry. So these pointers are not deleted by the
  //destructor of TBeam.
  if (fPhot) {
    delete fPhot;
    fPhot = 0;
  }
  gLit->fBeam.Remove(this);
  gLit->fNBeam--;
}
Bool_t TBeam::AddToList() const {
  //record the new Beam into gLit->fBeam
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TBeam");
    gMes->M(kill_prog,1,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneBeam(this,GetName());
}
Bool_t TBeam::AddToList(const char *name) const {
  //record the new Beam into gLit->fBeam
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TBeam");
    gMes->M(kill_prog,2,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneBeam(this,name);
}
void TBeam::AxisBeam() {
  //Complement fAxisb3 with fAxisb1,2 in order to have a full orthonormal
  //coordinate system.
  Double_t s,c;
  s = TMath::Abs(fAxis1*fAxisb3);
  c = TMath::Abs(fAxis2*fAxisb3);
  if (s<=c) {
    fAxisb1 = fAxis1;
    fAxisb1.MinOrthoN(fAxisb3,s,c);
    fAxisb2 = fAxisb3^fAxisb1;
  }
  else {
    fAxisb2 = fAxis2;
    fAxisb2.MinOrthoN(fAxisb3,s,c);
    fAxisb1 = fAxisb2^fAxisb3;
  }
}
void TBeam::Beam(KindOfDist kind, Double_t aper,Double_t theta,Double_t phi,
  const char *distrib) {
  //Defines the characteristics of direction of emission of the beam.
  //This instance of Beam() is the one to use when the axis of the beam,
  //fAxisb3, is not equal to fAxis3, i.e. when the axis of the beam is not
  //orthogonal to the generating surface (which is perfectly allowed), or
  //when there is no generating surface (point).
  //
  //  Every positions and directions are given in the Local Coordinate System
  //of the shape named "shname" given in the constructor. All coordinates or
  //directions given by the user will be transformed into World Coordinate.
  //
  //
  // Parameters :
  //
  //  kind     : {on4pi,flat,sinuscosinus,provided} See the meaning of these options
  //              in the class description.
  //  aper     : aperture of the beam, in degree, around fAxisb3.
  //  theta    : theta angle, in degree, in LCS of fSuppl, of fAxisb3.
  //  phi      : phi angle, in degree, in LCS of fSuppl, of fAxisb3.
  //  distrib  : name of the fit or interpolation of type TSplineFit giving
  //             the distribution for the theta angle of the generated
  //             photons around fAxisb3 in case kind == provided.
  //              (default "")
  // Notice the following restriction for (theta,phi) of fAxisb3 : fAxisb3
  //must not lie inside the plane of the generating surface.
  //
  const Double_t torad = TMath::Pi()/180.0;
  const Double_t small = 1.0e-6;
  Double_t st,ct,sf,cf,s;
  fKindDist = kind;
  if (fKindDist==provided) FindDist(distrib);
  fAperture = torad*TMath::Abs(aper);
  fApersin  = TMath::Sin(fAperture);
  fApercos  = TMath::Cos(fAperture);
  theta *= torad;
  phi   *= torad;
  st = TMath::Sin(theta);
  ct = TMath::Cos(theta);
  sf = TMath::Sin(phi);
  cf = TMath::Cos(phi);
  T3Vector axis(st*cf,st*sf,ct);
  fSNode->AxisToWorld(axis,fAxisb3);
  fAxisb3.Unit();
  axis = fAxis1^fAxis2;
  axis.Unit();
  s    = TMath::Abs(axis*fAxisb3);
  if (s<small) {
    gMes->SetName("Beam");
    gMes->SetTitle("TBeam");
    gMes->M(Warning_M,1,"fAxisb3 inside generating surface");
    gMes->M(Warning_M,1,"We change it a little bit");
    fAxisb3 = fAxisb3 + small*axis;
  }
  AxisBeam();
}
void TBeam::Beam(KindOfDist kind, Double_t aper,const char *distrib) {
  //Defines the characteristics of direction of emission of the beam 
  //This instance of Beam() is the one to use when the axis of the beam,
  //fAxisb3, is identical to fAxis3, i.e. when the axis of the beam is
  //orthogonal to the generating surface.
  //
  // Parameters
  //
  //  kind     : {on4pi,flat,sinuscosinus,provided}. See the meaning of these options
  //              in the class description.
  //  aper     : aperture of the beam, in degree, around fAxis3.
  //  distrib  : name of the fit or interpolation of type TSplineFit giving
  //             the distribution for the theta angle of the generated
  //             photons around fAxisb3 in case kind == provided.
  //              (default "")
  //
  const Double_t torad = TMath::Pi()/180.0;
  fKindDist = kind;
  if (fKindDist==provided) FindDist(distrib);
  fAperture = torad*TMath::Abs(aper);
  fApersin  = TMath::Sin(fAperture);
  fApercos  = TMath::Cos(fAperture);
  fAxisb3 = fAxis3;
  AxisBeam();
}
void TBeam::DefaultInit(T3Vector &center) {
  //Default initialization
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  const Double_t DefAperture = 10.0;
  const Double_t torad = TMath::Pi()/180.0;
  T3Vector axis1,axis2,axis3;
  T3Vector v0(zero,zero,zero);
  fEmitCerenkov = kTRUE;
  fSNode->LocalToWorld(center,fCenter);
  fKindCdl   = point;
  fA1 = zero; fA2 = zero;
  fKindDist  = flat;
  fDistName  = "";
  fAperture  = torad*DefAperture;
  fApersin  = TMath::Sin(fAperture);
  fApercos  = TMath::Cos(fAperture);
  axis1.Set(un,zero,zero);
  axis2.Set(zero,un,zero);
  axis3.Set(zero,zero,un);
  fSNode->AxisToWorld(axis1,fAxis1);
  fSNode->AxisToWorld(axis2,fAxis2);
  fSNode->AxisToWorld(axis3,fAxis3);
  fAxisb1 = fAxis1;
  fAxisb2 = v0 - fAxis2;
  fAxisb3 = v0 - fAxis3;
  if (!gGs)
    gGs = new TResults("GlobStat","Global statistics of all runs",0);
  if (!gIdx) {
    gIdx = new TEqIndex("TBeam");
    if (gMes->TestCalc()) gIdx->SetDebug();
  }
}
void TBeam::EmitLight(Double_t &t1,T3Vector &x1,Double_t &t2,T3Vector &x2) {
  //  Emits light, by step, between x1 and x2. First emit light due to dE/dx
  //then emit light due to Cerenkov radiation
  const Int_t ntimesmax = 100;
  const Double_t dix    = 10.0;
  const Double_t eps    = 1.0e-6;
  Bool_t prt;
  Int_t i,j,nstep,bin;
  Double_t x,step;
  Int_t ntimes;
  Int_t kfluo;
  Axis_t afluo;
  T3Vector K;        //k vector of generated photon
  T3Vector E;        //direction of the E field of the photon
  Double_t tstart;   //distance in [cm] between generation of particle and generation of photon
  Bool_t stepok;     //fStep is correctly adjusted
  Int_t nphotdEdx;   //number of photons due to dEdx (integer !)
  Int_t nphotCer;    //number of photons due to Cerenkov radiation
  Double_t DeltaE;   //energy deposited in step [Mev]
  Double_t PhotMev;  //number of photons generated per Mev of deposited energy
  Double_t PhotdEdx; //number of photons due to dEdx (real !)
  Double_t reliquat; //left-over energy due to discretization of the number of photons
  stepok = kFALSE;
  ntimes = 0;
  Bool_t notyet = kTRUE;
  while ((ntimes<ntimesmax) && (!stepok)) {
    ntimes += 1;
    nstep = Int_t((t2-t1)/fStep);
    nstep += 1;
    for (i=1;i<=nstep;i++) {
      if (i==nstep) step = t2 - t1 - (nstep-1)*fStep;
      else step = fStep;
      //generation of photons due to dE/dx
      DeltaE    = fMat->GetdEdx(fEnergy);
      PhotMev   = fMat->GetPhotMev();
      PhotdEdx  = step*DeltaE*PhotMev;
      nphotdEdx = Int_t(PhotdEdx);
      stepok    = ((nphotdEdx>1) || (PhotMev<eps) || (i>1) || 
        (ntimes>=ntimesmax));
      if ((!stepok) && (TMath::Abs(fStep-step)<eps)) {
        fStep *= dix;
        break;
      }
      else stepok = kTRUE;
      reliquat = PhotdEdx - nphotdEdx;
      x = gRandom3->Rndm();
      if (reliquat>=x) nphotdEdx += 1;
      if ((gLit->fNCallEL==50) && (notyet)) {
        gMes->SetName("EmitLight");
        gMes->SetTitle("TBeam");
        gMes->M(Info_M,1,"Switch to almost quiet working");
        notyet = kFALSE;
      }
      prt = ((nphotdEdx>5000) || (gLit->fNCallEL % 100 == 0) ||
        (gLit->fNCallEL < 50));
      prt = prt && stepok;
      if (prt) {
        gMes->SetName("EmitLight");
        gMes->SetTitle("TBeam");
        gMes->M(Info_M,1,"Run, step and number of dE/dx photons",fRun,i,nphotdEdx);
      }
      //loop on generation of photons due to dEdx
      for (j=1;j<=nphotdEdx;j++) {
        gGs->fNpGener += 1;
        if (gCs) gCs->fNpGener += 1;
        //determines where, along the step, the photon is emitted
        x = step*gRandom3->Rndm();
        tstart = t1 + (i-1)*step + x;
        fX0    = fX + tstart*fP;
        fTime  = tstart/fSpeed;
        //determines wavelength and time of emission of photon
        fMat->WaveAndLife(fWvlgth,fT0,kfluo);
        fT0 += fTime;
        //update all values depending upon wavelength
        gLit->NewWavelength(fWvlgth);
        bin = gGs->fHTimeAll->Fill(fT0);
        bin = gGs->fHWvlgthAll->Fill(fWvlgth);
        if (gCs) {
          bin = gCs->fHTimeAll->Fill(fT0);
          bin = gCs->fHWvlgthAll->Fill(fWvlgth);
        }
        if (gGs->HasFluo()) {
          afluo = kfluo;
          bin = gGs->fHTimeEach->Fill(fT0,afluo);
          bin = gGs->fHWvlgthEach->Fill(fWvlgth,afluo);
        }  //if (gGs->HasFluo())
        if ((gCs) && (gCs->HasFluo())) {
          afluo = kfluo;
          bin = gCs->fHTimeEach->Fill(fT0,afluo);
          bin = gCs->fHWvlgthEach->Fill(fWvlgth,afluo);
        }  //if (gCs)
        //generates the photon
        GenK(K);
        GenE(K,E);
        fPhot = new TPhoton(fRun,j,fX0,fSup,fWvlgth,fT0,K,E,0);
        //follow the photon from its birth to its death
        fPhot->Move();
        if (gGs->fNpAbnorm>TLitPhys::Get()->Anomalies()) {
          gMes->SetName("EmitLight");
          gMes->SetTitle("TBeam");
          gMes->M(kill_prog,1,"too many anomalies");
        }
        delete fPhot;
        fPhot = 0;
      }// end for (j=1;j<=nphotdEdx;j++)
      //generation of photons due to Cerenkov radiation
      if (fEmitCerenkov) {
        fParticle->Cerenkov(fnCerenkov,step);
        nphotCer = fParticle->NbCerPhot();
        if (prt) {
          gMes->SetName("EmitLight");
          gMes->SetTitle("TBeam");
          gMes->M(Info_M,2,"Run, step and number of Cerenkov photons",
            fRun,i,nphotCer);
        }
        //loop on generation of photons due to Cerenkov radiation
        for (j=1;j<=nphotCer;j++) {
          gGs->fNpGener += 1;
          if (gCs) gCs->fNpGener += 1;
          //determines where, along the step, the photon is emitted
          x = step*gRandom3->Rndm();
          tstart = t1 + (i-1)*step + x;
          fX0    = fX + tstart*fP;
          fTime  = tstart/fSpeed;
          //determines wavelength and time of emission of photon
          WandLCerenkov(fWvlgth,fT0);
          fT0 += fTime;
          //update all values depending upon wavelength
          gLit->NewWavelength(fWvlgth);
          bin = gGs->fHTimeCer->Fill(fT0);
          bin = gGs->fHWvlgthCer->Fill(fWvlgth);
          if (gCs) {
            bin = gCs->fHTimeCer->Fill(fT0);
            bin = gCs->fHWvlgthCer->Fill(fWvlgth);
          }
          GenKECer(K,E);
          fPhot = new TPhoton(fRun,j,fX0,fSup,fWvlgth,fT0,K,E,0);
          //follow the photon from its birth to its death
          fPhot->Move();
          if (gGs->fNpAbnorm>TLitPhys::Get()->Anomalies()) {
            gMes->SetName("EmitLight");
            gMes->SetTitle("TBeam");
            gMes->M(kill_prog,3,"too many anomalies");
          }
          delete fPhot;
          fPhot = 0;
        }// end for (j=1;j<=nphotCer;j++)
      }// end if (fEmitCerenkov)
    }// end for (i=1;i<=nstep;i++)
    //    if (!stepok) break;
  }// end while (!stepok)
}
void TBeam::FindDist(const char *distrib) {
  //  Finds the fit or interpolation of class TSplineFit, named distrib, that
  //the user has provided to describe the angular distribution of the beam
  //of particles.
  fFitDist = TSplineFit::FindFit(distrib,12);
  if (fFitDist) fDistName = distrib;
  else {
    gMes->SetName("TBeam");
    gMes->SetTitle("TBeam");
    gMes->M(kill_prog,1,"Distribution for theta beam not found");
  }
}
void TBeam::FindShape(TString &s1) {
  //  Finds the pointer to the TSupplShape associated with the shape whose
  //name is s1. Also finds the pointer to the node associated to this shape.
  TString s;
  TSupplShape *p;
  TIter next(&gLit->fSupplShape);
  Bool_t found = kFALSE;
  while ((!found) && (p = (TSupplShape *)next())) {
    s = p->GetName();
    if (!s.CompareTo(s1)) {
      found = kTRUE;
      fSuppl = p;
      fSNode = fSuppl->GetNode();
    }
  }
  if (!found) {
    gMes->SetName("FindShape");
    gMes->SetTitle("TBeam");
    gMes->M(kill_prog,1,"Reference shape not found");
  }
}
void TBeam::Gen(Int_t run, Int_t npart, Double_t xparam, Bool_t runstat,
  Bool_t ForgetLast, Double_t step) {
  //
  //  Starts a run generating npart particles which in turn will generate
  //photons while crossing fluorescent materials. The parameters are the
  //following :
  //
  //  run        : run number. Arbitrary, but has to be greater than 0.
  //
  //  npart      : number of beam particles to be generated in this run.
  //
  //  xparam     : this parameter is used as abscissa in the plotting of the
  //               results by the class TPublication. For instance, if you
  //               have a setup with a crystal and a beam transversed to the
  //               axis of the crystal and you make 10 runs varying the 
  //               coordinate of the beam along the axis of the crystal,
  //               you can choose this coordinate as xparam. All plots
  //               provided by TPublication will then have this coordinate
  //               as x coordinate. You will get efficiency versus beam posi-
  //               tion along axis of crystal, and so on. If you do not
  //               give xparam, or give a value smaller than -1.0e+20, all
  //               plots of TPublication will have the run number as x coord.
  //
  //
  //  runstat and ForgetLast :
  //
  //    if runstat == true [Default] AND ForgetLast == false [Default] :
  //
  //      in memory   : this run statistics keeped but this run histos deleted
  //                     when next run begins
  //      on the file : this run statistics and histos recorded
  //
  //            It is the only configuration which allows the working of
  //          TPublication, i.e. allowing to show histograms of quantities
  //          varying as a function of a run dependent parameter.
  //          Usage : normal case : try it first.
  //
  //    if runstat == true AND ForgetLast == true :
  //
  //      in memory   : this run statistics and histograms deleted when next
  //                     run begins.
  //      on the file : this run statistics and histos recorded
  //
  //          Disadvantage : TPublication not working, gGp pointer unavailable
  //          Advantage    : no increase of memory with runs
  //                         per run histograms still available on file
  //          Usage : use this configuration when your number of runs is big
  //                  but you still want per run statistics and histograms
  //                  on file.
  //
  //    if runstat == false ( ForgetLast true or false ) :
  //
  //      in memory   : no statistics and histograms for this run
  //      on the file : no statistics and histograms for this run
  //
  //          Disadvantage : TPublication not working, gGp pointer unavailable
  //                         per run statistics and histograms not available
  //          Advantage    : no increase of memory with runs
  //                         a bit faster, half less histograms to fill
  //          Usage : use this configuration when your number of runs is very
  //                  big and you are not interested in the per run statistics
  //                  and histograms.
  //
  //  Notice that in any case, the global statistics and histograms for all
  //runs is always present in memory and on the file. This global statistics
  //is an object of class TResults, pointed to by the pointer gGs.
  //
  //  step       : step is the distance, in [cm], into which the trajectory of
  //               the particle inside a shape will be cut for generating
  //               photons. Default 0.5 cm. Each time Litrani enters the next
  //               step, it prints a message about the number of photons
  //               produced in the step. This value is not important, except
  //               that if you choose it too big, the time between messages
  //               will be very long, so that the user will get the impression
  //               that the program is stuck. And if you choose it too small,
  //               the number of messages will be overwhelming. If it appears
  //               that the user has choosen step so small that less than 10
  //               dEdx photons are emitted, Litrani multiplies step by 10 as
  //               many times as necessary to get more than 10 photons.
  //
  Int_t ipart,bin;
  Short_t NCrossings;
  Double_t t1,t2,t3,t4;
  T3Vector x1,x2,x3,x4;
  Bool_t gGpok;
  gGpok = (runstat && (!ForgetLast));
  TIter next(&gLit->fSupplShape);
  gGs->RefreshDet();
  if (run<=0) {
    gMes->SetName("Gen");
    gMes->SetTitle("TBeam");
    gMes->M(Error_M,1,"run number must be >= 1. We change it !");
    run = TMath::Abs(run);
    if (run==0) run += 1;
  }
  fRun = run;
  cout << "Run : " << fRun << endl;
  *gMes->fListing << "Run : " << fRun << endl;
  fRunStat = runstat;
  fStep = step;
  OutsideOk();
  NameFile();
  //initializes the summary statistics of class TPublication
  if (!gGp && gGpok) gGp = new TPublication("Summary","Runs",gGpok);
  //Verifies that shapes are numbered
  Int_t nbofsh;
  if (!gLit->fShapeNumbered)
    nbofsh = gLit->GiveNbToShape();
  //Handle the pointer to the previous run. Even if gCs is put to 0 without
  //having been deleted, pointer to the previous run is not lost, it stays
  //inside gLit->fResults.
  if (gCs) {
    if (ForgetLast) {
      gLit->fResults.Remove(gCs);
      delete gCs;
      gCs = 0;
    }
    else gCs->DelHistos();
  }
  gCs = 0;
  //Makes gCs point towards this run.
  if (fRunStat)
    gCs = new TResults(fFullName,fTitle,fRun,xparam);
  //Open the .root file for writing, if not yet done
  if (gLit->fFilesClosed) {
    gLit->OpenFilesW(runstat);
    gROOT->cd();
    if ((!runstat) && (gCs)) {
      delete gCs;
      gCs = 0;
    }
  }
  //Big loop on generation of particles
  for (ipart=1;ipart<=npart;ipart++) {
    fSpeed = fParticle->GenP();
    fMomentum = fParticle->GetMomentum();
    fEnergy   = fParticle->GetEnergy();
    StartOneParticle();
    gGs->fNbPart += 1;
    bin = gGs->fHPMomentum->Fill(fMomentum);
    bin = gGs->fHPTheta->Fill(fTheta);
    if (gCs) {
      gCs->fNbPart += 1;
      bin = gCs->fHPMomentum->Fill(fMomentum);
      bin = gCs->fHPTheta->Fill(fTheta);
    }
    //Loop on the crossing of all shapes
    while ((fSup = (TSupplShape *)next())) {
      if (fSup->IsFluorescent()) {
        fMat = fSup->GetOptMat();
        if (fMat->dEdxActiv() != nodEdx) {
          NCrossings = fSup->AllCrossingPoints(fX,fP,t1,t2,t3,t4,x1,x2,x3,x4);
          if ((fEmitCerenkov) && (NCrossings))
            fnCerenkov = fMat->GetnCerenkov();
          switch (NCrossings) {
          case 0:
            break;
          case 2:
            gLit->fNCallEL += 1;
            EmitLight(t1,x1,t2,x2);
            break;
          case 4:
            gLit->fNCallEL += 1;
            EmitLight(t1,x1,t2,x2);
            gLit->fNCallEL += 1;
            EmitLight(t3,x3,t4,x4);
            break;
          default:
            gMes->SetName("Gen");
            gMes->SetTitle("TBeam");
            gMes->M(Error_M,2,"Absurd number of intercepts");
            break;
          }//end switch (NCrossings)
        }//end if (fMat->dEdxActiv() != nodEdx)
      }// end if (fSup->IsFluorescent())
    }// end while (fSup = (TSupplShape *)next())
    next.Reset();
  }// end for (ipart=1;ipart<=npart;ipart++)
  //Put statistics of this run into the tree.
  if (gCs) {
    gCs->Conclusion();
    gLit->fLitFile->cd();
    gLit->fLitNb += gLit->fLitTree->Fill();
    gROOT->cd();
  }
}
void TBeam::Gen4pi(Double_t &phi) {
  //generates fTheta and phi according to sin(t)dtdf   0<fTheta<pi
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  Double_t x,y;
  x = gRandom3->Rndm();
  y = un - deux*x;
  fTheta = TLitPhys::ACos(y);
  phi   = deux*TMath::Pi()*gRandom3->Rndm();
}
void TBeam::GenE(const T3Vector& K,T3Vector& E) const {
  //  Generates the direction of the E field of the photon generated by the
  //beam particle. Refuses to do it inside anisotropic material, where one is
  //not at all free to choose it in the plane orthogonal to k, but where it
  //is fixed when k is fixed.
  const Double_t zero = 0.0;
  const Double_t deux = 2.0;
  Double_t s,c,phi;
  if (!fMat->GetAnisotropy()) {
    T3Vector Ox,Oy;
    s = TMath::Abs(K*fAxisb1);
    c = TMath::Abs(K*fAxisb2);
    if (s<c) Ox = fAxisb1;
    else     Ox = fAxisb2;
    Ox.MinOrthoN(K,s,c);
    Oy = K^Ox;
    phi = deux*TMath::Pi()*gRandom3->Rndm();
    s   = TMath::Sin(phi);
    c   = TMath::Cos(phi);
    E   = c*Ox + s*Oy;
  }
  else E.Set(zero,zero,zero);
}
void TBeam::GenFlat(Double_t &phi) {
  //generates fTheta and phi according to sin(t)dtdf   0<fTheta<fAperture
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-14;
  if (fAperture<vsmall) {
    fTheta = zero;
  }
  else {
    Double_t x,y;
    x = gRandom3->Rndm();
    y = un - x*(un - fApercos);
    fTheta = TLitPhys::ACos(y);
  }
  phi = deux*TMath::Pi()*gRandom3->Rndm();
}
void TBeam::GenK(T3Vector& K) {
  //  Generates isotropically the direction of the k vector of the photon
  //generated by the beam particle.
  Double_t phi;
  Double_t x,y,z,st,ct,sf,cf;
  Gen4pi(phi);
  st = TMath::Sin(fTheta);
  ct = TMath::Cos(fTheta);
  sf = TMath::Sin(phi);
  cf = TMath::Cos(phi);
  x  = st*cf;
  y  = st*sf;
  z  = ct;
  K = x*fAxisb1 + y*fAxisb2 + z*fAxisb3;
}
void TBeam::GenKECer(T3Vector& K,T3Vector& E) const {
  //  Generates k vector and E field for a Cerenkov photon.
  const Double_t zero = 0.0;
  const Double_t deux = 2.0;
  Double_t phi;
  Double_t x,y,z,st,ct,sf,cf,s,c;
  st    = fParticle->CerSin();
  ct    = fParticle->CerCos();
  phi   = deux*TMath::Pi()*gRandom3->Rndm();
  sf = TMath::Sin(phi);
  cf = TMath::Cos(phi);
  x  = st*cf;
  y  = st*sf;
  z  = ct;
  K = x*fAxisb1 + y*fAxisb2 + z*fAxisb3;
  if (!fMat->GetAnisotropy()) {
    T3Vector Ox,Oy;
    s = TMath::Abs(K*fAxisb1);
    c = TMath::Abs(K*fAxisb2);
    if (s<c) Ox = fAxisb1;
    else     Ox = fAxisb2;
    Ox.MinOrthoN(K,s,c);
    Oy = K^Ox;
    phi = deux*TMath::Pi()*gRandom3->Rndm();
    s   = TMath::Sin(phi);
    c   = TMath::Cos(phi);
    E   = c*Ox + s*Oy;
  }
  else E.Set(zero,zero,zero);
}
void TBeam::GenP() {
  //Generates the direction of the beam particle
  Double_t phi;
  Double_t x,y,z,st,ct,sf,cf;
  switch (fKindDist) {
  case on4pi:
    Gen4pi(phi);
    break;
  case flat:
    GenFlat(phi);
    break;
  case sinuscosinus:
    GenSinCos(phi);
    break;
  case provided:
    GenProvided(phi);
    break;
  }
  st = TMath::Sin(fTheta);
  ct = TMath::Cos(fTheta);
  sf = TMath::Sin(phi);
  cf = TMath::Cos(phi);
  x  = st*cf;
  y  = st*sf;
  z  = ct;
  fP = x*fAxisb1 + y*fAxisb2 + z*fAxisb3;
  fP.Unit();
}
void TBeam::GenProvided(Double_t &phi) {
  //generates fTheta and phi according to provided distribution.
  //  0<fTheta<fAperture
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-14;
  const Int_t ntrymax   = 1000000;
  if (fAperture<vsmall) {
    fTheta = zero;
  }
  else {
    Bool_t ok = kFALSE;
    Int_t ntry = 0;
    while ((!ok) && (ntry<ntrymax)) {
      ntry++;
      fTheta = fFitDist->GetRandom();
      if (fTheta<=fAperture) ok = kTRUE;
    }
    if (!ok) {
      gMes->SetName("GenProvided");
      gMes->SetTitle("TBeam");
      gMes->M(kill_prog,1,"Problem generating fTheta");
    }
  }
  phi = deux*TMath::Pi()*gRandom3->Rndm();
}
void TBeam::GenSinCos(Double_t &phi) {
  //generates fTheta and phi according to sin(t)cos(t)dtdf   0<fTheta<fAperture
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-14;
  if (fAperture<vsmall) {
    fTheta = zero;
  }
  else {
    Double_t x,y;
    x = gRandom3->Rndm();
    y = fApersin*TMath::Sqrt(x);
    fTheta = TLitPhys::ASin(y);
  }
  phi = deux*TMath::Pi()*gRandom3->Rndm();
}
void TBeam::GenX() {
  //Generates the point of emission of the beam particle
  const Double_t un   = 1.0;
  const Double_t deux = 2.0;
  Double_t x,y,r,phi;
  switch (fKindCdl) {
  case point:
    fX = fCenter;
    break;
  case disk:
    x   = gRandom3->Rndm();
    r   = fA1*TMath::Sqrt(x);
    phi = deux*TMath::Pi()*gRandom3->Rndm();
    x   = r*TMath::Cos(phi);
    y   = r*TMath::Sin(phi);
    fX  = fCenter + x*fAxis1 + y*fAxis2;
    break;
  case ellipse:
    x   = gRandom3->Rndm();
    r   = TMath::Sqrt(x);
    phi = deux*TMath::Pi()*gRandom3->Rndm();
    x   = fA1*r*TMath::Cos(phi);
    y   = fA2*r*TMath::Sin(phi);
    fX  = fCenter + x*fAxis1 + y*fAxis2;
    break;
  case parallelogram:
    x   = fA1*(deux*gRandom3->Rndm() - un);
    y   = fA2*(deux*gRandom3->Rndm() - un);
    fX  = fCenter + x*fAxis1 + y*fAxis2;
    break;
  case parallelipiped:
    gMes->SetName("GenX");
    gMes->SetTitle("TBeam");
    gMes->M(kill_prog,1,"Parallelipiped forbidden as beam cradle");
    break;
  }
}
void TBeam::InitP() {
  // Pointers to 0
  fParticle = 0;
  fSuppl    = 0;
  fSNode    = 0;
  fFitDist  = 0;
  fSup      = 0;
  fMat      = 0;
  fPhot     = 0;
}
void TBeam::NameFile() {
  //Give a name to the histogram file for one run.
  //The name of the statistics and histograms file for a run is name ( the
  //name you have given in the constructor ) appended with '_'
  //followed by the run number and '.root'. For instance, if you have given
  //the name "mu_beam" to this instance of TBeam, and you call Gen with
  //run number 100, the name of the file will be "mu_beam_100.root".
  const Int_t kBufferSize = 64;
  Int_t l,l1;
  TString NameOnly;
  NameOnly = fName;
  l1 = kBufferSize - 10;
  l = NameOnly.Length();
  if (l>l1) NameOnly.Remove(l1);
  fFullName  = NameOnly;
  fFullName.Append('_');
  fFullName += fRun;
}
void TBeam::OutsideOk() const {
  //Insures that gOutside is defined
  if (!gOutside) {
    Bool_t b;
    gOutside = new TOpticMaterial("Outside","Material of the outside world",
      kFALSE,1.0,1.0e+6);
    gOutside->IsIsotropic(1.0);
    b = gOutside->CheckIt();
  }
}
void TBeam::Print() const {
  //Prints everything about Beam
  //output
  cout << endl;
  cout << "    Beam           : " << fName.Data()  << endl;
  cout << "    Title          : " << fTitle.Data() << endl;
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "    Beam           : " << fName.Data()  << endl;
  *gMes->fListing << "    Title          : " << fTitle.Data() << endl;
  *gMes->fListing << endl;
}
void TBeam::SetCerenkov(Bool_t cer) {
  //  Put ON or OFF the emission of Cerenkov light by the particle. If you
  //do not call this method, emission of Cerenkov light is ON by default.
  fEmitCerenkov = cer;
}
void TBeam::StartOneParticle() {
  //Generates a particle conform to all characteristics provided in the cons-
  //tructor and in the method Beam(). Time is set to 0 exactly when the
  //particle leaves the generating surface.
  const Double_t zero = 0.0;
  GenX();
  GenP();
  fTime = zero;
}
void TBeam::WandLCerenkov(Double_t &wavelength,Double_t &lifetime) const {
  //  Generates wavelength [nm] and lifetime [ps] for a photon of Cerenkov
  //light !
  //  Wavelength is distributed as 1/(wavelength)^2
  //  I know nothing yet about the time distribution of Cerenkov ligth. I
  //have to read some book on the subject ! For the time being, I set it as
  //immediate.
  //
  const Double_t zero = 0.0;
  Double_t l1,l2; //max and min values for a wavelength
  Double_t x;
  //Wavelength
  l1 = TLitPhys::Get()->MaxWaveL();
  l2 = TLitPhys::Get()->MinWaveL();
  x = gRandom3->Rndm();
  wavelength = (l1*l2)/(l1-x*(l1-l2));
  //Time
  lifetime = zero;
}
