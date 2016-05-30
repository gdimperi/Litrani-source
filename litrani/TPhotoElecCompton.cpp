// @(#)root/html:$Id: TPhotoElecCompton.cpp 2008-06-11
// Author: D. Wahl <mailto:david.n.wahl@gmail.com>,F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TROOT.h"
#include "TMath.h"
#include "TString.h"
#include "TLitPhys.h"
#include "TMessErr.h"
#include "T3Vector.h"
#include "TOpticMaterial.h"
#include "TEqIndex.h"
#include "TSupplShape.h"
#include "TSNode.h"
#include "TLitGlob.h"
#include "TResults.h"
#include "TPublication.h"
#include "TPhotonCradle.h"
#include "TPhoton.h"
#include "TPhotoElecCompton.h"
#include "TSParticle.h"

ClassImp(TPhotoElecCompton)
//___________________________________________________________________________
//
//BEGIN_HTML <!--
/* -->
<div align="center">
<h3>Class description of TPhotoElecCompton</h3>
</div>
<p>The class <b><code><font size="+1">TPhotoElecCompton</font></code></b> is designed 
to simulate energy deposition by the photo-electric effect and Compton scattering. 
It has an approximate validity range of up to 1MeV. There are two approximations 
made in the implementation of the class:</p>
<ul>
<li>Coherent scattering is not included.
<li>Scattered electrons are not simulated. Care should be taken when simulating 
gammas which have sufficient energy to eject electrons which will have a non-negligible 
path length. &#x201C;Non-negligible&#x201D; will depend on the application simulated, 
as well as the materials involved and the homogeneity of the detector.
</ul>
<p>The Compton scattering algorithm is based on the paper:</p>
<div align="center">
<p><font size="+1"><b><code>F.Arqueros and G.D.Montesinos, Am. J. Phys. 71(1), 
January 2003</code></b>.</font></p>
</div>
<p>The cross section for Compton scattering is calculated analytically by <b><code><font size="+1">TPhotoElecCompton</font></code></b>. 
Notice that the calculation of the Compton cross-section requires that the values 
A, Z and rho be given for the fluorescent material. It is the only case in <b><code><font size="+1">Litrani</font></code></b> 
where this is required. The photoelectric cross-section must be provided by the user 
in the form of a fit of type <b><code><a href="http://gentit.home.cern.ch/gentit/splinefit/code/TSplineFit.html#TSplineFit:description"><font size="+1">TSplineFit</font></a></code></b> 
of category 18. The fit must provide the photo-electric cross section multiplied 
by 10<sup>24</sup> [cm<sup>2</sup>]. Cross sections can be obtained for most materials 
at:</p>
<div align="center">
<p><b><code><a href="http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html"><font size="+1">XCOM: 
Photon Cross Sections Database (NIST Standard Reference Database 8 (XGAM)</font></a></code></b> 
</p>
</div>
<p>An example photo-electric fit is provided with <b><code><font size="+1">Litrani</font></code></b> 
in the directory <b><code><font size="+1">FitMacros</font></code></b>: <b><code><font size="+1">PhotoEl_CaWO4.C</font></code></b>. 
As for the other classes generating photons, [<b><code><font size="+1"><a href="TSpontan.html">TSpontan</a>, 
<a href="TBeam.html">TBeam</a> and <a href="TElecCascade.html">TElecCascade</a></font></code></b>], 
you have to call the constructor of the class and then method <b><code><font size="+1">Gen()</font></code></b>.The 
<b><code><font size="+1">TPhotoElecCompton</font></code></b> class will automatically 
check that the materials you want to simulate energy deposition in have been correctly 
defined so as to permit calculations of cross sections. Two options are available:</p>
<ul>
<li>You wish to follow the gamma in all shapes, i.e. energy can be lost in any 
shape of the setup by the photo-electric effect or Compton scattering. To do this 
you must call <b><code><font size="+1">TPhotoElecCompton::SetFollowGamma(kTRUE)</font></code></b>.
<li>You only wish to simulate energy deposition in one shape (typically a scintillator), 
in which case it is not necessary that all shapes have rho, A, Z, and the photo-electric 
cross sections defined. The shape you wish to simulate energy deposition in must 
have these quantities defined.
</ul>
<p>There are two options for the type of source you are using: collimated (beam) 
or isotopic. All gamma sources are point-like, i.e. you must define a start position 
for the beam in the constructor. The two non-default constructors are:</p>
<ul>
<li><code><b><font size="+1">TPhotoElecCompton(Text_t* name,Text_t* title,Text_t* 
namesh,Double_t E0,T3Vector sourceposition)</font></b></code>
<li><code><b><font size="+1">TPhotoElecCompton(Text_t* name,Text_t* title,Text_t* 
namesh,Double_t E0,T3Vector sourceposition,T3Vector gammadirection)</font></b></code>
</ul>
<p>The first constructor is for isotropic sources. In the second constructor you 
must define the direction of the Gamma. Note that neither beam aperture nor extended 
gamma sources are implemented so far. At run time, <b><code><font size="+1">Litrani</font></code></b> 
will automatically check if the position and direction you have chosen (or the direction 
<b><code><font size="+1">Litrani</font></code></b> assigns to the gamma in the case 
of an isotropic source) is acceptable. There are four possible scenarios:</p>
<ol>
<li>Source collimated, <b><code><font size="+1">fFollowGamma</font></code></b> 
false: The gamma must be produced in or have an intersection with the shape namesh.
<li>Source collimated, <b><code><font size="+1">fFollowGamma</font></code></b> 
true: gamma must be inside the shape namesh
<li>Source isotropic,  <b><code><font size="+1">fFollowGamma</font></code></b> 
false: The gamma must be produced in or have an intersection with the shape namesh
<li>Source isotropic, <b><code><font size="+1">fFollowGamma</font></code></b> 
true: gamma must be inside the shape namesh
</ol>
<p>Notice that you must give the position of the gamma in the local coordinates 
of the shape namesh.</p>
<p>Pair creation is not implemented in this class, entailing a maximum validity 
range of ~1.022MeV. It is planned that in a future release, scattered electrons will 
be followed, hence extending the validity range and allowing to simulate EM cascades 
and photoelectric and Compton processes together. Examples of use of <b><code><font size="+1">TPhotoElecCompton</font></code></b> 
are provided with <b><code><font size="+1">Litrani</font></code></b> in the directory 
<b><code><font size="+1">Macros</font></code></b>:</p>
<ul>
<li><b><code><font size="+1">Gamma.C and Gamma2.C</font></code></b>
</ul>
<p>Extra Disclaimer: This class has been successfully tested for a number of scenarios, 
but has not yet been tested by multiple users and should thus be used with extra 
care. If you notice a bug please notify the author David Wahl [ <a href="mailto:dwahl@fisica.pucp.edu.pe">dwahl@fisica.pucp.edu.pe</a> 
].</p>
<p></p>
<!--*/
// -->END_HTML
// 
TPhotoElecCompton::TPhotoElecCompton(Text_t* name, Text_t* title,Text_t* namesh, Double_t E0,T3Vector sp)
: TNamed(name,title), fE0(E0), fRefShape(0), fSNode(0), fPhot(0), fSourcePosition(0,0,0), fGammaDirection(0), fCollimated(kFALSE), fRun(-1), fFollowGamma(kFALSE) {
  // A non-collimated (isotropic) source
  //
  // PARAMETERS OF THE CONSTRUCTOR :
  //
  //-  Text_t*  name   : Name of the TPhotoElecCompton.
  //-  Text_t*  title  : Title of the TPhotoElecCompton.
  //-  Text_t*  namesh : name of the shape used as reference for giving the
  //                     coordinnates of the incoming Gamma.
  //-  Double_t E0     : energy in MeV of the incoming Gamma.
  //-  T3Vector sp     : Source Position of the Gamma. IN THE LOCAL COORDINATES 
  //                     of the shape "namesh". [cm]
  //
  InitP();
  gMes->SetName("TPhotoElecCompton");
  gMes->SetTitle("TPhotoElecCompton");
  TIter next(&gLit->fSupplShape);
  gLit->VerifyOutside();  // Check globals
  if (!gGs) gGs = new TResults("GlobStat","Global statistics of all runs",0);
  if (!gIdx) {
    gIdx = new TEqIndex("TPhotoElecCompton");
    if (gMes->TestCalc()) gIdx->SetDebug();
  }

  // Get shape and check material
  fRefName = namesh;
  fRefShape = static_cast<TSupplShape*>(gLit->fSupplShape.FindObject(fRefName));
  if (!fRefShape) {
    gMes->M(kill_prog,1,"This shape has not been found",fRefName);}
  if (!fRefShape->GetOptMat()->GoodForCPE()) {
    gMes->M(kill_prog,2,"Material cannot be used for TPhotoElectricCompton",fRefName);}
  TString s = fRefShape->ClassName();
  if (s.Contains("TUBE")) {
    gMes->M(kill_prog,3,"Cannot have isotropic source for a TSTUBE");}

  if (!gLit->fShapeNumbered) fNbOfShape = gLit->GiveNbToShape();//Gentit
  else                       fNbOfShape = gLit->fNSupplShape;//Gentit
  fShEdeposit  = new Double_t [fNbOfShape];//Gentit
  for (Int_t k=0;k<fNbOfShape;k++) fShEdeposit[k] = 0.0;//Gentit
  fShEdepositR = new Double_t [fNbOfShape];//Gentit

  // Check all materials are good for CPE
  if (fFollowGamma) {
    gMes->SetName("TPhotoElecCompton ");
    gMes->SetTitle("Checking for all shapes are good for CPE");
    TSupplShape* psup;
    next.Reset();
    while ((psup = (TSupplShape *)next())) {
      if (!psup->GetOptMat()->GoodForCPE()) {
        gMes->M(Warning_M,5,"The following does not support Compton scattering and the PE effect: ");
        gMes->M(Info_M,5,psup->GetName());}
    }
  }

  // Initialise gamma direction variables
  SetSourcePosition(sp);
  fGammaDirection.Set(0,0,0);
  fFillDeath = kFALSE;
}
TPhotoElecCompton::TPhotoElecCompton(Text_t* name, Text_t* title, Text_t* namesh, Double_t E0, T3Vector sp, T3Vector gd)
: TNamed(name,title), fE0(E0), fRefShape(0), fSNode(0), fPhot(0), fSourcePosition(0,0,0), 
fGammaDirection(0), fCollimated(kTRUE), fRun(-1), fFollowGamma(kFALSE) {
  // A collimated ("beam") source
  //
  // PARAMETERS OF THE CONSTRUCTOR :
  //
  //-  Text_t*  name   : Name of the TPhotoElecCompton.
  //-  Text_t*  title  : Title of the TPhotoElecCompton.
  //-  Text_t*  namesh : name of the shape used as reference for giving 
  //                     the coordinnates of the incoming Gamma.
  //-  Double_t E0     : energy in MeV of the incoming Gamma.
  //-  T3Vector sp     : Source Position of the Gamma. IN THE LOCAL COORDINATES 
  //                     of the shape "namesh". [cm]
  //-  T3Vector gd     : The direction of the gamma. IN THE LOCAL COORDINATES 
  //                     of the shape "namesh".
  //
  InitP();
  gMes->SetName("TPhotoElecCompton");
  gMes->SetTitle("TPhotoElecCompton");
  TIter next(&gLit->fSupplShape);
  gLit->VerifyOutside(); // Check globals
  if (!gGs) gGs = new TResults("GlobStat","Global statistics of all runs",0);
  if (!gIdx) {
    gIdx = new TEqIndex("TPhotoElecCompton");
    if (gMes->TestCalc()) gIdx->SetDebug();
  }

  // Get shape and check material
  fRefName = namesh;
  fRefShape = static_cast<TSupplShape*>(gLit->fSupplShape.FindObject(fRefName));
  if (!fRefShape) {
    gMes->M(kill_prog,1,"This shape has not been found",fRefName);}
  if (!fRefShape->GetOptMat()->GoodForCPE()) {
    gMes->M(kill_prog,2,"Material cannot be used for TPhotoElectricCompton",fRefName);}
  TString s = fRefShape->ClassName();
  if (s.Contains("TUBE")) {
    gMes->M(kill_prog,3,"Cannot have isotropic source for a TSTUBE");}

  if (!gLit->fShapeNumbered) fNbOfShape = gLit->GiveNbToShape();//Gentit
  else                       fNbOfShape = gLit->fNSupplShape;//Gentit
  fShEdeposit  = new Double_t [fNbOfShape];//Gentit
  for (Int_t k=0;k<fNbOfShape;k++) fShEdeposit[k] = 0.0;//Gentit
  fShEdepositR = new Double_t [fNbOfShape];//Gentit

  // Check all materials are good for CPE
  if (fFollowGamma) {
    gMes->SetName("TPhotoElecCompton ");
    gMes->SetTitle("Checking for all shapes are good for CPE");
    TSupplShape* psup;
    next.Reset();
    while ((psup = (TSupplShape *)next())) {
      if (!psup->GetOptMat()->GoodForCPE()) {
        gMes->M(Warning_M,5,"The following does not support Compton scattering and the PE effect: ");
        gMes->M(Info_M,5,psup->GetName());}
    }
  }

  // Initialise gamma direction
  SetSourcePosition(sp);
  SetGammaDirection(gd);
  fFillDeath = kFALSE;
}
TPhotoElecCompton::~TPhotoElecCompton() {
  Clear();
}


