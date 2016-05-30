// @(#)root/html:$Id: TPhotonCradle.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMessErr.h"
#include "TOpticMaterial.h"
#include "TSupplShape.h"
#include "TSNode.h"
#include "TPhotonCradle.h"

ClassImp(TPhotonCradle)
//______________________________________________________________________________
//
//        --------------------------------------------------------------
//        |                                                             |
//        |  This class is an internal class of Litrani. It is not ne-  |
//        | cessary for a user of Litrani to know anything about it.    |
//        | The documentation about it is of interest only for those    |
//        | interested in the inner working of Litrani.                 |
//        |                                                             |
//         -------------------------------------------------------------
//
//  TPhotonCradle is used to determines position, direction and polarization
//of a photon at its birth. The photon may be generated :
//
//  (fKindCdl==point)          --> at a given point fCenter
//  (fKindCdl==disk)           --> on a given disk, centered at fCenter,
//                                  with a radius fA1, normal to fAxis3.
//  (fKindCdl==ellipse)        --> on a given ellipse, centered at fCenter.
//                                  The axis of the ellipse are along fAxis1
//                                  and fAxis2 and have length fA1 and fA2.
//  (fKindCdl==parallelogram)  --> on a given parallelogram, centered at
//                                  fCenter, normal to fAxis3,
//                                  with half-length of 1st side of +-fA1
//                                    along fAxis1
//                                  with half-length of 2nd side of +-fA2
//                                    along fAxis2
//  (fKindCdl==parallelipiped) --> inside a given parallelipiped, centered
//                                    at fCenter,
//                                  with half-length of 1st side of +-fA1
//                                    along fAxis1
//                                  with half-length of 2nd side of +-fA2
//                                    along fAxis2
//                                  with half-length of 3rd side of +-fA3
//                                    along fAxis3
//
//  fAxis2 and fAxis1 MAY not be orthogonal. fAxis3 is always orthogonal to
//fAxis1 and fAxis2, except perhaps in the case fKindCdl == parallelipiped.
//  At least a small part of the surface or volume of the photon cradle must
//be inside the shape fSuppl. If a point, belonging to the cradle, is
//generated outside fSuppl, a new point will be generated until a point
//inside fSuppl is found. If you have provided a cradle totally outside
//fSuppl, an acceptable point will never be found. Litrani will make 1000000
//trials and then give up, aborting the program.
//  Use the first version of the constructor for the simplest case where the
//cradle is a point. Use the second version of the constructor when the
//cradle is at the same time a face of the shape fSuppl. But this is allowed
//only if the face has the shape of a disk or of a parallelogram. Use the
//3rd version of the constructor if either the parallelogram is a rectangle
//(so that fAxis2 is orthogonal to fAxis1 and fAxis1, fAxis2 and fAxis3
//build an orthonormal system), or the parallelipiped is a rectangular
//parallelipiped, (so that fAxis2 is orthogonal to fAxis1 and fAxis1, fAxis2
//and fAxis3 build an orthonormal system). Use the 4th version of the
//constructor in the most general case when fAxis1 is not necessarily
//orthogonal to fAxis2.
//
//  Notice that the beam of photons may have the following shapes :
//( t == angle theta, f == angle phi )
//
//  (fKindDist==on4pi)         --> the k vector is generated isotropically on 4 pi
//                            with the distribution sin(t)dtdf.
//  (fKindDist==flat)          --> the k vector is generated with t <= fAperture,
//                            with the distribution sin(t)dtdf around the
//                            direction given by the vector fAxisb3.
//  (fKindDist==sinuscosinus)  --> the k vector is generated with t <= fAperture,
//                            with the distribution sin(t)cos(t)dtdf around
//                            the direction given by the vector fAxisb3.
//                            (With an extra cos(t) factor to slightly
//                            favour the forward direction with respect to a
//                            flat distribution sin(t)dtdf. This is
//                            recommanded for an optical fibre.) 
//  (fKindDist==provided)      --> the k vector is generated according to a
//                            distribution given by the user under the
//                            form of a fit or interpolation of type
//                            TSplineFit, around the direction given by the
//                            vector fAxisb3.
//
//  In the case you provide yourself the distribution fDistName, please be
//careful about the following non trivial issues :
//    (0) - To know how to define yourself a distribution of type TSplineFit
//to be used to generate random numbers according to a distribution, please
//read the documentation about TSplineFit and in particular the documentation
//about the 2 methods UseForRandom() and GetRandom(). You have to call
//UseForRandom() !
//    (1) - your distribution has to generate theta in radians, not degrees.
//    (2) - you have to give a 2-dimensional distribution, not a 1-dimensio-
//nal distribution. If you have measured yourself the distribution, your
//measurement was very likely a 1-dimensional distribution, (you have
//displaced your instrument inside a plane and you have assumed isotropy in
//phi) so you have to multiply each measurement by sin(t).
//    (3) - you have probably measured your distribution in air, so recalcu-
//lates all angles in the material of the shape fSuppl using the Fresnel
//formula ni sin(ti) = nr sin(tr).
//    (4) - Even if you have provided yourself the distribution, the value
//for fAperture still matters : if a t angle is generated from your distri-
//bution which is bigger than fAperture, then we start again until a value
//smaller than fAperture is found.
//
//  The generated photons may be asked to be polarized, i.e. to have the
//direction of the E field parallel to a given direction fPolar. Notice this
//restriction : it is forbidden to ask for polarized photons if the shape
//fSuppl contains anisotropic material. If you want a polarized beam inside
//an anisotropic material, generate the photons outside and let them enter
//into the anisotropic marerial later on. If the material is isotropic, the
//constraint remain that the E field must be orthogonal to the k vector.
//Suppose that you have given an aperture of 0 (fAperture == 0) and that you
//have given fAxisb3 and fPolar orthogonal : everything is ok. E field fE will be
//generated parallel to fPolar. But suppose that the aperture is non-zero.
//The direction of the k vector generated for the photon will no more be
//exactly parallel to fAxisb3, and therefore no more orthogonal to fPolar.
//What we do in that case is to generate an fE vector (direction of the E
//field) as close as possible to fPolar, but orthogonal to the k vector of
//the photon.
//
//  The characteristics of the beam are not given in the constructor of
//TPhotonCradle. They have to be provided by a call to Beam(). If you do
//not call Beam(), default characteristics for the beam will be used. which
//are the following :
//
//            fPolarized = kFALSE;
//            fKindDist  = on4pi;
//            fAxisb3    = fAxis3;  (irrelevant)
//            fAperture  = 0;       (irrelevant)
//            fPolar     = fAxis1;  (irrelevant)
//
//  Notice that there are 2 instances of the Beam() method, depending whether
//the axis of the beam, fAxisb3, is parallel or not to fAxis3, i.e. whether
//the axis of the beam is orthogonal or not to the generating surface.
//
TPhotonCradle::TPhotonCradle(TSupplShape *suppl,const T3Vector &center,Bool_t InWCS) {
  //  Constructor in the case one wants that the photons originates from a
  //point.
  //
  //  The characteristics of the beam are not given in the constructor of
  //TPhotonCradle. They have to be provided by a call to Beam(). If you do not
  //call Beam(), default characteristics for the beam will be used. which are
  //the following :
  //
  //            fPolarized = kFALSE;
  //            fKindDist  = on4pi;
  //            fAxisb3    = fAxis3;
  //            fAperture  = 0;       (irrelevant)
  //            fPolar     = fAxis1;  (irrelevant)
  //
  //  It is expected that center be given in the local coordinates of the
  //Shape suppl. If it is not the case, and center is in World Coordinate system,
  //Put InWCS to true! [Default false]
  //
  InitP();
  DefaultInit(suppl,center,InWCS);
}
TPhotonCradle::TPhotonCradle(TSupplShape* suppl,Int_t iface) {
  //  Constructor in the case one wants that the photons originates from one face of
  //the shape suppl. This is very handy when you have for instance to simulate an
  //optical fibre : in that case, choose a TSCYL for the shape suppl, and choose
  //the exit face (iface = 0 or 1) as the face generating the photons. Notice that
  //the face you choose MUST be a disk or a parallelogram. No other shape face is
  //allowed. Notice also that the photons are generated on the inner side of the
  //face, i.e. if you are simulating an optical fibre, you have to give the
  //distribution for the theta angle of the beam of photons IN THE MATERIAL OF THE
  //FIBRE (in the material of suppl) and not in the material of the shape with
  //which suppl is in contact with ( i.e. NOT IN AIR, for instance ). The transition
  //fibre->air (suppl ->shape in contact) will be done by Litrani.
  //
  //The parameters are the following :
  //
  //  suppl   : TSupplShape in which the cradle of the photons is.
  //  iface   : face number of the face generating the photons. To know
  //            how face are numbered in a shape, refer to the documentation
  //            of the relevant shape, for instance the documentation of TSCYL.
  //
  //  Notice that the characteristics of the beam are not given in this constructor of
  //TPhotonCradle. They have to be provided by a call to Beam(). If you do not call
  //Beam(), default characteristics for the beam will be used which are the following :
  //            fPolarized = kFALSE;
  //            fAxisb3    = normal to face
  //            fAperture  = 0;
  //            fKindDist  = flat;
  //
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  InitP();
  fSuppl     = suppl;
  fSNode     = fSuppl->GetNode();
  fFromFace  = kTRUE;
  TOpticMaterial *pmat = fSuppl->GetOptMat();
  fAni       = pmat->GetAnisotropy();
  fAxis1.Set(zero,zero,zero);
  fAxis2.Set(zero,zero,zero);
  fA2        = zero;
  fA3        = zero;
  fPolarized = kFALSE;
  fAperture  = zero;
  fKindDist  = flat;
  TFace *f;
  f = fSuppl->GetFace(iface);
  KindOfFace kindf;
  kindf = f->GetKind();
  Int_t npoints = f->NumberOfPoints();
  Bool_t typeok1 = ((kindf==polygonal) && (npoints == 4));
  Bool_t typeok2 = (kindf==circular);
  Bool_t typeok  = typeok1 || typeok2;
  if ((f->GetPlanar()) && (typeok)) {
    fAxis3  = f->Normal();
    fAxisb3 = fAxis3;
    if (kindf==circular) {
      fKindCdl = disk;
      fCenter  = *f->Point(0);
      fA1      = f->GetRadius1();
      //in the case of disk, one is free to choose fAxis1 anywhere but
      //orthogonal to fAxis3
      Short_t k;
      k = fAxis3.Smallest();
      switch (k) {
      case 0:
        fAxis2.Set(un,zero,zero);
        break;
      case 1:
        fAxis2.Set(zero,un,zero);
        break;
      case 2:
        fAxis2.Set(zero,zero,un);
        break;
      }
      fAxis1 = fAxis2^fAxis3;
    }
    else {
      fKindCdl = parallelogram;
      Bool_t b;
      b = f->Parallelogram(fCenter,fAxis1,fA1,fAxis2,fA2);
      if (!b) {
        gMes->SetName("TPhotonCradle");
        gMes->SetTitle("TPhotonCradle");
        gMes->M(kill_prog,2,"Face not disk neither parallelogram");
      }
    }
  }
  else {
    gMes->SetName("TPhotonCradle");
    gMes->SetTitle("TPhotonCradle");
    gMes->M(kill_prog,3,"Face not disk neither parallelogram");
  }
}
TPhotonCradle::TPhotonCradle(TSupplShape* suppl,const T3Vector &center,
  KindOfCradle kind,Double_t a1,Double_t a2,Double_t a3,Double_t theta3,
  Double_t phi3,Double_t theta1,Double_t phi1,Bool_t InWCS) {
  //
  //  Constructor in the general case, BUT with the restriction that (fAxis1,
  //fAxis2 and fAxis3 build an orthonormal system, which is always the case,
  //except when kind == parallelogram and the parallelogram is not a
  //rectangle or when kind == parallelipiped and the parallelipiped is not a
  //rectangular parallelipiped ! So use this constructor if the photon cradle
  //is a disk, an ellipse, a rectangle or a rectangular parallelipiped. If
  //kind == point, use rather the 1st constructor, although the use of this
  //one is possible. If kind == parallelogram and the parallelogram is not a
  //rectangle or kind == parallelipiped and the parallelipiped is not rectan-
  //gular, use the next constructor.
  //
  //  EVERY POSITIONS AND DIRECTIONS (center, theta3,phi3,theta1,phi1) ARE
  //GIVEN IN THE LOCAL COORDINATE SYSTEM LCS of the shape suppl, with the
  //only possible exception of T3Vector center, which will be considered as given
  //in the Wold Coordinate System WCS, in case InWCS is true [default false]
  //  The shape suppl MUST contain at least a portion of the photon cradle. The
  //photons will be created in the portion of the beam cradle which is inside the
  //shape suppl. All coordinates or directions given by the user will be transformed
  //into World Coordinate.
  //
  //The parameters are the following :
  //
  //  suppl   : TSupplShape in which at least part of the photon cradle is.
  //  center  : center of the photon cradle in LCS of the shape "suppl".
  //            [except in case InWCS true]
  //  kind    : kind of cradle : {point,disk,ellipse,parallelogram,parallelipiped}
  //  a1      : - radius of disk or
  //            - half-length of 1st axis of ellipse along fAxis1 or 
  //            - half-length of rectangle along fAxis1 or
  //            - half-length of rectangular parallelipiped along fAxis1
  //            default 1.0
  //            Irrelevant in case kind == {point}
  //  a2      : - half-length of 2nd axis of ellipse along fAxis2 or 
  //            - half-length of rectangle along fAxis2 or
  //            - half-length of rectangular parallelipiped along fAxis2
  //            default 1.0
  //            Irrelevant in case kind == {point,disk}
  //  a3      : - half-length of rectangular parallelipiped along fAxis3
  //            default 1.0
  //            Irrelevant in case kind == {point,disk,ellipse,parallelogram}
  //  theta3  : angle theta (degree, in LCS) of the axis fAxis3, normal to
  //             disk, ellipse or rectangle, or parallel to 3rd axis of
  //             parallelipiped. Default 180.
  //  phi3    : angle phi   (degree, in LCS) of the axis fAxis3, normal to
  //             disk, ellipse or rectangle, or parallel to 3rd axis of
  //             parallelipiped. Default 0.
  //             If you do not give these 2 parameters, fAxis3 will be along
  //              -Oz in LCS of shape suppl.
  //             These 2 parameters are irrelevant in case kind == point.
  //  theta1  : angle theta (degree, in LCS) of the axis fAxis1, 1st axis of
  //             ellipse or 1st axis of rectangle or rectangular paralleli-
  //             piped. Default 90.
  //  phi1    : angle phi   (degree, in LCS) of the axis fAxis1, 1st axis of
  //             ellipse or 1st axis of rectangle or rectangular paralleli-
  //             piped. Default 180.
  //            If you do not give these 2 parameters, fAxis1 will be along
  //            -Ox in LCS. These 2 parameters are irrelevant in case
  //            kind == point or disk.
  //            Not giving the 4 last parameters will result in the following
  //            in the LCS of shape "suppl" :
  //            fAxis1 = -Ox   fAxis2 = +Oy   fAxis3 = -Oz  (falling beam)
  //
  //  Notice that fAxis2 needs not be given. It can be obtained from fAxis1
  //and fAxis3, because it is assumed in this constructor that fAxis1, fAxis2
  //and fAxis3 build an orthonormal system.
  //  The characteristics of the beam are not given in the constructor of
  //TPhotonCradle. They have to be provided by a call to Beam(). If you do
  //not call Beam(), default characteristics for the beam will be used. which
  //are the following :
  //
  //            fPolarized = kFALSE;
  //            fKindDist  = on4pi;
  //            fAxisb3    = fAxis3;  (irrelevant)
  //            fAperture  = 0;       (irrelevant)
  //            fPolar     = fAxis1;  (irrelevant)
  //
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  const Double_t small = 1.0e-3;
  const Double_t torad = TMath::Pi()/180.0;
  T3Vector v,axis1,axis2,axis3;
  Double_t s,c,sf,cf;
  InitP();
  DefaultInit(suppl,center,InWCS);
  theta3 *= torad;
  phi3   *= torad;
  theta1 *= torad;
  phi1   *= torad;
  fA1 = TMath::Abs(a1);
  fA2 = TMath::Abs(a2);
  fA3 = TMath::Abs(a3);
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
  case parallelipiped:
    s  = TMath::Sin(theta1);
    c  = TMath::Cos(theta1);
    sf = TMath::Sin(phi1);
    cf = TMath::Cos(phi1);
    axis1.Set(s*cf,s*sf,c);
    Double_t test = TMath::Abs(axis1*axis3);
    if (test>small) {
      gMes->SetName("TPhotonCradle");
      gMes->SetTitle("TPhotonCradle");
      gMes->M(Error_M,4,"axis1 not orthogonal to axis3");
      gMes->M(Error_M,4,"axis1 changed to be orthogonal to axis3");
    }
    axis1.MinOrthoN(axis3,s,c);
    fSNode->AxisToWorld(axis1,fAxis1);
    fAxis1.Unit();
    fAxis2 = fAxis3^fAxis1;
    break;
  }
  //We have to provide default values for the 3 beam axis
  fAxisb1 = fAxis1;
  fAxisb2 = fAxis2;
  fAxisb3 = fAxis3;
  fPolar  = fAxisb1;  //irrelevant
}
TPhotonCradle::TPhotonCradle(const T3Vector &center,TSupplShape* suppl,
  KindOfCradle kind,Double_t a1,Double_t a2,Double_t a3,Double_t theta3,
  Double_t phi3,Double_t theta1,Double_t phi1,Double_t theta2,Double_t phi2,
  Bool_t InWCS) {
  //
  //  Constructor in the MOST general case, without any restriction. fAxis1,
  //fAxis2 and fAxis3 are not necessarily orthogonal. This constructor can be
  //used for any situation ( kind == {point,disk,ellipse,parallelogram,
  //parallelipiped} ), but if kind == point, it is much easier to use the 1st
  //constructor, and if kind = {disk,ellipse} or kind == parallelogram, the
  //parallelogram being a rectangle or kind == parallelipiped, the paralleli-
  //piped being rectangular, it is easier to use the 3rd constructor. So, use
  //this constructor only if kind == parallelogram, and the parallelogram is
  //not a rectangle or kind == parallelipiped and the parallelipiped is not
  //rectangular.
  //
  //  EVERY POSITIONS AND DIRECTIONS (center, theta3,phi3,theta1,phi1,theta2,phi2)
  //ARE GIVEN IN THE LOCAL COORDINATE SYSTEM LCS of the shape suppl, with the
  //only possible exception of T3Vector center, which will be considered as given
  //in the Wold Coordinate System WCS, in case InWCS is true [default false]
  //  The shape suppl MUST contain at least a portion of the photon cradle. The
  //photons will be created in the portion of the beam cradle which is inside the
  //shape suppl. All coordinates or directions given by the user will be transformed
  //into World Coordinate.
  //
  //The parameters are the following :
  //
  //  center  : center of the photon cradle in LCS of the shape "suppl".
  //            [except in case InWCS true]
  //  suppl   : TSupplShape in which at least part of the photon cradle is.
  //  kind    : kind of cradle : {point,disk,ellipse,parallelogram,parallelipiped}
  //  a1      : - radius of disk or
  //            - half-length of 1st axis of ellipse along fAxis1 or 
  //            - half-length of parallelogram along fAxis1 or
  //            - half-length of parallelipiped along fAxis1
  //            default 1.0
  //            Irrelevant in case kind == {point}
  //  a2      : - half-length of 2nd axis of ellipse along fAxis2 or 
  //            - half-length of parallelogram along fAxis2 or
  //            - half-length of parallelipiped along fAxis2
  //            default 1.0
  //            Irrelevant in case kind == {point,disk}
  //  a3      : - half-length of parallelipiped along fAxis3
  //            default 1.0
  //            Irrelevant in case kind == {point,disk,ellipse,parallelogram}
  //  theta1  : angle theta (degree, in LCS) of the axis fAxis1, 1st axis of
  //             ellipse or 1st axis of parallelogram or parallelipiped.
  //             Default 90.
  //  phi1    : angle phi   (degree, in LCS) of the axis fAxis1, 1st axis of
  //             ellipse or 1st axis of parallelogram or parallelipiped.
  //             Default 180.
  //            If you do not give these 2 parameters, fAxis1 will be along
  //            -Ox in LCS. These 2 parameters are irrelevant in case
  //            kind == point or disk.
  //  theta2  : angle theta (degree, in LCS) of the axis fAxis2, 2nd axis of
  //             ellipse or 2nd axis of parallelogram or parallelipiped.
  //             Default 90.
  //  phi2    : angle phi   (degree, in LCS) of the axis fAxis2, 2nd axis of
  //             ellipse or 2nd axis of parallelogram or parallelipiped.
  //             Default 90.
  //             (if you do not give these 2 parameters, fAxis2 will be along
  //             +Oy in LCS)
  //  theta3  : angle theta (degree, in LCS) of the axis fAxis3, normal to
  //             disk or ellipse or parallelogram, or 3rd axis of paralleli-
  //             piped. Default 180.
  //  phi3    : angle phi   (degree, in LCS) of the axis fAxis3, normal to
  //             disk or ellipse or parallelogram, or 3rd axis of paralleli-
  //             piped. Default 0.
  //             If you do not give these 2 parameters, fAxis3 will be along
  //              -Oz in LCS of shape suppl.
  //             Notice that these 2 parameters are used ONLY in the case of
  //            parallelipiped. In all other cases, fAxis3 is obtained by :
  //              fAxis3 = fAxis1^fAxis2
  //
  //  Notice the inversion of the first 2 parameters, as compared with the
  //preceeding constructor : this inversion is there only to allow the compiler
  //not to confuse the two ! Notice again that it is only necessary to give
  //theta3, phi3 of fAxis3 in the case of parallelipiped, because fAxis3 can
  //be obtained from fAxis1 and fAxis2 by fAxis3 = fAxis1^fAxis2 ! But this
  //implies that EVEN IN THE CASE OF {point, disk, ellipse } you carefully
  //define fAxis1 AND fAxis2 so that fAxis3 be correctly calculated. This
  //shows once more that this constructor is best suited to define non-rectan-
  //gularparallelograms and parallelipiped and that you should use the other
  //constructors for the other cases.
  //
  //  The characteristics of the beam are not given in the constructor of TPhotonCradle.
  //They have to be provided by a call to Beam(). If you do not call Beam(), default
  //characteristics for the beam will be used. which are the following :
  //
  //            fPolarized = kFALSE;
  //            fKindDist  = on4pi;
  //            fAxisb3    = fAxis3;  (irrelevant)
  //            fAperture  = 0;       (irrelevant)
  //            fPolar     = fAxis1;  (irrelevant)
  //
  const Double_t small = 1.0e-3;
  const Double_t torad = TMath::Pi()/180.0;
  T3Vector v,axis1,axis2,axis3;
  Double_t s,c,sf,cf,test;
  InitP();
  DefaultInit(suppl,center,InWCS);
  theta3 *= torad;
  phi3   *= torad;
  theta1 *= torad;
  phi1   *= torad;
  theta2 *= torad;
  phi2   *= torad;
  fA1 = TMath::Abs(a1);
  fA2 = TMath::Abs(a2);
  fA3 = TMath::Abs(a3);
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
      gMes->SetName("TPhotonCradle");
      gMes->SetTitle("TPhotonCradle");
      gMes->M(Error_M,5,"axis2 not orthogonal to axis1");
      gMes->M(Error_M,5,"axis2 changed to be orthogonal to axis1");
    }
    axis2.MinOrthoN(axis1,s,c);
    break;
  case parallelogram:
  case parallelipiped:
    break;
  }
  fSNode->AxisToWorld(axis1,fAxis1);
  fAxis1.Unit();
  fSNode->AxisToWorld(axis2,fAxis2);
  fAxis2.Unit();
  if (kind == parallelipiped) {
    s  = TMath::Sin(theta3);
    c  = TMath::Cos(theta3);
    sf = TMath::Sin(phi3);
    cf = TMath::Cos(phi3);
    axis3.Set(s*cf,s*sf,c);
    fSNode->AxisToWorld(axis3,fAxis3);
    fAxis3.Unit();
  }
  else {
    fAxis3 = fAxis1^fAxis2;
    fAxis3.Unit();
  }
  //We have to provide default values for the 3 beam axis
  fAxisb3 = fAxis3;
  fAxisb1 = fAxis2^fAxis3;
  fAxisb1.Unit();
  fAxisb2 = fAxisb3^fAxisb1;
  fAxisb2.Unit();
  fPolar     = fAxisb1;  //irrelevant
}
void TPhotonCradle::AxisBeam() {
  //Complement fAxisb3 with fAxisb1,2 in order to have a full coordinate system
  Double_t s,c;
  s = TMath::Abs(fAxis1*fAxisb3);
  c = TMath::Abs(fAxis2*fAxisb3);
  if (s<c) {
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
void TPhotonCradle::Beam(KindOfDist kind, Double_t aper,Double_t theta,Double_t phi,
  const char *distrib,Bool_t polar,Double_t thetp, Double_t phip) {
  //Defines the characteristics of the beam of photon. This instance of Beam()
  //is the one to use when the axis of the beam, fAxisb3, is not equal to fAxis3,
  //i.e. when the axis of the beam is not orthogonal to the generating surface
  //of the beam (which is perfectly allowed). Parameters :
  //
  //  kind     : {on4pi,flat,sinuscosinus,provided}
  //  aper     : aperture of the beam, in degree, around fAxisb3.
  //  theta    : theta angle, in degree, in LCS of fSuppl, of fAxisb3.
  //  phi      : phi angle, in degree, in LCS of fSuppl, of fAxisb3.
  //  distrib  : name of the fit or interpolation of type TSplineFit giving
  //             the distribution for the theta angle of the generated
  //             photons around fAxisb3 in case kind == provided.
  //              (default "")
  //  polar    : true if the beam is polarized
  //              (default kFALSE)
  //  thetp    : theta angle, in degree, in LCS of fSuppl, of fPolar, the axis
  //             along which the electric field E of the photons have to be
  //             parallel if polar is true.
  //  phip     : phi angle, in degree, in LCS of fSuppl, of fPolar, the axis
  //             along which the electric field E of the photons have to be
  //             parallel if polar is true.
  //
  const Double_t torad = TMath::Pi()/180.0;
  Double_t st,ct,sf,cf;
  fKindDist = kind;
  FindDist(distrib);
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
  AxisBeam();
  fPolarized = polar;
  SetPolar(thetp,phip);
}
void TPhotonCradle::Beam(KindOfDist kind, Double_t aper,const char *distrib,
  Bool_t polar,Double_t thetp, Double_t phip) {
  //Defines the characteristics of the beam of photon. This instance of Beam()
  //is the one to use when the axis of the beam, fAxisb3, is identical to fAxis3,
  //i.e. when the axis of the beam is orthogonal to the generating surface
  //of the beam (or along the 3rd axis of the parallelipiped). Parameters :
  //
  //  kind     : {on4pi,flat,sinuscosinus,provided}
  //  aper     : aperture of the beam, in degree, around fAxisb3.
  //              (default 0)
  //  distrib  : name of the fit or interpolation of type TSplineFit giving
  //             the distribution for the theta angle of the generated
  //             photons around fAxisb3 in case kind == provided.
  //              (default "")
  //  polar    : true if the beam is polarized
  //              (default kFALSE)
  //  thetp    : theta angle, in degree, in LCS of fSuppl, of fPolar, the axis
  //             along which the electric field E of the photons have to be
  //             parallel if polar is true.
  //              (default 90)
  //  phip     : phi angle, in degree, in LCS of fSuppl, of fPolar, the axis
  //             along which the electric field E of the photons have to be
  //             parallel if polar is true.
  //              (default 0) If these 2 parameters are not given, the axis of
  //              polarization, fPolar, will be along Ox of LCS of fSuppl.
  //
  const Double_t vsmall = 1.0e-6;
  const Double_t torad = TMath::Pi()/180.0;
  fKindDist = kind;
  FindDist(distrib);
  fAperture = torad*TMath::Abs(aper);
  fApersin  = TMath::Sin(fAperture);
  fApercos  = TMath::Cos(fAperture);
  fAxisb3 = fAxis3;
  Double_t s,c,length;
  T3Vector v;
  v = fAxis1^fAxisb3;
  length = TMath::Abs(v.Module());
  if (length>vsmall) fAxisb1 = fAxis1;
  else fAxisb1 = fAxis2;
  fAxisb1.MinOrthoN(fAxisb3,s,c);
  fAxisb2 = fAxisb3^fAxisb1;
  fPolarized = polar;
  SetPolar(thetp,phip);
}
void TPhotonCradle::DefaultInit(TSupplShape *suppl, const T3Vector &center,Bool_t InWCS) {
  //Default initialization
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  T3Vector axis1,axis2,axis3;
  fSuppl     = suppl;
  fFromFace  = kFALSE;
  TOpticMaterial *pmat = fSuppl->GetOptMat();
  fAni       = pmat->GetAnisotropy();
  fSNode     = fSuppl->GetNode();
  if (InWCS) fCenter = center;
  else       fSNode->LocalToWorld(center,fCenter);
  fKindCdl = point;
  fA1 = zero; fA2 = zero; fA3 = zero;
  fPolarized = kFALSE;
  fKindDist  = on4pi;
  fDistName  = "";
  fAperture  = zero;
  fApersin   = zero;
  fApercos   = un;
  axis1.Set(un,zero,zero);
  axis2.Set(zero,un,zero);
  axis3.Set(zero,zero,un);
  fSNode->AxisToWorld(axis1,fAxis1);
  fSNode->AxisToWorld(axis2,fAxis2);
  fSNode->AxisToWorld(axis3,fAxis3);
  fAxisb1 = fAxis1;
  fAxisb2 = fAxis2;
  fAxisb3 = fAxis3;
}
void TPhotonCradle::FindDist(const char *distrib) {
  //Finds the distribution of the beam in theta
  if (fKindDist==provided) {
    fFitDist = TSplineFit::FindFit(distrib,12);
    if (fFitDist) fDistName = distrib;
    else {
      fDistName = "";
      gMes->SetName("Beam");
      gMes->SetTitle("TPhotonCradle");
      gMes->M(kill_prog,1,"Distribution for theta beam not found");
    }
  }
}
void TPhotonCradle::Gen4pi(Double_t &theta,Double_t &phi) const {
  //generates theta and phi according to sin(t)dtdf   0<theta<pi
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  Double_t x,y;
  x = gRandom3->Rndm();
  y = un - deux*x;
  theta = TLitPhys::ACos(y);
  phi   = deux*TMath::Pi()*gRandom3->Rndm();
}
void TPhotonCradle::GenE() {
  //Generates the unit fE vector, direction of the E field.
  //Refuses to do it inside anisotropic material. GenK() must have been
  //called before.
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  const Double_t deux  = 2.0;
  const Double_t small = 1.0e-6;
  Double_t s,c,phi,test;
  if (!fAni) {
    if (fPolarized) {
      fE = fPolar;
      fE.MinOrthoN(fK,s,c);
    }
    else {
      gMes->SetName("GenE");
      gMes->SetTitle("TPhotonCradle");
      T3Vector Ox,Oy;
      s = TMath::Abs(fK*fAxisb1);
      c = TMath::Abs(fK*fAxisb2);
      if (s<=c) Ox = fAxisb1;
      else      Ox = fAxisb2;
      Ox.MinOrthoN(fK,s,c);
      Oy = fK^Ox;
      Oy.Unit();
      if (gMes->TestCalc()) {
        test = TMath::Abs(un - Ox.Module());
        if (test>small)
          gMes->M(Error_M,1,"Ox not of length 1",test);
        test = TMath::Abs(Ox*Oy);
        if (test>small)
          gMes->M(Error_M,2,"Ox not orthogonal to Oy",test);
        test = TMath::Abs(Ox*fK);
        if (test>small)
          gMes->M(Error_M,3,"Ox not orthogonal to fK",test);
        test = TMath::Abs(Oy*fK);
        if (test>small)
          gMes->M(Error_M,4,"Oy not orthogonal to fK",test);
      }
      phi = deux*TMath::Pi()*gRandom3->Rndm();
      s   = TMath::Sin(phi);
      c   = TMath::Cos(phi);
      fE   = c*Ox + s*Oy;
    }
  }
  else fE.Set(zero,zero,zero);
}
void TPhotonCradle::Generate(T3Vector &x0,T3Vector &K,T3Vector &E) {
  //Generates a photon conform to the characteristics of the cradle and beam.
  GenX(); x0 = fX;
  GenK(); K  = fK;
  GenE(); E  = fE;
}
void TPhotonCradle::GenFlat(Double_t &theta,Double_t &phi) const {
  //generates theta and phi according to sin(t)dtdf   0<theta<fAperture
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-14;
  if (fAperture<vsmall) {
    theta = zero;
  }
  else {
    Double_t x,y;
    x = gRandom3->Rndm();
    y = un - x*(un - fApercos);
    theta = TLitPhys::ACos(y);
  }
  phi = deux*TMath::Pi()*gRandom3->Rndm();
}
void TPhotonCradle::GenK() {
  //Generates the direction of the k vector of the photon
  Double_t theta,phi;
  Double_t x,y,z,st,ct,sf,cf;
  switch (fKindDist) {
  case on4pi:
    Gen4pi(theta,phi);
    break;
  case flat:
    GenFlat(theta,phi);
    break;
  case sinuscosinus:
    GenSinCos(theta,phi);
    break;
  case provided:
    GenProvided(theta,phi);
    break;
  }
  st = TMath::Sin(theta);
  ct = TMath::Cos(theta);
  sf = TMath::Sin(phi);
  cf = TMath::Cos(phi);
  x  = st*cf;
  y  = st*sf;
  z  = ct;
  if (fKindDist == on4pi) fK.Set(x,y,z);
  else fK = x*fAxisb1 + y*fAxisb2 + z*fAxisb3;
  fK.Unit();
}
void TPhotonCradle::GenProvided(Double_t &theta,Double_t &phi) const {
  //generates theta and phi according to provided distribution.
  //  0<theta<fAperture
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-14;
  const Int_t ntrymax   = 1000000;
  if (fAperture<vsmall) {
    theta = zero;
  }
  else {
    Bool_t ok = kFALSE;
    Int_t ntry = 0;
    while ((!ok) && (ntry<ntrymax)) {
      ntry++;
      theta = fFitDist->GetRandom();
      if (theta<=fAperture) ok = kTRUE;
    }
    if (!ok) {
      gMes->SetName("GenProvided");
      gMes->SetTitle("TPhotonCradle");
      gMes->M(kill_prog,1,"Problem generating theta");
    }
  }
  phi = deux*TMath::Pi()*gRandom3->Rndm();
}
void TPhotonCradle::GenSinCos(Double_t &theta,Double_t &phi) const {
  //generates theta and phi according to sin(t)cos(t)dtdf   0<theta<fAperture
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-14;
  if (fAperture<vsmall) {
    theta = zero;
  }
  else {
    Double_t x,y;
    x = gRandom3->Rndm();
    y = fApersin*TMath::Sqrt(x);
    theta = TLitPhys::ASin(y);
  }
  phi = deux*TMath::Pi()*gRandom3->Rndm();
}
void TPhotonCradle::GenX() {
  //Generates the point of emission of the photon in the cradle
  const Int_t ntrymax = 1000000;
  const Double_t un   = 1.0;
  const Double_t deux = 2.0;
  Double_t x,y,z,r,phi;
  Bool_t Inside = kFALSE;
  Int_t ntry = 0;
  while ((!Inside) && (ntry<ntrymax)) {
    ntry++;
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
      x   = fA1*(deux*gRandom3->Rndm() - un);
      y   = fA2*(deux*gRandom3->Rndm() - un);
      z   = fA3*(deux*gRandom3->Rndm() - un);
      fX  = fCenter + x*fAxis1 + y*fAxis2 + z*fAxis3;
      break;
    }
    if (fFromFace) Inside = kTRUE;
    else Inside = IsInside();
  }
  if (ntry>=ntrymax) {
    gMes->SetName("GenX");
    gMes->SetTitle("TPhotonCradle");
    gMes->M(kill_prog,1,"Impossible to find point inside shape");
  }
}
void TPhotonCradle::InitP() {
  //Pointers to 0
  fSuppl   = 0;
  fSNode   = 0;
  fFitDist = 0;
}
Bool_t TPhotonCradle::IsInside() const {
  //Verifies that the point fX is inside the shape
  Bool_t b;
  b = fSuppl->IsInside(fX(0),fX(1),fX(2));
  return b;
}
void TPhotonCradle::SetPolar(Double_t thetp,Double_t phip) {
  //Defines the axis of polarization
  const Double_t torad = TMath::Pi()/180.0;
  Double_t st,ct,sf,cf;
  T3Vector axis;
  if (fPolarized) {
    //polarized beam inside anisotropic material is forbidden
    if (!fAni) {
      thetp *= torad;
      phip  *= torad;
      st = TMath::Sin(thetp);
      ct = TMath::Cos(thetp);
      sf = TMath::Sin(phip);
      cf = TMath::Cos(phip);
      axis.Set(st*cf,st*sf,ct);
      fSNode->AxisToWorld(axis,fPolar);
      fPolar.Unit();
    }
    else {
      gMes->SetName("Beam");
      gMes->SetTitle("TPhotonCradle");
      gMes->M(Error_M,2,"Polarized photons generated only in isotropic material");
      gMes->M(Error_M,2,"Your beam will not be polarized !");
      fPolarized = kFALSE;
      fPolar = fAxisb1;
    }
  }
  else fPolar = fAxisb1;
}
