// @(#)root/html:$Id: TSGTRA.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "T3Vector.h"
#include "TMessErr.h"
#include "TFace.h"
#include "TSupplShape.h"
#include "TSGTRA.h"

ClassImp(TSGTRA)
//______________________________________________________________________________
//
//BEGIN_HTML <!--
/* -->
</pre>
<h4><font color="#990000">Recall that all dimensions of shapes are in cm in Litrani and that all angles of shapes are in degrees, not in radians.</font></h4>
<p><b>TSGTRA</b> inherits from <b>TGTRA</b> and adds supplementary information required for Litrani, in particular the default revetment of all faces.</p>
<p>Shape <b>TSGTRA</b> is the most general shape with 8 apexes available in ROOT or GEANT. See the drawing of a <b>TGTRA</b> in the ROOT documentation of <b>TGTRA</b>.</p>
<p><b>TSGTRA</b> is a general twisted trapezoid. The faces perpendicular to z are trapeze and their centres are not necessarily on a line parallel to the z axis as the <b>TSTRAP</b>; additionally, the faces may be twisted so that none of their edges are parallel. It is a <b>TSTRAP</b> shape, except that it is twisted in the x-y plane as a function of z. The parallel sides perpendicular to the z axis are rotated with respect to the x axis by an angle fDtwist, which is one of the parameters. The shape is defined by the eight corners and is assumed to be constructed of straight lines joining points on the boundary of the trapezoidal face at z=-dz to the corresponding points on the face at z=dz.</p>
<center>
<p>
<table border="6" cellpadding="2" cellspacing="3" bgcolor="#cccc99">
<caption>
<h4>17 parameters of the first constructor</h4>
</caption>
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
<td>title of the shape</td>
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
<td>optical material of the shape or name of the other shape having same material and optical properties. (see FromOther).</td>
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
<td>fDdz</td>
<td>d<sub>z</sub></td>
<td>half-length along the z axis in cm</td>
</tr>
<tr>
<td>Double_t</td>
<td>theta</td>
<td>fDtheta</td>
<td><font face="Symbol">q</font></td>
<td>polar angle of the line joining the centre of the face at -dz to the centre of the one at +dz. <font color="#9c0031"><b>0 &lt;= </b></font><font face="Symbol" color="#9c0031"><b>q</b></font><b><font color="#9c0031"> &lt; 90&deg;</font></b></td>
</tr>
<tr>
<td>Double_t</td>
<td>phi</td>
<td>fDphi</td>
<td><font face="Symbol">f</font></td>
<td>azimuthal angle of the line joining the centre of the face at -dz to the centre of the one at +dz. <font color="#9c0031"><b>0 &lt;= </b></font><font face="Symbol" color="#9c0031"><b>f</b></font><b><font color="#9c0031"> &lt; 360&deg;</font></b></td>
</tr>
<tr>
<td>Double_t</td>
<td>twist</td>
<td>fDtwist</td>
<td><font face="Symbol">t</font></td>
<td>twist angle of the faces parallel to the x-y plane at z = +/- dz around an axis parallel to z passing through their centre</td>
</tr>
<tr>
<td>Double_t</td>
<td>h1</td>
<td>fDh1</td>
<td>h<font size="2"><sub>1</sub></font></td>
<td>half-length along y of the face at -dz in cm</td>
</tr>
<tr>
<td>Double_t</td>
<td>bl1</td>
<td>fDbl1</td>
<td>b<sub>1</sub></td>
<td>half-length along x of the side at -h1 in y of the face at -dz in z in cm.</td>
</tr>
<tr>
<td>Double_t</td>
<td>tl1</td>
<td>fDtl1</td>
<td>t<sub>1</sub></td>
<td>half-length along x of the side at +h1 in y of the face at -dz in z in cm.</td>
</tr>
<tr>
<td>Double_t</td>
<td>alpha1</td>
<td>fDalpha1</td>
<td><font face="Symbol">a</font><sub>1</sub></td>
<td>angle with respect to the y axis from the centre of the side at -h1 in y to the centre of the side at +h1 in y of the face at -dz in z.</td>
</tr>
<tr>
<td>Double_t</td>
<td>h2</td>
<td>fDh2</td>
<td>h<sub>2</sub></td>
<td>half-length along y of the face at +dz in cm.</td>
</tr>
<tr>
<td>Double_t</td>
<td>bl2</td>
<td>fDbl2</td>
<td>b<sub>2</sub></td>
<td>half-length along x of the side at -h2 in y of the face at +dz in z in cm.</td>
</tr>
<tr>
<td>Double_t</td>
<td>tl2</td>
<td>fDtl2</td>
<td>t<sub>2</sub></td>
<td>half-length along x of the side at +h2 in y of the face at +dz in z in cm</td>
</tr>
<tr>
<td>Double_t</td>
<td>alpha2</td>
<td>fDalpha2</td>
<td><font face="Symbol">a</font><sub>2</sub></td>
<td>angle with respect to the y axis from the centre of the side at -h2 in y to the centre of the side at +h2 in y of the face at +dz in z.</td>
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
<p><b>TSGTRA</b> presents the following difficulty : nothing prevents the user to give values to the parameters such that the lateral faces are not plane ! In order to prevent this, checks are done in the constructor of <b>TSGTRA</b> and values are changed in order that faces become plane. More specifically, the 8 points of the shape are calculated using all the input parameters. Points 0 1 2 3 are always taken as given, points 4 5 6 7 are moved the least possible, but such that faces 0 1 2 3 become plane. Then one recalculates the new values of the parameters from the new coordinates of the points and inform the user by a print that the parameters have been changed. The new values of the parameters or of the points can then be obtained by the Get... methods.</p>
<p>Notice that very often one is in the situation where one knows the coordinates of the 8 pts of the shape, but not the values of all the parameters given above. For this kind of situation, we provide a second constructor. See below. We first intend to show how one goes from parameters to coordinates and vice-versa.</p>
<h3>Relation between the parameters and coordinates of the apexes of the shape.</h3>
<p>Let us define ( x<sub>k</sub>,y<sub>k</sub>,z<sub>k </sub>) k = 0..7 the coordinates of the 8 apexes of a <b>TSGTRA</b>. Suppose that we have <b>a very general shape with 8 apexes</b>, satisfying <b>only</b> to the following conditions :</p>
<center>
<p>
<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
<caption></caption>
<tr>
<td>
<center>
-</center>
</td>
<td>
<center>
<h4>conditions</h4>
</center>
</td>
</tr>
<tr>
<td>(1)</td>
<td>z<sub>0</sub> = z<sub>1</sub> = z<sub>2</sub> = z<sub>3</sub> = - d<sub>z</sub></td>
</tr>
<tr>
<td>(2)</td>
<td>z<sub>4</sub> = z<sub>5</sub> = z<sub>6</sub> = z<sub>7</sub> = + d<sub>z</sub></td>
</tr>
<tr>
<td>(3)</td>
<td>y<sub>1</sub> = y<sub>0</sub></td>
</tr>
<tr>
<td>(4)</td>
<td>y<sub>3</sub> = y<sub>2</sub></td>
</tr>
<tr>
<td>(5)</td>
<td>(y<sub>4</sub>-y<sub>5</sub>)/(x<sub>4</sub>-x<sub>5</sub>) = (y<sub>6</sub>-y<sub>7</sub>)/(x<sub>6</sub>-x<sub>7</sub>)</td>
</tr>
<tr>
<td>(6)</td>
<td>x<sub>1</sub> = - x<sub>0</sub></td>
</tr>
<tr>
<td>(7)</td>
<td>y<sub>2</sub> = - y<sub>0</sub></td>
</tr>
</table>
</p>
</center>
<p>If you compare this list of 7 conditions for a <b>TSGTRA</b> with the list given of 6 conditions for a <b>TSTRAP</b>, you could wrongly conclude that a <b>TSTRAP</b> is a more general shape than a <b>TSGTRA</b>, because it has one condition less.&nbsp;This is wrong, because for <b>TSTRAP</b>, we have left free the possibility of a translation along Ox and Oy of all x,y coordinates.&nbsp;On the contrary, here, with conditions (6) and (7), we fix the position of the coordinate system : (6) and (7) <b>are not really conditions</b> : they can always be satisfied with translation in the Oxy plane. So that in fact we have only 5 conditions for the <b>TSGTRA</b> as compared with 6 for a <b>TSTRAP</b>. <b>TSGTRA</b> is the most general shape of ROOT. In Litrani, you also have the shape <b>TS8PTS</b> which is in principle more general but which it is better to avoid.</p>
<p>So the true conditions are 1 to 5. If not satisfied, your shape cannot be represented by a <b>TSGTRA</b>. It remains to you to try the shape <b>TS8PTS</b>, with its disadvantages. (but notice that <b>TS8PTS</b> has also conditions (1) and (2) ).</p>
<p>The relations between the coordinates and the parameters for a <b>TSGTRA</b> are the following :</p>
<center>
<p>
<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
<caption>relation between the coordinates and the parameters for a <b>TSGTRA</b></caption>
<tr>
<td>
<center>
<h4>coord.</h4>
</center>
</td>
<td>
<center>
-</center>
</td>
<td>
<center>
<h4>relation</h4>
</center>
</td>
<td>
<center>
<h4>Eq.</h4>
</center>
</td>
<td>
<center>
<h4>comments</h4>
</center>
</td>
</tr>
<tr>
<td>x<sub>0</sub></td>
<td>
<center>
=</center>
</td>
<td>- b<sub>1</sub></td>
<td>(1)</td>
<td><b>c(6) ok :</b></td>
</tr>
<tr>
<td>x<sub>1</sub></td>
<td>
<center>
=</center>
</td>
<td>+ b<sub>1</sub></td>
<td>(2)</td>
<td>
<dl>
<dt><font color="#9c0031"><b>==&gt; x<sub>1</sub> = - x<sub>0</sub></b></font>
<dt><font color="#9c0031"><b>==&gt; b<sub>1</sub></b></font>
</dl>
</td>
</tr>
<tr>
<td>x<sub>2</sub></td>
<td>
<center>
=</center>
</td>
<td>+ t<sub>1 </sub>+ 2 h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub></td>
<td>(3)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>3</sub></td>
<td>
<center>
=</center>
</td>
<td>- t<sub>1 </sub>+ 2 h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub></td>
<td>(4)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>y<sub>0 </sub></td>
<td>
<center>
=</center>
</td>
<td>- h<font size="2"><sub>1</sub></font></td>
<td>(5)</td>
<td><b>c(3) ok :</b></td>
</tr>
<tr>
<td>y<sub>1</sub></td>
<td>
<center>
=</center>
</td>
<td>- h<font size="2"><sub>1</sub></font></td>
<td>(6)</td>
<td>
<dl>
<dt><font color="#9c3031"><b>==&gt; y<sub>1 </sub>= y<sub>0</sub></b></font>
<dt><font color="#9c0031"><b>==&gt; h<sub>1</sub></b></font>
</dl>
</td>
</tr>
<tr>
<td>y<sub>2</sub></td>
<td>
<center>
=</center>
</td>
<td>+ h<font size="2"><sub>1</sub></font></td>
<td>(7)</td>
<td><b>c(4) ok :</b></td>
</tr>
<tr>
<td>y<sub>3</sub></td>
<td>
<center>
=</center>
</td>
<td>+ h<font size="2"><sub>1</sub></font></td>
<td>(8)</td>
<td><font color="#9c3031"><b>==&gt; y<sub>3 </sub>= y<sub>2</sub></b></font></td>
</tr>
<tr>
<td>x<sub>4</sub></td>
<td>
<center>
=</center>
</td>
<td>( -b<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f </font>) cos<font face="Symbol">t</font> + ( -h<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) sin<font face="Symbol">t</font></td>
<td>(9)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>5</sub></td>
<td>
<center>
=</center>
</td>
<td>( b<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f </font>) cos<font face="Symbol">t</font> + ( -h<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) sin<font face="Symbol">t</font></td>
<td>(10)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>6</sub></td>
<td>
<center>
=</center>
</td>
<td>( t<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f + 2</font>h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2 </sub>) cos<font face="Symbol">t</font> + ( h<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) sin<font face="Symbol">t</font></td>
<td>(11)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>7</sub></td>
<td>
<center>
=</center>
</td>
<td>( -t<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f + 2</font>h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2 </sub>) cos<font face="Symbol">t</font> + ( h<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) sin<font face="Symbol">t</font></td>
<td>(12)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>y<sub>4</sub></td>
<td>
<center>
=</center>
</td>
<td>- ( -b<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f </font>) sin<font face="Symbol">t</font> + ( -h<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) cos<font face="Symbol">t</font></td>
<td>(13)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>y<sub>5</sub></td>
<td>
<center>
=</center>
</td>
<td>- ( b<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f </font>) sin<font face="Symbol">t</font> + ( -h<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) cos<font face="Symbol">t</font></td>
<td>(14)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>y<sub>6</sub></td>
<td>
<center>
=</center>
</td>
<td>- ( t<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f + 2</font>h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub><font face="Symbol"> </font>) sin<font face="Symbol">t</font> + ( h<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) cos<font face="Symbol">t</font></td>
<td>(15)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>y<sub>7</sub></td>
<td>
<center>
=</center>
</td>
<td>- ( -t<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f + 2</font>h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub><font face="Symbol"> </font>) sin<font face="Symbol">t</font> + ( h<font size="2"><sub>2</sub></font> + 2d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) cos<font face="Symbol">t</font></td>
<td>(16)</td>
<td>
<center>
-</center>
</td>
</tr>
</table>
</p>
<p>
<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
<caption>
<h4>From equations (1) to (16), we obtain :</h4>
</caption>
<tr>
<td>
<center>
<h4>From</h4>
</center>
</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
<td>
<center>
<h4>Eq.</h4>
</center>
</td>
<td>
<center>
<h4>comments</h4>
</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(3) - (4)</font></center>
</td>
<td>
<div align="right">
x<sub>2 </sub>- x<sub>3</sub></div>
</td>
<td>=</td>
<td>2t<font size="2"><sub>1</sub></font></td>
<td>(17)</td>
<td><font color="#9c3031"><b>==&gt; t</b></font><font size="2" color="#9c3031"><sub><b>1</b></sub></font></td>
</tr>
<tr>
<td>
<center>
<font size="2">(3)+ (4)</font></center>
</td>
<td>
<div align="right">
x<sub>2 </sub>+ x<sub>3</sub></div>
</td>
<td>=</td>
<td>4 h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub></td>
<td>(18)</td>
<td><font color="#9c0031"><b>==&gt; </b></font><font face="Symbol" color="#9c0031"><b>a</b></font><b><font color="#9c0031"><sub>1</sub></font></b></td>
</tr>
<tr>
<td>
<center>
<font size="2">(9) - (10)</font></center>
</td>
<td>
<div align="right">
x<sub>4 </sub>- x<sub>5</sub></div>
</td>
<td>=</td>
<td>- 2 b<font size="2"><sub>2 </sub></font>cos<font face="Symbol">t</font></td>
<td>(19)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(13) - (14)</font></center>
</td>
<td>
<div align="right">
y<sub>4</sub> - y<sub>5</sub></div>
</td>
<td>=</td>
<td>+ 2 b<font size="2"><sub>2 </sub></font>sin<font face="Symbol">t</font></td>
<td>(20)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(11) - (12)</font></center>
</td>
<td>
<div align="right">
x<sub>6</sub> - x<sub>7</sub></div>
</td>
<td>=</td>
<td>+ 2 t<font size="2"><sub>2 </sub></font>cos<font face="Symbol">t</font></td>
<td>(21)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(15) - (16)</font></center>
</td>
<td>
<div align="right">
y<sub>6</sub> - y<sub>7</sub></div>
</td>
<td>=</td>
<td>- 2 t<font size="2"><sub>2 </sub></font>sin<font face="Symbol">t</font></td>
<td>(22)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(20) / (19)</font></center>
</td>
<td>
<div align="right">
( y<sub>4</sub> - y<sub>5</sub> ) / ( x<sub>4 </sub>- x<sub>5</sub> )</div>
</td>
<td>=</td>
<td>- tg<font face="Symbol">t</font></td>
<td>(23)</td>
<td>
<dl>
<dt><font color="#9c0031"><b>==&gt; </b></font><font face="Symbol" color="#9c0031"><b>t</b></font>
<dt><font color="#9c3031"><b>(19)==&gt; b</b></font><font size="2" color="#9c3031"><sub><b>2</b></sub></font>
<dt><font color="#9c3031"><b>(21)==&gt; t</b></font><font size="2" color="#9c3031"><sub><b>2</b></sub></font>
</dl>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(22) / (21)</font></center>
</td>
<td>
<div align="right">
( y<sub>6</sub> - y<sub>7</sub> ) / ( x<sub>6</sub> - x<sub>7</sub> )</div>
</td>
<td>=</td>
<td>- tg<font face="Symbol">t</font></td>
<td>(24)</td>
<td><b>c(5) ok !</b></td>
</tr>
<tr>
<td>
<center>
<font size="2">(9) + (10)</font></center>
</td>
<td>
<div align="right">
x<sub>4 </sub>+ x<sub>5</sub></div>
</td>
<td>=</td>
<td>4d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f </font>cos<font face="Symbol">t</font> + ( -2 h<font size="2"><sub>2</sub></font> + 4d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) sin<font face="Symbol">t</font></td>
<td>(25)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(13) + (14)</font></center>
</td>
<td>
<div align="right">
y<sub>4</sub> + y<sub>5</sub></div>
</td>
<td>=</td>
<td>- 4d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f </font>sin<font face="Symbol">t</font> + ( -2 h<font size="2"><sub>2</sub></font> + 4d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) cos<font face="Symbol">t</font></td>
<td>(26)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(11) + (12)</font></center>
</td>
<td>
<div align="right">
x<sub>6</sub> +<sub> </sub>x<sub>7</sub></div>
</td>
<td>=</td>
<td>(4d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f + 4</font>h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub>)cos<font face="Symbol">t</font> + (2h<font size="2"><sub>2</sub></font> + 4d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font>)sin<font face="Symbol">t</font></td>
<td>(27)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(15) + (16)</font></center>
</td>
<td>
<div align="right">
y<sub>6</sub> + y<sub>7</sub></div>
</td>
<td>=</td>
<td>-(4d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f + 4</font>h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub>)sin<font face="Symbol">t</font> + (2h<font size="2"><sub>2</sub></font> + 4d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font>)cos<font face="Symbol">t</font></td>
<td>(28)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(25) &amp; (26)</font></center>
</td>
<td>(x<sub>4 </sub>+ x<sub>5</sub>)sin<font face="Symbol">t</font> + (y<sub>4</sub> + y<sub>5</sub>)cos<font face="Symbol">t</font></td>
<td>=</td>
<td>-2 h<font size="2"><sub>2</sub></font> + 4d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(29)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(27) &amp; (28)</font></center>
</td>
<td>(x<sub>6</sub> +<sub> </sub>x<sub>7</sub>)sin<font face="Symbol">t</font> + (y<sub>6</sub> + y<sub>7</sub>)cos<font face="Symbol">t</font></td>
<td>=</td>
<td>2h<font size="2"><sub>2</sub></font> + 4d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(30)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(30) - (29)</font></center>
</td>
<td>
<div align="right">
4 h<font size="2"><sub>2</sub></font></div>
</td>
<td>=</td>
<td>( x<sub>6</sub> +<sub> </sub>x<sub>7</sub> - x<sub>4 </sub>- x<sub>5 </sub>)sin<font face="Symbol">t</font> + ( y<sub>6</sub> + y<sub>7</sub> - y<sub>4</sub> - y<sub>5 </sub>)cos<font face="Symbol">t</font></td>
<td>(31)</td>
<td><font color="#9c3031"><b>==&gt; h</b></font><font size="2" color="#9c3031"><sub><b>2</b></sub></font></td>
</tr>
<tr>
<td>
<center>
<font size="2">(29) + (30)</font></center>
</td>
<td>
<div align="right">
8d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font></div>
</td>
<td>=</td>
<td>( x<sub>4 </sub>+ x<sub>5</sub> + x<sub>6</sub> +<sub> </sub>x<sub>7 </sub>)sin<font face="Symbol">t</font> + ( y<sub>4</sub> + y<sub>5 </sub>+ y<sub>6</sub> + y<sub>7</sub> )cos<font face="Symbol">t</font></td>
<td>(32)</td>
<td><font color="#9c0031"><b>&gt; sin</b></font><font face="Symbol" color="#9c0031"><b>q</b></font><font color="#9c0031"><b> sin</b></font><font face="Symbol" color="#9c0031"><b>f</b></font></td>
</tr>
<tr>
<td>
<center>
<font size="2">(25)</font></center>
</td>
<td>
<div align="right">
4d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f </font>cos<font face="Symbol">t</font></div>
</td>
<td>=</td>
<td>( -2 h<font size="2"><sub>2</sub></font> + 4d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) sin<font face="Symbol">t</font> - ( x<sub>4 </sub>+ x<sub>5</sub> )</td>
<td>(33)</td>
<td><font color="#9c0031"><b>&gt; sin</b></font><font face="Symbol" color="#9c0031"><b>q</b></font><font color="#9c0031"><b> cos</b></font><font face="Symbol" color="#9c0031"><b>f</b></font></td>
</tr>
<tr>
<td>
<center>
<font size="2">(32) &amp; (33)</font></center>
</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
<td>
<dl>
<dt><font color="#9c0031"><b>==&gt; </b></font><font face="Symbol" color="#9c0031"><b>q</b></font>
<dt><font color="#9c0031"><b>==&gt; </b></font><font face="Symbol" color="#9c0031"><b>f</b></font>
</dl>
</td>
</tr>
<tr>
<td>
<center>
<font size="2">(27)</font></center>
</td>
<td>
<div align="right">
<font face="Symbol">4</font>h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2 </sub>cos<font face="Symbol">t</font></div>
</td>
<td>=</td>
<td>( x<sub>6</sub> +<sub> </sub>x<sub>7</sub> ) - 4d<sub>z</sub> sin<font face="Symbol">q</font> cos<font face="Symbol">f</font> cos<font face="Symbol">t</font> - ( 2h<font size="2"><sub>2</sub></font> + 4d<sub>z</sub> sin<font face="Symbol">q</font> sin<font face="Symbol">f</font> ) sin<font face="Symbol">t</font></td>
<td>
<center>
-</center>
</td>
<td><font color="#9c0031"><b>==&gt; </b></font><font face="Symbol" color="#9c0031"><b>a</b></font><b><font color="#9c0031"><sub>2</sub></font></b></td>
</tr>
</table>
</p>
</center>
<p>We have found back all the parameters starting from coordinates satisfying conditions (1) to (7). It proves that all shapes satisfying conditions (1) to (7) can be represented by a <b>TSGTRA</b>. Due to this fact, we are able to offer a second constructor, whose arguments are the coordinates of the points of the apexes instead of the usual argument of a <b>TSGTRA</b> used in the first constructor.</p>
<p>Notice that instead of giving x<sub>0</sub>, y<sub>0</sub>, x<sub>1</sub>, y<sub>1</sub>, y<sub>2</sub>, y<sub>3</sub>, y<sub>7</sub> we give dx, dy, x<sub>2</sub> with :</p>
<center>
<p>
<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
<tr>
<td>x<sub>0</sub> = - dx</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>1</sub> = + dx</td>
<td>x<sub>1</sub> = - x<sub>0</sub></td>
<td>condition (6)</td>
</tr>
<tr>
<td>y<sub>0</sub> = - dy</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>y<sub>2</sub> = + dy</td>
<td>y<sub>2</sub> = - y<sub>0</sub></td>
<td>condition 7</td>
</tr>
<tr>
<td>y<sub>1</sub> = -dy</td>
<td>y<sub>1</sub> = y<sub>0</sub></td>
<td>condition (3)</td>
</tr>
<tr>
<td>y3 = +dy</td>
<td>y3 = y2</td>
<td>condition (4)</td>
</tr>
<tr>
<td>y<sub>7</sub></td>
<td>not given</td>
<td>condition (5)</td>
</tr>
</table>
</p>
</center>
<center>
<p>
<table border="5" cellpadding="1" cellspacing="3" bgcolor="#cccc99">
<caption>
<h4>18 parameters of the second constructor</h4>
</caption>
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
<td>dx</td>
<td>
<center>
-</center>
</td>
<td>d<sub>x</sub></td>
<td>= (x1 - x0)/2</td>
</tr>
<tr>
<td>Double_t</td>
<td>dy</td>
<td>
<center>
-</center>
</td>
<td>d<sub>y</sub></td>
<td>= (y2 - y0)/2</td>
</tr>
<tr>
<td>Double_t</td>
<td>dz</td>
<td>fDdz</td>
<td>d<sub>z</sub></td>
<td>z coord. of points 4/5/6/7, -z coord. of points 0/1/2/3</td>
</tr>
<tr>
<td>Double_t</td>
<td>x2</td>
<td>
<center>
-</center>
</td>
<td>x<sub>2</sub></td>
<td>x coordinate of point (2)</td>
</tr>
<tr>
<td>Double_t</td>
<td>x3</td>
<td>
<center>
-</center>
</td>
<td>x<sub>3</sub></td>
<td>x coordinate of point (3)</td>
</tr>
<tr>
<td>Double_t</td>
<td>x4</td>
<td>
<center>
-</center>
</td>
<td>x<sub>4</sub></td>
<td>x coordinate of point (4)</td>
</tr>
<tr>
<td>Double_t</td>
<td>y4</td>
<td>
<center>
-</center>
</td>
<td>y<sub>4</sub></td>
<td>y coordinate of point (4)</td>
</tr>
<tr>
<td>Double_t</td>
<td>x5</td>
<td>
<center>
-</center>
</td>
<td>x<sub>5</sub></td>
<td>x coordinate of point (5)</td>
</tr>
<tr>
<td>Double_t</td>
<td>y5</td>
<td>
<center>
-</center>
</td>
<td>y<sub>5</sub></td>
<td>y coordinate of point (5)</td>
</tr>
<tr>
<td>Double_t</td>
<td>x6</td>
<td>
<center>
-</center>
</td>
<td>x<sub>6</sub></td>
<td>x coordinate of point (6)</td>
</tr>
<tr>
<td>Double_t</td>
<td>y6</td>
<td>
<center>
-</center>
</td>
<td>y<sub>6</sub></td>
<td>y coordinate of point (6)</td>
</tr>
<tr>
<td>Double_t</td>
<td>x7</td>
<td>
<center>
-</center>
</td>
<td>x<sub>7</sub></td>
<td>x coordinate of point (7)</td>
</tr>
<tr>
<td>Int_t</td>
<td>PrintPts</td>
<td>
<center>
-</center>
</td>
<td>
<center>
-</center>
</td>
<td>if !=0, coordinates of points after check will be printed</td>
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
<div align="left">
<p>Look at the macros xanibeam10.C and xanibeam11.C to have examples of the use of <b>TSGTRA</b>.</p>
</div>
<p>Finally a <b>not necessary</b>, but <b>convenient</b> condition. If it disturbs you to have negative values for the parameters t<sub>1</sub> , b<sub>1</sub> , t<sub>2</sub> , b<sub>2</sub>, h<sub>1</sub>, h<sub>2</sub> ( on a mathematical point of view, it does not matter, on a geometrical point of view, it does ), number your points in such a way that :</p>
<center>
<p>
<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
<caption>
<h4>not necessary, but convenient conditions</h4>
</caption>
<tr>
<td>
<center>
<h4>Point</h4>
</center>
</td>
<td>
<center>
<h4>condition</h4>
</center>
</td>
<td>
<center>
<h4>condition</h4>
</center>
</td>
<td>
<center>
<h4>Point</h4>
</center>
</td>
<td>
<center>
<h4>condition</h4>
</center>
</td>
<td>
<center>
<h4>condition</h4>
</center>
</td>
</tr>
<tr>
<td>
<center>
(0)</center>
</td>
<td>
<center>
x<sub>0</sub>&nbsp;&lt; 0</center>
</td>
<td>
<center>
y<sub>0 </sub>&lt; 0</center>
</td>
<td>
<center>
(4)</center>
</td>
<td>x<sub>4</sub>&nbsp;&lt; 0 or x<sub>4</sub> small</td>
<td>y<sub>4 </sub>&lt; 0 or y<sub>4</sub> small</td>
</tr>
<tr>
<td>
<center>
(1)</center>
</td>
<td>
<center>
x<sub>1</sub>&nbsp;&gt; 0</center>
</td>
<td>
<center>
y<sub>1 </sub>&lt; 0</center>
</td>
<td>
<center>
(5)</center>
</td>
<td>x<sub>5</sub>&nbsp;&gt; 0 or x<sub>5</sub> big</td>
<td>y<sub>5 </sub>&lt; 0 or y<sub>5</sub> small</td>
</tr>
<tr>
<td>
<center>
(2)</center>
</td>
<td>
<center>
x<sub>2</sub>&nbsp;&gt; 0</center>
</td>
<td>
<center>
y<sub>2 </sub>&gt; 0</center>
</td>
<td>
<center>
(6)</center>
</td>
<td>x<sub>6</sub>&nbsp;&gt; 0 or x<sub>6</sub> big</td>
<td>y<sub>6 </sub>&gt; 0 or y<sub>6</sub> big</td>
</tr>
<tr>
<td>
<center>
(3)</center>
</td>
<td>
<center>
x<sub>3</sub>&nbsp;&lt; 0</center>
</td>
<td>
<center>
y<sub>3 </sub>&gt; 0</center>
</td>
<td>
<center>
(7)</center>
</td>
<td>x<sub>7</sub>&nbsp;&lt; 0 or x<sub>7</sub> small</td>
<td>y<sub>7 </sub>&gt; 0 or y<sub>7</sub> big</td>
</tr>
</table>
</p>
</center>
<p>The 6 faces of the <b>TSGTRA</b> are built from these points in the following way :</p>
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
<p>For each shape of Litrani {tstrap,tstrd1,tstrd2,tspara,tstrap,tsgtra,ts8pts,tscyl,tstube,tscone} there is an associated &quot;friend&quot; class <b>TSupplShape</b>, which contains everything which is common to all kind of shapes. This associated class may be reached from <b>TSGTRA</b> (or from any other shape) like this (supposing that pgtra is a pointer on a <b>TSGTRA</b>) :</p>
<center>
<p><font color="#9c3031"><b><code>TSupplShape *suppl = pgtra-&gt;fSuppl;</code></b></font></p>
</center>
<p>All shapes in Litrani may at the same time be a detector of photon. For instance, a shape may represent a phototube or an APD. See the description how to define a shape as a detector in the presentation of the <b>TSupplShape</b> class. If FromOther is true, it means that the material of the shape and the optical axis are taken from an other shape. If this is the case, it means that in case of transition between this shape and the other shape having same material and same optical axis, the photon simply continues from one shape to the other, without any transition calculation.
<pre>
<!--*/
// -->END_HTML
//
TSGTRA::TSGTRA (const char *name,const char *title,const char *namemat,
  const char *namerev,Double_t dz,Double_t theta,Double_t phi,Double_t twist,
  Double_t h1,Double_t bl1,Double_t tl1,Double_t alpha1,Double_t h2, Double_t bl2,
  Double_t tl2, Double_t alpha2,Bool_t FromOther):TGTRA (name,title,namemat,
  Float_t(dz),Float_t(theta),Float_t(phi),Float_t(twist),Float_t(h1),Float_t(bl1),
  Float_t(tl1),Float_t(alpha1),Float_t(h2),Float_t(bl2),Float_t(tl2),Float_t(alpha2)) {
  //Constructor with the usual arguments of a TSGTRA.
  //BEGIN_HTML <!--
  /* -->
  </pre>
  <center>
  <p>
  <table border="6" cellpadding="2" cellspacing="3" bgcolor="#cccc99">
  <caption>
  <h4>17 parameters of the first constructor</h4>
  </caption>
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
  <td>title of the shape</td>
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
  <td>optical material of the shape or name of the other shape having same material and optical properties. (see FromOther).</td>
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
  <td>fDdz</td>
  <td>d<sub>z</sub></td>
  <td>half-length along the z axis in cm</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>theta</td>
  <td>fDtheta</td>
  <td><font face="Symbol">q</font></td>
  <td>polar angle of the line joining the centre of the face at -dz to the centre of the one at +dz. <font color="#9c0031"><b>0 &lt;= </b></font><font face="Symbol" color="#9c0031"><b>q</b></font><b><font color="#9c0031"> &lt; 90&deg;</font></b></td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>phi</td>
  <td>fDphi</td>
  <td><font face="Symbol">f</font></td>
  <td>azimuthal angle of the line joining the centre of the face at -dz to the centre of the one at +dz. <font color="#9c0031"><b>0 &lt;= </b></font><font face="Symbol" color="#9c0031"><b>f</b></font><b><font color="#9c0031"> &lt; 360&deg;</font></b></td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>twist</td>
  <td>fDtwist</td>
  <td><font face="Symbol">t</font></td>
  <td>twist angle of the faces parallel to the x-y plane at z = +/- dz around an axis parallel to z passing through their centre</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>h1</td>
  <td>fDh1</td>
  <td>h<font size="2"><sub>1</sub></font></td>
  <td>half-length along y of the face at -dz in cm</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>bl1</td>
  <td>fDbl1</td>
  <td>b<sub>1</sub></td>
  <td>half-length along x of the side at -h1 in y of the face at -dz in z in cm.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>tl1</td>
  <td>fDtl1</td>
  <td>t<sub>1</sub></td>
  <td>half-length along x of the side at +h1 in y of the face at -dz in z in cm.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>alpha1</td>
  <td>fDalpha1</td>
  <td><font face="Symbol">a</font><sub>1</sub></td>
  <td>angle with respect to the y axis from the centre of the side at -h1 in y to the centre of the side at +h1 in y of the face at -dz in z.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>h2</td>
  <td>fDh2</td>
  <td>h<sub>2</sub></td>
  <td>half-length along y of the face at +dz in cm.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>bl2</td>
  <td>fDbl2</td>
  <td>b<sub>2</sub></td>
  <td>half-length along x of the side at -h2 in y of the face at +dz in z in cm.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>tl2</td>
  <td>fDtl2</td>
  <td>t<sub>2</sub></td>
  <td>half-length along x of the side at +h2 in y of the face at +dz in z in cm</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>alpha2</td>
  <td>fDalpha2</td>
  <td><font face="Symbol">a</font><sub>2</sub></td>
  <td>angle with respect to the y axis from the centre of the side at -h2 in y to the centre of the side at +h2 in y of the face at +dz in z.</td>
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
  //
  const Double_t zero    = 0.0;
  const Double_t small   = 1.0e-4;
  const Double_t pis2d   = 90.0;
  const Double_t deuxpid = 360.0;
  Double_t planarity;
  TString nameshape;
  TFace *face;
  InitP();
  fDdz     = dz;
  gMes->SetName("TSGTRA");
  gMes->SetTitle("TSGTRA");
  if (theta<zero) {
    gMes->M(kill_prog,1,"theta has to be >= 0");
  }
  if (theta>pis2d) {
    gMes->M(kill_prog,2,"theta has to be < 90");
  }
  fDtheta  = theta;
  if (phi<zero) {
    gMes->M(Error_M,3,"phi has to be >= 0. We add 360");
    while (phi<zero) phi   += deuxpid;
  }
  fDphi    = phi;
  fDtwist  = twist;
  fDh1     = h1;
  fDbl1    = bl1;
  fDtl1    = tl1;
  fDalpha1 = alpha1;
  fDh2     = h2;
  fDbl2    = bl2;
  fDtl2    = tl2;
  fDalpha2 = alpha2;
  if (FromOther) {
    nameshape = namemat;
    fSuppl = new TSupplShape(name,title,this,tsgtra,nameshape,namerev);
  }
  else {
    fSuppl = new TSupplShape(name,title,this,tsgtra,namemat,namerev);
  }
  fSuppl->fGCenter.Set(zero,zero,zero);
  fSuppl->fNbSubPoints = 0;
  fSuppl->fNbPoints = 8;
  fSuppl->fTotalNbPoints = fSuppl->fNbPoints;
  fSuppl->fPoints = new TObjArray();
  //
  ToPoints();
  planarity = MakePlanar();
  ToParam();
  if (planarity>small) {
    gMes->SetName("TSGTRA");
    gMes->SetTitle("TSGTRA");
    gMes->M(Warning_M,4,"Parameters changed in order to have plane faces");
    Print();
  }
  //
  fSuppl->fPoints->Add(fV0);
  fSuppl->fPoints->Add(fV1);
  fSuppl->fPoints->Add(fV2);
  fSuppl->fPoints->Add(fV3);
  fSuppl->fPoints->Add(fV4);
  fSuppl->fPoints->Add(fV5);
  fSuppl->fPoints->Add(fV6);
  fSuppl->fPoints->Add(fV7);
  fSuppl->fNbMasterFaces = 6;
  fSuppl->fNbSubFaces = 0;
  face = new TFace(this,0,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,0,1,5,4);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,1,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,1,2,6,5);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,2,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,2,3,7,6);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,3,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,3,0,4,7);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,4,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,1,0,3,2);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,5,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,4,5,6,7);
  fSuppl->fFaces.Add(face);
  fSuppl->fFaces.Sort();
}
TSGTRA::TSGTRA (const char *name,const char *title,const char *namemat,
  const char *namerev,Double_t dx,Double_t dy,Double_t dz,Double_t x2,Double_t x3,
  Double_t x4,Double_t y4,Double_t x5,Double_t y5,Double_t x6,Double_t y6,
  Double_t x7,Int_t PrintPar,Bool_t FromOther):TGTRA (name,title,namemat,1.0,1.0,
  1.0,0.0,1.0,1.0,1.0,0.0,1.0,1.0,1.0,0.0) {
  //Second constructor for a TSGTRA, starting from the coordinates of the
  //apexes.
  //BEGIN_HTML <!--
  /* -->
  </pre>
  <center>
  <p>
  <table border="5" cellpadding="1" cellspacing="3" bgcolor="#cccc99">
  <caption>
  <h4>18 parameters of the second constructor</h4>
  </caption>
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
  <td>dx</td>
  <td>
  <center>
  -</center>
  </td>
  <td>d<sub>x</sub></td>
  <td>= (x1 - x0)/2</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>dy</td>
  <td>
  <center>
  -</center>
  </td>
  <td>d<sub>y</sub></td>
  <td>= (y2 - y0)/2</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>dz</td>
  <td>fDdz</td>
  <td>d<sub>z</sub></td>
  <td>z coord. of points 4/5/6/7, -z coord. of points 0/1/2/3</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x2</td>
  <td>
  <center>
  -</center>
  </td>
  <td>x<sub>2</sub></td>
  <td>x coordinate of point (2)</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x3</td>
  <td>
  <center>
  -</center>
  </td>
  <td>x<sub>3</sub></td>
  <td>x coordinate of point (3)</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x4</td>
  <td>
  <center>
  -</center>
  </td>
  <td>x<sub>4</sub></td>
  <td>x coordinate of point (4)</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>y4</td>
  <td>
  <center>
  -</center>
  </td>
  <td>y<sub>4</sub></td>
  <td>y coordinate of point (4)</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x5</td>
  <td>
  <center>
  -</center>
  </td>
  <td>x<sub>5</sub></td>
  <td>x coordinate of point (5) </td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>y5</td>
  <td>
  <center>
  -</center>
  </td>
  <td>y<sub>5</sub></td>
  <td>y coordinate of point (5)</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x6</td>
  <td>
  <center>
  -</center>
  </td>
  <td>x<sub>6</sub></td>
  <td>x coordinate of point (6)</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>y6</td>
  <td>
  <center>
  -</center>
  </td>
  <td>y<sub>6</sub></td>
  <td>y coordinate of point (6)</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x7</td>
  <td>
  <center>
  -</center>
  </td>
  <td>x<sub>7</sub></td>
  <td>x coordinate of point (7)</td>
  </tr>
  <tr>
  <td>Int_t</td>
  <td>PrintPar</td>
  <td>
  <center>
  -</center>
  </td>
  <td>
  <center>
  -</center>
  </td>
  <td>if !=0, coordinates of points after check will be printed</td>
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
  //
  const Double_t zero  = 0.0;
  const Double_t small = 1.0e-4;
  Double_t planarity;
  TString nameshape;
  TFace *face;
  Double_t x0,y0,x1,y1,y2,y3,y7;
  Double_t tgtw;
  InitP();
  if (FromOther) {
    nameshape = namemat;
    fSuppl = new TSupplShape(name,title,this,tsgtra,nameshape,namerev);
  }
  else {
    fSuppl = new TSupplShape(name,title,this,tsgtra,namemat,namerev);
  }
  fSuppl->fGCenter.Set(zero,zero,zero);
  fSuppl->fNbSubPoints = 0;
  fSuppl->fNbPoints = 8;
  fSuppl->fTotalNbPoints = fSuppl->fNbPoints;
  fSuppl->fPoints = new TObjArray();
  fDdz = dz;
  x0   = -dx;
  x1   =  dx;
  y0   = -dy;
  y2   =  dy;
  y1   =  y0;
  y3   =  y2;
  tgtw = - (y4-y5)/(x4-x5);
  y7   =  y6 + tgtw*(x6-x7);
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
    gMes->SetName("TSGTRA");
    gMes->SetTitle("TSGTRA");
    gMes->M(Warning_M,1,"Coordinate changed in order to have plane faces");
    PrintPts();
  }
  ToParam();
  if (PrintPar) Print();
  fSuppl->fPoints->Add(fV0);
  fSuppl->fPoints->Add(fV1);
  fSuppl->fPoints->Add(fV2);
  fSuppl->fPoints->Add(fV3);
  fSuppl->fPoints->Add(fV4);
  fSuppl->fPoints->Add(fV5);
  fSuppl->fPoints->Add(fV6);
  fSuppl->fPoints->Add(fV7);
  fSuppl->fNbMasterFaces = 6;
  fSuppl->fNbSubFaces = 0;
  face = new TFace(this,0,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,0,1,5,4);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,1,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,1,2,6,5);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,2,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,2,3,7,6);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,3,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,3,0,4,7);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,4,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,1,0,3,2);
  fSuppl->fFaces.Add(face);
  face = new TFace(this,5,-1,polygonal,fSuppl->fPoints,
    fSuppl->fGCenter,fSuppl->fDefRev,4,5,6,7);
  fSuppl->fFaces.Add(face);
  fSuppl->fFaces.Sort();
}
TSGTRA::~TSGTRA() {
  //Destructor. Notice that memory for vectors fV0..fV7 is released by the
  //call to delete fSuppl. No need to delete fVn here.
  //  DelPoints();
  if (fSuppl) {
    delete fSuppl;
    fSuppl = 0;
  }
}
void TSGTRA::DelPoints() {
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
T3Vector* TSGTRA::GetfV(Int_t i) const {
  //return the coordinates of the selected point
  T3Vector *v;
  switch (i) {
    case 0:
      v = fV0;
      break;
    case 1:
      v = fV1;
      break;
    case 2:
      v = fV2;
      break;
    case 3:
      v = fV3;
      break;
    case 4:
      v = fV4;
      break;
    case 5:
      v = fV5;
      break;
    case 6:
      v = fV6;
      break;
    case 7:
      v = fV7;
      break;
    default:
      v = fV0;
      break;
  }
  return v;
}
void TSGTRA::InitP() {
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
Double_t TSGTRA::MakePlanar() {
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
    gMes->SetTitle("TSGTRA");
    gMes->M(kill_prog,1,"Cannot get faces to be plane");
  }
  return ddkeep;
}
void TSGTRA::Print() const {
  // Prints the parameters defining the TSTRAP
  gMes->SetName("Print");
  gMes->SetTitle("TSGTRA");
  gMes->M(Info_M,1,"Values of parameters");
  gMes->M(Info_M,1,"dz     : ",fDdz);
  gMes->M(Info_M,1,"theta  : ",fDtheta);
  gMes->M(Info_M,1,"phi    : ",fDphi);
  gMes->M(Info_M,1,"twist  : ",fDtwist);
  gMes->M(Info_M,1,"h1     : ",fDh1);
  gMes->M(Info_M,1,"bl1    : ",fDbl1);
  gMes->M(Info_M,1,"tl1    : ",fDtl1);
  gMes->M(Info_M,1,"alpha1 : ",fDalpha1);
  gMes->M(Info_M,1,"h2     : ",fDh2);
  gMes->M(Info_M,1,"bl2    : ",fDbl2);
  gMes->M(Info_M,1,"tl2    : ",fDtl2);
  gMes->M(Info_M,1,"alpha2 : ",fDalpha2);
}
void TSGTRA::PrintPts() const {
  // Prints the coordinates of the points of the TSTRAP
  gMes->SetName("PrintPts");
  gMes->SetTitle("TSGTRA");
  gMes->M(Info_M,1,"Coordinate of the points of the TSTRAP");
  gMes->M(Info_M,1,"point 0",*fV0);
  gMes->M(Info_M,1,"point 1",*fV1);
  gMes->M(Info_M,1,"point 2",*fV2);
  gMes->M(Info_M,1,"point 3",*fV3);
  gMes->M(Info_M,1,"point 4",*fV4);
  gMes->M(Info_M,1,"point 5",*fV5);
  gMes->M(Info_M,1,"point 6",*fV6);
  gMes->M(Info_M,1,"point 7",*fV7);
}
void TSGTRA::SetOpticalAxis(Double_t thetax, Double_t phix,Double_t thetay,
  Double_t phiy) const {
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
void TSGTRA::ToParam() {
  //Given that the 8 vectors fV0-7 contain the coordinates of the 8 apexes,
  //satisfying to the conditions :
  //
  //  (1)  z0 = z1 = z2 = z3 = -dz
  //  (2)  z4 = z5 = z6 = z7 = +dz
  //  (3)  y1 =  y0
  //  (4)  y3 =  y2
  //  (5)  (y4-y5)/(x4-x5) = (y6-y7)/(x6-x7)
  //  (6)  x1 = -x0
  //  (7)  y2 = -y0
  //
  //then ToParam retrieves the value of the basis parameters of the TSGTRA :
  // fDdz,  fDtheta, fDphi,   fDtwist, fDh1, fDbl1, fDtl1, fDalpha1, fDh2,
  // fDbl2, fDtl2,   fDalpha2
  //
  const Double_t q0   = 0.0;
  const Double_t q1     = 1.0;
  const Double_t q2   = 2.0;
  const Double_t q4 = 4.0;
  const Double_t pideg  = 180.0;
  const Double_t lim    = 1.0e-6;
  const Double_t vsmall = 1.0e-12;
  Double_t pi,deuxpi,facdeg;
  Double_t eps,m1,m2,stw,ctw,a,b,c,d,stsf,stcf,st,sphi,cphi;
  Double_t phi = q0;
  Double_t alpha1,alpha2,theta,twist;
  Double_t x0,x1,x2,x3,x4,x5,x6,x7;
  Double_t y0,y1,y2,y3,y4,y5,y6,y7;
  Double_t z0,z1,z2,z3,z4,z5,z6,z7;
  pi = TMath::Pi();
  deuxpi = q2*pi;
  facdeg = pideg/pi;
  x0 = (*fV0)(0);
  x1 = (*fV1)(0);
  x2 = (*fV2)(0);
  x3 = (*fV3)(0);
  x4 = (*fV4)(0);
  x5 = (*fV5)(0);
  x6 = (*fV6)(0);
  x7 = (*fV7)(0);
  y0 = (*fV0)(1);
  y1 = (*fV1)(1);
  y2 = (*fV2)(1);
  y3 = (*fV3)(1);
  y4 = (*fV4)(1);
  y5 = (*fV5)(1);
  y6 = (*fV6)(1);
  y7 = (*fV7)(1);
  z0 = (*fV0)(2);
  z1 = (*fV1)(2);
  z2 = (*fV2)(2);
  z3 = (*fV3)(2);
  z4 = (*fV4)(2);
  z5 = (*fV5)(2);
  z6 = (*fV6)(2);
  z7 = (*fV7)(2);
  gMes->SetName("ToParam");
  gMes->SetTitle("TSGTRA");
  //Condition (1)
  fDdz = z4;
  eps  = TMath::Abs(z0+fDdz) + TMath::Abs(z1+fDdz) +
    TMath::Abs(z2+fDdz) + TMath::Abs(z3+fDdz);
  if (eps>lim) gMes->M(kill_prog,1,"Condition (1) not ok");
  z0        = -fDdz;
  z1        = -fDdz;
  z2        = -fDdz;
  z3        = -fDdz;
  (*fV0)(2) = -fDdz;
  (*fV1)(2) = -fDdz;
  (*fV2)(2) = -fDdz;
  (*fV3)(2) = -fDdz;
  //Condition (2)
  eps = TMath::Abs(z5-fDdz) + TMath::Abs(z6-fDdz) + TMath::Abs(z7-fDdz);
  if (eps>lim) gMes->M(kill_prog,2,"Condition (2) not ok");
  z5        = fDdz;
  z6        = fDdz;
  z7        = fDdz;
  (*fV5)(2) = fDdz;
  (*fV6)(2) = fDdz;
  (*fV7)(2) = fDdz;
  //Condition (3)
  eps = TMath::Abs(y1-y0);
  if (eps>lim) gMes->M(kill_prog,3,"Condition (3) not ok");
  y1        = y0;
  (*fV1)(1) = y0;
  //Condition (4)
  eps = TMath::Abs(y3-y2);
  if (eps>lim) gMes->M(kill_prog,4,"Condition (4) not ok");
  y3        = y2;
  (*fV3)(1) = y2;
  //Condition (5)
  m1 = (y4-y5)/(x4-x5);
  m2 = (y6-y7)/(x6-x7);
  eps = TMath::Abs(m1-m2);
  if (eps>lim) gMes->M(kill_prog,5,"Condition (5) not ok");
  y7        = y6 - m1*(x6-x7);
  (*fV7)(1) = y7;
  //Condition (6)
  eps = TMath::Abs(x0+x1);
  if (eps>lim) gMes->M(kill_prog,6,"Condition (6) not ok");
  x1        = -x0;
  (*fV1)(0) = x1;
  //Condition (7)
  eps = TMath::Abs(y0+y2);
  if (eps>lim) gMes->M(kill_prog,7,"Condition (7) not ok");
  y2        = -y0;
  (*fV2)(1) = y2;
  //Finding the parameters
  fDbl1    = -x0;
  fDh1     = -y0;
  fDtl1    = (x2-x3)/q2;
  alpha1   = TMath::ATan((x2+x3)/(q4*fDh1));
  twist    = TMath::ATan(-m1);
  stw      = TMath::Sin(twist);
  ctw      = TMath::Cos(twist);
  fDbl2    = -(x4-x5)/(q2*ctw);
  fDtl2    =  (x6-x7)/(q2*ctw);
  a        = (x6+x7)*stw + (y6+y7)*ctw;
  b        = (x4+x5)*stw + (y4+y5)*ctw;
  fDh2     = (a-b)/q4;
  stsf     = (a - q2*fDh2)/(q4*fDdz);
  c        = (-q2*fDh2 + q4*fDdz*stsf)*stw;
  stcf     = (x4 + x5 - c)/(q4*fDdz*ctw);
  st       = TMath::Sqrt(stsf*stsf + stcf*stcf);
  if (st>q1) {
    gMes->M(Error_M,8,"Sin(theta) > 1",st);
    st = q1;
  }
  theta    = TLitPhys::ASin(st);
  if (st>vsmall) {
    sphi  = stsf/st;
    cphi  = stcf/st;
    phi = TMath::ATan2(sphi,cphi);
    if (sphi<q0) phi += deuxpi;
  }
  else {
    st  = q0;
    sphi  = q0;
    cphi  = q1;
  }
  c        = (q2*fDh2 + q4*fDdz*stsf)*stw;
  d        = (x6 + x7 - c)/ctw;
  alpha2   = TMath::ATan((d - q4*fDdz*stcf)/(q4*fDh2));
  //values in degrees
  fDalpha1 = facdeg*alpha1;
  fDalpha2 = facdeg*alpha2;
  fDtheta  = facdeg*theta;
  fDphi    = facdeg*phi;
  fDtwist  = facdeg*twist;
  //giving new values to parent classes
  fDx      = fDtheta;
  fDy      = fDphi;
  fDz      = fDdz;
  fTwist   = fDtwist;     // twisting parameter
  fH1      = fDh1;        // half length in y at low z
  fBl1     = fDbl1;       // half length in x at low z and y low edge
  fTl1     = fDtl1;       // half length in x at low z and y high edge
  fAlpha1  = fDalpha1;    // angle w.r.t. the y axis
  fH2      = fDh2;        // half length in y at high z
  fBl2     = fDbl2;       // half length in x at high z and y low edge
  fTl2     = fDtl2;       // half length in x at high z and y high edge
  fAlpha2  = fDalpha2;    // angle w.r.t. the y axis
}
void TSGTRA::ToPoints() {
  //Calculates the coordinates of the 8 points of the TSGTRA.
  const Double_t deux = 2.0;
  const Double_t pideg  = 180.0;
  Double_t pi;
  Double_t alpha1,alpha2,theta,phi,twist;
  Double_t x,y,z,u,v,a,dsc,dss,tth1,tth2,sta,sphi,cphi,stw,ctw;
  pi = TMath::Pi();
  Double_t facrad;
  facrad = pi/pideg;
  //
  DelPoints();
  alpha1 = facrad*fDalpha1;
  alpha2 = facrad*fDalpha2;
  theta  = facrad*fDtheta;
  phi    = facrad*fDphi;
  twist  = facrad*fDtwist;
  tth1   = TMath::Tan(alpha1);
  tth2   = TMath::Tan(alpha2);
  sta    = TMath::Sin(theta);
  sphi   = TMath::Sin(phi);
  cphi   = TMath::Cos(phi);
  stw    = TMath::Sin(twist);
  ctw    = TMath::Cos(twist);
  //
  x   = -fDbl1;
  y   = -fDh1;
  z   = -fDdz;
  fV0 = new T3Vector(x,y,z);
  x   =  fDbl1;
  fV1 = new T3Vector(x,y,z);
  a   = deux*fDh1*tth1;
  x   =  fDtl1 + a;
  y   =  fDh1;
  fV2 = new T3Vector(x,y,z);
  x   = -fDtl1 + a;
  fV3 = new T3Vector(x,y,z);
  a   = deux*fDdz*sta;
  dsc = a*cphi;
  dss = a*sphi;
  a   = deux*fDh2*tth2;
  u   = -fDbl2 + dsc;
  v   = -fDh2  + dss;
  x   =  u*ctw + v*stw;
  y   = -u*stw + v*ctw;
  z   = fDdz;
  fV4 = new T3Vector(x,y,z);
  u   =  fDbl2 + dsc;
  v   = -fDh2  + dss;
  x   =  u*ctw + v*stw;
  y   = -u*stw + v*ctw;
  fV5 = new T3Vector(x,y,z);
  u   =  fDtl2 + dsc + a;
  v   =  fDh2  + dss;
  x   =  u*ctw + v*stw;
  y   = -u*stw + v*ctw;
  fV6 = new T3Vector(x,y,z);
  u   = -fDtl2 + dsc + a;
  v   =  fDh2  + dss;
  x   =  u*ctw + v*stw;
  y   = -u*stw + v*ctw;
  fV7 = new T3Vector(x,y,z);
}