Bool_t TPhotoElecCompton::AcceptDirection() const {
  // Decides whether a gamma of position fPosGamma and of direction fDirGamma
  //is an acceptable gamma. There are a number of cases:
  // Source collimated, fFollowGamma kFALSE: The gamma must be produced in or have an intersection with fRefShape
  // Source collimated, fFollowGamma kTRUE: gamma must be inside fRefShape
  // Source isotropic, fFollowGamma kFALSE: The gamma must be produced in or have an intersection with fRefShape
  // Source isotropic, fFollowGamma kTRUE: gamma must be inside fRefShape

  if (fFollowGamma) {
    if (!fRefShape->IsInside(fPosGamma)) {
      gMes->SetName("AcceptDirection");
      gMes->SetTitle("TPhotoElecCompton");
      gMes->M(kill_prog,1,"If you use fFollowGamma = kTRUE, the gamma has to originate inside the reference shape");
      return kFALSE;}
    return kTRUE;}

  if (!fRefShape->IsFluorescent()) {
    gMes->SetName("AcceptDirection");
    gMes->SetTitle("TPhotoElecCompton");
    gMes->M(kill_prog,2,"If you use fFollowGamma = kFALSE, the reference shape must be fluorescent");
    return kFALSE;}

  Double_t t1,t2,t3,t4;
  T3Vector a1, a2, a3 ,a4;
  if (fRefShape->IsInside(fPosGamma)) return kTRUE;
  if (fRefShape->AllCrossingPoints(fPosGamma,fDirGamma,t1,t2,t3,t4,a1,a2,a3,a4)>0) return kTRUE;

  return kFALSE;
}
void TPhotoElecCompton::Clear() {
  // Clear pointers owned by TPhotoElecCompton
  if (fPhot) {
    delete fPhot;
    fPhot = 0;
  }

  //Gentit
  if (fShEdeposit) {
    delete [] fShEdeposit;
    fShEdeposit = 0;
  }
  if (fShEdepositR) {
    delete [] fShEdepositR;
    fShEdepositR = 0;
  }
  //Gentit end
}
void TPhotoElecCompton::EmitLight(Double_t E, TSupplShape* shape, EDepProcess process, T3Vector& dirElec) {
  //  Emit the corrresponding number of photons for a deposited energy of
  //  E Mev at the point fPosGamma in the shape.

  Int_t kfluo,bin;
  Axis_t afluo;
  T3Vector x0,K,Elec;
  Double_t Wvlgth, t0; // Wavelength and start time of photon (in ps)
  Int_t nphot = shape->GetOptMat()->GetNPhot(E);

  gMes->SetName("EmitLight");
  gMes->SetTitle("TPhotoElecCompton");

  if (fFollowElectron && (process==kComptonScattering)) {
    Double_t step = fStep;
    Double_t x;
    Double_t deltaE;   //energy deposited in step [Mev]
    Double_t dEdx;     //dEdx for this particular energy
    Bool_t stop = kFALSE;
    Double_t photMeV = shape->GetOptMat()->GetPhotMev();
    TSParticle* electron = new TSParticle("e-", "e-", singlevalued, E, E,
      0.0, 0.0, 0.0, 0.000511);
    T3Vector posElec = fPosGamma;
    T3Vector posLight = fPosGamma;

    while (!stop) {
      // Check steplength
      dEdx = GetElectrondEdx(electron->GetEnergy());
      if (dEdx*step<E) {
        stop = kTRUE;
        step = E/dEdx;}
      deltaE = step*dEdx;

      // Generate light
      if (shape->GetOptMat()->IsFluorescent()) {
        Double_t photStep = deltaE*photMeV;
        nphot = (Int_t)photStep;
        if ((photStep-nphot)>gRandom3->Rndm()) ++nphot;
        if (!(nphot>1)) {
          gMes->M(Warning_M,1,"Your step length produces no photons");
        }

        for (Int_t i=0; i<nphot; ++i) {
          gGs->fNpGener += 1;
          if (gCs) gCs->fNpGener += 1;

          //Determine where to generate the photon
          x = step*gRandom3->Rndm();
          Double_t timePhot = fTime + x*step/electron->GenP();
          posLight = posElec + x*step*dirElec;

          shape->GetOptMat()->WaveAndLife(Wvlgth,t0,kfluo);
          timePhot += t0;

          gLit->NewWavelength(Wvlgth);
          bin = gGs->fHTimeAll->Fill(timePhot);
          bin = gGs->fHWvlgthAll->Fill(Wvlgth);
          if (gCs) {
            bin = gCs->fHTimeAll->Fill(timePhot);
            bin = gCs->fHWvlgthAll->Fill(Wvlgth);
          }
          if (gGs->HasFluo()) {
            afluo = kfluo;
            bin = gGs->fHTimeEach->Fill(timePhot,afluo);
            bin = gGs->fHWvlgthEach->Fill(Wvlgth,afluo);
          }  //if (gGs->HasFluo())
          if ((gCs) && (gCs->HasFluo())) {
            afluo = kfluo;
            bin = gCs->fHTimeEach->Fill(timePhot,afluo);
            bin = gCs->fHWvlgthEach->Fill(Wvlgth,afluo);
          }  //if (gCs)

          //generates the photon
          TPhotonCradle* Beam = new TPhotonCradle(shape,posLight,kTRUE);
          Beam->Generate(x0,K,Elec);
          fPhot = new TPhoton(fRun,fNphot+i,x0,shape,Wvlgth,timePhot,K,Elec,0);

          //follow the photon from its birth to its death
          fPhot->Move(-1);
          if (gGs->fNpAbnorm>TLitPhys::Get()->Anomalies()) {
            gMes->M(kill_prog,2,"too many anomalies");
          }
          if (fFillDeath) FillDeath();
          delete fPhot; fPhot = 0;
          delete Beam;
        }//End of loop over nphotons
      }//End of IfFluorescent

      E -= deltaE;
      if (stop) cout << "Energy left should be 0: " << E << endl;
      posElec += step*dirElec;
      fTime += step/electron->GenP();
      fNphot += nphot;

      if (!shape->IsInside(posElec)) {
        shape = 0;
        TSupplShape* psup;
        TIter next(&gLit->fSupplShape);
        while ((psup = (TSupplShape *)next())) {
          if (psup->IsInside(fPosGamma)) shape = psup;}
      }
      if (!shape) stop = kTRUE;

    } //End of loop over steps     
  } // End of if fFollowElectron

  else {
    Int_t kfluo,bin;
    Axis_t afluo;
    T3Vector x0,K,Elec;
    Double_t Wvlgth, t0; // Wavelength and start time of photon (in ps)
    Int_t nphot = shape->GetOptMat()->GetNPhot(E);
    //
    // Loop on generation of photons
    //
    for (Int_t i=0; i<nphot;++i) {
      gGs->fNpGener += 1;
      if (gCs) gCs->fNpGener += 1;
      //first determines wavelength and time of emission of photon and store results
      //in histograms
      shape->GetOptMat()->WaveAndLife(Wvlgth,t0,kfluo);
      t0 += fTime;
      //update all values depending upon wavelength
      gLit->NewWavelength(Wvlgth);
      bin = gGs->fHTimeAll->Fill(t0);
      bin = gGs->fHWvlgthAll->Fill(Wvlgth);
      if (gCs) {
        bin = gCs->fHTimeAll->Fill(t0);
        bin = gCs->fHWvlgthAll->Fill(Wvlgth);
      }
      if (gGs->HasFluo()) {
        afluo = kfluo;
        bin = gGs->fHTimeEach->Fill(t0,afluo);
        bin = gGs->fHWvlgthEach->Fill(Wvlgth,afluo);
      }  //if (gGs->HasFluo())
      if ((gCs) && (gCs->HasFluo())) {
        afluo = kfluo;
        bin = gCs->fHTimeEach->Fill(t0,afluo);
        bin = gCs->fHWvlgthEach->Fill(Wvlgth,afluo);
      }  //if (gCs)
      //generates the photon
      TPhotonCradle* Beam = new TPhotonCradle(shape,fPosGamma,kTRUE);//Gentit
      Beam->Generate(x0,K,Elec);
      fPhot = new TPhoton(fRun,fNphot+i,x0, shape ,Wvlgth,t0,K,Elec,0);
      //follow the photon from its birth to its death
      fPhot->Move(-1);
      if (gGs->fNpAbnorm>TLitPhys::Get()->Anomalies()) {
        gMes->M(kill_prog,2,"too many anomalies");
      }
      if (fFillDeath) FillDeath();
      delete fPhot; fPhot = 0;
      delete Beam;
    }//end for loop on photons
    fNphot += nphot;
  }
}

