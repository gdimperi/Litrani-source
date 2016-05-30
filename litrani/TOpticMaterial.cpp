// @(#)root/html:$Id: TOpticMaterial.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>, D. Wahl <mailto:david.n.wahl@gmail.com>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TMath.h"
#include "TMessErr.h"
#include "TLitPhys.h"
#include "TLitGlob.h"
#include "TOpticMaterial.h"

TOpticMaterial* gOutside = 0;

ClassImp(TOpticMaterial)

//______________________________________________________________________________
//
//BEGIN_HTML <!--
/* -->
</pre>
<center>
<h3>TOpticMaterial</h3>
</center>
<div align="justify">
<p>The class <b><code><font size="+1">TOpticMaterial</font></code></b> defines 
all the properties of the materials used by <b><code><font size="+1">Litrani</font></code></b>. 
Notice that calling the constructor of the class is not enough for defining all the 
properties. All constructors, except the default constructor and the last one, define 
name, title, sensibility characteristics (detector or not), magnetic permeability, 
absorption length(s), A, Z, and density. The last constructor is useful in the context 
of diffusion, wavelength shifting or Rayleigh scattering.&nbsp;It also allows an 
infinite absorption length, which is not the case for the others. A, Z, and density 
are only used by the class <b><code><font size="+1"><a href="TPhotoElecCompton.html">TPhotoElecCompton</a></font></code></b> 
for the calculation of the Compton scattering cross-section.&nbsp;If you do not use 
<b><code><font size="+1"><a href="TPhotoElecCompton.html">TPhotoElecCompton</a></font></code></b>, 
you do not need to give them.</p>
<p>The constructors do not define index of refraction or the components of the 
dielectric tensor, neither fluorescent characteristics of the material, nor dE/dx 
deposit, nor diffusion characteristics, no radiation length, no Moliere radius, no 
critical energy. See below how to define these characteristics.</p>
<p>There is an important feature of <b><code><font size="+1">Litrani</font></code></b> 
about absorption lengths that we have to explain: the absorption length may be anisotropic. 
Let us first recall that there exists a coordinate system, called <dfn>&quot;the 
coordinate system of the 3 optical axis&quot;</dfn>, in which the dielectric tensor 
<font face="Symbol" color="red"><b>e</b></font> is diagonal. In this system, the 
dielectric tensor looks like :</p>
</div>
<center>
<table border="4" cellspacing="4" cellpadding="8" align="Center" bgcolor="#ccffcc">
<tr>
<td><font face="Symbol">e</font><font size="1"><sub>a </sub></font></td>
<td>0</td>
<td>0</td>
</tr>
<tr>
<td>0</td>
<td><font face="Symbol">e</font><font size="1"><sub>b</sub></font></td>
<td>0</td>
</tr>
<tr>
<td>0</td>
<td>0</td>
<td><font face="Symbol">e</font><font size="1"><sub>c</sub></font></td>
</tr>
</table>
</center>
<p><dfn>In the isotropic case</dfn> the absorption length is linked to a small 
imaginary part <font face="Symbol">f</font> subtracted from the dielectric constant</p>
<div align="center">
<p><font face="Symbol">e</font><font size="2"><sub>c</sub></font>&nbsp;= &nbsp;<font face="Symbol">e</font> 
- i <font face="Symbol">f</font></p>
</div>
<p> This results into a small imaginary part n<sub>I</sub> for the index of refraction: 
</p>
<div align="center">
<p>n<sub>C</sub> = n<sub>R</sub> - i n<sub>I</sub></p>
</div>
<p>If the plane wave is written:</p>
<div align="center">
<p><font color="blue"><b>E</b></font> = <font color="blue"><b>E</b></font><font size="1"><sub>0</sub></font> 
e<sup>i (<font face="Symbol">w</font>t - <font color="blue"><b>k</b></font>*<font color="blue"><b>x</b></font>)</sup></p>
</div>
<div align="left">
<p>then the Maxwell equations leads to :</p>
</div>
<div align="center">
<p><font color="black"><b>k</b></font><font size="1"><sup>2</sup></font> = (<font face="Symbol"> 
w</font><font face="Symbol" size="1"><sup>2</sup></font><font face="Symbol">em</font> 
/ c<font size="1"><sup>2</sup></font> )= (<font face="Symbol"> w</font><font face="Symbol" size="1"><sup>2</sup></font>n<font size="1"><sup>2</sup></font> 
/ c<font size="1"><sup>2</sup></font> ) ===&gt; n<sub>C</sub><sup>2</sup> = <font face="Symbol">e</font><font size="2"><sub>c</sub></font><font face="Symbol">m 
</font>===&gt; n<sub>R</sub><sup>2</sup> - n<sub>I</sub><sup>2</sup> - 2 i n<sub>R</sub> 
n<sub>I</sub> = <font face="Symbol">em</font> - i <font face="Symbol">fm</font></p>
<p>===&gt; k = (<font face="Symbol"> w</font> / c) (n<sub>R</sub> - i n<sub>I</sub>)</p>
</div>
<div align="left">
<p>implying a real absorption factor e<sup>- (  <font face="Symbol">w</font> n<sub>I</sub> 
x ) / c</sup> . This absorption factor is for the amplitude of the electric field. 
For going to an absorption length, we have to take not the amplitude, but the intensity.&nbsp;So 
the attenuation factor is e<sup>- ( 2 <font face="Symbol">w</font> n<sub>I</sub> 
x ) / c</sup> . leading to an absorption length of </p>
</div>
<div align="center">
<p>L = c / 2 <font face="Symbol">w </font>n<sub>I</sub></p>
</div>
<center>
<p>
<table border="4" cellspacing="4" cellpadding="8" align="Center" bgcolor="#ccffcc">
<tr>
<td><font face="Symbol">e</font><font size="2"><sub>c</sub></font> = <font face="Symbol">e</font> 
- i <font face="Symbol">f </font></td>
<td>n<font size="2"><sub>c</sub></font> = n - i <font face="Symbol">t</font></td>
<td>n<font size="2"><sub>c</sub></font><font size="1"><sup>2</sup></font> = 
<font face="Symbol">e</font><font size="2"><sub>c</sub></font> <font face="Symbol">m</font>&nbsp;</td>
</tr>
<tr>
<td>L = c / 2<font face="Symbol">w</font>n<sub>I</sub><sup> </sup></td>
<td><font face="Symbol">f</font> = 2 n<sub>R</sub> n<sub>I</sub> / <font face="Symbol">m</font>&nbsp;</td>
<td>n<sub>I</sub> = <font face="Symbol">f</font> <font face="Symbol">m</font> 
/ 2 n<sub>R</sub></td>
</tr>
<tr>
<td>==&gt; (1)</td>
<th colspan="2">L = ( n<sub>R</sub> c &nbsp;) / <font face="Symbol">( w f</font> 
<font face="Symbol">m</font> <font face="Symbol">)</font></th>
</tr>
</table>
</center>
<p>So the generalisation of this to <dfn>the anisotropic case</dfn> seems evident 
: we have to subtract a small imaginary part, this times to each of the diagonal 
elements of the dielectric tensor &nbsp;( see for instance &quot;Principles of Optics&quot; 
Max Born&amp;Emil Wolf page 708) :</p>
<center>
<p>
<table border="4" cellspacing="4" cellpadding="8" align="Center" bgcolor="#ccffcc">
<tr>
<td><font face="Symbol">e</font><font size="1"><sub>a </sub></font>- i <font face="Symbol">f</font><font size="2"><sub>a</sub></font></td>
<td>0</td>
<td>0</td>
</tr>
<tr>
<td>0</td>
<td><font face="Symbol">e</font><font size="1"><sub>b</sub></font>- i <font face="Symbol">f</font><font size="2"><sub>b</sub></font></td>
<td>0</td>
</tr>
<tr>
<td>0</td>
<td>0</td>
<td><font face="Symbol">e</font><font size="1"><sub>c</sub></font> - i <font face="Symbol">f</font><font size="1"><sub>c</sub></font></td>
</tr>
</table>
</center>
<div align="justify">
<p>When calculating the index of refraction ( which in the anisotropic case is 
dependent upon the direction of the <font color="blue"><b>k</b></font> vector ), 
we will include these 3 small imaginary parts, which will end up into a complex index 
n whose imaginary part will give the absorption length for this wave with vector 
<font color="blue"><b>k</b></font>. So we will have a different absorption length 
for each different <font color="blue"><b>k</b></font> vector.</p>
<p>It would be very inconvenient to ask the user of <b><code><font size="+1">Litrani</font></code></b> 
to enter the 3 values <font face="Symbol">f</font><font size="2"><sub>a</sub></font>, 
&nbsp;<font face="Symbol">f</font><font size="2"><sub>b</sub></font>, &nbsp;<font face="Symbol">f</font><font size="1"><sub>c</sub></font>. 
Instead, we ask him to enter 3 values &nbsp;of absorption lengths L<font size="1"><sub>a</sub></font>, 
L<font size="1"><sub>b</sub></font>, L<font size="1"><sub>c</sub></font>&nbsp;, which 
are linked to <font face="Symbol">f</font><font size="2"><sub>a</sub></font>, &nbsp;<font face="Symbol">f</font><font size="2"><sub>b</sub></font>, 
&nbsp;<font face="Symbol">f</font><font size="1"><sub>c</sub></font> by equation 
(1), which <b><code><font size="+1">Litrani</font></code></b> will apply to get <font face="Symbol">f</font><font size="2"><sub>a</sub></font>, 
&nbsp;<font face="Symbol">f</font><font size="2"><sub>b</sub></font>, &nbsp;<font face="Symbol">f</font><font size="1"><sub>c</sub></font> 
. In general L<font size="1"><sub>a</sub></font>, L<font size="1"><sub>b</sub></font>, 
L<font size="1"><sub>c</sub></font>&nbsp;will be given by mean of fits of type <b><code><font size="+1"><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html">TSplineFit</a></font></code></b>, 
being dependent upon the wavelength.</p>
<p>More specifically, <strong>if the user only give L<font size="1"><sub>a</sub></font> 
</strong>, then it will be assumed that the user <strong>does not want an anisotropic 
treatment of the absorption length</strong> and L<font size="1"><sub>a</sub></font> 
will be taken as the unique, isotropic absorption length applicable to all waves, 
whatever their <font color="blue"><b>k</b></font> vector. On the contrary, <strong>if 
more than one value of L is given</strong>, then an <strong>anisotropic treatment 
of the absorption length</strong> will be undertaken by <b><code>Litrani</code></b>. 
Notice that it does not make sense to declare the material to be isotropic ( by a 
later call to <b><code><font size="+1"><a href="#TOpticMaterial:IsIsotropic">TOpticMaterial::IsIsotropic</a>()</font></code></b> 
) and to give more than one value of L. There are 4 possible cases :</p>
</div>
<center>
<p>
<table border="4" cellspacing="4" cellpadding="8" align="Center" bgcolor="#ccffcc">
<tr>
<td>You intend to call IsIsotropic()</td>
<td><font face="Symbol">e</font><font size="1"><sub>a</sub></font> = <font face="Symbol">e</font><font size="1"><sub>b</sub></font>&nbsp;= 
<font face="Symbol">e</font><font size="1"><sub>c</sub></font> &nbsp;</td>
<td>L<font size="1"><sub>a</sub></font> = L<font size="1"><sub>b</sub></font>&nbsp;= 
L<font size="1"><sub>c</sub></font></td>
<td>give only L<font size="1"><sub>a</sub></font></td>
</tr>
<tr>
<td>You intend to call IsUniAxialNegBirefr()</td>
<td><font face="Symbol">e</font><font size="1"><sub>a</sub></font> &lt; <font face="Symbol">e</font><font size="1"><sub>b</sub></font>&nbsp;= 
<font face="Symbol">e</font><font size="1"><sub>c</sub></font> &nbsp;</td>
<td>L<font size="1"><sub>b</sub></font>&nbsp;= L<font size="1"><sub>c</sub></font> 
is
<p>not imposed !</td>
<td>you are allowed to give 3 different values for L<font size="1"><sub>a</sub></font>, 
L<font size="1"><sub>b</sub></font>, L<font size="1"><sub>c</sub></font>.
<p>If you give only L<font size="1"><sub>a</sub></font>, L<font size="1"><sub>b</sub></font> 
then L<font size="1"><sub>c</sub></font> == L<font size="1"><sub>b</sub></font></td>
</tr>
<tr>
<td>You intend to call IsUniAxialPosBirefr()</td>
<td><font face="Symbol">e</font><font size="1"><sub>a</sub></font> = <font face="Symbol">e</font><font size="1"><sub>b</sub></font>&nbsp;&lt; 
<font face="Symbol">e</font><font size="1"><sub>c</sub></font></td>
<td>L<font size="1"><sub>b</sub></font>&nbsp;= L<font size="1"><sub>a</sub></font> 
is
<p>not imposed !</td>
<td>you are allowed to give 3 different values for L<font size="1"><sub>a</sub></font>, 
L<font size="1"><sub>b</sub></font>, L<font size="1"><sub>c</sub></font>.
<p>you can give the same values (same fit) for L<font size="1"><sub>a</sub></font>, 
L<font size="1"><sub>b</sub></font></td>
</tr>
<tr>
<td>You intend to call IsBirefr()</td>
<td><font face="Symbol">e</font><font size="1"><sub>a</sub></font> &lt; <font face="Symbol">e</font><font size="1"><sub>b</sub></font>&nbsp;&lt; 
<font face="Symbol">e</font><font size="1"><sub>c</sub></font> &nbsp;</td>
<td>L<font size="1"><sub>a</sub></font> &lt;&gt; L<font size="1"><sub>b</sub></font>&nbsp;&lt;&gt; 
L<font size="1"><sub>c</sub></font></td>
<td>give 3 different values for L<font size="1"><sub>a</sub></font>, L<font size="1"><sub>b</sub></font>, 
L<font size="1"><sub>c</sub></font> or give only L<font size="1"><sub>a</sub></font>.</td>
</tr>
<tr>
<th colspan="4">
<center>
In each of these 4 cases, you are allowed to give only L<font size="1"><sub>a</sub></font>, 
in which case absorption will be isotropic</center>
</th>
</tr>
</table>
</center>
<center>
<h3>Absorption length put to 0</h3>
</center>
<div align="justify">
<p>In <b><code><font size="+1">Litrani</font></code></b>, there is a difference 
between an absorption length strictly put to 0 and a very small absorption length 
! When the user put the absorption length to 0, he implies doing so that it does 
not make sense to propagate a photon inside a shape made of this material. The photon 
is immediatly killed and it is counted apart in the counter <b><code><font size="+1">fNpAbsL0</font></code></b> 
of the class <b><code><a href="TResults.html"><font size="+1">TResults</font></a></code></b>. 
The photons which have died inside a shape with material of absorption length 0 are 
subtracted from the generated photons when calculating efficiency, i.e. they are 
not considered as having been emitted !</p>
<p>When does it makes sense ? Consider for instance photons emitted by an optical 
fibre. In <b><code><font size="+1">Litrani</font></code></b>, photons emitted by 
an optical fibre are emitted from the exit face of the fibre, <b>but from the inner 
side of the fibre</b>, so that the possibility exists that the photon be reflected 
instead of refracted, i.e. returns inside the fibre without any chance to come back. 
These photons have never been emitted by the fibre and so have not to be counted 
among the emitted photons when doing statistics calculations. To understand more 
about this (on first sight) strange way of emitting photons from within the fibre, 
read the very detailed description of the simulation of optical fibre in <b><code><font size="+1">Litrani</font></code></b> 
given in the description of the class <b><code><a href="TSpontan.html#TSpontan:description"><font size="+1">TSpontan</font></a></code></b>.</p>
</div>
<center>
<h3>Diffusion length</h3>
</center>
<div align="justify">
<p>It is observed that sometimes a material diffuses a photon in some other direction. 
This phenomenon is due to bubbles of impurities (for instance air) or to defaults 
in the crystal structure. A very big progress has been achieved in <b><code><font size="+1">Litrani</font></code></b> 
starting from version 3_6. This is due to a very interesting thesis of David Wahl 
[<b><code><font size="+1"><a href="mailto:dwahl@fisica.pucp.edu.pe">dwahl@fisica.pucp.edu.pe</a></font></code></b>] 
:</p>
<ul>
<li><code><b><font size="+1">The Monte-Carlo refractive index matching technique 
for determining the input parameters for simulation of the light collection in scintillating 
crystals</font></b></code>
<li><code><b><font size="+1">D. Wahl, V. Mikhailik and H. Kraus,</font></b></code>
<li><code><b><font size="+1">Nuclear Instruments and Methods in Physics Research 
Section A</font></b></code>
</ul>
<p>Essentially, it is a method, using both a Monte-Carlo (<b><code><font size="+1">Litrani</font></code></b>) 
and lab measurements, to determine precisely the amount of optical photons which 
are diffused, versus the ones which are absorbed. These measurements showed how important 
diffusion is in crystals.&nbsp;It was also discovered in the <b><code><font size="+1"><a href="http://cms.cern.ch/">CMS</a></font></code></b> 
experiment that a correct description of the <b><code><font size="+1">PbWO<sub>4</sub></font></code></b> 
crystals by <b><code><font size="+1">Litrani</font></code></b> was only possible 
if one takes into account diffusion.</p>
<p>David has made a big improvement of the code of  <b><code><font size="+1">TOpticMaterial</font></code></b>, 
in such a way that <b><code><font size="+1">Litrani</font></code></b> is now able 
to describe 3 kinds of diffusion processes:</p>
<ol>
<li><b>Simple diffusion.</b> The user has the possibility to introduce, in addition 
to the absorption length, a diffusion length. The way <b><code><font size="+1">Litrani</font></code></b> 
simulates simple diffusion, is by absorbing the photon and reemitting it <b>at the 
same place</b>, with the <b>same wavelength</b> but with <b>a new </b><font color="blue"><b>k</b></font><b> 
vector</b>, which is randomly generated on 4<font face="Symbol">p</font>. The probability 
of having a diffusion is handled exactly the same way as the probability of having 
an absorption. The user has to introduce a &quot;diffusion length&quot;, similar 
to the &quot;absorption length&quot;. But contrary to the absorption length, the 
diffusion length is always taken as isitropic by <b><code><font size="+1">Litrani</font></code></b>. 
The diffusion length may be fixed or depending upon wavelength. If depending upon 
wavelength, it is described by a <b><code><font size="+1"><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html">TSplineFit</a></font></code></b>. 
It is the reason to have 2 version of the method <b><code><font size="+1"><a href="#TOpticMaterial:SetDiffusionLength">TOpticMaterial::SetDiffusionLength</a>()</font></code></b>.
<li><b>Diffusion (or absorption ?) with WaveLength Shifting</b>. The process 
is the same as the simple diffusion, except that the photon is reemitted <i>at a 
larger wavelength</i>, according to an emission spectrum of type <b><code><font size="+1"><a href="TLitSpectrum.html">TLitSpectrum</a></font></code></b>, 
which is specific to the <b><code><font size="+1">WLS</font></code></b> process of 
the material. We hesitate to name this process &quot;diffusion&quot; or &quot;absorption&quot;. 
One can argue that the photon is first absorbed and then remitted at an other wavelength, 
and for this reason should be called &quot;absorption&quot;.&nbsp;But we have included 
this process into the diffusion processes. This requires the introduction of a special 
diffusion (or absortion ) length, specific of the <b><code><font size="+1">WLS</font></code></b> 
process, which can be either a constant or depending upon wavelength.&nbsp;If depending 
upon a wavelength, it is described by a <b><code><font size="+1"><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html">TSplineFit</a></font></code></b>. 
Spectrum and diffusion length for <b><code><font size="+1">WLS</font></code></b> 
are given by a call to <b><code><font size="+1"><a href="#TOpticMaterial:SetWLS">TOpticMaterial::SetWLS</a></font></code></b>(). 
An example of definition of this diffusion (or absorption) length with <b><code><font size="+1">WLS</font></code></b> 
can be found in the <b><code><font size="+1">CINT</font></code></b> macro <b><code><font size="+1">AbsLengthWLS_Fibre.C</font></code></b> 
delivered with <b><code><font size="+1">Litrani</font></code></b> in the directory 
<b><code><font size="+1">FitMacros</font></code></b>. It defines a fit of type <b><code><font size="+1"><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html">TSplineFit</a></font></code></b>, 
of category 20, whose name begin with <b><code><font size="+1">&quot;AbsLengthWLS_&quot;</font></code></b> 
and whose title begin with <b><code><font size="+1">&quot;Absorption Length for WLS 
| &quot;</font></code></b>, to be stored into the database <b><code><font size="+1">SplineFitDB.rdb</font></code></b>. 
A simple example of <b><code><font size="+1">WLS</font></code></b> is given in the 
<b><code><font size="+1">CINT</font></code></b> macro <b><code><font size="+1">FibreWLS.C</font></code></b>, 
provided with <b><code><font size="+1">Litrani</font></code></b> in the directory 
&quot;<b><code><font size="+1">Macros</font></code></b>&quot;, and using the fit 
<b><code><font size="+1">AbsLengthWLS_Fibre</font></code></b>.
<li><b>Rayleigh scattering</b>. Rayleigh scattering refers to the process of 
scattering of light off particles much smaller than the wavelength of the photons, 
such as molecules or atoms. It differs from other scattering in that the photons 
do not scatter isotropically, as is implemented for diffusion in <b><code><font size="+1">Litrani</font></code></b>. 
Rayleigh scattering is intrinsic to all materials, though often the scattering length 
is so long that it can be neglected. Rayleigh scattering has a preference for regeneration 
of photons in the forward and backward directions. Here again, the process needs 
the introduction of a &quot;scattering length&quot; which can be a constant or depending 
upon wavelength. If depending upon wavelength, it has to be introduced as a <b><code><font size="+1"><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html">TSplineFit</a></font></code></b> 
of category 19, whose name begin with <b><code><font size="+1">&quot;Rayleigh&quot;</font></code></b> 
and whose title begin with <b><code><font size="+1">&quot;Rayleigh length | &quot;</font></code></b>. 
The above quoted example macro <b><code><font size="+1">FibreWLS.C</font></code></b> 
also include Rayleigh scattering as an example. Asking for Rayleigh scattering is 
done by mean of a call to the method <b><code><font size="+1"><a href="#TOpticMaterial:SetRayleighLength">TOpticMaterial::SetRayleighLength()</a></font></code></b>.
</ol>
<p>The 3 diffusion processes are not exclusive of one another. All 3 can be implemented 
at the same time for a given material.</p>
</div>
<center>
<h3></h3>
<h3></h3>
<h3>Electromagnetic showers</h3>
</center>
<div align="justify">
<p>3 more values have to be defined, through the method <b><code><font size="+1"><a href="#TOpticMaterial:SetCascades">SetCascades</a>()</font></code></b>, 
if you intend to instantiate later on the class <b><code><a href="TElecCascade.html"><font size="+1">TElecCascade</font></a></code></b>&nbsp;:</p>
<ol>
<li>The radiation length X<sub>0</sub>
<li>The Moliere radius R<sub>M</sub>
<li>The Critical Energy E<sub>c</sub>.
</ol>
<p>These 3 quantities are only used by <b><code><a href="TElecCascade.html"><font size="+1">TElecCascade</font></a></code></b>.&nbsp;You 
do not need to define them if you do not use <b><code><a href="TElecCascade.html"><font size="+1">TElecCascade</font></a></code></b>.</p>
</div>
<center>
<p>
<table border="4" cellspacing="4" cellpadding="8" align="Center" bgcolor="#ccffcc">
<caption>
<h3>Description of the arguments of the constructor of TOpticMaterial</h3>
</caption>
<tr>
<th width="100"><font size="4"><code><b>Type</b></code></font></th>
<th><font size="4"><code><b>Name</b></code></font></th>
<th><font size="4"><code><b>Due</b></code></font></th>
<th><font size="4"><code><b>Description</b></code></font></th>
</tr>
<tr>
<td width="100"><code><b>const char*</b></code></td>
<td><code><b>name</b></code></td>
<td><code><b>yes</b></code></td>
<td><code><b>Name of the material.</b></code></td>
</tr>
<tr valign="top">
<td width="100"><code><b>const char*</b></code></td>
<td><code><b>title</b></code></td>
<td><code><b>yes</b></code></td>
<td valign="top"><code><b>Title for the material.</b></code></td>
</tr>
<tr>
<td width="100"><code><b>Bool_t</b></code></td>
<td><code><b>sensible</b></code></td>
<td><code><b>yes</b></code></td>
<td>
<div align="justify">
<code><b>
Decides wheter the material is a detector. For instance, if you define a material 
&quot;Silicium&quot;, intended to be the sensitive part of an APD, then sensible 
is true.</b></code></div>
</td>
</tr>
<tr>
<td width="100"><code><b>const char* </b></code><code><b>or Double_t</b></code></td>
<td><code><b>nameMu or mu</b></code></td>
<td><code><b>yes</b></code></td>
<td>
<div align="justify">
<code><b>
For the definition of the magnetic permeability <font face="Symbol">m</font>. 
If you use the first form, nameMu is the name of the fit giving the magnetic permeability 
as a function of wavelength. If you use the second form, mu is the value of the magnetic 
permeability, which is fixed in this case : it does not depend upon wavelength. In 
general, the magnetic permeability is equal to 1, but a value different from 1 or 
even depending upon wavelength is allowed by Litrani.</b></code></div>
</td>
</tr>
<tr>
<td width="100"><code><b>const char* </b></code><code><b>or Double_t</b></code></td>
<td><code><b>nameAbsLa or absLa</b></code></td>
<td><code><b>yes</b></code></td>
<td><code><b>Parameter L<font size="1"><sub>a</sub></font> for absorption length 
as described above. If you use the first form, nameAbsLa is the name of the fit or 
interpolation of class TSplineFit giving L<font size="1"><sub>a</sub></font> as a 
function of wavelength. ( look at class TSplineFit to know how to define a fit or 
an interpolation from a sery of measurements. ) If you use the second form, L<font size="1"><sub>a</sub></font> 
= absLa does not depend upon wavelength<font size="4">.</font></b></code></td>
</tr>
<tr>
<td width="100"><code><b>const char* </b></code><code><b>or Double_t</b></code></td>
<td><code><b>nameAbsLb or absLb</b></code></td>
<td><code><b>no</b></code></td>
<td><code><b>Parameter L<font size="1"><sub>b</sub></font> for absorption length 
as described above. If you use the first form, nameAbsLb is the name of the fit or 
interpolation of class TSplineFit giving L<font size="1"><sub>b</sub></font> as a 
function of wavelength. If you use the second form, L<font size="1"><sub>b</sub></font> 
= absLb does not depend upon wavelength<font size="4">.</font></b></code></td>
</tr>
<tr>
<td width="100"><code><b>const char* </b></code><code><b>or Double_t</b></code></td>
<td><code><b>nameAbsLc or absLc</b></code></td>
<td><code><b>no</b></code></td>
<td><code><b>Parameter L<font size="1"><sub>c</sub></font> for absorption length 
as described above. If you use the first form, nameAbsLc is the name of the fit or 
interpolation of class TSplineFit giving L<font size="1"><sub>c</sub></font> as a 
function of wavelength. If you use the second form, L<font size="1"><sub>c</sub></font> 
= absLc does not depend upon wavelength<font size="4">.</font></b></code></td>
</tr>
<tr>
<td width="100"><code><b>Float_t</b></code></td>
<td><code><b>A</b></code></td>
<td><code><b>no</b></code></td>
<td><code><b>A of Material. Note that you can omit the 3 last parameters. They 
are not used by Litrani. They are there because TOpticMaterial inherits from TMaterial, 
a class defined in ROOT. It is not excluded that they will be of some use in future 
versions of Litrani.</b></code></td>
</tr>
<tr>
<td width="100"><code><b>Float_t</b></code></td>
<td><code><b>Z</b></code></td>
<td><code><b>no</b></code></td>
<td><code><b>Z of Material.</b></code></td>
</tr>
<tr>
<td width="100"><code><b>Float_t</b></code></td>
<td><code><b>density</b></code></td>
<td><code><b>no</b></code></td>
<td><code><b>density of Material.</b></code></td>
</tr>
</table>
</center>
<p></p>
<div align="center">
<h3>Definition of a TOpticMaterial and fluorescent characteristics</h3>
</div>
<p>A typical definition for an optical material looks like :</p>
<div align="left">
<table cellspacing="1" cellpadding="2" align="Center" border="1">
<tr>
<td><b><font size="+1">&gt;</font></b></td>
<td><code><b><font size="+1">TOpticMaterial *pbwo4;</font></b></code></td>
</tr>
<tr>
<td><b><font size="+1">&gt;</font></b></td>
<td><code><b><font size="+1">pbwo4 = new TOpticMaterial(&quot;PbWO4&quot;,&quot;PbWO4 
crystal&quot;, kFALSE, 1.0, &quot;AbsorptionLength_PbWO4&quot;);</font></b></code></td>
</tr>
</table>
</div>
<div align="justify">
<p>Notice that the material is not completly defined by this call. It remains 
to define the index of refraction or the components of the dielectric tensor by calls 
to <b><code><font size="+1"><a href="#TOpticMaterial:IsIsotropic">IsIsotropic</a>()</font></code></b> 
or <b><code><font size="+1"><a href="#TOpticMaterial:IsUniAxialNegBirefr">IsUniAxialNegBirefr</a>()</font></code></b> 
or <b><code><font size="+1"><a href="#TOpticMaterial:IsUniAxialPosBirefr">IsUniAxialPosBirefr</a>()</font></code></b> 
or <b><code><font size="+1"><a href="#TOpticMaterial:IsBirefr">IsBirefr</a>()</font></code></b>. 
Look at these 4 methods for more details. These 4 methods for defining index of refraction 
or dielectric tensor all exist in 3 instances :</p>
</div>
<ol>
<li>n or <font face="Symbol">e</font> is constant
<li>n or <font face="Symbol">e</font> vary according to fit or interpolation of 
type <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html#TSplineFit:description"><font size="+1">TSplineFit</font></a></code></b>		<li>n 
or <font face="Symbol">e</font> vary according to the <font size="+1"><a href="TSellmeier.html"><code><b>Sellmeier</b></code></a> 
</font>law.	</ol>
<div align="justify">
<p>The fluorescent characteristics of material are described in <b><code><font size="+1">Litrani</font></code></b> 
by the class <font size="+1"><b><code><a href="TLitSpectrum.html">TLitSpectrum</a></code></b>.</font> 
Look at the <a href="TLitSpectrum.html#TLitSpectrum:description"><font size="+1">class 
description of <b><code>TLitSpectrum</code></b></font></a> for a detailed description. 
Let us show here 2 examples of use.&nbsp;In the first one, the fluorescent characteristics 
of the material <b><code><font size="+1">PbWO4</font></code></b> are retrieved from 
a &quot;database&quot; <b><code><font size="+1">ROOT</font></code></b> file, <b><code><font size="+1">SpectraDB.rdb</font></code></b>.&nbsp;In 
the second example, <b><code><font size="+1">SpectraDB.rdb</font></code></b> is not 
used and the <b><code><font size="+1">PbWO4</font></code></b> fluorescent spectrum 
is defined explicitly in the <b><code><font size="+1">CINT</font></code></b> code.</p>
<h4><font size="+1">1st example</font></h4>
</div>
<div align="left">
<ul>
<li><b><code><font size="+1">TOpticMaterial *pbwo4;</font></code></b>
<li><b><code><font size="+1">pbwo4 = new TOpticMaterial(&quot;PbWO4&quot;,&quot;PbWO4 
crystal&quot;, kFALSE, 1.0, &quot;AbsLengthTensor_PbWO4__000&quot;, &quot;AbsLengthTensor_PbWO4__001&quot;);</font></code></b>
<li><b><code><font size="+1">pbwo4-&gt;IsUniAxialNegBirefr(&quot;DielTensor_PbWO4__000&quot;,&quot;DielTensor_PbWO4__002&quot;);</font></code></b>
<li><b><code><font size="+1">pbwo4-&gt;FindSpectrum(&quot;Spectrum_PbWO4La&quot;);</font></code></b>
<li><b><code><font size="+1">pbwo4-&gt;dEdx(&quot;DEdx_MusInPbWO4__000&quot;,100.0);</font></code></b>
</ul>
<p></p>
</div>
<div align="justify">
<h4><font size="+1">2nd example</font></h4>
</div>
<div align="left">
<ul>
<li><b><code><font size="+1">TOpticMaterial *pbwo4;</font></code></b>
<li><b><code><font size="+1">pbwo4 = new TOpticMaterial(&quot;PbWO4&quot;,&quot;PbWO4 
crystal&quot;, kFALSE, 1.0, </font></code></b>
<ul>
<li><b><code><font size="+1">&quot;AbsLengthTensor_PbWO4__000&quot;, &quot;AbsLengthTensor_PbWO4__001&quot;);</font></code></b>
</ul>
<li><b><code><font size="+1">pbwo4-&gt;IsUniAxialNegBirefr(&quot;DielTensor_PbWO4__000&quot;,&quot;DielTensor_PbWO4__002&quot;);</font></code></b>
<li><b><code><font size="+1">TLitSpectrum *sp;</font></code></b>
<li><b><code><font size="+1">sp = new TLitSpectrum(&quot;Spectrum_PbWO4La&quot;,&quot;Emission 
Spectrum | Lead tungstate PbWO4 doped with lanthane&quot;);</font></code></b>
<li><b><code><font size="+1">sp-&gt;AddOneComponent(420,35.4938722588,598715.663229,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);</font></code></b>
<li><b><code><font size="+1">sp-&gt;AddOneComponent(500,67.7989582402,251867.894271,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);</font></code></b>
<li><b><code><font size="+1">sp-&gt;AddOneComponent(437.620987064,19.5870778123,53885.7693666,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);</font></code></b>
<li><b><code><font size="+1">sp-&gt;AddOneComponent(492.366250454,27.1662871551,100601.517916,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);</font></code></b>
<li><b><code><font size="+1">sp-&gt;Normalize();</font></code></b>
<li><b><code><font size="+1">pbwo4-&gt;SetLitSpectrum(sp);</font></code></b>
<li><b><code><font size="+1">pbwo4-&gt;dEdx(&quot;DEdx_MusInPbWO4__000&quot;,100.0);</font></code></b>
</ul>
<p>The call to <b><code><a href="#TOpticMaterial:dEdx"><font size="+1">dEdx</font></a>()</code></b> 
means that the material generates light when crossed by particles. For the generation 
of <b><code>dE/dx</code></b>, you can have or not Landau fluctuations.&nbsp;Look 
at the method <b><code><font size="+1"><a href="#TOpticMaterial:dEdx">dEdx</a>()</font></code></b> 
for an explanation on how to take into account or not Landau fluctuations. The above 
example is not arbitrary : it is a good description for the crystal of the <b><code><font size="+1">CMS</font></code></b> 
experiment which is made of <b><code>PbWO4</code></b> doped with Lantane. This example 
works provided you have defined beforehand the fits ( of class <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html#TSplineFit:description"><font size="+1">TSplineFit</font></a></code></b> 
) with names: <font size="+1"><b><code>&quot;AbsLengthTensor_PbWO4_000&quot;, &quot;AbsLengthTensor_PbWO4_001&quot;, 
&quot;DielTensor_PbWO4_000&quot; &quot;DielTensor_PbWO4_002&quot;</code></b> <b><code>&quot;DEdx_MusInPbWO4__000&quot;.</code></b></font></p>
<p></p>
</div>
<div align="center">
<h3>Material &quot;Outside&quot;</h3>
</div>
<div align="justify">
<p>The material &quot;Outside&quot; is the material encountered when leaving a 
shape through an uncovered face, at a location where there is no contact with any 
other shape. &quot;Outside&quot; is the material of the outside world. It matters, 
because we need its index of refraction in order to be able to calculate the total 
reflection on an uncovered face in contact with the outside world. We give to &quot;Outside&quot; 
the characteristics of the vacuum, but the user is free to redefine it, giving to 
it the characteristics of air, for instance. The material &quot;Outside&quot; is 
predefined in <b><code><font size="+1">Litrani</font></code></b>. If the characteristics 
of vacuum suits your setup, ok. Otherwise, fill free to redefine it. There exists 
a global pointer <b><code><font size="+1">gOutside</font></code></b> pointing towards 
the &quot;Outside&quot; material. So redefine it in the following way :</p>
</div>
<center>
<p>
</center>
<div align="left">
<table border="1" cellspacing="2" cellpadding="2" align="Center">
<tr>
<td><font size="+1"><b>&gt;</b></font></td>
<td><font size="+1"><code><b>delete gOutside;</b></code></font></td>
</tr>
<tr>
<td><font size="+1"><b>&gt;</b></font></td>
<td><font size="+1"><code><b>gOutside = new TOpticMaterial(&quot;Outside&quot;,&quot;Air 
taken as outside material&quot;,...</b></code></font></td>
</tr>
</table>
</div>
<p>Do not change the name &quot;Outside&quot; !
<pre>
<!--*/
// -->END_HTML
//
Double_t TOpticMaterial::fMinimalGap = 1.0e-6;
Double_t TOpticMaterial::fCerWaveL   = 450.0;

