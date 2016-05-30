// @(#)root/html:$Id: TSellmeier.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TMessErr.h"
#include "TSellmeier.h"

ClassImp(TSellmeier)
//______________________________________________________________________________
//
//BEGIN_HTML <!--
/* -->
</pre>
<h4><font color="#990000">TSellmeier</font></h4>
<P>
In Litrani, &nbsp;you can define index of refraction ( or diagonal components
<FONT FACE="Symbol">e</FONT><SMALL><SUB><SMALL>a</SMALL></SUB></SMALL>
<FONT FACE="Symbol">e</FONT><SMALL><SUB><SMALL>b</SMALL></SUB></SMALL>
<FONT FACE="Symbol">e</FONT><SUB><SMALL>c</SMALL></SUB><FONT FACE="Symbol">
</FONT>of the dielectric tensor
<FONT COLOR="Red"><FONT FACE="Symbol"><BIG><BIG><B>e</B></BIG></BIG></FONT></FONT>&nbsp;in
the system in which it is diagonal ) in 3 ways :
<OL>
<LI>
either these elements are constant, not depending upon wavelength.
<LI>
or they depend upon wavelength and you have provided the dependency by defining
a fit of class TSplineFit giving the dependency of these elements upon wavelength.
<LI>
or you instruct the program to obtain these elements using the Sellmeier
law.
</OL>
<P>
This class TSellmeier is for use in the 3rd case. For a derivation of the
Sellmeier law, see for instance : "Principles of Optics" by Max Born and
Emil Wolf, Cambridge University Press, page 96. We use the Sellmeier law
expressed in a slightly different form, using wavelengths instead of frequencies
:
<P>
<CENTER>
<TABLE BORDER="8" CELLSPACING="8" CELLPADDING="8" ALIGN="Center">
<CAPTION ALIGN="Bottom">
Sellmeier law
</CAPTION>
<TR>
<TD><BIG>n</BIG><SUP>2</SUP><BIG><BIG>/</BIG></BIG><FONT FACE="Symbol"><BIG>m</BIG></FONT><BIG>
- 1 =
n<SMALL><SUB><SMALL><SMALL>0</SMALL></SMALL></SUB></SMALL><SUP><SMALL><SMALL>2</SMALL></SMALL></SUP><BIG>
</BIG>+
<FONT FACE="Symbol"><BIG><BIG><B>S</B></BIG></BIG></FONT>&nbsp;
n<SUB><SMALL>i</SMALL></SUB><SUP><SMALL><SMALL><SMALL>2</SMALL></SMALL></SMALL></SUP>
/ (1 -
<FONT FACE="Symbol">l</FONT><SUB><SMALL>i</SMALL></SUB><SUP><SMALL><SMALL>2</SMALL></SMALL></SUP>/<FONT
FACE="Symbol">l</FONT><SUP><SMALL>2</SMALL></SUP>)</BIG></TD>
<TD>i = 1..N &nbsp;/ n : index, <FONT FACE="Symbol">l</FONT>: wavelength</TD>
</TR>
<TR>
<TD><FONT FACE="Symbol"><BIG>e</BIG></FONT><BIG> - 1 =
n<SMALL><SUB><SMALL><SMALL>0</SMALL></SMALL></SUB></SMALL><SUP><SMALL><SMALL>2</SMALL></SMALL></SUP><BIG>
</BIG>+
<FONT FACE="Symbol"><BIG><BIG><B>S</B></BIG></BIG></FONT>&nbsp;
n<SUB><SMALL>i</SMALL></SUB><SUP><SMALL><SMALL><SMALL>2</SMALL></SMALL></SMALL></SUP>
/ (1 -
<FONT FACE="Symbol">l</FONT><SUB><SMALL>i</SMALL></SUB><SUP><SMALL><SMALL>2</SMALL></SMALL></SUP>/<FONT
FACE="Symbol">l</FONT><SUP><SMALL>2</SMALL></SUP>)</BIG></TD>
<TD>i = 1..N / <FONT FACE="Symbol">e</FONT> : diag. el. of diel. tensor</TD>
</TR>
</TABLE>
</CENTER>
<P>
Notice that in the Sellmeier law for the index, we have written
n<SUP>2</SUP><BIG>/</BIG><FONT FACE="Symbol">m</FONT><BIG> </BIG>- 1 and
not n<SUP>2</SUP><BIG> </BIG>- 1 as in the book of Max Born. It is because
<FONT FACE="Symbol">m</FONT> is supposed to be 1 in the text of Born, but
Litrani accepts any value for <FONT FACE="Symbol">m</FONT>. Take this into
account if you have entered into Litrani ++ values of
<FONT FACE="Symbol">m</FONT> different from 1.
<P>
The experimental parameters to be provided are :
<UL>
<LI>
N : the number of terms in the sum.
<LI>
n<BIG><SMALL><SUB><SMALL><SMALL>0</SMALL></SMALL></SUB></SMALL> </BIG>: the
1st term, always present, associated with the resonant wavelength 0. 
<LI>
(n<SUB>i</SUB> , <FONT FACE="Symbol">l</FONT><SUB>i</SUB>) : N couples of
values (n<SUB>i</SUB> , <FONT FACE="Symbol">l</FONT><SUB>i</SUB>).
<FONT FACE="Symbol">l</FONT><SUB>i</SUB> has to be given in nanometers .
n<BIG><SMALL><SUB><SMALL><SMALL>0</SMALL></SMALL></SUB></SMALL></BIG> and
n<SUB>i</SUB> have no dimension.
</UL>
<P>
The constructor of the class TSellmeier allow you to input up to 3 couples
(n<SUB>i</SUB> , <FONT FACE="Symbol">l</FONT><SUB>i</SUB>) . At least one
couple is mandatory. If you want more than 3, you have to call method
AddComponent() as many times as you want more components. To get then the
value of n at a given value of <FONT FACE="Symbol">l</FONT>, you have to
use method GetIndex(). To get the value of <FONT FACE="Symbol"><BIG>e
</BIG></FONT> at a given value of <FONT FACE="Symbol">l</FONT>, you have
to use the method GetDiel().
<P>
<pre>
<!--*/
// -->END_HTML
//
//______________________________________________________________________________