void TPhotoElecCompton::FillDeath() const {
  // This method is called when a photon dies, in order to record the
  // coordinates of the death point, for the cases where the photon is seen.
  Int_t bin;
  if ((fFillDeath) && (gLit->fSeen)) {
    bin = fHX0->Fill(gLit->fX0);
    bin = fHY0->Fill(gLit->fY0);
    bin = fHZ0->Fill(gLit->fZ0);
  }
}


T3Vector TPhotoElecCompton::DepositEnergy(TSupplShape* shape, Double_t MeV) {
  //BEGIN_HTML <!--
  /* -->
  </pre>
  <div align="center">
  <h3>FindNextStep</h3>
  </div>
  <p>Finds the next point at which light is to be generated. Returns whether or not 
  energy is to be deposited in the step.</p>
  <div align="center">
  <table border="2" cellpadding="4" cellspacing="4" bgcolor="#f5f5f5" width="700">
  <caption>
  <p>Let us define:</p>
  </caption>
  <tr>
  <td><b>A</b></td>
  <td><b>Atomic weight of material</b></td>
  </tr>
  <tr>
  <td><b>N</b></td>
  <td><b>Avogadro number == Number of molecules in A grams of material == 6.025e+23</b></td>
  </tr>
  <tr>
  <td><font face="Symbol"><b>r</b></font></td>
  <td><b>Density of material in grams*cm<sup>-3</sup>.</b></td>
  </tr>
  <tr>
  <td><font face="Symbol"><b>s</b></font></td>
  <td><b>cross-section in cm<sup>2</sup>.</b></td>
  </tr>
  <tr>
  <td><b><font face="Symbol">D</font>s</b></td>
  <td><b>infinitesimal step</b></td>
  </tr>
  <tr>
  <td><b>s</b></td>
  <td><b>step</b></td>
  </tr>
  <tr>
  <td><b>n</b></td>
  <td><b>= s / <font face="Symbol">D</font>s</b></td>
  </tr>
  <tr>
  <td><b><font face="Symbol">a</font></b></td>
  <td><b>= ( </b><font face="Symbol"><b>s</b></font><b> * N * </b><font face="Symbol"><b>r</b></font><b> 
  ) / A</b></td>
  </tr>
  <tr>
  <td><b><font face="Symbol">D</font>p</b></td>
  <td><b>probability of interaction on an infinitesimal step <font face="Symbol">D</font>s</b></td>
  </tr>
  <tr>
  <td><b>p(s)<font face="Symbol">D</font>s </b></td>
  <td><b>probability of interaction after a step s</b></td>
  </tr>
  </table>
  </div>
  <p></p>
  <div align="center">
  <table border="2" cellpadding="4" cellspacing="4" bgcolor="#f5f5f5" width="700">
  <caption>
  <p>We have then</p>
  </caption>
  <tr>
  <td><b><font face="Symbol">D</font>p</b></td>
  <td><b>=</b></td>
  <td><b>(( <font face="Symbol">s</font> * N * <font face="Symbol">r</font> ) 
  / A)*<font face="Symbol">D</font>s</b></td>
  <td><b>=</b></td>
  <td><b><font face="Symbol">a D</font>s<font face="Symbol"> </font></b></td>
  </tr>
  <tr>
  <td colspan="5">
  <div align="center">
  <b>The probability p of interaction after a step s is the probability of non-interaction 
  for n steps <font face="Symbol">D</font>s, </b><b>multiplied by the probability of 
  interaction in the last step <font face="Symbol">D</font>s.</b></div>
  </td>
  </tr>
  <tr>
  <td><b>p(s)<font face="Symbol">D</font>s </b></td>
  <td><b>=</b></td>
  <td><b>( 1 -  <font face="Symbol">a D</font>s<font face="Symbol"> </font>)<sup>n</sup> 
  * <font face="Symbol">a D</font>s<font face="Symbol"> </font></b></td>
  <td><b>= </b></td>
  <td><b>( 1 - ((<font face="Symbol">a </font>s) / n)<font face="Symbol"> </font>)<sup>n</sup> 
  * <font face="Symbol">a D</font>s<font face="Symbol"> </font></b></td>
  </tr>
  <tr>
  <td><b>p(s)<font face="Symbol">D</font>s </b></td>
  <td><b>= </b></td>
  <td colspan="2"><b>exp (-<font face="Symbol">a</font>s) <font face="Symbol">a 
  D</font>s<font face="Symbol"> </font></b></td>
  <td><b>(1)</b></td>
  </tr>
  <tr>
  <td colspan="5">
  <div align="center">
  <b>If we want to generate random numbers according to distribution (1), and 
  have at hand a generator which generates flat distribution of x between 0 and 1, 
  we must have:</b></div>
  </td>
  </tr>
  <tr>
  <td><b>exp (-<font face="Symbol">a</font>s) <font face="Symbol">a D</font>s<font face="Symbol"> 
  </font></b></td>
  <td><b>= </b></td>
  <td colspan="3"><b>dx</b></td>
  </tr>
  <tr>
  <td><b>dx / ds</b></td>
  <td><b>= </b></td>
  <td colspan="3"><b><font face="Symbol">a </font>exp (-<font face="Symbol">a</font>s)</b></td>
  </tr>
  <tr>
  <td><b>x</b></td>
  <td><b>=</b></td>
  <td colspan="3"><b>- exp (-<font face="Symbol">a</font>s) +&nbsp;K</b></td>
  </tr>
  <tr>
  <td colspan="5">
  <div align="center">
  <b>Requiring x=0 for s=0 and x=1 for s = infinity, we get  K = 1</b></div>
  </td>
  </tr>
  <tr>
  <td><b>x</b></td>
  <td><b>= </b></td>
  <td colspan="3"><b> 1 - exp (-<font face="Symbol">a</font>s)</b></td>
  </tr>
  <tr>
  <td><b>1 - x</b></td>
  <td><b>= </b></td>
  <td colspan="3"><b>exp (-<font face="Symbol">a</font>s)</b></td>
  </tr>
  <tr>
  <td><b>-<font face="Symbol">a</font>s</b></td>
  <td><b>= </b></td>
  <td colspan="3"><b>ln (1-x)</b></td>
  </tr>
  <tr>
  <td colspan="5"></td>
  </tr>
  <tr>
  <td><b><font color="red">s</font></b></td>
  <td><b><font color="red">= </font></b></td>
  <td colspan="3"><b><font color="red">- ln(1-x) / </font><font face="Symbol" color="red">a</font></b></td>
  </tr>
  </table>
  </div>
  <p></p>
  <pre>
  <!--*/
  // -->END_HTML
  //  Notice the precompiler variable OLDMETHOD which has to be false in the normal
  //working of this method. If you put it to true, the calculations of the direction
  //of the gamma after Compton effect are done twice, the first one with the normal
  //method not using trigonometric functions, the second one with the method with
  //trigonometric functions, and the 2 results are compared.
  // The normal method constructs a provisional coord. system where Oz is || to
  //fDirGamma and Oy is horizontal. This is OEDA (ohne einschränkung der allgemeinheit!)
  //We replace phi by 2pi-phi in the second method, which is also OEDA. This is in
  //order that formula (11a-c) of de paper of F. Arqueros be compatible with the
  //choice of the axis in the first method. This is OEDA because generating phi or
  //2Pi-phi is equivalent.
  //

  const Double_t zero       = 0.0;
  const Double_t eps        = 1.0e-12;
  const Double_t theta_min  = 0.0;
  const Double_t theta_max  = TMath::Pi();
  const Double_t Fmin       = 0.0;

  if (!shape) cout << "An error which should not happen" << endl;
  //cout << "-------- Testing shape: " << shape->GetName() << endl;

  // Find coordinates of interaction and check if in shape
  Double_t pathlength = GetPathlength(MeV,shape->GetOptMat());
  T3Vector NextPos = fPosGamma+pathlength*fDirGamma;
  fTime += pathlength/(TLitPhys::Get()->C());
  if (!shape->IsInside(NextPos)) {
    return NextPos;}
  fPosGamma = NextPos;

  //cout << "-------- Depositing Energy: " << endl;

  Double_t Phi,sPhi,cPhi;
  Double_t Fmax;
  Double_t Theta,sTheta,cTheta;
  Double_t F;
  Double_t m1,m2,m3;
  Double_t n1,n2;
  Double_t p1, p2, p3,p12;
  Double_t q1,q2,q3;
  Double_t Theta1,sTheta1,cTheta1;
  Double_t Phi1,sPhi1,cPhi1;
  Double_t sKN = GetXSectnKN(MeV, shape->GetOptMat());
  Double_t sPE = GetXSectnPE(MeV, shape->GetOptMat());
  Double_t EDepositStep = 0;
  EDepProcess process;
  T3Vector eDir(0,0,0);

  //Compton Scattering or PE?
  if (gRandom3->Rndm() < (sPE/(sPE + sKN))) { // Photo-Electric
    process = kPhotoElectric;
    fTotEdeposit += MeV;
    fShEdeposit[shape->fShapeNumber]  += MeV;//Gentit
    fShEdepositR[shape->fShapeNumber] += MeV;//Gentit
    EDepositStep = MeV;
    fGammaDead    = kTRUE;
    fCurrentMeV   = zero;}
  else { // Compton
    process = kComptonScattering;
    Phi  = 2*TMath::Pi()*gRandom3->Rndm();
    sPhi = TMath::Sin(Phi);
    cPhi = TMath::Cos(Phi);
    Fmax = Get_dSigma_dOmega(MeV, theta_min);
    //Random variables for acceptance-rejection method
    do {
      Theta = theta_min +  gRandom3->Rndm()*(theta_max - theta_min);
      sTheta = TMath::Sin(Theta);
      cTheta = TMath::Cos(Theta);
      F = Fmin + gRandom3->Rndm()*(Fmax - Fmin);
    } while (F > Get_dSigma_dOmega(MeV,Theta)*sTheta);
    // Calculate next direction of the gamma
    // This other method avoid the calculations of Sin(), Cos(), ACos() ...
    // It constructs a provisional coord. system where Oz is || to fDirGamma,
    // and Oy is horizontal.
    fDirGamma.Get(p1, p2, p3);
    eDir = MeV*fDirGamma;
    p12 = TMath::Sqrt(p1*p1 + p2*p2);
    if (p12<eps) {
      Theta1  = Theta;
      Phi1    = Phi;
      sTheta1 = sTheta;
      cTheta1 = cTheta;
      sPhi1   = sPhi;
      cPhi1   = cPhi;
      q1      = sTheta1*cPhi1;
      q2      = sTheta1*sPhi1;
      q3      = cTheta1;
    }
    else {
      n1 =  p2/p12;        // == +sPhi0
      n2 = -p1/p12;        // == -cPhi0
      m1 =  n2*p3;         // == -cTheta0*cPhi0
      m2 = -n1*p3;         // == -cTheta0*sPhi0
      m3 =  n1*p2 - n2*p1; // ==  sTheta0
      q1 =  sTheta*cPhi*m1 + sTheta*sPhi*n1 + cTheta*p1;
      q2 =  sTheta*cPhi*m2 + sTheta*sPhi*n2 + cTheta*p2;
      q3 =  sTheta*cPhi*m3                  + cTheta*p3;

    }
    fDirGamma.Set(q1,q2,q3);
    fDirGamma.Unit();  // to cope with numerical errors

    EDepositStep = MeV - MeV/(1+(MeV/TLitPhys::Get()->Me())*(1-cTheta));
    fCurrentMeV = MeV - EDepositStep;
    fTotEdeposit += EDepositStep;
    fShEdeposit[shape->fShapeNumber]  += MeV;//Gentit
    fShEdepositR[shape->fShapeNumber] += MeV;//Gentit
    eDir = eDir - fCurrentMeV*fDirGamma;
    eDir.Unit();
    fCompton++;
  }

  Int_t bin;
  Int_t k = shape->fShapeNumber;//Gentit
  if (gGs) bin = gGs->fHEnergyInShape->Fill((Double_t)k,fShEdepositR[k]);//Gentit
  if (gCs) bin = gCs->fHEnergyInShape->Fill((Double_t)k,fShEdepositR[k]);//Gentit

  if (shape->GetOptMat()->IsFluorescent()) EmitLight(EDepositStep, shape, 
    process, eDir);

  return fPosGamma;
}