TOpticMaterial::TOpticMaterial(const char *name,const char *title,Bool_t sensible,
  const char *nameMu,const char *nameAbsLa,const char *nameAbsLb,
  const char *nameAbsLc,Float_t A,Float_t Z,Float_t density):TMaterial(name,title,A,Z,density) {
  // Constructor when absorption length variable and magnetic permeability
  //variable. Look at the class description to get information about the
  //arguments of all constructors.
  // The following only applies if dielectric tensor AND absorption length
  //are defined as anisotropic:
  // Notice the following restriction: if you later on intend to define
  //the dielectric tensor (by a call to IsBirefr or IsUniAxial...) as NOT
  //depending upon wavelength, but fixed, DO NOT use this constructor if the
  //absorbtion length has to be anisotropic.
  // In the anisotropic case, if the dielectric tensor does not depend upon
  //wavelength, the absorption length MUST NOT depend upon wavelength. You
  //can easily overcome this limitation by defining the dielectric tensor
  //as depending upon wavelength (as a fit of type TSplineFit), but setting the
  //degree of the fit to be 0!
  //
  const Double_t un = 1.0;
  TString s;
  TString snone = "";
  BasicInit();
  gMes->SetName("TOpticMaterial");
  gMes->SetTitle("TOpticMaterial");
  fSensible = sensible;
  Bool_t ok    = kTRUE;
  //magnetic permeability will vary with wavelength.
  //fit giving this variation must have been defined before
  fFitMu = TSplineFit::FindFit(nameMu,14);
  if (fFitMu) {
    fNameMu = nameMu;
    fMuFound = kTRUE;
  }
  else {
    fMuFound = kFALSE;
    gMes->M(Error_M,1,"Fit for magnetic permeability not found");
    gMes->M(Info_M,1,nameMu);
    gMes->M(kill_prog,1,"Define fit for magnetic permeability before material");
    return;
  }
  //absorption length will vary with wavelength.
  //fit giving this variation must have been defined before
  fFitAbsLa = TSplineFit::FindFit(nameAbsLa);
  if (fFitAbsLa) {
    fNameAbsLa = nameAbsLa;
    fAbsLa     = un;    //means initialization ok for fAbsLa
  }
  else {
    ok = kFALSE;
    gMes->M(Error_M,2,"Fit for abs. length fAbsLa not found");
    gMes->M(Info_M,2,nameAbsLa);
    gMes->M(kill_prog,2,"Define fit for absorption length before material");
    return;
  }
  if (snone.CompareTo(nameAbsLb)) {
    fAniAbsL = 1;
    fFitAbsLb = TSplineFit::FindFit(nameAbsLb);
    if (fFitAbsLb) {
      fNameAbsLb = nameAbsLb;
      fAbsLb     = un;    //means initialization ok for fAbsLb
    }
    else {
      ok = kFALSE;
      gMes->M(Error_M,3,"Fit for abs. length fAbsLb not found");
      gMes->M(Info_M,3,nameAbsLb);
      gMes->M(kill_prog,3,"Define fit for absorption length before material");
      return;
    }
  }
  if (snone.CompareTo(nameAbsLc)) {
    fAniAbsL += 2;
    fFitAbsLc = TSplineFit::FindFit(nameAbsLc);
    if (fFitAbsLc) {
      fNameAbsLc = nameAbsLc;
      fAbsLc     = un;    //means initialization ok for fAbsLc
    }
    else {
      ok = kFALSE;
      gMes->M(Error_M,4,"Fit for abs. length fAbsLc not found");
      gMes->M(Info_M,4,nameAbsLc);
      gMes->M(kill_prog,4,"Define fit for absorption length before material");
      return;
    }
  }
  SetDiffusionLength(-1.0);
  if (ok) AddToList(name);
}
TOpticMaterial::TOpticMaterial(const char *name,const char *title,Bool_t sensible,
  const char *nameMu,Double_t absLa,Double_t absLb,Double_t absLc,
  Float_t A,Float_t Z,Float_t density):TMaterial(name,title,A,Z,density) {
  //constructor when absorption length fixed and magnetic permeability
  //variable. Look at the class description to get information about the
  //arguments of all constructors.
  const Double_t zero = 0.0;
  TString s;
  BasicInit();
  gMes->SetName("TOpticMaterial");
  gMes->SetTitle("TOpticMaterial");
  fSensible = sensible;
  Bool_t ok    = kTRUE;
  fFitMu = TSplineFit::FindFit(nameMu,14);
  if (fFitMu) fNameMu = nameMu;
  else {
    ok = kFALSE;
    fMuFound = kFALSE;
    gMes->M(Error_M,11,"Fit for magnetic permeability not found");
    gMes->M(Info_M,11,nameMu);
    gMes->M(kill_prog,11,"Define fit for magnetic permeability before material");
    return;
  }
  //absorption length will not vary with wavelength
  fAbsLa = TMath::Abs(absLa);
  if (absLb>zero) {
    fAniAbsL  = 1;
    fAbsLb    = absLb;
  }
  if (absLc>zero) {
    fAniAbsL += 2;
    fAbsLc    = absLc;
  }
  SetDiffusionLength(-1.0);
  if (ok)
    AddToList(name);
}
TOpticMaterial::TOpticMaterial(const char *name,const char *title,Bool_t sensible,
  Double_t mu,const char *nameAbsLa,const char *nameAbsLb,const char *nameAbsLc,
  Float_t A,Float_t Z,Float_t density):TMaterial(name,title,A,Z,density) {
  //constructor when absorption length variable and magnetic permeability
  //fixed. Look at the class description to get information about the
  //arguments of all constructors.
  // The following only applies if dielectric tensor AND absorption length
  //are defined as anisotropic:
  // Notice the following restriction: if you later on intend to define
  //the dielectric tensor (by a call to IsBirefr or IsUniAxial...) as NOT
  //depending upon wavelength, but fixed, DO NOT use this constructor if the
  //absorbtion length has to be anisotropic.
  // In the anisotropic case, if the dielectric tensor does not depend upon
  //wavelength, the absorption length MUST NOT depend upon wavelength. You
  //can easily overcome this limitation by defining the dielectric tensor
  //as depending upon wavelength (as a fit of type TSplineFit), but setting the
  //degree of the fit to be 0!
  //
  const Double_t un = 1.0;
  TString s;
  TString snone="";
  BasicInit();
  gMes->SetName("TOpticMaterial");
  gMes->SetTitle("TOpticMaterial");
  fSensible = sensible;
  //magnetic permeability will not vary with wavelength : fixed at mu
  fMu = TMath::Abs(mu);
  //absorption length will vary with wavelength.
  //fit giving this variation must have been defined before
  Bool_t ok    = kTRUE;
  fFitAbsLa = TSplineFit::FindFit(nameAbsLa);
  if (fFitAbsLa) {
    fNameAbsLa = nameAbsLa;
    fAbsLa     = un;    //means initialization ok for Abs. length
  }
  else {
    ok = kFALSE;
    gMes->M(Error_M,21,"Fit for absorption length fAbsLa not found");
    gMes->M(Info_M,21,nameAbsLa);
    gMes->M(kill_prog,21,"Define fit for absorption length before material");
    return;
  }
  if (snone.CompareTo(nameAbsLb)) {
    fAniAbsL = 1;
    fFitAbsLb = TSplineFit::FindFit(nameAbsLb);
    if (fFitAbsLb) {
      fNameAbsLb = nameAbsLb;
      fAbsLb     = un;    //means initialization ok for fAbsLb
    }
    else {
      ok = kFALSE;
      gMes->M(Error_M,22,"Fit for abs. length fAbsLb not found");
      gMes->M(Info_M,22,nameAbsLb);
      gMes->M(kill_prog,22,"Define fit for absorption length before material");
      return;
    }
  }
  if (snone.CompareTo(nameAbsLc)) {
    fAniAbsL += 2;
    fFitAbsLc = TSplineFit::FindFit(nameAbsLc);
    if (fFitAbsLc) {
      fNameAbsLc = nameAbsLc;
      fAbsLc     = un;    //means initialization ok for fAbsLc
    }
    else {
      ok = kFALSE;
      gMes->M(Error_M,23,"Fit for abs. length fAbsLc not found");
      gMes->M(Info_M,23,nameAbsLc);
      gMes->M(kill_prog,23,"Define fit for absorption length before material");
      return;
    }
  }
  SetDiffusionLength(-1.0);
  if (ok)
    AddToList(name);
}
TOpticMaterial::TOpticMaterial(const char *name,const char *title,Bool_t sensible,
  Double_t mu,Double_t absLa,Double_t absLb,Double_t absLc,Float_t A,Float_t Z,
  Float_t density):TMaterial(name,title,A,Z,density) {
  //constructor when absorption length fixed and magnetic permeability fixed.
  //Look at the class description to get information about the arguments of
  //all constructors.
  const Double_t zero = 0.0;
  BasicInit();
  fSensible = sensible;
  //magnetic permeability will not vary with wavelength : fixed at mu
  fMu = TMath::Abs(mu);
  //absorption length will not vary with wavelength : fixed at absL
  fAbsLa = TMath::Abs(absLa);
  if (absLb>zero) {
    fAniAbsL  = 1;
    fAbsLb    = absLb;
  }
  if (absLc>zero) {
    fAniAbsL += 2;
    fAbsLc    = absLc;
  }
  SetDiffusionLength(-1.0);
  AddToList(name);
}
TOpticMaterial::TOpticMaterial(const char* name, Float_t A, Float_t Z, Float_t density,
  Double_t mu, Bool_t sensible):TMaterial(name,name,A,Z,density) {
  // This is the constructor where all optical properties (apart from the 
  //magnetic permeability) need to be fixed by the user. Note that if no 
  //parameters are given by the user, the TOpticMaterial will have no 
  //absorption, no defect or Rayleigh scattering and no WLS component.
  const Double_t zero = 0.0;
  BasicInit();
  fSensible = sensible;
  fMu = TMath::Abs(mu);
  // Absorption
  fHasAbsorption = kFALSE;
  fAbsLa = zero;
  // Diffusion
  SetDiffusionLength(-1.0);
  AddToList(name);
}
TOpticMaterial::~TOpticMaterial() {
  // Destructor. We do not delete the TSplineFit or TLitSpectrum! Perhaps they are
  //used by an other material
  if (fInUse) {
    gMes->SetName("~TOpticMaterial");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(Error_M,4,"deleting an optical material which is in use");
  }
  if (gLit) {
    gLit->fOptMat.Remove(this);
    gLit->fNOptMat--;
  }
  if (fIndexS)      delete fIndexS;
  if (fEpsaS)       delete fEpsaS;
  if (fEpsbS)       delete fEpsbS;
  if (fEpscS)       delete fEpscS;
}
Bool_t TOpticMaterial::AddToList() const {
  //record this material into gLit->fOptMat
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,1,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneOptMat(this,fName);
}
Bool_t TOpticMaterial::AddToList(const char *name) const {
  //record this material into gLit->fOptMat
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,1,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneOptMat(this,name);
}
void TOpticMaterial::BasicInit() {
  //Do basic initialization
  BasicInit1();
  fNameMu     = "";
  fNameAbsLa  = "";
  fNameAbsLb  = "";
  fNameAbsLc  = "";
  fNameIndex  = "";
  fNameEpsa   = "";
  fNameEpsb   = "";
  fNameEpsb   = "";
  fNamedEdx   = "";
  fNameNPLY   = "";
  fNamePE     = "";
}
void TOpticMaterial::BasicInit1() {
  //Do first basic initialization. All pointers to 0
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  // Pointers to 0
  fFitAbsLa     = 0;
  fFitAbsLb     = 0;
  fFitAbsLc     = 0;
  fFitDiffusL   = 0;
  fFitRayleighL = 0;
  fIndexS       = 0;
  fFitIndex     = 0;
  fFitMu        = 0;
  fEpsaS        = 0;
  fFitEpsa      = 0;
  fEpsbS        = 0;
  fFitEpsb      = 0;
  fEpscS        = 0;
  fFitEpsc      = 0;
  fDeposit      = 0;
  fLitSpectrum  = 0;
  fFitNPLY      = 0;
  fFitPE        = 0;
  fWLSSpectrum  = 0;
  fFitWLSL      = 0;
  //
  fInUse        = kFALSE;      //not yet used by a convex polyhedron
  fSensible     = kFALSE;      //by default, not sensible;
  fFacAbsL      = un;          //by default, no factor for all values of abs. length
  fAniAbsL      = 0;           //abs. length isotropic by default
  fAbsLa        = -un;         //means Abs. length not yet initialized
  fPhimua       = zero;        //default imaginary part of fEpsa
  fAbsLb        = -un;         //means Abs. length not yet initialized
  fPhimub       = zero;        //default imaginary part of fEpsb
  fAbsLc        = -un;         //means Abs. length not yet initialized
  fPhimuc       = zero;        //default imaginary part of fEpsc
  fAnisotropic  = -1;          //optical properties defined later
  fIndexM       = undefined;
  fEpsaM        = undefined;
  fEpsbM        = undefined;
  fEpscM        = undefined;
  fIndex        = -1;          //means not yet initilized
  fEpsa         = -1;          //    "
  fEpsb         = -1;          //    "
  fEpsb         = -1;          //    "
  fMuFound      = kTRUE;
  //Everything concerning dE/dx not initialized in the constructor,
  // but requires further calls to other methods
  fdEdxUsed     = nodEdx;
  fPhotMev      = zero;
  //Everything concerning Fluorescence not initialized in the constructor,
  // but requires further calls to other methods
  fHistFluo     = kFALSE;
  //Everything concerning electromagnetic showers
  fX0           = -un;
  fRm           = -un;
  fEc           = -un;
  fHasNPLY      = kFALSE;
  fPEIsInitialised = kFALSE;
  fResolutionScale = 1.0;
  fHasDiffusion    = kFALSE; 
  fHasRayleigh     = kFALSE; 
  fHasWLS          = kFALSE;
  fHasAbsorption   = kTRUE;
}
Bool_t TOpticMaterial::CheckIt() {
  //Checks that everything needed has been defined. Call this method to
  //be sure you have defined properly this material. If you get no message,
  //everything is ok !
  if (fChecked)
    return fChecked;
  gMes->SetName("CheckIt");
  gMes->SetTitle("TOpticMaterial");
  //absorption length
  if ((fAniAbsL) && (fAbsLa<0.0)) {
    ProblemHeader();
    gMes->M(Error_M,20,"Absorption Length fAbsLa not entered properly");
    return fChecked;
  }
  if ((fAniAbsL) && (fAbsLb<0.0)) {
    ProblemHeader();
    gMes->M(Error_M,21,"Absorption Length fAbsLb not entered properly");
    return fChecked;
  }
  if ((fAniAbsL>1) && (fAbsLc<0.0)) {
    ProblemHeader();
    gMes->M(Error_M,22,"Absorption Length fAbsLc not entered properly");
    return fChecked;
  }
  if ((fAnisotropic==0) && (fAniAbsL!=0)) {
    ProblemHeader();
    gMes->M(Error_M,23,"Abs. L. cannot be anisotropic if diel isotropic");
    return fChecked;
  }
  if ((fAnisotropic<0) || (fAnisotropic>3)) {
    ProblemHeader();
    gMes->M(Error_M,1,"optical properties not defined");
    gMes->M(Info_M,1,"call IsIsotropic or IsBirefr or IsUniA...");
    return fChecked;
  }
  if ((fAnisotropic==0) && (fIndex<1.0)) {
    ProblemHeader();
    gMes->M(Error_M,3,"Index of refraction smaller than 1");
    return fChecked;
  }
  if (fMu<=0.0) {
    ProblemHeader();
    gMes->M(Error_M,4,"Magnetic permeability not entered properly");
    return fChecked;
  }
  if ((fAnisotropic) && (fEpsa<=0.0)) {
    ProblemHeader();
    gMes->M(Error_M,5,"Epsa of diel. tensor not defined properly");
    return fChecked;
  }
  if ((fAnisotropic==3) && (fEpsb<=0.0)) {
    ProblemHeader();
    gMes->M(Error_M,6,"Epsb of diel. tensor not defined properly");
    return fChecked;
  }
  if ((fAnisotropic) && (fEpsc<=0.0)) {
    ProblemHeader();
    gMes->M(Error_M,7,"Epsc of diel. tensor not defined properly");
    return fChecked;
  }
  switch (fdEdxUsed) {
  case nodEdx:
    break;
  case WithoutLandau:
    if (!fDeposit) {
      ProblemHeader();
      gMes->M(Error_M,8,"Fit or interpolation for dEdx not found");
      return fChecked;
    }
    break;
  case WithLandau:
    if (!fDeposit) {
      ProblemHeader();
      gMes->M(Error_M,9,"Family of fits for dE/dx not found");
      return fChecked;
    }
    break;
  }
  fChecked = kTRUE;
  return fChecked;
}
void TOpticMaterial::dEdx(const char *nameDist,Double_t photmev,Bool_t Landau) {
  //  Declares the material as having a distribution dE/dx for the generation
  //of deposited energy and that photmev photons are generated per mev of
  //deposited energy.
  //  There are 2 possibilities :
  //    (1) - you want Landau fluctuations in the deposited energy. Then put
  //Landau to true and give for nameDist the name of the first element [ ending
  //in _000 ] of a family of fits of type TSplineFit. Each member (each fit) in
  //the family will then give a randomized value of dEdx distributed according
  //the Landau distribution valid for the energy range for which this fit is valid.
  //Look at the class description of the class TSplineFit for more informations.
  //  For example, in this family of fits, you can have one distribution for
  //muons between 0 and 10 Gev, an other for muons between 10 and 20 Gev, and
  //so on.
  //    (2) - you do not want Landau fluctuations in the deposited energy, but
  //you want that the deposited energy be fixed for a fixed energy of the par-
  //ticle. Then put Landau to false. The deposited energy, however, varies if
  //the energy of the particle varies, and this variation is described by a fit
  //or an interpolation of type TSplineFit whose name is nameDist, and which
  //does not belong to a family of fits.
  //
  const Double_t MinPhotMev=1.0e-4;
  TString s;
  gMes->SetName("dEdx");
  gMes->SetTitle("TOpticMaterial");
  if (Landau) fdEdxUsed = WithLandau;
  else        fdEdxUsed = WithoutLandau;
  switch (fdEdxUsed) {
  case WithoutLandau:
    fDeposit = TSplineFit::FindFit(nameDist,11);
    if (fDeposit) fNamedEdx = nameDist;
    else {
      gMes->M(Error_M,1,"Distribution for dE/dx not found");
      gMes->M(Info_M,1,nameDist);
      gMes->M(kill_prog,1,"Please define distribution for dE/dx before calling dEdx");
      return;
    }
    break;
  case WithLandau:
    fDeposit = TSplineFit::FindFit(nameDist,11);
    if (fDeposit) {
      fNamedEdx = nameDist;
      fNbdEdx   = fDeposit->LoadFamily();
    }
    else {
      gMes->M(Error_M,1,"Family distribution for dE/dx not found");
      gMes->M(Info_M,1,nameDist);
      gMes->M(kill_prog,1,"Please define Family distribution for dE/dx before calling dEdx");
      return;
    }
    break;
  default:
    break;
  }
  photmev  = TMath::Abs(photmev);
  fPhotMev = photmev;
  if (photmev<MinPhotMev)
    gMes->M(Warning_M,1,"your number of photons per mev is extraordinary small ?");
}
void TOpticMaterial::DoWLS(Double_t &wavelength,Double_t &lifetime,Int_t &kfluo) {
  // Generate the new wavelength of a photon which undergoes wavelength shifting.
  //It is assumed that the emission spectrum is the same whatever the wavelength
  //of the absorbed photon. Since there is often an overlap between the emission
  //and absorption spectra, the routine checks that the photons are not shifted 
  //up in energy by the WLS process.
  // TOpticMaterial::DoWLS() is called by TPhoton in method TPhoton::Regenerate().
  Double_t oldwavelength = wavelength;
  Double_t lt = 0;
  // Condition to avoid that the energy of the photon increases with WLS
  while (!(wavelength>oldwavelength)) {
    kfluo = fWLSSpectrum->GenWaveLAndDecayT(wavelength,lt);}
  lifetime += 1000*lt;//Should this be in ps or ns?
  gLit->NewWavelength(wavelength,kTRUE);
}
void TOpticMaterial::FindSpectrum(const char *namesp) {
  // Look for a TLitSpectrum with name "namesp" first in the collection TLitSpectrum::fgSpectra,
  //and if not found there, then in the "database" file of spectra, SpectraDB.rdb. Not
  //finding the spectrum is a fatal error
  fLitSpectrum = TLitSpectrum::FindSpectrum(namesp);
  if (!fLitSpectrum) {
    gMes->SetName("FindSpectrum");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,1,"Specified optical spectrum not found");
  }
}
void TOpticMaterial::FluoComponent(Double_t wavel,Double_t stddev,Double_t weigth,Double_t lifetime) {
  //
  //  This method is obsolete. It has been kept only to be compatible with old CINT macros
  //driving Litrani. We advise you not to use it and to rather define the fluorescent
  //characteristics of this material outside TOpticMaterial, as is detailed in the
  //class description of the class TLitSpectrum, and then to link the created TLitSpectrum
  //to this material, calling the method TOpticMaterial::FindSpectrum(const char *).
  //
  //  Add one component of fluorescence. You can add as many components of
  //fluorescence as you like, by calling "FluoComponent" several times.
  //
  //  Arguments
  //
  //    wavel   - the mean value of the wavelength (in nanometers ) of this component.
  //    stddev  - is the standard deviation ( in nanometers ) of the generated spectrum of
  //              this component. The spectrum for one component is assumed gaussian.
  //    weigth  - is the weigth of this component. You do not need to make sure that the
  //              sum of all weigths of all components is 1 : the weigths will be
  //              renormalized to 1 by the program.
  //   lifetime - is the lifetime of this component in nanoseconds.
  //
  const Double_t m1 = -1.0;
  TLitSpectrumCp *q;
  if (!fLitSpectrum) {
    TString s1 = "Spectrum_";
    TString s2 = "Emission Spectrum | ";
    s1.Append(GetName());
    s2.Append(GetTitle());
    fLitSpectrum = new TLitSpectrum(s1.Data(),s2.Data());
    fLitSpectrum->AddOneComponent(wavel,stddev,m1,lifetime,m1,weigth);
  }
  else {
    q = fLitSpectrum->FindComponent(wavel);
    if (q) fLitSpectrum->AddTimeComponent(q,lifetime,m1,weigth);
    else   fLitSpectrum->AddOneComponent(wavel,stddev,m1,lifetime,m1,weigth);
  }
}
Double_t TOpticMaterial::GetdEdx(Double_t E) const {
  //returns a value for dE/dx for a particle of energy E
  const Double_t zero = 0.0;
  Double_t dedx = zero;
  switch (fdEdxUsed) {
  case nodEdx:
    gMes->SetName("GetdEdx");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(Error_M,1,"No dEdx defined for this material");
    break;
  case WithoutLandau:
    dedx = fDeposit->V(E);
    break;
  case WithLandau:
    dedx = fDeposit->GetRandom(fNbdEdx,E);
    break;
  }
  return dedx;
}
Short_t TOpticMaterial::GetFluorescent() const {
  // Returns number of gaussian components of fluorescence
  Short_t k = 0;
  if (fLitSpectrum) k = fLitSpectrum->GetNbOfComponents();
  return k;
}
Bool_t TOpticMaterial::GetIChangeWithWvl() const {
  //returns true if index or dielectric tensor changes with wavelength
  Bool_t b = kFALSE;
  switch (fIndexM) {
  case undefined:
  case constant:
    break;
  case byfit:
  case sellmeier:
    b = kTRUE;
    break;
  }
  if (!b) {
    switch (fEpsaM) {
    case undefined:
    case constant:
      break;
    case byfit:
    case sellmeier:
      b = kTRUE;
      break;
    }
  }
  if (!b) {
    switch (fEpsbM) {
    case undefined:
    case constant:
      break;
    case byfit:
    case sellmeier:
      b = kTRUE;
      break;
    }
  }
  if (!b) {
    switch (fEpscM) {
    case undefined:
    case constant:
      break;
    case byfit:
    case sellmeier:
      b = kTRUE;
      break;
    }
  }
  return b;
}
Double_t TOpticMaterial::GetnCerenkov() const {
  //  Delivers the value of the index of refraction used in the formulae for
  //Cerenkov radiation.
  const Double_t un = 1.0;
  Double_t mu;
  Double_t epsc = 0.0;
  Double_t n = 0.0;
  if (fFitMu) mu = TMath::Abs(fFitMu->V(fCerWaveL));
  else mu = fMu;
  switch (fAnisotropic) {
  case 0:
    switch (fIndexM) {
  case undefined:
    gMes->SetName("GetnCerenkov");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,1,"method for index not defined");
    break;
  case constant:
    n = fIndex;
    break;
  case byfit:
    n = TMath::Abs(fFitIndex->V(fCerWaveL));
    if (n<un) n = un;
    break;
  case sellmeier:
    n = fIndexS->GetIndex(fCerWaveL,mu);
    if (n<un) n = un;
    break;
    }
    break;
  default:
    switch (fEpscM) {
  case undefined:
    gMes->SetName("GetnCerenkov");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,2,"method for Epsc not defined");
    break;
  case constant:
    epsc = fEpsc;
    break;
  case byfit:
    epsc = TMath::Abs(fFitEpsc->V(fCerWaveL));
    break;
  case sellmeier:
    epsc = fEpscS->GetDiel(fCerWaveL);
    break;
    }
    n = TMath::Sqrt(epsc*mu);
    break;
  }
  return n;
}

