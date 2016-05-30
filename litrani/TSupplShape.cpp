// @(#)root/html:$Id: TSupplShape.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TwoPadDisplay.h"
#include "TMessErr.h"
#include "TLitGlob.h"
#include "TOpticMaterial.h"
#include "TRevetment.h"
#include "TDetector.h"
#include "TSBRIK.h"
#include "TSTRD1.h"
#include "TSTRD2.h"
#include "TSPARA.h"
#include "TSTRAP.h"
#include "TS8PTS.h"
#include "TSGTRA.h"
#include "TSCYL.h"
#include "TSTUBE.h"
#include "TSCONE.h"
#include "TSupplShape.h"

ClassImp(TSupplShape)
//______________________________________________________________________________
//
//BEGIN_HTML <!--
/* -->
</pre>
<h2>Description of TSupplShape</h2>
<h4><font color="#9c0031">Recall that all dimensions of shapes are in [cm] in Litrani 
!</font></h4>
<div align="justify">
<p>TSupplShape adds data and methods used by ALL types of TSShape {tsbrik, tstrd1, 
tstrd2, tspara, tstrap, tsgtra, ts8pts tscyl, tstube, tscone}. To each shape of Litrani 
is added a class of type TSupplShape, which contains everything which is common to 
all types of shapes. The name of the TSupplShape associated with a shape is the same 
as the name of the shape. The user has not to define the TSupplShape. It is done 
automatically by the constructor of the associated shape. If you have at hand a pointer 
p pointing towards a shape, for instance you have defined :</p>
</div>
<div align="justify">
<ul>
<li><font color="red"><code>TSCYL *p = new TSCYL((&quot;APD&quot;,&quot;APD&quot;,&quot;Silicium&quot;,&quot;TotAbsorbing&quot;,0.4,0.1);</code></font>
</ul>
</div>
<p>then <font color="red"><code>p-&gt;fSuppl</code> </font>is the pointer towards 
the TSupplShape associated with the shape &quot;APD&quot; of type TSCYL. The TSupplShape 
contains information common to all Shape, like :</p>
<ul>
<li>number of points
<li>number of supplementary points
<li>list of all points
<li>number of master faces
<li>number of subfaces
<li>optical material of the shape
<li>default revetment of the shape
</ul>
<p>If in the definition of TSCYL, you define the last parameter [Bool_t FromOther] 
as true, then the second constructor for TSupplShape is used. It means that the material 
of the shape and the optical axis are taken from an other shape. In that case, for 
a transition between this shape and the other shape having same material and same 
optical axis, the photon simply continues from one shape to the other, without any 
transition calculation.</p>
<h3>Matrix for Optical Anisotropy</h3>
<p>Let <b>Oxyz</b> be <font color="#9c0031"><i>the local coordinate system</i></font>, 
<b>Ox'y'z'</b> be <font color="#9c0031"><cite>the system in which the dielectric 
tensor is diagonal</cite></font>, with eigenvalues <font face="Symbol"><b>e</b></font><sub>a</sub>, 
<font face="Symbol"><b>e</b></font><sub>b</sub> and <font face="Symbol"><b>e</b></font><sub>c</sub> 
. &nbsp;We call the <b>Ox'y'z'</b> system <font color="#9c0031">&quot;<cite>the system 
of the 3 optical axis</cite>&quot;</font>. By convention, <font face="Symbol"><b>e</b></font><sub>a</sub> 
&lt;= <font face="Symbol"><b>e</b></font><sub>b</sub> &lt;= <font face="Symbol"><b>e</b></font><sub>c</sub> 
. By convention also, <font face="Symbol"><b>e</b></font><sub>a</sub> is associated 
with the <b>Ox' </b>axis, <font face="Symbol"><b>e</b></font><sub>b</sub> with the 
<b>Oy'</b> axis and <font face="Symbol"><b>e</b></font><sub>c</sub> with the <b>Oz</b>' 
axis. Let <font color="#9c0031"><b>M</b></font> be the rotation matrix which converts 
the <b>Ox, Oy, Oz </b>axis to the <b>Ox'</b>, <b>Oy'</b> and <b>Oz' </b>axis :</p>
<center>
<p>
<table border="2" cellpadding="2">
<tr>
<td><b>Ox' </b>= <font color="#9c0031"><b>M</b></font> <b>Ox</b></td>
</tr>
<tr>
<td><b>Oy'</b> = <font color="#9c0031"><b>M</b></font> <b>Oy</b></td>
</tr>
<tr>
<td><b>Oz'</b> = <font color="#9c0031"><b>M</b></font> <b>Oz</b></td>
</tr>
</table>
</center>
<p>If you have given</p>
<ul>
<li>the <b>Ox' </b>axis &quot;&agrave; la G&eacute;ant&quot; with ( <font face="Symbol"><b>q</b></font><font size="1"><sub>x</sub></font> 
, <font face="Symbol"><b>f</b></font><font size="1"><sub>x</sub></font> ) = theta<font size="1"><sub>x</sub></font> 
and phi<font size="1"><sub>x</sub></font> angle of the <b>Ox'</b> axis as seen in 
the <b>Oxyz</b> coordinate system
<li>the <b>Oy' </b>axis &quot;&agrave; la G&eacute;ant&quot; with ( <font face="Symbol"><b>q</b></font><font size="1"><sub>y</sub></font> 
, <font face="Symbol"><b>f</b></font><font size="1"><sub>y</sub></font> ) = theta<font size="1"><sub>y</sub></font> 
and phi<font size="1"><sub>y</sub></font> angle of the <b>Oy'</b> axis as seen in 
the <b>Oxyz</b> coordinate system
<li>the <b>Oz' </b>axis &quot;&agrave; la G&eacute;ant&quot; with ( <font face="Symbol"><b>q</b></font><font size="1"><sub>z</sub></font> 
, <font face="Symbol"><b>f</b></font><font size="1"><sub>z</sub></font> ) = theta<font size="2"><sub>z 
</sub></font><sub>&nbsp;</sub>and phi<font size="2"><sub>z</sub></font> angle of 
the <b>Oz'</b> axis as seen in the <b>Oxyz</b> coordinate system
</ul>
<p>Then we have :</p>
<center>
<p>
<table border="2" cellpadding="2">
<caption><font color="#9c0031"><b>M</b></font> matrix</caption>
<tr>
<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>x</sub></font> 
cos<font face="Symbol"><b>f</b></font><font size="1"><sub>x</sub></font></td>
<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>y</sub></font> 
cos<font face="Symbol"><b>f</b></font><font size="1"><sub>y</sub></font></td>
<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>z</sub></font> 
cos<font face="Symbol"><b>f</b></font><font size="1"><sub>z</sub></font></td>
</tr>
<tr>
<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>x</sub></font> 
sin<font face="Symbol"><b>f</b></font><font size="1"><sub>x</sub></font></td>
<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>y</sub></font> 
sin<font face="Symbol"><b>f</b></font><font size="1"><sub>y</sub></font></td>
<td>sin<font face="Symbol"><b>q</b></font><font size="1"><sub>z</sub></font> 
sin<font face="Symbol"><b>f</b></font><font size="1"><sub>z</sub></font></td>
</tr>
<tr>
<td>cos<font face="Symbol"><b>q</b></font><font size="1"><sub>x</sub></font></td>
<td>cos<font face="Symbol"><b>q</b></font><font size="1"><sub>y</sub></font></td>
<td>cos<font face="Symbol"><b>q</b></font><font size="1"><sub>z</sub></font></td>
</tr>
</table>
</center>
<p>If <b>E</b> is a vector expressed in the LCS <b>Oxyz</b> ( Local Coordinate 
System ) and <b>E'</b> is the same vector expressed in the system where the dielectric 
tensor is diagonal , <cite>the system </cite><b>Ox'y'z'</b> <cite>of the optical 
axis,</cite> then : &nbsp;<b>E</b> = <font color="#9c0031"><b>M</b></font> <b>E'</b> 
&nbsp;( and not <b>E'</b> = <font color="#9c0031"><b>M</b></font> <b>E </b>! ) . 
This is the well known fact that vectors are contravariant if axis vector are covariant. 
&nbsp;Now let us take the Maxwell equation in both systems ( <font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font> 
diagonal matrix ):</p>
<center>
<p>
<table border="1" cellpadding="2">
<tr>
<td></td>
<td>In System <b>Oxyz</b></td>
<td>In system <b>Ox'y'z'</b></td>
</tr>
<tr>
<td>Maxwell equation</td>
<td><font color="black"><b>D</b></font> = e<sub>0</sub> <font face="Symbol" color="#9c0031" size="4"><b>e</b></font> 
<font color="black"><b>E</b></font></td>
<td><font color="black"><b>D'</b></font> = e<sub>0</sub> <font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font> 
<font color="black"><b>E' </b></font></td>
</tr>
<tr>
<td>Transform</td>
<td><font color="black"><b>E</b></font> = <font color="#9c0031"><b>M</b></font> 
<font color="black"><b>E' </b></font>&nbsp; &nbsp; &nbsp; &nbsp; <font color="black"><b>D</b></font>= 
<font color="#9c0031"><b>M</b></font><font color="black"><b> D'</b></font></td>
<td><font color="black"><b>E' </b></font>= <font color="#9c0031"><b>M</b></font><font size="1" color="#9c0031"><sup><b>T</b></sup></font><font color="black"><b> 
E</b></font>&nbsp; &nbsp;&nbsp; <font color="black"><b>D'</b></font>= <font color="#9c0031"><b>M</b></font><font size="1" color="#9c0031"><sup><b>T</b></sup></font> 
<font color="black"><b>D</b></font></td>
</tr>
<tr>
<td>Maxwell equation rewritten</td>
<td><font color="#9c0031"><b>M</b></font> <font color="black"><b>D'</b></font>= 
e<sub>0</sub> <font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font color="#9c0031"> 
<b>M</b></font> <font color="black"><b>E'</b></font>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp;==&gt;</td>
<td><font color="black"><b>D'</b></font>= e<sub>0</sub> <font color="#9c0031"><b>M</b></font><font size="1" color="#9c0031"><sup><b>T</b></sup></font><font color="#9c0031"><b> 
</b></font><b><font face="Symbol" color="#9c0031" size="4">e</font><font color="#9c0031"> 
M</font></b> <font color="black"><b>E' </b></font></td>
</tr>
<tr>
<td></td>
<td><font face="Symbol" color="#9c0031" size="4"><b>e</b></font> = <font color="#9c0031"><b>M 
</b></font><font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font><b><font color="#9c0031"> 
M</font></b><font color="#9c0031"> </font><font size="1" color="#9c0031"><sup><b>T</b></sup></font> 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&lt;==</td>
<td><font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font> 
=<font color="#9c0031"> <b>M</b></font><font size="1" color="#9c0031"><sup><b>T</b></sup></font><font color="#9c0031"><b> 
</b></font><b><font face="Symbol" color="#9c0031" size="4">e</font><font color="#9c0031"> 
M</font></b></td>
</tr>
</table>
</center>
<p>This implies that knowing the dielectic tensor <font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font> 
in the system where it is diagonal with diagonal elements <font face="Symbol"><b>e</b></font><sub>a</sub>, 
<font face="Symbol"><b>e</b></font><sub>b</sub> and <font face="Symbol"><b>e</b></font><sub>c</sub> 
&nbsp;, we get the dielectric tensor in the local coordinate system through : <font face="Symbol" color="#9c0031" size="4"><b>e</b></font> 
= <font color="#9c0031"><b>M </b></font><font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font size="1" color="#9c0031"><sub><b>D</b></sub></font><b><font color="#9c0031"> 
M</font></b><font color="#9c0031"> </font><font size="1" color="#9c0031"><sup><b>T</b></sup></font> 
. All this is also true if the Oxyz system is the World Coordinate System and not 
the Local Coordinate System. So if &nbsp;the Ox', Oy' and Oz' axis are given <em>with 
respect to the World Coordinate system</em>, equation <font face="Symbol" color="#9c0031" size="4"><b>e</b></font><font color="#9c0031"> 
= <b>M </b></font><b><font face="Symbol" color="#9c0031" size="4">e</font><font size="1" color="#9c0031"><sub>D</sub></font><font color="#9c0031"> 
M</font></b><font color="#9c0031"> </font><font size="1" color="#9c0031"><sup><b>T</b></sup></font> 
&nbsp; &nbsp; applies identically, with a different <font color="#9c0031"><b>M</b></font> 
matrix.</p>
<h4>Note concerning opticians jargon.</h4>
<p>Like Moli&egrave;re's physicians, opticians do have a jargon which can be misleading. 
We call here the <b>Ox'y'z'</b> system, the system in which the dielectric tensor 
is diagonal, <font color="#9c0031">&quot;<cite>the system of the 3 optical axis</cite>&quot;</font>. 
This is not to be confused with what is called <font color="#9c0031"><i>&quot;the 
optical axis&quot;</i></font>. <font color="#9c0031"><i>&quot;The optical axis&quot;</i></font> 
has a meaning only in the case of birefringence, where either <font face="Symbol"><b>e</b></font><sub>a</sub> 
= <font face="Symbol"><b>e</b></font><sub>b</sub> or <font face="Symbol"><b>e</b></font><sub>b</sub>&nbsp;= 
<font face="Symbol"><b>e</b></font><sub>c</sub> . <font color="#9c0031"><i>&quot;The 
optical axis&quot;</i></font> &nbsp;is a unique axis, corresponding to the diagonal 
element which is isolated. So</p>
<ul>
<li>if <font face="Symbol"><b>e</b></font><sub>a</sub> = <font face="Symbol"><b>e</b></font><sub>b</sub> 
then <font color="#9c0031"><i>&quot;The optical axis&quot;</i></font> is <b>Oz'</b> 
.
<li>if <font face="Symbol"><b>e</b></font><sub>b</sub>&nbsp;= <font face="Symbol"><b>e</b></font><sub>c</sub> 
then <font color="#9c0031"><i>&quot;The optical axis&quot;</i></font> is <b>Ox'</b> 
.
</ul>
<p>Notice that 3*3 matrices are represented in the code by one dimensional array 
of 9 elements numbered column wise :</p>
<center>
<p>
<table border="5" cellspacing="5" cellpadding="5" align="Center">
<tr>
<td>0</td>
<td>3</td>
<td>6</td>
</tr>
<tr>
<td>1</td>
<td>4</td>
<td>7</td>
</tr>
<tr>
<td>2</td>
<td>5</td>
<td>8</td>
</tr>
</table>
</center>
<h3>Transforming a shape into a detector</h3>
<p>All shapes in Litrani may at the same time be a detector of photon. 5 types 
of detector may be defined :</p>
<ul>
<li><b><code>none : </code></b>this shape is not a detector
<li><b><code>surfdet : </code></b>Surface detector. In that case, the photon is 
detected if it reaches a face or a subface of the shape, under some conditions. See 
the description of class TFace to know what a subface is. The conditions may be of 
2 kinds : acceptance angle of the photon and cathode efficiency. Look at the method 
SetSurfDet() to know more about surface detectors.
<li><b><code>phototube : </code></b>A phototube is a special case of a surface 
detector, with the following restrictions : the associated shape is a TSCYL, there 
is a condition of cathode efficiency, there are not conditions about acceptance angle. 
See the method SetPhotoTube(). If it ever happens to you that your phototube has 
not the shape of a cylinder, no problem : define it as a surfdet. Notice that the 
length of the cylinder is not the length of the phototube from its entrance windows 
until its basis ! it is the length from its entrance window until the photocathode.
<li><b><code>volumedet : </code></b>Volume detector. In that case, a photon is 
detected if it stops inside the volume of the shape. One can associate an equivalent 
of photocathode efficiency, a &quot;volume efficiency&quot;, depending upon wavelength. 
See method VolD().
<li><b><code>apd : </code></b>An APD is a more complicated volume detector. It 
must be associated with a TSCYL or a TSBRIK. 2 parallel faces of the shape must be 
defined as cathode and anode. A distribution function (of class TSplineFit) gives the 
number of electrons generated by the stopping of the photon inside the volume at 
a coordinate z with respect to the axis going from cathode to anode. This distribution 
function is what is called the gain profile of the APD. The photon is considered 
as detected if it has stopped at a z coordinate such that at least 5 electrons have 
been emitted. This value of 5 is arbitrary and may be changed by the user. In fact, 
what really matters here is not whether the photon has been detected or not (has 
emitted more or less than 5 electrons), but how many electrons have been emitted 
and at what time they have reached the anode. This value is recorded into an histogram 
which gives directly the shape of the pulse emitted by the APD after the emission 
of all photons of the run. See method SetAPD().
</ul>
<p>Important notice concerning surface detector : if you declare that some face 
of a shape is a detector, (by a call to SetSurfDet() or SetPhotoTube()), it is the 
whole surface of the face declared in argument which is a detector. Is it possible 
to have a face for which only a portion of the surface is a detector ? Is it possible 
to place many detectors on one face ? The answer is <font color="#9c0031">no</font>, 
but it is <font color="#9c0031">not at all a restriction</font> ! Nothing prevents 
you to create onto the face one or many subfaces and to make a call to SetSurfDet() 
or SetPhotoTube() for each subface. See the description of the class TFace to learn 
how to create subfaces.</p>
<h3>simulating bevellings (chamfers)</h3>
<p>Litrani has a simple way to simulate bevellings : see the method TSupplShape::SetBevellings(). 
If a photon hit a face of a shape and if the distance of the hit point to the nearest 
ridge (ar&egrave;te) of this face is smaller than a distance specified in the call 
to SetBevellings(), the photon is supposed to be on the bevelling and do not see 
the normal revetment of the face, but the kind of revetment specific to the bevellings 
that has been specified in SetBevellings(). This method has the advantage of being 
extremely simple for the user to see immediatly what kind of effects is to be expected 
from bevellings, without setting up a complicate geometrical set-up.&nbsp;However 
it has to be reminded that :</p>
<ul>
<li>on the geometrical point of view, it is not a true simulation of bevellings, 
since using this method does not generate the right form of bevellings (i.e thin 
faces at 45&deg; of the 2 adjacent faces).
<li>it is not possible to declare that some ridges are bevelled, some other not.&nbsp;Either 
all ridges have bevellings, or none.
</ul>
<p>If you want a more elaborate simulation of bevellings, you cannot avoid to do 
it yourself by constructing, using shapes like TSTRD1, TSTRD2, TSTRAP, ... the true 
geometrical set-up with bevellings.</p>
<p></p>
<p></p>
<h3>Simulating radiation damages inside a shape</h3>
<p>Let us consider a shape having suffered from radiation damages. The radiation 
damages have been produced by electromagnetic or hadronic showers having irradiated 
the shape.&nbsp;Let <b><code><font size="+1" color="#00008b">D</font></code></b> 
[<b>damaxis</b>] be the axis of the showers having caused the damages, and <b><code><font size="+1" color="#00008b">A</font></code></b> 
[<b>damstart</b>] the starting point of these showers. Let <code><font size="+1" color="#00008b"><b>P</b></font></code> 
(x,y,z) be a point inside the shape and <b><code><font size="+1" color="#00008b">Q</font></code></b> 
the projection of this point on the axis <b><code><font size="+1" color="#00008b">D</font></code></b>.&nbsp;We 
call z the distance <b><code><font size="+1" color="#00008b"><u>AQ</u></font></code></b> 
and r the distance <b><code><font size="+1" color="#00008b"><u>PQ</u></font></code></b>. 
We will try to describe the damages as a function of (z,r). Let:</p>
<div align="center">
<table border="8" cellpadding="4" cellspacing="2" bgcolor="#faebd7">
<tr>
<td width="200"><b><code><font size="+1">K</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">) 
== 1/L</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">)</font></code></b></td>
<td><b><code><font size="+1">L</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">)</font></code></b>be 
the absorption length of the material of the shape <b>before</b> radiation damage. 
It is not a function of <b><code><font size="+1">(z,r)</font></code></b>. It is a 
function of the wavelength <b><font face="Symbol">l</font></b>. <b><code><font size="+1">K</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">)</font></code></b> 
is defined because handier in the final formula than <b><code><font size="+1">L</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">)</font></code></b>.</td>
<td></td>
</tr>
<tr>
<td width="200"><b><code><font size="+1">L</font>(z,r,</code><font face="Symbol">l</font><code>)</code></b></td>
<td colspan="2">be the absorption length of the material of the shape <b>after</b> 
irradiation.&nbsp;It is a function of <b><code>(z,r,</code><font face="Symbol">l</font><code>)</code></b>.</td>
</tr>
<tr>
<td width="200"><b><code><font size="+1">L</font><sub>i</sub>(z,r,</code><font face="Symbol">l</font><code>)</code></b></td>
<td colspan="2">be what the absorption length of the material <b>would be</b> 
if the <b>only</b> reasons for a finite absorption length were the irradiation damages. 
See formula (1).</td>
</tr>
<tr>
<td width="200">
<div align="left">
<b><code><font size="+1">L</font><sub>i</sub></code><code><sub>min</sub>(</code><font face="Symbol">l</font><code>)<sub> 
</sub>= <font size="+1">L</font><sub>i</sub>(<font size="+1">z<sub>max</sub>,0</font>,</code><font face="Symbol">l</font><code>)</code></b></div>
</td>
<td colspan="2"><b><code><font size="+1">L</font><sub>i</sub>(z,r,</code><font face="Symbol">l</font><code>)</code></b> 
at the point where it is <b>minimal</b>, i.e. at the point where the damages are 
<b>maximal</b>.</td>
</tr>
<tr>
<td width="200">
<div align="left">
<b><code><font size="+1">K</font><sub>imax</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">) 
= 1/L</font><sub>imin</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">)</font></code></b></div>
</td>
<td colspan="2"><b><code><font size="+1">K</font><sub>imax</sub></code></b> 
is handier then <b><code><font size="+1">L</font><sub>imin</sub></code></b> since 
<b><code><font size="+1">K</font><sub>imax</sub></code></b> is 0, but <b><code><font size="+1">L</font><sub>imin</sub></code></b> 
is infinite for 0 damage.</td>
</tr>
<tr>
<td width="200">
<div align="left">
Now we have:</div>
</td>
<td>
<div align="center">
<code><b><font size="+1">1/</font></b></code><b><code><font size="+1">L</font>(z,r,</code><font face="Symbol">l</font><code>) 
= </code></b><code><b><font size="+1">1/</font></b></code><b><code><font size="+1">L</font><sub>0</sub>(</code><font face="Symbol">l</font><code>) 
+ </code></b><code><b><font size="+1">1/</font></b></code><b><code><font size="+1">L</font><sub>i</sub>(z,r,</code><font face="Symbol">l</font><code>)</code></b></div>
</td>
<td>(1)</td>
</tr>
<tr>
<td width="200">
<div align="left">
<b><code><font size="+1">f(z,r)</font></code></b></div>
</td>
<td colspan="2">is a distribution which is proportionnal to the damages.&nbsp;It 
is a function of <b><code><font size="+1">(z,r)</font></code></b>. For instance, 
for damages caused by electromagnetic showers, <b><code><font size="+1">f(z,r)</font></code></b> 
can be taken as the distribution of the damaging shower itself: the energy deposited 
at <b><code><font size="+1">(z,r)</font></code></b>. Or we can let the user give 
himself  <b><code><font size="+1">f(z,r)</font></code></b>.</td>
</tr>
<tr>
<td width="200">
<div align="left">
<b><code><font size="+1">f<sub>max</sub> = f(z<sub>max</sub>,0)</font></code></b></div>
</td>
<td colspan="2">Value of <b><code><font size="+1">f(z,r)</font></code></b> at 
the point where it is maximum</td>
</tr>
<tr>
<td colspan="3">
<div align="center">
We postulate then (hoping it is not to wrong an assumption)</div>
</td>
</tr>
<tr>
<td colspan="2">
<div align="center">
<b><code><font size="+1">L</font><sub>i</sub>(z,r,</code><font face="Symbol">l</font><code>) 
= <font size="+1">L</font><sub>i</sub></code><code><sub>min</sub>(</code><font face="Symbol">l</font><code>)*(<font size="+1">f<sub>max</sub></font>/<font size="+1">f(z,r)</font>)</code></b></div>
</td>
<td>(2)</td>
</tr>
<tr>
<td colspan="3">
<div align="center">
Which gives for the absorption length, using (1) and (2) :</div>
</td>
</tr>
<tr>
<td colspan="2">
<div align="center">
<b><font color="#9c0031"><code><font size="+1">L</font>(z,r,</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>) 
= (<font size="+1">f</font><font size="+1"><sub>max</sub></font><font size="+1">*L</font><sub>0</sub>(</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>)*<font size="+1">L</font><sub>imin</sub>(</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>)<font size="+1">) 
/ (f(z,r)*L</font><sub>0</sub>(</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>) 
+ <font size="+1">f</font><font size="+1"><sub>max</sub></font>*<font size="+1">L</font><sub>imin</sub>(</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>)<font size="+1">)</font></code></font></b></div>
</td>
<td>(3)</td>
</tr>
<tr>
<td colspan="2">
<div align="center">
<b><font color="#9c0031"><code><font size="+1">L</font></code><code>(z,r,</code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code>) 
= </code><code><font size="+1">f</font></code><code><font size="+1"><sub>max </sub>/(</font></code><code><font size="+1">f</font></code><code><font size="+1"><sub>max</sub></font></code><code>*<font size="+1">K</font><sub>0</sub><font size="+1">(</font><font face="Symbol" size="+1">l</font><font size="+1">) 
+ f(z,r)*K</font><sub>imax</sub><font size="+1">(</font></code></font><font face="Symbol" color="#9c0031">l</font><font color="#9c0031"><code><font size="+1">))</font></code></font></b></div>
</td>
<td>(3)</td>
</tr>
<tr>
<td colspan="3">
<div align="center">
The idea is now to proceed like this:</div>
</td>
</tr>
<tr>
<td width="200">damages by electromagnetic showers</td>
<td colspan="2"><b><code><font size="+1">f(z,r)</font></code></b> taken as the 
energy deposit of the electromagnetic shower as described in the class description 
of <a href="TElecCascade.html">TElecCascade</a>, and in the Review of Particle Physics, 
Vol 3 N&deg;1-4, 1998 pages 148-149. By that, <b><code><font size="+1">f<sub>max</sub></font></code></b> 
is also given. We ask the user to provide <b><code><font size="+1">K</font><sub>imax</sub></code></b> 
either as a constant, not depending upon <b><font face="Symbol">l</font></b>, or 
as a fit describing the dependency of <b><code><font size="+1">K</font><sub>imax</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">)</font></code></b> 
as a function of <b><font face="Symbol">l</font></b>. Nothing else has to be provided.</td>
</tr>
<tr>
<td width="200">other type of damages</td>
<td colspan="2">We continue to assume even in that case that the damages have 
an axis <b><code><font size="+1" color="#00008b">D</font></code></b> [<b>damaxis</b>] 
and a starting point <b><code><font size="+1" color="#00008b">A</font></code></b> 
[<b>damstart</b>] . We continue to describe the damages as a function of <b><code><font size="+1">(z,r)</font></code></b>. 
We continue to ask the user to provide <b><code><font size="+1">K</font><sub>imax</sub></code></b> 
either as a constant, not depending upon <b><font face="Symbol">l</font></b>, or 
as a fit describing the dependency of <b><code><font size="+1">K</font><sub>imax</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">)</font></code></b> 
as a function of <b><font face="Symbol">l</font></b>. But we ask the user to define 
<b><code><font size="+1">f(z,r)</font></code></b>.&nbsp;For that, we propose 2 methods:</td>
</tr>
<tr>
<td width="200"><b><code><font size="+1">f(z,r)</font></code></b> method 1</td>
<td>
<div align="center">
<p><code><b><font size="+1">f(z,r) = exp(-(z-</font></b></code><b><code><font size="+1">z<sub>m</sub></font></code></b><code><b><font size="+1">)<sup>2</sup>/(2*</font></b></code><b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>z</sub></font></code></b><code><b><font size="+1"><sup>2</sup>))*exp(-r<sup>2</sup>/(2*</font></b></code><b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b><code><b><font size="+1"><sup>2</sup>))</font></b></code></p>
<p><b><code><font size="+1">f<sub>max </sub></font></code></b><code><b><font size="+1">= 
1</font></b></code></p>
</div>
<p>The user has to provide the 3 constants <b><code><font size="+1">z<sub>m</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>z</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b>. 
He has also to provide <b><code><font size="+1">K</font><sub>imax</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">)</font> 
</code></b>either as a constant or as a fit describing the dependency upon <b><font face="Symbol">l</font></b>.</p>
</td>
<td>(4)</td>
</tr>
<tr>
<td width="200"><b><code><font size="+1">f(z,r)</font></code></b> method 2</td>
<td>
<div align="center">
<p><code><b><font size="+1">f(z,r) = </font></b></code><code><b><font size="+1">g(z)*exp(-r<sup>2</sup>/(2*</font></b></code><b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b><code><b><font size="+1"><sup>2</sup>))</font></b></code></p>
<p><b><code><font size="+1">f<sub>max </sub></font></code></b><code><b><font size="+1">= 
g<sub>max</sub></font></b></code></p>
</div>
<p>The user is asked to provide a fit for <code><b><font size="+1">g(z)</font></b></code>.&nbsp;This 
fit <b>has to be bounded</b> at a value <code><b><font size="+1">g<sub>max</sub></font></b></code> 
provided by the user. The user has also to provide <b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b>. 
He has also to provide <b><code><font size="+1">K</font><sub>imax</sub><font size="+1">(</font></code><font face="Symbol">l</font><code><font size="+1">)</font> 
</code></b>either as a constant or as a fit describing the dependency upon <b><font face="Symbol">l</font></b>.</p>
</td>
<td>(5)</td>
</tr>
</table>
</div>
<p></p>
<p>The methods <code><font size="+1"><b>ShowerDamage() </b></font></code>or <code><font size="+1"><b>RadiationDamage()</b></font></code> 
implement the processes described above.</p>
<p></p>
<h4><font color="#9c0031">ShowerDamage()</font></h4>
<p>ShowerDamage() is to be preferred when the damages have been caused by the irradiation 
by electromagnetic showers, gammas or electrons. In order that <code><font size="+1"><b>ShowerDamage()</b></font></code> 
can be called for this shape, it is necessary that radiation length, critical energy 
and Moliere radius have been defined for the material of this shape. It means that 
you have called <a href="TOpticMaterial.html">TOpticMaterial::SetCascades()</a>. 
</p>
<ul>
<li>In the first instance of ShowerDamage(), the user has to provide <b><code><font size="+1">K</font><sub>imax</sub> 
</code></b>as a constant, not depending upon <b><font face="Symbol">l</font></b>.
<li>In the second instance of the method ShowerDamage()<code><font size="+1"><b>, 
</b></font></code>he has to provide a pointer towards a fit, giving <b><code><font size="+1">K</font><sub>imax</sub> 
</code></b>as a function of wavelength <b><font face="Symbol">l</font></b>.
</ul>
<h4><font color="#9c0031">RadiationDamage()</font></h4>
<p>RadiationDamage() has to be taken in all other cases, when the damages have 
not been caused by electromagnetic showers.&nbsp;For instance, take it when the damages 
have been caused by hadronic showers. </p>
<ul>
<li>In the 1st instance of this method, damages are fully described by gaussians, 
as seen in (4). The user has to provide the parameters of these gaussians:  <b><code><font size="+1">z<sub>m</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>z</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b> 
and a constant value for <b><code><font size="+1">K</font><sub>imax</sub></code></b>, 
which is not considered as a function of wavelength <b><font face="Symbol">l</font></b> 
in this case.
<li>In the 2nd instance of this method, damages are fully described by gaussians, 
as seen in (4). The user has to provide the parameters of these gaussians: <b><code><font size="+1">z<sub>m</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>z</sub></font></code></b>, 
<b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b> 
and a fit for <b><code><font size="+1">K</font><sub>imax</sub>(</code><font face="Symbol">l</font><code>)</code></b>, 
which is considered as a function of wavelength <b><font face="Symbol">l</font></b> 
in this case.
<li>In the 3rd instance of this method, the z dependency is not a gaussian, but 
is described by a fit provided by the user. The r dependency is still described by 
a gaussian, whose sigma <b><code><font size="+1" face="Symbol">s</font><font size="+1"><sub>r</sub></font></code></b> 
has to be given by the user. The user has to provide a constant value for <b><code><font size="+1">K</font><sub>imax</sub></code></b>, 
which is not considered as a function of wavelength <b><font face="Symbol">l</font></b> 
in this case.
<li>In the 4th instance of this method, the z dependency is not a gaussian, but 
is described by a fit provided by the user. The r dependency is still described by 
a gaussian, whose sigma has to be given by the user. The user has to provide a fit 
for <b><code><font size="+1">K</font><sub>imax</sub>(</code><font face="Symbol">l</font><code>)</code></b>, 
which is considered as a function of wavelength <b><font face="Symbol">l</font></b> 
in this case.
</ul>
<p></p>
<h4><font color="#9c0031">Visual checks</font></h4>
<p>Calling method <b><code><font size="+1">RecordDamages()</font></code></b> before 
any generation of photons and calling <b><code><font size="+1">ShowDamage()</font></code></b> 
after the generation of photons, you will have a visual check of the damage factors 
produced as a function of (z,r).&nbsp;In fact, what is plotted is </p>
<div align="center">
<p><b><code><font size="+1">1 - F, where F = L(z,r,</font><font face="Symbol" size="+1">l</font><font size="+1">)/L<sub>0</sub>(</font><font face="Symbol" size="+1">l</font><font size="+1">)</font></code></b></p>
</div>
<p>Do call these methods only for checks, do not call them inside a production: 
they take time.</p>
<p></p>
<h4><font color="#9c0031">Examples of use</font></h4>
<p>In the CINT macros provided with Litrani, you will find:</p>
<ul>
<li><b><code><font size="+1">xdamage1.C</font></code></b> which is identical to 
<b><code><font size="+1">xtransmission1.C</font></code></b>, except that we have 
added radiation damages to the crystal

<li><b><code><font size="+1">xdambeam1.C</font></code></b> which is identical 
to <b><code><font size="+1">xanibeam1.C</font></code></b> plus radiation damages 
to the crystal.

<li><b><code><font size="+1">xdambeam2.C</font></code></b> which is identical 
to <b><code><font size="+1">xanibeam2.C</font></code></b> plus radiation damages 
to the crystal.  
</ul>
<p></p>
<pre>
<!--*/
// -->END_HTML
Int_t TIntercept::Compare(const TObject *b) const {
  if (fT > ((TIntercept *)b)->fT) return 1;
  else if (fT < ((TIntercept *)b)->fT) return -1;
  else return 0;
}
Bool_t TIntercept::IsEqual(const TObject *obj) const {
  return (fT == ((TIntercept *)obj)->fT);
}
TSupplShape::TSupplShape(const char *name, const char *title, TShape *shape,
  const KindOfShape kind, const char *namemat, const char *namerev):TNamed(name,title) {
  //  Usual constructor when material and optical axis are specified for this
  //shape. They are not taken from an other shape.
  const Double_t zero = 0.0;
  Init();
  fSShape = shape;
  fKind   = kind;
  Bool_t found = kFALSE;
  TOpticMaterial *pMat;
  TRevetment *pRev;
  TString s,s1;
  TIter next1(&gLit->fOptMat);
  while ((!found) && (pMat = (TOpticMaterial *)next1())) {
    s = pMat->GetName();
    if (!s.CompareTo(namemat)) {
      found = kTRUE;
      fOptMat = pMat;
      fOptMat->SetInUse();
    }
  }
  if (!found) {
    s1 = namemat;
    gMes->SetName("TSupplShape");
    gMes->SetTitle("TSupplShape");
    gMes->M(kill_prog,1,"This material has not been found",s1);
  }
  else {
    fAnisotropic = fOptMat->GetAnisotropy();
    fAniAbsL     = fOptMat->GetAniAbsL();
    fDielChange  = fOptMat->GetIChangeWithWvl();
  }
  found = kFALSE;
  s="none";
  s1="";
  if ((!s.CompareTo(namerev)) || (!s1.CompareTo(namerev))) {
    found = kTRUE;
    fDefRev = 0;
  }
  else {
    TIter next(&gLit->fRevetment);
    while ((!found) && (pRev = (TRevetment *)next())) {
      s = pRev->GetName();
      if (!s.CompareTo(namerev)) {
        found = kTRUE;
        fDefRev = pRev;
      }
    }
    if (!found) {
      s1 = namerev;
      gMes->SetName("TSupplShape");
      gMes->SetTitle("TSupplShape");
      gMes->M(Error_M,2,"This revetment has not been found. None assumed !",s1);
      fDefRev = 0;
    }
  }
  //Set as not a detector by defaults
  fNDet = -1;
  fDet  = none;
  fIsQE = kFALSE;
  fQE   = 0;
  fDistCatAn = zero;
  SetBevellings(kFALSE); //no bevellings by default
  AddToList(name);
}
TSupplShape::TSupplShape(const char *name, const char *title, TShape *shape,
  const KindOfShape kind, TString nameshape, const char *namerev):TNamed(name,title) {
  //  Constructor when material and optical axis are not specified for the
  //shape. They are taken from an other shape. This is useful when many diffe-
  //rent shapes are made of the same material, with the optical axis oriented
  //in the same way.
  const Double_t zero = 0.0;
  Init();
  fSShape = shape;
  fKind   = kind;
  fDamage    = kFALSE;
  fFromOther = kTRUE;
  Bool_t found = kFALSE;
  TString s,s1;
  TSupplShape *pOther;
  TIter next2(&gLit->fSupplShape);
  while ((!found) && (pOther = (TSupplShape *)next2())) {
    s = pOther->GetName();
    if (!s.CompareTo(nameshape)) {
      found = kTRUE;
      fOther = pOther;
    }
  }
  if (!found) {
    s1 = nameshape;
    gMes->SetName("TSupplShape");
    gMes->SetTitle("TSupplShape");
    gMes->M(kill_prog,1,"Reference shape not found",s1);
  }
  fOptMat = fOther->GetOptMat();
  fAnisotropic = fOptMat->GetAnisotropy();
  fAniAbsL     = fOptMat->GetAniAbsL();
  fDielChange  = fOptMat->GetIChangeWithWvl();
  TRevetment *pRev;
  found = kFALSE;
  s="none";
  s1="";
  if ((!s.CompareTo(namerev)) || (!s1.CompareTo(namerev))) {
    found = kTRUE;
    fDefRev = 0;
  }
  else {
    TIter next(&gLit->fRevetment);
    while ((!found) && (pRev = (TRevetment *)next())) {
      s = pRev->GetName();
      if (!s.CompareTo(namerev)) {
        found = kTRUE;
        fDefRev = pRev;
      }
    }
    if (!found) {
      s1 = namerev;
      gMes->SetName("TSupplShape");
      gMes->SetTitle("TSupplShape");
      gMes->M(Error_M,2,"This revetment has not been found. None assumed !",s1);
      fDefRev = 0;
    }
  }
  //Set as not a detector by defaults
  fNDet = -1;
  fDet  = none;
  fIsQE = kFALSE;
  fQE   = 0;
  fDistCatAn = zero;
  SetBevellings(kFALSE); //no bevellings by default
  AddToList(name);
}
TSupplShape::~TSupplShape() {
  //Destructor.
  SetNoDamage();
  fFaces.Delete();
  if (fNbSubFaces) fSubFaces.Delete();
  fPoints->Delete();
  delete fPoints;
  if (gLit) {
    gLit->fSupplShape.Remove(this);
    gLit->fNSupplShape--;
    gLit->fShapeNumbered = kFALSE;
  }
  switch (fKind) {
    case tsbrik:
      ((TSBRIK *)fSShape)->fSuppl = 0;
      break;
    case tstrd1:
      ((TSTRD1 *)fSShape)->fSuppl = 0;
      break;
    case tstrd2:
      ((TSTRD2 *)fSShape)->fSuppl = 0;
      break;
    case tspara:
      ((TSPARA *)fSShape)->fSuppl = 0;
      break;
    case tstrap:
      ((TSTRAP *)fSShape)->fSuppl = 0;
      break;
    case ts8pts:
      ((TS8PTS *)fSShape)->fSuppl = 0;
      break;
    case tsgtra:
      ((TSGTRA *)fSShape)->fSuppl = 0;
      break;
    case tscyl:
      ((TSCYL *)fSShape)->fSuppl = 0;
      break;
    case tstube:
      ((TSTUBE *)fSShape)->fSuppl = 0;
      break;
    case tscone:
      ((TSCONE *)fSShape)->fSuppl = 0;
      break;
  }
  Bool_t found = kFALSE;
  TDetector   *pdet;
  TSupplShape *psup;
  TIter next(&gLit->fDetector);
  switch (fDet) {
    case none:
      break;
    case surfdet:
    case phototube:
    case volumedet:
    case apd:
      while ((!found) && (pdet = (TDetector *)next())) {
        psup = pdet->GetShape();
        if (psup == this) {
          found = kTRUE;
          gLit->fDetector.Remove(pdet);
          delete pdet;
          pdet = 0;
        }
      }
      break;
  }
}
Bool_t TSupplShape::AddToList() const {
  //record this shape into gLit->fSupplShape
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TSupplShape");
    gMes->M(kill_prog,1,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneSupplShape(this,fName);
}
Bool_t TSupplShape::AddToList(const char *name) const {
  //record this shape into gLit->fSupplShape
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TSupplShape");
    gMes->M(kill_prog,2,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneSupplShape(this,name);
}
void TSupplShape::AddSubFace(TFace *f) {
  //Adds a supplementary subface to the shape TSupplShape. A subface is a portion of a master face which is
  //covered with an other type of revetment.
  fNbSubFaces += 1;
  fSubFaces.Add(f);
}
void TSupplShape::AddSubPoint(T3Vector *p) {
  //Adds a supplementary point to the shape TSupplShape. Supplementary points are necessary in order to
  //define the contour of a subface.
  fNbSubPoints += 1;
  fTotalNbPoints += 1;
  fPoints->Add(p);
}
Short_t TSupplShape::AllCrossingPoints(const T3Vector &x0,const T3Vector &nd,
  Double_t &t1,Double_t &t2,Double_t &t3,Double_t &t4,T3Vector &x1,T3Vector &x2,
  T3Vector &x3,T3Vector &x4) const {
  //  Given a straight line going through point x0 and having a direction nd,
  //finds the intersections of this straigth line with the surface of the
  //shape. As the faces of the shapes accepted in Litrani are either plane
  //or of the second order, the maximum possible number of intersection of
  //a straight line with a shape is 4. The return value is the number of
  //intersections found.
  //
  // Parameters :
  //
  // - x0 :  the straight line goes through x0, [cm] [WCS]
  // - nd :  unit vector, direction of straight line x0 + t*nd
  // - ti :  t (of eq. x0 + t*nd), of all possible solutions  [cm]
  // - xi :  all possible solutions
  //
  const Double_t zero  = 0.0;
  const Double_t mdix300 = -1.0e+300;
  Short_t npoints = 0;
  Double_t dist,dist1,dist2,at1,at2;
  Int_t nsoli,nsol;
  T3Vector sol1,sol2;
  TIntercept *tv;
  Bool_t planar;
  Bool_t AllPointsFound = kFALSE;
  TObjArray a(4);
  //Decides on the maximum possible number of points that we can find
  Short_t MaxNbOfPoints;
  switch (fKind) {
  case tstube:
    MaxNbOfPoints = 4;
    break;
  default:
    MaxNbOfPoints = 2;
    break;
  }
  TFace *f;
  TIter next(&fFaces);
  while ((!AllPointsFound) && (f = (TFace *)next())) {
    planar = f->GetPlanar();
    dist = f->CrossFace(x0,nd,at1,sol1,nsol,nsoli,at2,sol2,dist1,dist2,mdix300);
    if (nsol>0) {
      if ((planar) && (nsol>1)) {
        gMes->SetName("AllCrossingPoints");
        gMes->SetTitle("TSupplShape");
        gMes->M(Error_M,1,"planar faces can only have 1 intercept !");
      }
      switch (nsol) {
      case 1:
        if (dist1<=zero) {
          npoints += 1;
          tv = new TIntercept(at1,sol1);
          a.Add(tv);
        }
        break;
      case 2:
        if (dist2<=zero) {
          npoints += 1;
          tv = new TIntercept(at2,sol2);
          a.Add(tv);
        }
        if (dist1<=zero) {
          npoints += 1;
          tv = new TIntercept(at1,sol1);
          a.Add(tv);
        }
        break;
      default:
        gMes->SetName("AllCrossingPoints");
        gMes->SetTitle("TSupplShape");
        gMes->M(Error_M,2,"Absurd number of intercepts of face");
        break;
      }
    }// end if (nsol>0)
    AllPointsFound = (npoints >= MaxNbOfPoints);
  }// end loop on all faces
  if ((npoints==1) || (npoints==3)) {
    gMes->SetName("AllCrossingPoints");
    gMes->SetTitle("TSupplShape");
    gMes->M(Error_M,3,"Absurd number of intercepts of shape");
  }
  TIter nextb(&a);
  a.Sort();
  if (npoints>=1) {
    tv = (TIntercept *)nextb();
    t1 = tv->GetT();
    x1 = tv->GetX();
  }
  if (npoints>=2) {
    tv = (TIntercept *)nextb();
    t2 = tv->GetT();
    x2 = tv->GetX();
  }
  if (npoints>=3) {
    tv = (TIntercept *)nextb();
    t3 = tv->GetT();
    x3 = tv->GetX();
  }
  if (npoints>=4) {
    tv = (TIntercept *)nextb();
    t4 = tv->GetT();
    x4 = tv->GetX();
  }
  a.Delete();
  return npoints;
}
Bool_t TSupplShape::FindContact(TSupplShape *other, Int_t &face1, TFace *&pface1,
  Int_t &face2, TFace *&pface2) {
  //Finds which face of this is in contact with which face of other.
  const Double_t zero  = 0.0;
  const Double_t small = 1.0e-6;
  Double_t test,test1,test2;
  Bool_t contact = kFALSE;
  T3Vector n1, n2;
  T3Vector n(0);
  T3Vector *p1, *p2;
  face1 = -1;
  face2 = -1;
  TIter next1(&fFaces);
  TIter next2(&other->fFaces);
  while ((!contact) && (pface1 = (TFace *)next1())) {
    if (pface1->GetPlanar()) {
      n1 = pface1->Normal();
      while ((!contact) && (pface2 = (TFace *)next2())) {
        if (pface2->GetPlanar()) {
          n2 = pface2->Normal();
          //verify that the normals are opposite
          test1 = n1*n2;
          if (test1<zero) {
            //verify that the normals are aligned
            n  = n1^n2;
            test = n.Module();
            if (test<small) {
              //verify that a point of face2 is in the plane of face 1
              p1 = pface1->Point(0);
              p2 = pface2->Point(0);
              n  = *p2 - *p1;
              test2 = TMath::Abs(n1*n);
              if (test2<small) {
                contact = kTRUE;
                face1   = pface1->GetNumber();
                face2   = pface2->GetNumber();
              }//end if (test2<small)
            }//end if (test<small)
          }//end if (test1<zero)
        }//end if (pface2->GetPlanar())
      }//end while ((!contact) && (pface2 = (TFace *)next2()))
      next2.Reset();
    }//end if (pface1->GetPlanar())
  }//end while ((!contact) && (pface1 = (TFace *)next1()))
  if (!contact) {
    //  If no contact found, look at the possibility of a cylinder or a tube
    //inside a tube.
    Double_t r_in,r_ou,h_ou,ddd;
    T3Vector *x0in,*x1in,*x2in,*x3in;
    T3Vector *x0ou,*x1ou,*x2ou,*x3ou;
    T3Vector v01,v02,ain,aou,aca,p00;
    if (((fKind==tscyl) || (fKind==tstube)) && (other->fKind==tstube)) {
      //First case : "this" cylinder or tube inside "other"
      x0in = GetPoint(0);
      x1in = GetPoint(1);
      x2in = GetPoint(2);
      x3in = GetPoint(3);
      x0ou = other->GetPoint(0);
      x1ou = other->GetPoint(1);
      x2ou = other->GetPoint(2);
      x3ou = other->GetPoint(3);
      v01  = *x1in - *x0in;
      r_in = v01.Module();
      v02  = *x2ou - *x0ou;
      r_ou = v02.Module();
      if (TMath::Abs(r_ou-r_in)<small) {
        //inner and outer radius ok
        ain  = *x3in - *x0in;
        aou  = *x3ou - *x0ou;
        h_ou = aou.Module();
        ain.Unit();
        aou.Unit();
        aca  = ain^aou;
        test = aca.Module();
        if (test<small) {
          //the 2 axis are parallel
          p00 = *x0in - *x0ou;
          ddd = p00.Module();
          if (ddd<small) test = ddd;
          else {
            p00.Unit();
            aca = aou^p00;
            test = aca.Module();
          }
          //pt 0 of inner shape on axis of outer tube
          contact = (test<small);
          if (contact) {
            face1  = 2;
            pface1 = GetFace(face1);
            face2  = 3;
            pface2 = other->GetFace(face2);
          }// if (contact)
        }//if the 2 axis are parallel
      }//if inner and outer radius ok
    }// if (((fKind==tscyl) || (fKind==tstube)) && (other->fKind==tstube))
    if ((!contact) && (
      ((other->fKind==tscyl) || (other->fKind==tstube)) && (fKind==tstube))) {
        //Second case : "other" cylinder or tube inside "this"
        x0in = other->GetPoint(0);
        x1in = other->GetPoint(1);
        x2in = other->GetPoint(2);
        x3in = other->GetPoint(3);
        x0ou = GetPoint(0);
        x1ou = GetPoint(1);
        x2ou = GetPoint(2);
        x3ou = GetPoint(3);
        v01  = *x1in - *x0in;
        r_in = v01.Module();
        v02  = *x2ou - *x0ou;
        r_ou = v02.Module();
        if (TMath::Abs(r_ou-r_in)<small) {
          //inner and outer radius ok
          ain  = *x3in - *x0in;
          aou  = *x3ou - *x0ou;
          h_ou = aou.Module();
          ain.Unit();
          aou.Unit();
          aca  = ain^aou;
          test = aca.Module();
          if (test<small) {
            //the 2 axis are parallel
            p00 = *x0in - *x0ou;
            ddd = p00.Module();
            if (ddd<small) test = ddd;
            else {
              p00.Unit();
              aca = aou^p00;
              test = aca.Module();
            }
            //pt 0 of inner shape on axis of outer tube
            contact = (test<small);
            if (contact) {
              face1  = 3;
              pface1 = GetFace(face1);
              face2  = 2;
              pface2 = other->GetFace(face2);
            }// if (contact)
          }//if the 2 axis are parallel
        }//if inner and outer radius ok
    }// (other->fKind==tscyl) || (other->fKind==tstube)) && (fKind==tstube))
  }// if (!contact)
  return contact;
}
Double_t TSupplShape::GetDamage(const T3Vector *x,Double_t L0) {
  //  Get the value with which the absorption length of the material of
  //the shape has to be multiplied in order to simulate radiation damages.
  //  T3Vector x is the coordinate of the point for which we want the damage
  //factor. See explanation of method RadiationDamage() or of the method
  //ShowerDamage().
  //      z is the distance between the projection of the point x on the  damage
  //        axis fDamAxis and the starting point fDamStart.
  //      r is the distance between point x and the axis of the shower. It is
  //        the radial distance to be taken in the damage or shower
  //        distribution.
  //  L0 is the absorption length without damage
  //
  const Double_t un   = 1.0;
  const Double_t deux = 1.0;
  const Double_t b    = 0.5;
  const Double_t eps  = 1.0e-8;
  Double_t L;  //Absorption length after damage
  Double_t K0; // 1/L0
  Double_t F = un;
  if (fDamage) {
    Double_t z;
    T3Vector d;
    Double_t r,fzr;
    T3Vector R;
    L0 = TMath::Abs(L0);
    if (L0<eps) L0 = eps;
    K0 = un/L0;
    d = *x - fDamStart;
    z = fDamAxis*d;
    R = d^fDamAxis;
    r = R.Module();
    if (fDamByShower) {
      Double_t t,am1,den;
      t    = z/fOptMat->GetX0();
      am1  = fDamA - un;
      den  = r*r + fRk2;
      den *= den;
      fzr  = fDamFac*TMath::Exp(am1*TMath::Log(b*t))*TMath::Exp(-b*t);
      fzr /= den;
    }
    else {
      Double_t a1,a2;
      a2  = r/fDamRs;
      a2 *= a2;
      a2 /= deux;
      if (fDamFitGz) {
        fDamGz = fDamFitGz->V(z);
        fzr    = fDamGz*TMath::Exp(-a2);
      }
      else {
        a1  = (z-fDamZm)/fDamZs;
        a1 *= a1;
        a1 /= deux;
        fzr = TMath::Exp(-a1)*TMath::Exp(-a2);
      }
    }
    L = fDamFmax/(fDamFmax*K0 + fzr*fDamKimax*fDamSupFac);
    F = L/L0;
    if (fHDamage) {
      Int_t kbin;
      kbin = fHDamage->FindBin(z,r);
      fHDamage->SetBinContent(kbin,un-F);
    }
  }//end if (fDamage)
  return F;
}
TFace* TSupplShape::GetFace(Int_t i) const {
  //returns a pointer towards face number i
  TFace *p = 0;
  TIter next(&fFaces);
  Bool_t found = kFALSE;
  while ((!found) && (p = (TFace *)next())) {
    if (i == p->GetNumber()) found = kTRUE;
  }
  if (!found) {
    TIter next1(&fSubFaces);
    while ((!found) && (p = (TFace *)next1())) {
      if (i == p->GetNumber()) found = kTRUE;
    }
  }
  if (!found) p = 0;
  return p;
}
void TSupplShape::Init() {
  // Initial Settings of variables
  const Double_t un       =  1.0;
  const Double_t minusone = -1.0;
  InitP();
  SetNoDamage();
  fFromOther       = kFALSE;
  fNbPoints        = 0;
  fNbSubPoints     = 0;
  fTotalNbPoints   = 0;
  fNbMasterFaces   = 0;
  fNbSubFaces      = 0;
  fAnisotropic     = 0;
  fAniAbsL         = 0;
  fDielChange      = kFALSE;
  fChf             = kFALSE;
  fChfDist         = minusone;
  fChfIsDepolished = kFALSE;
  fChfCosDpAngle   = minusone;
  fNDet            = -1;
  fDet             = none;
  fNFaceA          = -1;
  fNFaceC          = -1;
  fIsQE            = kFALSE;
  fVDrifte         = minusone;
  fDistCatAn       = minusone;
  fShapeNumber     = -1;
  fDamSupFac       = un;
}
void TSupplShape::InitP() {
  //Pointers to 0
  fOther       = 0;
  fSNode       = 0;
  fSShape      = 0;
  fOptMat      = 0;
  fDefRev      = 0;
  fChfRev      = 0;
  fQE          = 0;
  fGP          = 0;
  fDamFitKimax = 0;
  fHDamage     = 0;
  fDamFitGz    = 0;
  fPoints      = 0;
}
Bool_t TSupplShape::IsFluorescent() const {
  //True if the material of the shape is fluorescent
  return fOptMat->IsFluorescent();
}
Bool_t TSupplShape::IsInside(Double_t x, Double_t y, Double_t z) const {
  //Decides whether point x,y,z is inside the shape or not
  //If the shape is still in local coordinates, x,y,z have to be in local coordinates
  //If the shape is in world coordinates, so must be x,y,z.
  T3Vector v(x,y,z);
  Bool_t isin = kTRUE;
  TFace *face;
  TIter next(&fFaces);
  while ((isin) && (face = (TFace *)next())) {
    isin = isin && face->IsInside(v);
  }
  return isin;
}

Bool_t TSupplShape::IsInside(const T3Vector& v) const {
  //Decides whether the point defined by v is inside the shape or not
  //If the shape is still in local coordinates, v has to be in local coordinates
  //If the shape is in world coordinates, v must be too.
  Bool_t isin = kTRUE;
  TFace *face;
  TIter next(&fFaces);
  while ((isin) && (face = (TFace *)next())) {
    isin = isin && face->IsInside(v);
  }
  return isin;
}

void TSupplShape::NewWavelength(Double_t wavelength) {
  //Recalculates all parameters depending upon wavelength.
  if (fDamage) {
    if (fDamFitKimax) {
      fDamKimax = TMath::Abs(fDamFitKimax->V(wavelength));
    }
  }//end if (fDamage)
}
TFace* TSupplShape::NextFace(const TFace *here,const T3Vector &x0,
  const T3Vector &nd,T3Vector &x1, Double_t &distmin,Double_t cut) const {
  //Given a straight line passing through point x0 and having direction nd,
  //finds the next face encountered inside the shape. We assume that the point
  //x0 is inside or on the shape. 
  //
  // Parameters :
  //
  // - here     : here is the face onto which x0 is when calling NextFace. If
  //              x0 is not on a face when calling NextFace, then here is 0.
  // - x0       : the straight line passes through the point x0. x0 + t*nd is
  //              the equation of the straight line.
  // - nd       : unit vector, direction of the straight line.
  // - x1       : point on the new face.
  // - distmin  : negative distance to the nearest ridge.
  // - cut      : value of t below which a solution is NOT accepted. This
  //              parameter is useful for instance in the case where one does
  //              not want to find intersections which are behind the current
  //              point *this. In that case, put tcut=0 !
  //
  const Double_t zero   = 0.0;
  const Double_t mdixm3 = -1.0e-3;
  Bool_t planar;
  Double_t t1,t2;
  Int_t nsoli,nsol;
  T3Vector sol1(0),sol2(0);
  Double_t dist,dist1,dist2;
  distmin = 1.0e+300;
  TFace *f;
  TFace *fkeep = 0;
  TIter next(&fFaces);
  Bool_t found = kFALSE;
  if (fKind != tstube) {
    while ((distmin>=mdixm3) && (f = (TFace *)next())) {
      planar = f->GetPlanar();
      if ((f != here) || ((f == here) && (!planar))) {
        dist = f->CrossFace(x0,nd,t1,sol1,nsol,nsoli,t2,sol2,dist1,dist2,cut);
        if (nsol) {
          if (planar) { //here the face is planar
            if ((t1>zero) && (dist1<distmin)) {
              found   = kTRUE;
              distmin = dist1;
              fkeep   = f;
              x1      = sol1;
            } // end if ((t1>zero) && (dist1<distmin))
          }
          else {        //here the face is curved
            if (nsol==1) {
              if ((t1>zero) && (dist1<distmin)) {
                found   = kTRUE;
                distmin = dist1;
                fkeep   = f;
                x1      = sol1;
              } // end if ((t1>zero) && (dist1<distmin))
            }
            if (nsol==2) {
              if (t2>t1) {
                //this should never happen !
                gMes->SetName("NextFace");
                gMes->SetTitle("TSupplShape");
                gMes->M(Error_M,1,"Solutions in wrong order");
                if ((t2>zero) && (dist2<distmin)) {
                  found   = kTRUE;
                  distmin = dist2;
                  fkeep   = f;
                  x1      = sol2;
                }
              }
              else {
                if ((t1>zero) && (dist1<distmin)) {
                  if (f==here) {
                    found   = kTRUE;
                    distmin = dist1;
                    fkeep   = f;
                    x1      = sol1;
                  }
                  else {
                    found = kTRUE;
                    distmin = dist2;
                    fkeep   = f;
                    x1      = sol2;
                  }
                } // end if ((t1>zero) && (dist1<distmin))
              }//end else if (t2>t1)
            }//end if (nsol==2)
          } //end if (planar)
        } //end if (nsol)
      } //end if ((f != here) || ((f == here) && (!planar)))
    } //end while
  }// end if (fKind != tstube)
  else {
    T3Vector x1keep;
    Double_t distminkeep = -1.0;
    Double_t cutkeep;
    Double_t t;
    Double_t tkeep = 1.0e+100;
    Int_t N = 0;
    while ((N<2) && (f = (TFace *)next())) {
      planar = f->GetPlanar();
      if ((f != here) || ((f == here) && (!planar))) {
        dist = f->CrossFace(x0,nd,t1,sol1,nsol,nsoli,t2,sol2,dist1,dist2,cut);
        if (nsol) {
          if (planar) { //here the face is planar
            if ((t1>zero) && (dist1<distmin)) {
              found   = kTRUE;
              distmin = dist1;
              fkeep   = f;
              x1      = sol1;
              t       = t1;
              if (t<tkeep) {
                N++;
                tkeep       = t;
                distminkeep = distmin;
                cutkeep     = cut;
                x1keep      = x1;
              }
            } // end if ((t1>zero) && (dist1<distmin))
          }
          else {        //here the face is curved
            if (nsol==1) {
              if (t1>zero) {
                found   = kTRUE;
                distmin = dist1;
                fkeep   = f;
                x1      = sol1;
                t       = t1;
                if (t<tkeep) {
                  N++;
                  tkeep       = t;
                  distminkeep = distmin;
                  cutkeep     = cut;
                  x1keep      = x1;
                }
              } // end if ((t1>zero) && (dist1<distmin))
            }
            if (nsol==2) {
              if (t2>t1) {
                //this should never happen !
                gMes->SetName("NextFace");
                gMes->SetTitle("TSupplShape");
                gMes->M(Error_M,1,"Solutions in wrong order");
                if (t2>zero) {
                  found   = kTRUE;
                  distmin = dist2;
                  fkeep   = f;
                  x1      = sol2;
                  t       = t2;
                  if (t<tkeep) {
                    N++;
                    tkeep       = t;
                    distminkeep = distmin;
                    cutkeep     = cut;
                    x1keep      = x1;
                  }
                }
              }
              else {
                if (t1>zero) {
                  if (f==here) {
                    found   = kTRUE;
                    distmin = dist1;
                    fkeep   = f;
                    x1      = sol1;
                    t       = t1;
                    if (t<tkeep) {
                      N++;
                      tkeep       = t;
                      distminkeep = distmin;
                      cutkeep     = cut;
                      x1keep      = x1;
                    }
                  }
                  else {
                    found   = kTRUE;
                    distmin = dist2;
                    fkeep   = f;
                    x1      = sol2;
                    t       = t2;
                    if (t<tkeep) {
                      N++;
                      tkeep       = t;
                      distminkeep = distmin;
                      cutkeep     = cut;
                      x1keep      = x1;
                    }
                  }
                } // end if ((t1>zero) && (dist1<distmin))
              }//end else if (t2>t1)
            }//end if (nsol==2)
          } //end if (planar)
        } //end if (nsol)
      } //end if ((f != here) || ((f == here) && (!planar)))
    } //end while
    x1 = x1keep;
    distmin = distminkeep;
    cut = cutkeep;
  }// end else if (fKind != tstube)
  return fkeep;
}
void TSupplShape::Print() const {
  //prints all characteristics of this TSupplShape.
  Short_t i,j;
  TString s;
  cout << endl;
  cout << "Name of shape                  : " << fName.Data()  << endl;
  cout << "Title of shape                 : " << fTitle.Data() << endl;
  cout << "Kind of shape                  : ";
  switch (fKind) {
  case tsbrik:
    cout << "tsbrik";
    break;
  case tstrd1:
    cout << "tstrd1";
    break;
  case tstrd2:
    cout << "tstrd2";
    break;
  case tspara:
    cout << "tspara";
    break;
  case tstrap:
    cout << "tstrap";
    break;
  case tsgtra:
    cout << "tsgtra";
    break;
  case ts8pts:
    cout << "ts8pts";
    break;
  case tscyl:
    cout << "tscyl";
    break;
  case tstube:
    cout << "tstube";
    break;
  case tscone:
    cout << "tscone";
    break;
  }
  cout << endl;
  cout << "Number of points               : " << fNbPoints      << endl;
  cout << "Number of supplementary points : " << fNbSubPoints   << endl;
  cout << "Center of gravity              : " << fGCenter       << endl;
  cout << "Number of master faces         : " << fNbMasterFaces << endl;
  cout << "Number of subfaces             : " << fNbSubFaces    << endl;
  s = fOptMat->GetName();
  cout << "Material of this shape         : " << s.Data() << endl;
  //s = fDefRev->GetName();
  //cout << "Default revetment ";
  //if (s) cout << s.Data() << endl;
  //else cout << "      None" << endl;
  cout << "Type of optical anisotropy     : " << fAnisotropic << endl;
  if (fAnisotropic) {
    cout << "Diel. tensor change with wvl   : " << Float_t(fDielChange) << endl;
    cout << "Ox axis of dielectric tensor   : " << fAniOx << endl;
    cout << "Oy axis of dielectric tensor   : " << fAniOy << endl;
    cout << "Oz axis of dielectric tensor   : " << fAniOz << endl;
    cout << "Rotation matrix M              : " << endl;
    for (i=0;i<3;i++) {
      cout << "   ";
      for (j=0;j<3;j++) cout << fM(i,j) << "  ";
      cout << endl;
    }
    if (!fDielChange) PrintDiel();
  }
  //Listing
  *gMes->fListing << endl;
  *gMes->fListing << "Name of shape                  : " << fName.Data()  << endl;
  *gMes->fListing << "Title of shape                 : " << fTitle.Data() << endl;
  *gMes->fListing << "Kind of shape                  : ";
  switch (fKind) {
  case tsbrik:
    *gMes->fListing << "tsbrik";
    break;
  case tstrd1:
    *gMes->fListing << "tstrd1";
    break;
  case tstrd2:
    *gMes->fListing << "tstrd2";
    break;
  case tspara:
    *gMes->fListing << "tspara";
    break;
  case tstrap:
    *gMes->fListing << "tstrap";
    break;
  case tsgtra:
    *gMes->fListing << "tsgtra";
    break;
  case ts8pts:
    *gMes->fListing << "ts8pts";
    break;
  case tscyl:
    *gMes->fListing << "tscyl";
    break;
  case tstube:
    *gMes->fListing << "tstube";
    break;
  case tscone:
    *gMes->fListing << "tscone";
    break;
  }
  *gMes->fListing << endl;
  *gMes->fListing << "Number of points               : " << fNbPoints      << endl;
  *gMes->fListing << "Number of supplementary points : " << fNbSubPoints   << endl;
  *gMes->fListing << "Center of gravity              : " << fGCenter       << endl;
  *gMes->fListing << "Number of master faces         : " << fNbMasterFaces << endl;
  *gMes->fListing << "Number of subfaces             : " << fNbSubFaces    << endl;
  s = fOptMat->GetName();
  *gMes->fListing << "Material of this shape         : " << s.Data() << endl;
  //s = fDefRev->GetName();
  //*gMes->fListing << "Default revetment              : " << s.Data() << endl;
  *gMes->fListing << "Type of optical anisotropy     : " << fAnisotropic << endl;
  if (fAnisotropic) {
    *gMes->fListing << "Diel. tensor change with wvl   : " << Float_t(fDielChange) << endl;
    *gMes->fListing << "Ox axis of dielectric tensor   : " << fAniOx << endl;
    *gMes->fListing << "Oy axis of dielectric tensor   : " << fAniOy << endl;
    *gMes->fListing << "Oz axis of dielectric tensor   : " << fAniOz << endl;
    *gMes->fListing << "Rotation matrix M              : " << endl;
    for (i=0;i<3;i++) {
      *gMes->fListing << "   ";
      for (j=0;j<3;j++) *gMes->fListing << fM(i,j) << "  ";
      *gMes->fListing << endl;
    }
    if (!fDielChange) PrintDiel();
  }
}
void TSupplShape::PrintDiel() const {
  //Print the dielectric tensor
  Short_t i,j;
  cout << "Dielectric tensor              : " << endl;
  for (i=0;i<3;i++) {
    cout << "   ";
    for (j=0;j<3;j++) cout << fDTmu(i,j) << "  ";
    cout << endl;
  }
  //Listing
  *gMes->fListing << "Dielectric tensor              : " << endl;
  for (i=0;i<3;i++) {
    *gMes->fListing << "   ";
    for (j=0;j<3;j++) *gMes->fListing << fDTmu(i,j) << "  ";
    *gMes->fListing << endl;
  }
}
void TSupplShape::PrintFace(Int_t i) const {
  //Prints everything on face i
  TFace *f = GetFace(i);
  f->Print();
}
void TSupplShape::PrintPoints() const {
  // Prints the coordinates of all points belonging to the shape. Does not
  //print the supplementary points.
  Int_t i;
  T3Vector *v;
  cout << endl;
  cout << "Coordinates of the " << fNbPoints << " points" << endl;
  cout << endl;
  for (i=0;i<fNbPoints;i++) {
    v = GetPoint(i);
    cout << "point : " << i << *v << endl;
  }
  *gMes->fListing << endl;
  *gMes->fListing << "Coordinates of the " << fNbPoints << " points" << endl;
  *gMes->fListing << endl;
  for (i=0;i<fNbPoints;i++) {
    v = GetPoint(i);
    *gMes->fListing << "point : " << i << *v << endl;
  }
}
Bool_t TSupplShape::RadiationDamage(const T3Vector *damaxis, const T3Vector *damstart,
  Double_t Kimax, Double_t zmean, Double_t zsig, Double_t rsig, Double_t step,
  Double_t damsupfac) {
  // Please read first the chapter "Simulating radiation damages inside a
  //shape" in the class description of this class for a full understanding of
  //how radiation damages are implemented in Litrani.
  //  RadiationDamage gives the possibility of imposing for this shape a
  //radiation damage, i.e. a (z,r) dependent factor affecting the radia-
  //tion length inside this shape.
  //  For a point (x,y,z) in local coordinate of the shape, the position
  //dependent factor is obtained by projecting the point (x,y,z) on the
  //the axis having the direction "damaxis" and going through the point
  //"damstart". The distance z on the axis from this projected point to the
  //starting point "damstart" and the distance r from the point (x,y,z) to
  //the axis damaxis give the couple (z,r) for which the damage is calculated.
  //  The damages are assumed here as having a gaussian shape in z and in r.
  //The gaussian giving the damages in z has a mean zmean and a sigma zsig.
  //The gaussian giving the damages in r is centered on 0 and has a sigma
  //rsig. If the damages have been produced by electromagnetic showers
  //look rather at the method ShowerDamage().
  //  damaxis and damstart are to be given in the local coordinate system of
  //the shape. It means that you have to call RadiationDamage() BEFORE the
  //shape goes into world coordinates, i.e. before the constructor of TSNode
  //for this shape is called, which makes all coordinates to go into world
  //coordinates.
  //  If the distribution of the damages is f(z,r), we have:
  //
  //    f(z,r) = exp(-(z-zmean)^2/(2*zsig^2))exp(-r^2/(2*rsig^2))
  //
  //  Parameters :
  //
  // damaxis   : axis along which damage varies
  // damstart  : starting point for calculating the damages
  // Kimax     : 1/Limin, where Limin is the contribution to the absorption
  //             length of the damages, at the point where it is maximal. In
  //             this instance of ShowerDamage(), Kimax is not taken as depending
  //             upon wavelength.
  // zmean     : depth along damaxis where the damages are at maximum
  // zsig      : gives the width along z of the damages.
  // rsig      : gives the width along r of the damages.
  // step      : steps for calculating absorption [cm]. Default 0.4. Refuses
  //             values smaller than 0.001.
  // damsupfac : possible supplementary constant factor multiplying Ki(lambda), i.e.
  //             increasing the damages if bigger than 1.
  //
  const Double_t un    = 1.0;
  const Double_t small = 0.001;
  gMes->SetName("RadiationDamage");
  gMes->SetTitle("TSupplShape");
  SetNoDamage();
  fDamSupFac = damsupfac;
  step  = TMath::Abs(step);
  if (step<small) step = small;
  fDamage      = kTRUE;
  fDamByShower = kFALSE;
  fDamKimax    = Kimax;
  fDamAxis     = *damaxis;
  fDamAxis.Unit();
  fDamStart    = *damstart;
  fDamZm       = zmean;
  fDamZs       = TMath::Abs(zsig);
  fDamRs       = TMath::Abs(rsig);
  fDamStep     = step;
  fDamFmax     = un;
  return fDamage;
}
Bool_t TSupplShape::RadiationDamage(const T3Vector *damaxis, const T3Vector *damstart,
  const char *namefitkimax, Double_t zmean, Double_t zsig, Double_t rsig,
  Double_t step,Double_t damsupfac) {
  // Please read first the chapter "Simulating radiation damages inside a
  //shape" in the class description of this class for a full understanding of
  //how radiation damages are implemented in Litrani.
  //  RadiationDamage gives the possibility of imposing for this shape a
  //radiation damage, i.e. a (z,r) dependent factor affecting the radia-
  //tion length inside this shape.
  //  For a point (x,y,z) in local coordinate of the shape, the position
  //dependent factor is obtained by projecting the point (x,y,z) on the
  //the axis having the direction "damaxis" and going through the point
  //"damstart". The distance z on the axis from this projected point to the
  //starting point "damstart" and the distance r from the point (x,y,z) to
  //the axis damaxis give the couple (z,r) for which the damage is calculated.
  //  The damages are assumed here as having a gaussian shape in z and in r.
  //The gaussian giving the damages in z has a mean zmean and a sigma zsig.
  //The gaussian giving the damages in r is centered on 0 and has a sigma
  //rsig. If the damages have been produced by electromagnetic showers
  //look rather at the method ShowerDamage().
  //  damaxis and damstart are to be given in the local coordinate system of
  //the shape. It means that you have to call RadiationDamage() BEFORE the
  //shape goes into world coordinates, i.e. before the constructor of TSNode
  //for this shape is called, which makes all coordinates to go into world
  //coordinates.
  //  If the distribution of the damages is f(z,r), we have:
  //
  //    f(z,r) = exp(-(z-zmean)^2/(2*zsig^2))exp(-r^2/(2*rsig^2))
  //
  //  Parameters :
  //
  // damaxis      : axis along which damage varies
  // damstart     : starting point for calculating the damages
  // namefitkimax : name of the fit giving Kimax = 1/Limin, where Limin is the
  //                 contribution to the absorption length of the damages, at the
  //                 point where it is maximal. In this instance of ShowerDamage(),
  //                 Kimax is depending upon wavelength.
  // zmean        : depth along damaxis where the damages are at maximum
  // zsig         : gives the width along z of the damages.
  // rsig         : gives the width along r of the damages.
  // step         : steps for calculating absorption [cm]. Default 0.4. Refuses
  //                 values smaller than 0.001.
  // damsupfac    : possible supplementary constant factor multiplying Ki(lambda), i.e.
  //                increasing the damages if bigger than 1.
  //
  const Double_t un    = 1.0;
  const Double_t small = 0.001;
  gMes->SetName("RadiationDamage");
  gMes->SetTitle("TSupplShape");
  SetNoDamage();
  fDamSupFac = damsupfac;
  if (fDamFitKimax) {
    delete fDamFitKimax;
    fDamFitKimax = 0;
  }
  fDamFitKimax = TSplineFit::FindFit(namefitkimax,15);
  if (fDamFitKimax) {
    fDamage      = kTRUE;
    fDamByShower = kFALSE;
    step  = TMath::Abs(step);
    if (step<small) step = small;
    fDamAxis     = *damaxis;
    fDamAxis.Unit();
    fDamStart    = *damstart;
    fDamZm       = zmean;
    fDamZs       = TMath::Abs(zsig);
    fDamRs       = TMath::Abs(rsig);
    fDamStep     = step;
    fDamFmax     = un;
  }
  else gMes->M(kill_prog,2,"Fit for Kimax not found");
  return fDamage;
}
Bool_t TSupplShape::RadiationDamage(const T3Vector *damaxis, const T3Vector *damstart,
  Double_t Kimax, const char *namefitgz, Double_t rsig, Double_t step,
  Double_t damsupfac) {
  // Please read first the chapter "Simulating radiation damages inside a
  //shape" in the class description of this class for a full understanding of
  //how radiation damages are implemented in Litrani.
  //  RadiationDamage gives the possibility of imposing for this shape a
  //radiation damage, i.e. a (z,r) dependent factor affecting the radia-
  //tion length inside this shape.
  //  For a point (x,y,z) in local coordinate of the shape, the position
  //dependent factor is obtained by projecting the point (x,y,z) on the
  //the axis having the direction "damaxis" and going through the point
  //"damstart". The distance z on the axis from this projected point to the
  //starting point "damstart" and the distance r from the point (x,y,z) to
  //the axis damaxis give the couple (z,r) for which the damage is calculated.
  //  The damages are assumed here as having a gaussian shape in z and in r.
  //The gaussian giving the damages in z has a mean zmean and a sigma zsig.
  //The gaussian giving the damages in r is centered on 0 and has a sigma
  //rsig. If the damages have been produced by electromagnetic showers
  //look rather at the method ShowerDamage().
  //  damaxis and damstart are to be given in the local coordinate system of
  //the shape. It means that you have to call RadiationDamage() BEFORE the
  //shape goes into world coordinates, i.e. before the constructor of TSNode
  //for this shape is called, which makes all coordinates to go into world
  //coordinates.
  //  If the distribution of the damages is f(z,r), we have:
  //
  //  f(z,r) = g(z)*exp(-r^2/(2*rsig^2))   where g(z) is a fit given by the user
  //
  //  Parameters :
  //
  // damaxis   : axis along which damage varies
  // damstart  : starting point for calculating the damages
  // Kimax     : 1/Limin, where Limin is the contribution to the absorption
  //             length of the damages, at the point where it is maximal. In
  //             this instance of ShowerDamage(), Kimax is not taken as depending
  //             upon wavelength.
  // namefitgz : name of the fit giving g(z), the z dependent part of f(z,r)
  // rsig      : gives the width along r of the damages.
  // step      : steps for calculating absorption [cm]. Default 0.4. Refuses
  //             values smaller than 0.001.
  // damsupfac : possible supplementary constant factor multiplying Ki(lambda), i.e.
  //             increasing the damages if bigger than 1.
  //
  const Double_t small = 0.001;
  gMes->SetName("RadiationDamage");
  gMes->SetTitle("TSupplShape");
  SetNoDamage();
  Bool_t upbound = kFALSE;
  fDamSupFac = damsupfac;
  if (fDamFitGz) {
    delete fDamFitGz;
    fDamFitGz = 0;
  }
  fDamFitGz = TSplineFit::FindFit(namefitgz,15);
  if (fDamFitGz) {
    upbound = fDamFitGz->GetUpBound(fDamGzMax);
    if (!upbound) gMes->M(kill_prog,1,"Fit for gz MUST have an upper bound");
    else {
      step  = TMath::Abs(step);
      if (step<small) step = small;
      fDamage      = kTRUE;
      fDamByShower = kFALSE;
      fDamKimax    = Kimax;
      fDamAxis     = *damaxis;
      fDamAxis.Unit();
      fDamStart    = *damstart;
      fDamRs       = TMath::Abs(rsig);
      fDamStep     = step;
      fDamFmax     = fDamGzMax;
    }//end else if (!upbound)
  }//end if (fDamFitGz)
  else gMes->M(kill_prog,2,"Fit for gz not found");
  return fDamage;
}
Bool_t TSupplShape::RadiationDamage(const T3Vector *damaxis, const T3Vector *damstart,
  const char *namefitkimax, const char *namefitgz, Double_t rsig, Double_t step,
  Double_t damsupfac) {
  // Please read first the chapter "Simulating radiation damages inside a
  //shape" in the class description of this class for a full understanding of
  //how radiation damages are implemented in Litrani.
  //  RadiationDamage gives the possibility of imposing for this shape a
  //radiation damage, i.e. a (z,r) dependent factor affecting the radia-
  //tion length inside this shape.
  //  For a point (x,y,z) in local coordinate of the shape, the position
  //dependent factor is obtained by projecting the point (x,y,z) on the
  //the axis having the direction "damaxis" and going through the point
  //"damstart". The distance z on the axis from this projected point to the
  //starting point "damstart" and the distance r from the point (x,y,z) to
  //the axis damaxis give the couple (z,r) for which the damage is calculated.
  //  The damages are assumed here as having a gaussian shape in z and in r.
  //The gaussian giving the damages in z has a mean zmean and a sigma zsig.
  //The gaussian giving the damages in r is centered on 0 and has a sigma
  //rsig. If the damages have been produced by electromagnetic showers
  //look rather at the method ShowerDamage().
  //  damaxis and damstart are to be given in the local coordinate system of
  //the shape. It means that you have to call RadiationDamage() BEFORE the
  //shape goes into world coordinates, i.e. before the constructor of TSNode
  //for this shape is called, which makes all coordinates to go into world
  //coordinates.
  //  If the distribution of the damages is f(z,r), we have:
  //
  //    f(z,r) = g(z)*exp(-r^2/(2*rsig^2))
  //
  //  Parameters :
  //
  // damaxis      : axis along which damage varies
  // damstart     : starting point for calculating the damages
  // namefitkimax : name of the fit giving Kimax = 1/Limin, where Limin is the
  //                 contribution to the absorption length of the damages, at the
  //                 point where it is maximal. In this instance of ShowerDamage(),
  //                 Kimax is depending upon wavelength.
  // namefitgz    : name of the fit giving g(z), the z dependent part of f(z,r)
  // rsig         : gives the width along r of the damages.
  // step         : steps for calculating absorption [cm]. Default 0.4. Refuses
  //                 values smaller than 0.001.
  // damsupfac    : possible supplementary constant factor multiplying Ki(lambda), i.e.
  //                increasing the damages if bigger than 1.
  //
  const Double_t small = 0.001;
  gMes->SetName("RadiationDamage");
  gMes->SetTitle("TSupplShape");
  SetNoDamage();
  fDamSupFac = damsupfac;
  Bool_t upbound = kFALSE;
  if (fDamFitGz) {
    delete fDamFitGz;
    fDamFitGz = 0;
  }
  fDamFitGz = TSplineFit::FindFit(namefitgz,15);
  if (fDamFitGz) {
    upbound = fDamFitGz->GetUpBound(fDamGzMax);
    if (!upbound) gMes->M(kill_prog,1,"Fit for gz MUST have an upper bound");
    else {
      if (fDamFitKimax) {
        delete fDamFitKimax;
        fDamFitKimax = 0;
      }
      fDamFitKimax = TSplineFit::FindFit(namefitkimax,15);
      if (fDamFitKimax) {
        fDamage      = kTRUE;
        fDamByShower = kFALSE;
        step  = TMath::Abs(step);
        if (step<small) step = small;
        fDamAxis     = *damaxis;
        fDamAxis.Unit();
        fDamStart    = *damstart;
        fDamRs       = TMath::Abs(rsig);
        fDamStep     = step;
        fDamFmax     = fDamGzMax;
      }//end if (fDamFitKimax)
      else gMes->M(kill_prog,2,"Fit for Kimax not found");
    }//end else if (!upbound)
  }//end if (fDamFitGz)
  else gMes->M(kill_prog,3,"Fit for g(z) not found");
  return fDamage;
}
void TSupplShape::RecordDamages(Int_t nz,Double_t zmax,Int_t nr,Double_t rmax) {
  // If you call this method before any generation, each damage factor generated
  //will be recorded into a 2D histogram. The histogram will not be filled by
  //call to Fill() [it is not the number of hits which interests us], the value
  //of 1 - damage factor will be put as the content of the bin, such that the
  //histogram will show the calculated distribution for 1-damage factor, not
  //the number of hits. After all generations, you can call ShowDamages() to see
  //this histogram.
  // Call this method only as a check. Do not use it in production. It is
  //costly in time.
  //
  // Arguments:
  //
  //  nz    : number of channels in z (coord. along damaxis from damstart)
  //  zmax  : maximum z recorded. zmin is always 0
  //  nr    : number of channels in r (distance to damaxis)
  //  rmax  : maximum r recorded. rmin is always 0
  //
  //
  const Double_t zero = 0.0;
  fHDamage = new TH2D("Damages","1 minus damage factor as a function of (z,r)",
    nz,zero,zmax,nr,zero,rmax);
}
Bool_t TSupplShape::SameMedium(TSupplShape *pSup) const {
  //  Finds whether this shape and shape pSup have the same material and the
  //same optical axis.
  Bool_t b = kFALSE;
  TSupplShape *other2;
  if (fFromOther) {
    if (fOther->IsFromOther()) {
      other2 = fOther->GetOther();
      if (fOther == other2) b = kTRUE;
    }
    else {
      if (fOther == pSup) b = kTRUE;
    }
  }
  else {
    if (pSup->IsFromOther()) {
      other2 = pSup->GetOther();
      if (this == other2) b = kTRUE;
    }
  }
  return b;
}
void TSupplShape::SetAPD(const char *name,const char *title,Int_t faceA, Int_t faceC,
  const char *nameGP,Double_t threshe, Double_t vdrifte) {
  // Declares that this shape is an APD, an Avalanche PhotoDiode. An APD is a more
  //complicated volume detector. It must be associated with a TSCYL or a TSBRIK.
  //2 parallel faces of the shape must be defined as cathode (entrance) and anode
  //(back face). A distribution function (of class TSplineFit) gives the number of
  //electrons generated by the stopping of the photon inside the volume at a
  //coordinate z with respect to the axis going from cathode to anode. This
  //distribution function is what is called the gain profile of the APD. To be
  //meaningful, a gain profile must have a qualitative shape defining 6 regions :
  //    (1) - a first region named "dead zone", where the electric field is 0
  //      and consequently the gain is 0. Width of this region : ~200 nm.
  //    (2) - a second region where the gain is rapidly rising to its maximal
  //      value ( of the order of 60). Width of this region : ~250 nm.
  //    (3) - a third region where the gain approximatly stays at its maximal
  //      value ( of the order of 60). Width of this region : ~2500 nm.
  //    (4) - a fourth region where the gain drops towards a value close to 2.
  //      Width of this region : ~5000 nm.	
  //    (5) - a fifth region where the gain almost stays at 2.
  //      Width of this region : ~90000 nm.
  //    (6) - a sixth region where it drops to zero.
  //    	Total Width of APD : ~100000nm.
  // Take these considerations into account when you define the interpolation (never
  //use a fit for this kind of curve, it will always be bad. Always use a linear
  //interpolation) of type TSplineFit giving the gain profile. Do not forget
  //that for this gain profile, you have to give the coordinate z in nanometers
  //and not in cm (contrary to the shapes which are defined in cm).
  // Notice that if the shape is a tsbrik, the 2 faces can only be :
  //(4,5) or (0,2) or (1,3) in order to be opposite and parralel. If you
  //do not understand why, look at the documentation about class TSBRIK.
  //If the shape is a TSCYL, the 2 faces can only be (0,1). If you
  //do not understand why, look at the documentation about class TSCYL.
  //
  //  The arguments are the following :
  //
  //     name    : name of the detector.  It is necessary to give a name to each
  //                detector. 2 different detectors must have 2 different names.
  //     title   : title of a detector.
  //     faceA   : the face number of the face acting as cathode (entrance)
  //     faceC   : the face number of the face acting as anode (back face)
  //               these two faces must be parallel. Look at the documentation
  //               about the respective shape to know which face has which face
  //               number.
  //     nameGP  : the name of the fit or interpolation, of type TSplineFit giving
  //               the gain profile of the APD.
  //     threshe : the threshold number of emitted electrons needed to consider
  //               the photon as seen by the APD. If you do not give this argument,
  //               the default value of 5.0 contained in gLitPhys will be taken.
  //               The photon is considered as detected if it has stopped at a z
  //               coordinate such that at least nthreshe (default 5) electrons
  //               have been emitted. In fact, what really matters here is not
  //               whether the photon has been detected or not (has emitted more
  //               or less than nthreshe electrons), but how many electrons have
  //               been emitted and at what time they have reached the anode.
  //               This value is recorded into an histogram which gives directly
  //               the shape of the pulse emitted by the APD after the emission
  //               of all photons of the run. 
  //     vdrifte : the speed of electrons inside the APD (~100000 cm/s). This pa-
  //               rameter is not very important because this drift velocity is
  //               extremely fast and so does not affect very much the shape of
  //               the pulse given by the APD. You can omit it, in that case, the
  //               velocity of 100000 cm/s will be taken.
  //
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t small  = 1.0e-5;
  const Double_t fourth = 0.25;
  const Double_t dixp7  = 1.0e+7;
  TDetector *pdet;
  Double_t y;
  T3Vector x0(0),x1(0),x2(0),x3(0);
  T3Vector nA, nB, *v;
  TFace *pfA,*pfB;
  fDet    = apd;
  fNameGP = nameGP;
  fNFaceA = faceA;
  fNFaceC = faceC;
  switch (fKind) {
    case tsbrik:
      //verifies that cathode and anode are opposite and parralel
      pfA = GetFace(faceA);
      pfB = GetFace(faceC);
      nA  = pfA->Normal();
      nB  = pfB->Normal();
      y = -(nA*nB);
      y = TMath::Abs(un-y);
      if (y>small) {
        gMes->SetName("SetAPD");
        gMes->SetTitle("TSupplShape");
        gMes->M(Error_M,1,"cathode and anode not parralel");
      }
      fAxisAPD = nB;
      //calculates the center of gravity of the cathode
      v  = pfA->Point(0);
      x0 = *v;
      v  = pfA->Point(1);
      x1 = *v;
      v  = pfA->Point(2);
      x2 = *v;
      v  = pfA->Point(3);
      x3 = *v;
      fx0APD = fourth*(x0+x1+x2+x3);
      //calculates the center of gravity of the anode
      v  = pfB->Point(0);
      x0 = *v;
      v  = pfB->Point(1);
      x1 = *v;
      v  = pfB->Point(2);
      x2 = *v;
      v  = pfB->Point(3);
      x3 = *v;
      fx1APD = fourth*(x0+x1+x2+x3);
      x0 = fx1APD - fx0APD;
      fDistCatAn = dixp7*x0.Module();
      break;
    case tscyl:
      //verifies that cathode and anode are opposite and parralel
      pfA = GetFace(faceA);
      pfB = GetFace(faceC);
      nA  = pfA->Normal();
      nB  = pfB->Normal();
      y = -(nA*nB);
      y = TMath::Abs(un-y);
      if (y>small) {
        gMes->SetName("SetAPD");
        gMes->SetTitle("TSupplShape");
        gMes->M(Error_M,2,"cathode and anode not parralel");
      }
      fAxisAPD = nB;
      v        = pfA->Point(0);
      fx0APD = *v;
      v        = pfB->Point(0);
      fx1APD  = *v;
      x0 = fx1APD - fx0APD;
      fDistCatAn = dixp7*x0.Module();
      break;
    default:
      gMes->SetName("SetAPD");
      gMes->SetTitle("TSupplShape");
      gMes->M(Error_M,3,"Shape must be tsbrik or tscyl");
      break;
  }
  fGP = TSplineFit::FindFit(nameGP,13);
  if (!fGP) {
    TString s = nameGP;
    gMes->SetName("SetAPD");
    gMes->SetTitle("TSupplShape");
    gMes->M(kill_prog,4,"Fit on gain profile not found",s);
  }
  if (threshe>zero) TLitPhys::Get()->SetThreshe(threshe);
  fVDrifte = vdrifte;
  pdet = new TDetector(name,title,apd,this);
}
void TSupplShape::SetBevellings(Bool_t chf, Double_t dist, const char *namerev,
  Double_t ang) {
  // Declares that this shape has bevellings (chamfers). It is a simple-
  //minded simulation of bevellings. It has just the effect of defining a
  //distance from the line of intersection of 2 faces (ridge) below which the
  //photon does not see the normal revetment of the face, but the revetment
  //rev. This portion of the face near the ridge may also be depolished, on
  //the condition that the material of the shape is not anisotropic.
  //  This method has the advantage of  being extremely simple for the user to
  //see immediatly what kind of effects is to be expected from bevellings,
  //without setting up a complicate geometrical set-up.
  //However it has to be reminded that :
  // - on the geometrical point of view, it is not a true simulation of
  //bevellings, since using this method does not generate the right form of
  //bevellings (i.e thin faces at 45° of the 2 adjacent faces).
  // - it is not possible to declare that some ridges are bevelled, some
  //other not. Either all ridges of the shape have bevellings, or none.
  //  If you want a more elaborate simulation of bevellings, you cannot avoid
  //to do it yourself by constructing, using shapes like TSTRD1, TSTRD2,
  //TSTRAP, ... the true geometrical set-up with bevellings.
  //
  // Parameters :
  //
  // - chf     : if true, the shape has bevellings.
  // - dist    : below dist, the photon is considered to be on the portion of
  //             the shape which simulate the bevelling. dist is the distance
  //             in cm from the nearest ridge of the shape.
  // - namerev : name of the revetment of the portion of the shape simulating
  //             the bevellings.
  // - ang     : [degree, default 0] if you give to ang a value greater than
  //             0, the portion of the face simulating the bevellings is in
  //             addition considered as depolished. ang is the maximum depo-
  //             lish angle. See the method TFace::SetDepolished to know
  //             what the maximum depolish angle is and to know how Litrani
  //             handles depolished surfaces. If the material of the shape
  //             is optically anisotropic, you have not the right to ask for
  //             depolished surface.
  //
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  const Double_t small = 1.0e-6;
  const Double_t pideg = 180.0;
  fChf = chf;
  if (fChf) {
    fChfDist = TMath::Abs(dist);
    TRevetment *pRev;
    TString s="none";
    TString s1="";
    Bool_t found = kFALSE;
    if ((!s.CompareTo(namerev)) || (!s1.CompareTo(namerev))) {
      found = kTRUE;
      fChfRev = 0;
    }
    else {
      TIter next(&gLit->fRevetment);
      while ((!found) && (pRev = (TRevetment *)next())) {
        s = pRev->GetName();
        if (!s.CompareTo(namerev)) {
          found = kTRUE;
          fChfRev = pRev;
        }
      }
      if (!found) {
        s1 = namerev;
        gMes->SetName("HasBevellings");
        gMes->SetTitle("TSupplShape");
        gMes->M(Error_M,1,"This revetment not found. None assumed !",s1);
        fChfRev = 0;
      }
    }
    ang = TMath::Abs(ang);
    if (ang>small) {
      if (!fOptMat->GetAnisotropy()) {
        fChfIsDepolished = kTRUE;
        ang = (ang*TMath::Pi())/pideg;
        fChfCosDpAngle = TMath::Cos(ang);
      }
      else {
        gMes->SetName("HasBevellings");
        gMes->SetTitle("TSupplShape");
        gMes->M(Error_M,2,"Depolish not allowed when anisotropy");
        fChfIsDepolished = kFALSE;
        fChfCosDpAngle = un;
      }
    }
    else {
      fChfIsDepolished = kFALSE;
      fChfCosDpAngle = un;
    }
  }
  else {
    fChfDist = zero;
    fChfRev  = 0;
    fChfIsDepolished = kFALSE;
    fChfCosDpAngle = un;
  }
}
void TSupplShape::SetDielTensor() {
  //  Calculates the dielectric tensor when rotation matrix M is known. The
  //dielectric tensor is depending upon wavelength, which may change the
  //values ea,eb and ec. So it has to be recalculated for each new photon if
  //fDielChange is true.
  //  Notice that fDTmu is not the dielectric tensor : it is the dielectric
  //tensor multiplied by the magnetic permeability !
  //
  if (fAnisotropic) {
    if (fFromOther) {
      fM = *(fOther->GetfM());
      fMt.TransposedOf(fM);
    }
    T33Matrix diag;
    Double_t muea,mueb,muec,mu;
    mu = fOptMat->GetMu();
    muea = mu*fOptMat->GetEpsa();
    mueb = mu*fOptMat->GetEpsb();
    muec = mu*fOptMat->GetEpsc();
    diag.Diagonal(muea,mueb,muec);
    fDTmu = fM*diag*fMt;
    if (fAniAbsL) {
      Double_t muphia,muphib,muphic;
      muphia = - fOptMat->GetPhimua();
      muphib = - fOptMat->GetPhimub();
      muphic = - fOptMat->GetPhimuc();
      TSComplex c00(muea,muphia);
      TSComplex c11(mueb,muphib);
      TSComplex c22(muec,muphic);
      T33CMatrix cdiag;
      cdiag.Diagonal(c00,c11,c22);
      fCDTmu = fM*cdiag*fMt;
    }
  }
}
void TSupplShape::SetM() {
  //  Defines matrix M which allows to calculate the dielectric tensor fDTmu once
  //the 3 diagonal elements ea, eb and ec are known.
  if (fAnisotropic) {
    if (fFromOther) {
      T33Matrix *m;
      m = fOther->GetfM();
      fM = *m;
    }
    else {
      fM.SetColumn(0,fAniOx(0),fAniOx(1),fAniOx(2));
      fM.SetColumn(1,fAniOy(0),fAniOy(1),fAniOy(2));
      fM.SetColumn(2,fAniOz(0),fAniOz(1),fAniOz(2));
    }
    fMt.TransposedOf(fM);
    if (!fDielChange) SetDielTensor();
  }
} 
void TSupplShape::SetNoDamage() {
  //  Suppress radiation damage. We do not delete fits. They will be deleted
  //by TSplineFit::PurgeStatic() in the destructor of TLitGlob.
  const Double_t minusone = -1.0;
  fDamage      = kFALSE;
  fDamKimax    = minusone;
  fDamFmax     = minusone;
  fDamStep     = 0.4;
  fDamZm       = minusone;
  fDamZs       = minusone;
  fDamRs       = minusone;
  fDamGz       = minusone;
  fDamGzMax    = minusone;
  fDamByShower = kFALSE;
  fDamE0       = minusone;
  fDamGamma    = kTRUE;
  fDamA        = minusone;
  fRk2         = minusone;
  fDamFac      = minusone;
  if (fHDamage) {
    delete fHDamage;
    fHDamage = 0;
  }
  // Do not do:   delete fDamFitKimax;
  fDamFitKimax = 0;
  // Do not do:   delete fDamFitGz;
  fDamFitGz = 0;
}
void TSupplShape::SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay,
  Double_t phiy) {
  //  Sets the 3 optical axis fAniOx, fAniOy, fAniOz. The 3 optical axis are
  //the 3 axis of the coordinate system in which the dielectric tensor is
  //diagonal, with eigenvalues
  //
  //   ea   along the Ox axis
  //   eb   along the Oy axis
  //   ec   along the Oz axis
  //
  //   ea <  eb <  ec  for totally anisotropic crystals (==birefringent)
  //   ea <  eb =  ec  for "uniaxial negativ birefringent" crystals
  //   ea =  eb <  ec  for "uniaxial positiv birefringent" crystals
  //
  //  These 3 axis are given "a la GEANT" : give 
  //   theta and phi of fAniOx axis   (thetax,phix)
  //   theta and phi of fAniOy axis   (thetay,phiy)
  //  It is not necessary to give theta and phi of fAniOz : fAniOz can be
  //determined with fAniOx and fAniOy. This way of giving a coordinate system
  //inside the local coordinate system is intuitive, but inelegant, because it
  //is redundant. If you give an fAniOy axis which is not orthogonal to the
  //fAniOx axis, you will be warned and your fAniOy axis will be corrected.
  //
  //  GIVE ALL ANGLES IN DEGREE !
  //
  //  To understand the construction of the dielectric tensor both in Local
  //Coordinate System and in the World Coordinate system, read the general
  //description of the class TSupplShape.
  //  This method must NOT be called if the shape is declared to have material
  //and optical axis from an other shape (fFromOther = kTRUE).
  //
  if (fFromOther) {
    gMes->SetName("SetOpticalAxis");
    gMes->SetTitle("TSupplShape");
    gMes->M(Error_M,0,
      "Do not call SetOpticalAxis when material taken from other shape");
  }
  else {
    const Double_t zero   = 0.0;
    const Double_t limit  = 1.0e-6;
    const Double_t degrad = 0.0174532925199432958;
    const Double_t wsmall = 1.0e-16;
    Bool_t problem = kFALSE;
    Double_t test;
    Double_t sthetax, cthetax, sthetay, cthetay;
    Double_t sphix, cphix, sphiy, cphiy;
    T3Vector yv(0),zv(0);
    thetax *= degrad;  thetay *= degrad;
    phix   *= degrad;  phiy   *= degrad;
    sthetax = TMath::Sin(thetax);  cthetax = TMath::Cos(thetax);
    sthetay = TMath::Sin(thetay);  cthetay = TMath::Cos(thetay);
    sphix   = TMath::Sin(phix);    cphix   = TMath::Cos(phix);
    sphiy   = TMath::Sin(phiy);    cphiy   = TMath::Cos(phiy);
    if (TMath::Abs(sthetax)<wsmall) sthetax = zero;
    if (TMath::Abs(cthetax)<wsmall) cthetax = zero;
    if (TMath::Abs(sthetay)<wsmall) sthetay = zero;
    if (TMath::Abs(cthetay)<wsmall) cthetay = zero;
    if (TMath::Abs(sphix)<wsmall) sphix = zero;
    if (TMath::Abs(cphix)<wsmall) cphix = zero;
    if (TMath::Abs(sphiy)<wsmall) sphiy = zero;
    if (TMath::Abs(cphiy)<wsmall) cphiy = zero;
    fAniOx(0) = sthetax*cphix;
    fAniOx(1) = sthetax*sphix;
    fAniOx(2) = cthetax;
    fAniOy(0) = sthetay*cphiy;
    fAniOy(1) = sthetay*sphiy;
    fAniOy(2) = cthetay;
    fAniOx.Unit();
    fAniOy.Unit();
    //verification of orthogonality of fAniOy with fAniOx
    zv = fAniOx^fAniOy;
    zv.Unit();
    yv = zv^fAniOx;
    yv.Unit();
    if (TMath::Abs(yv(0))<wsmall) yv(0) = zero;
    if (TMath::Abs(yv(1))<wsmall) yv(1) = zero;
    if (TMath::Abs(yv(2))<wsmall) yv(2) = zero;
    test = TMath::Abs(fAniOx*fAniOy);
    if (test>limit) {
      problem = kTRUE;
      gMes->SetName("SetOpticalAxis");
      gMes->SetTitle("TSupplShape");
      gMes->M(Error_M,1,"The given y axis is not orthogonal to the x axis",test);
      gMes->M(Info_M,1,"We change it !");
    }
    fAniOy = yv;
    zv = fAniOx^fAniOy;
    zv.Unit();
    fAniOz = zv;
    if (problem) {
      gMes->SetName("SetOpticalAxis");
      gMes->SetTitle("TSupplShape");
      gMes->M(Warning_M,2,"Values taken for the optical axis :");
      gMes->M(Info_M,2,"fAniOx",fAniOx);
      gMes->M(Info_M,2,"fAniOy",fAniOy);
      gMes->M(Info_M,2,"fAniOz",fAniOz);
    }
    if (!fAnisotropic) {
      gMes->SetName("SetOpticalAxis");
      gMes->SetTitle("TSupplShape");
      gMes->M(Warning_M,3,"Why defining optical axis for an isotropic media ?");
    }
    SetM();
  }
}
void TSupplShape::SetPhotoTube(const char *name,const char *title,Int_t nface,
  const char *nameqe) {
  // Declares that this shape, hopefuly of type TSCYL, is a phototube.
  //A phototube is a special case of a surface detector, with the following
  //restrictions : the associated shape is a TSCYL there is a condition of
  //cathode efficiency (nameqe cannot be "" nor "none"), there is no condition
  //about acceptance angle.
  // If it ever happens to you that your phototube has not the shape of a cylinder,
  //no problem : define it as a surfdet using SetSurfDet.
  // Notice that the length of the cylinder is not the length of the phototube
  //from its entrance windows until its basis ! it is the length from its entrance
  //window until the photocathode.
  //  The arguments are the following :
  //
  //     name    : name of the detector.  It is necessary to give a name to each
  //                detector. 2 different detectors must have 2 different names.
  //     title   : title of a detector.
  //     nface   : face of the shape which IS the photocathode.
  //     nameqe  : cathode efficiency. The photocathode has a quantum efficiency
  //               which is depending upon wavelength of the photon. This
  //               quantum efficiency is described by a fit of class TSplineFit
  //               whose name is nameqe.
  //
  //
  //   Important notice concerning surface detector : if you declare that some
  //face of a shape is a detector, (by a call to SetSurfDet() or SetPhotoTube()),
  //it is the whole surface of the face declared in argument which is a detector.
  //Is it possible to have a face for which only a portion of the surface is a
  //detector ? Is it possible to place many detectors on one face ? The answer
  //is NO, but it is NOT AT ALL a restriction ! Nothing prevents you to create
  //onto the face one or many subfaces and to make a call to SetSurfDet() or
  //SetPhototube() for each subface. See the description of the class TFace to
  //learn how to create subfaces.
  //   
  SetSurfDet(name,title,nface,nameqe,91.0,phototube);
}
void TSupplShape::SetSurfDet(const char *name,const char *title,Int_t nface,
  const char *nameqe,Double_t accept,KindOfDetector det) {
  // Declares that one of the faces or subfaces of this shape is a surface
  //detector. To know what a subface is, look at the documentation about the
  //class TFace. For a surface detector, the photon is considered as detected
  //if it reaches a face or a subface of the shape, possibly under the following
  //2 conditions.
  //   (1) - nameqe : cathode efficiency. The sensitive face or subface has a
  //quantum efficiency which is depending upon wavelength of the photon. This
  //quantum efficiency is described by a fit of class TSplineFit whose name is
  //nameqe. If in your detector, there is no quantum efficiency, i-e photon are
  //always detected when reaching the face or subface, whatever their wavelength
  //then do not give this parameter, or define it to be "" or "none".
  //   (2) - accept : acceptance angle of the photon in degree. The photon will
  //be considered as detected if it reaches the sensitive face or subface with
  //an angle smaller than accept. If in your detector there is no condition
  //about acceptance angle, either do not give this argument, or give an angle
  //greater or equal to 90 degree.
  // Notice that a SurfDet associated with a TSCYL, without acceptance angle
  //and with a quantum efficiency is a phototube. In that case, use rather method
  //SetPhototube.
  //  The arguments are the following :
  //
  //     name    : name of the detector.  It is necessary to give a name to each
  //                detector. 2 different detectors must have 2 different names.
  //     title   : title of a detector.
  //     nface   : face of the shape which IS a surface detector.
  //     nameqe  : see above
  //     accept  : see above
  //     det     : do not give this argument.
  //
  //
  //   Important notice concerning surface detector : if you declare that some
  //face of a shape is a detector, (by a call to SetSurfDet() or SetPhotoTube()),
  //it is the whole surface of the face declared in argument which is a detector.
  //Is it possible to have a face for which only a portion of the surface is a
  //detector ? Is it possible to place many detectors on one face ? The answer
  //is NO, but it is NOT AT ALL a restriction ! Nothing prevents you to create
  //onto the face one or many subfaces and to make a call to SetSurfDet() or
  //SetPhototube() for each subface. See the description of the class TFace to
  //learn how to create subfaces.
  //   
  const Double_t zero  = 0.0;
  const Double_t deux  = 2.0;
  const Double_t pis2d = 90.0;
  const Double_t pid   = 180.0;
  Bool_t isqe;
  TSplineFit *qe = 0;
  TFace *pface=0;
  Int_t n;
  TDetector *pdet;
  TString s;
  n = fNbMasterFaces + fNbSubFaces;
  if (nface<n) {
    TString s1 = "";
    TString s2 = "none";
    isqe = !(!s1.CompareTo(nameqe) || !s2.CompareTo(nameqe));
    if (isqe) {
      qe = TSplineFit::FindFit(nameqe,10);
      if (!qe) {
        gMes->SetName("SetSurfDet");
        gMes->SetTitle("TSupplShape");
        gMes->M(Error_M,1,"fit on quantum efficiency not found : ");
        gMes->M(Info_M,1,nameqe);
      }
    }
    if ((accept>=pis2d) || (accept<=zero)) accept   = TMath::Pi()/deux;
    else accept   = (accept*TMath::Pi())/pid;
    pface = GetFace(nface);
    pface->SetAsDetector(det,qe,accept);
    pdet = new TDetector(name,title,det,this,pface);
  }
  else {
    gMes->SetName("SetSurfDet");
    gMes->SetTitle("TSupplShape");
    gMes->M(kill_prog,2,"non existing face",nface);
  }
}
void TSupplShape::SetVolumeDet(const char *name,const char *title,const char *nameqe) {
  // Declares that this shape is a volume detector. In that case, a photon is
  //detected if it stops inside the volume of the shape. One can associate an
  //equivalent of photocathode efficiency, a "volume efficiency", depending upon
  //wavelength.This quantum efficiency is described by a fit of class TSplineFit
  //whose name is nameqe.
  //  The arguments are the following :
  //
  //     name    : name of the detector.  It is necessary to give a name to each
  //                detector. 2 different detectors must have 2 different names.
  //     title   : title of a detector.
  //     nameqe  : volume efficiency depending upon wavelength of photon
  //
  TDetector *pdet;
  fDet  = volumedet;
  fIsQE = kFALSE;
  fQE   = 0;
  TString s1 = "";
  TString s2 = "none";
  fNameQE = "";
  fIsQE = !(!s1.CompareTo(nameqe) || !s2.CompareTo(nameqe));
  if (fIsQE) {
    fQE = TSplineFit::FindFit(nameqe,10);
    if ((fIsQE) && (!fQE)) {
      TString s = nameqe;
      gMes->SetName("SetVolumeDet");
      gMes->SetTitle("TSupplShape");
      gMes->M(Error_M,1,"fit on quantum efficiency not found",s);
      gMes->M(Warning_M,1,"No quantum efficiency will be used");
    }
    else fNameQE = nameqe;
  }
  pdet = new TDetector(name,title,volumedet,this);
}
void TSupplShape::ShowDamage() const {
  // Shows calculated distribution 1 - damage factor
  if (fDamage) {
    if (fHDamage) {
      fHDamage->Draw("LEGO2");
      gPad->Modified();
      gPad->Update();
      gTwoPad->ChangePad();
    }
  }
}
Bool_t TSupplShape::ShowerDamage(const T3Vector *damaxis, const T3Vector *damstart,
  Double_t Kimax, Double_t E0, Bool_t gamma, Double_t step,Double_t damsupfac) {
  // Please read first the chapter "Simulating radiation damages inside a
  //shape" in the class description of this class for a full understanding of
  //how radiation damages are implemented in Litrani.
  // ShowerDamage() is an alternative to RadiationDamage() to be preferred when
  //the damages have been caused by the irradiation by electromagnetic
  //showers, gammas or electrons. ShowerDamage() will generate damages with
  //the same (z,r) distributions as the showers which have caused the damages.
  // The chosen (z,r) distribution is the one which is described in the
  //class description of TElecCascade, and which is also described in the
  //Review of Particle Physics, Vol 3 N°1-4, 1998 pages 148-149.
  // In order that ShowerDamage() can be called for this shape, it is necessary
  //that radiation length, critical energy and Moliere radius have been
  //defined for the material of this shape. It means that you have called 
  //TOpticMaterial::SetCascades(),
  //  For a point (x,y,z) in local coordinate of the shape, the position
  //dependent factor is obtained by projecting the point (x,y,z) on the
  //the axis having the direction "damaxis" and going through the point
  //"damstart". The distance z on the axis from this projected point to the
  //starting point "damstart" and the distance r from the point (x,y,z) to
  //the axis damaxis give the couple (z,r) for which the damage is calculated.
  //  damaxis and damstart are to be given in the local coordinate system of
  //the shape. It means that you have to call ShowerDamage() BEFORE the
  //shape goes into world coordinates, i.e. before the constructor of TSNode
  //for this shape is called, which makes all coordinates to go into world
  //coordinates.
  //
  //  Parameters :
  //
  // damaxis   : axis along which damage varies
  // damstart  : starting point for calculating the damages == starting point
  //             of the cascades having produced the damages.
  // Kimax     : 1/Limin, where Limin is the contribution to the absorption
  //             length of the damages, at the point where it is maximal. In
  //             this instance of ShowerDamage(), Kimax is not taken as depending
  //             upon wavelength.
  // E0        : energy in [Gev] of the damaging cascades
  // gamma     : if true, damaging cascades are gammas. If false : electrons
  // step      : steps for calculating absorption [cm]. Default 0.4. Refuses
  //             values smaller than 0.001.
  // damsupfac    : possible supplementary constant factor multiplying Ki(lambda), i.e.
  //                increasing the damages if bigger than 1.
  //
  const Double_t zero = 0.0;
  const Double_t b    = 0.5;
  const Double_t un   = 1.0;
  const Double_t small = 0.001;
  gMes->SetName("ShowerDamage");
  gMes->SetTitle("TSupplShape");
  SetNoDamage();
  fDamSupFac = damsupfac;
  step  = TMath::Abs(step);
  if (step<small) step = small;
  Double_t X0,Rm,Ec;
  X0 = fOptMat->GetX0();
  Rm = fOptMat->GetRm();
  Ec = fOptMat->GetEc();
  if ((X0>zero) && (Rm>zero) && (Ec>zero)) {
    Double_t Cj,y,tmax,am1,Rk;
    Double_t facdEdz,facdEdr;
    fDamByShower = kTRUE;
    fDamage      = kTRUE;
    fDamKimax    = Kimax;
    fDamStep     = step;
    fDamAxis     = *damaxis;
    fDamAxis.Unit();
    fDamStart    = *damstart;
    fDamE0       = 1000.0*E0;
    fDamGamma    = gamma;
    if (fDamGamma) Cj =  0.5;
    else           Cj = -0.5;
    y            = fDamE0/Ec;
    tmax         = TMath::Log(y) + Cj;
    fDamA        = b*tmax + un;
    am1          = fDamA - un;
    Rk           = Rm/3.0;
    fRk2         = Rk*Rk;
    facdEdz      = (fDamE0*b)/(X0*TMath::Gamma(fDamA));
    facdEdr      = fRk2/TMath::Pi();
    fDamFac      = facdEdz*facdEdr;
    fDamFmax     = fDamFac*TMath::Exp(am1*TMath::Log(am1))*TMath::Exp(-am1);
    fDamFmax    /= fRk2*fRk2;
    fDamFmax     = TMath::Abs(fDamFmax);
  }
  else {
    gMes->M(kill_prog,1,"TOpticMaterial::SetCascades MUST be called first!");
  }
  if (!fDamage) gMes->M(Error_M,1,"No radiation damage assumed");
  return fDamage;
}
Bool_t TSupplShape::ShowerDamage(const T3Vector *damaxis, const T3Vector *damstart,
  const char *namefitkimax, Double_t E0, Bool_t gamma, Double_t step,Double_t damsupfac) {
  // Please read first the chapter "Simulating radiation damages inside a
  //shape" in the class description of this class for a full understanding of
  //how radiation damages are implemented in Litrani.
  // ShowerDamage() is an alternative to RadiationDamage() to be preferred when
  //the damages have been caused by the irradiation by electromagnetic
  //showers, gammas or electrons. ShowerDamage() will generate damages with
  //the same (z,r) distributions as the showers which have caused the damages.
  // The chosen (z,r) distribution is the one which is described in the
  //class description of TElecCascade, and which is also described in the
  //Review of Particle Physics, Vol 3 N°1-4, 1998 pages 148-149.
  // In order that ShowerDamage() can be called for this shape, it is necessary
  //that radiation length, critical energy and Moliere radius have been
  //defined for the material of this shape. It means that you have called 
  //TOpticMaterial::SetCascades(),
  //  For a point (x,y,z) in local coordinate of the shape, the position
  //dependent factor is obtained by projecting the point (x,y,z) on the
  //the axis having the direction "damaxis" and going through the point
  //"damstart". The distance z on the axis from this projected point to the
  //starting point "damstart" and the distance r from the point (x,y,z) to
  //the axis damaxis give the couple (z,r) for which the damage is calculated.
  //  damaxis and damstart are to be given in the local coordinate system of
  //the shape. It means that you have to call ShowerDamage() BEFORE the
  //shape goes into world coordinates, i.e. before the constructor of TSNode
  //for this shape is called, which makes all coordinates to go into world
  //coordinates.
  //
  //  Parameters :
  //
  // damaxis      : axis along which damage varies
  // damstart     : starting point for calculating the damages == starting point
  //                 of the cascades having produced the damages.
  // namefitkimax : name of the fit giving Kimax = 1/Limin, where Limin is the
  //                 contribution to the absorption length of the damages, at the
  //                 point where it is maximal. In this instance of ShowerDamage(),
  //                 Kimax is depending upon wavelength.
  // E0           : energy in [Gev] of the damaging cascades
  // gamma        : if true, damaging cascades are gammas. If false : electrons
  // step         : steps for calculating absorption [cm]. Default 0.4. Refuses
  //                 values smaller than 0.001.
  // damsupfac    : possible supplementary constant factor multiplying Ki(lambda), i.e.
  //                increasing the damages if bigger than 1.
  //
  const Double_t zero = 0.0;
  const Double_t b    = 0.5;
  const Double_t un   = 1.0;
  const Double_t small = 0.001;
  gMes->SetName("ShowerDamage");
  gMes->SetTitle("TSupplShape");
  SetNoDamage();
  fDamSupFac = damsupfac;
  step  = TMath::Abs(step);
  if (step<small) step = small;
  Double_t X0,Rm,Ec;
  X0 = fOptMat->GetX0();
  Rm = fOptMat->GetRm();
  Ec = fOptMat->GetEc();
  if ((X0>zero) && (Rm>zero) && (Ec>zero)) {
    if (fDamFitKimax) {
      delete fDamFitKimax;
      fDamFitKimax = 0;
    }
    fDamFitKimax = TSplineFit::FindFit(namefitkimax,15);
    if (fDamFitKimax) {
      Double_t Cj,y,tmax,am1,Rk;
      Double_t facdEdz,facdEdr;
      fDamByShower = kTRUE;
      fDamage      = kTRUE;
      fDamStep     = step;
      fDamAxis     = *damaxis;
      fDamAxis.Unit();
      fDamStart    = *damstart;
      fDamE0       = 1000.0*E0;
      fDamGamma    = gamma;
      if (fDamGamma) Cj =  0.5;
      else           Cj = -0.5;
      y            = fDamE0/Ec;
      tmax         = TMath::Log(y) + Cj;
      fDamA        = b*tmax + un;
      am1          = fDamA - un;
      Rk           = Rm/3.0;
      fRk2         = Rk*Rk;
      facdEdz      = (fDamE0*b)/(X0*TMath::Gamma(fDamA));
      facdEdr      = fRk2/TMath::Pi();
      fDamFac      = facdEdz*facdEdr;
      fDamFmax     = fDamFac*TMath::Exp(am1*TMath::Log(am1))*TMath::Exp(-am1);
      fDamFmax    /= fRk2*fRk2;
      fDamFmax     = TMath::Abs(fDamFmax);
    }
    else {
      gMes->M(Error_M,3,"Fit for damages not found");
    }
  }
  else {
    gMes->M(kill_prog,2,"TOpticMaterial::SetCascades MUST be called first!");
  }
  if (!fDamage) gMes->M(Error_M,1,"No radiation damage assumed");
  return fDamage;
}
