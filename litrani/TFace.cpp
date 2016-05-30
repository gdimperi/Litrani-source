// @(#)root/html:$Id: TFace.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TMath.h"
#include "TMessErr.h"
#include "TLitGlob.h"
#include "TRevetment.h"
#include "TSupplShape.h"
#include "TFace.h"

ClassImp(TFace)
//______________________________________________________________________________
//
//  TFace describes a face of a shape
//
//  As a consequence of kind of shapes existing in ROOT, a face may be of 6
//types ( notice that in Litrani, the shapes have the same name as the
//shapes in ROOT, except for an S inserted after the T : a TBRIK becomes a
//TSBRIK for instance. This is because the shapes of Litrani inherits from
//the shapes of ROOT, with features added )
//
//    polygonal   : PLANE FACE. A face is polygonal if it has the form of any
//                  polygon, with arbitrary number of edges. For instance, a
//                  TSBRIK has 6 faces of type polygonal. All the faces of
//                  the following shapes are of type polygonal :
//                  TSBRIK, TSTRD1, TSTRD2, TSPARA, TSTRAP, TS8PTS.
//
//    circular    : PLANE FACE. A face is circular if it has the form of a
//                  disk. For instance, the basis of a TSCYL is circular. A
//                  TSCYL is a cylinder, i.e. a TSTUBE with inner radius
//                  equal to 0.
//
//    ring        : PLANE FACE. A face is of type ring if it built from 2
//                  concentric disks, the inner disk being excluded. For
//                  instance, the basis of a TSTUBE is of type ring.
//
//    cylindrical : CURVED FACE. The lateral face of a cylinder.
//
//    conical     : CURVED FACE. The lateral face of a cone ( not of a
//                  truncated cone !) i.e. the upper radius is 0.
//
//    tronconical : CURVED FACE.The lateral face of a truncated cone (TSCONE).
//
//  A Face of a shape may or may not be covered with a revetment. In
//Litrani, each time you define a shape, you have to give a default
//revetment for all faces. It means that if your shape has all its faces
//covered with the same revetment, you have nothing to do. In particular,
//you have not to define the faces : it is done automatically in the
//constructor of the shape.
//  Notice that if, AT THE SAME TIME, a face is declared to have a revetment
//AND is declared to be in contact with an other face of an other shape, the
//part of the face which is in contact with the other shape is obviously NOT
//considered as covered by the revetment : if a contact is declared between
//2 shapes, it means that a photon may go from one to the other shape, which
//would not be possible if there was a revetment !
//
//  In the case where some face has a different revetment then the default
//one, you have :
//
//    (A) - TO FIND THE FACE NUMBER of the face whose revetment must be
//changed. For that, you must know how the constructor of the shape has
//numbered the faces. It is always explained in the documentation of the
//relevant shape. Let us take the example of a TSBRIK. For a TSBRIK, the
//points are numbered in the following way :
//
//  point 0  : (-fLx, -fLy, -fLz)
//  point 1  : (+fLx, -fLy, -fLz)
//  point 2  : (+fLx, +fLy, -fLz)
//  point 3  : (-fLx, +fLy, -fLz)
//  point 4  : (-fLx, -fLy, +fLz)
//  point 5  : (+fLx, -fLy, +fLz)
//  point 6  : (+fLx, +fLy, +fLz)
//  point 7  : (-fLx, +fLy, +fLz)
//
//  These 8 points are contained in the data member list *fPts of TFace. They
//are also contained in the data member list fPoints of TSBRIK.
//
//    The 6 faces of the TSBRIK are built from these points in the following
//way :
//
//  face 0  : defined by points 0 1 5 4
//  face 1  : defined by points 1 2 6 5
//  face 2  : defined by points 2 3 7 6
//  face 3  : defined by points 3 0 4 7
//  face 4  : defined by points 1 0 3 2
//  face 5  : defined by points 4 5 6 7
//
//  In the case of a shape different from a TSBRIK, refer to the documentation
//of the relevant shape to get the same information.
//
//    (B) - TO GET THE POINTER TO THIS FACE
//
//  Let us suppose that p is for instance the pointer on a TSBRIK and k is
//the face number of the face you want to change the revetment. For each
//shape in Litrani, there is an associated class TSupplShape that you can
//reach using the public pointer fSuppl associated to each shape. You can
//get the pointer to face number k like this :
//
//    TFace *q = p->fSuppl->GetFace(k);
//    q->ChangeRevetment("Aluminium")  //will change revetment of face k
//                                     //to aluminium ( provided a revetment
//                                     //of type Aluminium has been defined
//                                     // before ).
//
//  FACE WITH MANY REVETMENTS
//
//  Now let us turn to a more complicated case : the case where one face has
//more than one revetment ! Until now, we had never to define a Face ourself,
//we had just to change the revetment of an existing face. All the faces
//which are not created by the user, but are created automatically by the
//constructor of the TSHAPE are called master faces. Now if a master face is
//to have many different types of revetment, it means that it has to be
//subdivided into many subfaces and these subfaces are to be defined by the
//user. Only plane (polygonal, circular or ring ) subfaces are permitted by
//Litrani. The shape of the subface to be defined by the user is described
//by means of additionnal points added to the list of the points of the shape. 
//
//  If the subface is of type polygonal, the additionnal points to be given
//are the vertices of the polygon.
//
//  If the subface is of type circular, 3 NOT ALIGNED points are to be given : 
//    point 0  : center of the circle,
//    point 1  : any point on the circle,
//    point 2  : any other point on the circle.
//
//  If the subface is of type ring, 3 NOT ALIGNED points are to be given :
//    point 0  : center of the ring,
//    point 1  : any point on the large circle,
//    point 2  : any  point on the small circle.
//
//  Let us take as example a TSBRIK ( with fLx = fLy = fLz = 1.0 ) covered
//with mylar, having on face 0 a small circle covered with aluminium.
//Remember that according to point (A), face 0 is the one composed of
//points 0,1,5,4, all having an y coordinate of -fLy. The circle be centered
//on the face, and has a radius of 0.1 .  p is the pointer on the TSBRIK,
//ps the pointer onto the TSupplShape associated with this TSBRIK.
//
//    T3Vector *p0 = new T3Vector(0.0,-1.0,0.0);  //center of circle
//    T3Vector *p1 = new T3Vector(0.1,-1.0,0.0);  //a point on the circle
//    T3Vector *p2 = new T3Vector(0.0,-1.0,0.1);  //other point on the circle
//    TSupplShape *ps = p->fSuppl;
//    ps->AddSubPoint(p0);  //new point  8 added to the list of points
//    ps->AddSubPoint(p1);  //new point  9 added to the list of points
//    ps->AddSubPoint(p2);  //new point 10 added to the list of points
//    TFace *q;             //now we define the subface 6 belonging to
//                          //the masterface 0
//    q = new TFace(p,6,0,circular,ps->fPoints,
//                  ps->fGCenter,"Aluminium",8,9,10);
//    ps->AddSubFace(q);    //let TSBRIK know there is one face more !
//
//  For the sake of completness, let us define here how all kind of faces are
//defined on the basis of points :
//
// case polygonal    : N points
//
//  point i  : vertex i of polygon
//
// case circular     : 3 points NOT ALIGNED !!
//
//  point 0  : center of the circle
//  point 1  : any point on the circle
//  point 2  : any other point on the circle
//
// case ring         : 3 points  NOT ALIGNED !!!
//
//  point 0  : center of ring : DOES NOT belong to the surface
//  point 1  : any point on the large circle
//  point 2  : any point on the small circle
//
// case cylindrical  : 3 points
//
//  point 0  : center of the lower circular basis of the cylinder.
//             DOES NOT belong to the surface
//  point 1  : any point on the lower circular basis
//  point 2  : center of the upper circular basis of the cylinder.
//             DOES NOT belong to the surface
//
//  case conical      : 3 points
//
//  point 0  : center of the circular basis of the cone.
//             DOES NOT belong to the surface !
//  point 1  : any point on the circle of the basis
//  point 2  : top of cone
//
//  case tronconical  : 4 points : 0 2 3 MUST BE ALIGNED !!
//
//  point 0  : center of the lower basis of the truncated cone.
//             DOES NOT belong to the surface
//  point 1  : any point on the circle of the lower basis.
//  point 2  : top of cone if not truncated.
//             DOES NOT belong to the surface
//  point 3  : center of the small basis of the truncated cone.
//             DOES NOT belong to the surface
//
//
//    IMPORTANT NOTE CONCERNING THE COORDINATE SYSTEM
//
//    Immediatly after the creation of a shape, all coordinates ( and in
//particular all coordinates of the points of the list fPoints ) are in the
//system of coordinate local to the shape. This means that if you have to add
//points, for instance to define a subface, you must give these points in the
//local coordinates of the shape, which is by far the handiest one. But as
//soon as the TSNode associated with this shape is declared, the coordinates
//of the points and vectors of the shape are transformed into the WCS (World
//Coordinate system), which is the local coordinate system of the first shape
//(the master shape) that you have defined. This means that if you wanted to
//define a subface AFTER the definition of the TNode, you would have to
//define your subpoints in the world coordinate system, which is most of the
//time terribly difficult. Conclusion : define your subfaces IMMEDIATLY AFTER
//having defined your shape.
//
//
//    DEPOLISHED SURFACES
//
//  A new possibility has been added to Litrani : it is now possible to
//simulate a depolished surface, by a call to the method :
//    SetDepolished(ThetaDepls).
//
//Depolishing is simulated in the following way :
//
//   when the surface is declared depolished, the normal to the surface at
//the point hit by the photon is randomly tilted ( with respect to the true
//normal of the surface ) by an angle theta. theta is generated randomly
//according to a distribution sin(theta)dtheta dphi, between 0 and ThetaDepls.
//So ThetaDepls (0° < ThetaDepls < 90°) can adjust the roughness of the
//grinding. You have to give ThetaDepls in degree. If you omit it, the value
//of 60° will be taken.
//
//  Important restrictions :
//
//  The use of grinded ( depolished ) surface is only allowed for the faces
//of a shape containing an isotropic material. For an anisotropic material,
//Litrani gives up, for reasons explained below. Notice also that even if
//you declare a face to be grinded (depolished), the part of this face which
//is in contact with an other face of an other shape is never treated as
//grinded by Litrani. Only the part of the face which is NOT in contact
//with an other face (be it covered by a revetment or not) is treated as
//grinded. An other restriction is that you have not the right to declare as
//depolished a face having a revetment WITHOUT slice (of air or something
//else) between the face and the revetment.
//
//    Notice the following problems in this rather simple-minded way of
//simulating depolishing : 
//
// 1 - after the normal at the point hit by the photon has been randomly
//tilted, it may be that the angle of this new normal with the Poyinting
//vector of the photon has become greater than 90 degree, which would mean
//that the photon does not reach any more the surface at this point and would
//give meaningless results in the calculation of the Fresnel equations !
//If this happens, Litrani simply generates again theta until this does not
//happen any more. 
// 2 - after the normal at the point hit by the photon has been randomly
//tilted, it may be that in case of total reflection, the outgoing photon
//does not any more reenter the convex polyhedron ! If this happens, we
//simply generate again theta until this does not happen any more. 
// 3 - after the normal at the point hit by the photon has been randomly tilted,
//it may be that in case of refraction, the outgoing wave would make an angle
//greater than 90 degree with the untilted normal to the surface, i-e the
//refracted photon would not reach the revetment ! If this happens, we simply
//generate again t until this does not happen any more. 
// 4 - As in the case of anisotropy, angle of reflection is not necessarily
//equal to the incident angle, it is much too complicate to calculate the
//possibility that difficulty (2) or (3) arises or not. For that reason, we
//have forbidden the use of grinded surface associated with a shape
//containing an anisotropic material. 
// 5 - when the photon does not make a total reflection, but goes out of the
//face towards the revetment, and is reflected back by the revetment, we have
//not implemented for that "return" case a grinded surface, but in that case we
//leave the normal to the surface as it is ( as if the surface was polished ).
//Had we not done that simplification, we would have had the difficulties (1)
//(2) and (3) again on return, which we considered too much ! 
//
//  So keep in mind that our implementation of grinding (depolishing) is
//rather simplified !
//
TFace::TFace(TShape *shape, Int_t number,Int_t master, KindOfFace kind, TObjArray *pts,
  T3Vector &gc, const char *NameRevetment, Int_t i0, Int_t i1, Int_t i2, Bool_t reverse) {
  // constructor when there is exactly 3 points to define the surface and the revetment is known
  //by its name.
  //
  // shape           : shape to which this face belongs
  // number          : arbitrary number given to this face, <= total number of faces
  // master          : if this face is a subface, master is the face number of the master face,
  //                   else master must be equal to -1.
  // kind            : kind of face : polygonal, circular, ring, cylindrical, conical, tronconical.
  // pts             : list of points of the associated shape (also including points not belonging to
  //                   this face!)
  // gc              : pointer to center of gravity of all points in associated shape
  // NameRevetment   : name of the revetment covering this face
  // i0,i1,i2        : list of the 3 points (among the points of the shape) belonging to this face
  // reverse         : true if normal has to be reversed. See comment in method Normal().
  //
  InitP();
  fShape  = shape;
  fDet    = none;   fNDet = -1;
  InitTFace(number,master);
  fPts = pts;
  fGc = gc;
  fKind = kind;
  ChangeRevetment(NameRevetment);
  fNbPoints = 3;
  fListPts.Set(fNbPoints);
  fListPts[0] = i0;
  fListPts[1] = i1;
  fListPts[2] = i2;
  if (master<0) fReverse = reverse;
  else fReverse = kFALSE;
  Parameters(kTRUE);
  fSurface = Surface();
  fCharLength = TMath::Sqrt(fSurface);
}
TFace::TFace(TShape *shape, Int_t number,Int_t master, KindOfFace kind, TObjArray *pts,
  T3Vector &gc,TRevetment *prev, Int_t i0, Int_t i1, Int_t i2, Bool_t reverse) {
  // constructor when there is exactly 3 points to define the surface and the pointer to the
  //revetment is available.
  //
  // shape           : shape to which this face belongs
  // number          : arbitrary number given to this face, <= total number of faces
  // master          : if this face is a subface, master is the face number of the master face,
  //                   else master must be equal to -1.
  // kind            : kind of face : polygonal, circular, ring, cylindrical, conical, tronconical.
  // pts             : list of points of the associated shape (also including points not belonging to
  //                   this face!)
  // gc              : pointer to center of gravity of all points in associated shape
  // prev            : pointer to the revetment covering this face
  // i0,i1,i2        : list of the 3 points (among the points of the shape) belonging to this face
  // reverse         : true if normal has to be reversed. See comment in method Normal().
  //
  InitP();
  fShape  = shape;
  fDet    = none;   fNDet = -1;
  InitTFace(number,master);
  fPts = pts;
  fGc = gc;
  fKind = kind;
  fRevetment = prev;
  if (fRevetment) fRevetment->SetInUse();
  fNbPoints = 3;
  fListPts.Set(fNbPoints);
  fListPts[0] = i0;
  fListPts[1] = i1;
  fListPts[2] = i2;
  if (master<0) fReverse = reverse;
  else fReverse = kFALSE;
  Parameters(kTRUE);
  fSurface = Surface();
  fCharLength = TMath::Sqrt(fSurface);
}
TFace::TFace(TShape *shape, Int_t number,Int_t master, KindOfFace kind, TObjArray *pts,
  T3Vector &gc,const char *NameRevetment, Int_t i0, Int_t i1, Int_t i2, Int_t i3,
  Bool_t reverse) {
  // constructor when there is exactly 4 points to define the surface and the
  //revetment is known by its name.
  //
  // shape           : shape to which this face belongs
  // number          : arbitrary number given to this face, <= total number
  //                    of faces
  // master          : if this face is a subface, master is the face number
  //                    of the master face,
  //                   else master must be equal to -1.
  // kind            : kind of face : polygonal, circular, ring, cylindrical,
  //                    conical, tronconical.
  // pts             : list of points of the associated shape (also including
  //                    points not belonging to this face!)
  // gc              : pointer to center of gravity of all points in
  //                    associated shape
  // NameRevetment   : name of the revetment covering this face
  // i0,...,i3       : list of the 4 points (among the points of the shape)
  //                    belonging to this face
  // reverse         : true if normal has to be reversed. See comment in
  //                    method Normal().
  //
  InitP();  
  fShape  = shape;
  fDet    = none;   fNDet = -1;
  InitTFace(number,master);
  fPts = pts;
  fGc = gc;
  fKind = kind;
  ChangeRevetment(NameRevetment);
  fNbPoints = 4;
  fListPts.Set(fNbPoints);
  fListPts[0] = i0;
  fListPts[1] = i1;
  fListPts[2] = i2;
  fListPts[3] = i3;
  if (master<0) fReverse = reverse;
  else fReverse = kFALSE;
  Parameters(kTRUE);
  fSurface = Surface();
  fCharLength = TMath::Sqrt(fSurface);
}
TFace::TFace(TShape *shape, Int_t number,Int_t master, KindOfFace kind, TObjArray *pts,
  T3Vector &gc,TRevetment *prev, Int_t i0, Int_t i1, Int_t i2, Int_t i3, Bool_t reverse) {
  // constructor when there is exactly 4 points to define the surface and the pointer to the
  //revetment is available.
  //
  // shape           : shape to which this face belongs
  // number          : arbitrary number given to this face, <= total number of faces
  // master          : if this face is a subface, master is the face number of the master face,
  //                   else master must be equal to -1.
  // kind            : kind of face : polygonal, circular, ring, cylindrical, conical, tronconical.
  // pts             : list of points of the associated shape (also including points not belonging to
  //                   this face!)
  // gc              : pointer to center of gravity of all points in associated shape
  // prev            : pointer to the revetment covering this face
  // i0,...,i3       : list of the 4 points (among the points of the shape) belonging to this face
  // reverse         : true if normal has to be reversed. See comment in method Normal().
  // 
  InitP();
  fShape  = shape;
  fDet    = none;   fNDet = -1;
  InitTFace(number,master);
  fPts = pts;
  fGc = gc;
  fKind = kind;
  fRevetment = prev;
  if (fRevetment) fRevetment->SetInUse();
  fNbPoints = 4;
  fListPts.Set(fNbPoints);
  fListPts[0] = i0;
  fListPts[1] = i1;
  fListPts[2] = i2;
  fListPts[3] = i3;
  if (master<0) fReverse = reverse;
  else fReverse = kFALSE;
  Parameters(kTRUE);
  fSurface = Surface();
  fCharLength = TMath::Sqrt(fSurface);
}
TFace::TFace(TShape *shape, Int_t number,Int_t master, KindOfFace kind, TObjArray *pts,
  T3Vector &gc,const char *NameRevetment, Int_t i0, Int_t i1, Int_t i2, Int_t i3,
  Int_t i4, Bool_t reverse) {
  // constructor when there is exactly 5 points to define the surface and the revetment is known
  //by its name.
  //
  // shape           : shape to which this face belongs
  // number          : arbitrary number given to this face, <= total number of faces
  // master          : if this face is a subface, master is the face number of the master face,
  //                   else master must be equal to -1.
  // kind            : kind of face : polygonal, circular, ring, cylindrical, conical, tronconical.
  // pts             : list of points of the associated shape (also including points not belonging to
  //                   this face!)
  // gc              : pointer to center of gravity of all points in associated shape
  // NameRevetment   : name of the revetment covering this face
  // i0,...,i4       : list of the 5 points (among the points of the shape) belonging to this face
  // reverse         : true if normal has to be reversed. See comment in method Normal().
  //
  InitP();
  fShape  = shape;
  fDet    = none;   fNDet = -1;
  InitTFace(number,master);
  fShape  = shape;
  fPts = pts;
  fGc = gc;
  fKind = kind;
  ChangeRevetment(NameRevetment);
  fNbPoints = 5;
  fListPts.Set(fNbPoints);
  fListPts[0] = i0;
  fListPts[1] = i1;
  fListPts[2] = i2;
  fListPts[3] = i3;
  fListPts[4] = i4;
  if (master<0) fReverse = reverse;
  else fReverse = kFALSE;
  Parameters(kTRUE);
  fSurface = Surface();
  fCharLength = TMath::Sqrt(fSurface);
}
TFace::TFace(TShape *shape, Int_t number,Int_t master, KindOfFace kind, TObjArray *pts,
  T3Vector &gc,TRevetment *prev, Int_t i0, Int_t i1, Int_t i2, Int_t i3, Int_t i4,
  Bool_t reverse) {
  // constructor when there is exactly 5 points to define the surface and the pointer to the
  //revetment is available.
  //
  // shape           : shape to which this face belongs
  // number          : arbitrary number given to this face, <= total number of faces
  // master          : if this face is a subface, master is the face number of the master face,
  //                   else master must be equal to -1.
  // kind            : kind of face : polygonal, circular, ring, cylindrical, conical, tronconical.
  // pts             : list of points of the associated shape (also including points not belonging to
  //                   this face!)
  // gc              : pointer to center of gravity of all points in associated shape
  // prev            : pointer to the revetment covering this face
  // i0,...,i4       : list of the 5 points (among the points of the shape) belonging to this face
  // reverse         : true if normal has to be reversed. See comment in method Normal().
  //
  InitP();
  fShape  = shape;
  fDet    = none;   fNDet = -1;
  InitTFace(number,master);
  fPts = pts;
  fGc = gc;
  fKind = kind;
  fRevetment = prev;
  if (fRevetment) fRevetment->SetInUse();
  fNbPoints = 5;
  fListPts.Set(fNbPoints);
  fListPts[0] = i0;
  fListPts[1] = i1;
  fListPts[2] = i2;
  fListPts[3] = i3;
  fListPts[4] = i4;
  if (master<0) fReverse = reverse;
  else fReverse = kFALSE;
  Parameters(kTRUE);
  fSurface = Surface();
  fCharLength = TMath::Sqrt(fSurface);
}
TFace::TFace(TShape *shape, Int_t number,Int_t master, KindOfFace kind, TObjArray *pts,
  T3Vector &gc,const char *NameRevetment, Int_t i0, Int_t i1, Int_t i2, Int_t i3,
  Int_t i4, Int_t i5, Bool_t reverse) {
  // constructor when there is exactly 6 points to define the surface and the revetment is known
  //by its name.
  //
  // shape           : shape to which this face belongs
  // number          : arbitrary number given to this face, <= total number of faces
  // master          : if this face is a subface, master is the face number of the master face,
  //                   else master must be equal to -1.
  // kind            : kind of face : polygonal, circular, ring, cylindrical, conical, tronconical.
  // pts             : list of points of the associated shape (also including points not belonging to
  //                   this face!)
  // gc              : pointer to center of gravity of all points in associated shape
  // NameRevetment   : name of the revetment covering this face
  // i0,...,i5       : list of the 6 points (among the points of the shape) belonging to this face
  // reverse         : true if normal has to be reversed. See comment in method Normal().
  //
  InitP();
  fShape  = shape;
  fDet    = none;   fNDet = -1;
  InitTFace(number,master);
  fPts = pts;
  fGc = gc;
  fKind = kind;
  ChangeRevetment(NameRevetment);
  fNbPoints = 6;
  fListPts.Set(fNbPoints);
  fListPts[0] = i0;
  fListPts[1] = i1;
  fListPts[2] = i2;
  fListPts[3] = i3;
  fListPts[4] = i4;
  fListPts[5] = i5;
  if (master<0) fReverse = reverse;
  else fReverse = kFALSE;
  Parameters(kTRUE);
  fSurface = Surface();
  fCharLength = TMath::Sqrt(fSurface);
}
TFace::TFace(TShape *shape, Int_t number,Int_t master, KindOfFace kind, TObjArray *pts,
  T3Vector &gc,TRevetment *prev, Int_t i0, Int_t i1, Int_t i2, Int_t i3,Int_t i4,
  Int_t i5, Bool_t reverse) {
  // constructor when there is exactly 6 points to define the surface and the pointer to the
  //revetment is available.
  //
  // shape           : shape to which this face belongs
  // number          : arbitrary number given to this face, <= total number of faces
  // master          : if this face is a subface, master is the face number of the master face,
  //                   else master must be equal to -1.
  // kind            : kind of face : polygonal, circular, ring, cylindrical, conical, tronconical.
  // pts             : list of points of the associated shape (also including points not belonging to
  //                   this face!)
  // gc              : pointer to center of gravity of all points in associated shape
  // prev            : pointer to the revetment covering this face
  // i0,...,i5       : list of the 6 points (among the points of the shape) belonging to this face
  // reverse         : true if normal has to be reversed. See comment in method Normal().
  //
  InitP();
  fShape  = shape;
  fDet    = none;   fNDet = -1;
  InitTFace(number,master);
  fPts = pts;
  fGc = gc;
  fKind = kind;
  fRevetment = prev;
  if (fRevetment) fRevetment->SetInUse();
  fNbPoints = 6;
  fListPts.Set(fNbPoints);
  fListPts[0] = i0;
  fListPts[1] = i1;
  fListPts[2] = i2;
  fListPts[3] = i3;
  fListPts[4] = i4;
  fListPts[5] = i5;
  if (master<0) fReverse = reverse;
  else fReverse = kFALSE;
  Parameters(kTRUE);
  fSurface = Surface();
  fCharLength = TMath::Sqrt(fSurface);
}
TFace::TFace(TShape *shape, Int_t number,Int_t master, KindOfFace kind, TObjArray *pts,
  T3Vector &gc,const char *NameRevetment, TArrayI &ai, Bool_t reverse) {
  // constructor when there are more than 6 points to define the polygonal surface and the
  //Revetment is known by its name.
  //
  // shape           : shape to which this face belongs
  // number          : arbitrary number given to this face, <= total number of faces
  // master          : if this face is a subface, master is the face number of the master face,
  //                   else master must be equal to -1.
  // kind            : kind of face : polygonal, circular, ring, cylindrical, conical, tronconical.
  // pts             : list of points of the associated shape (also including points not belonging to
  //                   this face!)
  // gc              : pointer to center of gravity of all points in associated shape
  // NameRevetment   : name of the revetment covering this face
  // ai              : list of the N points (among the points of the shape) belonging to this face
  // reverse         : true if normal has to be reversed. See comment in method Normal().
  //
  InitP();
  fShape  = shape;
  fDet    = none;   fNDet = -1;
  InitTFace(number,master);
  fPts = pts;
  fGc = gc;
  fKind = kind;
  ChangeRevetment(NameRevetment);
  fNbPoints = ai.fN;
  fListPts.Set(fNbPoints);
  for (Int_t i=0;i<fNbPoints;i++) {
    fListPts[i] = ai[i];
  }
  if (master<0) fReverse = reverse;
  else fReverse = kFALSE;
  Parameters(kTRUE);
  fSurface = Surface();
  fCharLength = TMath::Sqrt(fSurface);
}
TFace::TFace(TShape *shape, Int_t number,Int_t master, KindOfFace kind, TObjArray *pts,
  T3Vector &gc,TRevetment *prev, TArrayI &ai, Bool_t reverse) {
  // constructor when there are more than 6 points to define the polygonal surface and the
  //Revetment is known by its name.
  //
  // shape           : shape to which this face belongs
  // number          : arbitrary number given to this face, <= total number of faces
  // master          : if this face is a subface, master is the face number of the master face,
  //                   else master must be equal to -1.
  // kind            : kind of face : polygonal, circular, ring, cylindrical, conical, tronconical.
  // pts             : list of points of the associated shape (also including points not belonging to
  //                   this face!)
  // gc              : pointer to center of gravity of all points in associated shape
  // prev            : pointer to the revetment covering this face
  // ai              : list of the N points (among the points of the shape) belonging to this face
  // reverse         : true if normal has to be reversed. See comment in method Normal().
  //
  InitP();
  fShape  = shape;
  fDet    = none;   fNDet = -1;
  InitTFace(number,master);
  fPts = pts;
  fGc = gc;
  fKind = kind;
  fRevetment = prev;
  if (fRevetment) fRevetment->SetInUse();
  fNbPoints = ai.fN;
  fListPts.Set(fNbPoints);
  for (Int_t i=0;i<fNbPoints;i++) {
    fListPts[i] = ai[i];
  }
  if (master<0) fReverse = reverse;
  else fReverse = kFALSE;
  Parameters(kTRUE);
  fSurface = Surface();
  fCharLength = TMath::Sqrt(fSurface);
}
TFace::~TFace() {
  // Destructor. TFace is not owner of fShape, fSuppl, fPts, fRevetment, fQE. Do
  //not delete them
  Int_t N;
  N = fRidge.GetEntries();
  if (N>0) fRidge.Delete();
  N = fOutRidge.GetEntries();
  if (N>0) fOutRidge.Delete();
}
void TFace::ChangeRevetment(const char *name) {
  //Assign an other revetment to this face. If the name of the revetment is "none", this face will
  //have no revetment. Argument is here of type const char* .
  //
  TRevetment *pRev;  
  TIter next(&gLit->fRevetment);
  TString s,s1;
  s1 = name;
  Bool_t found = kFALSE;
  TString snone = "none";
  if (!s1.CompareTo(snone))
    fRevetment = 0;
  else {
    while ((!found) && (pRev = (TRevetment *)next())) {
      s = pRev->GetName();
      if (!s.CompareTo(name)) {
        found      = kTRUE;
        fRevetment = pRev;
        pRev->SetInUse();
      }
    }
    if ( !found ) {
      gMes->SetName("ChangRevetment");
      gMes->SetTitle("TFace");
      gMes->M(kill_prog,1,"Revetment not found for this face",fNumber);
    }
  }
}
void TFace::ChangeRevetment(TString &s1) {
  //Assign an other revetment to this face. If the name of the revetment is "none", this face will
  //have no revetment. Argument is here of type TString .
  //
  TRevetment *pRev;
  TIter next(&gLit->fRevetment);
  TString s;
  Bool_t found = kFALSE;
  TString snone="none";
  if (!s1.CompareTo(snone))
    fRevetment = 0;
  else {
    while ((!found) && (pRev = (TRevetment *)next())) {
      s = pRev->GetName();
      if (!s.CompareTo(s1)) {
        found      = kTRUE;
        fRevetment = pRev;
        pRev->SetInUse();
      }
    }
    if ( !found ) {
      gMes->SetName("ChangRevetment");
      gMes->SetTitle("TFace");
      gMes->M(kill_prog,2,"Revetment not found for this face",fNumber);
    }
  }
}
Int_t  TFace::Compare(const TObject *obj) const {
  if (fSurface > ((TFace *)obj)->fSurface)
    return -1;
  else if (fSurface < ((TFace *)obj)->fSurface)
    return 1;
  else
    return 0;
}
TContact* TFace::Contact(Int_t i) const {
  return ((TOtherSide *)fContactSide[i])->GetContact();
}
TFace* TFace::ContactFace(Int_t i) const {
  return ((TOtherSide *)fContactSide[i])->GetFace();
}
TShape* TFace::ContactShape(Int_t i) const {
  return ((TOtherSide *)fContactSide[i])->GetShape();
}
TSupplShape* TFace::ContactSuppl(Int_t i) const {
  return ((TOtherSide *)fContactSide[i])->GetSuppl();
}
KindOfContact TFace::ContactType(Int_t i) const { 
  return ((TOtherSide *)fContactSide[i])->GetKind(); 
}
Bool_t TFace::Contained(const T3Vector &v) {
  //Determines whether a point is inside a surface. (Inside means here on the
  //surface, on the inner side with respect to all edges of the surface).
  const Double_t zero = 0.0;
  const Double_t tolerance = 1.0e-6;
  Bool_t reverse;
  Int_t nsol,nsoli;
  Double_t t1,t2,dist,dist1,dist2;
  T3Vector sol1,sol2,nd;
  Bool_t IsIn = kFALSE;
  if (fPlanar) nd = Normal();
  else nd = Normal(v);
  dist = CrossFace(v,nd,t1,sol1,nsol,nsoli,t2,sol2,dist1,dist2,-tolerance);
  t1  = TMath::Abs(t1);
  t2  = TMath::Abs(t2);
  reverse = t2<t1;
  switch (nsol) {
  case 1:
    IsIn = ((dist1 <= zero) && (t1<tolerance));
    break;
  case 2:
    if (reverse) IsIn = ((dist2 <= zero) && (t2<tolerance));
    else         IsIn = ((dist1 <= zero) && (t1<tolerance));
    break;
  default:
    break;
  }
  return IsIn;
}
Double_t TFace::CosDepolishAngle(Double_t dist) const {
  //  Returns the cosine of the depolish angle in case of bevellings, i.e.
  //when the state of the surface is not the same when near the ridge. dist
  //is the distance from the nearest ridge.
  Double_t cda;
  if (fSuppl->HasBevellings()) {
    if (dist<fSuppl->SizeBevellings()) cda = fSuppl->CosDplAngle();
    else cda = fCosThetaDepls;
  }
  else cda = fCosThetaDepls;
  return cda;
}
Double_t TFace::CrossFace(const T3Vector &x0,const T3Vector &nd,Double_t &t1,
  T3Vector &sol1,Int_t &nsol, Int_t &nsoli, Double_t &t2, T3Vector &sol2,
  Double_t &dist1, Double_t &dist2, Double_t tcut) const {
  //
  //  Finds whether this face is or is not crossed by a straight line.
  //
  //  PARAMETERS :
  //
  //    x0    : the straight line passes by the point x0.
  //    nd    : unit vector direction of the straight line.
  //    t1    : value of the parameter along the straight line to get the 1st
  //            solution.  sol1 = x0 + t1*nd in case of good 1st solution.
  //    sol1  : 1st solution, if any. sol1 is always chosen as being the
  //            FARTHEST from x0 in case there are 2 solutions.
  //    nsol  : number of solutions : 0, 1 or 2 for the intersection with the
  //            BOUNDED surface. Faces in ROOT are made of surfaces of at
  //            most 2nd order.
  //    nsoli : number of solutions for the intersection with the INFINITE
  //            surface in which the face is imbedded. These solutions are
  //            givent by t1 and t2 in any case.
  //    t2    : value of the parameter along the straight line to get the 2nd
  //            solution.  sol2 = x0 + t2*nd in case of good 2nd solution.
  //    sol2  : 2nd solution, if any. sol2 is always chosen as being the
  //            NEAREST from x0 in case there are 2 solutions. (This is becau-
  //            se very often one is in the situation where x0 is already on
  //            the 2nd order surface and one looks for the other
  //            intersection.)
  //    dist1 : Distance of the 1st solution to the BOUNDED surface.
  //            (a) - If ((dist1<=0) && (nsol>=1)) the 1st solution is good.
  //                 In that case : sol1 = x0 + t1*nd
  //            (b) - If ((dist1>0)  && (nsol>=1)) and dist1 small, 1st solu-
  //                 tion is almost good. The intersection was outside the
  //                 BOUNDED surface by the small distance dist1. We still
  //                 take the solution as good to cope with numerical impre-
  //                 cision, but the user is free either to reject this solu-
  //                 tion or to make more severe cuts on dist1.
  //                 In that case : sol1 != x0 + t1*nd !!!  x0 + t1*nd is the
  //                 true intersection, slightly outside the BOUNDED surface,
  //                 sol1 has been pushed onto the BOUNDED surface.
  //             (c) - If ((dist1>0 && (nsol== 0)) there is no intersection
  //                 with the BOUNDED surface and dist1 is of the order of the
  //                 distance to the BOUNDED surface.
  //    dist2 : Distance of the 2nd solution from the BOUNDED surface.
  //            (a) - If ((dist2<=0) && (nsol ==2)) the 2nd solution is good.
  //                 In that case : sol2 = x0 + t2*nd
  //            (b) - If ((dist2>0)  && (nsol ==2)) and dist2 small, 2nd solu-
  //                 tion is almost good. The intersection was outside the
  //                 BOUNDED surface by the small distance dist2. We still
  //                 take the solution as good to cope with numerical impre-
  //                 cision, but the user is free either to reject this solu-
  //                 tion or to make more severe cuts on dist2.
  //                 In that case : sol2 != x0 + t2*nd !!! x0 + t2*nd is the
  //                 true intersection, slightly outside the BOUNDED surface,
  //                 sol2 has been pushed onto the BOUNDED surface.
  //             (c) - If ((dist2>0 && (nsol <= 1)) there is no 2nd intersec-
  //                 tion with the BOUNDED surface and dist2 is of the order
  //                 of the distance to the BOUNDED surface of the 2nd inter-
  //                 section.
  //    tcut  : value of t below which a solution is NOT accepted. This
  //            parameter is useful for instance in the case where one does
  //            not want to find intersections which are behind the current
  //            point *this. In that case, put tcut=0 !
  //
  //  RETURNED VALUE
  //
  //    dist  : We call dist the return value of the method CrossFace. If the
  //            face is plane, dist and dist1 are equal and have the same
  //            meaning. If the face is not plane, dist concerns not the in-
  //            tersection with the bounded face, but the intersection with
  //            the INFINITE 2nd order surface in which the face is imbedded.
  //            (a) - If ((dist <= 0) && (nsoli == 2)), it means that there
  //                are 2 intersections with the INFINITE surface.
  //            (b) - If ((dist >  0) && (nsoli >= 1)) and dist small, there
  //                is almost one intersection with the INFINITE surface. The
  //                intersection was outside the INFINITE surface by the small
  //                distance dist. We still take the solution as good to cope
  //                with numerical imprecision, but the user is free either to
  //                reject this solution or to make more severe cuts on dist.
  //            (c) - If ((dist >  0) && (nsoli  = 0)) there is no intersec-
  //                tion with the INFINITE surface containing the face. In
  //                that case, nsol is also 0.
  //  Let us look at the meaning of dist, dist1 and dist2 for all cases of
  //faces :
  //
  //  Face     dist                 dist1                dist2
  //
  //  polygon  dist               = dist1              = dist2
  //           distance to polygon  distance to polygon  distance to polygon
  //  disk     dist               = dist1              = dist2
  //           distance to disk     distance to disk     distance to disk
  //  ring     dist               = dist1              = dist2
  //           distance to ring     distance to ring     distance to ring
  //  cylinder dist              != dist1             != dist2
  //           distance of line     distance of 1st sol. distance of 2nd sol.
  //           to infinite cylinder to bounded cylinder  to bounded cylinder.
  //  cone     dist              != dist1             != dist2
  //           distance of line     distance of 1st sol. distance of 2nd sol.
  //           to infinite cone     to bonded cone       to bounded cone.
  //
  Double_t dist = 0.0;
  T3Vector *cc = 0;
  nsoli = 1;
  if (fKind != polygonal)
    cc = (T3Vector *)fPtsFace[0];
  switch (fKind) {
  case polygonal:
    dist  = x0.CrossPolygon(nd,t1,sol1,nsol,fNbPoints,fN,fPtsFace,fOutRidge,
      tcut,fCharLength);
    dist1 = dist;
    dist2 = dist;
    t2    = t1;
    sol2  = sol1;
    break;
  case circular:
    dist  = x0.CrossDisk(nd,t1,sol1,nsol,fN,*cc,fRadiusBase1,tcut);
    dist1 = dist;
    dist2 = dist;
    t2    = t1;
    sol2  = sol1;
    break;
  case ring:
    dist  = x0.CrossRing(nd,t1,sol1,nsol,fN,*cc,fRadiusBase1,fRadiusBase2,
      tcut);
    dist1 = dist;
    dist2 = dist;
    t2    = t1;
    sol2  = sol1;
    break;
  case cylindrical:
    dist = x0.CrossCylinder(nd,nsol,nsoli,t1,t2,sol1,sol2,*cc,fAxis,
      fRadiusBase1,fHeight,dist1,dist2,tcut);
    break;
  case conical:
  case tronconical:
    dist = x0.CrossTCone(nd,nsol,nsoli,t1,t2,sol1,sol2,*cc,fAxis,
      fRadiusBase1,fHeight,fConeHeight,dist1,dist2,tcut);
    break;
  }
  return dist;
}
TRevetment* TFace::GetRevetment(Double_t dist) const {
  //  Get the revetment of the face when there are bevellings, i.e. when the
  //revetment is not the same when near the ridge. dist is the distance from
  //the nearest ridge.
  TRevetment *rev;
  if (fSuppl->HasBevellings()) {
    if (dist<fSuppl->SizeBevellings()) rev = fSuppl->RevBevellings();
    else rev = fRevetment;
  }
  else rev = fRevetment;
  return rev;
}
void TFace::InitP() {
  //All pointers to 0
  fShape     = 0;
  fSuppl     = 0;
  fPts       = 0;
  fRevetment = 0;
  fQE        = 0;
}
void TFace::InitTFace(Int_t number,Int_t master) {
  //Finds the pointer to the TSupplShape associated with fShape,Handles the
  //variables concerning master and subfaces, and set initially the surface as
  //polished.
  const char *t = fShape->GetName();
  TString s;
  TSupplShape *p;
  TIter next(&gLit->fSupplShape);
  Bool_t found = kFALSE;
  TSupplShape *ptsuppl = 0;
  while ((!found) && (p = (TSupplShape *)next())) {
    s = p->GetName();
    if (!s.CompareTo(t)) {
      found   = kTRUE;
      fSuppl  = p;
      ptsuppl = p;
    }
  }
  if (!found) {
    gMes->SetName("InitTFace");
    gMes->SetTitle("TFace");
    gMes->M(kill_prog,1,"TSupplShape not found");
  }
  fNumber = number;
  fMasterFace = master;
  fNbSubFaces = 0;
  if (master >= 0) {
    TFace *pmaster;
    pmaster = ptsuppl->GetFace(master);
    pmaster->fNbSubFaces += 1;
    pmaster->fSubFaces.Add(this);
  }
  fIsDepolished  = kFALSE;
  fThetaDepls    = 0.0;
  fCosThetaDepls = 1.0;
}
Bool_t TFace::IsDepolished(Double_t dist) const {
  //  Tells whether surface depolished, in case of bevellings, i.e. when the
  //state of the surface is not the same when near the ridge. dist is the
  //distance from the nearest ridge.
  Bool_t b;
  if (fSuppl->HasBevellings()) {
    if (dist<fSuppl->SizeBevellings()) b = fSuppl->DplBevellings();
    else b = fIsDepolished;
  }
  else b = fIsDepolished;
  return b;
}
Bool_t TFace::IsEqual(const TObject *obj) const {
  return fSurface == ((TFace*)obj)->fSurface;
}
Bool_t TFace::IsInside(const T3Vector &v) const {
  //Determines whether a point is on the positive (outside) or negative (inside)
  //side of a surface. (Inside means here inside the volume of the shape to which
  //this face belongs)
  const Double_t zero = 0.0;
  T3Vector *x0, *x2;
  T3Vector vi(0),pi(0),pj(0),pk(0),n2(0),n3(0),n4(0);
  Double_t d,d2;
  Bool_t IsIn = kFALSE;
  switch (fKind) {
    case polygonal:
    case circular:
    case ring:
      x0 = (T3Vector *) fPtsFace[1];
      d  = (v - *x0)*fN;
      IsIn = d <= zero;
      break;
    case cylindrical:
      x0 = (T3Vector *) fPtsFace[0];
      pj = v - *x0;
      d  = pj*fAxis;
      pi = *x0 + d*fAxis;
      pj = v - pi;
      d  = pj.Module();
      if (fReverse)
        IsIn = d > fRadiusBase1;
      else
        IsIn = d <= fRadiusBase1;
      break;
    case conical:
    case tronconical:
      //x0 : center of lower basis
      x0 = (T3Vector *) fPtsFace[0];
      //x2 : top of cone
      x2 = (T3Vector *) fPtsFace[2];
      //v  : point from which distance to cone has to be calculated
      //pi : intersection of plane x0,x2,v with circle of lower basis
      //plane 1 of lower basis x0,x1,pi is defined by point x0, normal fAxis
      //plane 2 x0,x2,v  is defined by point x0, normal n2
      vi = *x2 - v;
      n2 = vi^fAxis;
      n2.Unit();
      //we look for point pi which is on the circle of the lower basis
      //and in the plane x0,x2,v. the direction from x0 to pi is n3
      //n3 is orthogonal to fAxis and orthogonal to n2, being inside plane 2.
      //the distance between x0 and pi is fRadiusBase1
      n3 = n2^fAxis;
      n3.Unit();
      //2 possible solutions, we take the nearest one
      pi = *x0 - fRadiusBase1*n3;
      pj = *x0 + fRadiusBase1*n3;
      vi = v - pi;
      d  = vi*vi;
      vi = v - pj;
      d2 = vi*vi;
      if (d2<d) pi = pj;
      //we look now for the point pj on the cone which is nearest to v.
      //n4 be the unit vector in the direction x2 - pi. We can write :
      //  pj = pi + d*n4
      //the vector pj - pi must be orthogonal to the vector v - pj, which gives d
      n4 = *x2 - pi;
      n4.Unit();
      vi = v - pi;
      d  = n4*vi;
      pj = pi + d*n4;
      //we look now for the point on the axis of the cone and on the line pj - v
      //if we look for this point as pk = x2 - d*fAxis, then we must have that
      //(pk - pj) is orthogonal to (pj - x2)
      vi = *x2 - pj;
      d2 = fAxis*vi;
      d  = vi.Norm2Sqr()/d2;
      pk = *x2 - d*fAxis;
      //v is outside if (v-pj)*(pj-pk) is positiv
      d  = (v - pj)*(pj - pk);
      IsIn = d<=zero;
      break;
  }
  return IsIn;
}
T3Vector& TFace::Normal(const T3Vector &v) {
  //Returns the normal to the face at the point v, which should be in the face.
  //Please call Normal without argument if face is plane !
  //In Litrani, a normal to a face must always point outside the shape. For a shape which is convex,
  //outside always means that the scalar product of the normal fN with the vector pointing from the
  //point considered on the face (v) towards the center of gravity of the shape (fGc) is negativ. The
  //trouble is that not all the shapes of ROOT are convex ! For instance a TTUBE is not, and for the
  //inner surface of a TTUBE, the normal has to be reversed to point outside the shape ! This is why
  //the Data member fReverse has been introduced. Notice that defining a subface is only allowed on
  //a plane face, and by luck there is among the shapes of ROOT no plane face where the normal has to
  //be reversed ! So for the face defined by the user, this parameter can be omitted.
  //
  //
  const Double_t minus1 = -1.0;
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  Double_t t1;
  T3Vector w1(0),B(0),C(0);
  T3Vector A(0);
  T3Vector *p0, *p2;
  p0 = (T3Vector *)(*fPts)[fListPts[0]];
  A  = fAxis;
  switch (fKind) {
    case polygonal:
    case circular:
    case ring:
      gMes->SetName("Normal");
      gMes->SetTitle("TFace");
      gMes->M(Warning_M,1,"do not give argument v in case of plane face");
      break;
    case cylindrical:
      w1  = v - *p0;
      t1  = A*w1;
      fN = w1 - t1*A;
      t1  = un/fRadiusBase1;
      fN = t1*fN;
      break;
    case conical:
    case tronconical:
      w1 = v - *p0;  //2nd vector in plane (v,p0,p2). 1st is A.
      w1.Unit();
      C  = w1^A;
      C.Unit();      //normal to plane (v,p0,p2)
      p2 = (T3Vector *)(*fPts)[fListPts[2]];
      B  = *p2 - v;
      B.Unit();
      fN = B^C;
      fN.Unit();
      t1 = fN*w1;
      if (t1<zero) {
        fN = minus1*fN;
        gMes->SetName("Normal");
        gMes->SetTitle("TFace");
        gMes->M(Warning_M,2,"Normal had to be reversed");
      }
      break;
  }
  if (fReverse) fN = minus1*fN;
  return fN;
}
void TFace::OutRidges() {
  //Calculates all vectors in face, normal to ridges, pointing outside
  //Only valid for faces of type polygonal. We calculate all unit vectors which are orthogonal to the
  //ridges, orthogonal to the normal of the face (i.e. in the face) and pointing outside.
  //
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t minus1 = -1.0;
  Double_t d;
  Int_t i;
  T3Vector *p0, *p1, *t;
  T3Vector cgf(0),N(0),v(0);
  //determines the center of gravity of the face
  for (i=0;i<fNbPoints;i++) {
    p0 = (T3Vector *) fPtsFace[i];
    cgf  += *p0;
  }
  d = un/fNbPoints;
  cgf = d*cgf;
  //determines the outridge vectors
  for (i=0;i<fNbPoints;i++) {
    p0 = (T3Vector *) fPtsFace[i];
    v  = cgf - *p0;
    p1 = (T3Vector *) fRidge[i];
    t = new T3Vector(0);
    *t = fN^(*p1);
    d  = v*(*t);
    if (d>zero) *t = minus1*(*t);
    t->Unit();
    fOutRidge.Add(t);
  }
}
Bool_t TFace::Parallelogram(T3Vector& center,T3Vector &v1,Double_t &a1,T3Vector &v2,Double_t &a2) const {
  // Returns complete information on the parallelogram, if the face is
  //a parallelogram ! The arguments are the following :
  //
  //  center  : center of the parallelogram
  //  v1      : 1st axis of parallelogram
  //  a1      : half length of 1st side along v1
  //  v2      : 2nd axis of parallelogram
  //  a2      : half length of 2nd side along v2
  const Double_t deux  = 2.0;
  const Double_t small = 1.0e-4;
  Bool_t b = kFALSE;
  if ((fPlanar) && (fKind == polygonal) && (fNbPoints == 4)) {
    b = kTRUE;
    T3Vector *x0,*x1,*x2,*x3,w1,w2;
    x0 = (T3Vector *)fPtsFace[0];
    x1 = (T3Vector *)fPtsFace[1];
    x2 = (T3Vector *)fPtsFace[2];
    x3 = (T3Vector *)fPtsFace[3];
    v1 = *(T3Vector *)fRidge[0];
    v2 = *(T3Vector *)fRidge[1];
    w1 = *x1 - *x0;
    a1 = w1.Module()/deux;
    w2 = *x2 - *x1;
    a2 = w2.Module()/deux;
    center = *x0 + a1*v1 + a2*v2;
    //verify that face IS a parallelogram
    w1 = *x0 + deux*a2*v2;
    w2 = *x3 - w1;
    Double_t test = w2.Module();
    if (test>small) {
      b = kFALSE;
      gMes->SetName("Parallelogram");
      gMes->SetTitle("TFace");
      gMes->M(Error_M,1,"Face is not a parallelogram");
    }
  }
  return b;
}
void TFace::Parameters(Bool_t first) {
  //calculates all useful parameters for the face. Normal to the face if face is plane,
  //and all kinds of radius and heights. Calls Ridges() and OutRidges().
  //
  const Double_t zero   = 0.0;
  const Double_t minus1 = -1.0;
  const Double_t small  = 1.0E-6;
  const Double_t vsmall = 1.0E-20;
  Double_t d;
  T3Vector *p0, *p1, *p2, *p3;
  T3Vector w1(0),w2(0),w3(0),N(0);
  Int_t k;
  if (first) {
    //a priori no contact
    fHasContact   = kFALSE;
    fNContact     = 0;
    //set the list of points
    for (Int_t i=0;i<fNbPoints;i++) {
      k = fListPts[i];
      fPtsFace.Add((*fPts)[k]);
    }
  }
  p0 = (T3Vector *) fPtsFace[0];
  p1 = (T3Vector *) fPtsFace[1];
  p2 = (T3Vector *) fPtsFace[2];
  w1 = *p1 - *p0;
  w2 = *p2 - *p0;
  //make some verifications
  switch (fKind) {
    case polygonal:
      fPlanar = kTRUE;
      N = w1^w2;
      N.Unit();
      break;
    case circular:
    case ring:
      fPlanar = kTRUE;
      N = w1^w2;
      d = N.Module();
      if (d<vsmall) {
        gMes->SetName("Parameters");
        gMes->SetTitle("TFace");
        gMes->M(kill_prog,1,"The 3 first points are aligned");
      }
      N.Unit();
      break;
    case cylindrical:
      fPlanar = kFALSE;
      break;
    case conical:
      fPlanar = kFALSE;
      break;
    case tronconical:
      fPlanar = kFALSE;
      p3 = (T3Vector *) fPtsFace[3];
      w3 = *p3 - *p0;
      N  = w2^w3;
      d  = N.Module();
      if (d>small) {
        gMes->SetName("Parameters");
        gMes->SetTitle("TFace");
        gMes->M(kill_prog,2,"Points 0 2 3 are not aligned");
      }
      break;
  }
  fRadiusBase1 = zero;
  fRadiusBase2 = zero;
  fConeHeight  = zero;
  fHeight      = zero;
  //we fix the normal to the surface when surface plane
  switch (fKind) {
    case polygonal:  
    case circular:
    case ring:
      w3 = fGc - *p0;
      d  = N*w3;
      if (d>zero)
        N = minus1*N;
      if (fReverse) N = minus1*N;
      fN = N;
      break;
    case cylindrical:
    case conical:
    case tronconical:
      break;
  }
  //Then we fix the other parameters
  switch (fKind) {
    case polygonal:
      break;
    case circular:
      fRadiusBase1 = w1.Module();
      break;
    case ring:
      fRadiusBase1 = w1.Module();
      fRadiusBase2 = w2.Module();
      break;
    case cylindrical:
      fRadiusBase1 = w1.Module();
      fRadiusBase2 = fRadiusBase1;
      fHeight = w2.Module();
      w2.Unit();
      fAxis = w2;
      break;
    case conical:
      fRadiusBase1 = w1.Module();
      fConeHeight  = w2.Module();
      fHeight      = fConeHeight;
      w2.Unit();
      fAxis = w2;
      break;
    case tronconical:
      fRadiusBase1 = w1.Module();
      fHeight = w3.Module();
      fConeHeight = w2.Module();
      fRadiusBase2 = fRadiusBase1*((fConeHeight-fHeight)/fConeHeight);
      w3.Unit();
      fAxis = w3;
      break;
  }
  //defines ridges and outridges
  if (fKind==polygonal) {
    if (!first) {
      fRidge.Delete();
      fOutRidge.Delete();
    }
    Ridges();
    OutRidges();
  }
}
void TFace::Print() const {
  //prints everything concerning face
  T3Vector *v;
  //output
  cout << "Face number  : " << fNumber << endl;
  cout << "Kind of face : " << (long unsigned int)fKind << endl;
  switch (fKind) {
  case polygonal:
    cout << "polygonal";
    break;
  case circular:
    cout << "circular";
    break;
  case ring:
    cout << "ring";
    break;
  case cylindrical:
    cout << "cylindrical";
    break;
  case conical:
    cout << "conical";
    break;
  case tronconical:
    cout << "tronconical";
    break;
  }
  cout << endl;
  if (fRevetment) cout << "Revetment    : " << fRevetment->GetName() << endl;
  else            cout << "Revetment    : none" << endl;
  cout << "Surface      : " << fSurface << endl;
  cout << "Normal       : " << fN << endl;
  cout << "Status of fN : ";
  if (fReverse) cout << "reversed";
  else cout << "not reversed";
  cout << endl;
  if (fKind != polygonal) {
    cout << "1st radius   : " << fRadiusBase1 << endl;
    cout << "2nd radius   : " << fRadiusBase2 << endl;
    cout << "height cone  : " << fConeHeight  << endl;
    cout << "height tcone : " << fHeight      << endl;
  }
  cout << endl;
  cout << "Nb of points : " << fNbPoints << endl;
  cout << endl;
  cout << "List of pts  : i          x          y          z" << endl;
  cout << endl;
  Int_t i;
  for (i=0;i<fNbPoints;i++) {
    v = (T3Vector *) fPtsFace[i];
    cout << "  " << fListPts[i] << *v << endl;
  }
  cout << endl;
  //listing
  *gMes->fListing << "Face number  : " << fNumber << endl;
  *gMes->fListing << "Kind of face : " << (long unsigned int)fKind << endl;
  switch (fKind) {
  case polygonal:
    *gMes->fListing << "polygonal";
    break;
  case circular:
    *gMes->fListing << "circular";
    break;
  case ring:
    *gMes->fListing << "ring";
    break;
  case cylindrical:
    *gMes->fListing << "cylindrical";
    break;
  case conical:
    *gMes->fListing << "conical";
    break;
  case tronconical:
    *gMes->fListing << "tronconical";
    break;
  }
  *gMes->fListing << endl;
  if (fRevetment) *gMes->fListing << "Revetment    : " << fRevetment->GetName() << endl;
  else            *gMes->fListing << "Revetment    : none" << endl;
  *gMes->fListing << "Surface      : " << fSurface << endl;
  *gMes->fListing << "Normal       : " << fN << endl;
  *gMes->fListing << "Status of fN : ";
  if (fReverse) *gMes->fListing << "reversed";
  else *gMes->fListing << "not reversed";
  *gMes->fListing << endl;
  if (fKind != polygonal) {
    *gMes->fListing << "1st radius   : " << fRadiusBase1 << endl;
    *gMes->fListing << "2nd radius   : " << fRadiusBase2 << endl;
    *gMes->fListing << "height cone  : " << fConeHeight  << endl;
    *gMes->fListing << "height tcone : " << fHeight      << endl;
  }
  *gMes->fListing << endl;
  *gMes->fListing << "Nb of points : " << fNbPoints << endl;
  *gMes->fListing << endl;
  *gMes->fListing << "List of pts  : i          x          y          z" << endl;
  *gMes->fListing << endl;
  for (i=0;i<fNbPoints;i++) {
    v = (T3Vector *) fPtsFace[i];
    *gMes->fListing << "  " << fListPts[i] << *v << endl;
  }
  *gMes->fListing << endl;
}
void TFace::RemoveContact(TOtherSide *ct) {
  if (fNContact>0) {
    Int_t N;
    N = fContactSide.GetEntries();
    fNContact--;
    if (N>0) fContactSide.Remove(ct);
  }
  if (fNContact>1) fContactSide.Sort();
}
void TFace::Ridges() {
  //Calculates all ridges
  //Only valid for faces of type polygonal. We calculate all unit vectors pointing
  //from point i to point i+1. The last unit vector points from point fNbPoints-1
  //to point 0.
  Int_t i;
  T3Vector *p0, *p1, *w;
  for (i=0;i<fNbPoints-1;i++) {
    p0 = (T3Vector *) fPtsFace[i];
    p1 = (T3Vector *) fPtsFace[i+1];
    w = new T3Vector(0);
    *w = *p1 - *p0;
    w->Unit();
    fRidge.Add(w);
  }
  p0 = (T3Vector *) fPtsFace[fNbPoints-1];
  p1 = (T3Vector *) fPtsFace[0];
  w = new T3Vector(0);
  *w = *p1 - *p0;
  w->Unit();
  fRidge.Add(w);
}
void TFace::SetAsDetector(KindOfDetector kind,const char *distrib,Double_t accept) {
  // Declares that this face is also a detector
  fDet = kind;
  fQE  = TSplineFit::FindFit(distrib,12);
  fAccept = accept;
}
void TFace::SetAsDetector(KindOfDetector kind,TSplineFit *f,Double_t accept) {
  // Declares that this face is also a detector
  fDet = kind;
  fQE  = f;
  fAccept = accept;
}
void TFace::SetContact(TOtherSide *ct) {
  // Declares a contact with an other face of an other shape.
  fHasContact = kTRUE;
  fNContact  += 1;
  fContactSide.Add(ct);
  if (fNContact>1) {
    TContact *c1;
    TContact *c2;
    c1 = Contact(0);
    c2 = Contact(1);
    fContactSide.Sort();
    c1 = Contact(0);
    c2 = Contact(1);
  }
}
void TFace::SetDepolished(Double_t ThetaDepls) {
  //
  //    DEPOLISHED SURFACES
  //
  //  A new possibility has been added to Litrani : it is now possible to simulate
  //a depolished surface, by a call to the method SetDepolished(ThetaDepls).
  //Depolishing is simulated in the following way :
  //   when the surface is declared depolished, the normal to the surface at the
  //point hit by the photon is randomly tilted ( with respect to the true normal
  //of the surface ) by an angle theta. theta is generated randomly
  //according to a distribution sin(theta) dtheta dphi, between 0 and ThetaDepls.
  //So ThetaDepls (0 < ThetaDepls < 90 ) can adjust the roughness of the grinding.
  //You have to give ThetaDepls in degree. If you omit it, the value of 60° will
  //be taken.
  //
  //  Important restrictions :
  //
  //  The use of grinded ( depolished ) surface is only allowed for the faces of a
  //shape containing an isotropic material. For an anisotropic material, Litrani
  //give up, for reasons explained below. Notice also that even if you declare a
  //face to be grinded (depolished), the part of this face which is in contact
  //with an other face of an other shape is never treated as grinded by Litrani
  //Only the part of the face which is NOT in contact with an other face (be
  //it covered by a revetment or not) is treated as grinded. An other restriction
  //is that you have not the rigth to declare as depolished a face having a revetment
  //WITHOUT slice (of air or something else) between the face and the revetment.
  //
  //    Notice the following problems in this rather simple-minded way of
  //simulating depolishing : 
  //
  // 1 - after the normal at the point hit by the photon has been randomly tilted,
  //it may be that the angle of this new normal with the Poyinting vector of the
  //photon has become greater than 90 degree, which would mean that the photon
  //does not reach any more the surface at this point and would give meaningless
  //results in the calculation of the Fresnel equations ! If this happens, we
  //simply generate again theta until this does not happen any more. 
  // 2 - after the normal at the point hit by the photon has been randomly tilted,
  //it may be that in case of total reflection, the outgoing photon does not any
  //more reenter the convex polyhedron ! If this happens, we simply generate again
  //theta until this does not happen any more. 
  // 3 - after the normal at the point hit by the photon has been randomly tilted,
  //it may be that in case of refraction, the outgoing wave would make an angle
  //greater than 90 degree with the untilted normal to the surface, i-e the
  //refracted photon would not reach the revetment ! If this happens, we simply
  //generate again theta until this does not happen any more. 
  // 4 - As in the case of anisotropy, angle of reflection is not necessarily
  //equal to the incident angle, it is much too complicate to calculate the
  //possibility that difficulty (2) or (3) arises or not. For that reason, we
  //have forbidden the use of grinded surface associated with a shape
  //containing an anisotropic material. 
  // 5 - when the photon does not make a total reflection, but goes out of the
  //face towards the revetment, and is reflected back by the revetment, we have
  //not implemented for that "return" case a grinded surface, but in that case we
  //leave the normal to the surface as it is ( as if the surface was polished ).
  //Had we not done that simplification, we would have had the difficulties (1)
  //(2) and (3) again on return, which we considered too much ! 
  //
  //  So keep in mind that our implementation of grinding (depolishing) is
  //rather simplified !
  //
  const Double_t zero  = 0.0;
  const Double_t pid   = 180.0;
  const Double_t pis2d = 90.0;
  gMes->SetName("SetDepolished");
  gMes->SetTitle("TFace");
  if ((ThetaDepls<=zero) || (ThetaDepls>pis2d)) {
    gMes->M(Warning_M,1,"angle must be within (0,90)");
    gMes->M(Warning_M,1,"Depolishing refused");
    fIsDepolished = kFALSE;
  }
  else {
    if ((fRevetment) && (!fRevetment->IsSlice())) {
      gMes->M(Warning_M,2,"depolishing a face WITH a revetment WITHOUT slice is forbidden");
      gMes->M(Warning_M,2,"Depolishing refused");
      fIsDepolished = kFALSE;
    }
    else {
      fIsDepolished  = kTRUE;
      fThetaDepls    = (ThetaDepls*TMath::Pi())/pid;
      fCosThetaDepls = TMath::Cos(fThetaDepls);
    }
  }
}
Double_t TFace::Surface() const {
  //Calculates the surface of a Face. This is used by Litrani to decide in which order the faces
  //have to be tested, for instance when you look which will be the next face hit by a photon. It
  //is clear that faces having a big surface have a greater probability to be hit, so that the
  //program is accelerated by presenting the faces in reverse order of surfaces !
  //
  const Double_t deux = 2.0;
  Int_t i,k1,k2;
  Double_t s = 0;
  Double_t a;    //apotheme !
  T3Vector *p0, *p1, *p2;
  Double_t pi;
  pi = TMath::Pi();
  switch (fKind) {
    case polygonal:
      p0 = (T3Vector *)(*fPts)[fListPts[0]];
      for (i=1;i<fNbPoints-1;i++) {
        k1 = fListPts[i];
        k2 = fListPts[i+1];
        p1 = (T3Vector *)(*fPts)[k1];
        p2 = (T3Vector *)(*fPts)[k2];
        s  = s + p0->TriangleSurface(*p1, *p2);
      }
      break;
    case circular:
      s = fRadiusBase1*fRadiusBase1*pi;
      break;
    case ring:
      s = fRadiusBase1*fRadiusBase1*pi;
      s = s - fRadiusBase2*fRadiusBase2*pi;
      break;
    case cylindrical:
      s = deux*pi*fRadiusBase1*fHeight;
      break;
    case conical:
      a = TMath::Sqrt(fRadiusBase1*fRadiusBase1 + fConeHeight*fConeHeight);
      s = pi*fRadiusBase1*a;
      break;
    case tronconical:
      a = fRadiusBase1 - fRadiusBase2;
      a = TMath::Sqrt(a*a + fHeight*fHeight);
      s = pi*a*(fRadiusBase1 + fRadiusBase2);
      break;
  }
  return s;
}
void TFace::ToWCS() {
  // As soon as the TSNode associated with the TSupplShape of this face is defined, all
  //coordinates are changed from local coordinate system to the World Coordinate System.
  //This is done by a call to the method ToWCS() of TSNode which transforms all points
  //and all axis of the shape.
  // Once in TFace::ToWCS, all points and axis are already in WCS
  // Not yet in WCS :
  // - fN  - fAxis - all ridges vectors   - all outridges vectors. But this can be
  //remedied by a call to Parameters()
  //
  Parameters(kFALSE);
}