Int_t TOpticMaterial::GetNPhot(Double_t Edep) const {
  //
  // This method returns the number of photons generated for a deposit of Edep
  //energy, but affecting it with gaussian statistical fluctuations [intrinsic
  //resolution, Fano factor], and also, if fFitNPLY is non-zero, with a
  //non-linearity factor between the number of photons generated and the deposited
  //energy.
  // If you do not want these 2 features, call GetPhotMev() instead of
  //GetNPhot() and multiply by Edep.
  //
  Double_t num; 
  if (fHasNPLY) {
    if (!fFitNPLY) {
      gMes->SetName("GetNPhot");
      gMes->SetTitle("TOpticMaterial");
      gMes->M(Error_M,1,"The Non-proportionality has not been set correctly");
      gMes->M(kill_prog,1,"Please check SetNonPropLY() for material");}
    num = Edep*fPhotMev*(fFitNPLY->V(Edep));}
  else num = Edep*fPhotMev;
  return NPhotInteger(gRandom3->Gaus(num,fResolutionScale*TMath::Sqrt(num))); 
}

Double_t TOpticMaterial::GetPhotMev() const {
  // Backward compatible GetPhotMev which does not include statistical broadening
  const Double_t un  = 1.0;
  const Double_t eps = 1.0e-8;
  Double_t t;
  t = TMath::Abs(fResolutionScale - un);
  if (fHasNPLY) {
    gMes->SetName("GetPhotMev");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(Error_M,1,"Cannot call GetPhotMev() if material has non-proportional lightyield response");
    gMes->M(kill_prog,1,"Please use GetPhotMev(Double_t Edep)");
  }
  if (t>eps) {
    gMes->SetName("GetPhotMev");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(Error_M,2,"You are calling GetPhotMeV which does not support fResolutionScale");
    gMes->M(kill_prog,1,"Please use GetNPhot(Double_t Edep) to include the resolution scale");
  }
  return fPhotMev;
}