void TPhotoElecCompton::Gen(Int_t run, Int_t nGamma, Double_t xparam, Bool_t runstat,
  Bool_t ForgetLast) {
  // Starts a run generating Ngamma gammas. These gammas will generate photons
  //by Photo-electric effect or by Compton effect in all fluorescent shapes.
  //For all non fluorescent shapes nothing happens.
  //
  //  PARAMETERS :
  //
  //  run        : run number. Arbitrary, but has to be greater than 0.
  //  nGamma     : number of gammas to produce
  //  xparam     : this parameter is used as abscissa in the plotting of the
  //               results by the class TPublication. 
  //
  //  runstat and ForgetLast :
  //
  //    if runstat == true [Default] AND ForgetLast == false [Default] :
  //
  //      in memory   : this run statistics keeped but this run histos deleted
  //                     when next run begins
  //      on the file : this run statistics and histos recorded
  //
  //            It is the only configuration which allows the working of
  //          TPublication, i.e. allowing to show histograms of quantities
  //          varying as a function of a run dependent parameter.
  //          Usage : normal case : try it first.
  //
  //    if runstat == true AND ForgetLast == true :
  //
  //      in memory   : this run statistics and histograms deleted when next
  //                     run begins.
  //      on the file : this run statistics and histos recorded
  //
  //          Disadvantage : TPublication not working, gGp pointer unavailable
  //          Advantage    : no increase of memory with runs
  //                         per run histograms still available on file
  //          Usage : use this configuration when your number of runs is big
  //                  but you still want per run statistics and histograms
  //                  on file.
  //
  //    if runstat == false ( ForgetLast true or false ) :
  //
  //      in memory   : no statistics and histograms for this run
  //      on the file : no statistics and histograms for this run
  //
  //          Disadvantage : TPublication not working, gGp pointer unavailable
  //                         per run statistics and histograms not available
  //          Advantage    : no increase of memory with runs
  //                         a bit faster, half less histograms to fill
  //          Usage : use this configuration when your number of runs is very
  //                  big and you are not interested in the per run statistics
  //                  and histograms.
  //
  //  Notice that in any case, the global statistics and histograms for all
  //  runs is always present in memory and on the file. This global statistics
  //  is an object of class TResults, pointed to by the pointer gGs.
  //
  Bool_t gGpok;
  Int_t iGamma;
  TSupplShape* shape;
  T3Vector pos = 0;

  gGs->RefreshDet();
  if (run<=0) {
    gMes->M(Error_M,1,"run number must be >= 1. We change it !");
    run = TMath::Abs(run);
    if (run==0) run += 1;
  }
  fRun = run;
  cout << "Run : " << fRun << endl;
  *gMes->fListing << "Run : " << fRun << endl;
  OutsideOk();  // Ensures that gOutside is defined
  NameFile();
  //initializes the summary statistics of class TPublication
  gGpok = (runstat && (!ForgetLast));
  if (!gGp && gGpok) gGp = new TPublication("Summary","Runs",gGpok);
  //Verifies that shapes are numbered
  Int_t nbofsh;
  if (!gLit->fShapeNumbered)
    nbofsh = gLit->GiveNbToShape();
  //Handle the pointer to the previous run. Even if gCs is put to 0 without
  //having been deleted, pointer to the previous run is not lost, it stays
  //inside gLit->fResults.

  if (gCs) {
    if (ForgetLast) {
      gLit->fResults.Remove(gCs);
      delete gCs;
      gCs = 0;
    }
    else gCs->DelHistos();
  }
  gCs = 0;
  //Makes gCs point towards this run.
  if (runstat) gCs = new TResults(fFullName,fTitle,fRun,xparam);
  //Open the .root file for writing, if not yet done
  if (gLit->fFilesClosed) {
    gLit->OpenFilesW(runstat);
    gROOT->cd();
    if ((!runstat) && (gCs)) {
      delete gCs;
      gCs = 0;
    }
  }
  // Loop on generation of gammas
  for (iGamma=0;iGamma<nGamma;iGamma++) {
    //emission of light in all shapes
    InitTV();
    InitAxis();
    shape = fRefShape;

    while (shape) {
      pos = DepositEnergy(shape, fCurrentMeV);
      if (fGammaDead) shape = 0;
      if (shape && !shape->IsInside(pos)) shape = GotoNextShape(shape);
    }
  }

  //Put statistics of this run into the tree.
  if (gCs) {
    gCs->Conclusion();
    gLit->fLitFile->cd();
    gLit->fLitNb += gLit->fLitTree->Fill();
    gROOT->cd();
  }
}
Double_t TPhotoElecCompton::Get_dSigma_dOmega(Double_t MeV, Double_t theta) const {
  // Calculates the differenial cross section for a Compton Event
  //Differential Compton X Section for angular distribution
  //
  const Double_t un = 1.0;
  Double_t sig,ctheta,b,g,d1,d2,n2,F1,F2;
  ctheta = TMath::Cos(theta);
  g      = MeV/TLitPhys::Get()->Me();
  b      = un - ctheta;
  n2     = g*b;
  d2     = un + n2;
  d1     = d2*d2;
  F1     = un/d1;
  n2    *= n2;
  F2     = un + ctheta*ctheta + n2/d2;
  sig    = 2*TMath::Pi()*F1*F2;
  return sig;
}

