// @(#)root/html:$Id: TS8PTS.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "T3Vector.h"
#include "TMessErr.h"
#include "TFace.h"
#include "TSupplShape.h"
#include "TS8PTS.h"

ClassImp(TS8PTS)
//______________________________________________________________________________
//
//BEGIN_HTML <!--
/* -->
</pre>
<html>
<p>
<table border="32" cellpadding="6" cellspacing="4" bgcolor="#cccc99">
<tr>
<td>Please notice before all the following :
<p><b>TS8PTS</b> has been introduced into Litrani <font color="#9c0031"><b>by mistake</b></font> ! Working in the CMS experiment and examining the shape of the CMS crystals, I have reached a little bit too fast the conclusion that the shape of the CMS crystal was impossible to describe using the standard shapes of ROOT. In fact, a closer look let me realize later that it is perfectly possible to represent the CMS crystals with the shape TSTRAP. Later on, I introduced into Litrani the even more general shape TSGTRA. <font color="#9c0031"><b>So I warmly recommend the user to carefully examine whether his shape cannot be represented by the standard shape of ROOT </b></font>{tstrap,tstrd1,tstrd2,tspara,tstrap,tsgtra,tscyl,tstube,tscone}<font color="#9c0031"><b>before deciding to use TS8PTS.</b></font></p>
<p><b>TS8PTS</b> is the only shape of Litrani <font color="#9c0031"><b>which does not exist in ROOT</b></font>.&nbsp;The reason to be suspicious about <b>TS8PTS</b> are the following :</p>
<ul>
<li>As this shape is not a shape of ROOT, you accept, using it, to be <font color="#9c0031"><b>non-standard</b></font>.
<li>The drawing of the shapes is done by ROOT, not by Litrani. In order that <b>TS8PTS</b> be drawn correctly, I had to provide to <b>TS8PTS</b> its own SetPoints() method.&nbsp;I hope it will work, but I have not spent much time studying the drawing of shapes in ROOT to be sure about that.
<li>Even if your shape cannot be described by <font color="#9c0031"><b>one</b></font> shape of ROOT, you can represent it by <font color="#9c0031"><b>a combination of many</b></font> shapes of ROOT. Doing that, you remain standard.
</ul>
<p>The reason why I keep the class <b>TS8PTS</b> is that , may be, it can be useful to a user having found that no shape of ROOT can describe his shape and deciding that it is inadequate to split his shape into many standard ROOT shapes. Then <b>TS8PTS</b> may be useful. After all, even the most general shape of ROOT, TSGTRA, requires some condition between the (x,y) coordinates of the apexes [ y1 == y0  |  y2 == y3  |  (y4-y5)/(x4-x5) == (y6-y7)/(x6-x7) ], that <b>TS8PTS</b> does not require !</td>
</tr>
</table>
</p>
<p></p>
<h4><font color="#990000">Recall that all dimensions of shapes are in cm in Litrani.</font></h4>
<p><b>TS8PTS</b> is a very general shape composed of 8 points, 6 <b>plane</b> faces. Face 4 is built of the points 0 1 2 3 which are all at the z coordinate -dz. Face 5 is built of the points 4 5 6 7 which are all at the z coordinate +dz. <b>Except for these restrictions, all points are any whatsoever, but with the restriction that all faces have to be plane !</b> If you give coordinates such that the lateral faces are not plane, the constructor of <b>TS8PTS</b>  will try to move points 4 5 6 7 <b>the least possible</b>, but such that faces 0 1 2 3 become plane. If the points have to be moved by more than 1.0e-4, the user is informed that his points have been moved and the new values of coordinates are printed. The new values for the coordinates of the points are returned, it is why arguments of the constructor are passed by address, not by value.</p>
<p></p>
<center>
<p>
<table border="5" cellpadding="1" cellspacing="3" bgcolor="#cccc99">
<caption>22 parameters of the constructor of <b>TS8PTS</b></caption>
<tr>
<td>
<center>
<h4>Type</h4>
</center>
</td>
<td>
<center>
<h4>Parameter</h4>
</center>
</td>
<td>
<center>
<h4>fName</h4>
</center>
</td>
<td>
<center>
<h4>Notation</h4>
</center>
</td>
<td>
<center>
<h4>Definition</h4>
</center>
</td>
</tr>
<tr>
<td>const char</td>
<td>*name</td>
<td>fName</td>
<td>
<center>
-</center>
</td>
<td>name of the shape</td>
</tr>
<tr>
<td>const char</td>
<td>*title</td>
<td>fTitle</td>
<td>
<center>
-</center>
</td>
<td>shape's title</td>
</tr>
<tr>
<td>const char</td>
<td>*namemat</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
<td>
<div align="left">
<dl>
<dt>optical material of the shape or name of the other shape</div>
<dt>having same material and optical properties. (see FromOther).
</dl>
</td>
</tr>
<tr>
<td>const char</td>
<td>*namerev</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
<td>name of the default revetment for the 6 faces</td>
</tr>
<tr>
<td>Double_t</td>
<td>dz</td>
<td>
<center>
-</center>
</td>
<td>d<sub>z</sub></td>
<td>half-length along the z axis in cm
<ul>
<li>-dz : z coordinate of points 0/1/2/3
<li>+dz : z coordinate of points 4/5/6/7
</ul>
</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>x0</td>
<td>in fV0</td>
<td>x<sub>0</sub></td>
<td>x coordinate of point (0) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>y0</td>
<td>in fV0</td>
<td>y<sub>0</sub></td>
<td>y coordinate of point (0) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>x1</td>
<td>in fV1</td>
<td>x<sub>1</sub></td>
<td>x coordinate of point (1) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>y1</td>
<td>in fV1</td>
<td>y<sub>1</sub></td>
<td>y coordinate of point (1) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>x2</td>
<td>in fV2</td>
<td>x<sub>2</sub></td>
<td>x coordinate of point (2) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>y2</td>
<td>in fV2</td>
<td>y<sub>2</sub></td>
<td>y coordinate of point (2) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>x3</td>
<td>in fV3</td>
<td>x<sub>3</sub></td>
<td>x coordinate of point (3) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>y3</td>
<td>in fV3</td>
<td>y<sub>3</sub></td>
<td>y coordinate of point (3) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>x4</td>
<td>in fV4</td>
<td>x<sub>4</sub></td>
<td>x coordinate of point (4) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>y4</td>
<td>in fV4</td>
<td>y<sub>4</sub></td>
<td>y coordinate of point (4) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>x5</td>
<td>in fV5</td>
<td>x<sub>5</sub></td>
<td>x coordinate of point (5) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>y5</td>
<td>in fV5</td>
<td>y<sub>5</sub></td>
<td>y coordinate of point (5) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>x6</td>
<td>in fV6</td>
<td>x<sub>6</sub></td>
<td>x coordinate of point (6) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>y6</td>
<td>in fV6</td>
<td>y<sub>6</sub></td>
<td>y coordinate of point (5) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>x7</td>
<td>in fV7</td>
<td>x<sub>7</sub></td>
<td>x coordinate of point (7) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>y7</td>
<td>
<center>
-</center>
</td>
<td>y<sub>7</sub></td>
<td>y coordinate of point (7) wanted by the user.</td>
</tr>
<tr>
<td>Bool_t</td>
<td>FromOther</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
<td>true if material and optical axis have to be taken from an other shape.</td>
</tr>
</table>
</p>
</center>
<p></p>
<p>The 6 faces of the <b>TS8PTS</b> are built from these points in the following way :</p>
<p></p>
<center>
<p>
<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
<tr>
<td>
<center>
<h4>faces</h4>
</center>
</td>
<td>
<center>
<h4>defined by points</h4>
</center>
</td>
</tr>
<tr>
<td>0</td>
<td>
<center>
0 1 5 4</center>
</td>
</tr>
<tr>
<td>1</td>
<td>
<center>
1 2 6 5</center>
</td>
</tr>
<tr>
<td>2</td>
<td>
<center>
2 3 7 6</center>
</td>
</tr>
<tr>
<td>3</td>
<td>
<center>
3 0 4 7</center>
</td>
</tr>
<tr>
<td>4</td>
<td>
<center>
1 0 3 2</center>
</td>
</tr>
<tr>
<td>5</td>
<td>
<center>
4 5 6 7</center>
</td>
</tr>
</table>
</p>
</center>
<p></p>
<p>For each shape of Litrani {tstrap,tstrd1,tstrd2,tspara,tstrap,tsgtra,ts8pts,tscyl,tstube,tscone} there is an associated &quot;friend&quot; class TSupplShape, which contains everything which is common to all kind of shapes. This associated class may be reached from <b>TS8PTS</b> (or from any other shape) like this (supposing that p8pts is a pointer on a <b>TS8PTS</b>) :</p>
<center>
<p><font color="#9c3031"><code>TSupplShape *suppl = p8pts-&gt;fSuppl;</code></font></p>
</center>
<p>All shapes in Litrani may at the same time be a detector of photon. For instance, a shape may represent a phototube or an APD. See the description how to define a shape as a detector in the presentation of the TSupplShape class. If FromOther is true, it means that the material of the shape, the dielectric properties and the optical axis are taken from an other shape. If this is the case, it means that in case of transition between this shape and the other shape having same material and same optical axis, the photon simply continues from one shape to the other, without any transition calculation.
<pre>
<!--*/
// -->END_HTML
//
TS8PTS::TS8PTS(const char *name,const char *title,const char *namemat,
  const char *namerev,Double_t dz,Double_t &x0, Double_t &y0, Double_t &x1,
  Double_t &y1,Double_t &x2, Double_t &y2, Double_t &x3, Double_t &y3,Double_t &x4,
  Double_t &y4, Double_t &x5, Double_t &y5,Double_t &x6, Double_t &y6, Double_t &x7,
  Double_t &y7,Bool_t FromOther):TBRIK(name,title,namemat,Float_t(x2/2),
  Float_t(y3/2),Float_t(dz)) {
  //
  // Notice this important difference between the vectors fVi and the vectors
  //vi defined in this constructor: 
  //  - vectors fVi always keep there original coordinates. They are not chan-
  //ged by a call to TSNode. They remain in class TS8PTS and are not given to
  //the class TSupplShape. They stay in the local coordinates of the shape.
  //  - vectors vi change when a call to TSNode is made. They move into World
  //Coordinate. They are given to the class TSupplShape and then handled by
  //TSupplShape. They have not to be deleted here, but by TSupplShape.
  //
  //BEGIN_HTML <!--
  /* -->
  </pre>
  <center>
  <p>
  <table border="5" cellpadding="1" cellspacing="3" bgcolor="#cccc99">
  <caption>22 parameters of the constructor of <b>TS8PTS</b></caption>
  <tr>
  <td>
  <center>
  <h4>Type</h4>
  </center>
  </td>
  <td>
  <center>
  <h4>Parameter</h4>
  </center>
  </td>
  <td>
  <center>
  <h4>fName</h4>
  </center>
  </td>
  <td>
  <center>
  <h4>Notation</h4>
  </center>
  </td>
  <td>
  <center>
  <h4>Definition</h4>
  </center>
  </td>
  </tr>
  <tr>
  <td>const char</td>
  <td>*name</td>
  <td>fName</td>
  <td>
  <center>
  -</center>
  </td>
  <td>name of the shape</td>
  </tr>
  <tr>
  <td>const char</td>
  <td>*title</td>
  <td>fTitle</td>
  <td>
  <center>
  -</center>
  </td>
  <td>shape's title</td>
  </tr>
  <tr>
  <td>const char</td>
  <td>*namemat</td>
  <td>
  <center>
  -</center>
  </td>
  <td>
  <center>
  -</center>
  </td>
  <td>
  <div align="left">
  <dl>
  <dt>optical material of the shape or name of the other shape</div>
  <dt>having same material and optical properties. (see FromOther).
  </dl>
  </td>
  </tr>
  <tr>
  <td>const char</td>
  <td>*namerev</td>
  <td>
  <center>
  -</center>
  </td>
  <td>
  <center>
  -</center>
  </td>
  <td>name of the default revetment for the 6 faces</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>dz</td>
  <td>
  <center>
  -</center>
  </td>
  <td>d<sub>z</sub></td>
  <td>half-length along the z axis in cm
  <ul>
  <li>-dz : z coordinate of points 0/1/2/3
  <li>+dz : z coordinate of points 4/5/6/7
  </ul>
  </td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>x0</td>
  <td>in fV0</td>
  <td>x<sub>0</sub></td>
  <td>x coordinate of point (0) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>y0</td>
  <td>in fV0</td>
  <td>y<sub>0</sub></td>
  <td>y coordinate of point (0) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>x1</td>
  <td>in fV1</td>
  <td>x<sub>1</sub></td>
  <td>x coordinate of point (1) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>y1</td>
  <td>in fV1</td>
  <td>y<sub>1</sub></td>
  <td>y coordinate of point (1) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>x2</td>
  <td>in fV2</td>
  <td>x<sub>2</sub></td>
  <td>x coordinate of point (2) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>y2</td>
  <td>in fV2</td>
  <td>y<sub>2</sub></td>
  <td>y coordinate of point (2) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>x3</td>
  <td>in fV3</td>
  <td>x<sub>3</sub></td>
  <td>x coordinate of point (3) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>y3</td>
  <td>in fV3</td>
  <td>y<sub>3</sub></td>
  <td>y coordinate of point (3) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>x4</td>
  <td>in fV4</td>
  <td>x<sub>4</sub></td>
  <td>x coordinate of point (4) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>y4</td>
  <td>in fV4</td>
  <td>y<sub>4</sub></td>
  <td>y coordinate of point (4) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>x5</td>
  <td>in fV5</td>
  <td>x<sub>5</sub></td>
  <td>x coordinate of point (5) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>y5</td>
  <td>in fV5</td>
  <td>y<sub>5</sub></td>
  <td>y coordinate of point (5) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>x6</td>
  <td>in fV6</td>
  <td>x<sub>6</sub></td>
  <td>x coordinate of point (6) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>y6</td>
  <td>in fV6</td>
  <td>y<sub>6</sub></td>
  <td>y coordinate of point (5) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>x7</td>
  <td>in fV7</td>
  <td>x<sub>7</sub></td>
  <td>x coordinate of point (7) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>y7</td>
  <td>
  <center>
  -</center>
  </td>
  <td>y<sub>7</sub></td>
  <td>y coordinate of point (7) wanted by the user.</td>
  </tr>
  <tr>
  <td>Bool_t</td>
  <td>FromOther</td>
  <td>
  <center>
  -</center>
  </td>
  <td>
  <center>
  -</center>
  </td>
  <td>true if material and optical axis have to be taken from an other shape.</td>
  </tr>
  </table>
  </p>
  </center>
  <pre>
  <!--*/
  // -->END_HTML
  const Double_t small  = 1.0e-4;
  const Double_t zero   = 0.0;
  const Double_t huit   = 8.0;
  Double_t planarity;
  Double_t xcg,ycg;   //center of shape
  TString nameshape;
  TFace *face;
  InitP();
  if (FromOther) {
    nameshape = namemat;
    fSuppl = new TSupplShape(name,title,this,ts8pts,nameshape,namerev);
  }
  else {
    fSuppl = new TSupplShape(name,title,this,ts8pts,namemat,namerev);
  }
  fSuppl->fNbSubPoints = 0;
  fSuppl->fNbPoints = 8;
  fSuppl->fTotalNbPoints = fSuppl->fNbPoints;
  fV0  = new T3Vector(x0,y0,-dz);
  fV1  = new T3Vector(x1,y1,-dz);
  fV2  = new T3Vector(x2,y2,-dz);
  fV3  = new T3Vector(x3,y3,-dz);
  fV4  = new T3Vector(x4,y4,dz);
  fV5  = new T3Vector(x5,y5,dz);
  fV6  = new T3Vector(x6,y6,dz);
  fV7  = new T3Vector(x7,y7,dz);
  planarity = MakePlanar();
  if (planarity>small) {
    gMes->SetName("TS8PTS");
    gMes->SetTitle("TS8PTS");
    gMes->M(Warning_M,1,"Coordinate changed in order to have plane faces");
    gMes->M(Info_M,1,"point 0",*fV0);
    gMes->M(Info_M,1,"point 1",*fV1);
    gMes->M(Info_M,1,"point 2",*fV2);
    gMes->M(Info_M,1,"point 3",*fV3);
    gMes->M(Info_M,1,"point 4",*fV4);
    gMes->M(Info_M,1,"point 5",*fV5);
    gMes->M(Info_M,1,"point 6",*fV6);
    gMes->M(Info_M,1,"point 7",*fV7);
  }
  //
  x0 = (*fV0)(0);
  y0 = (*fV0)(1);
  x1 = (*fV1)(0);
  y1 = (*fV1)(1);
  x2 = (*fV2)(0);
  y2 = (*fV2)(1);
  x3 = (*fV3)(0);
  y3 = (*fV3)(1);
  x4 = (*fV4)(0);
  y4 = (*fV4)(1);
  x5 = (*fV5)(0);
  y5 = (*fV5)(1);
  x6 = (*fV6)(0);
  y6 = (*fV6)(1);
  x7 = (*fV7)(0);
  y7 = (*fV7)(1);
  xcg = (x0+x1+x2+x3+x4+x5+x6+x7)/huit;
  ycg = (y0+y1+y2+y3+y4+y5+y6+y7)/huit;
  fSuppl->fGCenter.Set(xcg,ycg,zero);
  //
  // Notice this important difference between the vectors fVi and the vectors
  //vi : 
  //  - vectors fVi always keep there original coordinates. They are not chan-
  //by a call to TSNode. They remain in class TS8PTS and are not given to
  //the class TSupplShape. They stay in the local coordinates of the shape.
  //  - vectors vi change when a call to TSNode is made. They move into World
  //Coordinate. They are given to the class TSupplShape and then handled by
  //TSupplShape. They have not to be deleted here, but by TSupplShape.
  //
  T3Vector *v0 = new T3Vector(x0,y0,-dz);
  T3Vector *v1 = new T3Vector(x1,y1,-dz);
  T3Vector *v2 = new T3Vector(x2,y2,-dz);
  T3Vector *v3 = new T3Vector(x3,y3,-dz);
  T3Vector *v4 = new T3Vector(x4,y4,+dz);
  T3Vector *v5 = new T3Vector(x5,y5,+dz);
  T3Vector *v6 = new T3Vector(x6,y6,+dz);
  T3Vector *v7 = new T3Vector(x7,y7,+dz);
  fSuppl->fPoints = new TObjArray();
  fSuppl->fPoints->Add(v0);
  fSuppl->fPoints->Add(v1);
  fSuppl->fPoints->Add(v2);
  fSuppl->fPoints->Add(v3);
  fSuppl->fPoints->Add(v4);
  fSuppl->fPoints->Add(v5);
  fSuppl->fPoints->Add(v6);
  fSuppl->fPoints->Add(v7);
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
TS8PTS::~TS8PTS() {
  //Destructor.  Notice that memory for vectors fV0..fV7 is released by the
  //call to delete fSuppl. No need to delete fVn here.
  if (fSuppl) {
    delete fSuppl;
    fSuppl = 0;
  }
}
void TS8PTS::DelPoints() {
  //Delete the 8 points of the apexes
  if (fV0) delete fV0;
  if (fV1) delete fV1;
  if (fV2) delete fV2;
  if (fV3) delete fV3;
  if (fV4) delete fV4;
  if (fV5) delete fV5;
  if (fV6) delete fV6;
  if (fV7) delete fV7;
  fV0 = 0;
  fV1 = 0;
  fV2 = 0;
  fV3 = 0;
  fV4 = 0;
  fV5 = 0;
  fV6 = 0;
  fV7 = 0;
}
void TS8PTS::InitP() {
  // Pointers to 0
  fSuppl = 0;
  fV0    = 0;
  fV1    = 0;
  fV2    = 0;
  fV3    = 0;
  fV4    = 0;
  fV5    = 0;
  fV6    = 0;
  fV7    = 0;
}
Double_t TS8PTS::MakePlanar() {
  //Verifies that the points in fV0..fV7 give plane faces. The return value
  //is a measure of how non-planar were the faces, before that MakePlanar()
  //moves them in order to have plane faces.
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t vsmall = 1.0e-12;
  Int_t n;
  Double_t tt1,tt2,a,b;
  Double_t ddkeep = zero;
  Double_t l;
  Double_t epsx, epsy;
  Double_t dd  = 0.0;
  Double_t dtt = 100.0;
  T3Vector v(0);
  T3Vector n0(0);
  T3Vector n1(0);
  T3Vector n2(0);
  T3Vector n3(0);
  n = 0;
  while ((dtt>vsmall) && (n<20)) {
    dtt = zero;
    // FACE 0  optimizes fV5
    n0   = (*fV1 - *fV0)^(*fV4 - *fV0);
    v    = *fV5 - *fV0;
    tt1  = n0*v;
    a    = n0(0);  b = n0(1);
    tt2  = (a*a + b*b)/deux;  l = tt1/tt2;
    epsx = -(l*a)/deux;  epsy = -(l*b)/deux;
    dd   = TMath::Sqrt(epsx*epsx + epsy*epsy);
    dtt += dd;
    (*fV5)(0) = (*fV5)(0) + epsx;
    (*fV5)(1) = (*fV5)(1) + epsy;
    // FACE 1  optimizes fV6
    n1   = (*fV2 - *fV1)^(*fV5 - *fV1);
    v    = *fV6 - *fV1;
    tt1  = n1*v;
    a    = n1(0);             b = n1(1);
    tt2  = (a*a + b*b)/deux;  l = tt1/tt2;
    epsx = -(l*a)/deux;  epsy = -(l*b)/deux;
    dd   = TMath::Sqrt(epsx*epsx + epsy*epsy);
    dtt += dd;
    (*fV6)(0) = (*fV6)(0) + epsx;
    (*fV6)(1) = (*fV6)(1) + epsy;
    // FACE 2  optimizes fV7
    n2   = (*fV3 - *fV2)^(*fV6 - *fV2);
    v    = *fV7 - *fV2;
    tt1  = n2*v;
    a    = n2(0);             b = n2(1);
    tt2  = (a*a + b*b)/deux;  l = tt1/tt2;
    epsx = -(l*a)/deux;  epsy = -(l*b)/deux;
    dd   = TMath::Sqrt(epsx*epsx + epsy*epsy);
    dtt += dd;
    (*fV7)(0) = (*fV7)(0) + epsx;
    (*fV7)(1) = (*fV7)(1) + epsy;
    // FACE 3  optimizes fV4
    n3   = (*fV0 - *fV3)^(*fV7 - *fV3);
    v    = *fV4 - *fV3;
    tt1  = n3*v;
    a    = n3(0);             b = n3(1);
    tt2  = (a*a + b*b)/deux;  l = tt1/tt2;
    epsx = -(l*a)/deux;  epsy = -(l*b)/deux;
    dd   = TMath::Sqrt(epsx*epsx + epsy*epsy);
    dtt += dd;
    (*fV4)(0) = (*fV4)(0) + epsx;
    (*fV4)(1) = (*fV4)(1) + epsy;
    if (!n) ddkeep = dtt;
    n   += 1;
  }
  if (dtt>vsmall) {
    gMes->SetName("MakePlanar");
    gMes->SetTitle("TSTRAP");
    gMes->M(kill_prog,1,"Cannot get faces to be plane");
  }
  return ddkeep;
}
void TS8PTS::SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const {
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
void TS8PTS::SetPoints(Float_t *buff) const {
  //  overrides the TBRIK::SetPoints() method. Hope that drawing will be ok !
  //
  if (buff) {
    //
    buff[ 0] = Float_t((*fV0)(0));
    buff[ 1] = Float_t((*fV0)(1));
    buff[ 2] = Float_t((*fV0)(2));
    //
    buff[ 3] = Float_t((*fV1)(0));
    buff[ 4] = Float_t((*fV1)(1));
    buff[ 5] = Float_t((*fV1)(2));
    //
    buff[ 6] = Float_t((*fV2)(0));
    buff[ 7] = Float_t((*fV2)(1));
    buff[ 8] = Float_t((*fV2)(2));
    //
    buff[ 9] = Float_t((*fV3)(0));
    buff[10] = Float_t((*fV3)(1));
    buff[11] = Float_t((*fV3)(2));
    //
    buff[12] = Float_t((*fV4)(0));
    buff[13] = Float_t((*fV4)(1));
    buff[14] = Float_t((*fV4)(2));
    //
    buff[15] = Float_t((*fV5)(0));
    buff[16] = Float_t((*fV5)(1));
    buff[17] = Float_t((*fV5)(2));
    //
    buff[18] = Float_t((*fV6)(0));
    buff[19] = Float_t((*fV6)(1));
    buff[20] = Float_t((*fV6)(2));
    //
    buff[21] = Float_t((*fV7)(0));
    buff[22] = Float_t((*fV7)(1));
    buff[23] = Float_t((*fV7)(2));
  }
}
