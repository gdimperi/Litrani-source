// @(#)root/html:$Id: TSTRAP.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "T3Vector.h"
#include "TMessErr.h"
#include "TFace.h"
#include "TSupplShape.h"
#include "TSTRAP.h"

ClassImp(TSTRAP)
//______________________________________________________________________________
//
//BEGIN_HTML <!--
/* -->
</pre>
<h4><font color="#990000">Recall that all dimensions of shapes are in cm in Litrani and that all angles of shapes are in degrees, not in radians.</font></h4>
<p>TSTRAP inherits from TTRAP and adds supplementary information required for Litrani, in particular the default revetment of all faces.</p>
<p>A TSTRAP is a general trapezoid. See the drawing of a TTRAP in the ROOT documentation of TTRAP. The faces perpendicular to z are trapeze and their centres are not necessarily on a line parallel to the z axis. The standard constructor of this shape has 16 parameters :</p>
<p>
<table border="6" cellpadding="2" cellspacing="3" bgcolor="#cccc99">
<caption>
<h4>16 parameters of the first constructor</h4>
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
<td>fDz</td>
<td>d<sub>z</sub></td>
<td>half-length along the z axis in cm</td>
</tr>
<tr>
<td>Double_t</td>
<td>theta</td>
<td>fDtheta</td>
<td><font face="Symbol">q</font></td>
<td>polar angle of the line joining the centre of the face at -dz to the centre of the one at +dz.. <font color="#9c0031"><b>0 &lt;= </b></font><font face="Symbol" color="#9c0031"><b>q</b></font><b><font color="#9c0031"> &lt; 90&deg;</font></b></td>
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
<p><b>TSTRAP</b> presents the following difficulty : nothing prevents the user to give values to the parameters such that the lateral faces are not plane ! In order to prevent this, checks are done in the constructor of <b>TSTRAP</b> and values are changed in order that faces become plane. More specifically, the 8 points of the shape are calculated using all the input parameters. Points 0 1 2 3 are always taken as given, points 4 5 6 7 are moved the least possible, but such that faces 0 1 2 3 become plane. Then one recalculates the new values of the parameters from the new coordinates of the points and inform the user by a print that the parameters have been changed. The new values of the parameters or of the points can then be obtained by the Get... methods.</p>
<p>Notice that very often one is in the situation where one knows the coordinates of the 8 pts of the shape, but not the values of all the parameters given above. For this kind of situation, we provide a second constructor. See below. We first intend to show how one goes from parameters to coordinates and vice-versa.</p>
<h3>Relation between those parameters and coordinates of the apexes of the shape.</h3>
<p>Let us define (x<sub>k</sub>,y<sub>k</sub>,z<sub>k</sub>) k=0..7 the coordinates of the 8 apexes of a TSTRAP. Suppose that we have <b>a very general shape with 8 apexes</b>, satisfying <b>only</b> to the following conditions :</p>
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
<td>z<sub>0</sub> = z<sub>1</sub> = z<sub>2</sub> = z<sub>3</sub> = -d<sub>z</sub></td>
</tr>
<tr>
<td>(2)</td>
<td>z<sub>4</sub> = z<sub>5</sub> = z<sub>6</sub> = z<sub>7</sub> = +d<sub>z</sub></td>
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
<td>y<sub>5</sub> = y<sub>4</sub></td>
</tr>
<tr>
<td>(6)</td>
<td>y<sub>7</sub> = y<sub>6</sub></td>
</tr>
</table>
</p>
</center>
<p>Then we intend to show that this shape <b>can be represented</b> by a TSTRAP. ( If your shape satisfies conditions (1) to (4), but not (5) nor (6), then your last hope is a TSGTRA ! ). First let us remind that we are always allowed to make a translation along Ox and a translation along Oy, i.e. to choose the position of the center of the coordinate system in the plane Oxy.&nbsp;For that reason, we do not write &quot;x<sub>k</sub>&quot;, but &quot;x<sub>k</sub> + t<sub>x</sub>&quot;, and not &quot;y<sub>k</sub>&quot; but &quot;y<sub>k</sub>+t<sub>y</sub>&quot;, and we keep the possibility to choose t<sub>x</sub> and t<sub>y</sub> in the most clever way until the end of the calculation. We intend to show that starting from the coordinates , satisfying the 6 conditions, we can find back the values of the parameters of the TSTRAP, which proves what we intended to prove.</p>
<center>
<p>
<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
<caption>
<h4>relation between the coordinates and the parameters</h4>
</caption>
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
<td>x<sub>0</sub> + t<sub>x</sub></td>
<td>
<center>
=</center>
</td>
<td>- d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> + h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub> + t<sub>1</sub></td>
<td>(1)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>1</sub> + t<sub>x</sub></td>
<td>
<center>
=</center>
</td>
<td>- d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> + h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub> - t<sub>1</sub></td>
<td>(2)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>2</sub> + t<sub>x</sub></td>
<td>
<center>
=</center>
</td>
<td>- d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> - h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub> - b<sub>1</sub></td>
<td>(3)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>3</sub> + t<sub>x</sub></td>
<td>
<center>
=</center>
</td>
<td>- d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> - h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub> + b<sub>1</sub></td>
<td>(4)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>y<sub>0 </sub>+ t<sub>y</sub></td>
<td>
<center>
=</center>
</td>
<td>h<font size="2"><sub>1</sub></font> - d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(5)</td>
<td><b>condition (3) ok :</b></td>
</tr>
<tr>
<td>y<sub>1 </sub>+ t<sub>y</sub></td>
<td>
<center>
=</center>
</td>
<td>h<font size="2"><sub>1</sub></font> - d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(6)</td>
<td><font color="#9c3031"><b>==&gt; y<sub>1 </sub>= y<sub>0</sub></b></font></td>
</tr>
<tr>
<td>y<sub>2 </sub>+ t<sub>y</sub></td>
<td>
<center>
=</center>
</td>
<td>-h<font size="2"><sub>1</sub></font> - d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(7)</td>
<td><b>condition (4) ok :</b></td>
</tr>
<tr>
<td>y<sub>3 </sub>+ t<sub>y</sub></td>
<td>
<center>
=</center>
</td>
<td>-h<font size="2"><sub>1</sub></font> - d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(8)</td>
<td><font color="#9c3031"><b>==&gt; y<sub>3 </sub>= y<sub>2</sub></b></font></td>
</tr>
<tr>
<td>x<sub>4</sub> + t<sub>x</sub></td>
<td>
<center>
=</center>
</td>
<td>+ d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> + h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub> + t<sub>2</sub></td>
<td>(9)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>5</sub> + t<sub>x</sub></td>
<td>
<center>
=</center>
</td>
<td>+ d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> + h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub> - t<sub>2</sub></td>
<td>(10)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>6</sub> + t<sub>x</sub></td>
<td>
<center>
=</center>
</td>
<td>+ d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> - h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub> - b<sub>2</sub></td>
<td>(11)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>x<sub>7</sub> + t<sub>x</sub></td>
<td>
<center>
=</center>
</td>
<td>+ d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> - h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub> + b<sub>2</sub></td>
<td>(12)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>y<sub>4 </sub>+ t<sub>y</sub></td>
<td>
<center>
=</center>
</td>
<td>h<font size="2"><sub>2</sub></font> + d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(13)</td>
<td><b>condition (5) ok :</b></td>
</tr>
<tr>
<td>y<sub>5 </sub>+ t<sub>y</sub></td>
<td>
<center>
=</center>
</td>
<td>h<font size="2"><sub>2</sub></font> + d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(14)</td>
<td><font color="#9c3031"><b>==&gt; y<sub>5 </sub>= y<sub>4</sub></b></font></td>
</tr>
<tr>
<td>y<sub>6 </sub>+ t<sub>y</sub></td>
<td>
<center>
=</center>
</td>
<td>-h<font size="2"><sub>2</sub></font> + d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(15)</td>
<td><b>condition (6) ok :</b></td>
</tr>
<tr>
<td>y<sub>7 </sub>+ t<sub>y</sub></td>
<td>
<center>
=</center>
</td>
<td>-h<font size="2"><sub>2</sub></font> + d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(16)</td>
<td><font color="#9c3031"><b>==&gt; y<sub>7 </sub>= y<sub>6</sub></b></font></td>
</tr>
</table>
</p>
<p>
<table border="4" cellpadding="4" cellspacing="2" bgcolor="#cccc99">
<caption>
<h4>From equations (1) to (8), we obtain :</h4>
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
<div align="left">
(5) - (7)</div>
</td>
<td>
<div align="right">
y<sub>0 </sub>- y<sub>2</sub></div>
</td>
<td>=</td>
<td>2h<font size="2"><sub>1</sub></font></td>
<td>(17)</td>
<td><font color="#9c3031"><b>==&gt; h</b></font><font size="2" color="#9c3031"><sub><b>1</b></sub></font><font color="#9c3031"><b> is found</b></font></td>
</tr>
<tr>
<td>(1) - (2)</td>
<td>
<div align="right">
x<sub>0</sub> - x<sub>1</sub></div>
</td>
<td>=</td>
<td>2t<sub>1</sub></td>
<td>(18)</td>
<td><font color="#9c3031"><b>==&gt; t<sub>1</sub> is found</b></font></td>
</tr>
<tr>
<td>(3) - (4)</td>
<td>
<div align="right">
x<sub>2</sub> - x<sub>3</sub></div>
</td>
<td>=</td>
<td>-2b<sub>1</sub></td>
<td>(19)</td>
<td><font color="#9c3031"><b>==&gt; b<sub>1</sub> is found</b></font></td>
</tr>
<tr>
<td>(5) + (7)</td>
<td>
<div align="right">
y<sub>0 </sub>+ y<sub>2 </sub>+ 2t<sub>y</sub></div>
</td>
<td>=</td>
<td>- 2d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(20)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>(1) + (2)</td>
<td>
<div align="right">
x<sub>0</sub> + x<sub>1</sub> + 2t<sub>x</sub></div>
</td>
<td>=</td>
<td>- 2d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> + 2h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub></td>
<td>(21)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>(3) + (4)</td>
<td>
<div align="right">
x<sub>2</sub> + x<sub>3</sub> + 2t<sub>x</sub></div>
</td>
<td>=</td>
<td>- 2d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> - 2h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub></td>
<td>(22)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>Definition :</td>
<td>
<div align="right">
X<sub>03</sub></div>
</td>
<td>=</td>
<td>x<sub>0</sub> + x<sub>1</sub> + x<sub>2</sub> + x<sub>3</sub></td>
<td>(23)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>(21) + (22)</td>
<td>
<div align="right">
X<sub>03</sub> + 4t<sub>x</sub></div>
</td>
<td>=</td>
<td>- 4d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font></td>
<td>(24)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>Definition :</td>
<td>
<div align="right">
Y<sub>03</sub></div>
</td>
<td>=</td>
<td>y<sub>0 </sub>+ y<sub>1 </sub>+ y<sub>2 </sub>+ y<sub>3</sub></td>
<td>(25)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>(20) and c3/4</td>
<td>
<div align="right">
Y<sub>03 </sub>+ 4t<sub>y</sub></div>
</td>
<td>=</td>
<td>- 4d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(26)</td>
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
<h4>From relations (9) to (16) we obtain :</h4>
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
<div align="left">
(13) - (15)</div>
</td>
<td>
<div align="right">
y<sub>4 </sub>- y<sub>6</sub></div>
</td>
<td>=</td>
<td>2h<font size="2"><sub>2</sub></font></td>
<td>(27)</td>
<td><font color="#9c3031"><b>==&gt; h</b></font><font size="2" color="#9c3031"><sub><b>2</b></sub></font><font color="#9c3031"><b> is found</b></font></td>
</tr>
<tr>
<td>(9) - (10)</td>
<td>
<div align="right">
x<sub>4</sub> - x<sub>5</sub></div>
</td>
<td>=</td>
<td>2t<sub>2</sub></td>
<td>(28)</td>
<td><font color="#9c3031"><b>==&gt; t<sub>2</sub> is found</b></font></td>
</tr>
<tr>
<td>(11) - (12)</td>
<td>
<div align="right">
x<sub>6</sub> - x<sub>7</sub></div>
</td>
<td>=</td>
<td>-2b<sub>2</sub></td>
<td>(29)</td>
<td><font color="#9c3031"><b>==&gt; b<sub>2</sub> is found</b></font></td>
</tr>
<tr>
<td>(13) + (15)</td>
<td>
<div align="right">
y<sub>4 </sub>+ y<sub>6 </sub>+ 2t<sub>y</sub></div>
</td>
<td>=</td>
<td>+ 2d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(30)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>(9) + (10)</td>
<td>
<div align="right">
x<sub>4</sub> + x<sub>5</sub> + 2t<sub>x</sub></div>
</td>
<td>=</td>
<td>+ 2d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> + 2h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub></td>
<td>(31)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>(11) + (12)</td>
<td>
<div align="right">
x<sub>6</sub> + x<sub>7</sub> + 2t<sub>x</sub></div>
</td>
<td>=</td>
<td>+ 2d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font> - 2h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub></td>
<td>(32)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>Definition :</td>
<td>
<div align="right">
X<sub>47</sub></div>
</td>
<td>=</td>
<td>x<sub>4</sub> + x<sub>5</sub> + x<sub>6</sub> + x<sub>7</sub></td>
<td>(33)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>(30) + (31)</td>
<td>
<div align="right">
X<sub>47</sub> + 4t<sub>x</sub></div>
</td>
<td>=</td>
<td>+ 4d<sub>z</sub> tg<font face="Symbol">q</font> cos<font face="Symbol">f</font></td>
<td>(34)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>Definition :</td>
<td>
<div align="right">
Y<sub>47</sub></div>
</td>
<td>=</td>
<td>y<sub>4 </sub>+ y<sub>5 </sub>+ y<sub>6 </sub>+ y<sub>7</sub></td>
<td>(35)</td>
<td>
<center>
-</center>
</td>
</tr>
<tr>
<td>(29) +c3/4</td>
<td>
<div align="right">
Y<sub>47 </sub>+ 4t<sub>y</sub></div>
</td>
<td>=</td>
<td>+ 4d<sub>z</sub> tg<font face="Symbol">q</font> sin<font face="Symbol">f</font></td>
<td>(36)</td>
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
<h4>We get t<sub>x</sub> and t<sub>y</sub> and find then all remaining parameters :</h4>
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
<div align="left">
(24) + (34)</div>
</td>
<td>
<div align="right">
t<sub>x</sub></div>
</td>
<td>=</td>
<td>- ( X<sub>03</sub> + X<sub>47 </sub>) /&nbsp;8</td>
<td>(36)</td>
<td><font color="#9c3031"><b>==&gt; t<sub>x</sub> is found</b></font></td>
</tr>
<tr>
<td>
<div align="left">
(26) + (36)</div>
</td>
<td>
<div align="right">
t<sub>y</sub></div>
</td>
<td>=</td>
<td>- ( Y<sub>03 </sub>+ Y<sub>47 </sub>) /&nbsp;8</td>
<td>(37)</td>
<td><font color="#9c3031"><b>==&gt; t<sub>y</sub> is found</b></font>-</td>
</tr>
<tr>
<td>(24) and (26)</td>
<td>( Y<sub>03 </sub>+ 4t<sub>y</sub> ) / ( X<sub>03</sub> + 4t<sub>x</sub> )</td>
<td>=</td>
<td>tg<font face="Symbol">f</font></td>
<td>(38)</td>
<td><font color="#9c3031"><b>==&gt; </b></font><font face="Symbol" color="#9c3031"><b>f</b></font><b><font color="#9c3031"> is found</font></b></td>
</tr>
<tr>
<td>(34) and (36)</td>
<td>( Y<sub>47 </sub>+ 4t<sub>y</sub> ) / ( X<sub>47</sub> + 4t<sub>x</sub> )</td>
<td>=</td>
<td>( Y<sub>03 </sub>+ 4t<sub>y</sub> ) / ( X<sub>03</sub> + 4t<sub>x</sub> )</td>
<td>(39)</td>
<td>
<center>
<font color="#9c0031"><b>ok with (36) &amp; (37)</b></font></center>
</td>
</tr>
<tr>
<td>(24)</td>
<td>
<div align="right">
tg<font face="Symbol">q</font></div>
</td>
<td>=</td>
<td>
<div align="left">
- ( X<sub>03</sub> + 4t<sub>x</sub> ) / ( 4d<sub>z</sub> cos<font face="Symbol">f</font> )</div>
</td>
<td>(40)</td>
<td><font color="#9c3031"><b>==&gt; </b></font><font face="Symbol" color="#9c3031"><b>q</b></font><b><font color="#9c3031"> is found</font></b></td>
</tr>
<tr>
<td>(21) - (22)</td>
<td>
<div align="right">
4 h<font size="2"><sub>1</sub></font> tg<font face="Symbol">a</font><sub>1</sub></div>
</td>
<td>=</td>
<td>x<sub>0</sub> + x<sub>1</sub> - x<sub>2</sub> - x<sub>3</sub></td>
<td>(41)</td>
<td><font color="#9c3031"><b>==&gt; </b></font><font face="Symbol" color="#9c3031"><b>a</b></font><b><font color="#9c3031"><sub>1</sub> is found</font></b></td>
</tr>
<tr>
<td>(31) - (32)</td>
<td>
<div align="right">
4 h<font size="2"><sub>2</sub></font> tg<font face="Symbol">a</font><sub>2</sub></div>
</td>
<td>=</td>
<td>x<sub>4</sub> + x<sub>5</sub> - x<sub>6</sub> - x<sub>7</sub></td>
<td>(42)</td>
<td><font color="#9c3031"><b>==&gt; </b></font><font face="Symbol" color="#9c3031"><b>a</b></font><b><font color="#9c3031"><sub>2</sub> is found</font></b></td>
</tr>
</table>
</p>
</center>
<p>We have found back all the parameters starting from coordinates satisfying conditions (1) to (6). It proves that all shapes satisfying conditions (1) to (6) can be represented by a <b>TSTRAP</b>. Due to this fact, we are able to offer a second constructor, whose arguments are the coordinates of the points of the apexes instead of the usual argument of a <b>TSTRAP</b> used in the first constructor.</p>
<p>Notice this important point concerning the second constructor : unless the user has chosen his (x<sub>k</sub>,y<sub>k</sub>) values in such a way that equations (36) and (37) give t<sub>x</sub> = t<sub>y</sub> = 0 , i.e. <b>such that </b></p>
<ul>
<li><b><code>x<sub>0</sub> + x<sub>1</sub> + x<sub>2</sub> + x<sub>3</sub> + x<sub>4</sub> + x<sub>5</sub> + x<sub>6</sub> + x<sub>7</sub> = 0</code></b>
<li><b><code>y<sub>0 </sub>+ y<sub>1 </sub>+ y<sub>2 </sub>+ y<sub>3</sub> + y<sub>4 </sub>+ y<sub>5 </sub>+ y<sub>6 </sub>+ y<sub>7</sub> = 0</code></b>
</ul>
<p>, the (x<sub>k</sub>,y<sub>k</sub>) values proposed by the user <b>will be shifted by t<sub>x</sub> and t<sub>y</sub></b>. t<sub>x</sub> and t<sub>y</sub> are returned by the second constructor so that the user is informed about this translation and can compensate for it in the call to TSNode.&nbsp;For instance, if he wants that the shape be positionned exactly at the given points, he has only to call TSNode with the translation vector (-t<sub>x</sub>,-t<sub>y</sub>,0.0).</p>
<p>Notice also that y<sub>1</sub>, y<sub>3</sub>,y<sub>5</sub>,y<sub>7</sub> are not given because they are fixed by the conditions (3) - (6).</p>
<center>
<p>
<table border="5" cellpadding="1" cellspacing="3" bgcolor="#cccc99">
<caption>
<h4>20 parameters of the second constructor</h4>
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
<h4>class name</h4>
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
<td></td>
<td></td>
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
<td>x0</td>
<td>
<center>
-</center>
</td>
<td>x<sub>0</sub></td>
<td>x coordinate of point (0) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>x1</td>
<td>
<center>
-</center>
</td>
<td>x<sub>1</sub></td>
<td>x coordinate of point (1) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>x2</td>
<td>
<center>
-</center>
</td>
<td>x<sub>2</sub></td>
<td>x coordinate of point (2) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>x3</td>
<td>
<center>
-</center>
</td>
<td>x<sub>3</sub></td>
<td>x coordinate of point (3) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>x4</td>
<td>
<center>
-</center>
</td>
<td>x<sub>4</sub></td>
<td>x coordinate of point (4) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>x5</td>
<td>
<center>
-</center>
</td>
<td>x<sub>5</sub></td>
<td>x coordinate of point (5) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>x6</td>
<td>
<center>
-</center>
</td>
<td>x<sub>6</sub></td>
<td>x coordinate of point (6) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>x7</td>
<td>
<center>
-</center>
</td>
<td>x<sub>7</sub></td>
<td>x coordinate of point (7) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>y0</td>
<td>
<center>
-</center>
</td>
<td>y<sub>0</sub></td>
<td>y coordinate of point (0) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>y2</td>
<td>
<center>
-</center>
</td>
<td>y<sub>2</sub></td>
<td>y coordinate of point (2) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>y4</td>
<td>
<center>
-</center>
</td>
<td>y<sub>4</sub></td>
<td>y coordinate of point (4) wanted by the user.</td>
</tr>
<tr>
<td>Double_t</td>
<td>y6</td>
<td>
<center>
-</center>
</td>
<td>y<sub>6</sub></td>
<td>y coordinate of point (6) wanted by the user.</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>tx</td>
<td>
<center>
-</center>
</td>
<td>t<sub>x</sub></td>
<td>translation that will be affected to all x<sub>k</sub> values</td>
</tr>
<tr>
<td>Double_t &amp;</td>
<td>ty</td>
<td>
<center>
-</center>
</td>
<td>t<sub>y</sub></td>
<td>translation that will be affected to all y<sub>k</sub> values</td>
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
<p>Finally a <b>not necessary</b>, but <b>convenient</b> condition. If it disturbs you to have negative values for the parameters t<sub>1</sub> , b<sub>1</sub> , h<sub>1 </sub>, t<sub>2</sub> , b<sub>2</sub>, h<sub>2</sub> ( on a mathematical point of view, it does not matter, on a geometrical point of view, it does ), number your points in such a way that :</p>
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
<td>x<sub>0</sub>&nbsp;&gt; 0 or x<sub>0</sub> big</td>
<td>y<sub>0 </sub>&gt; 0 or y<sub>0</sub> big</td>
<td>
<center>
(4)</center>
</td>
<td>x<sub>4</sub>&nbsp;&gt; 0 or x<sub>4</sub> big</td>
<td>y<sub>4 </sub>&gt; 0 or y<sub>4</sub> big</td>
</tr>
<tr>
<td>
<center>
(1)</center>
</td>
<td>x<sub>1</sub>&nbsp;&lt; 0 or x<sub>1</sub> small</td>
<td>y<sub>1 </sub>&gt; 0 or y<sub>1</sub> big</td>
<td>
<center>
(5)</center>
</td>
<td>x<sub>5</sub>&nbsp;&lt; 0 or x<sub>5</sub> small</td>
<td>y<sub>5 </sub>&gt; 0 or y<sub>5</sub> big</td>
</tr>
<tr>
<td>
<center>
(2)</center>
</td>
<td>x<sub>2</sub>&nbsp;&lt; 0 or x<sub>2</sub> small</td>
<td>y<sub>2 </sub>&lt; 0 or y<sub>2</sub> small</td>
<td>
<center>
(6)</center>
</td>
<td>x<sub>6</sub>&nbsp;&lt; 0 or x<sub>6</sub> small</td>
<td>y<sub>6 </sub>&lt; 0 or y<sub>6</sub> small</td>
</tr>
<tr>
<td>
<center>
(3)</center>
</td>
<td>x<sub>3</sub>&nbsp;&gt; 0 or x<sub>3</sub> big</td>
<td>y<sub>3 </sub>&lt; 0 or y<sub>3</sub> small</td>
<td>
<center>
(7)</center>
</td>
<td>x<sub>7</sub>&nbsp;&gt; 0 or x<sub>7</sub> big</td>
<td>y<sub>7 </sub>&lt; 0 or y<sub>7</sub> small</td>
</tr>
</table>
</p>
</center>
<div align="left">
<p>Look at the macros xanibeam8.C and xanibeam9.C to have examples of the use of <b>TSTRAP</b>.</p>
</div>
<p>The 6 faces of the TSTRAP are built from these points in the following way :</p>
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
<p>For each shape of Litrani {tstrap,tstrd1,tstrd2,tspara,tstrap,tsgtra,ts8pts,tscyl,tstube,tscone} there is an associated &quot;friend&quot; class <b>TSupplShape</b>, which contains everything which is common to all kind of shapes. This associated class may be reached from <b>TSTRAP</b> (or from any other shape) like this (supposing that ptrap is a pointer on a <b>TSTRAP</b>) :</p>
<center>
<p><font color="#9c3031"><code>TSupplShape *suppl = ptrap-&gt;fSuppl;</code></font></p>
</center>
<p>All shapes in Litrani may at the same time be a detector of photon. For instance, a shape may represent a phototube or an APD. See the description how to define a shape as a detector in the presentation of the <b>TSupplShape</b> class. If FromOther is true, it means that the material of the shape and the optical axis are taken from an other shape. If this is the case, it means that in case of transition between this shape and the other shape having same material and same optical axis, the photon simply continues from one shape to the other, without any transition calculation.
<pre>
<!--*/
// -->END_HTML
//
TSTRAP::TSTRAP(const char *name,const char *title,const char *namemat,
  const char *namerev,Double_t dz,Double_t theta,Double_t phi,Double_t h1,
  Double_t bl1,Double_t tl1,Double_t alpha1,Double_t h2,Double_t bl2,
  Double_t tl2,Double_t alpha2,Bool_t FromOther):TTRAP(name,title,namemat,
  Float_t(dz), Float_t(theta), Float_t(phi),Float_t(h1), Float_t(bl1),
  Float_t(tl1), Float_t(alpha1),Float_t(h2), Float_t(bl2), Float_t(tl2),
  Float_t(alpha2)) {
  //standard constructor for a TSTRAP
  //BEGIN_HTML <!--
  /* -->
  </pre>
  <table border="6" cellpadding="2" cellspacing="3" bgcolor="#cccc99">
  <caption>
  <h4>16 parameters of the first constructor</h4>
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
  <td>fDz</td>
  <td>d<sub>z</sub></td>
  <td>half-length along the z axis in cm</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>theta</td>
  <td>fDtheta</td>
  <td><font face="Symbol">q</font></td>
  <td>polar angle of the line joining the centre of the face at -dz to the centre of the one at +dz.. <font color="#9c0031"><b>0 &lt;= </b></font><font face="Symbol" color="#9c0031"><b>q</b></font><b><font color="#9c0031"> &lt; 90&deg;</font></b></td>
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
  <pre>
  <!--*/
  // -->END_HTML
  //
  const Double_t zero  = 0.0;
  const Double_t small = 1.0e-4;
  const Double_t pis2d   = 90.0;
  const Double_t deuxpid = 360.0;
  Double_t tx,ty,planarity;
  TString nameshape;
  TFace *face;
  InitP();
  fDdz     = dz;
  gMes->SetName("TSTRAP");
  gMes->SetTitle("TSTRAP");
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
    fSuppl = new TSupplShape(name,title,this,tstrap,nameshape,namerev);
  }
  else {
    fSuppl = new TSupplShape(name,title,this,tstrap,namemat,namerev);
  }
  fSuppl->fGCenter.Set(zero,zero,zero);
  fSuppl->fNbSubPoints = 0;
  fSuppl->fNbPoints = 8;
  fSuppl->fTotalNbPoints = fSuppl->fNbPoints;
  fSuppl->fPoints = new TObjArray();
  //
  ToPoints();
  planarity = MakePlanar();
  ToParam(tx,ty);
  if (planarity>small) {
    gMes->SetName("TSTRAP");
    gMes->SetTitle("TSTRAP");
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
TSTRAP::TSTRAP(const char *name,const char *title,const char *namemat,
  const char *namerev,Double_t dz,Double_t x0,Double_t x1,Double_t x2,Double_t x3,
  Double_t x4,Double_t x5,Double_t x6,Double_t x7,Double_t y0,Double_t y2,
  Double_t y4,Double_t y6,Double_t &tx,Double_t &ty,Bool_t FromOther):TTRAP(name,
  title,namemat,1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0,1.0, 0.0) {
  //Second constructor for a TSTRAP, starting from the coordinates of the
  //apexes.
  //BEGIN_HTML <!--
  /* -->
  </pre>
  <center>
  <p>
  <table border="5" cellpadding="1" cellspacing="3">
  <caption>Parameters of the second constructor</caption>
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
  <h4>class name</h4>
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
  <td></td>
  <td>name of the shape</td>
  </tr>
  <tr>
  <td>const char</td>
  <td>*title</td>
  <td>fTitle</td>
  <td></td>
  <td>shape's title</td>
  </tr>
  <tr>
  <td>const char</td>
  <td>*namemat</td>
  <td></td>
  <td></td>
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
  <td></td>
  <td></td>
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
  <td>x0</td>
  <td></td>
  <td>x<sub>0</sub></td>
  <td>x coordinate of point (0) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x1</td>
  <td></td>
  <td>x<sub>1</sub></td>
  <td>x coordinate of point (1) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x2</td>
  <td></td>
  <td>x<sub>2</sub></td>
  <td>x coordinate of point (2) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x3</td>
  <td></td>
  <td>x<sub>3</sub></td>
  <td>x coordinate of point (3) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x4</td>
  <td></td>
  <td>x<sub>4</sub></td>
  <td>x coordinate of point (4) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x5</td>
  <td></td>
  <td>x<sub>5</sub></td>
  <td>x coordinate of point (5) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x6</td>
  <td></td>
  <td>x<sub>6</sub></td>
  <td>x coordinate of point (6) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>x7</td>
  <td></td>
  <td>x<sub>7</sub></td>
  <td>x coordinate of point (7) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>y0</td>
  <td></td>
  <td>y<sub>0</sub></td>
  <td>y coordinate of point (0) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>y2</td>
  <td></td>
  <td>y<sub>2</sub></td>
  <td>y coordinate of point (2) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>y4</td>
  <td></td>
  <td>y<sub>4</sub></td>
  <td>y coordinate of point (4) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t</td>
  <td>y6</td>
  <td></td>
  <td>y<sub>6</sub></td>
  <td>y coordinate of point (6) wanted by the user.</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>tx</td>
  <td></td>
  <td>t<sub>x</sub></td>
  <td>translation that will be affected to all x<sub>k</sub> values because (35) and (41)</td>
  </tr>
  <tr>
  <td>Double_t &amp;</td>
  <td>ty</td>
  <td></td>
  <td>t<sub>y</sub></td>
  <td>translation that will be affected to all y<sub>k</sub> values because (35) and (41)</td>
  </tr>
  <tr>
  <td>Bool_t</td>
  <td>FromOther</td>
  <td></td>
  <td></td>
  <td>true if material and optical axis have to be taken from an other shape.</td>
  </tr>
  </table>
  </p>
  </center>
  <pre>
  <!--*/
  // -->END_HTML
  //
  //  Notice that if your points do not satisfy :
  //
  //  (1) - x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 = 0
  //  (2) - y0 + y1 + y2 + y3 + y4 + y5 + y6 + y7 = 0
  //
  //or if they satify (1) and (2), BUT have been moved because the lateral
  //faces where not plane and points (4)/(5)/(6)/(7) had to be moved in order
  //that lateral faces be plane, then conditions (1) and (2) are IMPOSED to
  //your points by a translation of tx along Ox and ty along Oy. The values of
  //tx and ty are returned, so that you can compensate for this translation
  // in the call to TSNode, if you want.
  const Double_t zero = 0.0;
  const Double_t small  = 1.0e-4;
  Double_t planarity;
  Double_t y1,y3,y5,y7;
  TString nameshape;
  TFace *face;
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
  y1 = y0;
  y3 = y2;
  y5 = y4;
  y7 = y6;
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
    gMes->SetName("TSTRAP");
    gMes->SetTitle("TSTRAP");
    gMes->M(Warning_M,1,"Coordinate changed in order to have plane faces");
    PrintPts();
  }
  ToParam(tx,ty);
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
TSTRAP::TSTRAP(const char *name,const char *title,const char *namemat,
  const char *namerev,Double_t dz,Double_t AF,Double_t BF,Double_t CF,Double_t AR,
  Double_t BR,Double_t CR,Bool_t FromOther):TTRAP(name,title,namemat,1.0, 0.0,
  0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0,1.0, 0.0) {
  //  Recall that a general TSTRAP satisfies to the following conditions:
  //
  //    y1 = y0
  //    y3 = y2
  //    y5 = y4
  //    y7 = y6
  //
  //  This constructor is for a SPECIAL TSTRAP, satisfying IN ADDITION to the following
  //conditions :
  //
  //    x3 = x0
  //    x4 = x0
  //    x7 = x4
  //    y4 = y0
  //
  //  Use it only if your TSTRAP satisfies to these additionnal conditions.
  //  It is also assumed by this constructor that the TSTRAP is centered, ie
  //
  //   x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 = 0
  //   y0 + y1 + y2 + y3 + y4 + y5 + y6 + y7 = 0
  //
  //  Under these conditions, if we define :
  //
  //    AF = x2 - x3          AR = x6 - x7
  //    BF = y3 - y0          BR = y7 - y4
  //    CF = x1 - x0          CR = x5 - x4
  //
  //  Then, the special TSTRAP is sufficiently described by the 6 parameters AF, BF, CF,
  //AR, BR, CR. For examples, all crystals of the CMS experiment are of this special
  //TSTRAP type. Notice that if you give such values for AF, BF, CF, AR, BR, CR that the
  //faces are not plane, the values for AR, BR, CR effectively used may be changed
  //a little bit. You will be informed of that by a warning.
  //
  //  The arguments are the following
  //
  //   name               : name of this shape
  //   title              : title of this shape
  //   namemat            : material of this shape
  //   namerev            : default revetment for this face
  //   dz                 : half height
  //   AF,BF,CF,AR,BR,CR  : the parameters explained above
  //   FromOther          : true if material and optical axis have to be taken
  //                        from an other shape.
  //
  const Int_t    ntimes = 100;
  const Double_t   zero = 0.0;
  const Double_t     un = 1.0;
  const Double_t small  = 1.0e-4;
  const Double_t small2 = 1.0e-8;
  Int_t itimes;
  Double_t planarity;
  Double_t x0,x1,x2,x3,x4,x5,x6,x7;
  Double_t y0,y1,y2,y3,y4,y5,y6,y7;
  Double_t tx,ty;
  TString nameshape;
  TFace *face;
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
  itimes = 0;
  planarity = un;
  while ((itimes<ntimes) && (planarity>small2)) {
    itimes++;
    x0  = -(AF + CF + CR + AR)/8.0;
    x1  = x0 + CF;
    x3  = x0;
    x2  = x3 + AF;
    x4  = x0;
    x5  = x4 + CR;
    x7  = x4;
    x6  = x7 + AR;
    y0  = -(BF + BR)/4.0;
    y1  = y0;
    y3  = y0 + BF;
    y2  = y3;
    y4  = y0;
    y5  = y4;
    y7  = y4 + BR;
    y6  = y7;
    fV0 = new T3Vector(x0,y0,-dz);
    fV1 = new T3Vector(x1,y1,-dz);
    fV2 = new T3Vector(x2,y2,-dz);
    fV3 = new T3Vector(x3,y3,-dz);
    fV4 = new T3Vector(x4,y4,dz);
    fV5 = new T3Vector(x5,y5,dz);
    fV6 = new T3Vector(x6,y6,dz);
    fV7 = new T3Vector(x7,y7,dz);
    planarity = MakePlanar();
    x0  = (*fV0)(0);
    y0  = (*fV0)(1);
    x1  = (*fV1)(0);
    y1  = (*fV1)(1);
    x2  = (*fV2)(0);
    y2  = (*fV2)(1);
    x3  = (*fV3)(0);
    y3  = (*fV3)(1);
    x4  = (*fV4)(0);
    y4  = (*fV4)(1);
    x5  = (*fV5)(0);
    y5  = (*fV5)(1);
    x6  = (*fV6)(0);
    y6  = (*fV6)(1);
    x7  = (*fV7)(0);
    y7  = (*fV7)(1);
    AF  = x2 - x3;
    BF  = y3 - y0;
    CF  = x1 - x0;
    AR  = x6 - x7;
    BR  = y7 - y4;
    CR  = x5 - x4;
    if (planarity>small) {
      gMes->SetName("TSTRAP");
      gMes->SetTitle("TSTRAP");
      gMes->M(Warning_M,1,"Parameters changed in order to have plane faces");
      cout << "AF : " << AF << "   BF : " << BF << "   CF : " << CF << endl;
      cout << "AR : " << AR << "   BR : " << BR << "   CR : " << CR << endl;
    }
  }
  if (itimes>=ntimes) {
    gMes->SetName("TSTRAP");
    gMes->SetTitle("TSTRAP");
    gMes->M(kill_prog,2,"Impossible to get plane faces");
  }
  ToParam(tx,ty);
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
TSTRAP::TSTRAP(const char *name,const char *title,const char *namemat,
  const char *namerev,Short_t crystaltype,Double_t iso_dx,Bool_t FromOther):TTRAP(name,
  title,namemat,1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0,1.0, 0.0) {
  //
  //  USE THIS CONSTRUCTOR ONLY FOR CMS CRYSTALS !
  //
  //  Recall that a general TSTRAP satisfies to the following conditions:
  //
  //    y1 = y0
  //    y3 = y2
  //    y5 = y4
  //    y7 = y6
  //
  //  This constructor is for a SPECIAL TSTRAP, satisfying IN ADDITION to the following
  //conditions :
  //
  //    x3 = x0
  //    x4 = x0
  //    x7 = x4
  //    y4 = y0
  //
  //  and also satisfying the condition of being a CMS crystal, whose type, crystaltype,
  //is in the range [1 .. 17]. Use this constructor only for CMS crystals! Values for
  //AF,BF,CF,AR,BR,CR as a function of crystal type are given by the method GetCMSdim().
  //  It is also assumed by this constructor that the TSTRAP is centered, ie
  //
  //   x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 = 0
  //   y0 + y1 + y2 + y3 + y4 + y5 + y6 + y7 = 0
  //
  //  Under these conditions, if we define :
  //
  //    AF = x2 - x3          AR = x6 - x7
  //    BF = y3 - y0          BR = y7 - y4
  //    CF = x1 - x0          CR = x5 - x4
  //
  //  Then, the special TSTRAP is sufficiently described by the 6 parameters AF, BF, CF,
  //AR, BR, CR. For examples, all crystals of the CMS experiment are of this special
  //TSTRAP type. Notice that if you give such values for AF, BF, CF, AR, BR, CR that the
  //faces are not plane, the values for AR, BR, CR effectively used may be changed
  //a little bit. You will be informed of that by a warning.
  //
  //  The arguments are the following
  //
  //   name         : name of this shape
  //   title        : title of this shape
  //   namemat      : material of this shape
  //   namerev      : default revetment for this face
  //   crystaltype  : type of CMS crystal
  //   iso_dx       : If iso_dx is not 0, the length iso_dx is subtracted from all crystal
  //                   dimensions in the Ox direction. This is usefule in the case one wants
  //                   to represent the crystal as 2 (not 1) TSTRAP, the second TSTRAP being
  //                   a small sheet of ISOTROPIC material in contact with face 1 of the
  //                   first TSTRAP. Face 1 of the CMS crystal is the one which is depolished,
  //                   and Litrani is not able to simulate depolish when the material of the
  //                   shape is anisotropic. It is why this small sheet is useful!
  //   FromOther   : true if material and optical axis have to be taken
  //                        from an other shape.
  //
  const Int_t    ntimes = 100;
  const Double_t   zero = 0.0;
  const Double_t     un = 1.0;
  const Double_t small  = 1.0e-3;
  const Double_t small2 = 1.0e-8;
  const Double_t     dz = 11.5;   //half length of barrel crystal
  const Double_t   dzEC = 11.0;   //half length of end-cap crystal
  Int_t itimes;
  Double_t planarity;
  Double_t x0,x1,x2,x3,x4,x5,x6,x7;
  Double_t y0,y1,y2,y3,y4,y5,y6,y7;
  Double_t z0,z1,z2,z3,z4,z5,z6,z7;
  Double_t AF,BF,CF,AR,BR,CR;
  Double_t ferule_BF;   //not used, but useful from CINT
  Double_t tx,ty;
  TString nameshape;
  TFace *face;
  InitP();
  if (FromOther) {
    nameshape = namemat;
    fSuppl = new TSupplShape(name,title,this,tstrap,nameshape,namerev);
  }
  else {
    fSuppl = new TSupplShape(name,title,this,tstrap,namemat,namerev);
  }
  fSuppl->fGCenter.Set(zero,zero,zero);
  fSuppl->fNbSubPoints = 0;
  fSuppl->fNbPoints = 8;
  fSuppl->fTotalNbPoints = fSuppl->fNbPoints;
  fSuppl->fPoints = new TObjArray();
  fDdz = dz;
  if (crystaltype == 18) fDdz = dzEC;
  GetCMSdim(crystaltype,ferule_BF,AF,BF,CF,AR,BR,CR,iso_dx);
  itimes = 0;
  planarity = un;
  while ((itimes<ntimes) && (planarity>small2)) {
    itimes++;
    x0  = -(AF + CF + CR + AR)/8.0;
    x1  = x0 + CF;
    x3  = x0;
    x2  = x3 + AF;
    x4  = x0;
    x5  = x4 + CR;
    x7  = x4;
    x6  = x7 + AR;
    y0  = -(BF + BR)/4.0;
    y1  = y0;
    y3  = y0 + BF;
    y2  = y3;
    y4  = y0;
    y5  = y4;
    y7  = y4 + BR;
    y6  = y7;
    if (itimes==1) {
      fV0 = new T3Vector(x0,y0,-fDdz);
      fV1 = new T3Vector(x1,y1,-fDdz);
      fV2 = new T3Vector(x2,y2,-fDdz);
      fV3 = new T3Vector(x3,y3,-fDdz);
      fV4 = new T3Vector(x4,y4,fDdz);
      fV5 = new T3Vector(x5,y5,fDdz);
      fV6 = new T3Vector(x6,y6,fDdz);
      fV7 = new T3Vector(x7,y7,fDdz);
    }
    else {
      fV0->Set(x0,y0,-fDdz);
      fV1->Set(x1,y1,-fDdz);
      fV2->Set(x2,y2,-fDdz);
      fV3->Set(x3,y3,-fDdz);
      fV4->Set(x4,y4,fDdz);
      fV5->Set(x5,y5,fDdz);
      fV6->Set(x6,y6,fDdz);
      fV7->Set(x7,y7,fDdz);
    }
    planarity = MakePlanar();
    fV0->Get(x0,y0,z0);
    fV1->Get(x1,y1,z1);
    fV2->Get(x2,y2,z2);
    fV3->Get(x3,y3,z3);
    fV4->Get(x4,y4,z4);
    fV5->Get(x5,y5,z5);
    fV6->Get(x6,y6,z6);
    fV7->Get(x7,y7,z7);
    AF  = x2 - x3;
    BF  = y3 - y0;
    CF  = x1 - x0;
    AR  = x6 - x7;
    BR  = y7 - y4;
    CR  = x5 - x4;
    if (planarity>small) {
      gMes->SetName("TSTRAP");
      gMes->SetTitle("TSTRAP");
      gMes->M(Warning_M,1,"Parameters changed in order to have plane faces");
      cout << "AF : " << AF << "   BF : " << BF << "   CF : " << CF << endl;
      cout << "AR : " << AR << "   BR : " << BR << "   CR : " << CR << endl;
    }
  }
  if (itimes>=ntimes) {
    gMes->SetName("TSTRAP");
    gMes->SetTitle("TSTRAP");
    gMes->M(kill_prog,2,"Impossible to get plane faces");
  }
  ToParam(tx,ty);
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
TSTRAP::~TSTRAP() {
  //Destructor. Notice that memory for vectors fV0..fV7 is released by the
  //call to delete fSuppl. No need to delete fVn here.
  if (fSuppl) {
    delete fSuppl;
    fSuppl = 0;
  }
}
void TSTRAP::DelPoints() {
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
void TSTRAP::GetCMSdim(Short_t crystaltype,Double_t &ferule_BF,Double_t &AF,
  Double_t &BF,Double_t &CF,Double_t &AR,Double_t &BR,Double_t &CR,Double_t iso_dx) const {
  //  This method is only for use with CMS crystals! It gives the 6 dimensions AF,BF,CF,
  //AR,BR,CR as a function of the CMS crystal type 1-17. It also gives the BF dimension
  //of the "ferule" on the small face of the crystal.
  //  Crystal of type 18 is the end cap crystal.
  //  If iso_dx is not 0, the length iso_dx is subtracted from all crystal dimensions
  //in the Ox direction. This is usefule in the case one wants to represent the crystal
  //as 2 (not 1) TSTRAP, the second TSTRAP being a small sheet of ISOTROPIC material
  //in contact with face 1 of the first TSTRAP. Face 1 of the CMS crystal is the one
  //which is depolished, and Litrani is not able to simulate depolish when the material
  //of the shape is anisotropic. It is why this small sheet is useful!
  //
  AF   = 2.183;
  switch (crystaltype) {
  case 1:
    BF  = 2.359; //side back,  small face |_ to OZ
    CF  = 2.185; //side right, small face |_ to OZ
    AR  = 2.584; //side left,  large face |_ to OZ
    BR  = 2.548; //side back,  large face |_ to OZ
    CR  = 2.586; //side right, large face |_ to OZ
    ferule_BF = 2.366;//ferule of type 5
    break;
  case 2:
    BF  = 2.222; //side back,  small face |_ to OZ
    CF  = 2.187; //side right, small face |_ to OZ
    AR  = 2.581; //side left,  large face |_ to OZ
    BR  = 2.622; //side back,  large face |_ to OZ
    CR  = 2.586; //side right, large face |_ to OZ
    ferule_BF = 2.228;//ferule of type 1
    break;
  case 3:
    BF  = 2.234; //side back,  small face |_ to OZ
    CF  = 2.191; //side right, small face |_ to OZ
    AR  = 2.575; //side left,  large face |_ to OZ
    BR  = 2.628; //side back,  large face |_ to OZ
    CR  = 2.584; //side right, large face |_ to OZ
    ferule_BF = 2.228;//ferule of type 1
    break;
  case 4:
    BF  = 2.247; //side back,  small face |_ to OZ
    CF  = 2.194; //side right, small face |_ to OZ
    AR  = 2.567; //side left,  large face |_ to OZ
    BR  = 2.632; //side back,  large face |_ to OZ
    CR  = 2.580; //side right, large face |_ to OZ
    ferule_BF = 2.228;//ferule of type 1
    break;
  case 5:
    BF  = 2.261; //side back,  small face |_ to OZ
    CF  = 2.197; //side right, small face |_ to OZ
    AR  = 2.556; //side left,  large face |_ to OZ
    BR  = 2.634; //side back,  large face |_ to OZ
    CR  = 2.572; //side right, large face |_ to OZ
    ferule_BF = 2.261;//ferule of type 2
    break;
  case 6:
    BF  = 2.260; //side back,  small face |_ to OZ
    CF  = 2.200; //side right, small face |_ to OZ
    AR  = 2.543; //side left,  large face |_ to OZ
    BR  = 2.618; //side back,  large face |_ to OZ
    CR  = 2.563; //side right, large face |_ to OZ
    ferule_BF = 2.261;//ferule of type 2
    break;
  case 7:
    BF  = 2.255; //side back,  small face |_ to OZ
    CF  = 2.203; //side right, small face |_ to OZ
    AR  = 2.529; //side left,  large face |_ to OZ
    BR  = 2.596; //side back,  large face |_ to OZ
    CR  = 2.552; //side right, large face |_ to OZ
    ferule_BF = 2.261;//ferule of type 2
    break;
  case 8:
    BF  = 2.267; //side back,  small face |_ to OZ
    CF  = 2.205; //side right, small face |_ to OZ
    AR  = 2.514; //side left,  large face |_ to OZ
    BR  = 2.592; //side back,  large face |_ to OZ
    CR  = 2.539; //side right, large face |_ to OZ
    ferule_BF = 2.261;//ferule of type 2
    break;
  case 9:
    BF  = 2.282; //side back,  small face |_ to OZ
    CF  = 2.208; //side right, small face |_ to OZ
    AR  = 2.498; //side left,  large face |_ to OZ
    BR  = 2.590; //side back,  large face |_ to OZ
    CR  = 2.526; //side right, large face |_ to OZ
    ferule_BF = 2.261;//ferule of type 2
    break;
  case 10:
    BF  = 2.308; //side back,  small face |_ to OZ
    CF  = 2.210; //side right, small face |_ to OZ
    AR  = 2.482; //side left,  large face |_ to OZ
    BR  = 2.600; //side back,  large face |_ to OZ
    CR  = 2.512; //side right, large face |_ to OZ
    ferule_BF = 2.315;//ferule of type 3
    break;
  case 11:
    BF  = 2.314; //side back,  small face |_ to OZ
    CF  = 2.212; //side right, small face |_ to OZ
    AR  = 2.465; //side left,  large face |_ to OZ
    BR  = 2.589; //side back,  large face |_ to OZ
    CR  = 2.497; //side right, large face |_ to OZ
    ferule_BF = 2.315;//ferule of type 3
    break;
  case 12:
    BF  = 2.329; //side back,  small face |_ to OZ
    CF  = 2.214; //side right, small face |_ to OZ
    AR  = 2.449; //side left,  large face |_ to OZ
    BR  = 2.586; //side back,  large face |_ to OZ
    CR  = 2.483; //side right, large face |_ to OZ
    ferule_BF = 2.336;//ferule of type 4
    break;
  case 13:
    BF  = 2.347; //side back,  small face |_ to OZ
    CF  = 2.215; //side right, small face |_ to OZ
    AR  = 2.433; //side left,  large face |_ to OZ
    BR  = 2.587; //side back,  large face |_ to OZ
    CR  = 2.468; //side right, large face |_ to OZ
    ferule_BF = 2.336;//ferule of type 4
    break;
  case 14:
    BF  = 2.371; //side back,  small face |_ to OZ
    CF  = 2.217; //side right, small face |_ to OZ
    AR  = 2.417; //side left,  large face |_ to OZ
    BR  = 2.595; //side back,  large face |_ to OZ
    CR  = 2.454; //side right, large face |_ to OZ
    ferule_BF = 2.366;//ferule of type 5
    break;
  case 15:
    BF  = 2.388; //side back,  small face |_ to OZ
    CF  = 2.218; //side right, small face |_ to OZ
    AR  = 2.402; //side left,  large face |_ to OZ
    BR  = 2.596; //side back,  large face |_ to OZ
    CR  = 2.440; //side right, large face |_ to OZ
    ferule_BF = 2.366;//ferule of type 5
    break;
  case 16:
    BF  = 2.406; //side back,  small face |_ to OZ
    CF  = 2.220; //side right, small face |_ to OZ
    AR  = 2.388; //side left,  large face |_ to OZ
    BR  = 2.599; //side back,  large face |_ to OZ
    CR  = 2.427; //side right, large face |_ to OZ
    ferule_BF = 2.413;//ferule of type 6
    break;
  case 17:
    BF  = 2.429; //side back,  small face |_ to OZ
    CF  = 2.221; //side right, small face |_ to OZ
    AR  = 2.374; //side left,  large face |_ to OZ
    BR  = 2.607; //side back,  large face |_ to OZ
    CR  = 2.415; //side right, large face |_ to OZ
    ferule_BF = 2.413;//ferule of type 6
    break;
  case 18:
    //End cap crystal
    AF  = 2.862;
    BF  = 2.862; //side back,  small face |_ to OZ
    CF  = 2.862; //side right, small face |_ to OZ
    AR  = 3.0;   //side left,  large face |_ to OZ
    BR  = 3.0;   //side back,  large face |_ to OZ
    CR  = 3.0;   //side right, large face |_ to OZ
    ferule_BF = 2.413;//ferule of type 6
    break;
  default:
    gMes->SetName("GetCMSdim");
    gMes->SetTitle("TSTRAP");
    gMes->M(kill_prog,1,"Type of crystal must lie between 1 and 18");
    break;
  }
  if (ferule_BF>BF) ferule_BF = BF;
  iso_dx = TMath::Abs(iso_dx);
  AF -= iso_dx;
  CF -= iso_dx;
  AR -= iso_dx;
  CR -= iso_dx;
}
T3Vector* TSTRAP::GetfV(Int_t i) const {
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
void TSTRAP::GetPoints(Double_t &x0,Double_t &y0,Double_t &z0,Double_t &x1,
  Double_t &y1,Double_t &z1,Double_t &x2,Double_t &y2,Double_t &z2,Double_t &x3,
  Double_t &y3,Double_t &z3,Double_t &x4,Double_t &y4,Double_t &z4,Double_t &x5,
  Double_t &y5,Double_t &z5,Double_t &x6,Double_t &y6,Double_t &z6,Double_t &x7,
  Double_t &y7,Double_t &z7) const {
  // Get all coordinates of Points
  fV0->Get(x0,y0,z0);
  fV1->Get(x1,y1,z1);
  fV2->Get(x2,y2,z2);
  fV3->Get(x3,y3,z3);
  fV4->Get(x4,y4,z4);
  fV5->Get(x5,y5,z5);
  fV6->Get(x6,y6,z6);
  fV7->Get(x7,y7,z7);
}
void TSTRAP::InitP() {
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
Double_t TSTRAP::MakePlanar() {
  //  Verifies that the points in fV0..fV7 give plane faces. The return value
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
void TSTRAP::Print() const {
  // Prints the parameters defining the TSTRAP
  gMes->SetName("Print");
  gMes->SetTitle("TSTRAP");
  gMes->M(Info_M,1,"Values of parameters");
  gMes->M(Info_M,1,"dz     : ",fDdz);
  gMes->M(Info_M,1,"theta  : ",fDtheta);
  gMes->M(Info_M,1,"phi    : ",fDphi);
  gMes->M(Info_M,1,"h1     : ",fDh1);
  gMes->M(Info_M,1,"bl1    : ",fDbl1);
  gMes->M(Info_M,1,"tl1    : ",fDtl1);
  gMes->M(Info_M,1,"alpha1 : ",fDalpha1);
  gMes->M(Info_M,1,"h2     : ",fDh2);
  gMes->M(Info_M,1,"bl2    : ",fDbl2);
  gMes->M(Info_M,1,"tl2    : ",fDtl2);
  gMes->M(Info_M,1,"alpha2 : ",fDalpha2);
}
void TSTRAP::PrintPts() const {
  // Prints the coordinates of the points of the TSTRAP
  gMes->SetName("PrintPts");
  gMes->SetTitle("TSTRAP");
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
void TSTRAP::SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const {
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
void TSTRAP::ToParam(Double_t &tx, Double_t &ty) {
  //Given that the 8 vectors fV0-7 contain the coordinates of the 8 apexes,
  //satisfying to the conditions :
  //
  //  (1)  z0 = z1 = z2 = z3 = -dz
  //  (2)  z4 = z5 = z6 = z7 = +dz
  //  (3)  y1 = y0
  //  (4)  y3 = y2
  //  (5)  y5 = y4
  //  (6)  y7 = y6
  //
  // then ToParam retrieves the value of the basic parameters of the TSTRAP :
  //
  //    fDdz, fDtheta, fDphi, 
  //    fDh1, fDbl1, fDtl1, fDalpha1,
  //    fDh2, fDbl2, fDtl2, fDalpha2.
  //
  // It is only possible if one allows for a translation
  //    tx  along the Ox axis
  //    ty  along the Oy axis.
  // ToParam returns the needed values for tx and ty.
  // For more details, see the class description.
  //
  const Double_t q0     = 0.0;
  const Double_t q1     = 1.0;
  const Double_t q2     = 2.0;
  const Double_t q4     = 4.0;
  const Double_t q8     = 8.0;
  const Double_t pideg  = 180.0;
  const Double_t lim    = 1.0e-6;
  const Double_t vsmall = 1.0e-12;
  Double_t pi,deuxpi,facdeg;
  Double_t eps;
  Double_t phi = 0.0;
  Double_t alpha1,alpha2,theta,sphi,cphi,tht,ta1,ta2;
  Double_t x0,x1,x2,x3,x4,x5,x6,x7;
  Double_t y0,y1,y2,y3,y4,y5,y6,y7;
  Double_t z0,z1,z2,z3,z4,z5,z6,z7;
  Double_t X03,X47,Y03,Y47;
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
  gMes->SetTitle("TSTRAP");
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
  eps = TMath::Abs(y5-y4);
  if (eps>lim) gMes->M(kill_prog,5,"Condition (5) not ok");
  y5        = y4;
  (*fV5)(1) = y4;
  //Condition (6)
  eps = TMath::Abs(y7-y6);
  if (eps>lim) gMes->M(kill_prog,6,"Condition (6) not ok");
  y7        = y6;
  (*fV7)(1) = y6;
  //
  //  Retrieving parameters
  //
  fDh1  = (y0 - y2)/q2;
  fDtl1 = (x0 - x1)/q2;
  fDbl1 = (x3 - x2)/q2;
  fDh2  = (y4 - y6)/q2;
  fDtl2 = (x4 - x5)/q2;
  fDbl2 = (x7 - x6)/q2;
  X03   = x0 + x1 + x2 + x3;
  X47   = x4 + x5 + x6 + x7;
  Y03   = y0 + y1 + y2 + y3;
  Y47   = y4 + y5 + y6 + y7;
  tx    = -(X03 + X47)/q8;
  ty    = -(Y03 + Y47)/q8;
  sphi  = (Y47 + q4*ty)/(q4*fDdz);
  cphi  = (X47 + q4*tx)/(q4*fDdz);
  tht   = TMath::Sqrt(sphi*sphi + cphi*cphi);
  if (tht>vsmall) {
    sphi  = sphi/tht;
    cphi  = cphi/tht;
    phi   = TMath::ATan2(sphi,cphi);
    if (sphi<q0) phi += deuxpi;
  }
  else {
    tht  = q0;
    sphi = q0;
    cphi = q1;
  }
  theta = TMath::ATan(tht);
  ta1   = (x0 + x1 - x2 - x3)/(q4*fDh1);
  ta2   = (x4 + x5 - x6 - x7)/(q4*fDh2);
  alpha1 = TMath::ATan(ta1);
  alpha2 = TMath::ATan(ta2);
  //values in degrees
  fDalpha1 = facdeg*alpha1;
  fDalpha2 = facdeg*alpha2;
  fDtheta  = facdeg*theta;
  fDphi    = facdeg*phi;
  //giving new values to parent classes
  fDx      = fDtheta;
  fDy      = fDphi;
  fDz      = fDdz;
  fH1      = fDh1;        // half length in y at low z
  fBl1     = fDbl1;       // half length in x at low z and y low edge
  fTl1     = fDtl1;       // half length in x at low z and y high edge
  fAlpha1  = fDalpha1;    // angle w.r.t. the y axis
  fH2      = fDh2;        // half length in y at high z
  fBl2     = fDbl2;       // half length in x at high z and y low edge
  fTl2     = fDtl2;       // half length in x at high z and y high edge
  fAlpha2  = fDalpha2;    // angle w.r.t. the y axis
  (*fV0)(0) += tx;
  (*fV1)(0) += tx;
  (*fV2)(0) += tx;
  (*fV3)(0) += tx;
  (*fV4)(0) += tx;
  (*fV5)(0) += tx;
  (*fV6)(0) += tx;
  (*fV7)(0) += tx;
  (*fV0)(1) += ty;
  (*fV1)(1) += ty;
  (*fV2)(1) += ty;
  (*fV3)(1) += ty;
  (*fV4)(1) += ty;
  (*fV5)(1) += ty;
  (*fV6)(1) += ty;
  (*fV7)(1) += ty;
}
void TSTRAP::ToPoints() {
  //Calculates the coordinates of the 8 apexes of the TSTRAP.
  const Double_t pideg  = 180.0;
  Double_t pi;
  Double_t alpha1,alpha2,theta,phi;
  Double_t x,y,z,tth,tx,ty,sphi,cphi,tth1,tth2;
  pi = TMath::Pi();
  Double_t facrad;
  facrad = pi/pideg;
  //
  DelPoints();
  alpha1 = facrad*fDalpha1;
  alpha2 = facrad*fDalpha2;
  theta  = facrad*fDtheta;
  phi    = facrad*fDphi;
  tth1   = TMath::Tan(alpha1);
  tth2   = TMath::Tan(alpha2);
  tth    = TMath::Tan(theta);
  sphi   = TMath::Sin(phi);
  cphi   = TMath::Cos(phi);
  tx     = tth*cphi;
  ty     = tth*sphi;
  // (0)
  x   = -fDdz*tx + fDh1*tth1 + fDtl1;
  y   =  fDh1 - fDdz*ty;
  z   = -fDdz;
  fV0 = new T3Vector(x,y,z);
  // (1)
  x   = -fDdz*tx + fDh1*tth1 - fDtl1;
  fV1 = new T3Vector(x,y,z);
  // (2)
  x   = -fDdz*tx - fDh1*tth1 - fDbl1;
  y   = -fDh1 - fDdz*ty;
  fV2 = new T3Vector(x,y,z);
  // (3)
  x   = -fDdz*tx - fDh1*tth1 + fDbl1;
  y   = -fDh1 - fDdz*ty;
  fV3 = new T3Vector(x,y,z);
  // (4)
  x   =  fDdz*tx + fDh2*tth2 + fDtl2;
  y   =  fDh2 + fDdz*ty;
  z   =  fDdz;
  fV4 = new T3Vector(x,y,z);
  // (5)
  x   =  fDdz*tx + fDh2*tth2 - fDtl2;
  fV5 = new T3Vector(x,y,z);
  // (6)
  x   =  fDdz*tx - fDh2*tth2 - fDbl2;
  y   = -fDh2 + fDdz*ty;
  fV6 = new T3Vector(x,y,z);
  // (7)
  x   =  fDdz*tx - fDh2*tth2 + fDbl2;
  fV7 = new T3Vector(x,y,z);
}