Double_t TPhotoElecCompton::GetXSectnKN(Double_t MeV, TOpticMaterial* mat) const {
  // Returns the integrated Klein-Nishina cross section for incoherent
  //scattering [=Compton scattering].
  // The relevance of this formula breaks down at around .1 MeV as Thompson
  //scattering becomes more important. Sigma is * 1.0E+24
  Double_t a;
  if (MeV < 0.1) {return 0.0;}
  Double_t twopRe2 = TMath::TwoPi()*TLitPhys::Get()->R02();
  Double_t g = MeV/TLitPhys::Get()->Me();
  Double_t sig1 = (1+g)/(g*g);
  Double_t sig2 = (2*(1+g)/(1+(2*g)));
  Double_t sig3 = TMath::Log(1 + (2*g))/g;
  Double_t sig4 = 0.5*sig3;
  a             = 1+2*g;
  Double_t sig5 = (1+3*g)/(a*a);
  Double_t sig  = twopRe2*(sig1*(sig2 - sig3) + sig4 - sig5);
  return (mat->GetZ()*sig);
}


Double_t TPhotoElecCompton::GetXSectnPE(Double_t MeV, TOpticMaterial* mat) const {
  // Fetches the PhotoElectric Cross Section from TOpticMaterial
  return mat->GetXSectnPE(MeV);
}


