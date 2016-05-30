// @(#)root/html:$Id: TSNode.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TwoPadDisplay.h"
#include "TMessErr.h"
#include "TSupplShape.h"
#include "TLitGlob.h"
#include "TSNode.h"

ClassImp(TSNode)
//______________________________________________________________________________
//
// TSNode inherits from TNode in particular to add the method LocalToMaster,
//because I do not understand the way Local2Master works.
//LocalToMaster does in principle what Local2Master should do.
//LocalToWorld transforms a point from the local coordinate system to the WCS
//AxisToMaster transforms a vector (not a point) from the LCS to the MasterCS
//AxisToWorld  transforms a vector (not a point) from the LCS to the WCS
//
//  The call to TSNode has also as a consequence that everything concerning
//the shape associated with this TSNode jumps from local to World Coordinate
//System, through the call to the method ToWCS.
//
TSNode::TSNode(const char *name, const char *title, const char *shapename,
  Double_t x, Double_t y, Double_t z, const char *matrixname, Option_t *option):TNode(name,title,shapename,x,y,z,matrixname,option) {
  //constructor
  const Double_t vsmall = 1.0e-12;
  Bool_t NoMatrix;
  TShape *shape;
  fSuppl = 0;
  TSNode *q  = (TSNode *)fParent;
  shape  = gGeometry->GetShape(shapename);
  TSupplShape *ptss;
  TString s;
  TString s1 = shapename;
  TIter next(&gLit->fSupplShape);
  Bool_t found = kFALSE;
  while ((!found) && (ptss = (TSupplShape *)next())) {
    s = ptss->GetName();
    if (!s.CompareTo(s1)) {
      found      = kTRUE;
      fSuppl = ptss;
    }
  }
  s = "";
  NoMatrix  = !s.CompareTo(matrixname);
  fMoveLast = ((TMath::Abs(x)>vsmall) || (TMath::Abs(y)>vsmall) ||
    (TMath::Abs(z)>vsmall) || (!NoMatrix));
  fMoveLast = fMoveLast && (q==0);
  if (!found) {
    gMes->SetName("TSNode");
    gMes->SetTitle("TSNode");
    gMes->M(Error_M,1,"SupplShape not found");
  }
  else {
    fSuppl->fSNode = this;
    ToWCS();
  }
}
TSNode::TSNode(const char *name, const char *title, TShape *shape,Double_t x,
  Double_t y, Double_t z, TRotMatrix *matrix,  Option_t *option):TNode(name,title,shape,x,y,z,matrix,option) {
  //constructor
  const Double_t vsmall = 1.0e-12;
  Bool_t NoMatrix;
  TSupplShape *ptss;
  TString s;
  fSuppl = 0;
  TSNode *q  = (TSNode *)fParent;
  TString s1 = shape->GetName();
  TIter next(&gLit->fSupplShape);
  Bool_t found = kFALSE;
  while ((!found) && (ptss = (TSupplShape *)next())) {
    s = ptss->GetName();
    if (!s.CompareTo(s1)) {
      found      = kTRUE;
      fSuppl = ptss;
    }
  }
  NoMatrix  = (matrix==0);
  fMoveLast = ((TMath::Abs(x)>vsmall) || (TMath::Abs(y)>vsmall) ||
    (TMath::Abs(z)>vsmall) || (!NoMatrix));
  fMoveLast = fMoveLast && (q==0);
  if (!found) {
    gMes->SetName("TSNode");
    gMes->SetTitle("TSNode");
    gMes->M(Error_M,1,"SupplShape not found");
  }
  else {
    fSuppl->fSNode = this;
    ToWCS();
  }
}
TSNode::~TSNode() {
  // TSNode is not owner of fSuppl. Do not delete it
}
void TSNode::AxisToMaster(const T3Vector &local, T3Vector &master) const {
  //Transforms an axis (not a point) from local to master coordinate system
  Double_t x,y,z;
  Double_t *matrix = fMatrix->GetMatrix();

  x = local(0)*matrix[0] + local(1)*matrix[3] + local(2)*matrix[6];
  y = local(0)*matrix[1] + local(1)*matrix[4] + local(2)*matrix[7];
  z = local(0)*matrix[2] + local(1)*matrix[5] + local(2)*matrix[8];
  master(0) = x; master(1) = y; master(2) = z;
}
void TSNode::AxisToWorld(const T3Vector &local, T3Vector &world) {
  //Transforms an axis (not a point) from local to World Coordinate System
  TSNode *p  = this;
  TSNode *q  = (TSNode *)fParent;
  T3Vector v(0);
  T3Vector w(0);
  v = local;
  while (q) {
    p->AxisToMaster(v,w);
    v = w;
    p = q;
    q = (TSNode *)p->fParent;
  }
  if ((q==0) && (p->fMoveLast)) {
    p->AxisToMaster(v,w);
    v = w;
    p = q;
  }
  world = v;  
}
void TSNode::Draw(Option_t *option) {
  // First check that gTwoPad is booked, then call TNode::Draw()
  if (!gTwoPad) gLit->BookCanvas();
  TNode::Draw(option);
}
void TSNode::LocalToMaster(const T3Vector &local, T3Vector &master) const {
  //Idem Local2Master, but do not use global variables
  Double_t x,y,z;
  Double_t *matrix = fMatrix->GetMatrix();

  x = fX
    + local(0)*matrix[0]
  + local(1)*matrix[3]
  + local(2)*matrix[6];

  y = fY
    + local(0)*matrix[1]
  + local(1)*matrix[4]
  + local(2)*matrix[7];

  z = fZ
    + local(0)*matrix[2]
  + local(1)*matrix[5]
  + local(2)*matrix[8];

  master(0) = x; master(1) = y; master(2) = z;
}
void TSNode::LocalToWorld(const T3Vector &local, T3Vector &world) {
  //LocalToWorld transforms a point from the local coordinate system to the WCS
  TSNode *p  = this;
  TSNode *q  = (TSNode *)fParent;
  T3Vector v(0);
  T3Vector w(0);
  v = local;
  while (q) {
    p->LocalToMaster(v,w);
    v = w;
    p = q;
    q = (TSNode *)p->fParent;
  }
  if ((q==0) && (p->fMoveLast)) {
    p->LocalToMaster(v,w);
    v = w;
    p = q;
  }
  world = v;
}
void TSNode::ToWCS() {
  //As soon as the TSNode associated with the TSupplShape is defined, all
  //coordinates are changed from local coordinate system to the World
  //Coordinate System. This is done by a call to the method ToWCS() in TSNode
  //and for each face of the TSuppplShape.
  //
  TSNode *q;
  q = (TSNode *)fParent;
  if (q || (q==0) && (fMoveLast)) {
    T3Vector *p;
    T3Vector master(0);
    //  All points in WCS
    TIter next(fSuppl->fPoints);
    while ((p = (T3Vector *)next())) {
      LocalToWorld(*p,master);
      *p = master;
    }
    //  Vectors of Damages
    if (fSuppl->fDamage) {
      LocalToWorld(fSuppl->fDamStart,master);
      fSuppl->fDamStart = master;
      AxisToWorld(fSuppl->fDamAxis,master);
      fSuppl->fDamAxis  = master;
    }
    //  fGCenter in WCS
    p = &fSuppl->fGCenter;
    LocalToWorld(*p,master);
    fSuppl->fGCenter = master;
    //  All axis in WCS
    if (fSuppl->fAnisotropic) {
      if (!fSuppl->IsFromOther()) {
        p = &fSuppl->fAniOx;
        AxisToWorld(*p,master);
        fSuppl->fAniOx = master;
        p = &fSuppl->fAniOy;
        AxisToWorld(*p,master);
        fSuppl->fAniOy = master;
        p = &fSuppl->fAniOz;
        AxisToWorld(*p,master);
        fSuppl->fAniOz = master;
      }
      fSuppl->SetM();
    }
    //  Points and axis of APD in WCS
    if (fSuppl->fDet == apd) {
      p = &fSuppl->fx0APD;
      LocalToWorld(*p,master);
      fSuppl->fx0APD = master;
      p = &fSuppl->fx1APD;
      LocalToWorld(*p,master);
      fSuppl->fx1APD = master;
      p = &fSuppl->fAxisAPD;
      AxisToWorld(*p,master);
      fSuppl->fAxisAPD = master;
    }
    //  All faces in WCS
    TIter next1(&fSuppl->fFaces);
    TFace *f;
    while ((f = (TFace *)next1())) {
      p = &f->fGc;
      LocalToWorld(*p,master);
      f->fGc = master;
      f->ToWCS();
    }
    //  All Subfaces in WCS
    TIter next2(&fSuppl->fSubFaces);
    while ((f = (TFace *)next2())) {
      f->ToWCS();
    }
  }
}