Double_t TOpticMaterial::GetXSectnPE(Double_t MeV) const {
  // Return the photo-electric cross section for a photon of energy MeV
  if (!fPEIsInitialised) return 0;
  return fFitPE->V(MeV);
}
Bool_t TOpticMaterial::GoodForCPE() const {
  // Check if all the quantities necessary for the photo-electric effect and 
  //Compton scattering have been initialised. This routine is called internally 
  //by Litrani for each material before trying to use these processes.
  if (!fPEIsInitialised) return kFALSE;
  if (GetZ() <= 0) return kFALSE;
  if (GetDensity() <= 0) return kFALSE;
  if (GetA() <= 0) return kFALSE;
  return kTRUE;
}
void TOpticMaterial::IsBirefr(const char *nameEpsa,const char *nameEpsb,
  const char *nameEpsc) {
  // To be called when the material is totally anisotropic (birefringent) :
  //                   epsa < epsb < epsc
  // Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention.
  //  This instance of IsBirefr handles the case where epsa, epsb and epsc
  //varies with wavelength according to fits of type TSplineFit provided by the
  //user. epsa, epsb and epsc are the 3 diagonal elements of the dielectric
  //tensor in the coordinate system where it is diagonal. nameEpsa, nameEpsb
  //and nameEpsc are the names of fits (of class TSplineFit) giving epsa, epsb
  //and epsc as a function of wavelength.
  //
  fAnisotropic = 3;
  TString s;
  gMes->SetName("IsBirefr");
  gMes->SetTitle("TOpticMaterial");
  fFitEpsa = TSplineFit::FindFit(nameEpsa,2);
  if (fFitEpsa) {
    fNameEpsa = nameEpsa;
    fEpsaM    = byfit;
    fEpsa     = 1.0;    //means initialization ok for Epsa
  }
  else {
    gMes->M(Error_M,1,"Fit for low component Epsa not found");
    gMes->M(Info_M,1,nameEpsa);
    gMes->M(Info_M,1,"Define fit for Epsa before defining optical material");
    return;
  }
  fFitEpsb = TSplineFit::FindFit(nameEpsb,2);
  if (fFitEpsb) {
    fNameEpsb = nameEpsb;
    fEpsbM    = byfit;
    fEpsb     = 1.0;    //means initialization ok for Epsb
  }
  else {
    gMes->M(Error_M,2,"Fit for medium component Epsb not found");
    gMes->M(Info_M,2,nameEpsb);
    gMes->M(Info_M,2,"Define fit for Epsb before defining optical material");
    return;
  }
  fFitEpsc = TSplineFit::FindFit(nameEpsc,2);
  if (fFitEpsc) {
    fNameEpsc = nameEpsc;
    fEpscM    = byfit;
    fEpsc     = 1.0;    //means initialization ok for Epsc
  }
  else {
    gMes->M(Error_M,3,"Fit for high component Epsc not found");
    gMes->M(Info_M,3,nameEpsc);
    gMes->M(Info_M,3,"Define fit for Epsc before defining optical material");
    return;
  }
}
void TOpticMaterial::IsBirefr(Double_t epsa,Double_t epsb,Double_t epsc) {
  // To be called when the material is totally anisotropic (birefringent) :
  //                     epsa < epsb < epsc
  // Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. The first argument
  //MUST be the smallest, the last argument MUST be the biggest.
  //  This instance of IsBirefr handles the case where epsa, epsb and epsc do
  //not vary with wavelength. epsa, epsb and epsc are the 3 diagonal elements
  //of the dielectric tensor in the coordinate system where it is diagonal.
  // The following only applies if dielectric tensor AND absorption length
  //are defined as anisotropic:
  // Notice the following restriction: if you later on intend to define
  //the dielectric tensor (by a call to IsBirefr or IsUniAxial...) as NOT
  //depending upon wavelength, but fixed, DO NOT use anisotropic absorption
  //length depending upon wavelength.
  // In the anisotropic case, if the dielectric tensor does not depend upon
  //wavelength, the absorption length MUST NOT depend upon wavelength. You
  //can easily overcome this limitation by defining the dielectric tensor
  //as depending upon wavelength (as a fit of type TSplineFit), but setting the
  //degree of the fit to be 0!
  //
  register Double_t a;
  Bool_t toocloseab = kFALSE;
  Bool_t tooclosebc = kFALSE;
  fAnisotropic = 3;
  epsa = TMath::Abs(epsa);
  epsb = TMath::Abs(epsb);
  epsc = TMath::Abs(epsc);
  gMes->SetName("IsBirefr");
  gMes->SetTitle("TOpticMaterial");
  //verifies that epsb > epsa
  if (epsa>=epsb) {
    gMes->M(Warning_M,4,"epsb must be bigger than epsa");
    gMes->M(Info_M,4,"We interchange epsa and epsb !");
    a = epsb;
    epsb = epsa;
    epsa = a;
  }
  //verifies that epsc > epsb
  if (epsb>=epsc) {
    gMes->M(Warning_M,5,"epsc must be bigger than epsb");
    gMes->M(Info_M,5,"We interchange epsc and epsb !");
    a = epsc;
    epsc = epsb;
    epsb = a;
  }
  //verifies minimal gap between epsa and epsb
  if (epsb < (epsa+fMinimalGap)) {
    epsb = epsb + fMinimalGap;
    toocloseab = kTRUE;
  }
  //verifies minimal gap between epsb and epsc
  while (epsc < (epsb+fMinimalGap)) {
    epsc = epsc + fMinimalGap;
    tooclosebc = kTRUE;
  }
  if (toocloseab) {
    gMes->M(Warning_M,6,"epsa ~= epsb ! You should call IsUniAxialNegBirefr instead of IsBirefr");
  }
  if (tooclosebc) {
    gMes->M(Warning_M,6,"epsb ~= epsc ! You should call IsUniAxialPosBirefr instead of IsBirefr");
  }
  fEpsa  = epsa;
  fEpsb  = epsb;
  fEpsc  = epsc;
  fEpsaM = constant;
  fEpsbM = constant;
  fEpscM = constant;
}
void TOpticMaterial::IsBirefr(Double_t ns0a,Double_t ns1a,Double_t ls1a,
  Double_t ns0b,Double_t ns1b,Double_t ls1b,Double_t ns0c,Double_t ns1c,Double_t ls1c) {
  //  To be called when the material is totally anisotropic (birefringent) :
  //                   epsa < epsb < epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance of
  //of IsBirefr handles the case where epsa, epsb and epsc vary according
  //to the Sellmeier law with exactly 1 term in the summation. If you need
  //more than 1 term in the summation, either for epsa or for epsc or for
  //epsc or for all, look at the next instance of IsBirefr. If you need
  //information about the Sellmeier law, look at the class TSellmeier. The
  //parameters of type lsi are resonant wavelengths that are to be given in
  //nanometers. The parameters of type nsi have no dimension and are of "kind"
  //index of refraction.
  //
  //  The parameters are the following :
  //
  // for epsa :
  //
  // ns0a       : 1st constant term, associated with wavelength 0
  // (ns1a,ls1a): 1st couple of value (nsi,lsi) is mandatory
  //
  // for epsb :
  //
  // ns0b       : 1st constant term, associated with wavelength 0
  // (ns1b,ls1b): 1st couple of value (nsi,lsi) is mandatory
  //
  // for epsc :
  //
  // ns0c       : 1st constant term, associated with wavelength 0
  // (ns1c,ls1c): 1st couple of value (nsi,lsi) is mandatory
  //
  fAnisotropic = 3;
  fEpsaM = sellmeier;
  fEpsbM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = new TSellmeier(1,ns0a,ns1a,ls1a);
  fEpsbS = new TSellmeier(1,ns0b,ns1b,ls1b);
  fEpscS = new TSellmeier(1,ns0c,ns1c,ls1c);
  fEpsa  = 1.0;    //means initialization ok for Epsa
  fEpsb  = 1.0;    //means initialization ok for Epsb
  fEpsc  = 1.0;    //means initialization ok for Epsc
}
void TOpticMaterial::IsBirefr(TSellmeier *pa,TSellmeier *pb,TSellmeier *pc) {
  //  To be called when the material is totally anisotropic (birefringent) :
  //                   epsa < epsb < epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance
  //of IsBirefr handles the case where epsa, epsb and epsc vary according
  //to the Sellmeier law and you need more than 1 couple of values (nsi,lsi),
  //for epsa, epsb or epsc or all. In that case, it is up to you to define 3
  //pointer pa, pb and pc to class TSellmeier, which is easy if you look at
  //the documentation associated with the class TSellmeier. For instance, let
  //us suppose you want a Sellmeier formula with
  //5 terms in the sum for epsa, 3 terms for epsb and 2 terms for epsc. You
  //can proceed like this in CINT (assuming pmat is your pointer towards the
  //TOpticMaterial) :
  //
  //  TSellmeier *pa,*pb,*pc;
  //  pa = new TSellmeier(3,ns0a,ns1a,ls1a,ns2a,ls2a,ns3a,ls3a);
  //  pa->AddComponent(ns4a,ls4a);
  //  pa->AddComponent(ns5a,ls5a);
  //  pb = new TSellmeier(3,ns0b,ns1b,ls1b,ns2b,ls2b,ns3b,ls3b);
  //  pc = new TSellmeier(2,ns0c,ns1c,ls1c,ns2c,ls2c);
  //  pmat->IsBirefr(pa,pb,pc);
  //  
  //  Do NOT delete pa, pb or pc !
  //
  fAnisotropic = 3;
  fEpsaM = sellmeier;
  fEpsbM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = pa;
  fEpsbS = pb;
  fEpscS = pc;
  fEpsa  = 1.0;    //means initialization ok for Epsa
  fEpsb  = 1.0;    //means initialization ok for Epsb
  fEpsc  = 1.0;    //means initialization ok for Epsc
}
void TOpticMaterial::IsIsotropic(const char *nameIndex) {
  //  To be called when the material is optically isotropic : the dielectric
  //constant epsilon is not a tensor. This instance of IsIsotropic handles
  //the case where the index of refraction varies with wavelength according
  //to a fit of type TSplineFit defined by the user. The argument is the name of
  //the fit giving the index of refraction as a function of wavelength for this
  //material.
  fAnisotropic = 0;
  TString s;
  fIndexM = byfit;
  fFitIndex = TSplineFit::FindFit(nameIndex,1);
  if (fFitIndex) {
    fNameIndex = nameIndex;
    fIndex = 1.0;    //means initialization ok for index
  }
  else {
    gMes->SetName("IsIsotropic");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(Error_M,1,"Fit for index of refraction not found for :");
    gMes->M(Info_M,1,nameIndex);
    gMes->M(Info_M,1,"Define fit for index of refraction before defining optical material");
    return;
  }
}
void TOpticMaterial::IsIsotropic(Double_t index) {
  //  To be called when the material is optically isotropic : the dielectric
  //constant epsilon is not a tensor. This instance of IsIsotropic handles
  //the case where the index of refraction does not vary with wavelength.
  //The argument is the fixed value of the index of refraction.
  const Double_t un = 1.0;
  fAnisotropic = 0;
  fIndexM = constant;
  index = TMath::Abs(index);
  if (index<un) {
    gMes->SetName("IsIsotropic");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(Error_M,2,"Index of refraction of a material must be >1",index);
    gMes->M(Info_M,2,"Call IsIsotropic again with good index");
    fIndex = un;
  }
  else fIndex = index;
}
void TOpticMaterial::IsIsotropic(Int_t n,Double_t ns0,Double_t ns1,Double_t ls1,
  Double_t ns2,Double_t ls2,Double_t ns3,Double_t ls3) {
  //  To be called when the material is optically isotropic : the dielectric
  //constant epsilon is not a tensor. This instance of IsIsotropic handles
  //the case where the index of refraction varies according to the Sellmeier
  //law with at most 3 terms ( and at least 1 ) in the summation. If you need
  //more than 3 terms in the summation, look at the next instance of
  //IsIsotropic. If you need information about the Sellmeier law, look at the
  //class TSellmeier. The parameters of type lsi are resonant wavelengths
  //that are to be given in nanometers. The parameters of type nsi have no
  //dimension and are of "kind" index of refraction.
  //
  //  The parameters are the following :
  //
  // n        : nb. of couples (nsi,lsi) used. nsi : constant in numerator
  //             1 <= n <= 3                   lsi : resonant wavelength in nm
  // ns0      : 1st constant term, associated with wavelength 0
  // (ns1,ls1): 1st couple of value (nsi,lsi) is mandatory
  // (ns2,ls2): 2nd couple, mandatory only if n >=2.
  // (ns3,ls3): 3rd couple, mandatory only if n >=3.
  //
  fAnisotropic = 0;
  fIndexM = sellmeier;
  fIndexS = new TSellmeier(n,ns0,ns1,ls1,ns2,ls2,ns3,ls3);
  fIndex  = 1.0;    //means initialization ok for index
}
void TOpticMaterial::IsIsotropic(TSellmeier *p) {
  //  To be called when the material is optically isotropic : the dielectric
  //constant epsilon is not a tensor. This instance of IsIsotropic handles
  //the case where the index of refraction varies according to the Sellmeier
  //law and you need more than 3 couples of values (nsi,lsi). In that case, it
  //is up to you to define a pointer to a class TSellmeier, which is easy if
  //you look at the class TSellmeier. For instance, let us suppose you want
  //a Sellmeier formula with 5 terms in the sum. You can proceed like this in
  //CINT (assuming pmat is your pointer towards the TOpticMaterial) :
  //
  //  TSellmeier *p;
  //  p = new TSellmeier(3,ns0,ns1,ls1,ns2,ls2,ns3,ls3);
  //  p->AddComponent(ns4,ls4);
  //  p->AddComponent(ns5,ls5);
  //  pmat->IsIsotropic(p);
  //  
  //  Do NOT delete p !
  //
  fAnisotropic = 0;
  fIndexM = sellmeier;
  fIndexS = p;
  fIndex  = 1.0;    //means initialization ok for index
}
void TOpticMaterial::IsUniAxialNegBirefr(const char *nameEpsa,const char *nameEpsc) {
  // To be called when the material is "Uniaxial negative birefringent" :
  //               epsa < epsb = epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention.
  // This instance of IsUniAxialNegBirefr handles the case where  epsa and
  //epsc varies with wavelength according to fits of type TSplineFit provided
  //by the user. epsa and epsc are the 2 diagonal elements of the dielectric
  //tensor in the coordinate system where it is diagonal. nameEpsa and nameEpsc
  //are the names of fits (of class TSplineFit) giving epsa and epsc as a function
  //of wavelength.
  fAnisotropic = 1;
  TString s;
  gMes->SetName("IsUniAxialNegBirefr");
  gMes->SetTitle("TOpticMaterial");
  fFitEpsa = TSplineFit::FindFit(nameEpsa,2);
  if (fFitEpsa) {
    fNameEpsa = nameEpsa;
    fEpsaM    = byfit;
    fEpsa     = 1.0;    //means initialization ok for Epsa
  }
  else {
    gMes->M(Error_M,1,"Fit for low component Epsa not found");
    gMes->M(Info_M,1,nameEpsa);
    gMes->M(Info_M,1,"Define fit for Epsa before defining optical material");
    return;
  }
  fFitEpsc = TSplineFit::FindFit(nameEpsc,2);
  if (fFitEpsc) {
    fNameEpsc = nameEpsc;
    fEpscM    = byfit;
    fEpsc     = 1.0;    //means initialization ok for Epsc
  }
  else {
    gMes->M(Error_M,2,"Fit for high component Epsc not found");
    gMes->M(Info_M,2,nameEpsc);
    gMes->M(Info_M,2,"Define fit for Epsc before defining optical material");
    return;
  }
}
void TOpticMaterial::IsUniAxialNegBirefr(Double_t epsa,Double_t epsc) {
  //  To be called when the material is "Uniaxial negative birefringent" :
  //                epsa < epsb = epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. The first argument
  //MUST be the smallest, the last argument MUST be the biggest.
  // This instance of IsUniAxialNegBirefr handles the case where epsa and
  //epsc do not vary with wavelength. epsa and epsc are the 2 diagonal
  //elements of the dielectric tensor in the coordinate system where it is
  //diagonal.
  // The following only applies if dielectric tensor AND absorption length
  //are defined as anisotropic:
  // Notice the following restriction: if you later on intend to define
  //the dielectric tensor (by a call to IsBirefr or IsUniAxial...) as NOT
  //depending upon wavelength, but fixed, DO NOT use anisotropic absorption
  //length depending upon wavelength.
  // In the anisotropic case, if the dielectric tensor does not depend upon
  //wavelength, the absorption length MUST NOT depend upon wavelength. You
  //can easily overcome this limitation by defining the dielectric tensor
  //as depending upon wavelength (as a fit of type TSplineFit), but setting the
  //degree of the fit to be 0!
  //
  fAnisotropic = 1;
  epsa = TMath::Abs(epsa);
  epsc = TMath::Abs(epsc);
  if (epsa>=epsc) {
    gMes->SetName("IsUniAxialNegBirefr");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(Warning_M,3,"epsc must be bigger than epsa");
    gMes->M(Info_M,3,"We interchange epsa and epsc !");
    register Double_t a;
    a = epsc;
    epsc = epsa;
    epsa = a;
  }
  fEpsa  = epsa;
  fEpsc  = epsc;
  fEpsaM = constant;
  fEpscM = constant;
}
void TOpticMaterial::IsUniAxialNegBirefr(
  Double_t ns0a,Double_t ns1a,Double_t ls1a,
  Double_t ns0c,Double_t ns1c,Double_t ls1c) {
    //  To be called when the material is "Uniaxial negative birefringent" :
    //                    epsa < epsb = epsc
    //Notice that by convention we call epsa the smallest element and epsc
    //the biggest element. Do NOT change this convention. This instance
    //of IsUniAxialNegBirefr handles the case where epsa and epsc vary according
    //to the Sellmeier law with exactly 1 term in the summation. If you need
    //more than 1 term in the summation, either for epsa or for epsc or for
    //both, look at the next instance of IsUniAxialNegBirefr. If you need
    //information about the Sellmeier law, look at the class TSellmeier. The
    //parameters of type lsi are resonant wavelengths that are to be given in
    //nanometers. The parameters of type nsi have no dimension and are of "kind"
    //index of refraction.
    //
    //  The parameters are the following :
    //
    // for epsa :
    //
    // ns0a       : 1st constant term, associated with wavelength 0
    // (ns1a,ls1a): 1st couple of value (nsi,lsi) is mandatory
    //
    // for epsc :
    //
    // ns0c       : 1st constant term, associated with wavelength 0
    // (ns1c,ls1c): 1st couple of value (nsi,lsi) is mandatory
    //
    fAnisotropic = 1;
    fEpsaM = sellmeier;
    fEpscM = sellmeier;
    fEpsaS = new TSellmeier(1,ns0a,ns1a,ls1a);
    fEpsa  = 1.0;    //means initialization ok for Epsa
    fEpscS = new TSellmeier(1,ns0c,ns1c,ls1c);
    fEpsc  = 1.0;    //means initialization ok for Epsc
}
void TOpticMaterial::IsUniAxialNegBirefr(TSellmeier *pa,TSellmeier *pc) {
  //  To be called when the material is "Uniaxial negative birefringent" :
  //                    epsa < epsb = epsc
  //Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance
  //of IsUniAxialNegBirefr handles the case where epsa and epsc vary according
  //to the Sellmeier law and you need more than 1 couple of values (nsi,lsi),
  //for epsa or epsc or both. In that case, it is up to you to define 2
  //pointer pa, pb to class TSellmeier, which is easy if you look at the class
  //TSellmeier. For instance, let us suppose you want a Sellmeier formula with
  //5 terms in the sum for epsa and 2 terms for epsc. You can proceed like this
  //in CINT (assuming pmat is your pointer towards the TOpticMaterial) :
  //
  //  TSellmeier *pa,*pc;
  //  pa = new TSellmeier(3,ns0a,ns1a,ls1a,ns2a,ls2a,ns3a,ls3a);
  //  pa->AddComponent(ns4a,ls4a);
  //  pa->AddComponent(ns5a,ls5a);
  //  pc = new TSellmeier(2,ns0c,ns1c,ls1c,ns2c,ls2c);
  //  pmat->IsUniAxialNegBirefr(pa,pc);
  //  
  //  Do NOT delete pa nor pc !
  //
  fAnisotropic = 1;
  fEpsaM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = pa;
  fEpscS = pc;
  fEpsa  = 1.0;    //means initialization ok for Epsa
  fEpsc  = 1.0;    //means initialization ok for Epsc
}
void TOpticMaterial::IsUniAxialPosBirefr(const char *nameEpsa,const char *nameEpsc) {
  //  To be called when the material is "Uniaxial positive birefringent" :
  //                    epsa = epsb < epsc
  //Notice that by convention we call epsa the smallest element and epsc the
  //biggest element. Do NOT change this convention.
  //This instance of IsUniAxialPosBirefr handles the case where epsa and epsc
  //vary with wavelength according to fits of type TSplineFit provided by the user.
  //epsa and epsc are the 2 diagonal elements of the dielectric tensor in the
  //coordinate system where it is diagonal, nameEpsa and nameEpsc are the
  //names of fits (of class TSplineFit) giving epsa and epsc as a function of
  //wavelength.
  //
  fAnisotropic = 2;
  TString s;
  gMes->SetName("IsUniAxialPosBirefr");
  gMes->SetTitle("TOpticMaterial");
  fFitEpsa = TSplineFit::FindFit(nameEpsa,2);
  if (fFitEpsa) {
    fNameEpsa = nameEpsa;
    fEpsaM    = byfit;
    fEpsa     = 1.0;    //means initialization ok for Epsa
  }
  else {
    gMes->M(Error_M,1,"Fit for low component Epsa not found");
    gMes->M(Info_M,1,nameEpsa);
    gMes->M(Info_M,1,"Define fit for Epsa before defining optical material");
    return;
  }
  fFitEpsc = TSplineFit::FindFit(nameEpsc,2);
  if (fFitEpsc) {
    fNameEpsc = nameEpsc;
    fEpscM    = byfit;
    fEpsc     = 1.0;    //means initialization ok for Epsc
  }
  else {
    gMes->M(Error_M,2,"Fit for high component Epsc not found");
    gMes->M(Info_M,2,nameEpsc);
    gMes->M(Info_M,2,"Define fit for Epsc before defining optical material");
    return;
  }
}
void TOpticMaterial::IsUniAxialPosBirefr(Double_t epsa,Double_t epsc) {
  //  To be called when the material is "UniAxial positive birefringent" :
  //                     epsa = epsb < epsc
  //Notice that by convention we call epsa the smallest element and epsc the
  //biggest element. Do NOT change this convention.
  // This instance of IsUniAxialPosBirefr handles the case where epsa and epsc
  //do not vary with wavelength. epsa and epsc are the 2 diagonal elements of
  //the dielectric tensor in the coordinate system where it is diagonal.
  // The following only applies if dielectric tensor AND absorption length
  //are defined as anisotropic:
  // Notice the following restriction: if you later on intend to define
  //the dielectric tensor (by a call to IsBirefr or IsUniAxial...) as NOT
  //depending upon wavelength, but fixed, DO NOT use anisotropic absorption
  //length depending upon wavelength.
  // In the anisotropic case, if the dielectric tensor does not depend upon
  //wavelength, the absorption length MUST NOT depend upon wavelength. You
  //can easily overcome this limitation by defining the dielectric tensor
  //as depending upon wavelength (as a fit of type TSplineFit), but setting the
  //degree of the fit to be 0!
  //
  fAnisotropic = 2;
  epsa = TMath::Abs(epsa);
  epsc = TMath::Abs(epsc);
  if (epsa>=epsc) {
    gMes->SetName("IsUniAxialPosBirefr");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(Warning_M,3,"epsc must be bigger than epsa");
    gMes->M(Info_M,3,"We interchange epsa and epsc !");
    register Double_t a;
    a = epsc;
    epsc = epsa;
    epsa = a;
  }
  fEpsa  = epsa;
  fEpsc  = epsc;
  fEpsaM = constant;
  fEpscM = constant;
}
void TOpticMaterial::IsUniAxialPosBirefr(Double_t ns0a,Double_t ns1a,Double_t ls1a,
  Double_t ns0c,Double_t ns1c,Double_t ls1c) {
  //  To be called when the material is "Uniaxial positive birefringent" :
  //                      epsa = epsb < epsc
  // Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance
  //of IsUniAxialPosBirefr handles the case where epsa and epsc vary according
  //to the Sellmeier law with exactly 1 term in the summation. If you need
  //more than 1 term in the summation, either for epsa or for epsc or for
  //both, look at the next instance of IsUniAxialPosBirefr. If you need
  //information about the Sellmeier law, look at the class TSellmeier. The
  //parameters of type lsi are resonant wavelengths that are to be given in
  //nanometers. The parameters of type nsi have no dimension and are of "kind"
  //index of refraction.
  //
  //  The parameters are the following :
  //
  // for epsa :
  //
  // ns0a       : 1st constant term, associated with wavelength 0
  // (ns1a,ls1a): 1st couple of value (nsi,lsi) is mandatory
  //
  // for epsc :
  //
  // ns0c       : 1st constant term, associated with wavelength 0
  // (ns1c,ls1c): 1st couple of value (nsi,lsi) is mandatory
  //
  fAnisotropic = 2;
  fEpsaM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = new TSellmeier(1,ns0a,ns1a,ls1a);
  fEpscS = new TSellmeier(1,ns0c,ns1c,ls1c);
  fEpsa     = 1.0;    //means initialization ok for Epsa
  fEpsc     = 1.0;    //means initialization ok for Epsc
}
void TOpticMaterial::IsUniAxialPosBirefr(TSellmeier *pa,TSellmeier *pc) {
  // To be called when the material is "Uniaxial positive birefringent" :
  //                    epsa = epsb < epsc
  // Notice that by convention we call epsa the smallest element and epsc
  //the biggest element. Do NOT change this convention. This instance
  //of IsUniAxialNegBirefr handles the case where epsa and epsc vary according
  //to the Sellmeier law and you need more than 1 couple of values (nsi,lsi),
  //for epsa or epsc or both. In that case, it is up to you to define 2
  //pointer pa, pb to class TSellmeier, which is easy if you look at the class
  //TSellmeier. For instance, let us suppose you want a Sellmeier formula with
  //5 terms in the sum for epsa and 2 terms for epsc. You can proceed like this
  //in CINT (assuming pmat is your pointer towards the TOpticMaterial) :
  //
  //  TSellmeier *pa,*pc;
  //  pa = new TSellmeier(3,ns0a,ns1a,ls1a,ns2a,ls2a,ns3a,ls3a);
  //  pa->AddComponent(ns4a,ls4a);
  //  pa->AddComponent(ns5a,ls5a);
  //  pc = new TSellmeier(2,ns0c,ns1c,ls1c,ns2c,ls2c);
  //  pmat->IsUniAxialPosBirefr(pa,pc);
  //  
  //  Do NOT delete pa nor pc !
  //
  fAnisotropic = 2;
  fEpsaM = sellmeier;
  fEpscM = sellmeier;
  fEpsaS = pa;
  fEpscS = pc;
  fEpsa     = 1.0;    //means initialization ok for Epsa
  fEpsc     = 1.0;    //means initialization ok for Epsc
}
void TOpticMaterial::NewPhiMu(Double_t wavelength) {
  //  Calculates the imaginary parts fPhimua/b/c of the diagonal elements
  //fEpsa/b/c of the dielectric tensor from the values of the 3 wavelength
  //fAbsLA/b/c.  wavelength is in nanometers.
  const Double_t quatre  = 4.0;
  const Double_t In_cm_s = 100.0;
  const Double_t In_cm   = 1.0e-7;
  Double_t c;       //speed of light in [cm/s]
  Double_t c2;      //square of it
  Double_t w;       // 2*pi*frequency
  Double_t w2,w4;   //w^2, w^4
  Double_t L;       //wavelength in [cm]
  Double_t b;     //for intermediate calculations
  Double_t la2,la4; //absorption length^2 or ^4
  //calculates w
  c  = In_cm_s*TLitPhys::Get()->Cmks();  //speed of light in cm/s
  c2 = c*c;
  L  = In_cm*wavelength;       //wavelength in cm
  w  = TMath::TwoPi();
  w  = (w*c)/L;
  w2 = w*w;
  w4 = w2*w2;
  b  = quatre*fMu*w2;
  //fPhimua
  la2     = fAbsLa*fAbsLa;
  la4     = la2*la2;
  fPhimua = TMath::Sqrt((c2*(c2 + b*fEpsa*la2))/(quatre*w4*la4));
  //fPhimub
  la2     = fAbsLb*fAbsLb;
  la4     = la2*la2;
  fPhimub = TMath::Sqrt((c2*(c2 + b*fEpsb*la2))/(quatre*w4*la4));
  //fPhimuc
  la2     = fAbsLc*fAbsLc;
  la4     = la2*la2;
  fPhimuc = TMath::Sqrt((c2*(c2 + b*fEpsc*la2))/(quatre*w4*la4));
}
void TOpticMaterial::NewWavelength(Double_t wavelength) {
  //Recalculates all parameters depending upon wavelength. wavelength is in
  //nanometers.
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  if (fFitMu)
    fMu = TMath::Abs(fFitMu->V(wavelength));
  if (fFitAbsLa) {
    fAbsLa = fFacAbsL*fFitAbsLa->V(wavelength);
    if (fFacDiffus > zero) fDiffusLength = fFacDiffus*fAbsLa;
  }
  switch (fAniAbsL) {
  case 1:
    if (fFitAbsLb) fAbsLb = fFacAbsL*fFitAbsLb->V(wavelength);
    fAbsLc = fAbsLb;
    break;
  case 2:
    fAbsLb = fAbsLa;
    if (fFitAbsLc) fAbsLc = fFacAbsL*fFitAbsLc->V(wavelength);
    break;
  case 3:
    if (fFitAbsLb) fAbsLb = fFacAbsL*fFitAbsLb->V(wavelength);
    if (fFitAbsLc) fAbsLc = fFacAbsL*fFitAbsLc->V(wavelength);
    break;
  default:
    fAbsLb = fAbsLa;
    fAbsLc = fAbsLa;
    break;
  }
  switch (fAnisotropic) {
  case -1:
    gMes->SetName("NewWavelength");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,1,"optical properties not defined !");
    break;
  case  0:
    switch (fIndexM) {
  case undefined:
    gMes->SetName("NewWavelength");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,2,"method for index not defined");
    break;
  case constant:
    break;
  case byfit:
    fIndex = TMath::Abs(fFitIndex->V(wavelength));
    if (fIndex<un) fIndex = un;
    break;
  case sellmeier:
    fIndex = fIndexS->GetIndex(wavelength,fMu);
    if (fIndex<un) fIndex = un;
    break;
    }
    break;
  case  3:
    switch (fEpsbM) {
  case undefined:
    gMes->SetName("NewWavelength");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,3,"method for Epsb not defined");
    break;
  case constant:
    break;
  case byfit:
    fEpsb = TMath::Abs(fFitEpsb->V(wavelength));
    break;
  case sellmeier:
    fEpsb = fEpsbS->GetDiel(wavelength);
    break;
    }
  case  1:
  case  2:
    switch (fEpsaM) {
  case undefined:
    gMes->SetName("NewWavelength");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,4,"method for Epsa not defined");
    break;
  case constant:
    break;
  case byfit:
    fEpsa = TMath::Abs(fFitEpsa->V(wavelength));
    break;
  case sellmeier:
    fEpsa = fEpsaS->GetDiel(wavelength);
    break;
    }
    switch (fEpscM) {
  case undefined:
    gMes->SetName("NewWavelength");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,5,"method for Epsc not defined");
    break;
  case constant:
    break;
  case byfit:
    fEpsc = TMath::Abs(fFitEpsc->V(wavelength));
    break;
  case sellmeier:
    fEpsc = fEpscS->GetDiel(wavelength);
    break;
    }
    break;
  };
  if (fAnisotropic==1) fEpsb = fEpsc;
  if (fAnisotropic==2) fEpsb = fEpsa;
  if (fAniAbsL) NewPhiMu(wavelength);

  if (fHasDiffusion && fFitDiffusL) fDiffusLength = TMath::Abs(fFitDiffusL->V(wavelength));
  if (fHasRayleigh && fFitRayleighL) fRayleighLength = TMath::Abs(fFitRayleighL->V(wavelength));
  if (fHasWLS && fFitWLSL) fWLSL = TMath::Abs(fFitWLSL->V(wavelength));
  return;
}
Int_t TOpticMaterial::NPhotInteger(Double_t nphot) const {
  // Converts a Double_t to an Int_t, randomly rounding up or down. Used to 
  Int_t iphot = (Int_t)nphot;
  if (gRandom3->Rndm()<= (nphot - iphot)) ++iphot;
  return iphot;
}
void TOpticMaterial::Print() const {
  //Print all characteristics of material
  PrintTitle();
  gMes->SetName("Print");
  gMes->SetTitle("TOpticMaterial");
  //detector characteristics
  if (fSensible)
    gMes->M(Info_M,1,"material is a detector of photons");
  else
    gMes->M(Info_M,1,"material is not a detector of photons");
  //magnetic permeability
  if (fFitMu) {
    gMes->M(Info_M,2,"magnetic permeability varies with wavelength");
    gMes->M(Info_M,2,"name of fit",fNameMu);
  }
  else
    gMes->M(Info_M,2,"magnetic permeability fixed",fMu);
  //absorption lengths
  switch (fAniAbsL) {
  case 0:
    gMes->M(Info_M,3,"absorption length isotropic");
    if (fFitAbsLa) {
      gMes->M(Info_M,3,"absorption length varies with wavelength");
      gMes->M(Info_M,3,"name of fit",fNameAbsLa);
    }
    else {
      gMes->M(Info_M,3,"absorption length fixed",fAbsLa);
    }
    break;
  case 1:
    gMes->M(Info_M,4,"absorption length anisotropic");
    gMes->M(Info_M,4,"La <> Lb = Lc");
    if (fFitAbsLa) {
      gMes->M(Info_M,4,"La, Lb and Lc vary with wavelength");
      gMes->M(Info_M,4,"name of fit for La",fNameAbsLa);
      gMes->M(Info_M,4,"name of fit for Lb and Lc",fNameAbsLb);
    }
    else {
      gMes->M(Info_M,4,"La fixed",fAbsLa);
      gMes->M(Info_M,4,"Lb fixed",fAbsLb);
      gMes->M(Info_M,4,"Lc fixed",fAbsLc);
    }
    break;
  case 2:
    gMes->M(Info_M,5,"absorption length anisotropic");
    gMes->M(Info_M,5,"La = Lb <> Lc");
    if (fFitAbsLa) {
      gMes->M(Info_M,5,"La, Lb and Lc vary with wavelength");
      gMes->M(Info_M,5,"name of fit for La",fNameAbsLa);
      gMes->M(Info_M,5,"name of fit for Lb and Lc",fNameAbsLc);
    }
    else {
      gMes->M(Info_M,5,"La fixed",fAbsLa);
      gMes->M(Info_M,5,"Lb fixed",fAbsLb);
      gMes->M(Info_M,5,"Lc fixed",fAbsLc);
    }
    break;
  case 3:
    gMes->M(Info_M,6,"absorption length anisotropic");
    gMes->M(Info_M,6,"La <> Lb <> Lc");
    if (fFitAbsLa) {
      gMes->M(Info_M,6,"La, Lb and Lc vary with wavelength");
      gMes->M(Info_M,6,"name of fit for La",fNameAbsLa);
      gMes->M(Info_M,6,"name of fit for Lb",fNameAbsLb);
      gMes->M(Info_M,6,"name of fit for Lc",fNameAbsLc);
    }
    else {
      gMes->M(Info_M,6,"La fixed",fAbsLa);
      gMes->M(Info_M,6,"Lb fixed",fAbsLb);
      gMes->M(Info_M,6,"Lc fixed",fAbsLc);
    }
    break;
  }
  //optical properties
  cout << "Optical properties : ";
  switch ( fAnisotropic ) {
  case  0 :
    gMes->M(Info_M,7,"material optically isotropic");
    switch (fIndexM) {
  case undefined:
    gMes->M(Error_M,7,"method to get index undefined");
    break;
  case constant:
    gMes->M(Info_M,7,"index of refraction fixed",fIndex);
    break;
  case byfit:
    gMes->M(Info_M,7,"index of refraction varies with wavelength");
    gMes->M(Info_M,7,"name of fit",fNameIndex);
    break;
  case sellmeier:
    gMes->M(Info_M,7,"index of refraction by Sellmeier law");
    break;
    }
    break;
  case  1 :
    gMes->M(Info_M,8,"material uniaxial negative birefringent");
    switch (fEpsaM) {
  case undefined:
    gMes->M(Error_M,8,"method to get epsa undefined");
    break;
  case constant:
    gMes->M(Info_M,8,"1st diagonal element epsa fixed",fEpsa);
    break;
  case byfit:
    gMes->M(Info_M,8,"1st diagonal element epsa varies with wavelength");
    gMes->M(Info_M,8,"name of fit",fNameEpsa);
    break;
  case sellmeier:
    gMes->M(Info_M,8,"1st diagonal element epsa by Sellmeier law");
    break;
    }
    switch (fEpscM) {
  case undefined:
    gMes->M(Error_M,8,"method to get epsc undefined");
    break;
  case constant:
    gMes->M(Info_M,8,"2nd diagonal element epsb fixed",fEpsb);
    gMes->M(Info_M,8,"3rd diagonal element epsc fixed",fEpsc);
    break;
  case byfit:
    gMes->M(Info_M,8,"2nd and 3rd diag. el. epsb/c vary with wavelength");
    gMes->M(Info_M,8,"name of fit",fNameEpsc);
    break;
  case sellmeier:
    gMes->M(Info_M,8,"2nd and 3rd diag. el. epsb/c by Sellmeier law");
    break;
    }
    break;
  case  2 :
    gMes->M(Info_M,9,"material uniaxial positive birefringent");
    switch (fEpsaM) {
  case undefined:
    gMes->M(Error_M,9,"method to get epsa and epsb undefined");
    break;
  case constant:
    gMes->M(Info_M,9,"1st diagonal element epsa fixed",fEpsa);
    gMes->M(Info_M,9,"2nd diagonal element epsb fixed",fEpsb);
    break;
  case byfit:
    gMes->M(Info_M,9,"1st and 2nd diag. el. epsa/b vary with wavelength");
    gMes->M(Info_M,9,"name of fit",fNameEpsa);
    break;
  case sellmeier:
    gMes->M(Info_M,9,"1st and 2nd diag. el. epsa/b by Sellmeier law");
    break;
    }
    switch (fEpscM) {
  case undefined:
    gMes->M(Error_M,9,"method to get epsc undefined");
    break;
  case constant:
    gMes->M(Info_M,9,"3rd diagonal element epsc fixed",fEpsc);
    break;
  case byfit:
    gMes->M(Info_M,9,"3rd diagonal element epsc vary with wavelength");
    gMes->M(Info_M,9,"name of fit",fNameEpsc);
    break;
  case sellmeier:
    gMes->M(Info_M,9,"rd diagonal element epsc by Sellmeier law");
    break;
    }
    break;
  case  3 :
    gMes->M(Info_M,10,"material birefringent, i.e. totally anisotropic");
    switch (fEpsaM) {
  case undefined:
    gMes->M(Error_M,10,"method to get epsa undefined");
    break;
  case constant:
    gMes->M(Info_M,10,"1st diagonal element epsa fixed",fEpsa);
    break;
  case byfit:
    gMes->M(Info_M,10,"1st diagonal element epsa varies with wavelength");
    gMes->M(Info_M,10,"name of fit",fNameEpsa);
    break;
  case sellmeier:
    gMes->M(Info_M,10,"1st diagonal element epsa by Sellmeier law");
    break;
    }
    switch (fEpsbM) {
  case undefined:
    gMes->M(Error_M,10,"method to get epsb undefined");
    break;
  case constant:
    gMes->M(Info_M,10,"2nd diagonal element epsb fixed",fEpsb);
    break;
  case byfit:
    gMes->M(Info_M,10,"2nd diagonal element epsb varies with wavelength");
    gMes->M(Info_M,10,"name of fit",fNameEpsb);
    break;
  case sellmeier:
    gMes->M(Info_M,10,"2nd diagonal element epsb by Sellmeier law");
    break;
    }
    switch (fEpscM) {
  case undefined:
    gMes->M(Error_M,10,"method to get epsc undefined");
    break;
  case constant:
    gMes->M(Info_M,10,"3rd diagonal element epsc fixed",fEpsc);
    break;
  case byfit:
    gMes->M(Info_M,10,"3rd diagonal element epsc vary with wavelength");
    gMes->M(Info_M,10,"name of fit",fNameEpsc);
    break;
  case sellmeier:
    gMes->M(Info_M,10,"3rd diagonal element epsc by Sellmeier law");
    break;
    }
    break;
  default:
    gMes->M(Warning_M,11,"optical characteristics undefined");
    break;
  }
  switch (fdEdxUsed) {
  case nodEdx:
    gMes->M(Info_M,12,"material optically inactiv when crossed by particle");
    break;
  case WithoutLandau:
    gMes->M(Info_M,12,"material optically activ when crossed by particle");
    gMes->M(Info_M,12,"number of photons emitted per deposited Mev",fPhotMev);
    gMes->M(Info_M,12,"name of fit for dE/dx versus E",fNamedEdx);
    break;
  case WithLandau:
    gMes->M(Info_M,12,"material optically activ when crossed by particle");
    gMes->M(Info_M,12,"number of photons emitted per deposited Mev",fPhotMev);
    gMes->M(Info_M,12,"name of family of fits for dE/dx with Landau",fNamedEdx);
    break;
  }
  if (fInUse)
    gMes->M(Info_M,17,"material in use");
  else
    gMes->M(Info_M,17,"material not in use");

  if (fHasNPLY) {
    gMes->M(Info_M,18,"material has non-proportionality of light yield");
  }
  gMes->M(Info_M,19,"The Resolution Scale of this material is:", fResolutionScale);
}
void TOpticMaterial::PrintProblem() const {
  //Header to point out a problem
  cout << endl;
  cout << "THERE IS A PROBLEM IN THE FOLLOWING OpticalMaterial :" << endl;
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "THERE IS A PROBLEM IN THE FOLLOWING OpticalMaterial :" << endl;
  *gMes->fListing << endl;
}
void TOpticMaterial::PrintTitle() const {
  //Prints name and title of the material
  cout << endl;
  cout << "    OpticMaterial : " << fName.Data() << endl;
  cout << "    Title         : " << fTitle.Data() << endl;
  cout << endl;
  //listing
  *gMes->fListing << endl;
  *gMes->fListing << "    OpticMaterial : " << fName.Data() << endl;
  *gMes->fListing << "    Title         : " << fTitle.Data() << endl;
  *gMes->fListing << endl;
}
void TOpticMaterial::ProblemHeader() const {
  //header before reporting a problem
  PrintProblem();
  PrintTitle();
  return;
}
void TOpticMaterial::SetAbsLa(Double_t absLa,Bool_t destroy) {
  // To change the absorption length versus wavelength (or in case of anisotropy,
  //the part of the absorption length linked with epsa) for this material.
  //Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is fixed, not varying with wavelength and has the
  //value absLa
  // If destroy is true, the preceeding fit (if it was a fit, not a constant value)
  //in use will be deleted. Default false.
  //
  fAbsLa = absLa;
  if (fFitAbsLa) {
    if (destroy) delete fFitAbsLa;
    fFitAbsLa = 0;
  }
  fHasAbsorption = kTRUE;
}
void TOpticMaterial::SetAbsLa(const char *nameAbsLa,Bool_t destroy) {
  // To change the absorption length versus wavelength (or in case of anisotropy,
  //the part of the absorption length linked with epsa) for this material.
  //Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is varying with wavelength according to the fit
  //TSplineFit named nameAbsLa.
  // If destroy is true, the preceeding fit in use will be deleted. Default false.
  //
  const Double_t un = 1.0;
  TString s;
  if (fFitAbsLa && destroy) delete fFitAbsLa;
  fFitAbsLa = TSplineFit::FindFit(nameAbsLa);
  if (fFitAbsLa) {
    fNameAbsLa = nameAbsLa;
    fAbsLa     = un;    //means initialization ok for Abs. length
    fHasAbsorption = kTRUE;
  }
  else {
    gMes->M(Error_M,21,"Fit for absorption length fAbsLa not found");
    gMes->M(Info_M,21,nameAbsLa);
    gMes->M(kill_prog,21,"Define fit for absorption length before material");
    return;
  }
}
void TOpticMaterial::SetAbsLb(Double_t absLb,Bool_t destroy) {
  // To change the part of the absorption length linked with epsb) for this
  //anisotropic material.
  // Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is fixed, not varying with wavelength and has the
  //value absLb
  // If destroy is true, the preceeding fit (if it was a fit, not a constant value)
  //in use will be deleted. Default false.
  //
  fAbsLb = absLb;
  if (fFitAbsLb) {
    if (destroy) delete fFitAbsLb;
    fFitAbsLb = 0;
  }
}
void TOpticMaterial::SetAbsLb(const char *nameAbsLb,Bool_t destroy) {
  // To change the part of the absorption length linked with epsb for this
  //anisotropic material.
  // Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is varying with wavelength according to the fit
  //TSplineFit named nameAbsLb.
  // If destroy is true, the preceeding fit in use will be deleted. Default false.
  //
  const Double_t un = 1.0;
  if (fFitAbsLb && destroy) delete fFitAbsLb;
  fFitAbsLb = TSplineFit::FindFit(nameAbsLb);
  if (fFitAbsLb) {
    fNameAbsLb = nameAbsLb;
    fAbsLb     = un;    //means initialization ok for Abs. length
  }
  else {
    gMes->M(Error_M,21,"Fit for absorption length fAbsLb not found");
    gMes->M(Info_M,21,nameAbsLb);
    gMes->M(kill_prog,21,"Define fit for absorption length before material");
    return;
  }
}
void TOpticMaterial::SetAbsLc(Double_t absLc,Bool_t destroy) {
  // To change the part of the absorption length linked with epsc for this
  //anisotropic material.
  // Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is fixed, not varying with wavelength and has the
  //value absLc
  // If destroy is true, the preceeding fit (if it was a fit, not a constant value)
  //in use will be deleted. Default false.
  //
  fAbsLc = absLc;
  if (fFitAbsLc) {
    if (destroy) delete fFitAbsLc;
    fFitAbsLc = 0;
  }
}
void TOpticMaterial::SetAbsLc(const char *nameAbsLc,Bool_t destroy) {
  // To change the part of the absorption length linked with epsc for this
  //anisotropic material.
  // Useful when one wants to make a sery of runs with different absorption length
  //for each run.
  // The new absorption length is varying with wavelength according to the fit
  //TSplineFit named nameAbsLc.
  // If destroy is true, the preceeding fit in use will be deleted. Default false.
  //
  const Double_t un = 1.0;
  if (fFitAbsLc && destroy) delete fFitAbsLc;
  fFitAbsLc = TSplineFit::FindFit(nameAbsLc);
  if (fFitAbsLc) {
    fNameAbsLc = nameAbsLc;
    fAbsLc     = un;    //means initialization ok for Abs. length
  }
  else {
    gMes->M(Error_M,21,"Fit for absorption length fAbsLc not found");
    gMes->M(Info_M,21,nameAbsLc);
    gMes->M(kill_prog,21,"Define fit for absorption length before material");
    return;
  }
}
void TOpticMaterial::SetCanStopElectron(Double_t T0, Double_t T1, Double_t b0,
  Double_t b1, Double_t b2, Double_t b3) {
  // Material properties necessary to calculate electron ionisation dE/dX
  //See S.M. Seltzer and M.J. Berger, Evaluation of the collision stopping
  //power of elements and compounds for electrons and positrons,
  //Int. J. Appl. Radiat., 33 (1982) 1189-1218
  // This method not used for the time being
  fEP[0] = T0;
  fEP[1] = T1;
  fEP[2] = b0;
  fEP[3] = b1;
  fEP[4] = b2;
  fEP[5] = b3;
  fCanStopElectron = kTRUE;
}
void TOpticMaterial::SetCascades(Double_t x0, Double_t rm, Double_t ec) {
  //  To define :
  //
  //  x0 : radiation length  in cm
  //  rm : Moliere's radius  in cm
  //  ec : critical energy   in Mev
  //
  //  If you do not give ec, it will be calculated through the formula :
  //
  //  (23.21)  rm = (x0*es)/ec            with es = 21 Mev.
  //
  //  If you do give ec, pay attention that formula (23.21) be almost ok !
  //
  const Double_t zero   = 0.0;
  const Double_t es     = 21.0;
  const Double_t limlow = 0.25;
  const Double_t limup  = 4.0;
  Double_t ec1,ec2,rap;
  fX0 = TMath::Abs(x0);
  fRm = TMath::Abs(rm);
  ec1 = TMath::Abs(ec);
  ec2 = (fX0*es)/fRm;
  if (ec>zero)  {
    rap = ec1/ec2;
    if ((rap>limlow) && (rap<limup)) fEc = ec1;
    else {
      gMes->SetName("SetCascades");
      gMes->SetTitle("TOpticMaterial");
      gMes->M(Error_M,1,"Impossible value for critical energy",ec);
      gMes->M(Info_M,1,"We take (X0*es)/rm !",ec2);
      fEc = ec2;
    }
  }
  if (ec<=zero) fEc = ec2;
}
void TOpticMaterial::SetDiffusionLength(Double_t diffL, Bool_t isdiff) {
  //  Declares that this material has a diffusion length. In this version of
  //SetDiffusionLength(), the diffusion length may be given as a fixed value,
  //not depending upon wavelength, or the diffusion length may be obtained as
  //being equal to the absorption length multiplied by a factor :
  //
  //  - if isdiff is true (default) diffL is the diffusion length. It is fixed
  //                      not depending upon wavelength.
  //  - if isdiff is false, the diffusion length is obtained taking the absor-
  //                      ption length fAbsLa and multiplying it by the factor
  //                      diffL. the diffusion length vary as a function of
  //                      wavelength as the absorption length, multiplied by
  //                      diffL.
  //  If you want a diffusion length varying with wavelength according to a
  //fit of type TSplineFit, look at the other version of SetDiffusionLength().
  //
  //  It is observed that sometimes a material diffuses a photon in some other
  //direction. This phenomenon is due to bubbles of impurities (for instance
  //air) or to defaults in the crystal structure. Litrani does not reproduces
  //exactly the diffusion on a bubble or on a defect. The way Litrani simula-
  //tes diffusion, is by absorbing the photon and reemitting it at the same
  //place, with the same wavelength but with a new K vector, which is randomly
  //generated on 4pi. The probability of having a diffusion is handled exactly
  //the same way as the probability of having an absorption. The user has to
  //introduce a "diffusion length", similar to the "absorption length".
  //
  //  Parameters :
  //
  //   diffL    : if <=0 : there is no diffusion
  //              if > 0 : there is diffusion, and :
  //                if isdiff is true, diffL is the value of the diffusion
  //                  length, fixed, not depending upon wavelength
  //                if isdiff is false, diffL is a factor which gives the
  //                  diffusion length if multiplied by the absorption length
  //   isdiff   : see above
  //
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  fFitDiffusL   = 0;
  if (diffL<=zero) {
    fHasDiffusion = kFALSE;
    fFacDiffus    = -un;
    fDiffusLength = -un;
  }
  else {
    if (isdiff) {
      fHasDiffusion = kTRUE;
      fFacDiffus    = -un;
      fDiffusLength = diffL;
    }
    else {
      fHasDiffusion = kTRUE;
      fFacDiffus    = diffL;
      fDiffusLength = diffL*fAbsLa;
    }
  }
}
void TOpticMaterial::SetDiffusionLength(const char *name) {
  //Declares that this material has a diffusion length. The value of the dif-
  //fusion length as a function of wavelength is obtained by a fit of type
  //TSplineFit whose name is name.
  //  It is observed that sometimes a material diffuses a photon in some other
  //direction. This phenomenon is due to bubbles of impurities (for instance
  //air) or to defaults in the crystal structure. Litrani does not reproduces
  //exactly the diffusion on a bubble or on a defect. The way Litrani simula-
  //tes diffusion, is by absorbing the photon and reemitting it at the same
  //place, with the same wavelength but with a new K vector, which is randomly
  //generated on 4pi. The probability of having a diffusion is handled exactly
  //the same way as the probability of having an absorption. The user has to
  //introduce a "diffusion length", similar to the "absorption length".
  //  The diffusion length may be fixed, not depending upon wavelength, or
  //varying proportionnaly to the absorption length. In that case the other
  //version of SetDiffusionLength() has to be called.
  //  Or the diffusion length is depending upon wavelength. In that case, 
  //this version of SetDiffusionLength() has to be called.
  //
  //   const char *name : name of the fit of type TSplineFit describing the diffu-
  //                  length as a function of wavelength. If name is "" or is
  //                  omitted, it is assumed that there is no diffusion.
  const Double_t un = 1.0;
  TString s;
  TString snone = "";
  gMes->SetName("SetDiffuionLength");
  gMes->SetTitle("TOpticMaterial");
  if (!snone.CompareTo(name)) {
    fHasDiffusion = kFALSE;
    fDiffusLength = -un;
    fFitDiffusL   = 0;
  }
  else {
    fHasDiffusion = kTRUE;
    fFitDiffusL = TSplineFit::FindFit(name,7);
    if (fFitDiffusL) {
      fNameDiffusL  = name;
      fDiffusLength = un;    //means initialization ok for Diffus. length
    }
    else {
      gMes->M(Error_M,1,"Fit for diffusion length not found");
      gMes->M(Info_M,1,name);
      gMes->M(Error_M,1,"No diffusion length assumed");
      fHasDiffusion = kFALSE;
      fDiffusLength = -un;
      fFitDiffusL   = 0;
    }
  }
}
void TOpticMaterial::SetFactorOnAbsL(Double_t f) {
  //All values of absorption lengths 
  // - [fAbsLa in case of isotropic absorption length
  // - [fAbsLa,fAbsLb,fAbsLc] in case of anisotropic absorption lengths
  //will be multiplied by the factor f before being used. It is useful for
  //instance when studying the effect of a degradation of the absorption
  //length.
  Double_t g;
  f = TMath::Abs(f);
  g = f/fFacAbsL;
  if (!fFitAbsLa) fAbsLa = g*fAbsLa;
  switch (fAniAbsL) {
  case 1:
    if (!fFitAbsLb) fAbsLb = g*fAbsLb;
    fAbsLc = fAbsLb;
    break;
  case 2:
    fAbsLb = fAbsLa;
    if (!fFitAbsLc) fAbsLc = g*fAbsLc;
    break;
  case 3:
    if (!fFitAbsLb) fAbsLb = g*fAbsLb;
    if (!fFitAbsLc) fAbsLc = g*fAbsLc;
    break;
  default:
    fAbsLb = fAbsLa;
    fAbsLc = fAbsLa;
    break;
  }
  fFacAbsL = f;
}
void TOpticMaterial::SetHistFluo(Bool_t tobe) {
  //Asks that the generated spectrum for this fluorescent material be histogrammed
  //or not during the runs. Notice that in your set-up, ONLY 1 FLUORESCENT MATERIAL may
  //be histogrammed. So if you ask that this fluorescent material be histogrammed
  //it will be so that no other fluorescent material in the set-up will be histo-
  //grammed.
  if (fLitSpectrum) {
    if (GetFluorescent()) {
      if (tobe) {
        TOpticMaterial *pmat;
        TIter next(&gLit->fOptMat);
        while ((pmat = (TOpticMaterial *)next()))
          if ((pmat->GetFluorescent()) && (pmat->fHistFluo))
            pmat->fHistFluo = kFALSE;
        fHistFluo = kTRUE;
      }
      else fHistFluo = kFALSE;
    }
  }//end if (fLitSpectrum)
}
void TOpticMaterial::SetNonPropLightYield(Text_t* name) {
  // Inorganic scintillators are often found to have a light yield which is not
  //proportional to energy of the incoming radiation particle. In the literature
  //this effect is referred to as the non-proportionality of the light yield.
  // Organic scintillators are known to have a light yield which varies with
  //ionistation density which can be accounted for using Birk's formula.
  // The SetNonPropLightYield(Text_t* name) function provides a method to
  //correct for these effects if desired
  TString s;
  TString snone = "";
  gMes->SetName("SetNonPropLightYield");
  gMes->SetTitle("TOpticMaterial");
  if (!snone.CompareTo(name)) {
    fHasNPLY = kFALSE;
    fFitNPLY   = 0;
  }
  else {
    fFitNPLY = TSplineFit::FindFit(name);
    if (fFitNPLY) {
      fNameNPLY = name;
      fHasNPLY = kTRUE;}
    else {
      gMes->M(Error_M,1,"Fit for non proportionality not found");
      gMes->M(Info_M,1,name);
      gMes->M(Error_M,1,"No non proportionality assumed");
      fHasNPLY = kFALSE;
      fFitNPLY   = 0;
    }
  }
}
void TOpticMaterial::SetPhotMev(Double_t photmev, Double_t resScale) {
  // Method to set the intrinsic light yield (photmev) and the resolution scale (resScale).
  //
  // If you declare your material as non-proportional, photmev will be scaled according to
  // the TSplinefit fFitNPLY.
  //
  // The default value for resScale = 1.0
  fPhotMev = photmev; 
  fResolutionScale = resScale;
}
void TOpticMaterial::SetRayleighLength(Double_t rl, Bool_t destroy) {
  // Declares that this material has Rayliegh scattering. The value of the 
  //Rayleigh scattering length is Double_t rl (the first argument). The second
  //argument specifies whether to delete the fit fFitRayleighL. 
  // Rayleigh scattering refers to the process of scattering of light off 
  //particles much smaller than the wavelength of the photons, such as molecules
  //or atoms. It differs from other scattering in that the photons do not 
  //scatter isotropically, as is implemented for Diffusion in Litrani. Rayleigh 
  //scattering is intrinsic to all materials, though often the scattering length
  //is so long it can be neglected.
  fRayleighLength = rl;
  if (fFitRayleighL) {
    if (destroy) delete fFitRayleighL;
    fFitRayleighL = 0;
  }
  if (fRayleighLength > 0.0) fHasRayleigh = kTRUE;
}
void TOpticMaterial::SetRayleighLength(const char *name) {
  // Declares that this material has Rayliegh scattering. The value of the 
  //Rayleigh scattering length is given by the TSplineFit with name "name". 
  //Rayleigh scattering refers to the process of scattering of light off 
  //particles much smaller than the wavelength of the photons, such as molecules
  //or atoms. It differs from other scattering in that the photons do not 
  //scatter isotropically, as is implemented for Diffusion in Litrani. Rayleigh 
  //scattering is intrinsic to all materials, though often the scattering length
  //is so long it can be neglected.
  TString s;
  if (fFitRayleighL) delete fFitRayleighL;
  fFitRayleighL = TSplineFit::FindFit(name);
  if (!fFitRayleighL) {
    gMes->M(Error_M,22,"Fit for Rayleigh scattering length not found");
    gMes->M(Info_M,22,name);
    gMes->M(kill_prog,22,"Define fit for Rayleigh scattering length before material");
    return;
  }
  fNameRayleighL = name;
  fHasRayleigh = kTRUE;
}
void TOpticMaterial::SetWLS(TLitSpectrum* sp, Double_t wlsl) {
  // Set the wavelength independent characteristic length for wavelength 
  //shifting and the emission spectrum. The second argument is the wavelength 
  //shifting length to be applied for all wavelengths. The first argument is 
  //the pointer to the re-emission TLitSpectrum.
  fWLSSpectrum = sp;
  if (!fWLSSpectrum) {
    gMes->SetName("SetWLS");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,1,"Specified WLS spectrum not found");
  }
  fWLSL = wlsl;
  fHasWLS = kTRUE;
}
void TOpticMaterial::SetWLS(TLitSpectrum* sp, Text_t* wlslFitName) {
  // Set the wavelength dependent characteristic length for wavelength shifting 
  //and the emission spectrum. The second argument is the name of the TSplineFit
  //to be used for the wavelength dependent WLS length to be used. The first 
  //argument is pointer to the re-emission TLitSpectrum.
  //
  fWLSSpectrum = sp;
  if (!fWLSSpectrum) {
    gMes->SetName("SetWLS");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,1,"WLS spectrum not initialised");
  }
  fNameWLSL = wlslFitName;
  fFitWLSL = TSplineFit::FindFit(fNameWLSL);
  if (!fFitWLSL) {
    gMes->SetName("SetWLS");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(kill_prog,2,"WLS absorption length fit not found");
  }
  fHasWLS = kTRUE;
}
void TOpticMaterial::SetXSectnPE(Text_t* name) {
  // Set the name of the TSplineFit for the photoelectric effect
  // In cases where you do not have access to a model or a table,
  //you may like to look at the XCOM page of the NIST (Photon cross
  //sections for compounds and mixtures of the National Institute of
  //Standards and Technology (USA)
  //
  // http://physics.nist.gov/PhysRefData/Xcom/html/xcom1.html
  //
  // Units of the cross section must be entered as
  //  sigma = (Atomic number * (cm2/g))/0.602
  // Sigma is x10^-24 cm2
  //
  // For an element this is equivalent to barns/atom
  fFitPE = TSplineFit::FindFit(name);
  if (fFitPE) {
    fNamePE  = name;
    fPEIsInitialised = kTRUE;}
  else {
    gMes->M(Error_M,1,"Fit for XSectn PE not found");
    gMes->M(Info_M,1,name);
    gMes->M(Info_M,1,"PE XSectn = 0");
    fFitPE   = 0;
    fPEIsInitialised = kFALSE;}
}
void TOpticMaterial::WaveAndLife(Double_t &wavelength,Double_t &lifetime,Int_t &kfluo) {
  // Generates new values for wavelength and lifetime of a new photon according to the
  //fluorescent characteristics of this material, pointed to by fLitSpectrum.
  // Returns wavelength in nanometers and lifetime in picoseconds
  kfluo = fLitSpectrum->GenWaveLAndDecayT(wavelength,lifetime);
  lifetime *= 1000;
}