TSupplShape* TPhotoElecCompton::GotoNextShape(TSupplShape* curShape) {
  // Starting from the current position fPosGamma and with the current
  //direction fDirGamma, finds the next point of interaction of the gamma
  // We have already checked to see that the photon isnt absorbed in the
  // current shape in DepositEnergy
  const Double_t eps8       = 1.0e-8;

  if (fGammaDead) return 0;
  if (!curShape) cout << "You must define a shape" << endl;

  T3Vector nextpos;
  Double_t dist;
  curShape->NextFace(0,fPosGamma,fDirGamma,nextpos,dist,1.0e-7);
  fPosGamma = nextpos + eps8*fDirGamma; //just to exit the shape!

  // Find next shape and check if supports Compton - PhotoElectric (CPE)
  TSupplShape* nextShape = 0;
  TSupplShape* psup;
  TIter next(&gLit->fSupplShape);
  while ((psup = (TSupplShape *)next())) {
    if (psup->IsInside(fPosGamma)) nextShape = psup;}
  if (!nextShape) return 0;

  if (!fFollowGamma && !nextShape->GetOptMat()->IsFluorescent()) {
    fGammaDead = kTRUE;
    return 0;}

  if (!nextShape->GetOptMat()->GoodForCPE()) {
    fGammaDead = kTRUE;
    return 0;}

  return nextShape;
}

