// @(#)root/html:$Id: TRevetment.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMessErr.h"
#include "TLitGlob.h"
#include "TRevetment.h"

ClassImp(TRevetment)
//______________________________________________________________________________
//
//  TRevetment  defines a revetment, with its optical properties.
//
//  The class TRevetment introduces a revetment with which a face of a
//geometrical shape may be covered. The arguments of the constructor are the
//following : (Notice that when you have the choice between 2 types,
//const char* or Double_t, if you choose the type const char*, then you have to give
//the name of the fit or interpolation ( of class TSplineFit ) which gives the
//dependency of this argument with respect to wavelength. This fit or inter-
//polation must have been defined before. If you choose the type Double_t,
//you have to give the fixed value of the argument, which in that case will
//not depend upon wavelength.)
//
//   1  : const char*   name       : name of the revetment
//
//   2  : const char*   title      : title of the revetment
//
//   3  : const char*   nameMat    : name of the optical material building the
//                               slice between the face of the geometrical
//           shape and the revetment, if there is such a slice, which is the
//           standard case: when you wrap a geometrical shape for instance
//           with a foil of aluminium, you have a slice of air between your
//           shape and your aluminium foil. If there is not such a slice
//          (painting), then enter "none".
//
//   4  : const char*   nameDiffus : proportion [0..1] of photons which are
//     or Double_t  diffus       diffused instead of reflected. All non-
//                               absorbed light which is not diffused  is
//           assumed to be reflected. This proportion of light diffused may
//           itself be dependent upon the wavelength : in that case,
//           nameDiffus is the name of the fit or interpolation ( of type
//           TSplineFit ) which gives this dependency. Otherwise, diffus is
//           the fixed value of this proportion.
//
//   5  : const char*   nameRindex : real part of the index of refraction of the
//     or Double_t  rindex       revetment. Here also, may be dependentor not
//                               upon wavelength, at will. It is the real
//           part of the index of refraction which allows absorption of the
//           wave. The bigger this real part, the greater the absorption.
//
//   6  : const char*   nameIindex : imaginary part of the index of refraction
//     or Double_t  iindex       of the revetment.Notice that an imaginary
//                               part of an index of refraction must always
//           be negative to have absorption ( and not explosion !) of the
//           wave. But, as the "Handbook of optical constants" gives it
//           positive, you are requested to give it also positive, Litrani
//           will change its sign.Here also, may be dependent or not upon
//           wavelength, at will.
//
//   7  : const char*   nameMu     : magnetic permeability mu of the revetment.
//     or Double_t  Mu           Here also, may be dependent or not upon 
//                               wavelength, at will. If you do not give
//                               this parameter, the magnetic permeability
//           mu will be set equal to 1 and constant. As the "Handbook of
//           optical constants of material" never quotes the magnetic
//           permeability of a revetment, I suppose that it is considered as
//           always equal to 1. But for the sake of completness, it is left
//           free by litrani. After all, the formula for reflection and
//           refraction do depend upon mu !
//
//   8  : Double_t  supplAbs   : Possibility of a supplementary absorption.
//                               Default to 0.0, normal value, because the
//           absorption by the revetment is calculated from the Fresnel
//           formula with the complex index of refraction ( given by rindex
//           and iindex ). If you give it a non-zero value, say 0.1, you
//           will add by that a supplementary probability (10%) of absorption
//         . It may be an handy way of studiing the effect of a revetment
//           getting worse and worse. This parameter is also useful if you
//           want to define a revetment which is 100% absorbing. In that
//           case, put it to 1.0. You will spare by that the calculation of
//           the Fresnel equations to the program. Notice that the photons
//           which are not absorbed by Fresnel but are absorbed because of
//           this supplementary, artificially added, absorption are said
//           "absorbed before revetment" by Litrani.
//
//   9  : Double_t ThetaMax    : maximum theta angle for the diffused photon.
//                               Default 90 degree. By putting a value
//           smaller than 90 degree, you avoid that diffused photons recoil
//           almost parallel with the diffusing surface. Look at the next
//           paragraph and also at the 3rd tricky point quoted below to
//           learn why it may be necessary to limit the angle of recoil of
//           the diffused photon to values less than 90 degree.
//
//
//    HOW DIFFUSION IS GENERATED
//
//  In Litrani, diffusion is treated exactly in the same way as reflection,
//except that the reflection is done on a plane of "any" orientation. More
//precisely, the orientation of the reflecting plane is chosen in such a way
//that the reflected photon has a distribution of reemission which is :
//
//       cos(t)sin(t) dt df     [ 0 <= t <= ThetaMax ]
//
//There is a supplementary factor cos(t) with respect to a totally isotropic
//distribution sin(t) dt df in order to slightly favour the direction normal
//to the diffusing surface. By default ThetaMax is 90 degree. You cannot
//choose it bigger than 90 degree, it would be meaningless. You can choose
//it smaller if you want to avoid that the diffused photon be almost tangent
//to the plane of the diffusing revetment. There is a good reason to choose
//it smaller than 90 degree in case of a diffusing revetment covering, wi-
//thout any slice of intermediate material, a face of a shape made of an
//anisotropic material : see tricky point c).
//  Once one knows what the direction of the diffused photon will be, one
//calculates the orientation of a pseudo plane of reflection in such a way
//that the photon REFLECTED on this pseudo plane of reflection has the
//wanted direction for the DIFFUSED photon. One does that assuming that the
//angle of reflection is equal to the incident angle, which is true only for
//isotropic material. This is the origin of the 3rd tricky point, see below.
//
//
//    TRICKY POINTS ABOUT REVETMENT
//
//  a) - Totally absorbing revetment
//
//   Suppose you want that a revetment absorbs a photon each time the
//photon reaches a face of the covered material. For instance, when a photon
//is inside the Si part of an APD and reaches the edge surface of the Si,
//you do not want that the photon be reflected or diffused, you want it to
//be considered as lost. But the possibility remains that the photon makes
//a total reflection on the slice of material between Si and revetment!
//In Litrani, we have decided to avoid this difficulty by imposing the
//following : if the user declares a supplementary absorption of 1.0, we
//decide that he wants the photon to be absorbed aniway, so that we cancel
//the possibility of total reflection on the slice of material between face
//and revetment. Keep that in mind !
//  There is a big difference between supplAbs=1.0 and supplAbs=0.99 ! With
//supplAbs=1.0, no total reflection possible, with supplAbs=0.99, total
//reflection possible ! What to do if you want the (rather strange) possi-
//bility that the supplementary absorption be for instance 0.9 and that no
//total reflection can happen ? In that case, declare "none" as the material
//of the slice when you define the revetment !
//
//  b) - Finding real and imaginary parts of index
//
//  In the way diffusion is calculated [ look at "How diffusion is genera-
//ted", above, and look at the method TPhoton::Diffused() ] it is tacitly
//assumed that you know real and imaginary part of the index of refraction
//of the diffusing revetment. It is easy to find such indexes for reflecting
//surfaces in the Handbook of Optical Constants, but not for diffusing
//surfaces. For diffusing surfaces, it is more current to find the albedo of
//the surface. But it is not at all evident to go from the albedo to the
//real and imaginary part of the index of refraction! There is a problem
//there that I do not know how to solve. If you get an idea, please tell it
//to me. On the contrary, you can go from the real and imaginary part of the
//index to the albedo by running Litrani and looking at the proportions of
//photons absorbed by the revetment. As an example, the way the diffusing
//revetment "tyvec" has been introduced into Litran reflects the difficulty
//quoted here : I was not able to find real and imaginary part of the index
//of tyvec, but I found the albedo. What I have done was first to give to
//tyvec the real and imaginary part of aluminium. Then I realised that it
//gave a worse albedo than the one quoted for tyvec. So I have decreased the
//real part of the index of aluminium until it gave the proper albedo for
//tyvec. I completly agree that this way of handling diffusing surfaces is
//tedious, but I have not yet found  a better one.
//
//  c) - Shape with anisotropic material and diffusing revetment without
//       slice of intermediate material
//
//  In the way diffusion is calculated [ look at "How diffusion is genera-
//ted", above, and look at the method TPhoton::Diffused() ] it is tacitly
//assumed that the angle of reflection is equal to the incident angle, which
//is not true for anisotropic material. In consequence, in anisotropic
//material, if the generated value for the angle t happens to be very close
//to 90 degree, you can end up with a diffused photon which is not going
//backward after having be diffused by the revetment !!! This places Litrani
//into a meaningless situation. You will encounter the following message :
//"No intersection with any face !" when Litrani searches what will be the
//next face encountered by the photon !
//  There are 2 ways out of this problem :
//
//      c1) - Either do not declare "none" for the 3rd par. nameMat when the
//revetment is diffusing and the material of the shape covered by the revet-
//ment is anisotropic. If your intention was to have no total reflection
//(it was the reason why you intended to declare nameMat as "none") declare
//for nameMat an isotropic material whose index is slightly bigger than the
//biggest of the 2 indices of the anisotropic material, so that total re-
//flection will be rare.
//      c2) - Or take a ThetaMax sufficiently smaller than 90 degree in such
//a way that the problem never appears.
//
TRevetment::TRevetment(const char *name,const char *title,const char *nameMat,
  const char *nameDiffus,const char *nameRindex,const char *nameIindex,
  const char *nameMu,Double_t supplAbs,Double_t ThetaMax):TNamed(name,title) {
  //constructor when diffusion variable and index of refraction variable and
  //magnetic permeability variable
  TString s;
  Init(ThetaMax);
  s = nameMat;
  FindSliceMat(s);
  s = nameDiffus;
  FindDiffus(s);
  s = nameRindex;
  FindRindex(s);
  s = nameIindex;
  FindIindex(s);
  s = nameMu;
  FindMu(s);
  MoreAbsorption(supplAbs);
  AddToList(name);
}
TRevetment::TRevetment(const char *name,const char *title,const char *nameMat,
  const char *nameDiffus,const char *nameRindex,const char *nameIindex,
  Double_t mu,Double_t supplAbs,Double_t ThetaMax):TNamed(name,title) {
  //constructor when diffusion variable and index of refraction variable and
  //magnetic permeability fixed
  TString s;
  TString snone = "none";
  Init(ThetaMax);
  s = nameMat;
  FindSliceMat(s);
  s = nameDiffus;
  FindDiffus(s);
  s = nameRindex;
  FindRindex(s);
  s = nameIindex;
  FindIindex(s);
  fMu = TMath::Abs(mu);
  fNameMu = snone;
  fFitMu  = 0;
  MoreAbsorption(supplAbs);
  AddToList(name);
}
TRevetment::TRevetment(const char *name,const char *title,const char *nameMat,
  const char *nameDiffus,Double_t rindex,Double_t iindex,const char *nameMu,
  Double_t supplAbs,Double_t ThetaMax):TNamed(name,title) {
  //constructor when diffusion variable and index of refraction fixed and
  //magnetic permeability variable
  const Double_t toosmall = 1.0e-6;
  TString s;
  TString snone = "none";
  Init(ThetaMax);
  s = nameMat;
  FindSliceMat(s);
  s = nameDiffus;
  FindDiffus(s);
  fRindex     = TMath::Abs(rindex);
  fNameRindex = snone;
  fFitRindex  = 0;
  //Notice the minus sign for fIindex
  iindex      = TMath::Abs(iindex);
  if (iindex<toosmall) {
    gMes->SetName("~TRevetment");
    gMes->SetTitle("TRevetment");
    gMes->M(kill_prog,1,"Imaginary part of refraction index too small");
  }
  fIindex     = -iindex;
  fNameIindex = snone;
  fFitIindex  = 0;
  s = nameMu;
  FindMu(s);
  MoreAbsorption(supplAbs);
  AddToList(name);
}
TRevetment::TRevetment(const char *name,const char *title,const char *nameMat,
  Double_t diffus,const char *nameRindex,const char *nameIindex,const char *nameMu,
  Double_t supplAbs,Double_t ThetaMax):TNamed(name,title) {
  //constructor when diffusion fixed and index of refraction variable and
  //magnetic permeability variable
  TString s;
  TString snone = "none";
  Init(ThetaMax);
  s = nameMat;
  FindSliceMat(s);
  diffus = TMath::Abs(diffus);
  if (diffus>=1.0)
    diffus = 1.0;
  fDiffus     = diffus;
  fNameDiffus = snone;
  fFitDiffus  = 0;
  s = nameRindex;
  FindRindex(s);
  s = nameIindex;
  FindIindex(s);
  s = nameMu;
  FindMu(s);
  MoreAbsorption(supplAbs);
  AddToList(name);
}
TRevetment::TRevetment(const char *name,const char *title,const char *nameMat,
  const char *nameDiffus,Double_t rindex,Double_t iindex,Double_t mu,
  Double_t supplAbs,Double_t ThetaMax):TNamed(name,title) {
  //constructor when diffusion variable and index of refraction fixed and
  //magnetic permeability fixed
  const Double_t toosmall = 1.0e-6;
  TString s;
  TString snone = "none";
  Init(ThetaMax);
  s = nameMat;
  FindSliceMat(s);
  s = nameDiffus;
  FindDiffus(s);
  fRindex     = TMath::Abs(rindex);
  fNameRindex = snone;
  fFitRindex  = 0;
  //Notice the minus sign for fIindex
  iindex      = TMath::Abs(iindex);
  if (iindex<toosmall) {
    gMes->SetName("~TRevetment");
    gMes->SetTitle("TRevetment");
    gMes->M(kill_prog,1,"Imaginary part of refraction index too small");
  }
  fIindex     = -iindex;
  fNameIindex = snone;
  fFitIindex  = 0;
  fMu         = TMath::Abs(mu);
  fNameMu     = snone;
  fFitMu      = 0;
  MoreAbsorption(supplAbs);
  AddToList(name);
}
TRevetment::TRevetment(const char *name,const char *title,const char *nameMat,
  Double_t diffus,const char *nameRindex,const char *nameIindex,Double_t mu,
  Double_t supplAbs,Double_t ThetaMax):TNamed(name,title) {
  //constructor when diffusion fixed and index of refraction variable and
  //magnetic permeability fixed
  TString s;
  TString snone = "none";
  Init(ThetaMax);
  s = nameMat;
  FindSliceMat(s);
  diffus = TMath::Abs(diffus);
  if (diffus>=1.0)
    diffus = 1.0;
  fDiffus     = diffus;
  fNameDiffus = snone;
  fFitDiffus  = 0;
  s = nameRindex;
  FindRindex(s);
  s = nameIindex;
  FindIindex(s);
  fMu = TMath::Abs(mu);
  fNameMu = snone;
  fFitMu  = 0;
  MoreAbsorption(supplAbs);
  AddToList(name);
}
TRevetment::TRevetment(const char *name,const char *title,const char *nameMat,
  Double_t diffus,Double_t rindex,Double_t iindex,const char *nameMu,
  Double_t supplAbs,Double_t ThetaMax):TNamed(name,title) {
  //constructor when diffusion fixed and index of refraction fixed and
  //magnetic permeability variable
  const Double_t toosmall = 1.0e-6;
  TString s;
  TString snone = "none";
  Init(ThetaMax);
  s = nameMat;
  FindSliceMat(s);
  diffus = TMath::Abs(diffus);
  if (diffus>=1.0)
    diffus = 1.0;
  fDiffus     = diffus;
  fNameDiffus = snone;
  fFitDiffus  = 0;
  fRindex     = TMath::Abs(rindex);
  fNameRindex = snone;
  fFitRindex  = 0;
  //Notice the minus sign for fIindex
  iindex      = TMath::Abs(iindex);
  if (iindex<toosmall) {
    gMes->SetName("~TRevetment");
    gMes->SetTitle("TRevetment");
    gMes->M(kill_prog,1,"Imaginary part of refraction index too small");
  }
  fIindex     = -iindex;
  fNameIindex = snone;
  fFitIindex  = 0;
  s = nameMu;
  FindMu(s);
  MoreAbsorption(supplAbs);
  AddToList(name);
}
TRevetment::TRevetment(const char *name,const char *title,const char *nameMat,
  Double_t diffus,Double_t rindex,Double_t iindex,Double_t mu,Double_t supplAbs,
  Double_t ThetaMax):TNamed(name,title) {
  //constructor when diffusion fixed    and index of refraction fixed and
  //magnetic permeability fixed
  const Double_t toosmall = 1.0e-6;
  TString s;
  TString snone = "none";
  Init(ThetaMax);
  s = nameMat;
  FindSliceMat(s);
  diffus = TMath::Abs(diffus);
  if (diffus>=1.0)
    diffus = 1.0;
  fDiffus     = diffus;
  fNameDiffus = snone;
  fFitDiffus  = 0;
  fRindex     = TMath::Abs(rindex);
  fNameRindex = snone;
  fFitRindex  = 0;
  //Notice the minus sign for fIindex
  iindex      = TMath::Abs(iindex);
  if (iindex<toosmall) {
    gMes->SetName("~TRevetment");
    gMes->SetTitle("TRevetment");
    gMes->M(kill_prog,1,"Imaginary part of refraction index too small");
  }
  fIindex     = -iindex;
  fNameIindex = snone;
  fFitIindex  = 0;
  fMu         = TMath::Abs(mu);
  fNameMu     = snone;
  fFitMu      = 0;
  MoreAbsorption(supplAbs);
  AddToList(name);
}
TRevetment::~TRevetment() {
  // Destructor. TRevetment is not owner of fSlice, fFitDiffus, fFitRindex, fFitIindex,
  //fFitMu. Do not delete them.
  if (fInUse) {
    gMes->SetName("~TRevetment");
    gMes->SetTitle("TRevetment");
    gMes->M(kill_prog,1,"You cannot delete a revetment which is in use");
  }
  if (gLit) {
    gLit->fRevetment.Remove(this);
    gLit->fNRevetment--;
  }
}
Bool_t TRevetment::AddToList() const {
  //record this revetment into gLit->fRevetment
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TRevetment");
    gMes->M(kill_prog,1,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneRevetment(this,fName);
}
Bool_t TRevetment::AddToList(const char *name) const {
  //record this revetment into gLit->fRevetment
  if (!gLit) {
    gLit = new TLitGlob(85);
    gMes->SetName("AddToList");
    gMes->SetTitle("TRevetment");
    gMes->M(kill_prog,1,"You must define gLit = new TLitGlob(...) first");
  }
  return gLit->AddOneRevetment(this,name);
}
Bool_t TRevetment::CheckIt() {
  //Check that the revetment has been defined correctly and is usable.
  const Double_t plus05  =  0.5;
  const Double_t minus05 = -0.5;
  if (fChecked)
    return fChecked;
  gMes->SetName("CheckIt");
  gMes->SetTitle("TRevetment");
  if ((!fSlice) && (fIsSlice)) {
    ProblemHeader();
    gMes->M(Error_M,1,"Slice material not found");
    return fChecked;
  }
  if (fDiffus<minus05) {
    ProblemHeader();
    gMes->M(Error_M,2,"Fit for % of diffusion not found");
    return fChecked;
  }
  if (fRindex<minus05) {
    ProblemHeader();
    gMes->M(Error_M,3,"Fit for real part of index not found");
    return fChecked;
  }
  if (fIindex>plus05) {
    ProblemHeader();
    gMes->M(Error_M,4,"Fit for imaginary part of index not found");
    return fChecked;
  }
  if (fMu<minus05) {
    ProblemHeader();
    gMes->M(Error_M,5,"Fit for magnetic permeability mu not found");
    return fChecked;
  }
  //If here, everything was ok
  fChecked = kTRUE;
  return fChecked;
}
void TRevetment::FindDiffus(const TString &s1) {
  //Finds the fit or interpolation giving the % of diffusion versus reflection
  const Double_t minus1 = -1.0;
  fFitDiffus = TSplineFit::FindFit(s1.Data(),9);
  if (fFitDiffus) fNameDiffus = s1;
  else {
    gMes->SetName("FindDiffus");
    gMes->SetTitle("TRevetment");
    gMes->M(Error_M,1,"Fit for % of diffusion not found",s1);
    gMes->M(Error_M,1,"Define this fit before defining the revetment !");
    fFitDiffus = 0;
    fDiffus    = minus1;
    return;
  }
}
void TRevetment::FindIindex(const TString &s1) {
  //Finds the fit giving the imaginary part of index of refraction of revetment
  fFitIindex = TSplineFit::FindFit(s1.Data(),4);
  if (fFitIindex) fNameIindex=s1;
  else {
    fNameIindex="";
    gMes->SetName("FindIindex");
    gMes->SetTitle("TRevetment");
    gMes->M(Error_M,1,"Fit for imaginary part of index not found",s1);
    gMes->M(kill_prog,1,"Define this fit before defining the revetment !");
  }
}
void TRevetment::FindMu(const TString &s1) {
  //Finds the fit giving the magnetic permeability mu of revetment.
  fFitMu = TSplineFit::FindFit(s1.Data(),14);
  if (fFitMu) fNameMu=s1;
  else {
    fNameMu="";
    gMes->SetName("FindMu");
    gMes->SetTitle("TRevetment");
    gMes->M(Error_M,1,"Fit for magnetic permeability mu not found : ",s1);
    gMes->M(kill_prog,1,"Define this fit before defining the revetment !");
  }
}
void TRevetment::FindRindex(const TString &s1) {
  //Finds the fit giving the real part of index of refraction of revetment.
  fFitRindex = TSplineFit::FindFit(s1.Data(),3);
  if (fFitRindex) fNameRindex=s1;
  else {
    fNameRindex="";
    gMes->SetName("FindRindex");
    gMes->SetTitle("TRevetment");
    gMes->M(Error_M,1,"Fit for real part of index not found : ",s1);
    gMes->M(kill_prog,1,"Define this fit before defining the revetment !");
  }
}
void TRevetment::FindSliceMat(const TString &s1) {
  //Identifies the material of the slice between face and revetment
  //If s="none", there is no slice : revetment adheres to surface (painting)
  TOpticMaterial *pmat;
  TIter next(&gLit->fOptMat);
  TString s;
  Bool_t found = kFALSE;
  TString snone="none";
  if (!s1.CompareTo(snone))
    fIsSlice = kFALSE;
  else {
    while ((!found) && (pmat = (TOpticMaterial *)next())) {
      s = pmat->GetName();
      if (!s.CompareTo(s1)) {
        pmat->SetInUse();
        found = kTRUE;
        fSlice = pmat;
      }
    }
    if (!found) {
      gMes->SetName("FindSliceMat");
      gMes->SetTitle("TRevetment");
      gMes->M(Error_M,1,"Slice material not found : ",s1);
      gMes->M(Error_M,1,"Please define material of slice before defining revetment");
      gMes->M(Error_M,1,"No slice will be assumed !");
      fSlice = 0;
      return;
    }
  }
}
void TRevetment::Init(Double_t ThetaMax) {
  //basic default initialization
  const Double_t zero    = 0.0;
  const Double_t nonante = 90.0;
  const Double_t pideg   = 180.0;
  //
  fSlice     = 0;
  fFitDiffus = 0;
  fFitRindex = 0;
  fFitIindex = 0;
  fFitMu     = 0;
  //
  fAbsorbing = 0;
  fInUse     = kFALSE;
  fIsSlice   = kTRUE;
  fChecked   = kFALSE;
  fDiffus    = zero;
  fRindex    = zero;
  fIindex    = zero;
  fMu        = zero;
  ThetaMax   = TMath::Abs(ThetaMax);
  if (ThetaMax>nonante) ThetaMax = nonante;
  ThetaMax  *= TMath::Pi()/pideg;
  fSinThetaMax = TMath::Sin(ThetaMax);
}
void TRevetment::MoreAbsorption(Double_t supplAbs) {
  //Look whether there is a supplementary absorption, besides the normal
  //Fresnel absorption due to real part of complex index of refraction.
  //This supplementary absorption may be used to simulate dust or to make
  //the revetment 100% absorbing by setting supplAbs = 1.0
  const Double_t zero  = 0.0;
  const Double_t one   = 1.0;
  const Double_t small = 1.0e-7;
  Double_t almostone;
  almostone  = one-small;
  fAbsorbing = 1;
  fSupplAbs  = supplAbs;
  if (supplAbs<=small) {
    fSupplAbs  = zero;
    fAbsorbing = 0;
  }
  if (supplAbs>=almostone) {
    fSupplAbs = one;
    fAbsorbing = 2;
  }
}
void TRevetment::NewWavelength(Double_t wavelength) {
  //Recalculates all parameters depending upon wavelength.
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  if (fFitDiffus) {
    fDiffus = fFitDiffus->V(wavelength);
    if (fDiffus<zero)
      fDiffus = zero;
    if (fDiffus>un)
      fDiffus = un;
  }
  if (fFitRindex)
    fRindex = TMath::Abs(fFitRindex->V(wavelength));
  //Notice the minus sign for the imaginary part of the index of refraction !
  if (fFitIindex)
    fIindex = - TMath::Abs(fFitIindex->V(wavelength));
  if (fFitMu)
    fMu = TMath::Abs(fFitMu->V(wavelength));
}
void TRevetment::Print() const {
  //Print all characteristics of revetment
  if (!fChecked) {
    gMes->SetName("Print");
    gMes->SetTitle("TRevetment");
    gMes->M(Error_M,1,"Call CheckIt() before calling TRevetment::Print()");
    return;
  }
  PrintTitle();
  if (fIsSlice) {
    cout << "slice of material between face and revetment" << endl;
    cout << "slice material : " << fSlice->GetName() << endl;
  }
  else
    cout << "no slice of material between face and revetment" << endl;
  if (fFitDiffus) {
    cout << "% of diffusion varies with wavelength" << endl;
    cout << "name of fit for % diffusion : " << fNameDiffus.Data() << endl;
  }
  else
    cout << "% of diffusion fixed at     : " << fDiffus << endl;
  if (fFitRindex) {
    cout << "real part of index varies with wavelength" << endl;
    cout << "name of fit for Real(index) : " << fNameRindex.Data() << endl;
  }
  else
    cout << "Real(index) fixed at        : " << fRindex << endl;
  if (fFitIindex) {
    cout << "imaginary part of index varies with wavelength" << endl;
    cout << "name of fit for Im(index)   : " << fNameIindex.Data() << endl;
  }
  else
    cout << "Im(index) fixed at          : " << fIindex << endl;
  if (fFitMu) {
    cout << "magnetic permeability mu varies with wavelength" << endl;
    cout << "name of fit for mu          : " << fNameMu.Data() << endl;
  }
  else
    cout << "mu fixed at                 : " << fMu << endl;
  switch (fAbsorbing) {
    case 0:
      cout << "there is no supplementary absorption" << endl;
      break;
    case 1:
      cout << "supplementary absorption : " << fSupplAbs << endl;
      break;
    case 2:
      cout << "revetment is 100% absorbing" << endl;
      break;
  };
  if (fInUse)
    cout << "in use" << endl;
  else
    cout << "not in use" << endl;
  //Listing
  if (fIsSlice) {
    *gMes->fListing << "slice of material between face and revetment" << endl;
    *gMes->fListing << "slice material : " << fSlice->GetName() << endl;
  }
  else
    *gMes->fListing << "no slice of material between face and revetment" << endl;
  if (fFitDiffus) {
    *gMes->fListing << "% of diffusion varies with wavelength" << endl;
    *gMes->fListing << "name of fit for % diffusion : " << fNameDiffus.Data() << endl;
  }
  else
    *gMes->fListing << "% of diffusion fixed at     : " << fDiffus << endl;
  if (fFitRindex) {
    *gMes->fListing << "real part of index varies with wavelength" << endl;
    *gMes->fListing << "name of fit for Real(index) : " << fNameRindex.Data() << endl;
  }
  else
    *gMes->fListing << "Real(index) fixed at        : " << fRindex << endl;
  if (fFitIindex) {
    *gMes->fListing << "imaginary part of index varies with wavelength" << endl;
    *gMes->fListing << "name of fit for Im(index)   : " << fNameIindex.Data() << endl;
  }
  else
    *gMes->fListing << "Im(index) fixed at          : " << fIindex << endl;
  if (fFitMu) {
    *gMes->fListing << "magnetic permeability mu varies with wavelength" << endl;
    *gMes->fListing << "name of fit for mu          : " << fNameMu.Data() << endl;
  }
  else
    *gMes->fListing << "mu fixed at                 : " << fMu << endl;
  switch (fAbsorbing) {
    case 0:
      *gMes->fListing << "there is no supplementary absorption" << endl;
      break;
    case 1:
      *gMes->fListing << "supplementary absorption : " << fSupplAbs << endl;
      break;
    case 2:
      *gMes->fListing << "revetment is 100% absorbing" << endl;
      break;
  };
  if (fInUse)
    *gMes->fListing << "in use" << endl;
  else
    *gMes->fListing << "not in use" << endl;
}
void TRevetment::PrintProblem() const {
  //Header to point out a problem
  cout << endl;
  cout << "THERE IS A PROBLEM IN THE FOLLOWING REVETMENT :" << endl;
  cout << endl;
  //Listing
  *gMes->fListing << endl;
  *gMes->fListing << "THERE IS A PROBLEM IN THE FOLLOWING REVETMENT :" << endl;
  *gMes->fListing << endl;
}
void TRevetment::PrintTitle() const {
  //Prints name and title of the revetment
  cout << endl;
  cout << "    Revetment     : " << fName.Data() << endl;
  cout << "    Title         : " << fTitle.Data() << endl;
  cout << endl;
  //Listing
  *gMes->fListing << endl;
  *gMes->fListing << "    Revetment     : " << fName.Data() << endl;
  *gMes->fListing << "    Title         : " << fTitle.Data() << endl;
  *gMes->fListing << endl;
}
void TRevetment::ProblemHeader() const {
  //header before reporting a problem
  PrintProblem();
  PrintTitle();
  return;
}
