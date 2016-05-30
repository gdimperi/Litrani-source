// @(#)root/html:$Id: TLitSpectrum.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TROOT.h"
#include "TMath.h"
#include "TSystem.h"
#include "TSpectrum.h"
#include "TFitter.h"
#include "TPolyMarker.h"
#include "TwoPadDisplay.h"
#include "TSplineFit.h"
#include "TLitGlob.h"
#include "TLitPhys.h"
#include "TLitSpectrumCp.h"
#include "TLitSpectrum.h"

Int_t     gLitSpecNb  = 0;

Int_t       TLitSpectrum::fgNextDraw       = 0;
TFile      *TLitSpectrum::fgSpectrumFile   = 0;
TTree      *TLitSpectrum::fgSpectrumTree   = 0;
TBranch    *TLitSpectrum::fgSpectrumBranch = 0;
TObjArray  *TLitSpectrum::fgSpectra        = 0;
TString    *TLitSpectrum::fgFileName       = 0;

ClassImp(TLitSpectrum)
//______________________________________________________________________________
//
//BEGIN_HTML <!--
/* -->
</pre>
<div align="center">
<h2>TLitSpectrum Optical scintillation spectrum and time distribution</h2>
<h2></h2>
</div>
<div align="left">
<h3>1- Introduction.</h3>
</div>
<p>In the litterature, one easily finds the global scintillation emission spectra 
of material.&nbsp;One also finds additionnal informations, like the number of <i>emission 
bands</i>, with their mean values of wavelength of emission. For <b><code>PbWO4</code></b>, 
for instance, the global scintillation emission spectrum can be found in the <code>CMS 
ECAL TDR</code> page 31, and one can get the additionnal information that this spectrum 
<i>results from the superposition of two broad and complex emission bands at 420 
nm and 500 nm respectively</i>. <font color="red">But nothing can be found concerning 
the shapes of these individual</font> <i>emission bands</i>, except that, as said, 
they may be complex, which means, not gaussian.</p>
<p>Not being able to know the shapes of the individual <i>emission bands</i>, we 
have decided in <b><code>Litrani</code></b> to decompose the global scintillation 
spectrum into <i>gaussian</i> peaks, also for reasons of ease of generation of wavelength.&nbsp;But 
since the shape of the <i>emission bands</i> may be complex, it may lead to the necessity 
of representing <i>one</i> emission band by <i>more than one</i> gaussian peaks.&nbsp;In 
the case of <b><code>PbWO4</code></b>, with 2 emission bands, we have found that 
it was impossible to get a good fit with 2 gaussians, one at 420 nm and the other 
one at 500 nm. We had to use 4 gaussians, the first two with means fixed at 420 and 
500 nm, the last two with mean adjusted by the fit.&nbsp;The last two can be considered 
as &quot;auxiliary&quot; gaussians, necessary for a good representation of the two 
<i>emission bands</i>.</p>
<p>In the previous version of <b><code>Litrani</code></b>, it was left to the user 
to find the decomposition of the golbal scintillation spectrum into gaussians, which 
was a tedious operation.&nbsp;Class <b><code>TLitSpectrum</code></b> has been introduced 
to make this process easier. Let us consider 3 possible circumstances:</p>
<ol>
<li><b>Full knowledge</b>. You know everything about the global scintillation 
spectrum.&nbsp;You know into how many gaussian peaks to decompose it, you know position, 
width and amplitude of each of these gaussian peaks [a rather unrealistic case]. 
And you know the decay times for each of the gaussian peak. You can directly define 
the needed <b><code>TLitSpectrum</code></b> for instance like this:
<ul>
<li><code><b>TLitSpectrum *sp;</b></code>
<li><code><b>sp = new TLitSpectrum(&quot;Spectrum_PbWO4La&quot;,&quot;Emission 
Spectrum | PbWO4 doped with lanthane&quot;);</b></code>
<li><code><b>sp-&gt;AddOneComponent(420.0,40.0,0.75,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);</b></code>
<li><code><b>sp-&gt;AddOneComponent(500.0,50.0,0.25,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);</b></code>
<li><code><b>sp-&gt;Normalize();</b></code>
</ul>
<li><b>Partial knowledge</b>. You have at hand the global scintillation spectrum, 
you know the number of <i>emission bands</i> with their position, but are not sure 
that an equal number of gaussians may give a good fit.&nbsp;You do not know the widths 
and the amplitudes of the gaussian peaks.&nbsp;Then the best is to use the method 
<b><code><font color="red"><a href="#TLitSpectrum:BetterPeaks">TLitSpectrum::BetterPeaks()</a></font></code></b>.&nbsp;An 
example of use is in the <b><code>CINT</code></b> macro &quot;<b><code>GaussDecomp_PbWO4La.C</code></b>&quot; 
provided with <b><code>Litrani</code></b> in the directory &quot;<b><code>FitMacros</code></b>&quot;. 
You can play with the initial values given to the fit until you are satisfied. One 
nice thing with the method <b><code>BetterPeaks()</code></b> is that it writes a 
text file, named &quot;<b><code>Spectrum.C</code></b>&quot;, that you can complete 
by adding the decay times in order to get for instance the final macro, like &quot;<b><code>LitSpectrum_PbWO4La.C</code></b>&quot;, 
which defines completly the <b><code>TLitSpectrum</code></b>.
<li><b>Only global spectrum</b>. The only thing you have at hand is the global 
scintillation spectrum.&nbsp;You do not know how many <i>emission bands</i> there 
are. The best thing in that case is to use the method <b><code><font color="red"><a href="#TLitSpectrum:FindPeaks">TLitSpectrum::FindPeaks()</a></font></code></b>.&nbsp;We 
have put into &quot;<b><code>FitMacros</code></b>&quot;, with the 2 macros &quot;<b><code>GaussDecomp_In6WOTb.C</code></b>&quot; 
and &quot;<b><code>LitSpectrum_In6WOTb.C</code></b>&quot;  an example for such a 
case. Method <b><code>FindPeaks()</code></b> uses the <b><code>ROOT</code></b> class 
<b><code><a href="http://root.cern.ch/root/html/TSpectrum.html">TSpectrum</a></code></b> 
to find the peaks, and then makes the fit. Here again, a file &quot;<b><code>Spectrum.C</code></b>&quot; 
is produced by <b><code>FindPeaks()</code></b>, that you can complete by adding the 
decay times in order to get for instance the final macro, like &quot;<b><code>LitSpectrum_In6WOTb.C</code></b>&quot;, 
which defines completly the <b><code>TLitSpectrum</code></b>.
</ol>
<p></p>
<h3>2- Gaussian Components</h3>
<p>There can be any number of gaussian components inside a <b><code>TLitSpectrum</code></b>.&nbsp;A 
gaussian component is represented by the class <b><code><a href="TLitSpectrumCp.html">TLitSpectrumCp</a></code></b>. 
All compnents are contained in the collection <b><code>TLitSpectrum::fComponents</code></b>. 
A gaussian component is added to a <b><code>TLitSpectrum</code></b> by a call to 
the method <b><code><a href="#TLitSpectrum:AddOneComponent">TLitSpectrum::AddOneComponent</a>()</code></b>.&nbsp;A 
possible instance of <b><code>AddOneComponent()</code></b> is:</p>
<ul>
<li><b><code>void TLitSpectrum::AddOneComponent(Double_t m, Double_t s, Double_t 
A,</code></b>
<ul>
<li><b><code>Double_t td1, Double_t tr1, Double_t w1,</code></b>
<li><b><code>Double_t td2, Double_t tr2, Double_t w2)</code></b>
</ul>
</ul>
<p>The arguments are the following:</p>
<ul>
<li><code><b>m   : mean of the gaussian component</b></code>
<li><code><b>s   : sigma of the gaussian component</b></code>
<li><code><b>A   : weight to be affected to this component.</b></code>
<li><code><b>td1 : 1st decay time in ns</b></code>
<li><code><b>tr1 : 1st rise time in ns. Put -1.0 if no rise time</b></code>
<li><code><b>w1  : weight of this time pair (td1,tr1) component.</b></code>
<li><code><b>td2 : 2nd decay time in ns</b></code>
<li><code><b>tr2 : 2nd rise time in ns. Put -1.0 if no rise time</b></code>
<li><code><b>w2  : weight of this time pair (td2,tr2) component.</b></code>
</ul>
<p>In this case, there are 2 pairs of decay/rise times associated with this component. 
Each component can have any number of decay/rise times pairs, and the decay/rise 
times pairs may be different from one component to the other. <font color="red">The 
appearance of rise times is a novelty in this version of <b><code>Litrani</code></b></font>.&nbsp;See 
the paragraph about decay and rise times. Notice the appearance of 2 different types 
of weights:</p>
<ul>
<li><b>the weights A</b> is the respective weight of this component with respect 
of all other components.&nbsp;After a call to <b><code><a href="#TLitSpectrum:Normalize">TLitSpectrum::Normalize()</a></code></b>, 
the sum of  all A values of each component is equal to 1.
<li><b>the weights w<sub>i</sub></b> is the weight, <i>inside a component</i>, 
of the pair of decay/rise time i with respect to the other decay/rise time pairs 
<i>of this same component</i>. After a call to <b><code>TLitSpectrum<font color="red">Cp</font>::Normalize()</code></b>, 
the sum of all these w<sub>i</sub> will be equal to 1.
</ul>
<p>The user has not to bother to give normalized values for the A or the w<sub>i</sub>: 
the methods <b><code>Normalize()</code></b> will do it.</p>
<p></p>
<h3>3- Decay and rise times</h3>
<p>In this new version of <b><code>Litrani</code></b>, there are 2 possible distribution 
for the time of emission of the photon:</p>
<ol>
<li><code><b>(1/t<sub>d</sub>) exp(-t/t<sub>d</sub>)</b></code>
<li><code><b>((t<sub>d</sub> + t<sub>r</sub>)/t<sub>d</sub><sup>2</sup>) exp(-t/t<sub>d</sub>) 
(1 -exp(-t/t<sub>r</sub>))</b></code>
</ol>
<p>The first formula was the only available in the previous version of <b><code>Litrani</code></b>. 
The second formula corresponds to the addition of a rise time before the decay time. 
Some scintillation materials have this property, so that it is good to be able to 
simulate it.&nbsp;Notice however that using formula (2) instead of (1) is much more 
costly in time, because it gives rise to a transcendental equation which can only 
be solved numerically.&nbsp;Use it only if it matters. To avoid using it, give a 
negative value to the rise times.</p>
<p></p>
<h3>4- Methods <code>BetterPeakks()</code> and <code>FindPeaks()</code></h3>
<p>Before using methods <b><code><a href="#TLitSpectrum:BetterPeaks">BetterPeaks()</a></code></b> 
or <code><b><a href="#TLitSpectrum:FindPeaks">FindPeaks()</a></b></code> to find 
the decomposition of the global scintillation spectrum into gaussian components, 
it is necessary that the global scintillation spectrum has been introduced as a <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html#TSplineFit:description">TSplineFit</a></code></b> 
of category 16. You can find examples of how to do it in the <b><code>CINT</code></b> 
macros </p>
<ul>
<li type="circle"><b><code>Spectrum_In6WOTb.C</code></b>
<li type="circle"><code><b>Spectrum_PbWO4La.C</b></code>
<li type="circle"><code><b>Spectrum_PbWO4Nb.C</b></code>
</ul>
<p>given with <b><code>Litrani</code></b> in the directory <b><code>FitMacros</code></b>. 
For these kind of <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html#TSplineFit:description">TSplineFit</a></code></b>, 
we have taken the following conventions:</p>
<ul>
<li type="circle"><b><code>16 is category number for this kind of TSplineFit</code></b>
<li type="circle"><b><code>the name of the TSplineFit begins with &quot;Spectrum_&quot;</code></b>
<li type="circle"><b><code>the title of the TSplineFit begins with &quot;Emission 
Spectrum | &quot;</code></b>
</ul>
<p>As usual, this <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html#TSplineFit:description">TSplineFit</a></code></b> 
has to be stored into the database <b><code>SplineFitDB.rdb</code></b> by a call 
to the method <b><code>TSplineFit::UpdateFile()</code></b>. The fit will then be 
retrieved from <b><code>SplineFitDB.rdb</code></b> by <b><code><a href="#TLitSpectrum:FindFit">TLitSpectrum::FindFit()</a></code></b>.</p>
<p><font color="red">Notice that if you have entered the global scintillation spectrum 
as a <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html#TSplineFit:description">TSplineFit</a></code></b> 
into <b><code>SplineFitDB.rdb</code></b>, in order to be able to use the methods 
<b><code>BetterPeaks()</code></b> or <code><b>FindPeaks()</b></code>, you must give 
to the <b><code>TLitSpectrum</code></b> class using it the same name and title as 
you have given to the <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html#TSplineFit:description">TSplineFit</a></code></b> 
class.</font></p>
<p></p>
<h3>5- Database</h3>
<p>In the same way as all the fits of type <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html#TSplineFit:description">TSplineFit</a></code></b> 
are stored into a kind of &quot;database&quot; [which is in fact only a <b><code>ROOT</code></b> 
file] <b><code>SplineFitDB.rdb</code></b>, all the scintillation spectra of type 
<b><code>TLitSpectrum</code></b> can also be stored into a &quot;database&quot; [which 
is in fact only a <b><code>ROOT</code></b> file] <b><code>SpectraDB.rdb</code></b>. 
Place the file <b><code>SpectraDB.rdb</code></b> in the same directory as you have 
placed <b><code>SplineFitDB.rdb</code></b>.&nbsp;<b><code>SpectraDB.rdb</code></b> 
is delivered with <b><code>Litrani</code></b> in the directory <b><code>Database</code></b>, 
as <b><code>SplineFitDB.rdb</code></b>. Once a spectrum is inside the database, you 
can retrieve it, for instance from the class <b><code><a href="TOpticMaterial.html">TOpticMaterial</a></code></b>, 
using the method <b><code><a href="TOpticMaterial.html#TOpticMaterial:FindSpectrum">TOpticMaterial::FindSpectrum()</a></code></b>. 
We advise you to no more use the method <b><code>TOpticMaterial::FluoComponents()</code></b>, 
although we have keep it for backward compatibility.</p>
<p>The reason why we call <b><code>SplineFitDB.rdb</code></b> or <b><code>SpectraDB.rdb</code></b> 
&quot;database&quot; is because you have at hands methods for</p>
<ul>
<li><b><code>TLitSpectrum::UpdateFile()</code></b> - adding an element
<li><b><code>TLitSpectrum::RemoveSpectrumFromFile()</code></b> - removing an element
<li><b><code>TLitSpectrum::OrderFile()</code></b> - ordering elements in file

<li><b><code>TLitSpectrum::FindSpectrum()</code></b>- finding an element in the 
file
<li><b><code>TLitSpectrum::DrawSpectraInFile()</code></b> - drawing in turn each 
element in the file
<li><b><code>TLitSpectrum::ShowSpectraInFile()</code></b> - printing the list 
of all elements in the file

<li><b><code>TLitSpectrum::VerifyNotInFile()</code></b>- testing if an element 
is not in the file  
</ul>
<p>Before entering a new <b><code>TLitSpectrum</code></b> into <b><code>SpectraDB.rdb</code></b>, 
it is a good idea to:</p>
<ul>
<li>give the information concerning the source of the data you have used by a 
call to <b><code>TLitSpectrum::SetSource()</code></b>.
<li>associate a comment of up to 10 lines to this <b><code>TLitSpectrum</code></b>.&nbsp;See 
how it is done in the example macros<b><code> LitSpectrum_In6WOTb.C, LitSpectrum_PbWO4La.C, 
LitSpectrum_PbWO4Nb.C</code></b>.
</ul>
<h4>Where to put the 2 &quot;database&quot; files <b><code>SplineFitDB.rdb</code></b> 
and  <b><code>SpectraDB.rdb</code></b>?</h4>
<p>The simplest solution is to put them into the same directory as the one containing 
the executable &quot;litrani&quot; [linux] or &quot;litrani.exe&quot; [Windows].&nbsp;In 
that case, you have nothing to do and litrani will immediatly find the 2 files. For 
instance,</p>
<ul>
<li>if you start <b><code>Litrani</code></b> from the directory &quot;<b><code>.../litrani</code></b>&quot; 
where you have compiled and linked it, then put the two database files there.
<li>if you start <b><code>Litrani</code></b> from the directory &quot;<b><code>$ROOTDEV/bin</code></b>&quot; 
[linux]  or &quot;<b><code>%ROOTDEV%\bin</code></b>&quot; [Windows] where it has 
been installed by the <b><code>Makefile</code></b>,then put the two database files 
there.
<li>if you insist to <i>not</i> put the two database files into the directory 
where the executable &quot;<b><code>litrani</code></b>&quot; is, but want to put 
these two files into a specified directory, for instance
<ul>
<li><b><code>/home/mnt/gentit/mydatabases</code></b>  [linux]   or 
<li><b><code>D:\databases\mydatabases</code></b>  [Windows]
</ul>
<li>it is also possible, but you have then to put in front of all your <b><code>CINT</code></b> 
macros using them following line:
<ul>
<li><b><code>TLitSpectrum::NameFile(&quot;/home/mnt/gentit/mydatabases/SpectraDB.rdb&quot;);</code></b> 
[linux]  or
<li><b><code>TLitSpectrum::NameFile(&quot;D:\\databases\\mydatabases\\SpectraDB.rdb&quot;);</code></b> 
[Windows]
</ul>
</ul>
<p></p>
<h3>6- Generation of wavelength and time of emission</h3>
<p>The method to call is </p>
<ul>
<li><b><code>Int_t <a href="#TLitSpectrum:GenWaveLAndDecayT">TLitSpectrum::GenWaveLAndDecayT</a>(Double_t 
&amp;wavelength,Double_t &amp;time)</code></b>
</ul>
<p>The integer returned is the randomly chosen component which has done the emission. 
This method is called by the method <b><code><a href="TOpticMaterial.html#TOpticMaterial:WaveAndLife">TOpticMaterial::WaveAndLife()</a></code></b>; 
time is given in ns.</p>
<p></p>
<h3>6- Drawings</h3>
<p>You can ask for the drawing of the global wavelength spectrum of a TLitSpectrum 
*p by:</p>
<ul>
<li><b><code>p-&gt;<a href="#TLitSpectrum:DrawSpectrum">DrawSpectrum()</a>;</code></b>
</ul>
<p>You can ask for a time generation spectrum by calling:</p>
<ul>
<li><b><code>p-&gt;<a href="#TLitSpectrum:DrawDecayTimes">DrawDecayTimes()</a>;</code></b>
</ul>
<p></p>
<h3>7- Examples of use</h3>
<p>In the directory <b><code>&quot;FitMacros&quot;</code></b> provided with <b><code>Litrani</code></b>, 
you can look at the following example <b><code>CINT</code></b> macros:</p>
<ul>
<li><b><code>LitSpectrum_CsITl.C</code></b>
<li><b><code>LitSpectrum_CsITlJE.C</code></b>
<li><b><code>LitSpectrum_In6WOTb.C</code></b>
<li><b><code>LitSpectrum_PbWO4La.C</code></b>
<li><b><code>LitSpectrum_PbWO4Nb.C</code></b>
</ul>
<p>All these <b><code>CINT</code></b> macros are function with 3 boolean arguments, 
which are false by default:</p>
<ul>
<li><b><code>todraw</code></b> if true, the spectrum is drawn.&nbsp;Example for 
just having a drawing:
<ul>
<li><b><code>.x LitSpectrum_CsITl.C(kTRUE)</code></b>
</ul>
<li><b><code>infile</code></b> if true, the spectrum is added to the database 
file <b><code>SpectraDB.rdb</code></b>.&nbsp;Example, for just putting the spectrum 
into the database file:
<ul>
<li><b><code>.x LitSpectrum_CsITl.C(kFALSE,kTRUE)</code></b>
</ul>
<li><b><code>firstinfile</code></b> if both <b><code>infile</code></b> and <b><code>firstinfile</code></b> 
are true, then the database <b><code>SpectraDB.rdb</code></b> is <b><font color="#ce0000">destroyed</font></b>, 
and the spectrum is entered as the first component in a new database file <b><code>SpectraDB.rdb</code></b>.&nbsp;This 
case is of interest <b><font color="#ce0000">only to start a new database file</font></b>. 
Be cautious! risk of loosing <b><code>SpectraDB.rdb</code></b>! Example to enter 
the first spectrum into a new database file, destroying the old one:
<ul>
<li><b><code>.x LitSpectrum_CsITl.C(kFALSE,kTRUE,kTRUE)   &lt;=== <font color="#ce0000">dangerous 
!</font></code></b>
</ul>
</ul>
<p>Look at the <b><code>CINT</code></b> macro <b><code>Rebuild_SpectraDB.C</code></b> 
to see how to rebuild from scratch the file <b><code>SpectraDB.rdb</code></b>.</p>
<p>In order to produce for instance the <b><code>CINT</code></b> macro <b><code>LitSpectrum_In6WOTb.C</code></b>, 
a <b><code>TSplineFit</code></b> describing the <i>global </i>emission spectrum of 
<b><code>In6WOTb, </code></b>named <b><code>Spectrum_In6WOTb</code></b> has been 
created and put into the database <b><code>SplineFitDB.rdb</code></b> by the <b><code>CINT</code></b> 
macro <b><code>Spectrum_In6WOTb.C</code></b>. Then, in order to find the decomposition 
of the spectrum into gaussian peaks, the <b><code>CINT</code></b> macro <b><code>GaussDecomp_In6WOTb.C</code></b> 
has been used, which calls <b><code><a href="#TLitSpectrum:FindPeaks">TLitSpectrum::FindPeaks()</a></code></b>. 
<b><code>FindPeaks()</code></b> has created a file <b><code>Spectrum.C</code></b>, 
which has been transformed into <b><code>LitSpectrum_In6WOTb.C</code></b> by manual 
addition of the decay times. A similar process has been used for the other examples 
<b><code>LitSpectrum_PbWO4La.C</code></b> and <b><code>LitSpectrum_PbWO4Nb.C</code></b>.</p>
<p>In addition to all the <b><code>CINT</code></b> macros quoted above, you can 
have a look at the macro <b><code>Anibm1.C</code></b> delivered with <b><code>Litrani</code></b> 
in the directory &quot;<b><code>Macro&quot;</code></b>. It has been modified to show 
6 different cases of using <b><code>TLitSpectrum</code></b>. Look at it.</p>
<pre>
<!--*/
// -->END_HTML
//
TLitSpectrum::TLitSpectrum() {
  // Default constructor
  Init();
}
TLitSpectrum::TLitSpectrum(const char *name, const char *title,Bool_t indb):TNamed(name,title) {
  // Constructor with name and title. If a TSplineFit exists showing the spectrum
  //to be handled by TLitSpectrum it is asked to the user to give same name and title
  //to this TLitSpectrum class as the name and title of the TSplineFit spectrum.
  Init();
  if (!fgSpectra) InitStatic();
  TLitPhys::Book();
  FindDate();
  if (indb) FindFit(kTRUE);
  else      FindFit();
  if (fInSplineFitDB) {
    fSource = fSplineFit->GetSource();
  }
  AddThisSpectrum();
}
TLitSpectrum::TLitSpectrum(const TLitSpectrum &sp) {
  // Copy constructor
  Short_t k;
  TLitSpectrumCp *cp, *dp;
  SetName(sp.GetName());
  SetTitle(sp.GetTitle());
  fNormalized    = sp.fNormalized;
  fInSplineFitDB = sp.fInSplineFitDB;
  fNbComponents  = sp.fNbComponents;
  fArea = new Double_t [fNbComponents];
  for (k=0;k<fNbComponents;k++) {
    fArea[k] = sp.fArea[k];
    cp = (TLitSpectrumCp*)sp.fComponents[k];
    dp = new TLitSpectrumCp(*cp);
    fComponents.AddLast(dp);
  }
  fDate          = sp.fDate;
  fSource        = sp.fSource;
  fMacro         = sp.fMacro;
  fXLabel        = sp.fXLabel;
  fVLabel        = sp.fVLabel;
  FindFit();
  for (k=0;k<10;k++) fComment[k] = sp.fComment[k];
  fFitGraph      = 0;
  fH             = 0;
  fPM            = 0;
}
TLitSpectrum::~TLitSpectrum() {
  // If fgSpectrumFile is != 0, then the destructor is called by ROOT. In that
  //case, we do not want ROOT to try to modify the collection. Else we also remove
  //the spectrum from the collection.
  if (!fgSpectrumFile) {
    if (IsInCollection()) fgSpectra->Remove(this);
  }
  Clear();
}
void TLitSpectrum::AddOneComponent(Double_t m,Double_t s,Double_t A) {
  // Add one gaussian component, without giving its decay times. A check is
  //first made that no component exists in the collection fComponents with
  //the same value of gaussian mean. It is forbidden to have more than 1
  //component for a given value of mean. Arguments:
  //
  //  m : mean of the gaussian component
  //  s : sigma of the gaussian component
  //  A : weight to be affected to this component. The user has not to bother
  //      that the sum of all weight A be equal to 1. It will be done after
  //      having entered all components, by a call to method Normalize().
  //
  TLitSpectrumCp *p;
  p = FindComponent(m);
  if (p) Error("TLitSpectrum::AddOneComponent","Such a component already exists");
  else {
    fNormalized = kFALSE;
    p = new TLitSpectrumCp(m,s,A);
    fComponents.AddLast(p);
    fNbComponents++;
  }
}
void TLitSpectrum::AddOneComponent(Double_t m,Double_t s,Double_t A,Double_t td,
  Double_t tr,Double_t w) {
  // Add one gaussian component with one pair of (td,tr) of decay times. A check is
  //first made that no component exists in the collection with the same value
  //of gaussian mean. It is forbidden to have more than 1 component for a
  //given value of mean.  All times in ns. Arguments:
  //
  //  m  : mean of the gaussian component
  //  s  : sigma of the gaussian component
  //  A  : weight to be affected to this component. The user has not to bother
  //       that the sum of all weight A be equal to 1. It will be done after
  //       having entered all components, by a call to method Normalize().
  //  td : decay time
  //  tr : rise time. Put -1.0 if no rise time
  //  w  : weight of this time pair (td,tr) component. If this time pair remains
  //       alone (you do not intend to add afterwards other time pairs associated
  //       with this TLitSpectrumCp (m,s,A) ), then clearly this weight does not
  //       matter: it will be set to 1.0 by the call to TLitSpectrumCp::Normalize().
  //       But if you intend to add afterwards other time pair components, this
  //       weight does matter!
  //
  //  Do not confuse the weights A with the weights w. Weights A are the weigths
  //of this component (m,s,A) among the other components (m',s',A') that will be entered
  //later. Weight w is the weight of this time pair (td,tr) among other time pairs
  //(td',tr') belonging to the SAME gaussian component (m,s,A).
  //
  TLitSpectrumCp *p;
  p = FindComponent(m);
  if (p) Error("TLitSpectrum::AddOneComponent","Such a component already exists");
  else {
    fNormalized = kFALSE;
    p = new TLitSpectrumCp(m,s,A,td,tr,w);
    fComponents.AddLast(p);
    fNbComponents++;
  }
}
void TLitSpectrum::AddOneComponent(Double_t m,Double_t s,Double_t A,Double_t td1,
  Double_t tr1,Double_t w1,Double_t td2,Double_t tr2,Double_t w2) {
  // Add one gaussian component with 2 pairs (td1,tr1) and (td2,tr2) of decay times.
  //A check is first made that no component exists in the collection with the same value
  //of gaussian mean. It is forbidden to have more than 1 component for a
  //given value of mean. All times in ns. Arguments:
  //
  //  m   : mean of the gaussian component
  //  s   : sigma of the gaussian component
  //  A   : weight to be affected to this component. The user has not to bother
  //       that the sum of all weight A be equal to 1. It will be done after
  //       having entered all components, by a call to method Normalize().
  //  td1 : 1st decay time [ns]
  //  tr1 : 1st rise time. Put -1.0 if no rise time
  //  w1  : weight of this time pair (td1,tr1) component.
  //  td2 : 2nd decay time
  //  tr2 : 2nd rise time. Put -1.0 if no rise time
  //  w2  : weight of this time pair (td2,tr2) component.
  //
  //  Do not confuse the weights A with the weights w1/2. Weights A are the weigths
  //of this component (m,s,A) among the other components (m',s',A') that will be entered
  //later. Weight wi is the weight of this time pair (tdi,tri) among other time pairs
  //(tdj,trj) belonging to the SAME gaussian component (m,s,A).
  //
  TLitSpectrumCp *p;
  p = FindComponent(m);
  if (p) Error("TLitSpectrum::AddOneComponent","Such a component already exists");
  else {
    fNormalized = kFALSE;
    p = new TLitSpectrumCp(m,s,A,td1,tr1,w1,td2,tr2,w2);
    fComponents.AddLast(p);
    fNbComponents++;
  }
}
void TLitSpectrum::AddOneComponent(Double_t m,Double_t s,Double_t A,Double_t td1,
  Double_t tr1,Double_t w1,Double_t td2,Double_t tr2,Double_t w2,Double_t td3,Double_t tr3,Double_t w3) {
  // Add one gaussian component with 3 pairs (td1,tr1), (td2,tr2) and (td3,tr3) of decay
  //times. A check is first made that no component exists in the collection with the same
  //value of gaussian mean. It is forbidden to have more than 1 component for a
  //given value of mean.  All times in ns. Arguments:
  //
  //  m   : mean of the gaussian component
  //  s   : sigma of the gaussian component
  //  A   : weight to be affected to this component. The user has not to bother
  //       that the sum of all weight A be equal to 1. It will be done after
  //       having entered all components, by a call to method Normalize().
  //  td1 : 1st decay time
  //  tr1 : 1st rise time. Put -1.0 if no rise time
  //  w1  : weight of this time pair (td1,tr1) component.
  //  td2 : 2nd decay time
  //  tr2 : 2nd rise time. Put -1.0 if no rise time
  //  w2  : weight of this time pair (td2,tr2) component.
  //  td3 : 3rd decay time
  //  tr3 : 3rd rise time. Put -1.0 if no rise time
  //  w3  : weight of this time pair (td3,tr3) component.
  //
  //  Do not confuse the weights A with the weights w1/2. Weights A are the weigths
  //of this component (m,s,A) among the other components (m',s',A') that will be entered
  //later. Weight wi is the weight of this time pair (tdi,tri) among other time pairs
  //(tdj,trj) belonging to the SAME gaussian component (m,s,A).
  //
  TLitSpectrumCp *p;
  p = FindComponent(m);
  if (p) Error("TLitSpectrum::AddOneComponent","Such a component already exists");
  else {
    fNormalized = kFALSE;
    p = new TLitSpectrumCp(m,s,A,td1,tr1,w1,td2,tr2,w2,td3,tr3,w3);
    fComponents.AddLast(p);
    fNbComponents++;
  }
}
void TLitSpectrum::AddOneComponent(Double_t m,Double_t s,Double_t A,Int_t N,
  Double_t *td,Double_t *tr,Double_t *w) {
  // Add one gaussian component with N pairs (tdi,tri) of decay times. A check is first
  //made that no component exists in the collection with the same value of gaussian mean.
  //It is forbidden to have more than 1 component for a given value of mean.
  // All times in ns.
  //
  // Arguments:
  //
  //  m   : mean of the gaussian component
  //  s   : sigma of the gaussian component
  //  A   : weight to be affected to this component. The user has not to bother
  //        that the sum of all weights A be equal to 1. It will be done after
  //        having entered all components, by a call to method Normalize().
  //  N   : number of time pair (tdi,tri)
  //  *td : array of decay times
  //  *tr : array of rise times. Put tr[i] = -1.0 if no rise time for component i
  //  *w  : array of weights of all time pairs (tdi,tri).
  //
  //  Do not confuse the weights A with the weights w[i]. Weights A are the weigths
  //of this component (m,s,A) among the other components (m',s',A') that will be entered
  //later. Weight wi is the weight of this time pair (tdi,tri) among other time pairs
  //(tdj,trj) belonging to the SAME gaussian component (m,s,A).
  //
  TLitSpectrumCp *p;
  p = FindComponent(m);
  if (p) Error("TLitSpectrum::AddOneComponent","Such a component already exists");
  else {
    fNormalized = kFALSE;
    p = new TLitSpectrumCp(m,s,A,N,td,tr,w);
    fComponents.AddLast(p);
    fNbComponents++;
  }
}
void TLitSpectrum::AddSpectrum(TLitSpectrum *pf) {
  // Add spectrum pf to the collection of spectra. Do not add it into the data base file.
  //For that call UpdateFile().
  TLitSpectrum *pspectrum;
  if (!fgSpectra) fgSpectra = new TObjArray();
  TIter next(fgSpectra);
  //Look first if spectrum already there
  Bool_t already = kFALSE;
  while ((!already) && (pspectrum = (TLitSpectrum *)next())) {
    if (pspectrum==pf) {
      already = kTRUE;
      fgSpectra->Remove(pspectrum);
      delete pspectrum;
    }//end if (IsEqual(pspectrum))
  }//end while ((!already) && (pspectrum = (TLitSpectrum *)next()))
  fgSpectra->Add(pf);
}
void TLitSpectrum::AddThisSpectrum() {
  //Add this spectrum to the list of spectra. Do not add it into the data base file.
  //For that call UpdateFile()
  TLitSpectrum *pspectrum;
  TIter next(fgSpectra);
  //Look first if spectrum already there
  Bool_t already = kFALSE;
  while ((!already) && (pspectrum = (TLitSpectrum *)next())) {
    if (IsEqual(pspectrum)) {
      already = kTRUE;
      fgSpectra->Remove(pspectrum);
      delete pspectrum;
    }//end if (IsEqual(pspectrum))
  }//end while ((!already) && (pspectrum = (TLitSpectrum *)next()))
  fgSpectra->Add(this);
}
void TLitSpectrum::AddTimeComponent(Double_t m,Double_t td,Double_t tr,Double_t w) {
  // Add one pair (td,tr) with weight w to the already existing time pairs (tdi,tri)
  //of the gaussian component of mean m
  TLitSpectrumCp *p;
  p = FindComponent(m);
  if (!p) Error("TLitSpectrum::AddTimeComponent","No gaussian component of mean m");
  else {
    p->AddTimeComponent(td,tr,w);
  }
}
void TLitSpectrum::AddTimeComponent(TLitSpectrumCp *p,Double_t td,Double_t tr,Double_t w) {
  // Add one pair (td,tr) with weight w to the already existing time pairs (tdi,tri)
  //of the gaussian component of mean m
  p->AddTimeComponent(td,tr,w);
}
Bool_t TLitSpectrum::BetterPeaks(Int_t Npeaks,Double_t *mean,Double_t *sigma,
  Double_t *weight,Short_t *meanfixed,Bool_t bavard) {
  //
  // This method is to be used if one has at hand both:
  //
  //   (1) - the graph representation of the emission spectrum that one has entered
  //         as a TSplineFit inside SplineFitDB.rdb
  //   (2) - a preliminary knowledge about the emission bands: the position of the
  //         peaks
  //
  // Method FindFit() must return a positive result: SplineFitDB.rdb MUST contain a fit
  //of category 16, with the same name and title as this TLitSpectrum class.
  //
  // A text file, named "Spectrum.C" is produced by this method, which can be used as
  //a starting point to produce the final macro defining this TLitSpectrum. Only
  //remains the addition of the decay/rise times and the replacement of the "blabla"
  //by usefule comments
  //
  //  Arguments
  //
  // Npeaks      Number of gaussian peaks. Fixed. BetterPeaks will not change it
  // *mean       Array containing the initial values of the mean of the Npeaks peaks.
  //             If meanfixed[k] is true, mean[k] will be fixed.
  //             If meanfixed[k] is false, mean[k] will be adjusted by the fit.
  // *sigma      Array containing the initial values for the sigmas of the peaks. They
  //             will be adjusted by the fit.
  // *weight     Array containing the initial values for the weights (or amplitude)
  //             of the gaussian peaks. They will be adjusted by the fit.
  // *meanfixed  If meanfixed[k] is != 0, the fit will not change the position of the
  //             mean of peak k. The reason why meanfixed is Short_t* and not Bool_t*
  //             is a bug in CINT !
  // bavard      If true, printing results
  //
  const Int_t    nchanmin   = 100;
  const Double_t zero       = 0.0;
  const Double_t Nphot      = 1.0e+6;
  const Int_t    width      = 20;
  const Int_t    precision  = 12;
  const Option_t *option    = "";
  Bool_t    ok = kTRUE;
  Int_t     nchan;
  Int_t     facnchan;
  Int_t     first,last,size;
  Int_t     i,k,bin;
  Double_t  a,x,xmin,xmax,w,error;
  Double_t  m,sig,S,step,steps2,chi2;
  ofstream  cintcode;
  Double_t *ppar  = 0;
  TF1      *ppfit = 0;
  if (!fSplineFit) FindFit(kTRUE);
  if (!fSplineFit) return kFALSE;
  gLitSpecNb = Npeaks;
  fSplineFit->DrawFit();
  if (fH) {
    delete fH;
    fH = 0;
  }
  nchan     = fSplineFit->GetNbOfMeas();
  if (nchan%2) nchan--;
  facnchan = 1;
  if (nchan<nchanmin) {
    while (facnchan*nchan<nchanmin) facnchan *= 2;
  }
  nchan    *= facnchan;
  xmin      = fSplineFit->GetXmin();
  if (xmin<TLitPhys::Get()->MinWaveL()) xmin = TLitPhys::Get()->MinWaveL();
  xmax      = fSplineFit->GetXmax();
  if (xmax>TLitPhys::Get()->MaxWaveL()) xmax = TLitPhys::Get()->MaxWaveL();
  fH = new TH1F("InGaussians","Decomposition of spectrum into gaussians",nchan,xmin,xmax);
  first = fH->GetXaxis()->GetFirst();
  last  = fH->GetXaxis()->GetLast();
  size  = last-first+1;
  if (bavard) {
    cout << "first : " << first << endl;
    cout << "last  : " << last  << endl;
    cout << "size  : " << size  << endl;
  }
  step   = (xmax-xmin)/nchan;
  steps2 = step/2.0;
  x = xmin + steps2;
  for (bin=first;bin<=last;bin++) {
    i     = (bin-1)/facnchan;
    w     = fSplineFit->V(x);
    error = fSplineFit->GetMeasErr(i);
    fH->SetBinContent(bin,w);
    fH->SetBinError(bin,error);
    x += step;
  }
  ((TH1*)fH)->Draw(option);
  ppar = new Double_t [3*Npeaks];
  for (i=0;i<Npeaks;i++) {
    k = 3*i;
    ppar[k]   = weight[i];
    ppar[k+1] = mean[i];
    ppar[k+2] = sigma[i];
  }
  if (Npeaks>8)
    TVirtualFitter::Fitter(fH,10+3*Npeaks); //we may have more than the default 25 parameters
  ppfit = new TF1("fit",SumOfGaussians,xmin,xmax,3*Npeaks);
  ppfit->SetParameters(ppar);
  ppfit->SetNpx(nchan);
  for (i=0;i<Npeaks;i++) {
    ppfit->SetParLimits(3*i,zero,Nphot);
    if (meanfixed[i]) ppfit->FixParameter(3*i+1,ppar[3*i+1]);
    else ppfit->SetParLimits(3*i+1,xmin,xmax);
    ppfit->SetParLimits(3*i+2,zero,xmax-xmin);
  }
  if (bavard) {
    cout << endl;
    cout << "  Starting values for fit:" << endl;
    cout << endl;
    for (i=0;i<Npeaks;i++) {
      cout << "peak: ";
      cout.width(4);
      cout << i << "   norm: ";
      cout.width(width);
      cout.precision(precision);
      cout << ppar[3*i];
      cout << "   mean: ";
      cout.width(width);
      cout.precision(precision);
      cout << ppar[3*i+1] << "   sigma: ";
      cout.width(width);
      cout.precision(precision);
      cout << ppar[3*i+2] << endl;
    }
  }
  cout << endl;
  cout << "Now fitting: Be patient" << endl;
  cout << endl;
  fH->Fit(ppfit,"");
  if (bavard) {
    S = zero;
    for (i=0;i<Npeaks;i++) {
      a   = ppfit->GetParameter(3*i);
      S  += a;
      m   = ppfit->GetParameter(3*i+1);
      sig = ppfit->GetParameter(3*i+2);
      cout << "peak: ";
      cout.width(4);
      cout << i << "   norm: ";
      cout.width(width);
      cout.precision(precision);
      cout << a;
      cout << "   mean: ";
      cout.width(width);
      cout.precision(precision);
      cout << m << "   sigma: ";
      cout.width(width);
      cout.precision(precision);
      cout << sig << endl;
    }
    chi2 = ppfit->GetChisquare();
    cout << endl;
    cout << "Chi2 of fit             : " << chi2 << endl;
    cout << "Surface on fitted curve : " << S << endl;
    cout << "Surface normalized      : " << S/(Nphot) << endl;
    cout << endl;
  }
  if (ok) {
    cintcode.open("Spectrum.C",ios::out);
    cintcode << "TLitSpectrum* " << GetName();
    cintcode << "(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)";
    cintcode << endl;
    cintcode << '{' << endl;
    cintcode << "//" << endl;
    cintcode << "// Please replace ... by the time components" << endl;
    cintcode << "// Please replace \"blabla\" by useful comments" << endl;
    cintcode << "// Then this CINT code will be ready for the definition" << endl;
    cintcode << "//  of this emission spectrum !" << endl;
    cintcode << "//" << endl;
    cintcode << "  char c;" << endl;
    cintcode << "  Bool_t ok;" << endl;
    cintcode << "  TLitSpectrum *sp;" << endl;
    cintcode << "  sp = new TLitSpectrum(";
    cintcode << '"' << GetName() << '"' << ',' << '"' << GetTitle() << '"' << ");" << endl;
    for (i=0;i<gLitSpecNb;i++) {
      cintcode << "  sp->AddOneComponent(";
      cintcode.width(width);
      cintcode.precision(precision);
      cintcode << ppfit->GetParameter(3*i+1);
      cintcode << ',';
      cintcode.width(width);
      cintcode.precision(precision);
      cintcode << ppfit->GetParameter(3*i+2);
      cintcode << ',';
      cintcode.width(width);
      cintcode.precision(precision);
      cintcode << ppfit->GetParameter(3*i);
      cintcode << ",...);" << endl;
    }
    cintcode << "  sp->Normalize();" << endl;
    cintcode << "  sp->SetMacro(\"Lit" << GetName() << ".C\");" << endl;
    cintcode << "//Up to 10 lines of comments are allowed. We write only 2 here" << endl;
    cintcode << "  sp->fComment[0] = \"blabla\";" << endl;
    cintcode << "  sp->fComment[1] = \"blabla\";" << endl;
    cintcode << "  if (todraw) {" << endl;
    cintcode << "    if (!gTwoPad) {" << endl;
    cintcode << "      gLit = new TLitGlob(5);" << endl;
    cintcode << "      gLit->BookCanvas();" << endl;
    cintcode << "    }" << endl;
    cintcode << "    sp->DrawDecayTimes();" << endl;
    cintcode << "    sp->DrawSpectrum();" << endl;
    cintcode << "    sp->Print();" << endl;
    cintcode << "  }" << endl;
    cintcode << "  if (infile) {" << endl;
    cintcode << "    if (firstinfile) sp->UpdateFile(kTRUE);" << endl;
    cintcode << "    else             sp->UpdateFile(kFALSE);" << endl;
    cintcode << "  }" << endl;
    cintcode << "  return sp;" << endl;
    cintcode << '}' << endl;
    cintcode << endl;
    cintcode.close();
  }
  if (ppfit) delete ppfit;
  if (ppar) delete [] ppar;
  return ok;
}
void TLitSpectrum::Clear() {
  // Empties spectrum of all its components. Do not delete fSplineFit. It is the task
  //SplineFit, not of this class.
  ClearGraphs();
  if (fArea) {
    delete [] fArea;
    fArea = 0;
  }
  fComponents.Delete();
  fNbComponents = 0;
}
void TLitSpectrum::ClearGraphs() {
  // Empties spectrum of all its components. Do not delete fSplineFit. It is the task
  //SplineFit, not of this class.
  fSplineFit = 0;
  if (fFitGraph) {
    delete fFitGraph;
    fFitGraph = 0;
  }
  if (fH) {
    delete fH;
    fH = 0;
  }
  if (fPM) {
    delete fPM;
    fPM = 0;
  }
}
Int_t TLitSpectrum::Compare(const TObject *obj) const {
  // We order components according to alphabetical order of name
  Int_t k;
  TLitSpectrum *p = (TLitSpectrum*)obj;
  TString s1 = GetName();
  TString s2 = p->GetName();
  k = s1.CompareTo(s2);
  return k;
}
void TLitSpectrum::DrawDecayTimes(Int_t N) {
  // Generates N decay time to show the time spectrum
  Int_t i,k;
  Int_t ntimes;
  Double_t time;
  Double_t wavelength,decaytime;
  TAxis *xaxis, *yaxis;
  Double_t BiggestTime = 0.0;
  TLitSpectrumCp *cp;
  if (fNbComponents<=0) return;
  for (k=0;k<fNbComponents;k++) {
    cp = (TLitSpectrumCp*)fComponents[k];
    ntimes = cp->GetNbTimes();
    for (i=0;i<ntimes;i++) {
      time = cp->GetDecayTime(i);
      if (time>BiggestTime) BiggestTime = time;
      time = cp->GetRiseTime(i);
      if (time>BiggestTime) BiggestTime = time;
    }
  }
  BiggestTime *= 4;
  if (fH) {
    delete fH;
    fH = 0;
  }
  fH = new TH1F("DecayTimes","Generation of decay and rise times",500,0.0,BiggestTime);
  for (k=0;k<N;k++) {
    GenWaveLAndDecayT(wavelength,decaytime);
    fH->Fill(decaytime);
  }
  SetDefaultLabels();
  gTwoPad->ChangePad();
  xaxis = fH->GetXaxis();
  yaxis = fH->GetYaxis();
  xaxis->SetTitle("time [ns]");
  xaxis->SetLabelSize(0.02);
  xaxis->SetTitleSize(0.03);
  yaxis->SetTitle("counts");
  yaxis->SetLabelSize(0.02);
  yaxis->SetTitleSize(0.03);
  yaxis->SetTitleOffset(1.5);
  fH->Draw();
  gPad->Modified();
  gPad->Update();
}
void TLitSpectrum::DrawNextInCollection() {
  // Draws spectrum pointed to by fgNextDraw and increment fgNextDraw
  Int_t N;
  TLitSpectrum *fit;
  if (!gTwoPad) gLit->BookCanvas();
  if (!fgSpectra) fgSpectra = new TObjArray();
  N = fgSpectra->GetEntries();
  if (N>0) {
    gTwoPad->SetStateOfPads(Pad1AndPad2);
    fit = (TLitSpectrum *)(*fgSpectra)[fgNextDraw];
    fit->ClearGraphs();
    fit->SetDefaultLabels();
    fit->DrawDecayTimes();
    fit->DrawSpectrum();
    fgNextDraw++;
    if (fgNextDraw>=N) fgNextDraw = 0;
    if (gLit) gLit->fBTRchanged = kTRUE;
  }
}
void TLitSpectrum::DrawSpectraInCollection() {
  // Shows the drawing of all spectra in the collection fgSpectra. To go from one drawing to
  //the next, hit 'n' and then <CR>. To stop the show hit 'q' and then <CR>
  Int_t i,N;
  Char_t c;
  TLitSpectrum *spectrum;
  N = fgSpectra->GetEntries();
  if (N>0) {
    if (!gTwoPad) gLit->BookCanvas();
    i=0;
    do {
      spectrum = (TLitSpectrum *)(*fgSpectra)[i];
      spectrum->SetDefaultLabels();
      spectrum->DrawSpectrum();
      cout << "enter n to continue  q to quit : ";
      cin  >> c;
      spectrum->ClearGraphs();
      i++;
    } while ((i<N) && (c!='q'));
  }
}
void TLitSpectrum::DrawSpectraInFile() {
  // Shows the drawing of all spectra in the "database" root file fgSpectrumFile.
  //To go from one drawing to the next, hit 'n' and then <CR>. To stop the show
  //hit 'q' and then <CR>
  Int_t i,nb,N;
  Char_t c;
  TLitSpectrum *spectrum = 0;
  if (!gLit) gLit = new TLitGlob(5);
  if (!fgFileName) NameFile();
  TLitPhys::Book();
  if (!gTwoPad) gLit->BookCanvas();
  fgSpectrumFile = 0;
  fgSpectrumFile = new TFile(fgFileName->Data(),"READ");
  if (fgSpectrumFile) {
    fgSpectrumTree = 0;
    fgSpectrumTree = (TTree *)fgSpectrumFile->Get("AllSpectra");
    if (fgSpectrumTree) {
      spectrum = new TLitSpectrum();
      fgSpectrumBranch = 0;
      fgSpectrumBranch = fgSpectrumTree->GetBranch("Spectra");
      if (fgSpectrumBranch) {
        fgSpectrumBranch->SetAddress(&spectrum);
        N  = (Int_t)fgSpectrumTree->GetEntries();
        nb = 0;
        if (N>0) {
          i=0;
          do {
            nb += fgSpectrumTree->GetEntry(i);
            spectrum->SetDefaultLabels();
            spectrum->DrawDecayTimes();
            spectrum->DrawSpectrum();
            cout << "enter n to continue  q to quit : ";
            cin  >> c;
            spectrum->ClearGraphs();
            i++;
          } while ((i<N) && (c!='q'));
        }
      }//end if (fgSpectrumBranch)
      else cout << "TLitSpectrum::DrawSpectraInFile   Branch \"Spectra\" not in Tree \"AllSpectra\"" << endl;
      delete spectrum;
      spectrum = 0;
    }//end if (fgSpectrumTree)
    else cout << "TLitSpectrum::DrawSpectraInFile   Tree \"AllSpectra\" not in file SpectraDB.rdb" << endl;
    fgSpectrumFile->Close();
    delete fgSpectrumFile;
    fgSpectrumFile   = 0;
    fgSpectrumTree   = 0;
    fgSpectrumBranch = 0;
  }//end if (fgSpectrumFile)
  else cout << "TLitSpectrum::DrawSpectraInFile   File SpectraDB.rdb not found" << endl;
}
void TLitSpectrum::DrawSpectrum(Int_t N) {
  // Draws this spectrum. N is the number of points to plot
  Int_t j,k;
  Double_t step;
  Double_t xx[1];
  TAxis *xaxis, *yaxis;
  TArrayD x(N), y(N);
  TArrayD par(3*fNbComponents);
  TLitSpectrumCp *q;
  TIter next(&fComponents);
  gLitSpecNb = fNbComponents;
  k = 0;
  while ((q = (TLitSpectrumCp*)next())) {
    j = 3*k;
    par[j]   = q->GetGaussAmp();
    par[j+1] = q->GetGaussMean();
    par[j+2] = q->GetGaussSigma();
    k++;
  }
  step  = (TLitPhys::Get()->MaxWaveL() - TLitPhys::Get()->MinWaveL())/(N-1);
  xx[0] = TLitPhys::Get()->MinWaveL();
  for (k=0;k<N;k++) {
    x[k]   = xx[0];
    y[k]   = SumOfGaussians(xx,par.fArray);
    xx[0] += step;
  }
  //
  SetDefaultLabels();
  gTwoPad->ChangePad();
  if (fFitGraph) {
    delete fFitGraph;
    fFitGraph = 0;
  }
  fFitGraph = new TGraph(N,x.fArray,y.fArray);
  fFitGraph->SetName(GetName());
  fFitGraph->SetTitle(GetTitle());
  fFitGraph->SetMarkerStyle(20);
  fFitGraph->SetMarkerSize(0.5);
  fFitGraph->SetMarkerColor(2);
  fFitGraph->SetFillColor(41);
  gPad->Clear();
  fFitGraph->Draw("APF");
  if (fXLabel.Length()>0) {
    xaxis = fFitGraph->GetXaxis();
    xaxis->SetTitle(fXLabel.Data());
    xaxis->SetLabelSize(0.02);
    xaxis->SetTitleSize(0.03);
  }
  if (fVLabel.Length()>0) {
    yaxis = fFitGraph->GetYaxis();
    yaxis->SetTitle(fVLabel.Data());
    yaxis->SetLabelSize(0.02);
    yaxis->SetTitleSize(0.03);
    yaxis->SetTitleOffset(1.5);
  }
  gPad->Modified();
  gPad->Update();
}
TLitSpectrumCp* TLitSpectrum::FindComponent(Double_t m) const {
  // Finds if there is already a gaussian component with mean m inside the collection.
  //Having more than one component with mean m in the collection is forbidden
  const Double_t eps = 1.0e-10;
  TLitSpectrumCp *p = 0;
  TLitSpectrumCp *q;
  Double_t mean,d;
  Bool_t found = kFALSE;
  if (fNbComponents>0) {
    TIter next(&fComponents);
    while ((!found) && (q = (TLitSpectrumCp*)next())) {
      mean = q->GetGaussMean();
      d    = TMath::Abs(mean-m);
      if (d<eps) {
        found = kTRUE;
        p = q;
      }
    }
  }
  return p;
}
void TLitSpectrum::FindDate() {
  // Finds the date
  Int_t day,month,year,date;
  TDatime *td;
  td = new TDatime();
  date  = td->GetDate();
  day   = date % 100;
  date /= 100;
  month = date % 100;
  date /= 100;
  year  = date;
  delete td;
  fDate  = "  ";
  fDate += day;
  fDate.Append(" / ");
  fDate += month;
  fDate.Append(" / ");
  fDate += year;
  fDate.Append("  ");
}
void TLitSpectrum::FindFit(Bool_t shouldbe) {
  // Finds the fit TSplineFit of the emission spectrum
  Int_t k;
  fInSplineFitDB = kFALSE;
  fSplineFit     = 0;
  TString s      = GetName();
  k = s.Index('_');
  if (k>1)  fSplineFit = TSplineFit::FindFit(GetName(),16);
  if (fSplineFit) fInSplineFitDB = kTRUE;
  else if (shouldbe) Error("TLitSpectrum::FindFit","Spectrum is not in SplineFitDB.rdb");
}
Bool_t TLitSpectrum::FindPeaks(Int_t deconIterations,Double_t threshold,
  Double_t sigma,Double_t tooclose,Bool_t MeanFixed,Bool_t bavard) {
  // This method is to be used if the only thing one knows about the emission spectrum
  //is its representation as a graph, as entered as a TSplineFit in SplineFitDB.rdb,
  //but one knows nothing about the position of the peaks (emission band). If you have
  //in addition a knowledge of where the emission bands are, use rather the method
  //BetterPeaks().
  // It finds a decomposition of the TSplineFit spectrum into gaussian components
  //using the ROOT class TSpectrum. Method FindFit() must return a positive result:
  //SplineFitDB.rdb MUST contain a fit of category 16, with the same name and title
  //as this TLitSpectrum class.
  //
  // A text file, named "Spectrum.C" is produced by this method, which can be used as
  //a starting point to produce the final macro defining this TLitSpectrum. Only
  //remains the addition of the decay/rise times and the replacement of the "blabla"
  //by usefule comments
  //
  //  Arguments
  //
  // deconIterations : number of iterations in deconvolution operation. I do not now
  //                   exactly what this argument given to TSpectrum::SearchHighRes()
  //                   is, but I have found experimentally that one has to vary it
  //                   in a range of [3-150] to find a good fit!
  // threshold       : threshold value in % for selected peaks, peaks with amplitude
  //                   less than threshold*highest_peak/100 are ignored
  // sigma           : sigma of searched peaks [???]. Again, I do not know exactly
  //                   what this argument of TSpectrum::SearchHighRes() do, but probably
  //                   you can avoid peaks with too low sigmas by increasing this value.
  // tooclose        : peaks found by TSpectrum::SearchHighRes() closer than tooclose
  //                   from the edges (xmin,xmax) of the spectrum are rejected before
  //                   the fit
  // MeanFixed       : If true [default false], the mean values of the peaks found by
  //                   TSpectrum are fixed, not moved by the fit
  // bavard          : Put it true if you want printed info on the found peaks.
  //
  //  Do not hesitate to call FindPeaks repeatedly varying its arguments until you
  //are satisfied with the obtained decomposition into gaussians
  //  At the exit of this method, a text file "Spectrum.C" is produced, into which
  //you have still to enter the time components to have a CINT file ready for
  //definig this spectrum.
  //
  const Int_t    nchanmin   = 100;
  const Int_t    mpeaks     = 40;
  const Double_t zero       = 0.0;
  const Double_t Nphot      = 1.0e+6;
  const Double_t StartSigma = 12.0;
  const Double_t rac2pi     = 2.5066282746310002;
  const Int_t    width      = 20;
  const Int_t    precision  = 12;
  const Option_t *option    = "";
  Bool_t    ok = kTRUE;
  Int_t     nchan;
  Int_t     facnchan;
  Double_t  xmin;
  Double_t  xmax;
  Float_t   xp,yp;
  Float_t  *source, *dest;
  Float_t  *PositionX;
  Float_t  *PositionY;
  Float_t  *xpeaks;
  Double_t  x,w,step,steps2,S,error;
  Double_t *ppar  = 0;
  TF1      *ppfit = 0;
  Int_t     i,bin,p,nfound;
  Int_t     first,last,size;
  ofstream  cintcode;
  //
  Double_t a,m,sig,chi2;
  if (!fSplineFit) FindFit(kTRUE);
  if (!fSplineFit) return kFALSE;
  fSplineFit->DrawFit();
  if (fH) {
    delete fH;
    fH = 0;
  }
  nchan     = fSplineFit->GetNbOfMeas();
  if (nchan%2) nchan--;
  facnchan = 1;
  if (nchan<nchanmin) {
    while (facnchan*nchan<nchanmin) facnchan *= 2;
  }
  nchan    *= facnchan;
  xmin      = fSplineFit->GetXmin();
  if (xmin<TLitPhys::Get()->MinWaveL()) xmin = TLitPhys::Get()->MinWaveL();
  xmax      = fSplineFit->GetXmax();
  if (xmax>TLitPhys::Get()->MaxWaveL()) xmax = TLitPhys::Get()->MaxWaveL();
  fH = new TH1F("InGaussians","Decomposition of spectrum into gaussians",nchan,xmin,xmax);
  first = fH->GetXaxis()->GetFirst();
  last  = fH->GetXaxis()->GetLast();
  size  = last-first+1;
  if (bavard) {
    cout << "first : " << first << endl;
    cout << "last  : " << last  << endl;
    cout << "size  : " << size  << endl;
  }
  source = new Float_t[size];
  dest   = new Float_t[size];
  step   = (xmax-xmin)/nchan;
  steps2 = step/2.0;
  x = xmin + steps2;
  for (bin=first;bin<=last;bin++) {
    i     = (bin-1)/facnchan;
    w     = fSplineFit->V(x);
    error = fSplineFit->GetMeasErr(i);
    fH->SetBinContent(bin,w);
    fH->SetBinError(bin,error);
    x += step;
  }
  //Use TSpectrum to find the peak candidates
  TSpectrum *sp = new TSpectrum(mpeaks);
  for (i=0;i<size;i++) source[i] = fH->GetBinContent(i + first);
  if (sigma <= 1.0) {
    sigma = size/mpeaks;
    if (sigma < 1) sigma = 1;
    if (sigma > 8) sigma = 8;
  }
  gLitSpecNb = sp->SearchHighRes(source,dest,size,sigma,100*threshold,kFALSE,
    deconIterations,kFALSE,3);
  PositionX = sp->GetPositionX();
  PositionY = sp->GetPositionY();
  for (i = 0; i < gLitSpecNb; i++) {
    bin = first + Int_t(PositionX[i] + 0.5);
    PositionX[i] = fH->GetBinCenter(bin);
    PositionY[i] = fH->GetBinContent(bin);
  }
  delete [] source;
  delete [] dest;
  if (!gLitSpecNb) return kFALSE;
  TPolyMarker *fPM = (TPolyMarker*)fH->GetListOfFunctions()->FindObject("TPolyMarker");
  if (fPM) {
    fH->GetListOfFunctions()->Remove(fPM);
    delete fPM;
  }
  fPM = new TPolyMarker(gLitSpecNb, PositionX, PositionY);
  fH->GetListOfFunctions()->Add(fPM);
  fPM->SetMarkerStyle(23);
  fPM->SetMarkerColor(kRed);
  fPM->SetMarkerSize(1.3);
  ((TH1*)fH)->Draw(option);
  if (bavard) {
    cout << endl;
    cout << " Nb. of peaks found: " << gLitSpecNb << endl;
    cout << endl;
    for (i=0;i<gLitSpecNb;i++) {
      cout << "peak: ";
      cout.width(4);
      cout << i << "   X = ";
      cout.width(width);
      cout.precision(precision);
      cout << PositionX[i] << "   Y = ";
      cout.width(width);
      cout.precision(precision);
      cout << PositionY[i] << endl;
    }
  }
  //Loop on all found peaks. Eliminate peaks too close to edge
  xpeaks = sp->GetPositionX();
  nfound = gLitSpecNb;
  if (ppar) {
    delete [] ppar;
    ppar = 0;
  }
  ppar = new Double_t [3*nfound];
  gLitSpecNb  = 0;
  for (p=0;p<nfound;p++) {
    xp = xpeaks[p];
    if ((TMath::Abs(xp-xmin)<tooclose) || (TMath::Abs(xmax-xp)<tooclose)) {
      cout << "Peak too close to edge. We eliminate it.   xp = " << xp << endl;
    }
    else {
      bin = fH->GetXaxis()->FindBin(xp);
      yp  = fH->GetBinContent(bin);
      ppar[3*gLitSpecNb+1] = xp;
      ppar[3*gLitSpecNb+2] = StartSigma;
      ppar[3*gLitSpecNb]   = yp*rac2pi*ppar[3*gLitSpecNb+2];
      gLitSpecNb++;
    }
  }
  TVirtualFitter::Fitter(fH,10+3*gLitSpecNb); //we may have more than the default 25 parameters
  ppfit = new TF1("fit",SumOfGaussians,xmin,xmax,3*gLitSpecNb);
  ppfit->SetParameters(ppar);
  ppfit->SetNpx(nchan);
  for (i=0;i<gLitSpecNb;i++) {
    ppfit->SetParLimits(3*i,zero,Nphot);
    if (MeanFixed) ppfit->FixParameter(3*i+1,ppar[3*i+1]);
    else ppfit->SetParLimits(3*i+1,xmin,xmax);
    ppfit->SetParLimits(3*i+2,zero,xmax-xmin);
  }
  if (bavard) {
    cout << endl;
    cout << "  Starting values for fit:" << endl;
    cout << endl;
    for (i=0;i<gLitSpecNb;i++) {
      cout << "peak: ";
      cout.width(4);
      cout << i << "   norm: ";
      cout.width(width);
      cout.precision(precision);
      cout << ppar[3*i];
      cout << "   mean: ";
      cout.width(width);
      cout.precision(precision);
      cout << ppar[3*i+1] << "   sigma: ";
      cout.width(width);
      cout.precision(precision);
      cout << ppar[3*i+2] << endl;
    }
  }
  cout << endl;
  cout << "Now fitting: Be patient" << endl;
  cout << endl;
  fH->Fit(ppfit,"");
  if (bavard) {
    S = zero;
    for (i=0;i<gLitSpecNb;i++) {
      a   = ppfit->GetParameter(3*i);
      S  += a;
      m   = ppfit->GetParameter(3*i+1);
      sig = ppfit->GetParameter(3*i+2);
      cout << "peak: ";
      cout.width(4);
      cout << i << "   norm: ";
      cout.width(width);
      cout.precision(precision);
      cout << a;
      cout << "   mean: ";
      cout.width(width);
      cout.precision(precision);
      cout << m << "   sigma: ";
      cout.width(width);
      cout.precision(precision);
      cout << sig << endl;
    }
    chi2 = ppfit->GetChisquare();
    cout << endl;
    cout << "Chi2 of fit             : " << chi2 << endl;
    cout << "Surface on fitted curve : " << S << endl;
    cout << "Surface normalized      : " << S/(Nphot) << endl;
    cout << endl;
  }
  if (ok) {
    cintcode.open("Spectrum.C",ios::out);
    cintcode << "TLitSpectrum* " << GetName();
    cintcode << "(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)";
    cintcode << endl;
    cintcode << '{' << endl;
    cintcode << "//" << endl;
    cintcode << "// Please replace ... by the time components" << endl;
    cintcode << "// Please replace \"blabla\" by useful comments" << endl;
    cintcode << "// Then this CINT code will be ready for the definition" << endl;
    cintcode << "//  of this emission spectrum !" << endl;
    cintcode << "//" << endl;
    cintcode << "  char c;" << endl;
    cintcode << "  Bool_t ok;" << endl;
    cintcode << "  TLitSpectrum *sp;" << endl;
    cintcode << "  sp = new TLitSpectrum(";
    cintcode << '"' << GetName() << '"' << ',' << '"' << GetTitle() << '"' << ");" << endl;
    for (i=0;i<gLitSpecNb;i++) {
      cintcode << "  sp->AddOneComponent(";
      cintcode.width(width);
      cintcode.precision(precision);
      cintcode << ppfit->GetParameter(3*i+1);
      cintcode << ',';
      cintcode.width(width);
      cintcode.precision(precision);
      cintcode << ppfit->GetParameter(3*i+2);
      cintcode << ',';
      cintcode.width(width);
      cintcode.precision(precision);
      cintcode << ppfit->GetParameter(3*i);
      cintcode << ",...);" << endl;
    }
    cintcode << "  sp->Normalize();" << endl;
    cintcode << "  sp->SetMacro(\"Lit" << GetName() << ".C\");" << endl;
    cintcode << "//Up to 10 lines of comments are allowed. We write only 2 here" << endl;
    cintcode << "  sp->fComment[0] = \"blabla\";" << endl;
    cintcode << "  sp->fComment[1] = \"blabla\";" << endl;
    cintcode << "  if (todraw) {" << endl;
    cintcode << "    if (!gTwoPad) {" << endl;
    cintcode << "      gLit = new TLitGlob(5);" << endl;
    cintcode << "      gLit->BookCanvas();" << endl;
    cintcode << "    }" << endl;
    cintcode << "    sp->DrawDecayTimes();" << endl;
    cintcode << "    sp->DrawSpectrum();" << endl;
    cintcode << "    sp->Print();" << endl;
    cintcode << "  }" << endl;
    cintcode << "  if (infile) {" << endl;
    cintcode << "    if (firstinfile) sp->UpdateFile(kTRUE);" << endl;
    cintcode << "    else             sp->UpdateFile(kFALSE);" << endl;
    cintcode << "  }" << endl;
    cintcode << "  return sp;" << endl;
    cintcode << '}' << endl;
    cintcode << endl;
    cintcode.close();
  }
  if (ppfit) delete ppfit;
  if (ppar) delete [] ppar;
  return ok;
}
TLitSpectrum *TLitSpectrum::FindSpectrum(const char *name,Bool_t storeit) {
  // Finds the spectrum specified by name. If the specified spectrum is not found
  //in the collection fgSpectra, then the file fgSpectrumFile is searched! If the
  //spectrum is found in the file, it is put into the collection in case storeit
  //true [default], it is not in case storeit false.
  //
  // 0 returned if spectrum found neither in collection, nor in file.
  Int_t i,N,nspectra,nb;
  TString s1;
  TLitSpectrum *spectrum = 0;
  Bool_t found = kFALSE;
  if (!fgFileName) NameFile();
  if (!fgSpectra) fgSpectra = new TObjArray();
  N = fgSpectra->GetEntries();
  //Search in the collection
  if (N>0) {
    i = 0;
    do {
      spectrum = (TLitSpectrum *)(*fgSpectra)[i];
      s1  = spectrum->GetName();
      found = !s1.CompareTo(name);
      i++;
    } while ((!found) && (i<N));
  }//end if (N>0)
  if (!found) {
    //Search in the file
    fgSpectrumFile = 0;
    fgSpectrumFile = new TFile(fgFileName->Data(),"READ");
    if (fgSpectrumFile) {
      fgSpectrumTree = 0;
      fgSpectrumTree = (TTree *)fgSpectrumFile->Get("AllSpectra");
      if (fgSpectrumTree) {
        spectrum = new TLitSpectrum();
        fgSpectrumBranch = 0;
        fgSpectrumBranch = fgSpectrumTree->GetBranch("Spectra");
        if (fgSpectrumBranch) {
          fgSpectrumBranch->SetAddress(&spectrum);
          nspectra = (Int_t)fgSpectrumTree->GetEntries();
          nb   = 0;
          i    = 0;
          while ((!found) && (i<nspectra)) {
            nb += fgSpectrumTree->GetEntry(i);
            s1  = spectrum->GetName();
            if (!s1.CompareTo(name)) found = kTRUE;
            i++;
          }
          if (found && storeit) AddSpectrum(spectrum);
          else {
            if (!found) {
              delete spectrum;
              spectrum = 0;
            }
          }
        }//end if (fgSpectrumBranch)
        else cout << "TLitSpectrum::FindSpectrum   Branch \"Spectra\" not in Tree \"AllSpectra\"" << endl;
      }//end if (fgSpectrumTree)
      else cout << "TLitSpectrum::FindSpectrum   Tree \"AllSpectra\" not in file SpectraDB.rdb" << endl;
      fgSpectrumFile->Close();
      delete fgSpectrumFile;
      fgSpectrumFile   = 0;
      fgSpectrumTree   = 0;
      fgSpectrumBranch = 0;
    }//end if (fgSpectrumFile)
    else cout << "TLitSpectrum::FindSpectrum   File SpectraDB.rdb not found" << endl;
  }//end if (!found)
  if (!found) spectrum=0;
  return spectrum;
}
Int_t TLitSpectrum::GenWaveLAndDecayT(Double_t &wavelength,Double_t &decaytime) {
  // Generates wavelength and decay time in nanosecond. Returns which gaussian
  //component has been selected
  Bool_t ok;
  Int_t kcp = 0;
  TLitSpectrumCp *p;
  if (!fNormalized) Normalize();
  // Choice of which component
  do {
    if (fNbComponents>1) {
      Bool_t found = kFALSE;
      Short_t k;
      Double_t x;
      x = gRandom3->Rndm();
      k = 0;
      while ((!found) && (k<fNbComponents)) {
        if (x<=fArea[k]) {
          found = kTRUE;
          kcp = k;
        }
        k++;
      }//end while ((!found) && (k<fNbTimes))
      if (!found) {
        Error("TLitSpectrum::GenWaveLAndDecayT","Component not found");
        kcp = fNbComponents - 1;
      }
    }//end if (fNbComponents>1)
    p = (TLitSpectrumCp*)fComponents[kcp];
    wavelength = p->GenWaveLength(ok);
    if (ok) decaytime  = p->GenDecayTime();
  } while (!ok);
  return kcp;
}
void TLitSpectrum::Init() {
  //Put here the description of the method
  Short_t k;
  fNormalized    = kFALSE;
  fInSplineFitDB = kFALSE;
  fNbComponents  = 0;
  fMacro         = "";
  fSource        = "";
  fXLabel        = "Wavelength [nm]";
  fVLabel        = "Intensity";
  fArea          = 0;
  fSplineFit     = 0;
  for (k=0;k<10;k++) fComment[k] = "";
  fFitGraph      = 0;
  fH             = 0;
  fPM            = 0;
}
void TLitSpectrum::InitStatic() {
  // Initialization of all static pointers
  if (!fgFileName)   NameFile();
  if (!fgSpectra) fgSpectra = new TObjArray();
}
Bool_t TLitSpectrum::IsEqual(const TObject* obj) const {
  // 2 spectra are considered as equal if they have the same name
  Int_t k;
  TLitSpectrum *p = (TLitSpectrum*)obj;
  TString s1 = GetName();
  TString s2 = p->GetName();
  k = s1.CompareTo(s2);
  return !k;
}
Bool_t TLitSpectrum::IsInCollection() const {
  // Returns true if "this" is in collection fgSpectra, false otherwise.
  TLitSpectrum *spectrum;
  TIter next(fgSpectra);
  Bool_t found = kFALSE;
  while ((!found) && (spectrum = (TLitSpectrum *)next()))  found = spectrum==this;
  return found;
}
Bool_t TLitSpectrum::IsInCollection(TLitSpectrum *pf) {
  // Returns true if spectrum pf is in collection fgSpectra, false otherwise.
  TLitSpectrum *spectrum;
  if (!fgSpectra) fgSpectra = new TObjArray();
  TIter next(fgSpectra);
  Bool_t found = kFALSE;
  while ((!found) && (spectrum = (TLitSpectrum *)next()))  found = spectrum==pf;
  return found;
}
void TLitSpectrum::NameFile(const char *name) {
  // Give a [tree]name to the database of spectra. Default SpectraDB.rdb. If you want
  //to place this file into an other directory then the one containing the executable
  //litrani [linux] or litrani.exe [Windows], then call NameFile() at the beginning of
  //all your CINT macros, in the following way:
  //
  // TLitSpectrum::NameFile("/home/mnt/gentit/mydatabases/SpectraDB.rdb"); //[linux]
  // TLitSpectrum::NameFile("D:\\databases\\mydatabases\\SpectraDB.rdb");  //[Windows]
  //
  if (!fgFileName) fgFileName = new TString(name);
}
void TLitSpectrum::Normalize() {
  // To be called after having entered all gaussian components with all their time
  //components
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Short_t k;
  Double_t amp,sumB;
  Double_t sumA = zero;
  TLitSpectrumCp *q;
  Int_t NbCp,ntimes;
  NbCp = fComponents.GetEntries();
  if (NbCp != fNbComponents) {
    Error("TLitSpectrum::Normalize","Bad number of components in collection");
    fNbComponents = NbCp;
  }
  TIter next(&fComponents);
  while ((q = (TLitSpectrumCp*)next())) {
    amp = q->GetGaussAmpNN();
    if (amp<=zero) {
      sumB = zero;
      ntimes = q->GetNbTimes();
      for (k=0;k<ntimes;k++) sumB += q->GetTimeWeightNN(k);
      q->SetGaussAmpNN(sumB);
    }
  }
  next.Reset();
  while ((q = (TLitSpectrumCp*)next())) {
    sumA += q->GetGaussAmpNN();
    q->Normalize();
  }
  next.Reset();
  fArea = new Double_t [fNbComponents];
  sumB = zero;
  k = 0;
  while ((q = (TLitSpectrumCp*)next())) {
    amp   = q->GetGaussAmpNN()/sumA;
    sumB += amp;
    fArea[k] = sumB;
    q->SetGaussAmp(amp);
    k++;
  }
  fArea[k-1]  = un;
  fNormalized = kTRUE;
}
Bool_t TLitSpectrum::OrderFile(Bool_t destroycopy) {
  // rewrites the file with all its elements ordered. If destroycopy
  //is true, the copied database file with suffix .rdbold is destroyed, else not.
  const Int_t    bufsize = 64000;
  const char *win32 = "win32";
  Int_t i,nb,N;
  TString s,s2,s3;
  TFile *lSpectrumFile;
  TTree *lSpectrumTree;
  TBranch *lSpectrumBranch;
  TLitSpectrum *spectrum,*pspectrum;
  if (!fgFileName) NameFile();
  if (!fgSpectra) fgSpectra = new TObjArray();
  TIter next(fgSpectra);
  s = *fgFileName;
  s.Append("old");
  //First make a copy of the file
  fgSpectrumFile = 0;
  fgSpectrumFile = new TFile(fgFileName->Data(),"READ");
  if (fgSpectrumFile) {
    fgSpectrumTree = 0;
    fgSpectrumTree = (TTree *)fgSpectrumFile->Get("AllSpectra");
    if (fgSpectrumTree) {
      spectrum = new TLitSpectrum();
      fgSpectrumBranch = 0;
      fgSpectrumBranch = fgSpectrumTree->GetBranch("Spectra");
      if (fgSpectrumBranch) {
        fgSpectrumBranch->SetAddress(&spectrum);
        N = (Int_t)fgSpectrumTree->GetEntries();
        lSpectrumFile = new TFile(s.Data(),"RECREATE");
        lSpectrumTree = new TTree("AllSpectra","AllSpectra");
        lSpectrumBranch = lSpectrumTree->Branch("Spectra","TLitSpectrum",&spectrum,bufsize,0);
        nb = 0;
        i  = 0;
        while (i<N) {
          nb += fgSpectrumTree->GetEntry(i);
          lSpectrumTree->Fill();
          i++;
        }//end while (i<N)
        lSpectrumFile->Write();
        lSpectrumFile->Close();
        delete lSpectrumFile;
        lSpectrumFile    = 0;
        lSpectrumTree    = 0;
        lSpectrumBranch  = 0;
      }//end if (fgSpectrumBranch)
      else {
        cout << "TLitSpectrum::OrderFile   Branch \"Spectra\" not in Tree \"AllSpectra\"" << endl;
        delete spectrum;
        spectrum = 0;
        fgSpectrumFile->Close();
        delete fgSpectrumFile;
        fgSpectrumFile = 0;
        fgSpectrumTree = 0;
        return kFALSE;
      }
      delete spectrum;
      spectrum = 0;
    }//end if (fgSpectrumTree)
    else {
      cout << "TLitSpectrum::OrderFile   Tree \"AllSpectra\" not in file SpectraDB.rdb" << endl;
      fgSpectrumFile->Close();
      delete fgSpectrumFile;
      fgSpectrumFile = 0;
      return kFALSE;
    }//end else if (fgSpectrumTree)
    fgSpectrumFile->Close();
    delete fgSpectrumFile;
    fgSpectrumFile   = 0;
    fgSpectrumTree   = 0;
    fgSpectrumBranch = 0;
  }//end if (fgSpectrumFile)
  else {
    cout << "TLitSpectrum::OrderFile   File SpectraDB.rdb not found" << endl;
    return kFALSE;
  }//end else if (fgSpectrumFile)
  //
  // Then put all elements into the collection, order them and copy them to the new file
  //
  fgSpectra->Delete();
  lSpectrumFile = 0;
  lSpectrumFile = new TFile(s.Data(),"READ");
  if (lSpectrumFile) {
    lSpectrumTree = (TTree *)lSpectrumFile->Get("AllSpectra");
    if (lSpectrumTree) {
      spectrum = new TLitSpectrum();
      lSpectrumBranch = 0;
      lSpectrumBranch = lSpectrumTree->GetBranch("Spectra");
      if (lSpectrumBranch) {
        lSpectrumBranch->SetAddress(&spectrum);
        N = (Int_t)lSpectrumTree->GetEntries();
        nb = 0;
        i  = 0;
        while (i<N) {
          nb += lSpectrumTree->GetEntry(i);
          pspectrum = new TLitSpectrum(*spectrum);
          AddSpectrum(pspectrum);
          i++;
        }//end while (i<N)
      }//end if (lSpectrumBranch)
      else {
        cout << "TLitSpectrum::OrderFile   Branch \"Spectra\" not in Tree \"AllSpectra\"" << endl;
        return kFALSE;
      }
      delete spectrum;
      spectrum = 0;
    }//end if (lSpectrumTree)
    else {
      cout << "TLitSpectrum::OrderFile   Tree \"AllSpectra\" not in file SpectraDB.rdbold" << endl;
      lSpectrumFile->Close();
      delete lSpectrumFile;
      return kFALSE;
    }//end else if (lSpectrumTree)
    lSpectrumFile->Close();
    delete lSpectrumFile;
    lSpectrumFile   = 0;
    lSpectrumTree   = 0;
    lSpectrumBranch = 0;
  }//end if (lSpectrumFile)
  else {
    cout << "TLitSpectrum::OrderFile   File SpectraDB.rdbold not found" << endl;
    return kFALSE;
  }//end else if (lSpectrumFile->IsOpen())
  fgSpectra->UnSort();
  fgSpectra->Sort();
  //
  // Take all elements in collection and write them to the new file
  //
  fgSpectrumFile = 0;
  fgSpectrumFile = new TFile(fgFileName->Data(),"RECREATE");
  if (fgSpectrumFile) {
    fgSpectrumTree = 0;
    fgSpectrumTree = new TTree("AllSpectra","AllSpectra");
    if (fgSpectrumTree) {
      //      spectrum = new TLitSpectrum();
      fgSpectrumBranch = 0;
      fgSpectrumBranch = fgSpectrumTree->Branch("Spectra","TLitSpectrum",&spectrum,bufsize,0);
      if (fgSpectrumBranch) {
        next.Reset();
        while ((pspectrum = (TLitSpectrum *)next())) {
          spectrum = pspectrum;
          fgSpectrumTree->Fill();
        }//end while (pspectrum = (TLitSpectrum *)next())
      }//end if (fgSpectrumBranch)
      else {
        cout << "TLitSpectrum::OrderFile   Branch \"Spectra\" not created" << endl;
        return kFALSE;
      }//end else if (fgSpectrumBranch)
    }//end if (fgSpectrumTree)
    else {
      cout << "TLitSpectrum::OrderFile   Tree \"AllSpectra\" not created" << endl;
      fgSpectrumFile->Close();
      delete fgSpectrumFile;
      fgSpectrumFile   = 0;
      return kFALSE;
    }//end else if (fgSpectrumTree)
    fgSpectrumFile->Write();
    fgSpectrumFile->Close();
    delete fgSpectrumFile;
    fgSpectrumFile   = 0;
    fgSpectrumTree   = 0;
    fgSpectrumBranch = 0;
  }//end if (fgSpectrumFile)
  else {
    cout << "TLitSpectrum::OrderFile   File SpectraDB.rdb not created" << endl;
    return kFALSE;
  }//end else if (fgSpectrumFile)
  if (destroycopy) {
    s2 = gSystem->GetBuildArch();
    if (!s2.CompareTo(win32)) {
      s3 = ".!del /q ";
      s3.Append(s.Data());
    }
    else {
      s3 = ".!rm -f ";
      s3.Append(s.Data());
    }
    gROOT->ProcessLine(s3.Data());
  }
  return kTRUE;
}
void TLitSpectrum::Print() const {
  // Prints everything
  Short_t k;
  TLitSpectrumCp *cp;
  cout << endl;
  cout << "          " << GetName() << endl;
  cout << "          " << GetTitle() << endl;
  cout << endl;
  cout << "Amplitudes have been normalized      : " << fNormalized    << endl;
  cout << "A SplineFit of this spectrum exists  : " << fInSplineFitDB << endl;
  cout << "Date of creation of this spectrum    : " << fDate.Data()   << endl;
  cout << "Source of measurements               : " << fSource.Data() << endl;
  cout << "Name of CINT macro for this spectrum : " << fMacro.Data()  << endl;
  PrintComment();
  cout << "Number of gaussian components        : " << fNbComponents  << endl;
  for (k=0;k<fNbComponents;k++) {
    cout << endl;
    cout << "        Component " << k+1 << endl;
    cout << endl;
    cp = (TLitSpectrumCp*)fComponents[k];
    cp->Print();
  }
  cout << endl;
}
void TLitSpectrum::PrintComment() const {
  // Comment about this spectrum is printed
  Short_t k;
  if (fComment[0].Length() > 0) {
    cout << endl;
    cout << "    Comment about this spectrum" << endl;
    cout << endl;
  }
  for (k=0;k<10;k++) {
    if (fComment[k].Length() > 0) cout << fComment[k].Data() << endl;
  }
  cout << endl;
}
void TLitSpectrum::PrintSpectrum2File(const char* filename,Int_t npoints,Int_t style, Double_t cutoff) const {
  // Routine to print the Spectrum to a text file (filename) for graphing or for 
  //input to other programs, such as analysis programs or geant4. The file will
  //contain "npoints" number of points. Cutoff refers to the minimum relative 
  //intensity above which points should be considered, i.e. ideirectly fixing 
  //the maximum and minumum photon energies displayed for the spectrum.
  // By default, the routine produces a column table, the first column is the
  //photon energy, the second is the relative intensity of the spectrum.
  // If (style = 1), the routine produces a file for easy input into geant4 code,
  //in essence a C++ array.
  //Styles:
  //default = table with xvalue = wavelength, yvalue = relative intensity
  //
  ofstream spectrumfile(filename);
  char wvl[30];
  char v[30];
  Double_t maxwl;
  Double_t minwl;
  Double_t maximum = 0; 
  Int_t j,k;
  Double_t step;
  Double_t xx[1];
  TArrayD x(npoints), y(npoints);
  TArrayD par(3*fNbComponents);
  TLitSpectrumCp *q;
  TIter next(&fComponents);
  gLitSpecNb = fNbComponents;
  k = 0;
  while ((q = (TLitSpectrumCp*)next())) {
    j = 3*k;
    par[j]   = q->GetGaussAmp();
    par[j+1] = q->GetGaussMean();
    par[j+2] = q->GetGaussSigma();
    k++;
  }
  maxwl = TLitPhys::Get()->MaxWaveL();
  minwl = TLitPhys::Get()->MinWaveL();
  step  = (maxwl - minwl)/(npoints-1);
  xx[0] = minwl;
  for (k=0;k<npoints;k++) {
    y[k]   = SumOfGaussians(xx,par.fArray);
    if (y[k]> maximum) maximum = y[k];
    xx[0] += step;
  }
  xx[0] = minwl;
  for (k=0;k<npoints;k++) {
    y[k]   = SumOfGaussians(xx,par.fArray);
    if (y[k]>=cutoff*maximum && k!=0) {minwl = xx[0]-step,k=npoints;}
    xx[0] += step;
  }
  xx[0] = maxwl;
  for (k=0;k<npoints;k++) {
    y[k]   = SumOfGaussians(xx,par.fArray);
    if (y[k]>=cutoff*maximum && k!=0) {maxwl = xx[0]+step,k=npoints;}
    xx[0] -= step;
  }
  step  = (maxwl - minwl)/(npoints-1);  
  xx[0] = minwl;
  for (k=0;k<npoints;k++) {
    x[k]   = xx[0];
    y[k]   = SumOfGaussians(xx,par.fArray);
    xx[0] += step;
  }
  Double_t energy = 0;
  switch (style) {
  case 1:
    for (Int_t i = 0; i<npoints; ++i) {
      if (i==0) {
        sprintf(wvl, "G4double Energy[] = {");
        spectrumfile << wvl;
        energy = 1238./x[npoints-(i+1)];
        sprintf(wvl," %f*eV",energy);
        spectrumfile << wvl;}
      else {
        energy = 1238./x[npoints-(i+1)];
        sprintf(wvl,", %f*eV",energy);
        spectrumfile << wvl;}
      if (i==npoints-1) {
        sprintf(wvl, "};");
        spectrumfile << wvl << endl;}
    }

    for (Int_t i = 0; i<npoints; ++i) {
      if (i==0) {
        sprintf(v, "G4double Spectrum[] = {");
        spectrumfile << v;
        sprintf(v," %f",y[npoints-(i+1)]);
        spectrumfile << v;}
      else {
        sprintf(v,", %f", y[npoints-(i+1)]);
        spectrumfile << v;}
      if (i==npoints-1) {
        sprintf(v, "};");
        spectrumfile << v << endl;}
    }
    break;
  default:
    for (Int_t i = 0; i<npoints; ++i) {
      spectrumfile << x[i] << " " << y[i] << endl;}
  }
}
void TLitSpectrum::Purge() {
  // The collection fgSpectra is purged
  if (!fgSpectra) fgSpectra = new TObjArray();
  fgSpectra->Delete();
  fgNextDraw = 0;
}
void TLitSpectrum::PurgeStatic() {
  // delete all static pointers
  if (fgSpectrumFile) {
    fgSpectrumFile->Close();
    delete fgSpectrumFile;
    fgSpectrumFile   = 0;
    fgSpectrumTree   = 0;
    fgSpectrumBranch = 0;
  }
  if (fgSpectra) {
    fgSpectra->Delete();
    delete fgSpectra;
    fgSpectra  = 0;
    fgNextDraw = 0;
  }
  if (fgFileName) {
    delete fgFileName;
    fgFileName = 0;
  }
}
Bool_t TLitSpectrum::RemoveSpectrumFromFile(const char *name,Bool_t destroycopy) {
  // Remove spectrum named "name" from the file. If not found, returns false. If destroycopy
  //is true, the old copied database file with suffix .rdbold is destroyed, else not
  const Int_t    bufsize = 64000;
  const char *win32 = "win32";
  Int_t i,nb,N;
  TString s,s2,s3;
  TFile *lSpectrumFile;
  TTree *lSpectrumTree;
  TBranch *lSpectrumBranch;
  Bool_t ok = kFALSE;
  TLitSpectrum *spectrum;
  if (!fgFileName) NameFile();
  if (!fgSpectra) fgSpectra = new TObjArray();
  s = *fgFileName;
  s.Append("old");
  spectrum = FindSpectrum(name,kFALSE);
  if (spectrum) {
    fgSpectra->Remove(spectrum);
    spectrum = FindSpectrum(name,kFALSE);
    if (spectrum) {
      //First make a copy of the file
      delete spectrum;
      spectrum = 0;
      fgSpectrumFile = 0;
      fgSpectrumFile = new TFile(fgFileName->Data(),"READ");
      if (fgSpectrumFile) {
        fgSpectrumTree = 0;
        fgSpectrumTree = (TTree *)fgSpectrumFile->Get("AllSpectra");
        if (fgSpectrumTree) {
          fgSpectrumBranch = 0;
          fgSpectrumBranch = fgSpectrumTree->GetBranch("Spectra");
          if (fgSpectrumBranch) {
            spectrum = new TLitSpectrum();
            fgSpectrumBranch->SetAddress(&spectrum);
            N = (Int_t)fgSpectrumTree->GetEntries();
            lSpectrumFile = new TFile(s.Data(),"RECREATE");
            lSpectrumTree = new TTree("AllSpectra","AllSpectra");
            lSpectrumBranch = lSpectrumTree->Branch("Spectra","TLitSpectrum",&spectrum,bufsize,0);
            nb = 0;
            i  = 0;
            while (i<N) {
              nb += fgSpectrumTree->GetEntry(i);
              lSpectrumTree->Fill();
              i++;
            }//end while (i<N)
            lSpectrumFile->Write();
            lSpectrumFile->Close();
            delete lSpectrumFile;
            lSpectrumFile    = 0;
            lSpectrumTree    = 0;
            lSpectrumBranch  = 0;
            fgSpectrumFile->Close();
            delete fgSpectrumFile;
            fgSpectrumFile   = 0;
            fgSpectrumTree   = 0;
            fgSpectrumBranch = 0;
            delete spectrum;
            spectrum = 0;
            //Then recreate the database file from the copy, omitting "name"
            Bool_t thisone;
            lSpectrumFile = new TFile(s.Data(),"READ");
            lSpectrumTree = (TTree *)lSpectrumFile->Get("AllSpectra");
            spectrum = new TLitSpectrum();
            lSpectrumBranch = lSpectrumTree->GetBranch("Spectra");
            lSpectrumBranch->SetAddress(&spectrum);
            N = (Int_t)lSpectrumTree->GetEntries();
            fgSpectrumFile = new TFile(fgFileName->Data(),"RECREATE");
            fgSpectrumTree = new TTree("AllSpectra","AllSpectra");
            fgSpectrumBranch = fgSpectrumTree->Branch("Spectra","TLitSpectrum",&spectrum,bufsize,0);
            nb = 0;
            i  = 0;
            while (i<N) {
              nb += lSpectrumTree->GetEntry(i);
              s2   = spectrum->GetName();
              if (!s2.CompareTo(name)) {
                ok = kTRUE;
                thisone = kTRUE;
              }
              else thisone = kFALSE;
              if (!thisone) fgSpectrumTree->Fill();
              i++;
            }//end while (i<N)
            fgSpectrumFile->Write();
          }//end if (fgSpectrumBranch)
          else {
            cout << "TLitSpectrum::RemoveSpectrumFromFile   Branch \"Spectra\" not found" << endl;
            fgSpectrumFile->Close();
            delete fgSpectrumFile;
            fgSpectrumFile    = 0;
            fgSpectrumTree    = 0;
            return kFALSE;
          }
        }//end if (fgSpectrumTree)
        else {
          cout << "TLitSpectrum::RemoveSpectrumFromFile   Tree \"AllSpectra\" not found" << endl;
          fgSpectrumFile->Close();
          delete fgSpectrumFile;
          fgSpectrumFile    = 0;
          return kFALSE;
        }
        fgSpectrumFile->Close();
        delete fgSpectrumFile;
        fgSpectrumFile    = 0;
        fgSpectrumTree    = 0;
        fgSpectrumBranch  = 0;
        lSpectrumFile->Close();
        delete lSpectrumFile;
        lSpectrumFile   = 0;
        lSpectrumTree   = 0;
        lSpectrumBranch = 0;
        delete spectrum;
        spectrum = 0;
      }//end if (fgSpectrumFile)
      else  {
        cout << "TLitSpectrum::RemoveSpectrumFromFile   File SpectraDB.rdb not found" << endl;
        return kFALSE;
      }
    }//end if (spectrum)
  }//end if (spectrum)
  if (ok && destroycopy) {
    s2 = gSystem->GetBuildArch();
    if (!s2.CompareTo(win32)) {
      s3 = ".!del /q ";
      s3.Append(s.Data());
    }
    else {
      s3 = ".!rm -f ";
      s3.Append(s.Data());
    }
    gROOT->ProcessLine(s3.Data());
  }
  return ok;
}
void TLitSpectrum::SetDefaultLabels() {
  // SetDefaultLabels() will replace the default labels of TwoPadDisplay (not
  //specific to this spectrum) by the default labels specific to this spectrum.
  // If gTwoPad not yet booked, SetDefaultLabels will do it. The default labels are:
  //
  // - label1 : source of measurements for this spectrum
  // - label2 : name of the CINT macro having produced this spectrum
  //
  if (!gTwoPad) gLit->BookCanvas();
  gTwoPad->CommentBR(fSource.Data());
  gTwoPad->CommentTR(fMacro.Data());
}
void TLitSpectrum::SetMacro(const char *macro) {
  // Set the name of the CINT macro having produced this spectrum
  fMacro = macro;
}
void TLitSpectrum::SetSource(const char *source) {
  // Set the name of the CINT macro having produced this spectrum
  fSource = source;
}
void TLitSpectrum::SetVLabel(const char *label) {
  // Gives a title to the value axis. Valid for 1D, 2D and 3D fits. The value axis
  //is the y axis for 1D, the z axis for 2D and the v axis for 3D !
  fVLabel = label;
}
void TLitSpectrum::SetXLabel(const char *label) {
  // Gives a title to the X axis. Valid for 1D, 2D and 3D fits
  fXLabel = label;
}
void TLitSpectrum::ShowSpectraInFile() {
  // Prints list of names and titles of all fits in the database root file
  TString s1;
  Int_t nspectra,nb,i;
  TLitSpectrum *spectrum;
  if (!fgFileName) NameFile();
  Int_t N = 0;
  cout << endl;
  cout << "    All Spectra in file "<< TLitSpectrum::fgFileName->Data() << endl;
  cout << endl;
  fgSpectrumFile = 0;
  fgSpectrumFile = new TFile(fgFileName->Data(),"READ");
  if (fgSpectrumFile) {
    fgSpectrumTree = 0;
    fgSpectrumTree = (TTree *)fgSpectrumFile->Get("AllSpectra");
    if (fgSpectrumTree) {
      spectrum = new TLitSpectrum();
      fgSpectrumBranch = 0;
      fgSpectrumBranch = fgSpectrumTree->GetBranch("Spectra");
      if (fgSpectrumBranch) {
        fgSpectrumBranch->SetAddress(&spectrum);
        nspectra = (Int_t)fgSpectrumTree->GetEntries();
        nb   = 0;
        for (i=0;i<nspectra;i++) {
          nb += fgSpectrumTree->GetEntry(i);
          cout.width(30);
          cout.fill('.');
          cout.setf(ios::left,ios::adjustfield);
          cout << spectrum->GetName() << "     ";
          cout.width(60);
          cout.fill('.');
          cout.setf(ios::left,ios::adjustfield);
          cout << spectrum->GetTitle() << endl;
          N++;
        }
        cout << endl;
        cout << "Nb. of spectra : " << N << endl;
        cout << endl;
      }//end if (fgSpectrumBranch)
      else cout << "TLitSpectrum::ShowSpectraInFile   Branch \"Spectra\" not found" << endl;
      delete spectrum;
      spectrum = 0;
    }//end if (fgSpectrumTree)
    else cout << "TLitSpectrum::ShowSpectraInFile   Tree \"AllSpectra\" not found" << endl;
    fgSpectrumFile->Close();
    delete fgSpectrumFile;
    fgSpectrumFile   = 0;
    fgSpectrumTree   = 0;
    fgSpectrumBranch = 0;
  }//end if (fgSpectrumFile)
  else cout << "TLitSpectrum::ShowSpectraInFile   File SpectraDB.rdb not found" << endl;
}
Bool_t TLitSpectrum::UpdateFile(Bool_t first) {
  // Put this spectrum into the database file SpectraDB.rdb.
  // WARNING : first must be true ONLY when entering the first spectrum into a not yet
  //existing database file. It must be false in all other cases! Putting it true if
  //the database file already exists will destroy it and replace it with a database
  //file containing just this last spectrum!
  // The name or treename of the database file is contained in the static variable
  //fgFileName which can be changed by the user calling NameFile().
  //
  const Int_t bufsize = 64000;
  TLitSpectrum *spectrum;
  Bool_t ok = kFALSE;
  if (first) {
    char c;
    cout << endl;
    cout << " Do you really want to destroy the database" << endl;
    cout << "SpectraDB.rdb and to replace it by a new one" << endl;
    cout << "containing just this TLitSpectrum ? " << endl;
    cout << " If yes, type 'y' : ";
    cin  >> c;
    if (c == 'y') {
      fgSpectrumFile = 0;
      fgSpectrumFile = new TFile(fgFileName->Data(),"RECREATE");
      if (fgSpectrumFile) {
        fgSpectrumTree = 0;
        fgSpectrumTree = new TTree("AllSpectra","AllSpectra");
        if (fgSpectrumTree) {
          spectrum = this;
          fgSpectrumBranch = 0;
          fgSpectrumBranch = fgSpectrumTree->Branch("Spectra","TLitSpectrum",&spectrum,bufsize,0);
          if (fgSpectrumBranch) {
            ok = kTRUE;
            fgSpectrumTree->Fill();
            fgSpectrumFile->Write();
          }//end if (fgSpectrumBranch)
          else {
            Error("TLitSpectrum::UpdateFile","Branch \"Spectra\" not created");
            fgSpectrumFile->Close();
            delete fgSpectrumFile;
            fgSpectrumFile   = 0;
            fgSpectrumTree   = 0;
            return kFALSE;
          }
        }//end if (fgSpectrumTree)
        else {
          Error("TLitSpectrum::UpdateFile","Tree \"AllSpectra\" not created");
          fgSpectrumFile->Close();
          delete fgSpectrumFile;
          fgSpectrumFile   = 0;
          return kFALSE;
        }
        fgSpectrumFile->Close();
        delete fgSpectrumFile;
        fgSpectrumFile   = 0;
        fgSpectrumTree   = 0;
        fgSpectrumBranch = 0;
      }//end if (fgSpectrumFile)
      else {
        Error("TLitSpectrum::UpdateFile","File SpectraDB.rdb not created");
        return kFALSE;
      }
    }//end if (c == 'y')
    else return kFALSE;
  }//end if (first)
  else {
    if (VerifyNotInFile()) {
      fgSpectrumFile = 0;
      fgSpectrumFile = new TFile(fgFileName->Data(),"UPDATE");
      if (fgSpectrumFile) {
        fgSpectrumTree = 0;
        fgSpectrumTree = (TTree *)fgSpectrumFile->Get("AllSpectra");
        if (fgSpectrumTree) {
          ok = kTRUE;
          spectrum = this;
          fgSpectrumTree->SetBranchAddress("Spectra",&spectrum);
          fgSpectrumTree->Fill();
          fgSpectrumTree->Write("",TObject::kOverwrite);
        }
        else {
          Error("TLitSpectrum::UpdateFile","Tree \"AllSpectra\" not found");
          fgSpectrumFile->Close();
          delete fgSpectrumFile;
          fgSpectrumFile   = 0;
          return kFALSE;
        }
        fgSpectrumFile->Close();
        delete fgSpectrumFile;
        fgSpectrumFile   = 0;
        fgSpectrumTree   = 0;
        fgSpectrumBranch = 0;
      }//end if (fgSpectrumFile)
      else {
        Error("TLitSpectrum::UpdateFile","File SpectraDB.rdb not found");
        return kFALSE;
      }
    }//end if (VerifyNotInFile())
    else ok = kTRUE;
  }//end else if (first)
  return ok;
}
Bool_t TLitSpectrum::VerifyNotInFile() const {
  // Verifies that this spectrum is not in the file
  TString s1,s2;
  Int_t nspec,nb,i;
  TLitSpectrum *spectrum;
  Bool_t found = kFALSE;
  s1 = GetName();
  if (!fgFileName) NameFile();
  fgSpectrumFile = 0;
  fgSpectrumFile = new TFile(fgFileName->Data(),"READ");
  if (fgSpectrumFile) {
    fgSpectrumTree = 0;
    fgSpectrumTree = (TTree *)fgSpectrumFile->Get("AllSpectra");
    if (fgSpectrumTree) {
      fgSpectrumBranch = 0;
      fgSpectrumBranch = fgSpectrumTree->GetBranch("Spectra");
      if (fgSpectrumBranch) {
        spectrum = new TLitSpectrum();
        fgSpectrumBranch->SetAddress(&spectrum);
        nspec = (Int_t)fgSpectrumTree->GetEntries();
        nb   = 0;
        i    = 0;
        while ((!found) && (i<nspec)) {
          nb += fgSpectrumTree->GetEntry(i);
          s2  = spectrum->GetName();
          if (!s1.CompareTo(s2)) found = kTRUE;
          i++;
        }
        delete spectrum;
        spectrum = 0;
      }//end if (fgSpectrumBranch)
      else Error("TLitSpectrum::VerifyNotInFile","Branch \"Spectra\" not found");
    }//end if (fgSpectrumTree)
    else Error("TLitSpectrum::VerifyNotInFile","Tree \"AllSpectra\" not found");
    fgSpectrumFile->Close();
    delete fgSpectrumFile;
    fgSpectrumFile   = 0;
    fgSpectrumTree   = 0;
    fgSpectrumBranch = 0;
  }//end if (fgSpectrumFile)
  else Error("TLitSpectrum::VerifyNotInFile","File SpectraDB.rdb not found");
  return !found;
}
Double_t SumOfGaussians(Double_t *x,Double_t *par) {
  Double_t result = 0.0;
  Double_t norm,mean,sigma;
  for (Int_t p=0;p<gLitSpecNb;p++) {
    norm  = TMath::Abs(par[3*p]);
    mean  = TMath::Abs(par[3*p+1]);
    sigma = TMath::Abs(par[3*p+2]);
    result += norm*TMath::Gaus(x[0],mean,sigma,kTRUE);
  }
  return result;
}