void TPhotoElecCompton::InitAxis() {
  // Initialise the entry point and direction for the gamma
  // There are two possible cases, collimated or isotropic
  // In case of collimated, we do not accept a direction for the
  //gamma not hitting any fluorescent shape.
  const Int_t TooMuch = 100000;
  Bool_t accept = kFALSE;
  Int_t N       = 0;
  fPosGamma = fSourcePosition;
  // A source with isotropic generation  
  if (!fCollimated) {
    Double_t theta,phi,stheta,ctheta,sphi,cphi;
    while ((!accept) && (N<TooMuch)) {
      theta  = TMath::ACos(gRandom3->Rndm());
      stheta = TMath::Sin(theta);
      ctheta = TMath::Cos(theta);
      phi    = 2*TMath::Pi()*gRandom3->Rndm();
      sphi   = TMath::Sin(phi);
      cphi   = TMath::Cos(phi);
      fDirGamma.Set(stheta*cphi,stheta*sphi,ctheta);
      accept = AcceptDirection();
      N++;
    }
  }
  // A collimated source, i.e. a gamma "beam"
  else {
    fDirGamma = fGammaDirection;
    accept = AcceptDirection();
    if (!accept) {
      gMes->SetName("TPhotoElecCompton");
      gMes->SetTitle("InitAxis");
      gMes->M(kill_prog,2,"Your beam does not point to any fluorescent shape");}
  }
}
void TPhotoElecCompton::InitP() {
  // Pointers to 0
  fRefShape    = 0;
  fSNode       = 0;
  fPhot        = 0;
  fHX0         = 0;
  fHY0         = 0;
  fHZ0         = 0;
  fShEdeposit  = 0;//Gentit
  fShEdepositR = 0;//Gentit
}
void TPhotoElecCompton::InitTV() {
  // Initialise tracking variables
  const Double_t zero = 0.0;
  fNphot         = 0;
  fCurrentMeV    = fE0;
  fTotEdeposit   = zero;
  fCompton       = 0;
  fGammaDead     = kFALSE;
  fIsNextShape   = kTRUE;
  for (Int_t k=0;k<fNbOfShape;k++) fShEdepositR[k] = zero;//Gentit
  fTime = 0;
}