TSellmeier::TSellmeier(Int_t n,Double_t ns0,Double_t ns1,Double_t ls1,Double_t ns2,
  Double_t ls2,Double_t ns3,Double_t ls3) {
  //  Constructor. See the description of the class. The parameters are the
  //following :
  // n        : nb. of couples (nsi,lsi) used. nsi : constant in numerator
  //             1 <= n <= 3                   lsi : resonant wavelength in nm
  // ns0      : 1st constant term, associated with wavelength 0
  // (ns1,ls1): 1st couple of value (nsi,lsi) is mandatory
  // (ns2,ls2): 2nd couple, mandatory only if n >=2.
  // (ns3,ls3): 3rd couple, mandatory only if n >=3.
  //
  //  If you want more than 3 couples of parameters, call AddComponent() as
  //many times as you want.
  //
  fN = (n-1)%2 + 1;
  if ((n<1) || (n>3)) {
    gMes->SetName("dEdx");
    gMes->SetTitle("TOpticMaterial");
    gMes->M(Warning_M,1,"At least 1, at most 3 couples in the constructor !");
    gMes->M(Warning_M,2,"n is taken to be the value quoted above",n);
  }
  fNs0 = ns0;
  fNsi.Set(fN);
  fLsi.Set(fN);
  fNsi[0] = ns1;
  fLsi[0] = ls1;
  if (fN>1) {
    fNsi[1] = ns2;
    fLsi[1] = ls2;
    if (fN>2) {
      fNsi[2] = ns3;
      fLsi[2] = ls3;
    }
  }
}
void TSellmeier::AddComponent(Double_t nsi,Double_t lsi) {
  //Add one component more to the Sellmeier law.
  //  nsi : constant in numerator
  //  lsi : resonant wavelength in nm
  //
  TArrayD an;
  TArrayD al;
  an.Set(fN,fNsi.fArray);
  al.Set(fN,fLsi.fArray);
  fN++;
  fNsi.Set(fN);
  fLsi.Set(fN);
  Int_t n = fN-1;
  fNsi[n] = nsi;
  fLsi[n] = lsi;
}
Double_t TSellmeier::GetDiel(Double_t wavelength) const {
  //Get the dielectric constant ( or one of the diagonal element of the
  //dielectric tensor ) at the value wavelength (nanometer) by the Sellmeier
  //law
  const Double_t un = 1.0;
  Double_t eps,a,b,c,d;
  b = wavelength*wavelength;
  eps = un + fNs0*fNs0;
  for (Int_t i=0;i<fN;i++) {
    c    = fNsi[i];
    d    = fLsi[i];
    a    = un - (d*d)/b;
    eps += (c*c)/a;
  }
  eps = TMath::Abs(eps);
  return eps;
}
Double_t TSellmeier::GetIndex(Double_t wavelength,Double_t mu) const {
  //Get the index of refraction at the value wavelength (nanometer) for a
  //magnetic permeability mu, by the Sellmeier law.
  const Double_t un = 1.0;
  Double_t n2bymu,a,b,c,d,n;
  b = wavelength*wavelength;
  n2bymu = un + fNs0*fNs0;
  for (Int_t i=0;i<fN;i++) {
    c       = fNsi[i];
    d       = fLsi[i];
    a       = un - (d*d)/b;
    n2bymu += (c*c)/a;
  }
  n = TMath::Sqrt(TMath::Abs(n2bymu*mu));
  return n;
}
