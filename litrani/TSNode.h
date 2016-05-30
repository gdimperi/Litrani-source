// @(#)root/html:$Id: TSNode.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSNode
#define LITR_TSNode
////////////////////////////////////////////////////////////////////////////
//                                                                        //
// TSNode Inherits from TNode just to have a correct LocalToMaster method //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
#include "TGeometry.h"
#include "TNode.h"
#include "T3Vector.h"

class TSupplShape;

class TSNode : public TNode {

protected:

  TSupplShape *fSuppl;    //! TSupplShape associated with this node.
  Bool_t       fMoveLast; //If true, master shape has also to be moved

  void         ToWCS();

public:
  TSNode() { fSuppl = 0;}
  TSNode(const char *name, const char *title, const char *shapename,
    Double_t x=0, Double_t y=0, Double_t z=0,
    const char *matrixname="", Option_t *option="");
  TSNode(const char *name, const char *title, TShape *shape,
    Double_t x=0, Double_t y=0, Double_t z=0,
    TRotMatrix *matrix=0,  Option_t *option="");
  virtual     ~TSNode();
  void         AxisToMaster(const T3Vector &local, T3Vector &master) const;
  void         AxisToWorld(const T3Vector &local, T3Vector &world);
  void         Draw(Option_t *option="");
  void         LocalToMaster(const T3Vector &local, T3Vector &master) const;
  void         LocalToWorld(const T3Vector &local, T3Vector &world);
  ClassDef(TSNode,0) //Idem TNode with other LocalToMaster
};
#endif