void TPhotoElecCompton::NameFile() {
  //Give a name to the statistics and histogram file for one run.  The
  //name of the statistics and histograms file for a run is fName ( the
  //name you have given in the constructor of TPhotoElecCompton)
  //appended with '_' followed by the run number and '.root'. For
  //instance, if you have given the name "gamma1MeV" to this instance of
  //TPhotoElecCompton, and you call Gen() with run number 100, the name
  //of the file will be "gamma1Mev_100.root".
  const Int_t kBufferSize = 64;
  Int_t l,l1;
  TString NameOnly;
  NameOnly = fName;
  l1 = kBufferSize - 10;
  l = NameOnly.Length();
  if (l>l1) NameOnly.Remove(l1);
  fFullName  = NameOnly;
  fFullName.Append('_');
  fFullName += fRun;
}
void TPhotoElecCompton::OutsideOk() {
  //Ensures that gOutside is defined
  if (!gOutside) {
    Bool_t b;
    gOutside = new TOpticMaterial("Outside","Material of the outside world",
      kFALSE,1.0,1.0e+6);
    gOutside->IsIsotropic(1.0);
    b = gOutside->CheckIt();
  }
}
void TPhotoElecCompton::SetFillDeath(TH1F *h1,TH1F *h2,TH1F *h3) {
  //  To be called once in order that the coordinates of the photons seen
  //be recorded into the provided histograms : 
  // - h1 for the x World Coordinate
  // - h2 for the y World Coordinate
  // - h3 for the z World Coordinate
  //  The creation and deletion of the histograms h1/2/3 has to be handled
  //by the user inside his CINT code. h1/2/3 must have been booked before
  //calling SetFillDeath
  //
  fFillDeath = kTRUE;
  fHX0 = h1;
  fHY0 = h2;
  fHZ0 = h3;
}
Double_t TPhotoElecCompton::GetPathlength(Double_t MeV, TOpticMaterial* mat) {
  const Double_t un = 1.0;
  const Double_t barn = 10.0; //barn conversion: 10E23 from avogadro and 10E24 for cross-section
  Double_t matN = (mat->GetDensity()*TLitPhys::Get()->AvoDivE26())/mat->GetA();
  Double_t sKN  = GetXSectnKN(MeV, mat); //x10E+24 cm2
  Double_t sPE  = GetXSectnPE(MeV, mat); //x10E+24 cm2
  Double_t x = gRandom3->Rndm();
  return -barn*TMath::Log(un - x)/(matN*(sKN+ sPE));
}
void TPhotoElecCompton::SetGammaDirection(T3Vector gd) {
  // Takes a new starting direction for the gamma
  gd.Unit();
  fRefShape->GetNode()->AxisToWorld(gd,fGammaDirection);
  fGammaDirection.Unit();
}
void TPhotoElecCompton::SetSourcePosition(T3Vector sp) {
  // Takes a new position for the starting point of the gamma
  fRefShape->GetNode()->LocalToWorld(sp,fSourcePosition);
}
void TPhotoElecCompton::SetFollowElectron(Bool_t b, Double_t step) {
  // Check following electrons is supported
  // Note we only follow electrons for the case of Compton Scattering!
  // Beware!! Not yet implemented!! Do not use
  TSupplShape* psup;
  TIter next(&gLit->fSupplShape);

  while ((psup = (TSupplShape *)next())) {
    if (!psup->GetOptMat()->GetCanStopElectron()) {
      gMes->M(kill_prog,1,"The following does not support Compton scattering with following of electrons ",psup->GetName());}
  }
  fFollowElectron = kTRUE;
  fStep = step;
}

Double_t TPhotoElecCompton::GetElectrondEdx(Double_t E) const {
  // Beware!! Not yet implemented!! Do not use
  return 1.0;
}
