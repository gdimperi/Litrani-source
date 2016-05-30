// @(#)root/html:$Id: TPhoton.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "Riostream.h"
#include "TMath.h"
#include "TLitPhys.h"
#include "TMessErr.h"
#include "TLitGlob.h"
#include "T33Matrix.h"
#include "T33CMatrix.h"
#include "T3CVector.h"
#include "TOpticMaterial.h"
#include "TRevetment.h"
#include "ThinSlice.h"
#include "TSupplShape.h"
#include "TContact.h"
#include "TStatVolD.h"
#include "TStatAPD.h"
#include "TStatSurfD.h"
#include "TResults.h"
#include "TTWave.h"
#include "TEqIndex.h"
#include "TPhoton.h"

ClassImp(TPhoton)
//______________________________________________________________________________
//
//
//        --------------------------------------------------------------
//        |                                                             |
//        |  This class is an internal class of Litrani. It is not ne-  |
//        | cessary for a user of Litrani to know anything about it.    |
//        | The documentation about it is of interest only for those    |
//        | interested in the inner working of Litrani.                 |
//        |                                                             |
//         -------------------------------------------------------------
//
//  TPhoton contains all data and methods describing a photon inside the set-up.
//
//______________________________________________________________________________
//
TPhoton::TPhoton(Int_t run,Int_t kphot,const T3Vector &x0,TSupplShape *SupplShape,Double_t wvlgth,Double_t t0,T3Vector &K,T3Vector &E,Short_t ks) {
  //Constructor of the photon. Wavelength, time of emission, point of emission
  //and k vector have already been generated. Direction of E vector has also
  //been generated, except in the case of generation inside anisotropic
  //material.
  //  x0         : starting point of photon in World Coordinate
  //  SupplShape : TSupplShape in which photon is generated
  //  wvlgth     : wavelength of photon in nanometers
  //  t0         : start time of photon
  //  K          : k vector of photon
  //  E          : direction of E field in case of isotropic material
  //               0 otherwise.
  //  ks         : choice between n1 and n2 {0->random,1->biggest,2->smallest}
  //                in anisotropic material. In anisotropic material, there
  //                are 2 possible values for the index of refraction once
  //                the direction of the k vector is fixed.
  //
  const Double_t zero = 0.0;
  InitP();
  Init();
  fRun   = run;
  fKphot = kphot;
  fChooseIndex = ks;
  fNPoints = 1;
  PrintWhich(gLit->fPrintFreq);
  fAnomaly = kFALSE;
  GetShapeAndMat(SupplShape);
  fI  = new TTWave("incident","incident wave");
  fI->fShare  = Taken; //Incident wave always participates to process
  fI->fPhases = 1;     //By construction, one arbitrary, irrelevant, global phase
  fR1 = new TTWave("reflected_1","1st reflected wave");
  fR2 = new TTWave("reflected_2","2nd reflected wave");
  fT1 = new TTWave("tranmitted_1","1st transmitted wave");
  fT2 = new TTWave("tranmitted_2","2nd transmitted wave");
  fX0 = x0;
  fWvlgth = wvlgth;
  fT0 = t0;
  fUk0 = K;
  fEf0 = E;
  fSl0    = zero;
  fSkl0   = zero;
  fNbOfDiffusion = 0;
  fNbOfRayleigh = 0;
  fNbOfWLS = 0;
  StartPhoton();
}
TPhoton::~TPhoton() {
  // Destructor : TPhoton is only owner of TTWave
  if (fI)  delete fI;
  if (fR1) delete fR1;
  if (fR2) delete fR2;
  if (fT1) delete fT1;
  if (fT2) delete fT2;
}
void TPhoton::AgainstRev() {
  //Handles the case of a photon hitting the face of a shape. Behind the face hit
  //is a revetment or nothing.
  //    if nothing   : it is assumed that behind the face space is filled with the
  //                   material "Outside".
  //    if revetment : there may be nothing between the face and the revetment
  //                   or there may be a slice of material (for instance air).
  //  Notice that the slice of material between face or revetment is always
  //isotropic (fAnist=0). If fMatSl is the material "Outside" of the outside
  //world, we REQUIRE that it be isotropic.
  //
  Int_t bin;
  Double_t y;
  Bool_t depolished;
  fBetween = kFALSE;
  if (fMatSl) {
    //There is a slice of material between the face and the revetment, or fMatSl is
    //the material of the outside world "Outside"
    fAnist    = 0;
    fAniat    = 0;
    fPmut     = fMatSl->GetMu();
    fRfIndext = fMatSl->GetIndex();
    //Depolishing of surface is forbidden if material anisotropic (too
    //complicate !)
    if (fS->HasBevellings()) depolished = fUsedFace->IsDepolished(fDistRidge);
    else depolished = fUsedFace->IsDepolished();
    if ((depolished) && (!fAnis0)) {
      Depolish();
      fOx = fFNd;
    }
    else {
      fOx = fFN;
    }
    if (!fAnomaly) {
      Transition();
      if (!fAnomaly) {
        if (fPhtOut) {
          fOutPoly = kTRUE;
          if (!fRevetment) {
            //If there is no revetment and the photon goes out, it is lost in outside world
            fPhtLive = kFALSE;
            fPhtAbs  = kTRUE;
            gGs->fNpLossAny += 1;
            gGs->fNpOutSide += 1;
            bin = gGs->fHAbsAny->Fill(fSl0);
            bin = gGs->fHOutSide->Fill(fSl0);
            bin = gGs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
            if (gCs) {
              gCs->fNpLossAny += 1;
              gCs->fNpOutSide += 1;
              bin = gCs->fHAbsAny->Fill(fSl0);
              bin = gCs->fHOutSide->Fill(fSl0);
              bin = gCs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
            }
          }
          else {
            //If there is a revetment and the photon goes out, it is in front of the
            //revetment. This is handled by ExitFace().
            ExitFace();
          }// end else if (!fRevetment)
        }// end if (fPhtOut)
        fOutPoly = kFALSE;
      }// end if (!fAnomaly)
    }// end if (!fAnomaly)
  }// end if (fMatSl)
  else {
    //There is a revetment, but there is a NO slice of material between the face
    //and the revetment. First we must check whether there is a supplementary
    //absorption (in addition to the normal absorption given by the Fresnel
    //equations)
    bin = gGs->fHHitRvt->Fill(fANShape,Axis_t(fKFace));
    if (gCs) bin = gCs->fHHitRvt->Fill(fANShape,Axis_t(fKFace));
    switch (fRevetment->IsSupplAbs()) {
    case 1:
      y = gRandom3->Rndm();
      if (y<fRevetment->GetSupplAbs()) fPhtAbs = kTRUE;
      break;
    case 2:
      fPhtAbs = kTRUE;
      break;
    default:
      break;
    }
    if (fPhtAbs) {
      //Photon has been absorbed before the revetment
      fPhtLive = kFALSE;
      gGs->fNpLossAny += 1;
      gGs->fNpAbsBef  += 1;
      bin = gGs->fHAbsAny->Fill(fSl0);
      bin = gGs->fHAbsBef->Fill(fSl0);
      bin = gGs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
      if (gCs) {
        gCs->fNpLossAny += 1;
        gCs->fNpAbsBef  += 1;
        bin = gCs->fHAbsAny->Fill(fSl0);
        bin = gCs->fHAbsBef->Fill(fSl0);
        bin = gCs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
      }
    }
    else {
      //Photon has NOT been absorbed before the revetment. Photon is alive, standing
      //before the revetment. A revetment is never anisotropic ==> fAnist = 0;
      //We must decide whether the photon is reflected or diffused
      fAnist = 0;
      fAniat = 0;
      fPmut  = fRevetment->GetMu();
      fRfIndext = fRevetment->GetIndex();
      y = gRandom3->Rndm();
      if (y>fRevetment->Diffus()) {
        //Here the photon is reflected
        if (fS->HasBevellings()) depolished = fUsedFace->IsDepolished(fDistRidge);
        else depolished = fUsedFace->IsDepolished();
        if ((depolished) && (!fAnis0)) {
          PrintWhich();
          gMes->SetName("AgainstRev");
          gMes->SetTitle("TPhoton");
          gMes->M(Error_M,1,"A face having a revetment without slice is forbidden to be depolished");
          gMes->M(Info_M,1,"we continue without depolish for this face");
        }
        fOx = fFN;
        if (!fAnomaly) {
          Transition();
          if (!fAnomaly) {
            if (fPhtAbs) {
              //Photon has been absorbed by the revetment
              fPhtLive = kFALSE;
              gGs->fNpLossAny += 1;
              gGs->fNpAbsRvt  += 1;
              bin = gGs->fHAbsAny->Fill(fSl0);
              bin = gGs->fHAbsRvt->Fill(fSl0);
              bin = gGs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
              if (gCs) {
                gCs->fNpLossAny += 1;
                gCs->fNpAbsRvt  += 1;
                bin = gCs->fHAbsAny->Fill(fSl0);
                bin = gCs->fHAbsRvt->Fill(fSl0);
                bin = gCs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
              }
            }// end if (fPhtAbs)
          }// end if (!fAnomaly)
        }// end if (!fAnomaly)
      }// end if (y>fRevetment->Diffus())
      else {
        //Here the photon is diffused. We do not depolish in that case.
        fOx = fFN;
        Diffused();
      }// end else if (y>fRevetment->Diffus())
    }// end else if (fPhtAbs)
  }// end else if (fMatSl) 
}// end AgainstRev()
void TPhoton::AgainstShape() {
  //Handles the case of a photon hitting the face of a shape. Behind the face
  //hit is an other face belonging to an other shape.
  Int_t i,bin;
  if (fS->SameMedium(fContactSuppl)) {
    //Photon penetrates into the other shape
    bin = gGs->fHCrossFace->Fill(fANShape,Axis_t(fKFace));
    if (gCs) bin = gCs->fHCrossFace->Fill(fANShape,Axis_t(fKFace));
    fShape   = fContactShape;
    fS       = fContactSuppl;
//    cout << "Entering " << fS->GetName() << endl;
    fNShape  = fS->fShapeNumber;
    fANShape = fNShape + 0.01;
    fFace    = fContactFace;
    fNFace   = fFace->GetNumber();
    fKFace   = fNFace;
    //case of subfaces
    fNbSubFaces = fFace->NbSubFaces();
    fSubFace = 0;
    if (fNbSubFaces) {
      Bool_t OnSubFace = kFALSE;
      i = 0;
      while ((!OnSubFace) && (i<fNbSubFaces)) {
        fUsedFace = fFace->GetSubFace(i);
        OnSubFace = fUsedFace->Contained(fX0);
        i++;
      }
      if (OnSubFace) fSubFace = fUsedFace;
    }
    if (fSubFace) {
      fUsedFace = fSubFace;
      fNSubFace = fSubFace->GetNumber();
      fKFace    = fNSubFace;
    }
    else fUsedFace = fFace;
    //end case of subfaces
    bin = gGs->fHHitFace->Fill(fANShape,Axis_t(fKFace));
    if (gCs) bin = gCs->fHHitFace->Fill(fANShape,Axis_t(fKFace));
    fMat0    = fS->GetOptMat();
    fPmu0    = fMat0->GetMu();
    fAnis0   = fMat0->GetAnisotropy();
    fAnia0   = fMat0->GetAniAbsL();
    if (fAnis0) fDzt0 = fS->fDTmu;
    else fRfIndex0 = fMat0->GetIndex();
    fIsAbsorb   = fMat0->GetHasAbsorption();
    if (fIsAbsorb) {
      if (fAnia0) {
        fCDzt0 = fS->fCDTmu;
        FindAbsLength();
      }
      else fAbsl0 = fMat0->GetAbsL();}
    fIsRayleigh = fMat0->GetHasRayleigh();  
    fRayleighL  = fMat0->GetRayleighLength();
    fIsWLS      = fMat0->GetHasWLS();
    fWLSL       = fMat0->GetWLSL();
    fIsDiffus = fMat0->GetHasDiffusion();
    fDiffusL  = fMat0->GetDiffLength();
    fOutPoly = kFALSE;
    fBetween = kFALSE;
    fToOther = kFALSE;
  }
  else {
    //Here media are different, so that calculations of transition needed
    fBetween  = kTRUE;
    fMat1     = fContactSuppl->GetOptMat();
    fPmu1     = fMat1->GetMu();
    fAnis1    = fMat1->GetAnisotropy();
    fAnia1    = fMat1->GetAniAbsL();
    if (fAnis1) fDzt1 = fContactSuppl->fDTmu;
    else fRfIndex1 = fMat1->GetIndex();
    fAnist    = fAnis1;
    fAniat    = fAnia1;
    fPmut     = fPmu1;
    if (fAnist) fDztt = fDzt1;
    else fRfIndext = fRfIndex1;
    //We do not accept depolish when in contact with an other shape
    fOx = fFN;
    Transition();
    if (!fAnomaly) {
      if (fPhtOut) {
        //Photon penetrates into the other shape
        bin = gGs->fHCrossFace->Fill(fANShape,Axis_t(fKFace));
        if (gCs) bin = gCs->fHCrossFace->Fill(fANShape,Axis_t(fKFace));
        fShape   = fContactShape;
        fS       = fContactSuppl;
        fNShape  = fS->fShapeNumber;
        fANShape = fNShape + 0.01;
        fFace    = fContactFace;
        fNFace   = fFace->GetNumber();
        fKFace   = fNFace;
        //case of subfaces
        fNbSubFaces = fFace->NbSubFaces();
        fSubFace = 0;
        if (fNbSubFaces) {
          Bool_t OnSubFace = kFALSE;
          i = 0;
          while ((!OnSubFace) && (i<fNbSubFaces)) {
            fUsedFace = fFace->GetSubFace(i);
            OnSubFace = fUsedFace->Contained(fX0);
            i++;
          }
          if (OnSubFace) fSubFace = fUsedFace;
        }
        if (fSubFace) {
          fUsedFace = fSubFace;
          fNSubFace = fSubFace->GetNumber();
          fKFace    = fNSubFace;
        }
        else fUsedFace = fFace;
        //end case of subfaces
        bin = gGs->fHHitFace->Fill(fANShape,Axis_t(fKFace));
        if (gCs) bin = gCs->fHHitFace->Fill(fANShape,Axis_t(fKFace));
        fMat0    = fS->GetOptMat();
        fPmu0    = fMat0->GetMu();
        fAnis0   = fMat0->GetAnisotropy();
        fAnia0   = fMat0->GetAniAbsL();
        if (fAnis0) fDzt0 = fS->fDTmu;
        else fRfIndex0 = fMat0->GetIndex();
        fIsAbsorb   = fMat0->GetHasAbsorption();
        if (fIsAbsorb) {
          if (fAnia0) {
            fCDzt0 = fS->fCDTmu;
            FindAbsLength();
          }
          else fAbsl0 = fMat0->GetAbsL();}
        fIsRayleigh = fMat0->GetHasRayleigh();  
        fRayleighL  = fMat0->GetRayleighLength();
        fIsWLS      = fMat0->GetHasWLS();
        fWLSL       = fMat0->GetWLSL();
        fIsDiffus   = fMat0->GetHasDiffusion();
        fDiffusL  = fMat0->GetDiffLength();
        fOutPoly = kFALSE;
        fBetween = kFALSE;
        fToOther = kFALSE;
      }
      else {
        //Photon turns back and remain in the same shape
        bin = gGs->fHDontCross->Fill(fANShape,Axis_t(fKFace));
        if (gCs) bin = gCs->fHDontCross->Fill(fANShape,Axis_t(fKFace));
        fBetween = kFALSE;
        fOutPoly = kFALSE;
      }
    }
  }
}
void TPhoton::AniToAni() const {
  //Calculates the transition from an anisotropic medium towards an anisotropic
  //medium
  PrintWhich();
  gMes->SetName("AniToAni");
  gMes->SetTitle("TPhoton");
  gMes->M(kill_prog,1,"Transition from anisotropic to anisotropic not yet implemented");
}
void TPhoton::AniToIso() {
  //Calculates the transition from an anisotropic medium towards an isotropic
  //medium
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  const Double_t small  = 1.0e-5;
  const Double_t dixm4  = 1.0e-4;
  const Double_t wsmall = 1.0e-11;
  const Double_t DbLim  = 1.0e-150;
  Double_t TestIm;
  Bool_t Good1;              //The 1st solution selected among the 4
  //proposed by TEqIndex is good or not
  Bool_t Good2;              //The 2nd solution selected among the 4
  //proposed by TEqIndex is good or not
  Short_t kIncoming;         //points at the one among the 4 solution which is
  //closest to the incoming wave. The incoming wave
  //appears among the 4 solutions and must be avoided !
  Short_t is;
  Short_t imin1,imin2;       //2 solutions among the 4 proposed by TEqIndex
  Bool_t  eva1,eva2;         //true if wave fR1(fR2) evanescent
  Bool_t isol[4];
  Double_t d00,d01,d02,d11,d12,d22;
  Double_t c01,c02,c12;
  Double_t a1,a3,a4,a5,gd;
  Double_t quality;          //quality of solution provided by TEqIndex
  Double_t ns;               //(index of refraction)*(sin(thetai) of incident wave
  Double_t a,b,c,d,g;        //coefficients for the equation giving the 2
  //indices for the 2 reflected waves
  Double_t dh1,dh2;          //intermediate calculations
  TSComplex rac;              //square root of equation (48)
  TSComplex z2,eps;           //for test of equation (48)
  TSComplex zh1;              //intermediate calculations
  T3Vector vh1;
  //
  Bool_t invers = kFALSE;    //true if the 2 solutions have been interchanged
  fR1->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  fR2->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  fT1->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  fT2->fShare = NotInvolved; //no 2nd transmitted wave in an isotropic medium
  fT1->fRfI   = fRfIndext;   //assigns to T1 wave its index of refraction
  //Fresnel or Descartes formula, between incident and transmitted
  ns          = fI->fRfI.Re()*fI->fStht.Re();
  fT1->fStht  = ns/fT1->fRfI;
  fT1->fCtht  = fT1->fStht;
  //
  //  Notice this important point :
  //   The imaginary part of fT1->fCtht MUST be negative in order to have
  //absorption and not explosion of the transmitted wave. This is why CosFromSin
  //always chooses the solution with negative imaginary part.
  //
  fT1->fCtht.CosFromSin();
  //If fT1->fCtht is "almost" pure imaginary, the very small real part is due
  //to numerical imprecision. We clean it.
  if ((TMath::Abs(fT1->fCtht.Im())>wsmall) &&
    (TMath::Abs(fT1->fCtht.Re())<wsmall)) fT1->fCtht.PureIm();
  //Later on, we will have to divide by fT1->fCtht. Hence we cannot accept it
  //to be 0. If it is the case, we signal an error, but continue giving a
  //small value to fT1->fCtht. fT1->fCtht = 0 corresponds to the case of a
  //grazing incident wave, which theoretically has a 0 probability to happen,
  //but can happen practically due to numerical imprecision.
  if (Abs(fT1->fCtht)<DbLim) {
    PrintWhich();
    gMes->SetName("AniToIso");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"fT1->fCtht too small",fT1->fCtht);
    fT1->fCtht = DbLim;
  }
  // Here comes the calculation of the equation (E1) (see theoretical paper)
  //giving the 2 possible indices for the 2 reflected waves
  g   = ns*ns;
  d00 = fDztii(0,0);
  d01 = fDztii(0,1);
  d02 = fDztii(0,2);
  d11 = fDztii(1,1);
  d12 = fDztii(1,2);
  d22 = fDztii(2,2);
  c01 = d01*d01;
  c02 = d02*d02;
  c12 = d12*d12;
  a1  = (d00-d11)*g + d00*(d11+d22) - c02 - c01;
  a3  = - deux*d01*ns;
  a4  = - deux*ns*(d01*d22-d02*d12);
  a5  = g*(c02 - c12 - d22*(d00-d11));
  a5 += - d00*d11*d22 - deux*d01*d02*d12 + d11*c02 + d00*c12 + d22*c01;
  // We have to divide by d00, so we cannot accept it to be 0
  if (TMath::Abs(d00) < DbLim) {
    PrintWhich();
    gMes->SetName("AniToIso");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,2,"Abs(fDztii(0,0)) is too small",d00);
    fAnomaly = kTRUE;
    return;
  }
  fFromani = kTRUE;
  a = -a1/d00;
  b =  a3/d00;
  c = -a4/d00;
  d = -a5/d00;
  if (fIsDiagi) {
    gd  = d22 - d11;
    gd += (g*(d11 - d00))/d00;
    gd *= gd;
  }
  else {
    gd  = a*a - quatre*d;
  }
  gIdx->Set(a,b,c,d,g,gd);
  quality = gIdx->Solve(fAnisi,kTRUE);
  if (quality>small) {
    PrintWhich();
    gMes->SetName("AniToIso");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,3,"bad solution for index equation",quality);
    gMes->M(kill_this,3,"a b  : ",a,b);
    gMes->M(kill_this,3,"c d  : ",c,d);
    gMes->M(kill_this,3,"g gd : ",g,gd);
    fAnomaly = kTRUE;
    return;
  }
  Bool_t bii = (fI->fCtht.Re()>=zero);
  kIncoming = gIdx->ClosestFrom(fI->fRfI.Re(),bii);
  for (is=0;is<4;is++) isol[is] = kTRUE;
  Short_t NumberOfTry = 0;
bigloop: 
  do {
    Good1 = kFALSE;
    Good2 = kFALSE;
    NumberOfTry += 1;
    fR1->fPhases = 0;
    fR2->fPhases = 0;
    fT1->fPhases = 0;
    //We look for 2 possible solutions. Notice that we refuse that both reflective waves
    //be evanescent.
    Bool_t found = kFALSE;
    while (!found) {
      GetImin12(NumberOfTry,imin1,imin2,kIncoming);
      found = (isol[imin1]) && (isol[imin2]);
      if (!found) {
        NumberOfTry += 1;
        if (NumberOfTry>6) {
          gMes->SetName("AniToIso");
          gMes->SetTitle("TPhoton");
          gMes->M(kill_this,4,"all solutions tried without success",fKphot);
          fAnomaly = kTRUE;
          return;
        }// end if (NumberOfTry>6)
      }// end if (!found)
    }// end while (!found)
    eva1 = gIdx->GetfR(imin1);
    eva2 = gIdx->GetfR(imin2);
    //We accept that one of the 2 reflected wave be evanescent, but it must be the
    //second one !
    if ((eva1) && (!eva2)) {
      is    = imin1;
      imin1 = imin2;
      imin2 = is;
      eva1  = kFALSE;
      eva2  = kTRUE;
    }
    //We assign found value of index to the 1st reflected wave :
    fR1->fRfI = Sqrt(gIdx->GetfZ(imin1));
    fR1->fEva = eva1;
    if (!eva1) fR1->fRfI.PureReal();
    //We assign found value of index to the 2nd reflected wave :
    fR2->fRfI = Sqrt(gIdx->GetfZ(imin2));
    fR2->fEva = eva2;
    if (!eva2) fR2->fRfI.PureReal();
    //Fresnel or Descartes equation for the sinus of the 1st reflected wave
    fR1->fStht = ns/fR1->fRfI;
    fR1->fCtht = gIdx->GetfD(imin1);
    //We clean very small real or imaginary parts. When presents, they are almost
    //always due to numerical imprecision. If on the contrary they were physical,
    //we at most introduce an error of wsmall in this cosine.
    if ((TMath::Abs(fR1->fCtht.Im())>wsmall) &&
      (TMath::Abs(fR1->fCtht.Re())<wsmall)) fR1->fCtht.PureIm();
    if ((TMath::Abs(fR1->fCtht.Re())>wsmall) &&
      (TMath::Abs(fR1->fCtht.Im())<wsmall)) fR1->fCtht.PureReal();
    else fR1->fEva = kTRUE;
    //Check that equation (48) is satisfied with solution (1)
    if (gMes->TestCalc()) {
      z2  = fR1->fRfI*fR1->fRfI;
      rac = fR1->fRfI*fR1->fCtht;
      eps = z2*z2 + a*z2 + d + (b*z2 + c)*rac;
      if (Abs(eps)>small) {
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(Error_M,25,"Bad solution of equation (48)");
      }
    }
    //Fresnel or Descartes equation for the sinus of the 2nd reflected wave
    fR2->fStht = ns/fR2->fRfI;
    fR2->fCtht = gIdx->GetfD(imin2);
    //We clean very very small real or imaginary parts. When presents, they are almost
    //always due to numerical imprecision. If on the contrary they were physical, we
    //at most introduce an error of wsmall in this cosine.
    if ((TMath::Abs(fR2->fCtht.Im())>wsmall) &&
      (TMath::Abs(fR2->fCtht.Re())<wsmall)) fR2->fCtht.PureIm();
    if ((TMath::Abs(fR2->fCtht.Re())>wsmall) &&
      (TMath::Abs(fR2->fCtht.Im())<wsmall)) fR2->fCtht.PureReal();
    else fR2->fEva = kTRUE;
    //Check that equation (48) is satisfied with solution (2)
    if (gMes->TestCalc()) {
      z2  = fR2->fRfI*fR2->fRfI;
      rac = fR2->fRfI*fR2->fCtht;
      eps = z2*z2 + a*z2 + d + (b*z2 + c)*rac;
      if (Abs(eps)>small) {
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(Error_M,26,"Bad solution of equation (48)");
      }
    }
    //Again, we must check that both waves are not evanescent.
    if ((fR1->fEva) && (fR2->fEva)) continue;
    //We must check that if only one wave evanescent,then it is the second
    if ((fR1->fEva) && (!fR2->fEva)) {
      TSComplex cc1,cc2,cc3;
      invers     = kTRUE;
      cc1        = fR2->fRfI;
      cc2        = fR2->fStht;
      cc3        = fR2->fCtht;
      fR2->fRfI  = fR1->fRfI;
      fR2->fStht = fR1->fStht;
      fR2->fCtht = fR1->fCtht;
      fR1->fRfI  = cc1;
      fR1->fStht = cc2;
      fR1->fCtht = cc3;
      fR1->fEva  = kFALSE;
      fR2->fEva  = kTRUE;
    }
    //
    //Determination of alfa (Salf,Calf) and delta (Sdel,Cdel) by solution of the
    //degenerate system (4), (5), (6) [see theoretical paper]. We separate the
    //real and the complex case.
    //
    //1st reflected wave :
    //
    if (fR1->fEva) {
      TSComplex za,z00,z01,z02,z11,z12,z22;
      za  = fR1->fRfI*fR1->fCtht;
      z00 = g - d00;
      z01 = ns*za - d01;
      z02 = d02;
      z11 = za*za - d11;
      z12 = d12;
      za  = fR1->fRfI;
      z22 = za*za - d22;
      T33CMatrix M;
      M.SetLine(0,z00,z01,z02);
      M.SetLine(1,z01,z11,z12);
      M.SetLine(2,z02,z12,z22);
      T3CVector v1,v2;
      Short_t tdeg;
      tdeg = v1.DegMatrix(M,v2,gMes->TestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,5,"1st reflected wave : matrix M (complex) not degenerate");
        fAnomaly = kTRUE;
        return;
      case 2:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,6,"1st reflected wave : matrix M (complex) twice degenerate");
        fAnomaly = kTRUE;
        return;
      case 3:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,7,"1st reflected wave : matrix M (complex) is 0");
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      bdeg = v1.AlfaDelta(fR1->fStht,fR1->fCtht,
        fR1->fSalf,fR1->fCalf,fR1->fSdel,fR1->fCdel);
      //bdeg = kFALSE is not a problem
    }
    else {
      //Here, the degenarate matrix M is real !
      Double_t qa,q00,q01,q02,q11,q12,q22;
      qa  = fR1->fRfI.Re()*fR1->fCtht.Re();
      q00 = g - d00;
      q01 = ns*qa - d01;
      q02 = d02;
      q11 = qa*qa - d11;
      q12 = d12;
      qa  = fR1->fRfI.Re();
      q22 = qa*qa - d22;
      T33Matrix M;
      M.SetLine(0,q00,q01,q02);
      M.SetLine(1,q01,q11,q12);
      M.SetLine(2,q02,q12,q22);
      T3Vector v1,v2;
      Short_t tdeg;
      tdeg = v1.DegMatrix(M,v2,gMes->TestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,8,"1st reflected wave : matrix M (real) not degenerate");
        fAnomaly = kTRUE;
        return;
      case 2:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,9,"1st reflected wave : matrix M (real) twice degenerate");
        fAnomaly = kTRUE;
        return;
      case 3:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,10,"1st reflected wave : matrix M (real) is 0");
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      Double_t qsa,qca,qsd,qcd;
      bdeg = v1.AlfaDelta(fR1->fStht.Re(),fR1->fCtht.Re(),qsa,qca,qsd,qcd);
      //bdeg = kFALSE is not a problem
      fR1->fSalf = qsa;
      fR1->fCalf = qca;
      fR1->fSdel = qsd;
      fR1->fCdel = qcd;
    }
    //
    //2nd reflected wave :
    //
    if (fR2->fEva) {
      TSComplex za,z00,z01,z02,z11,z12,z22;
      za  = fR2->fRfI*fR2->fCtht;
      z00 = g - d00;
      z01 = ns*za - d01;
      z02 = d02;
      z11 = za*za - d11;
      z12 = d12;
      za  = fR2->fRfI;
      z22 = za*za - d22;
      T33CMatrix M;
      M.SetLine(0,z00,z01,z02);
      M.SetLine(1,z01,z11,z12);
      M.SetLine(2,z02,z12,z22);
      T3CVector v1,v2;
      Short_t tdeg;
      tdeg = v1.DegMatrix(M,v2,gMes->TestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,11,"2nd reflected wave : matrix M (complex) not degenerate");
        fAnomaly = kTRUE;
        return;
      case 2:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,12,"2nd reflected wave : matrix M (complex) twice degenerate");
        fAnomaly = kTRUE;
        return;
      case 3:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,13,"2nd reflected wave : matrix M (complex) is 0");
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      bdeg = v1.AlfaDelta(fR2->fStht,fR2->fCtht,
        fR2->fSalf,fR2->fCalf,fR2->fSdel,fR2->fCdel);
      //bdeg = kFALSE is not a problem
    }
    else {
      //Here, the degenarate matrix M is real !
      Double_t qa,q00,q01,q02,q11,q12,q22;
      qa  = fR2->fRfI.Re()*fR2->fCtht.Re();
      q00 = g - d00;
      q01 = ns*qa - d01;
      q02 = d02;
      q11 = qa*qa - d11;
      q12 = d12;
      qa  = fR2->fRfI.Re();
      q22 = qa*qa - d22;
      T33Matrix M;
      M.SetLine(0,q00,q01,q02);
      M.SetLine(1,q01,q11,q12);
      M.SetLine(2,q02,q12,q22);
      T3Vector v1,v2;
      Short_t tdeg;
      tdeg = v1.DegMatrix(M,v2,gMes->TestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,14,"2nd reflected wave : matrix M (real) not degenerate");
        fAnomaly = kTRUE;
        return;
      case 2:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,15,"2nd reflected wave : matrix M (real) twice degenerate");
        fAnomaly = kTRUE;
        return;
      case 3:
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,16,"2nd reflected wave : matrix M (real) is 0");
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      Double_t qsa,qca,qsd,qcd;
      bdeg = v1.AlfaDelta(fR2->fStht.Re(),fR2->fCtht.Re(),qsa,qca,qsd,qcd);
      //bdeg = kFALSE is not a problem
      fR2->fSalf = qsa;
      fR2->fCalf = qca;
      fR2->fSdel = qsd;
      fR2->fCdel = qcd;
    }
    //
    //Here are the variables that we have determined until now :
    //
    //    fR1->fRfI
    //    fR1->fStht and fCtht
    //    fR1->fSalf and fCalf
    //    fR1->fSdel and fCdel
    //    fR2->fRfI
    //    fR2->fStht and fCtht
    //    fR2->fSalf and fCalf
    //    fR2->fSdel and fCdel
    //    fT1->fRfI
    //    fT1->fStht and fCtht
    //
    //Here are the variables that remain to be found :
    //
    //    fR1->fE
    //    fR2->fE
    //    fT1->fE
    //    fT1->fSalf and fCalf
    //
    //These last variables are found thanks to equations (10) (11) (12) (13)
    // (See theoretical paper)
    //
    //
    //Solution for fR1->fE and fR2->fE
    //
    zh1  = fPmut/(fT1->fRfI*fPmui);
    fBI  = fI->fRfI.Re()*zh1;
    fBR1 = fR1->fRfI*zh1;
    fBR2 = fR2->fRfI*zh1;
    fI->fAlm1  = fI->fStht*fI->fSdel + fI->fCtht*fI->fCdel;
    fR1->fAlm1 = fR1->fStht*fR1->fSdel + fR1->fCtht*fR1->fCdel;
    fR2->fAlm1 = fR2->fStht*fR2->fSdel + fR2->fCtht*fR2->fCdel;
    TSComplex F11,F12,F13,F21,F22,F23,F31,F32,F33,F41,F42,F43,disc;
    F11  = fBI*fI->fCalf*fI->fAlm1;
    F12  = fBR1*fR1->fCalf*fR1->fAlm1;
    F13  = fBR2*fR2->fCalf*fR2->fAlm1;
    F21  = (fI->fCdel*fI->fCalf)/fT1->fCtht;
    F22  = zero - (fR1->fCdel*fR1->fCalf)/fT1->fCtht;
    F23  = zero - (fR2->fCdel*fR2->fCalf)/fT1->fCtht;
    F31  = fI->fSalf;
    F32  = fR1->fSalf;
    F33  = fR2->fSalf;
    F41  = (fBI*fI->fSalf*fI->fCtht)/fT1->fCtht;
    F42  = zero - (fBR1*fR1->fSalf*fR1->fCtht)/fT1->fCtht;
    F43  = zero - (fBR2*fR2->fSalf*fR2->fCtht)/fT1->fCtht;
    disc = (F12-F22)*(F33-F43) - (F13-F23)*(F32-F42);
    //
    //We have to divide by disc, so it cannot be extremely small. If we find it
    //extremely small, we take it as a sign that we have made a wrong choice of
    //solution from TEqIndex. For instance, if by error TEqIndex has given twice
    //the same solution, disc will be 0. So we try an other pair of solution.
    //
    if (Abs(disc)<DbLim) continue;
    zh1 = fI->fE/disc;
    fR1->fE = zh1*((F21-F11)*(F33-F43) - (F41-F31)*(F13-F23));
    fR2->fE = zh1*((F12-F22)*(F41-F31) - (F32-F42)*(F21-F11));
    //We do not accept that the real part of the module of an electric field be <0
    if (fR1->fE.Re()<zero) {
      fR1->fE    = zero - fR1->fE;
      fR1->fSalf = zero - fR1->fSalf;
      fR1->fCalf = zero - fR1->fCalf;
      F12        = zero - F12;
      F22        = zero - F22;
      F32        = zero - F32;
      F42        = zero - F42;
    }
    if (fR2->fE.Re()<zero) {
      fR2->fE    = zero - fR2->fE;
      fR2->fSalf = zero - fR2->fSalf;
      fR2->fCalf = zero - fR2->fCalf;
      F13        = zero - F13;
      F23        = zero - F23;
      F33        = zero - F33;
      F43        = zero - F43;
    }
    //
    //Only 2 variables remain to be found :
    //
    //    fT1->fE
    //    fT1->fSalf and fCalf
    //
    TSComplex tca,tsa;
    tca = F11*fI->fE + F12*fR1->fE + F13*fR2->fE;
    tsa = F31*fI->fE + F32*fR1->fE + F33*fR2->fE;
    fT1->fE    = Sqrt(tsa*tsa + tca*tca);
    if (fT1->fE.Re()<zero) fT1->fE = zero - fT1->fE;
    fT1->fCalf = tca/fT1->fE;
    fT1->fSalf = tsa/fT1->fE;
    //
    //All the variables of system of equations (E1) have no been found ! We calculate
    //now the other physical values depending upon these variables
    //
    //
    //  Last calculations for wave fR1
    //
    // - module and phase of electric field :
    fR1->fE.RPhi(fR1->fAE,fR1->fPE);
    if (TMath::Abs(fR1->fPE)>wsmall) fR1->fPhases += 1;
    else fR1->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    fR1->fCalf.RPhi(fR1->fWcaf,fR1->fPcaf);
    fR1->fSalf.RPhi(fR1->fWsaf,fR1->fPsaf);
    if (TMath::Abs(fR1->fPcaf - fR1->fPsaf)>wsmall)  fR1->fPhases += 1;
    // - angle of refraction complex for the 1st reflective wave ? No !
    if (TMath::Abs(fR1->fCtht.Im()) + TMath::Abs(fR1->fStht.Im())>wsmall)
      fR1->fPhases = 3;
    switch (fR1->fPhases) {
    case 0:
    case 1:
      //0 or 1 phase. No reason to choose between parallel and normal component.
      //Notice the - sign in front of fR1->fCtht due to the fact that the reflection
      //angle is defined contrary to the trigonometric sense. A reflection angle
      //EQUAL to the incidence angle implies a reflected wave having an OPPOSITE x
      //direction as the incident wave.
      fR1->fUk.Set(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
      //The index of refraction of the 1st reflected wave cannot be complex in the
      //case where there are only 0 or 1 phase
      if (TMath::Abs(fR1->fRfI.Im())>wsmall) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      //Module of k vector
      fR1->fAk = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR1->fVk = fR1->fAk*fR1->fUk;
      //phase of electric field
      fR1->fPE += (fR1->fPcaf + fR1->fPsaf)/deux;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fR1->fSdel.Im())>small) ||
        (TMath::Abs(fR1->fCdel.Im())>small)) {
          if (invers) isol[imin2] = kFALSE;
          else        isol[imin1] = kFALSE;
          continue;
      }
      fR1->fSdel.PureReal();
      fR1->fCdel.PureReal();
      fR1->fEf.Set(- fR1->fAE*fR1->fSdel.Re()*fR1->fWcaf,
        - fR1->fAE*fR1->fCdel.Re()*fR1->fWcaf,
        fR1->fAE*fR1->fWsaf);
      BuildBPUP(fR1->fVk,fR1->fEf,fR1->fBf,fR1->fPg,fR1->fUp,fPmui);
      //
      //Now comes the decisive test on solution chosen for fR1 : the reflective wave
      //must go backward ! If not, we have to try an other solution for the index.
      //But we tolerate a small numerical error.
      //
      dh1 = fR1->fUp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR1->fUp(0) = dh1;
      }
      if (dh1>zero) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      else Good1 = kTRUE;
      break;
    case 2:
      //2 phases. Parallel and normal component have to be calculated separatly.
      //
      //WE SHOULD NEVER HAVE COME HERE !!!
      //
      //  The splitting between parallel and normal components which occurs in
      //isotropic media when the electric field has 2 phases does not occur in
      //anisotropic media : it is replaced by a splitting into 2 different reflected
      //waves ! To be absolutly sure about that, we still leave this possibility
      //open to the program, but we put an error message if it happens !
      //
      PrintWhich();
      gMes->SetName("AniToIso");
      gMes->SetTitle("TPhoton");
      gMes->M(Error_M,17,"Electric field fR1->fEf has 2 phases");
      gMes->M(Info_M,17,"This should not happen in anisotropic media");
      //
      //Notice the - sign in front of fR1->fCtht due to the fact that the reflection
      //angle is defined contrary to the trigonometric sense. A reflection angle
      //EQUAL to the incidence angle implies a reflected wave having an OPPOSITE x
      //direction as the incident wave.
      //
      fR1->fUk.Set(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
      //The index of refraction of the 1st reflected wave cannot be complex in the
      //case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fR1->fRfI.Im())>wsmall) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      //Module of k vector
      fR1->fAk = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR1->fVk = fR1->fAk*fR1->fUk;
      //the angle delta of the decomposition of the electric fiel cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fR1->fSdel.Im())>small) ||
        (TMath::Abs(fR1->fCdel.Im())>small)) {
          if (invers) isol[imin2] = kFALSE;
          else        isol[imin1] = kFALSE;
          continue;
      }
      fR1->fSdel.PureReal();
      fR1->fCdel.PureReal();
      //electric field
      fR1->fEfp.Set(- fR1->fAE*fR1->fSdel.Re()*fR1->fWcaf,
        - fR1->fAE*fR1->fCdel.Re()*fR1->fWcaf,
        zero);
      fR1->fEfn.Set(zero,zero,fR1->fAE*fR1->fWsaf);
      BuildBPUP(fR1->fVk,fR1->fEfp,fR1->fBfp,fR1->fPgp,fR1->fUpp,fPmui);
      BuildBPUP(fR1->fVk,fR1->fEfn,fR1->fBfn,fR1->fPgn,fR1->fUpn,fPmui);
      //
      //Now comes the decisive test on solution chosen for fR1 : the reflective wave
      //must go backward ! If not, we have to try an other solution for the index.
      //But we tolerate a small numerical error.
      //
      dh1 = fR1->fUpp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR1->fUpp(0) = dh1;
      }
      dh2 = fR1->fUpn(0);
      if ((dh2>zero) && (dh2<wsmall)) {
        dh2 = - dh2;
        fR1->fUpn(0) = dh2;
      }
      if ((dh1>zero) || (dh2>zero)) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      else Good1 = kTRUE;
      break;
    default:
      //
      //The 1st reflected wave cannot be absorbed
      //
      if (invers) isol[imin2] = kFALSE;
      else        isol[imin1] = kFALSE;
      continue;
    }// end switch (fR1->fPhases)
    //
    //  Last calculations for wave fR2
    //
    // - module and phase of electric field :
    fR2->fE.RPhi(fR2->fAE,fR2->fPE);
    if (TMath::Abs(fR2->fPE)>wsmall) fR2->fPhases += 1;
    else fR2->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    fR2->fCalf.RPhi(fR2->fWcaf,fR2->fPcaf);
    fR2->fSalf.RPhi(fR2->fWsaf,fR2->fPsaf);
    if (TMath::Abs(fR2->fPcaf - fR2->fPsaf)>wsmall)  fR2->fPhases += 1;
    // - angle of refraction complex for the 2nd reflective wave ?
    if (TMath::Abs(fR2->fCtht.Im()) + TMath::Abs(fR2->fStht.Im())>wsmall)
      fR2->fPhases = 3;
    switch (fR2->fPhases) {
    case 0:
    case 1:
      //0 or 1 phase. No reason to choose between parallel and normal component.
      //Notice the - sign in front of fR2->fCtht due to the fact that the reflection
      //angle is defined contrary to the trigonometric sense. A reflection angle
      //EQUAL to the incidence angle implies a reflected wave having an OPPOSITE x
      //direction as the incident wave.
      fR2->fUk.Set(-fR2->fCtht.Re(),fR2->fStht.Re(),zero);
      //The index of refraction of the 2nd reflected wave cannot be complex in the
      //case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fR2->fRfI.Im())>wsmall) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      //Module of k vector
      fR2->fAk = (fW*fR2->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR2->fVk = fR2->fAk*fR2->fUk;
      //phase of electric field
      fR2->fPE += (fR2->fPcaf + fR2->fPsaf)/deux;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fR2->fSdel.Im())>small) ||
        (TMath::Abs(fR2->fCdel.Im())>small)) {
          if (invers) isol[imin1] = kFALSE;
          else        isol[imin2] = kFALSE;
          continue;
      }
      fR2->fSdel.PureReal();
      fR2->fCdel.PureReal();
      fR2->fEf.Set(- fR2->fAE*fR2->fSdel.Re()*fR2->fWcaf,
        - fR2->fAE*fR2->fCdel.Re()*fR2->fWcaf,
        fR2->fAE*fR2->fWsaf);
      BuildBPUP(fR2->fVk,fR2->fEf,fR2->fBf,fR2->fPg,fR2->fUp,fPmui);
      //
      //Now comes the decisive test on solution chosen for fR2 : the reflective wave
      //must go backward ! If not, we have to try an other solution for the index.
      //But we tolerate a small numerical error.
      //
      dh1 = fR2->fUp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR2->fUp(0) = dh1;
      }
      if (dh1>zero) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      else Good2 = kTRUE;
      break;
    case 2:
      //2 phases. Parallel and normal component have to be calculated separatly.
      //
      //WE SHOULD NEVER HAVE COME HERE !!!
      //
      //  The splitting between parallel and normal components which occurs in
      //isotropic media when the electric field has 2 phases does not occur in
      //anisotropic media : it is replaced by a splitting into 2 different reflected
      //waves ! To be absolutly sure about that, we still leave this possibility
      //open to the program, but we put an error message if it happens !
      //
      PrintWhich();
      gMes->SetName("AniToIso");
      gMes->SetTitle("TPhoton");
      gMes->M(Error_M,18,"Electric field fR2->fEf has 2 phases");
      gMes->M(Info_M,18,"This should not happen in anisotropic media");
      //
      //Notice the - sign in front of fR2->fCtht due to the fact that the reflection
      //angle is defined contrary to the trigonometric sense. A reflection angle
      //EQUAL to the incidence angle implies a reflected wave having an OPPOSITE x
      //direction as the incident wave.
      //
      fR2->fUk.Set(-fR2->fCtht.Re(),fR2->fStht.Re(),zero);
      //The index of refraction of the 1st reflected wave cannot be complex in the
      //case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fR2->fRfI.Im())>wsmall) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      //Module of k vector
      fR2->fAk = (fW*fR2->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR2->fVk = fR2->fAk*fR2->fUk;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fR2->fSdel.Im())>small) ||
        (TMath::Abs(fR2->fCdel.Im())>small)) {
          if (invers) isol[imin1] = kFALSE;
          else        isol[imin2] = kFALSE;
          continue;
      }
      fR2->fSdel.PureReal();
      fR2->fCdel.PureReal();
      //electric field
      fR2->fEfp.Set(- fR2->fAE*fR2->fSdel.Re()*fR2->fWcaf,
        - fR2->fAE*fR2->fCdel.Re()*fR2->fWcaf,
        zero);
      fR2->fEfn.Set(zero,zero,fR2->fAE*fR2->fWsaf);
      BuildBPUP(fR2->fVk,fR2->fEfp,fR2->fBfp,fR2->fPgp,fR2->fUpp,fPmui);
      BuildBPUP(fR2->fVk,fR2->fEfn,fR2->fBfn,fR2->fPgn,fR2->fUpn,fPmui);
      //
      //Now comes the decisive test on solution chosen for fR2 : the reflective wave
      //must go backward ! If not, we have to try an other solution for the index.
      //But we tolerate a small numerical error.
      //
      dh1 = fR2->fUpp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR2->fUpp(0) = dh1;
      }
      dh2 = fR2->fUpn(0);
      if ((dh2>zero) && (dh2<wsmall)) {
        dh2 = - dh2;
        fR2->fUpn(0) = dh2;
      }
      if ((dh1>zero) || (dh2>zero)) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      else Good2 = kTRUE;
      break;
    default:
      //
      //The 2nd reflected CAN be absorbed, but if it is the case, we have nothing to
      //compute !
      //
      Good2 = kTRUE;
      break;
    }// end switch (fR2->fPhases)
    //
    //  Last calculations for wave fT1
    //
    //Transmitted wave is in isotropic medium, so that angle delta = theta
    fT1->fSdel = fT1->fStht;
    fT1->fCdel = fT1->fCtht;
    // - module and phase of electric field :
    fT1->fE.RPhi(fT1->fAE,fT1->fPE);
    if (TMath::Abs(fT1->fPE)>wsmall) fT1->fPhases += 1;
    else fT1->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    fT1->fCalf.RPhi(fT1->fWcaf,fT1->fPcaf);
    fT1->fSalf.RPhi(fT1->fWsaf,fT1->fPsaf);
    if (TMath::Abs(fT1->fPcaf-fT1->fPsaf)>wsmall) fT1->fPhases +=  1;
    // - angle of refraction complex for the 1st transmitted wave ?
    TestIm = TMath::Abs(fT1->fCtht.Im()) + TMath::Abs(fT1->fStht.Im()) +
      TMath::Abs(fT1->fRfI.Im());
    if (TestIm>wsmall) fT1->fPhases = 3;
    switch (fT1->fPhases) {
    case 0:
    case 1:
      //Here either no phase, or one global phase. Parallel and normal components
      //of the wave have the same phase. No reason to distinguish between the 2.
      //The transmitted wave is not elliptically polarized.
      fT1->fUk.Set(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
      //If the phases are 0,1 or 2 it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) {
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,19,"Index unduly complex",fT1->fRfI);
        fAnomaly = kTRUE;
        return;
      }
      fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
      fT1->fVk  = fT1->fAk*fT1->fUk;
      fT1->fPE += (fT1->fPcaf + fT1->fPsaf)/deux;
      fT1->fEf.Set(- fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fWsaf);
      BuildBPUP(fT1->fVk,fT1->fEf,fT1->fBf,fT1->fPg,fT1->fUp,fPmut);
      //Verify that the transmitted wave goes forward, up to a very small numerical
      //tolerance
      dh1 = fT1->fUp(0);
      if ((dh1<zero) && (dh1>-wsmall)) {
        dh1 = - dh1;
        fT1->fUp(0) = dh1;
      }
      if (dh1<zero) {
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,20,"transmitted wave going backward",dh1);
        fAnomaly = kTRUE;
        return;
      }
      //Verify that k vector and electric field are orthogonal : isotropic medium !
      if (gMes->TestCalc()) {
        vh1 = fT1->fEf;
        vh1.Unit();
        dh1 = TMath::Abs(vh1*fT1->fUk);
        if (dh1>small) {
          PrintWhich();
          gMes->SetName("AniToIso");
          gMes->SetTitle("TPhoton");
          gMes->M(kill_this,21,"k and E not orthogonal",dh1);
          fAnomaly = kTRUE;
          return;
        }
      }
      break;
    case 2:
      //  Here, there are 2 phases. Parallel and normal component have different phases,
      //so that they have to be calculated separatly. Later on, we will have to choose
      //between these 2 waves.
      fT1->fUk.Set(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
      //If the phases are 0,1 or 2 it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) {
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,22,"Index unduly complex",fT1->fRfI);
        fAnomaly = kTRUE;
        return;
      }
      fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
      fT1->fVk  = fT1->fAk*fT1->fUk;
      //electric field
      fT1->fEfp.Set(-fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
        zero);
      fT1->fEfn.Set(zero,zero,fT1->fAE*fT1->fWsaf);
      BuildBPUP(fT1->fVk,fT1->fEfp,fT1->fBfp,fT1->fPgp,fT1->fUpp,fPmut);
      BuildBPUP(fT1->fVk,fT1->fEfn,fT1->fBfn,fT1->fPgn,fT1->fUpn,fPmut);
      //Verify that the transmitted waves go forward, up to a very small numerical
      //tolerance
      dh1 = fT1->fUpp(0);
      dh2 = fT1->fUpn(0);
      if ((dh1<zero) && (dh1>-wsmall)) {
        dh1 = - dh1;
        fT1->fUpp(0) = dh1;
      }
      if ((dh2<zero) && (dh2>-wsmall)) {
        dh2 = - dh2;
        fT1->fUpn(0) = dh2;
      }
      if ((dh1<zero) || (dh2<zero)) {
        PrintWhich();
        gMes->SetName("AniToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,23,"transmitted wave going backward",dh1,dh2);
        fAnomaly = kTRUE;
        return;
      }
      //Verify that k vector and electric field are orthogonal : isotropic medium !
      if (gMes->TestCalc()) {
        vh1 = fT1->fEfp;
        vh1.Unit();
        dh1 = TMath::Abs(vh1*fT1->fUk);
        vh1 = fT1->fEfn;
        vh1.Unit();
        dh2 = TMath::Abs(vh1*fT1->fUk);
        if ((dh1>small) || (dh2>small)) {
          PrintWhich();
          gMes->SetName("AniToIso");
          gMes->SetTitle("TPhoton");
          gMes->M(kill_this,24,"k and E not orthogonal",dh1,dh2);
          fAnomaly = kTRUE;
          return;
        }
      }
      break;
    default:
      //  Here there are 3 phases so that the wave is absorbed. No reason to calculate
      //anything
      break;
    }//end switch (fT1->fPhases)
  }// end do while
  while (((!Good1) || (!Good2)) && (NumberOfTry<6));
  //Verify that the system of 13 equations has been solved properly
  if (gMes->TestCalc()) VerifyAtoI();
  if (fAnomaly) return;
  //
  //  After all these complicate calculations, we have at hand a powerful check
  //that everything is ok : we can verify that the flow of energy across the
  //boundary between the 2 media is conserved! Notice that if the transmitted
  //wave is absorbed because of a complex index of refraction of the "out" medium
  //then the energy flux is not conserved ! But if the transmitted wave is 
  //absorbed because of total reflection on an "out" medium with a real index
  //of refraction, then the energy flux is conserved !
  //
  //  We HAVE to do this test in any case, since a non-conservation of flux
  //may be the sign that we have taken a bad pair of solutions and that we
  //have to try an other one. ( We leave the "if ( 1 || gMes->TestCalc())"
  //in the case that it appears experimentally that it never happens. In
  //that case, we could render the test optional. )
  //
  if ( 1 /* gMes->TestCalc() */ ) {
    gMes->SetName("AniToIso");
    gMes->SetTitle("TPhoton");
    Double_t influx,outflux,dflux;
    influx = fI->fPg(0);
    switch (fR1->fPhases) {
    case 0:
    case 1:
      switch (fR2->fPhases) {
    case 0:
    case 1:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 1  fR2 : 1  fT1 : 1
      outflux = fT1->fPg(0) - fR1->fPg(0) - fR2->fPg(0);
      break;
    case 2:
      // fR1 : 1  fR2 : 1  fT1 : 2
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPg(0) - fR2->fPg(0);
      break;
    default:
      // fR1 : 1  fR2 : 1  fT1 : 3
      //Transmitted wave is absorbed.
      //Transmitted wave may be absorbed :
      //  (1) - because of total reflection. In that case the refraction index
      //        of the outer material is real and the energy is conserved.
      //  (2) - because of reflection on metallic surface. In that case the
      //        refraction index of the outer material is complex and the
      //        energy is not conserved ( the absorbed wave takes energy )
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else  outflux = - fR1->fPg(0) - fR2->fPg(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    case 2:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 1  fR2 : 2  fT1 : 1   !!! theoretically impossible  !!!
      outflux = fT1->fPg(0) - fR1->fPg(0) - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    case 2:
      // fR1 : 1  fR2 : 2  fT1 : 2   !!! theoretically impossible  !!!
      outflux =   fT1->fPgn(0) + fT1->fPgp(0)
        - fR1->fPg(0) - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    default:
      // fR1 : 1  fR2 : 2  fT1 : 3   !!! theoretically impossible  !!!
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR1->fPg(0) - fR2->fPgn(0) - fR2->fPgp(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    default:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 1  fR2 : 3  fT1 : 1
      outflux = fT1->fPg(0) - fR1->fPg(0);
      break;
    case 2:
      // fR1 : 1  fR2 : 3  fT1 : 2
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPg(0);
      break;
    default:
      // fR1 : 1  fR2 : 3  fT1 : 3
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR1->fPg(0);
      break;
      }// end switch (fT1->fPhases)
      break;
      }// end switch (fR2->fPhases)
      break;
    case 2:
      switch (fR2->fPhases) {
    case 0:
    case 1:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 2  fR2 : 1  fT1 : 1   !!! theoretically impossible  !!!
      outflux = fT1->fPg(0) - fR1->fPgn(0) - fR1->fPgp(0) - fR2->fPg(0);
      break;
    case 2:
      // fR1 : 2  fR2 : 1  fT1 : 2   !!! theoretically impossible  !!!
      outflux =   fT1->fPgn(0) + fT1->fPgp(0)
        - fR1->fPgn(0) - fR1->fPgp(0) - fR2->fPg(0);
      break;
    default:
      // fR1 : 2  fR2 : 1  fT1 : 3   !!! theoretically impossible  !!!
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR1->fPgn(0) - fR1->fPgp(0) - fR2->fPg(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    case 2:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 2  fR2 : 2  fT1 : 1   !!! theoretically impossible  !!!
      outflux =   fT1->fPg(0)  - fR1->fPgn(0) - fR1->fPgp(0)
        - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    case 2:
      // fR1 : 2  fR2 : 2  fT1 : 2   !!! theoretically impossible  !!!
      outflux =   fT1->fPgn(0) + fT1->fPgp(0)
        - fR1->fPgn(0) - fR1->fPgp(0)
        - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    default:
      // fR1 : 2  fR2 : 2  fT1 : 3   !!! theoretically impossible  !!!
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR1->fPgn(0) - fR1->fPgp(0)
        - fR2->fPgn(0) - fR2->fPgp(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    default:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 2  fR2 : 3  fT1 : 1   !!! theoretically impossible  !!!
      outflux = fT1->fPg(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    case 2:
      // fR1 : 2  fR2 : 3  fT1 : 2   !!! theoretically impossible  !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    default:
      // fR1 : 2  fR2 : 3  fT1 : 3   !!! theoretically impossible  !!!
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR1->fPgn(0) - fR1->fPgp(0);
      break;
      }// end switch (fT1->fPhases)
      break;
      }// end switch (fR2->fPhases)
      break;
    default:
      switch (fR2->fPhases) {
    case 0:
    case 1:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 3  fR2 : 1  fT1 : 1   !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gMes->M(Error_M,25,"fR1 and fR2 out of order");
      outflux = fT1->fPg(0) - fR2->fPg(0);
      break;
    case 2:
      // fR1 : 3  fR2 : 1  fT1 : 2   !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gMes->M(Error_M,26,"fR1 and fR2 out of order");
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR2->fPg(0);
      break;
    default:
      // fR1 : 3  fR2 : 1  fT1 : 3   !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gMes->M(Error_M,27,"fR1 and fR2 out of order");
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR2->fPg(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    case 2:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 3  fR2 : 2  fT1 : 1   !!! theoretically impossible  !!!
      //                             !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gMes->M(Error_M,28,"fR1 and fR2 out of order");
      gMes->M(Info_M,28,"solution theoretically impossible");
      outflux = fT1->fPg(0) - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    case 2:
      // fR1 : 3  fR2 : 2  fT1 : 2   !!! theoretically impossible  !!!
      //                             !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gMes->M(Error_M,29,"fR1 and fR2 out of order");
      gMes->M(Info_M,29,"solution theoretically impossible");
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR2->fPgn(0) - fR2->fPgp(0);
      break;
    default:
      // fR1 : 3  fR2 : 2  fT1 : 3   !!! theoretically impossible  !!!
      //                             !!! fR1 and fR2 out of order  !!!
      PrintWhich();
      gMes->M(Error_M,30,"fR1 and fR2 out of order");
      gMes->M(Info_M,30,"solution theoretically impossible");
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = - fR2->fPgn(0) - fR2->fPgp(0);
      break;
      }// end switch (fT1->fPhases)
      break;
    default:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      // fR1 : 3  fR2 : 3  fT1 : 1
      PrintWhich();
      gMes->M(Error_M,31,"all reflected waves absorbed");
      outflux = fT1->fPg(0);
      break;
    case 2:
      // fR1 : 3  fR2 : 3  fT1 : 2
      PrintWhich();
      gMes->M(Error_M,32,"all reflected waves absorbed");
      outflux = fT1->fPgn(0) + fT1->fPgp(0);
      break;
    default:
      // fR1 : 3  fR2 : 3  fT1 : 3
      PrintWhich();
      gMes->M(Error_M,33,"all reflected waves absorbed");
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) outflux = influx;
      else outflux = zero;
      break;
      }// end switch (fT1->fPhases)
      break;
      }// end switch (fR2->fPhases)
      break;
    }// end switch (fR1->fPhases)
    dflux = influx - outflux;
    if (TMath::Abs(dflux)>dixm4) {
      PrintWhich();
      gMes->M(Error_M,34,"flux not conserved",dflux);
      gMes->M(Info_M,34,"This may be due to a bad choice of solutions from TEqIndex");
      gMes->M(Info_M,34,"We try an other choice");
      // Correction of Alessandro Thea. Instead of:
      //   goto bigloop;
      if (NumberOfTry<7) goto bigloop;
      else {
        gMes->M(kill_this,30,"all solutions tried without success",fKphot);
        fAnomaly = kTRUE;
        return;
      }
      // End Correction of Alessandro Thea.
    }
  }//end if (1)
}
void TPhoton::APDDet() {
  //Handles the case of a photon having stopped inside an APD
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  const Double_t dixp7 = 1.0e+7;
  const Double_t dixm2 = 0.01;
  Int_t bin;
  //Finds the number of this APD
  Int_t kAPD;         //APD number
  kAPD = fS->GetDetNumber();
  //Finds pointer towards histograms of this APD
  TStatAPD *pstapdg;
  TStatAPD *pstapdc = 0;
  pstapdg = (TStatAPD *)gGs->fAPD[kAPD];
  if (gCs)
    pstapdc = (TStatAPD *)gCs->fAPD[kAPD];
  //Finds the distance of stop point of photon from the cathode and anode
  T3Vector FromCathode;
  Double_t DistFromCath;
  Double_t DistFromAn;
  FromCathode = fX0 - fS->fx0APD;
  DistFromCath = FromCathode*fS->fAxisAPD;
  DistFromCath *= dixp7;
  if (DistFromCath<zero) {
    PrintWhich();
    gMes->SetName("APDDet");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_prog,1,"distance to cathode negativ",DistFromCath);
  }
  DistFromAn = fS->GetDistCatAn() - DistFromCath;
  bin = pstapdg->fHdCathode->Fill(DistFromCath);
  if (gCs) bin = pstapdc->fHdCathode->Fill(DistFromCath);
  //Finds the number of electron-hole pairs produced
  Double_t NbOfPairs;
  TSplineFit *pGainProfile;
  pGainProfile = fS->GetGainProfile();
  NbOfPairs = pGainProfile->V(DistFromCath);
  pstapdg->fNelectrons += Int_t(NbOfPairs);
  bin = pstapdg->fHeVersusT->Fill(fT0,NbOfPairs);
  bin = pstapdg->fHeVersusW->Fill(fWvlgth,NbOfPairs);
  bin = pstapdg->fHInside->Fill(fWvlgth);
  if (NbOfPairs>=un) bin = pstapdg->fHQEff->Fill(fWvlgth);
  if (gCs) {
    pstapdc->fNelectrons += Int_t(NbOfPairs);
    bin = pstapdc->fHeVersusT->Fill(fT0,NbOfPairs);
    bin = pstapdc->fHeVersusW->Fill(fWvlgth,NbOfPairs);
    bin = pstapdc->fHInside->Fill(fWvlgth);
    if (NbOfPairs>=un) bin = pstapdc->fHQEff->Fill(fWvlgth);
  }
  //Is this photon seen ? Is the number of pairs produced considered as enough ?
  if (NbOfPairs>= TLitPhys::Get()->Threshe()) {
    //Here the number of pairs produced is enough to decide that photon is seen
    Double_t TimeElec;  //time [ps] taken by electrons from fX0 to the anode
    Double_t PulseTime; //time at which electrons arrive at anode
    Double_t vdrift;    //drift velocity of electrons in [nm/ps]
    vdrift = dixm2*fS->GetVDrifte();
    TimeElec = DistFromAn/vdrift;
    PulseTime = fT0 + TimeElec;
    if (PulseTime<=TLitPhys::Get()->TooLate()) {
      //Here the electrons arrive in time at anode
      fPhtSeen = kTRUE;
      gGs->fNpSeen += 1;
      bin = gGs->fHTimeSeen->Fill(PulseTime);
      bin = gGs->fHDistSeen->Fill(fSl0);
      bin = gGs->fHWvlgthSeen->Fill(fWvlgth);
      pstapdg->fNpSeen += 1;
      bin = pstapdg->fHTimeSeen->Fill(PulseTime);
      bin = pstapdg->fHWvlgthSeen->Fill(fWvlgth);
      if (gCs) {
        gCs->fNpSeen += 1;
        bin = gCs->fHTimeSeen->Fill(PulseTime);
        bin = gCs->fHDistSeen->Fill(fSl0);
        bin = gCs->fHWvlgthSeen->Fill(fWvlgth);
        pstapdc->fNpSeen += 1;
        bin = pstapdc->fHTimeSeen->Fill(PulseTime);
        bin = pstapdc->fHWvlgthSeen->Fill(fWvlgth);
      }
    }
    else {
      //Here the electrons do not arrive in time at anode
      gGs->fNpLossAny += 1;
      gGs->fNpTooLate += 1;
      bin = gGs->fHAbsAny->Fill(fSl0);
      bin = gGs->fHTooLate->Fill(fWvlgth);
      pstapdg->fNpNotSeen += 1;
      bin = pstapdg->fHWvlgthNot->Fill(fWvlgth);
      if (gCs) {
        gCs->fNpLossAny += 1;
        gCs->fNpTooLate += 1;
        bin = gCs->fHAbsAny->Fill(fSl0);
        bin = gCs->fHTooLate->Fill(fWvlgth);
        pstapdc->fNpNotSeen += 1;
        bin = pstapdc->fHWvlgthNot->Fill(fWvlgth);
      }
    }
  }
  else {
    //Here the number of pairs produced is not enough to decide that photon is seen
    gGs->fNpLossAny += 1;
    gGs->fNpTooFew  += 1;
    bin = gGs->fHAbsAny->Fill(fSl0);
    pstapdg->fNpNotSeen += 1;
    bin = pstapdg->fHWvlgthNot->Fill(fWvlgth);
    if (gCs) {
      gCs->fNpLossAny += 1;
      gCs->fNpTooFew  += 1;
      bin = gCs->fHAbsAny->Fill(fSl0);
      pstapdc->fNpNotSeen += 1;
      bin = pstapdc->fHWvlgthNot->Fill(fWvlgth);
    }
  }
}
void TPhoton::BuildBPUP(const T3Vector &k,const T3Vector &E,T3Vector &B,T3Vector &P,T3Vector &uP,Double_t mu) const {
  //when the k vector and the E field vector are defined, calculates B field,
  //the Poyinting vector and unit Poyinting vector. The calculations are
  //the same in the isotropic and anisotropic cases.
  //The conversion factor conv is due to the fact that the k vector is in [cm-1]
  //instead of [m-1] and that the frequency fW is in [ps-1] instead of [s-1]
  const Double_t conv   = 1.0e-10;
  const Double_t wsmall = 1.0e-300;
  Double_t y;
  B  = k^E;
  y  = conv/fW;
  B  = y*B;
  y  = (TLitPhys::Get()->Eps0()*TLitPhys::Get()->Cmks()*TLitPhys::Get()->Cmks())/mu;
  P  = E^B;
  P  = y*P;
  uP = P;
  y  = uP.Norm2Sqr();
  if (y>wsmall) uP.Unit();
}
void TPhoton::ch1111() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1111");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1112() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPgn(0));
  fWeigth[4] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1112");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1121() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1121");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1113() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<3;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1113");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 3:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1113");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1114() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1114");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1122() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1122");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1123() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<4;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1123");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 4:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1123");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1124() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1124");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1131() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<4;k++) 
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1131");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1131");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1132() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<5;k++) 
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1132");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1132");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1133() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  for (k=0;k<2;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since waves are absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1133");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1133");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1141() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1141");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1142() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  fWeigth[2] = TMath::Abs(fT2->fPgn(0));
  fWeigth[3] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1142");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1144() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPg(0));
  kwhich = Toss(2);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1144");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1211() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1211");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1212() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1212");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1213() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<4;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1213");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1213");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1214() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1214");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1221() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1221");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1222() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPgn(0));
  fWeigth[6] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 6:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1222");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1223() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<5;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1223");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[5] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR1->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR1->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1223");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1224() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1224");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1231() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<5;k++) 
    if (k!=3) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1231");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1231");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1232() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<6;k++) 
    if (k!=3) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1232");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1232");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1233() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  for (k=0;k<3;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since waves are absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1233");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1233");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1241() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1241");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1242() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  fWeigth[3] = TMath::Abs(fT2->fPgn(0));
  fWeigth[4] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1242");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1244() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[1] = TMath::Abs(fT1->fPgn(0));
  fWeigth[2] = TMath::Abs(fT1->fPgp(0));
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1244");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1311() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<4;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1311");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1311");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1312() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPgn(0));
  fWeigth[4] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<5;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1312");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1312");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1313() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<3;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1313");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1313");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1321() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<5;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1321");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1321");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1322() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<6;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1322");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1322");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1323() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fR2->fPgn(0));
  fWeigth[3] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<4;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1323");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 3:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1323");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1331() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<3;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1331");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1331");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1332() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  fWeigth[2] = TMath::Abs(fT2->fPgn(0));
  fWeigth[3] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<4;k++)
    if (k!=1) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1332");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 2:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1332");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch1333() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPg(0));
  outflux = fWeigth[0];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since waves are absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch1333");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[1] = dE;
  kwhich = Toss(2);
  switch (kwhich) {
  case 0:
    fR1->fShare = Taken;
    fBeta  += fR1->fPE;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEf;
    break;
  case 1:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch1333");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2111() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2111");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2112() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2112");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2113() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<4;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2113");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2113");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2114() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2114");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2121() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2121");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2122() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPgn(0));
  fWeigth[6] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 6:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2122");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2123() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<5;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2123");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[5] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2123");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2124() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2124");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2131() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<5;k++) 
    if (k!=3) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2131");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2131");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2132() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<6;k++) 
    if (k!=3) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2132");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2132");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2133() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  for (k=0;k<3;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since waves are absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2133");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[3] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2133");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2141() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2141");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2142() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  fWeigth[3] = TMath::Abs(fT2->fPgn(0));
  fWeigth[4] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2142");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2144() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPg(0));
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fBeta  += fT1->fPE;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2144");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2211() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPg(0));
  fWeigth[5] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 5:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2211");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2212() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPg(0));
  fWeigth[5] = TMath::Abs(fT2->fPgn(0));
  fWeigth[6] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 5:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 6:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2212");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2213() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<5;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2213");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[5] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 5:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2213");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2214() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPg(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2214");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2221() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPgn(0));
  fWeigth[5] = TMath::Abs(fR2->fPgp(0));
  fWeigth[6] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 5:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 6:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2221");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2222() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPgn(0));
  fWeigth[5] = TMath::Abs(fR2->fPgp(0));
  fWeigth[6] = TMath::Abs(fT2->fPgn(0));
  fWeigth[7] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(8);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 5:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 6:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 7:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2222");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2223() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPgn(0));
  fWeigth[5] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<6;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2223");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[6] = dE;
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 5:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 6:
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2223");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2224() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fR2->fPgn(0));
  fWeigth[5] = TMath::Abs(fR2->fPgp(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 5:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2224");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2231() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<6;k++) 
    if (k!=4) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2231");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2231");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2232() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPgn(0));
  fWeigth[6] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<7;k++) 
    if (k!=4) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2232");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 6:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2232");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2233() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  for (k=0;k<4;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since waves are absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2233");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[4] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2233");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2241() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2241");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2242() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2242");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2244() {
  Short_t kwhich;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[2] = TMath::Abs(fT1->fPgn(0));
  fWeigth[3] = TMath::Abs(fT1->fPgp(0));
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = NormalTaken;
    fBeta  += fT1->fPE + fT1->fPsaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfn;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT1->fShare = ParallelTaken;
    fBeta  += fT1->fPE + fT1->fPcaf;
    fAk0    = fT1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT1->fUk;
    fI->fVk = fT1->fVk;
    fI->fEf = fT1->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2244");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2311() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<5;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2311");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2311");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2312() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  fWeigth[4] = TMath::Abs(fT2->fPgn(0));
  fWeigth[5] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<6;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2312");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  case 4:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 5:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2312");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2313() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPg(0));
  for (k=0;k<4;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2313");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = Taken;
    fBeta  += fR2->fPE;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEf;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2313");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2321() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<6;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2321");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(6);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2321");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2322() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  fWeigth[5] = TMath::Abs(fT2->fPgn(0));
  fWeigth[6] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<7;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2322");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(7);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  case 5:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 6:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2322");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2323() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fR2->fPgn(0));
  fWeigth[4] = TMath::Abs(fR2->fPgp(0));
  for (k=0;k<5;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2323");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fR2->fShare = NormalTaken;
    fBeta  += fR2->fPE + fR2->fPsaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfn;
    break;
  case 4:
    fR2->fShare = ParallelTaken;
    fBeta  += fR2->fPE + fR2->fPcaf;
    fAk0    = fR2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR2->fUk;
    fI->fVk = fR2->fVk;
    fI->fEf = fR2->fEfp;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2323");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2331() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fT2->fPg(0));
  for (k=0;k<4;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2331");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(4);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = Taken;
    fBeta  += fT2->fPE;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEf;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2331");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2332() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  fWeigth[3] = TMath::Abs(fT2->fPgn(0));
  fWeigth[4] = TMath::Abs(fT2->fPgp(0));
  for (k=0;k<5;k++)
    if (k!=2) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2332");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(5);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  case 3:
    fT2->fShare = NormalTaken;
    fBeta  += fT2->fPE + fT2->fPsaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfn;
    fPhtOut = kTRUE;
    break;
  case 4:
    fT2->fShare = ParallelTaken;
    fBeta  += fT2->fPE + fT2->fPcaf;
    fAk0    = fT2->fAk;
    fI->fAk = fAk0;
    fI->fUk = fT2->fUk;
    fI->fVk = fT2->fVk;
    fI->fEf = fT2->fEfp;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2332");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::ch2333() {
  const Double_t zero   = 0.0;
  const Double_t msmall = -1.0e-4;
  Short_t k,kwhich;
  Double_t dE;             // = influx - outflux
  Double_t outflux = zero;
  fPhtAbs = kFALSE;
  fPhtOut = kFALSE;
  fWeigth[0] = TMath::Abs(fR1->fPgn(0));
  fWeigth[1] = TMath::Abs(fR1->fPgp(0));
  for (k=0;k<2;k++) outflux += fWeigth[k];
  dE = TMath::Abs(fI->fPg(0)) - outflux;
  //Since a wave is absorbed, it is not impossible that the flux of energy be
  //not conserved. But dE can never be negativ ! It would mean energy creation !
  if (dE<msmall) {
    PrintWhich();
    gMes->SetName("ch2333");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Energy creation ?");
    dE = zero;
  }
  if (dE<zero) dE = zero;
  fWeigth[2] = dE;
  kwhich = Toss(3);
  switch (kwhich) {
  case 0:
    fR1->fShare = NormalTaken;
    fBeta  += fR1->fPE + fR1->fPsaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfn;
    break;
  case 1:
    fR1->fShare = ParallelTaken;
    fBeta  += fR1->fPE + fR1->fPcaf;
    fAk0    = fR1->fAk;
    fI->fAk = fAk0;
    fI->fUk = fR1->fUk;
    fI->fVk = fR1->fVk;
    fI->fEf = fR1->fEfp;
    break;
  case 2:
    fT1->fShare = Taken;
    fR2->fShare = Taken;
    fT2->fShare = Taken;
    fPhtAbs = kTRUE;
    fPhtOut = kTRUE;
    break;
  default:
    PrintWhich();
    gMes->SetName("ch2333");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,1,"Impossible choice");
    break;
  }
}
void TPhoton::Choose() {
  //  Among all possible waves, chooses the one to take according to weights
  //given by the fluxes of energy across the x=0 plane. This is given by the
  //x component of the Poyinting vector of each wave. The maximum possible
  //number of waves is 8 ! ( yes 8 ! ). An absorbed wave may be chosen (for
  //instance transmitted wave against a revetment with complex index). In that
  //case the photon dies.
  //
  //  In fact, this is not true that there are 8 possible waves. It would be
  //true in the case where the possibility would exist, in anisotropic media,
  //that the parallel and the normal components of a wave has to have to be
  //splitted, a case that we have decided to leave open in the program ( in
  //order to really check that it does not happen !). But in fact, it does not
  //happen, so that the maximum number of waves is 4.
  //
  gMes->SetName("Choose");
  gMes->SetTitle("TPhoton");
  if (fR1->fShare == NotInvolved) {
    gMes->M(Error_M,1,"No reflected wave in process !");
  }
  if (fT1->fShare == NotInvolved) {
    gMes->M(Error_M,2,"No transmitted wave in process !");
  }
  if (fR2->fShare == NotInvolved) fR2->fPhases = 4;
  if (fT2->fShare == NotInvolved) fT2->fPhases = 4;
  switch (fR1->fPhases) {
  case 0:
  case 1:
    // fR1 : 1
    switch (fT1->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 1
    switch (fR2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 1 | fR2 : 1
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 1 | fR2 : 1 | fT2 : 1
    ch1111();
    break;
  case 2:
    // fR1 : 1 | fT1 : 1 | fR2 : 1 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,1112,"Should not happen");
    ch1112();
    break;
  case 3:
    // fR1 : 1 | fT1 : 1 | fR2 : 1 | fT2 : 3
    ch1113();
    break;
  default:
    // fR1 : 1 | fT1 : 1 | fR2 : 1 | fT2 : N
    ch1114();
    break;
    }//end switch (fT2->fPhases)
    break;
  case 2:
    // fR1 : 1 | fT1 : 1 | fR2 : 2
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 1 | fR2 : 2 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,1121,"Should not happen");
    ch1121();
    break;
  case 2:
    // fR1 : 1 | fT1 : 1 | fR2 : 2 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,1122,"Should not happen");
    ch1122();
    break;
  case 3:
    // fR1 : 1 | fT1 : 1 | fR2 : 2 | fT2 : 3
    PrintWhich();
    gMes->M(Error_M,1123,"Should not happen");
    ch1123();
    break;
  default:
    // fR1 : 1 | fT1 : 1 | fR2 : 2 | fT2 : N
    PrintWhich();
    gMes->M(Error_M,1124,"Should not happen");
    ch1124();
    break;
    }//end switch (fT2->fPhases)
    break;
  case 3:
    // fR1 : 1 | fT1 : 1 | fR2 : 3
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 1 | fR2 : 3 | fT2 : 1
    ch1131();
    break;
  case 2:
    // fR1 : 1 | fT1 : 1 | fR2 : 3 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,1132,"Should not happen");
    ch1132();
    break;
  case 3:
    // fR1 : 1 | fT1 : 1 | fR2 : 3 | fT2 : 3
    ch1133();
    break;
  default:
    // fR1 : 1 | fT1 : 1 | fR2 : 3 | fT2 : N
    ch1133();
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
  default:
    // fR1 : 1 | fT1 : 1 | fR2 : N
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 1 | fR2 : N | fT2 : 1
    ch1141();
    break;
  case 2:
    // fR1 : 1 | fT1 : 1 | fR2 : N | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,1142,"Should not happen");
    ch1142();
    break;
  case 3:
    // fR1 : 1 | fT1 : 1 | fR2 : N | fT2 : 3
    ch1133();
    fR2->fShare = NotInvolved;
    break;
  default:
    // fR1 : 1 | fT1 : 1 | fR2 : N | fT2 : N
    ch1144();
    break;
    }//end switch (fT2->fPhases)
    break;
    }//end switch (fR2->fPhases)
    break;
  case 2:
    // fR1 : 1 | fT1 : 2
    switch (fR2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 2 | fR2 : 1
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 2 | fR2 : 1 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,1211,"Should not happen");
    ch1211();
    break;
  case 2:
    // fR1 : 1 | fT1 : 2 | fR2 : 1 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,1212,"Should not happen");
    ch1212();
    break;
  case 3:
    // fR1 : 1 | fT1 : 2 | fR2 : 1 | fT2 : 3
    ch1213();
    break;
  default:
    // fR1 : 1 | fT1 : 2 | fR2 : 1 | fT2 : N
    ch1214();
    break;
    }//end switch (fT2->fPhases)
    break;
  case 2:
    // fR1 : 1 | fT1 : 2 | fR2 : 2
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 2 | fR2 : 2 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,1221,"Should not happen");
    ch1221();
    break;
  case 2:
    // fR1 : 1 | fT1 : 2 | fR2 : 2 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,1222,"Should not happen");
    ch1222();
    break;
  case 3:
    // fR1 : 1 | fT1 : 2 | fR2 : 2 | fT2 : 3
    PrintWhich();
    gMes->M(Error_M,1223,"Should not happen");
    ch1223();
    break;
  default:
    // fR1 : 1 | fT1 : 2 | fR2 : 2 | fT2 : N
    PrintWhich();
    gMes->M(Error_M,1224,"Should not happen");
    ch1224();
    break;
    }//end switch (fT2->fPhases)
    break;
  case 3:
    // fR1 : 1 | fT1 : 2 | fR2 : 3
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 2 | fR2 : 3 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,1231,"Should not happen");
    ch1231();
    break;
  case 2:
    // fR1 : 1 | fT1 : 2 | fR2 : 3 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,1232,"Should not happen");
    ch1232();
    break;
  case 3:
    // fR1 : 1 | fT1 : 2 | fR2 : 3 | fT2 : 3
    ch1233();
    break;
  default:
    // fR1 : 1 | fT1 : 2 | fR2 : 3 | fT2 : N
    ch1233();
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
  default:
    // fR1 : 1 | fT1 : 2 | fR2 : N
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 2 | fR2 : N | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,1241,"Should not happen");
    ch1241();
    break;
  case 2:
    // fR1 : 1 | fT1 : 2 | fR2 : N | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,1242,"Should not happen");
    ch1242();
    break;
  case 3:
    // fR1 : 1 | fT1 : 2 | fR2 : N | fT2 : 3
    ch1233();
    fR2->fShare = NotInvolved;
    break;
  default:
    // fR1 : 1 | fT1 : 2 | fR2 : N | fT2 : N
    ch1244();
    break;
    }//end switch (fT2->fPhases)
    break;
    }//end switch (fR2->fPhases)
    break;
  default:
    // fR1 : 1 | fT1 : 3
    switch (fR2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 3 | fR2 : 1
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 3 | fR2 : 1 | fT2 : 1
    ch1311();
    break;
  case 2:
    // fR1 : 1 | fT1 : 3 | fR2 : 1 | fT2 : 2
    ch1312();
    break;
  case 3:
    // fR1 : 1 | fT1 : 3 | fR2 : 1 | fT2 : 3
    ch1313();
    break;
  default:
    // fR1 : 1 | fT1 : 3 | fR2 : 1 | fT2 : N
    ch1313();
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
  case 2:
    // fR1 : 1 | fT1 : 3 | fR2 : 2
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 3 | fR2 : 2 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,1321,"Should not happen");
    ch1321();
    break;
  case 2:
    // fR1 : 1 | fT1 : 3 | fR2 : 2 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,1322,"Should not happen");
    ch1322();
    break;
  case 3:
    // fR1 : 1 | fT1 : 3 | fR2 : 2 | fT2 : 3
    PrintWhich();
    gMes->M(Error_M,1323,"Should not happen");
    ch1323();
    break;
  default:
    // fR1 : 1 | fT1 : 3 | fR2 : 2 | fT2 : N
    PrintWhich();
    gMes->M(Error_M,1324,"Should not happen");
    ch1323();
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
  case 3:
    // fR1 : 1 | fT1 : 3 | fR2 : 3
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 3 | fR2 : 3 | fT2 : 1
    ch1331();
    break;
  case 2:
    // fR1 : 1 | fT1 : 3 | fR2 : 3 | fT2 : 2
    ch1332();
    break;
  case 3:
    // fR1 : 1 | fT1 : 3 | fR2 : 3 | fT2 : 3
    ch1333();
    break;
  default:
    // fR1 : 1 | fT1 : 3 | fR2 : 3 | fT2 : N
    ch1333();
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
  default:
    // fR1 : 1 | fT1 : 3 | fR2 : N
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 1 | fT1 : 3 | fR2 : N | fT2 : 1
    ch1331();
    fR2->fShare = NotInvolved;
    break;
  case 2:
    // fR1 : 1 | fT1 : 3 | fR2 : N | fT2 : 2
    ch1332();
    fR2->fShare = NotInvolved;
    break;
  case 3:
    // fR1 : 1 | fT1 : 3 | fR2 : N | fT2 : 3
    ch1333();
    fR2->fShare = NotInvolved;
    break;
  default:
    // fR1 : 1 | fT1 : 3 | fR2 : N | fT2 : N
    ch1333();
    fR2->fShare = NotInvolved;
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
    }//end switch (fR2->fPhases)
    break;
    }//end switch (fT1->fPhases)
    break;
  case 2:
    // fR1 : 2
    switch (fT1->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 1
    switch (fR2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 1 | fR2 : 1
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 1 | fR2 : 1 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,2111,"Should not happen");
    ch2111();
    break;
  case 2:
    // fR1 : 2 | fT1 : 1 | fR2 : 1 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,2112,"Should not happen");
    ch2112();
    break;
  case 3:
    // fR1 : 2 | fT1 : 1 | fR2 : 1 | fT2 : 3
    PrintWhich();
    gMes->M(Error_M,2113,"Should not happen");
    ch2113();
    break;
  default:
    // fR1 : 2 | fT1 : 1 | fR2 : 1 | fT2 : N
    PrintWhich();
    gMes->M(Error_M,2114,"Should not happen");
    ch2114();
    break;
    }//end switch (fT2->fPhases)
    break;
  case 2:
    // fR1 : 2 | fT1 : 1 | fR2 : 2
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 1 | fR2 : 2 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,2121,"Should not happen");
    ch2121();
    break;
  case 2:
    // fR1 : 2 | fT1 : 1 | fR2 : 2 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,2122,"Should not happen");
    ch2122();
    break;
  case 3:
    // fR1 : 2 | fT1 : 1 | fR2 : 2 | fT2 : 3
    PrintWhich();
    gMes->M(Error_M,2123,"Should not happen");
    ch2123();
    break;
  default:
    // fR1 : 2 | fT1 : 1 | fR2 : 2 | fT2 : N
    PrintWhich();
    gMes->M(Error_M,2124,"Should not happen");
    ch2124();
    break;
    }//end switch (fT2->fPhases)
    break;
  case 3:
    // fR1 : 2 | fT1 : 1 | fR2 : 3
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 1 | fR2 : 3 | fT2 : 1
    ch2131();
    break;
  case 2:
    // fR1 : 2 | fT1 : 1 | fR2 : 3 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,2132,"Should not happen");
    ch2132();
    break;
  case 3:
    // fR1 : 2 | fT1 : 1 | fR2 : 3 | fT2 : 3
    ch2133();
    break;
  default:
    // fR1 : 2 | fT1 : 1 | fR2 : 3 | fT2 : N
    ch2133();
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
  default:
    // fR1 : 2 | fT1 : 1 | fR2 : N
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 1 | fR2 : N | fT2 : 1
    ch2141();
    break;
  case 2:
    // fR1 : 2 | fT1 : 1 | fR2 : N | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,2142,"Should not happen");
    ch2142();
    break;
  case 3:
    // fR1 : 2 | fT1 : 1 | fR2 : N | fT2 : 3
    ch2133();
    fR2->fShare = NotInvolved;
    break;
  default:
    // fR1 : 2 | fT1 : 1 | fR2 : N | fT2 : N
    ch2144();
    break;
    }//end switch (fT2->fPhases)
    break;
    }//end switch (fR2->fPhases)
    break;
  case 2:
    // fR1 : 2 | fT1 : 2
    switch (fR2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 2 | fR2 : 1
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 2 | fR2 : 1 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,2211,"Should not happen");
    ch2211();
    break;
  case 2:
    // fR1 : 2 | fT1 : 2 | fR2 : 1 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,2212,"Should not happen");
    ch2212();
    break;
  case 3:
    // fR1 : 2 | fT1 : 2 | fR2 : 1 | fT2 : 3
    PrintWhich();
    gMes->M(Error_M,2213,"Should not happen");
    ch2213();
    break;
  default:
    // fR1 : 2 | fT1 : 2 | fR2 : 1 | fT2 : N
    PrintWhich();
    gMes->M(Error_M,2214,"Should not happen");
    ch2214();
    break;
    }//end switch (fT2->fPhases)
    break;
  case 2:
    // fR1 : 2 | fT1 : 2 | fR2 : 2
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 2 | fR2 : 2 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,2221,"Should not happen");
    ch2221();
    break;
  case 2:
    // fR1 : 2 | fT1 : 2 | fR2 : 2 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,2222,"Should not happen");
    ch2222();
    break;
  case 3:
    // fR1 : 2 | fT1 : 2 | fR2 : 2 | fT2 : 3
    PrintWhich();
    gMes->M(Error_M,2223,"Should not happen");
    ch2223();
    break;
  default:
    // fR1 : 2 | fT1 : 2 | fR2 : 2 | fT2 : N
    PrintWhich();
    gMes->M(Error_M,2224,"Should not happen");
    ch2224();
    break;
    }//end switch (fT2->fPhases)
    break;
  case 3:
    // fR1 : 2 | fT1 : 2 | fR2 : 3
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 2 | fR2 : 3 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,2231,"Should not happen");
    ch2231();
    break;
  case 2:
    // fR1 : 2 | fT1 : 2 | fR2 : 3 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,2232,"Should not happen");
    ch2232();
    break;
  case 3:
    // fR1 : 2 | fT1 : 2 | fR2 : 3 | fT2 : 3
    ch2233();
    break;
  default:
    // fR1 : 2 | fT1 : 2 | fR2 : 3 | fT2 : N
    ch2233();
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
  default:
    // fR1 : 2 | fT1 : 2 | fR2 : N
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 2 | fR2 : N | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,2241,"Should not happen");
    ch2241();
    break;
  case 2:
    // fR1 : 2 | fT1 : 2 | fR2 : N | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,2242,"Should not happen");
    ch2242();
    break;
  case 3:
    // fR1 : 2 | fT1 : 2 | fR2 : N | fT2 : 3
    ch2233();
    fR2->fShare = NotInvolved;
    break;
  default:
    // fR1 : 2 | fT1 : 2 | fR2 : N | fT2 : N
    ch2244();
    break;
    }//end switch (fT2->fPhases)
    break;
    }//end switch (fR2->fPhases)
    break;
  default:
    // fR1 : 2 | fT1 : 3
    switch (fR2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 3 | fR2 : 1
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 3 | fR2 : 1 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,2311,"Should not happen");
    ch2311();
    break;
  case 2:
    // fR1 : 2 | fT1 : 3 | fR2 : 1 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,2312,"Should not happen");
    ch2312();
    break;
  case 3:
    // fR1 : 2 | fT1 : 3 | fR2 : 1 | fT2 : 3
  default:
    // fR1 : 2 | fT1 : 3 | fR2 : 1 | fT2 : N
    PrintWhich();
    gMes->M(Error_M,2314,"Should not happen");
    ch2313();
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
  case 2:
    // fR1 : 2 | fT1 : 3 | fR2 : 2
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 3 | fR2 : 2 | fT2 : 1
    PrintWhich();
    gMes->M(Error_M,2321,"Should not happen");
    ch2321();
    break;
  case 2:
    // fR1 : 2 | fT1 : 3 | fR2 : 2 | fT2 : 2
    PrintWhich();
    gMes->M(Error_M,2322,"Should not happen");
    ch2322();
    break;
  case 3:
    // fR1 : 2 | fT1 : 3 | fR2 : 2 | fT2 : 3
    PrintWhich();
    gMes->M(Error_M,2323,"Should not happen");
    ch2323();
    break;
  default:
    // fR1 : 2 | fT1 : 3 | fR2 : 2 | fT2 : N
    PrintWhich();
    gMes->M(Error_M,2324,"Should not happen");
    ch2323();
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
  case 3:
    // fR1 : 2 | fT1 : 3 | fR2 : 3
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 3 | fR2 : 3 | fT2 : 1
    ch2331();
    break;
  case 2:
    // fR1 : 2 | fT1 : 3 | fR2 : 3 | fT2 : 2
    ch2332();
    break;
  case 3:
    // fR1 : 2 | fT1 : 3 | fR2 : 3 | fT2 : 3
    ch2333();
    break;
  default:
    // fR1 : 2 | fT1 : 3 | fR2 : 3 | fT2 : N
    ch2333();
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
  default:
    // fR1 : 2 | fT1 : 3 | fR2 : N
    switch (fT2->fPhases) {
  case 0:
  case 1:
    // fR1 : 2 | fT1 : 3 | fR2 : N | fT2 : 1
    ch2331();
    fR2->fShare = NotInvolved;
    break;
  case 2:
    // fR1 : 2 | fT1 : 3 | fR2 : N | fT2 : 2
    ch2332();
    fR2->fShare = NotInvolved;
    break;
  case 3:
    // fR1 : 2 | fT1 : 3 | fR2 : N | fT2 : 3
    ch2333();
    fR2->fShare = NotInvolved;
    break;
  default:
    // fR1 : 2 | fT1 : 3 | fR2 : N | fT2 : N
    ch2333();
    fR2->fShare = NotInvolved;
    fT2->fShare = NotInvolved;
    break;
    }//end switch (fT2->fPhases)
    break;
    }//end switch (fR2->fPhases)
    break;
    }//end switch (fT1->fPhases)
    break;
  default:
    // fR1 : 3
    gMes->SetName("Choose");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,3,"1st reflected wave absorbed !");
    break;
  }//end switch (fR1->fPhases)
}
void TPhoton::Depolish() {
  //Tilt the normal to the surface to simulate a depolish. Implement grinding
  //of surface by tilting arbitrarily the normal to the surface. The tilt is
  //isotropic in space (sin(t) dt df) with t between 0 and fFace->fThetaDepls.
  //We need to build a coordinate system for which the Oz axis is the normal
  //to the face. Arbitrarily, we take the direction of the Ox axis to be the
  //direction given by point 1 (not 0) of the face, minus the current point
  //fX0. It can be verified by looking at the explanations for class TFace
  //that point 1 is always in the face, so that it works for all kind of
  //faces, even non planar faces.
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t small  = 1.0e-6;
  const Double_t vsmall = 1.0e-8;
  const Int_t nmax = 10000;
  Double_t y,sti,cti,stt,ctt;
  Double_t tgr,stgr,ctgr,fgr,sfgr,cfgr,ctgrM;
  T3Vector an1,an2,an3;
  T3Vector e2;
  if (TMath::Abs(fRfIndext.Im())>vsmall) {
    PrintWhich();
    gMes->SetName("Depolish");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_prog,1,"No grinding allowed in front of a complex index of refraction");
    fAnomaly = kTRUE;
    return;
  }
  else fRfIndext.PureReal();
  an3 = fFN;
  an1 = *fUsedFace->Point(1) - fX0;
  an1.Unit();
  if (gMes->TestCalc()) {
    Double_t eps = TMath::Abs(an1*an3);
    if (eps>small) {
      PrintWhich();
      gMes->SetName("Depolish");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,2,"vecteur Ox bad");
      fAnomaly = kTRUE;
    }
  }
  if (!fAnomaly) {
    an2 = an3^an1;
    an2.Unit();
    if (fS->HasBevellings()) ctgrM = fUsedFace->CosDepolishAngle(fDistRidge);
    else ctgrM = fUsedFace->CosDepolishAngle();
    Bool_t ok = kFALSE;
    Int_t n   = 0;
    while ((!ok) && (n<nmax)) {
      n   += 1;
      y    = gRandom3->Rndm();
      tgr  = TLitPhys::ACos(un - y*(un-ctgrM));
      stgr = TMath::Sin(tgr);
      ctgr = TMath::Cos(tgr);
      y    = gRandom3->Rndm();
      fgr  = deux*TMath::Pi()*y;
      sfgr = TMath::Sin(fgr);
      cfgr = TMath::Cos(fgr);
      fFNd = stgr*cfgr*an1 + stgr*sfgr*an2 + ctgr*an3;
      //Check that Poyinting vector of photon and new normal not opposite !
      cti = fUpg*fFNd;
      ok = (cti>zero);
      if (ok) {
        //In case of total reflection, photon must go inside polyhedron !
        e2 = fUpg - deux*cti*fFNd;
        y  = e2*an3;
        ok = (y<-small);
        if (ok) {
          //Now check that refractive wave will not be backward
          sti = TMath::Sqrt(un-cti*cti);
          stt = (fRfIndexi.Re()*sti)/fRfIndext.Re();
          if (stt<un) {
            ctt = TMath::Sqrt(un-stt*stt);
            e2  = stt*fUpg + (sti*ctt-cti*stt)*fFNd;
            e2.Unit();
            y   = e2*an3;
            ok  = (y>zero);
          }// end if (stt<un)
        }// end if (ok) 
      }// end if (ok) 
    }// end while ((!ok) && (n<nmax))
    if (n>=nmax) {
      PrintWhich();
      gMes->SetName("Depolish");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,3,"Anomaly in generation of grinding",n);
      fAnomaly = kTRUE;
    }
  }// end if (!fAnomaly)
}
void TPhoton::Diffused() {
  //  Handles the case of a photon in front of a revetment which diffuses the
  //photon.
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
  //anisotropic material : see tricky point c) in the documentation of
  //TRevetment.
  //  Once one knows what the direction of the diffused photon will be, one
  //calculates the orientation of a pseudo plane of reflection in such a way
  //that the photon REFLECTED on this pseudo plane of reflection has the
  //wanted direction for the DIFFUSED photon. One does that assuming that the
  //angle of reflection is equal to the incident angle, which is true only for
  //isotropic material. This is the origin of tricky point c) quoted above.
  //  We first construct a local coordinate system into which we intend
  //to gener the direction of the diffused photon.
  // "In" and "Out" characteristics of medium must be defined before calling
  //Diffused.
  const Double_t zero  =  0.0;
  const Double_t un    =  1.0;
  const Double_t minus = -1.0;
  const Double_t deux  =  2.0;
  const Double_t dixm6 =  1.0e-6;
  Int_t bin;
  Double_t y,a,b,c,thtdf,sthtdf,cthtdf,phidf,sphidf,cphidf;
  T3Vector an1,an2,an3;
  an3 = minus*fFN;
  a = an3(0);  b = an3(1);
  c = a*a+b*b;
  if (c>dixm6) {
    an1(0) =  an3(1);
    an1(1) = -an3(0);
    an1(2) =  zero;
    an1.Unit();
    an2 = an3^an1;
  }
  else {
    an1(0) =  an3(2);
    an1(1) =  zero;
    an1(2) = -an3(0);
    an1.Unit();
    an2 = an3^an1;
  }
  //  Now we generate a vector which is anywhere on the hemisphere around
  //the direction of an3. It makes an angle theta=thtdf < PI/2 with an3
  //and its projection onto the plane (an1,an2) makes an angle phi = phidf
  //with an1. This vector will be taken as the direction of the diffused
  //photon.
  y      = gRandom3->Rndm();
  phidf  = deux*TMath::Pi()*y;
  sphidf = TMath::Sin(phidf);
  cphidf = TMath::Cos(phidf);
  y      = gRandom3->Rndm();
  //
  // This WOULD BE the code for an isotropic distribution of diffusion,i.e. a
  //distribution of sin(thtdf)dthtdfdphidf :
  //
  //    thtdf  = TLitPhys::ACos(1-y)
  //
  // But in reality the distribution for diffusion is better described if we
  //favour the direction normal to the plane of the diffusing medium by a
  //factor cos(thtdf), i.e. if we take the distribution
  //
  //    cos(thtdf)sin(thtdf)dthtdfdphidf   [0<=thtdf<=ThetaMax]
  //
  // Look at the documentation about TRevetment to have a complete description
  //of diffusion.
  //
  thtdf  = TLitPhys::ASin(fSinThetaMax*TMath::Sqrt(y));
  sthtdf = TMath::Sin(thtdf);
  cthtdf = TMath::Cos(thtdf);
  a      = sthtdf*cphidf;
  b      = sthtdf*sphidf;
  c      = cthtdf;
  T3Vector tupg;
  //tupg is the direction of the diffused photon in WCS.
  tupg   = a*an1 + b*an2 + c*an3;
  //we find the normal to the pseudo reflecting plane, which will give
  //the reflected photon at tupg !
  a      = fUpg*tupg;
  b      = un/TMath::Sqrt(deux*(un-a));
  fOx    = b*(fUpg - tupg);
  Transition();
  if (!fAnomaly && fPhtAbs) {
    //Photon has been absorbed by the revetment
    fPhtLive = kFALSE;
    gGs->fNpLossAny += 1;
    gGs->fNpAbsRvt  += 1;
    bin = gGs->fHAbsAny->Fill(fSl0);
    bin = gGs->fHAbsRvt->Fill(fSl0);
    bin = gGs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
    if (gCs) {
      gCs->fNpLossAny += 1;
      gCs->fNpAbsRvt  += 1;
      bin = gCs->fHAbsAny->Fill(fSl0);
      bin = gCs->fHAbsRvt->Fill(fSl0);
      bin = gCs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
    }
  }// end if (!fAnomaly && fPhtAbs)
}
void TPhoton::ExitFace() {
  //Handles photon between face and revetment
  //(old CREXITFC)
  const Int_t nmaxloop = 10000; //doing following loop more than nmaxloop times is an anomaly
  Int_t nloop = 0;
  while (!fAnomaly && fOutPoly && fPhtLive) {
    SliceToRev();
    if (!fAnomaly && fPhtLive) {
      SliceToShape();
    }
    nloop++;
    if (nloop>nmaxloop) fAnomaly = kTRUE;
  }
}
void TPhoton::FindAbsLength() {
  //  Finds the complex index of refraction, including absorption, in the case
  //of anisotropic absorption.
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  Double_t f1,f2,f3,f1s,f2s,f3s,f1m,f2m,f3m,f12,f13,f23;
  TSComplex g11,g12,g13,g22,g23,g33;
  TSComplex g1,g2,g3,g4,g5,g6;
  TSComplex a0,a1,a2,dd;
  f1  = fUk0(0);
  f1s = f1*f1;
  f2  = fUk0(1);
  f2s = f2*f2;
  f3  = fUk0(2);
  f3s = f3*f3;
  f1m = un - f1s;
  f2m = un - f2s;
  f3m = un - f3s;
  f12 = f1*f2;
  f13 = f1*f3;
  f23 = f2*f3;
  g11 = fCDzt0(0,0);
  g12 = fCDzt0(0,1);
  g13 = fCDzt0(0,2);
  g22 = fCDzt0(1,1);
  g23 = fCDzt0(1,2);
  g33 = fCDzt0(2,2);
  g1  = g22*g33 - g23*g23;
  g2  = g11*g33 - g13*g13;
  g3  = g11*g22 - g12*g12;
  g4  = g12*g33 - g23*g13;
  g5  = g13*g22 - g12*g23;
  g6  = g23*g11 - g12*g13;
  a0  = g22*g13*g13-deux*g12*g13*g23+g11*g23*g23+g33*g12*g12-g11*g22*g33;
  a1  = f1m*g1+f2m*g2+f3m*g3+deux*(f12*g4+f13*g5+f23*g6);
  a2  = -f1s*g11 -f2s*g22 -f3s*g33 -deux*(f12*g12 + f13*g13 + f23*g23);
  dd  = Sqrt(a1*a1-quatre*a2*a0);
  g3  = deux*a2;
  g1  = Sqrt((zero -a1 - dd)/g3);
  g2  = Sqrt((zero -a1 + dd)/g3);
  f1  = g1.Re();
  f1s = g1.Im();
  if (f1<zero) {
    f1  = - f1;
    f1s = - f1s;
  }
  f2  = g2.Re();
  f2s = g2.Im();
  if (f2<zero) {
    f2  = - f2;
    f2s = - f2s;
  }
  f1m = TMath::Abs(fRfIndex0.Re() - f1);
  f2m = TMath::Abs(fRfIndex0.Re() - f2);
  if (f2m<f1m) {
    f1  = f2;
    f1s = f2s;
  }
  //f1s must be negativ
  if (f1s>zero) {
    f1s = - f1s;
    gMes->SetName("FindAbsLength");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,1,"absorption length id negativ");
    fAnomaly = kTRUE;
  }
  f1s = - f1s;
  fAbsl0 = TLitPhys::Get()->C()/(deux*fW*f1s);
}
void TPhoton::FindDielAni() {
  //Find dielectric properties and E field in case of anisotropic material
  //uz0 is the unit vector to which the E field has to be orthogonal
  //In anisotropic material, uz0 is NOT parallel to k vector.
  //fUx0, fUy0, uz0 build a right orthonormal system
  //fUx0 is taken horizontal, OBDA (ohne Beschränkung der Allgemeinheit !)
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t z05    = 0.5;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  const Double_t small  = 1.0e-7;
  const Double_t vsmall = 1.0e-300;
  Double_t f1,f2,f3,f1s,f2s,f3s;
  Double_t a0,a1,a2,dd;
  Double_t g1,g2,g3,g4,g5,g6;
  Double_t g11,g12,g13,g22,g23,g33;
  Short_t NbSol = 0;
  f1  = fUk0(0);
  f1s = f1*f1;
  f2  = fUk0(1);
  f2s = f2*f2;
  f3  = fUk0(2);
  f3s = f3*f3;
  g11 = fDzt0(0,0);
  g12 = fDzt0(0,1);
  g13 = fDzt0(0,2);
  g22 = fDzt0(1,1);
  g23 = fDzt0(1,2);
  g33 = fDzt0(2,2);
  g1  = g22*g33 - g23*g23;
  g2  = g11*g33 - g13*g13;
  g3  = g11*g22 - g12*g12;
  g4  = g12*g33 - g23*g13;
  g5  = g13*g22 - g12*g23;
  g6  = g23*g11 - g12*g13;
  a0  = g22*g13*g13-deux*g12*g13*g23+g11*g23*g23+g33*g12*g12-g11*g22*g33;
  a1  = (un-f1s)*g1+(un-f2s)*g2+(un-f3s)*g3+deux*(f1*f2*g4+f1*f3*g5+f2*f3*g6);
  a2 = -f1s*g11 -f2s*g22 -f3s*g33 -deux*(f1*f2*g12 + f1*f3*g13 + f2*f3*g23);
  dd = a1*a1-quatre*a2*a0;
  if ((dd < zero) && (dd > -small)) dd = zero;
  if (dd<zero) {
    PrintWhich();
    gMes->SetName("FindDielAni");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,1,"complex indices !");
    fAnomaly = kTRUE;
    return;
  }
  dd = TMath::Sqrt(dd);
  //g1 and g2 : the 2 acceptable index of refraction ^2
  g3 = deux*a2;
  if (TMath::Abs(g3)<vsmall) {
    gMes->SetName("FindDielAni");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,8,"indices are infinite !");
    fAnomaly = kTRUE;
    return;
  }
  g1 = (-a1 - dd)/g3;
  g2 = (-a1 + dd)/g3;
  if ((g1<=zero) && (g2<=zero)) {
    PrintWhich();
    gMes->SetName("FindDielAni");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,2,"both index imaginary");
    fAnomaly = kTRUE;
    return;
  }
  if (g1<=zero) {
    PrintWhich();
    gMes->SetName("FindDielAni");
    gMes->SetTitle("TPhoton");
    gMes->M(Warning_M,3,"1st index imaginary");
    g1 = g2;
  }
  else {
    NbSol += 1;
    g1 = TMath::Sqrt(g1);
  }
  if (g2<=zero) {
    PrintWhich();
    gMes->SetName("FindDielAni");
    gMes->SetTitle("TPhoton");
    gMes->M(Warning_M,4,"2nd index imaginary");
  }
  else {
    NbSol += 1;
    g2 = TMath::Sqrt(g2);
  }
  if (NbSol>=2) {
    switch (fChooseIndex) {
    case 1:
      if (g2>g1) g1=g2;
      break;
    case 2:
      if (g2<g1) g1=g2;
      break;
    default:
      //We toss a coin to choose between the 2 indices
      g3 = gRandom3->Rndm();
      if (g3<z05) g1=g2;
      break;
    }
  }
  fRfIndex0 = g1;
  fRfIndexi = g1;
  fI->fRfI  = g1;
  fDiel0    = (g1*g1)/fPmu0;
  fI->fDiel = fDiel0;
  //We can calculate fAk0 ( module of wave vector ) in [ cm-1 ]
  fAk0 = (fW/TLitPhys::Get()->C())*g1;
  //and the k vector itself :
  fVk0 = fAk0*fUk0;
  g3    = g1*g1;
  //We verify that the determinant is really 0 with g1
  T33Matrix aam;
  aam(0,0) = g3*(f2s+f3s) - g11;
  aam(0,1) = - ( g3*f1*f2 + g12 );
  aam(0,2) = - ( g3*f1*f3 + g13 );
  aam(1,0) = aam(0,1);
  aam(1,1) = g3*(f1s+f3s) - g22;
  aam(1,2) = - ( g3*f2*f3 + g23 );
  aam(2,0) = aam(0,2);
  aam(2,1) = aam(1,2);
  aam(2,2) = g3*(f1s+f2s) - g33;
  //Now we find the direction of the electric field.
  Short_t deg;
  T3Vector Ef2;
  deg = fEf0.DegMatrix(aam,Ef2,gMes->TestCalc());
  switch (deg) {
  case 0:
    PrintWhich();
    gMes->SetName("FindDielAni");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,9,"determinant non zero");
    fAnomaly = kTRUE;
    return;
  case 2:
    PrintWhich();
    gMes->SetName("FindDielAni");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,10,"matrix twice degenerate");
    fAnomaly = kTRUE;
    break;
  case 3:
    PrintWhich();
    gMes->SetName("FindDielAni");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,11,"matrix is 0");
    fAnomaly = kTRUE;
    return;
  default:
    break;
  }
  //We toss a coin to decide of the direction of the E field
  g3 = gRandom3->Rndm();
  if (g3<z05) fEf0.ChangeSign();
  //Verify that uz0 is orthogonal to E field
  if (gMes->TestCalc()) {
    T3Vector uz0;
    uz0 = fUk0*fDzt0;
    uz0.Unit();
    g1 = TMath::Abs(fEf0*uz0);
    if (g1>small) {
      PrintWhich();
      gMes->SetName("FindDielAni");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,7,"Bad direction for E field",g1);
      fAnomaly = kTRUE;
      return;
    }
  }
  //Calculates energy density
  Double_t u;
  u = fEf0*(fDzt0*fEf0);
  u = (u*TLitPhys::Get()->Eps0())/fPmu0;
  fAef0 = TMath::Sqrt(fEphot/(u*TLitPhys::Get()->Vphot()));
  //E field
  fEf0  = fAef0*fEf0;
  if (fAnia0) FindAbsLength();
  else fAbsl0 = fMat0->GetAbsL();
}
void TPhoton::FindDielIso() {
  //Find dielectric properties and E field in case of isotropic material
  Double_t x;
  fRfIndex0 = fMat0->GetIndex();
  x         = fRfIndex0.Re();
  //Dielectric constant
  fDiel0    = (x*x)/fPmu0;
  fI->fDiel = fDiel0;
  //Module of electric field. TLitPhys::Get()->fVphot is an arbitrary volume (we have taken
  //1 mm3) in which the energy of the total energy of the photon is hw, i.e. we
  //consider arbitrarily that the momentan extension of the photon is 1mm3.
  fAef0     = TMath::Sqrt((fEphot/TLitPhys::Get()->Eps0())/(TLitPhys::Get()->Vphot()*fDiel0));
  //fAk0 ( module of wave vector ) in [ cm-1 ]
  fAk0      = (fW/TLitPhys::Get()->C())*x;
  //k vector
  fVk0      = fAk0*fUk0;
  //E field
  fEf0      = fAef0*fEf0;
}
Double_t TPhoton::FindProcessAndLength(T3Vector x1) {
  // Finds which process is to occur for this step. Is called by Jump()
  const Double_t verysmall = 1.0e-50;
  const Bool_t   newmethod = kTRUE;
  T3Vector x10 = x1 - fX0;       // Vector to next point on face, in world coordinates 
  Double_t yDist = x10.Module(); // Distance to next point on face
  Double_t yDistTest = 0.0;      // Test distance to see if process occurs before photon reaches face
  Double_t yAbs,pstopAbs;
  if (fIsAbsorb) {
    if (fAbsl0<verysmall) {
      fProcess = kAbsorption;
      return 0.0;
    }
    if (!fS->IsDamage()) {
      yAbs      = gRandom3->Rndm();
      yDistTest = -fAbsl0*TMath::Log(1.0 - yAbs);
    }
    else {
      //
      // Position-dependent absorption length
      //
      T3Vector damxstart; //start of step idam
      T3Vector damxstop;  //stop  of step idam
      T3Vector damx;      //point where absorption length calculated.
      Int_t ndamstep;     //number of steps
      Int_t idam;         //which step
      Int_t idamm1;       //idam - 1
      Double_t damstep;   //length of one step in cm
      Double_t damabsli;  //absorption length for step idam;
      Double_t damstart;  //u coord. of start of step idam
      Double_t damstop;   //u coord. of stop  of step idam
      Double_t daml;      //length of step idam
      Double_t damylabs;  //last path before absorption
      Bool_t absorbed = kFALSE;
      damstep = fS->GetDamStep();
      ndamstep = Int_t(yDist/damstep + 1);
      x10.Unit();
      damstart = 0.0;
      idam = 1;
      while ((!absorbed) && (idam<=ndamstep)) {
        idamm1    = idam - 1;
        damstart  = idamm1*damstep;
        damxstart = fX0 + damstart*x10;
        if (idam<ndamstep) {
          damstop  = idam*damstep;
          damxstop = fX0 + idam*damstep*x10;
        }
        else {
          damstop  = yDist;
          damxstop = x1;
        }
        damx     = 0.5*(damxstart + damxstop);
        damabsli = fAbsl0*fS->GetDamage(&damx,fAbsl0);
        daml     = damstop - damstart;
        if (newmethod) {
          damylabs = gRandom3->Rndm();
          damylabs = -damabsli*TMath::Log(1.0 - damylabs);
          absorbed = damylabs<daml;
          if (absorbed) yDistTest += damylabs;
          else          yDistTest += daml;
        }//end if (newmethod)
        else {
          pstopAbs = 1 - TMath::Exp(-daml/damabsli);
          yAbs     = gRandom3->Rndm();
          absorbed = yAbs<pstopAbs;
          if (absorbed) {
            damylabs = gRandom3->Rndm();
            damylabs = -fAbsl0*TMath::Log(1.0-damylabs*pstopAbs);
            yDistTest   += damylabs;
          }
          else yDistTest   += daml;
        }//end else if (newmethod)
        idam++;
      } //end while ((!absorbed) && (idam<=ndamstep))
    } //end else (!fS->IsDamage())
    if (yDistTest<yDist){
      yDist = yDistTest;
      fProcess = kAbsorption;} 
  } // end if (fIsAbsorb)
  if (fIsDiffus) {
    yDistTest = -fDiffusL*TMath::Log(1.0-gRandom3->Rndm());
    if (yDistTest<yDist) {
      yDist = yDistTest;
      fProcess = kIsotropicScattering;} 
  }
  if (fIsRayleigh) {
    yDistTest = -fRayleighL*TMath::Log(1.0-gRandom3->Rndm());
    if (yDistTest<yDist) {
      yDist = yDistTest;
      fProcess = kRayleighScattering;}
  } 
  if (fIsWLS) {
    yDistTest = -fWLSL*TMath::Log(1.0-gRandom3->Rndm());
    if (yDistTest<yDist) {
      yDist = yDistTest;
      fProcess = kWLS;}
  }
  return yDist;
}
void TPhoton::GetImin12(Short_t k,Short_t &i1,Short_t &i2,Short_t ki) {
  //Finds the k th pair of solution. ki is the solution of the incoming
  //wave (if call from anitoiso) which should be tried last.
  Short_t k0,k1,k2,k3;
  k -= 1;
  k %= 6;
  if (ki<0) {
    //no incoming wave to avoid
    k0 = 0; k1 = 1; k2 = 2; k3 = 3;
  }
  else {
    //incoming wave to avoid
    k0 = -1; k1 = -1; k2 = -1; k3 = -1;
    for (Short_t i = 0; i<4; i++) {
      if (i != ki ) {
        if  (k0 == -1) k0 = i;
        if ((k1 == -1) && (k0 != i)) k1 = i;
        if ((k2 == -1) && (k0 != i) && (k1 != i)) k2 = i;
      }
    }
    k3 = ki;
  }
  switch (k) {
  case 0:
    i1 = k0; i2 = k1;
    break;
  case 1:
    i1 = k0; i2 = k2;
    break;
  case 2:
    i1 = k1; i2 = k2;
    break;
  case 3:
    i1 = k0; i2 = k3;
    break;
  case 4:
    i1 = k1; i2 = k3;
    break;
  case 5:
    i1 = k2; i2 = k3;
    break;
  }
}
void TPhoton::GetShapeAndMat(TSupplShape *SupplShape) {
  //finds pointer to shape and pointer to optical material.
  const Axis_t eps = 0.01;
  fFace     = 0;
  fSubFace  = 0;
  fUsedFace = 0;
  fS        = SupplShape;
  fNShape   = fS->fShapeNumber;
  fANShape  = fNShape + eps;
  fShape    = SupplShape->GetShape();
  fMat0     = SupplShape->GetOptMat();
  fAnis0    = fMat0->GetAnisotropy();
  fAnia0    = fMat0->GetAniAbsL();
  fIsAbsorb   = fMat0->GetHasAbsorption();
  if (fIsAbsorb) fAbsl0    = fMat0->GetAbsL();
  fIsRayleigh = fMat0->GetHasRayleigh();  
  fRayleighL  = fMat0->GetRayleighLength();
  fIsWLS      = fMat0->GetHasWLS();
  fWLSL       = fMat0->GetWLSL();
  fIsDiffus = fMat0->GetHasDiffusion();
  fDiffusL  = fMat0->GetDiffLength();
  fPmu0     = fMat0->GetMu();
  fKind     = SupplShape->GetKind();
  fDzt0     = SupplShape->fDTmu;
  if (fAnia0) fCDzt0 = SupplShape->fCDTmu;
}
void TPhoton::ICStoWCS() {
  //Returns from the coordinate system of the incident plane (ICS) into the
  //world coordinate system and defines there completly the photon.
  const Double_t un    = 1.0;
  const Double_t small = 1.0e-6;
  Short_t anisnew;
  Double_t d,pmunew;
  fUk0 = fMn*fI->fUk;
  fVk0 = fMn*fI->fVk;
  fEf0 = fMn*fI->fEf;
  fUk0.Unit();
  if (fPhtOut) {
    anisnew = fAnist;
    pmunew  = fPmut;
  }
  else {
    anisnew = fAnisi;
    pmunew  = fPmui;
  }
  //Refraction index for a photon of wave vector UK0.
  fI->fRfI  = (fAk0*TLitPhys::Get()->C())/fW;
  fRfIndexi = fI->fRfI;
  fPmui     = pmunew;
  //Dielectric constant for a photon of wave vector UK0
  d         = fI->fRfI.Re();
  fI->fDiel = (d*d)/pmunew;
  Double_t s,eu;
  T3Vector ee(fEf0);
  ee.Unit();
  s = fUk0*ee;
  //In isotropic case, check the orthogonality between k vector and E field,
  //and, if not perfect due to finite machine precision make the smallest
  //possible correction to ee so that it is perfectly orthogonal to UK0 and
  //remains of length 1.
  if ((!anisnew) && (TMath::Abs(s)>small)) {
    gMes->SetName("ICStoWCS");
    gMes->SetTitle("TPhoton");
    gMes->M(Warning_M,1,"k and E not orthogonal",s);
    d  = un/TMath::Sqrt(un - s*s);
    ee = d*(ee - s*fUk0);
  }
  eu = un - s;
  //fAef0 : module of renormalized E field ( we increase the module of the
  //electric field to this value in order that the energy of the photon after
  //the process is the same as the energy of the photon before the process ).
  //The following calculation is valid in both isotropic and anisotropic case.
  //In isotropic case : eu = 1.
  fAef0 = (fEphot/TLitPhys::Get()->Eps0())/(TLitPhys::Get()->Vphot()*fI->fDiel*eu);
  fEf0  = fAef0*ee;
  BuildBPUP(fVk0,fEf0,fBf0,fPtg,fUpg,pmunew);
  fCospk = fUk0*fUpg;
  if (fAnia0) FindAbsLength();
}
void TPhoton::Init() {
  const Axis_t   zeroa = 0.0;
  const Double_t zerod = 0.0;
  fRun           = 0;       //run number
  fKphot         = 0;       //photon number
  fKind          = tsbrik;  //kind of shape in which photon is
  fNShape        = 0;       //Number of this shape
  fANShape       = zeroa;   //Idem as Axis_t
  fSl0           = zerod;   //space travelled by photon
  fDistRidge     = zerod;   //Distance of photon to nearest ridge of face (when on a face )
  fT0            = zerod;   //time elapsed since birth of photon or excitation in [ps]
  //status
  fNPoints       = 0;       //(NPTPHOT) Number of points on the trajectory of the photon
  fPhtLive       = kFALSE;  //true if photon not yet absorbed or detected
  fPhtSeen       = kFALSE;  //true if photon seen by detector
  fToOther       = kFALSE;  //true if photon towards other convex polyhedron
  fBetween       = kFALSE;  //true if photon between 2 convex polyhedrons
  fOutPoly       = kFALSE;  //VERIFIER QUE PAS INUTILE !true if photon out of shape
  fPhtOut        = kFALSE;  //true if photon is transmitted or absorbed, false if reflected.
  fPhtAbs        = kFALSE;  //true if photon absorbed during transition process for instance on reflection on metal.
  fAnomaly       = kFALSE;  //photon to be killed for abnormal reasons. Should never happen
  fFromani       = kFALSE;  //true if incident wave in anisotropic medium
  fChooseIndex   = 0;       //choice between n1 and n2 {0->random,1->biggest,2->smallest}
  //physical values
  fSkl0          = zerod;   //Sum over all k*l
  fWvlgth        = zerod;   //wavelength of photon in nanometers
  fW             = zerod;   //frequency in ps-1
  fEphot         = zerod;   //energy of photon in joule [mksa]
  //k vector
  fAk0           = zerod;   //absolute value (module) of vector k in [cm-1]
  //Absorption length and diffusion length
  fIsAbsorb      = kFALSE;  //true if there is absorption inside this material
  fAbsl0         = zerod;   //abs. length for this photon.
  fIsDiffus      = kFALSE;  //true if there is diffusion inside this material
  fDiffusL       = zerod;   //diffusion length
  fNbOfDiffusion = 0;       //number of diffusions that occured for this photon
  fIsRayleigh    = kFALSE;  //true if there is Rayleigh scattering inside this material
  fRayleighL     = zerod;   //Rayleigh scattering length
  fNbOfRayleigh  = 0;       //number of Rayleigh scatterings that occured for this photon 
  fIsWLS         = kFALSE;  //true if there is this material is wavelength shifting
  fWLSL          = zerod;   //Wavelength shifting length
  fNbOfWLS       = 0;       //number of wavelength shifts that occured for this photon 
  //Electric Field
  fAef0          = zerod;   //absolute value (module) of electric field E
  fBeta          = zerod;   //phase of linearly polarized photon (irrelevant)
  fCospk         = zerod;   //cosine of angle between Poyinting and k vector
  //characteristics of the face where the photon is
  fNFace         = 0;       //face number of the master face fFace
  fNbSubFaces    = 0;       //number of subfaces on this face
  fNSubFace      = 0;       //face number of the sub face fSubFace, if any
  fKFace         = 0;       //= fNSubFace if (fSubFace), else = fNFace
  fPlanar        = kFALSE;  //true if fFace is planar
  fSinThetaMax   = zerod;   //Max. value for angle theta in diffusion
  fNDet          = 0;       //detector number if fFace or fSubFace is a detector
  fDetKind       = none;    // detector type
  fAccept        = zerod;   //acceptance angle of detector
  fHasContact    = kFALSE;  //true if face in contact with an other face
  fNContact      = 0;       //number of contacts
  fContactType   = nocontact; //type of contact
  //characteristics of material in which the photon is
  fAnis0         = 0;       //anisotropic optical characteristics of material in which photon is
  fAnia0         = 0;       //anisotropic absorption characteristics of material in which photon is
  fPmu0          = 1.0;     //magnetic permeability of material in which photon is
  fDiel0         = zerod;   //diel. constant eps in case of isotropic material
  fIsDiag        = kFALSE;  //the dielectric tensor in use is diagonal in ICS
  //characteristics of material associated with the incident wave
  fAnisi         = 0;       //anisotropic characteristics of material associated with the incident wave
  fAniai         = 0;       //anisotropic absorption characteristics of material associated with the incident wave
  fPmui          = zerod;   //magnetic permeability mu of material associated with the incident wave
  fIsDiagi       = kFALSE;  //the dielectric tensor of the shape containing the incident wave is diagonal in ICS
  //characteristics of material into which the photon may go
  fAnis1         = 0;       //anisotropic optical characteristics of destination material
  fAnia1         = 0;       //anisotropic absorption characteristics of destination material
  fPmu1          = 1.0;     //magnetic permeability of destination material
  fDiel1         = zerod;   //diel. constant eps in case of isotropic destination material
  //characteristics of material associated with the transmitted wave
  fAnist         = 0;       //anisotropic characteristics of material associated with the transmitted wave
  fAniat         = 0;       //anisotropic absorption characteristics of material associated with the transmitted wave
  fPmut          = zerod;   //magnetic permeability of  of material associated with the transmitted wave
  fIsDiagt       = kFALSE;  //the dielectric tensor of the shape containing the transmitted wave is diagonal in ICS
  //Weight of each possible choice for a wave
  for (Short_t j=0;j<8;j++) fWeigth[j] = zerod;
}
void TPhoton::InitP() {
  //Pointers to 0
  fShape        = 0;
  fS            = 0;
  fFace         = 0;
  fSubFace      = 0;
  fUsedFace     = 0;
  fRevetment    = 0;
  fMatSl        = 0;
  fQE           = 0;
  fContact      = 0;
  fThinSlice    = 0;
  fContactFace  = 0;
  fMat0         = 0;
  fContactShape = 0;
  fContactSuppl = 0;
  fMat1         = 0;
  fI            = 0;
  fR1           = 0;
  fR2           = 0;
  fT1           = 0;
  fT2           = 0;
}
void TPhoton::IsoToAni() {
  //Calculates the transition from an isotropic medium towards an anisotropic
  //medium
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t quatre = 4.0;
  const Double_t small  = 1.0e-5;
  const Double_t dixm4  = 1.0e-4;
  const Double_t wsmall = 1.0e-11;
  const Double_t DbLim  = 1.0e-150;
  //
  Double_t ns;               //(index of refraction)*(sin(thetai) of incident wave
  Double_t a1,a3,a4,a5,gd;
  Double_t a,b,c,d,g;        //coefficients for the equation (E1) handled by
  //TEqIndex giving the 2 indices for the 2
  //transmitted waves
  Double_t quality;          //quality of solution provided by TEqIndex
  Double_t d00,d01,d02,d11,d12,d22;
  Double_t c01,c02,c12;
  Double_t dh1,dh2;
  TSComplex zh1;
  T3Vector vh1;
  Short_t is;
  Bool_t isol[4];
  Bool_t  eva1,eva2;         //true if wave fR1(fR2) evanescent
  Bool_t Good1;              //The 1st solution selected among the 4
  //proposed by TEqIndex is good or not
  Bool_t Good2;              //The 2nd solution selected among the 4
  //proposed by TEqIndex is good or not
  Short_t imin1,imin2;       //2 solutions among the 4 proposed by TEqIndex
  //
  Bool_t invers = kFALSE;    //true if the 2 solutions have been interchanged
  fR1->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  fR2->fShare = NotInvolved; //no 2nd reflected wave in an isotropic medium
  fT1->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  fT2->fShare = NotTaken;    //we don't know yet whether this wave will be taken
  //Same index for reflected and incident wave since medium isotropic
  fI->fRfI    = fRfIndexi;
  fR1->fRfI   = fI->fRfI;
  //Angle of reflection == angle of incidence and real.
  fR1->fCtht  = fI->fCtht;
  fR1->fStht  = fI->fStht;
  // Here comes the calculation of the equation (E1) (see theoretical paper)
  //giving the 2 possible indices for the 2 transmitted waves
  ns  = fI->fRfI.Re()*fI->fStht.Re();
  g   = ns*ns;
  d00 = fDztti(0,0);
  d01 = fDztti(0,1);
  d02 = fDztti(0,2);
  d11 = fDztti(1,1);
  d12 = fDztti(1,2);
  d22 = fDztti(2,2);
  c01 = d01*d01;
  c02 = d02*d02;
  c12 = d12*d12;
  a1  = (d00-d11)*g + d00*(d11+d22) - c02 - c01;
  a3  = deux*d01*ns;               //+sign, contrary to same equation in AniToIso
  a4  = deux*ns*(d01*d22-d02*d12); //+sign, contrary to same equation in AniToIso
  a5  = g*(c02 - c12 - d22*(d00-d11));
  a5 += - d00*d11*d22 - deux*d01*d02*d12 + d11*c02 + d00*c12 + d22*c01;
  // We have to divide by d00, so we cannot accept it to be 0
  if (TMath::Abs(d00) < DbLim) {
    PrintWhich();
    gMes->SetName("IsoToAni");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,1,"fDztti(0,0) is 0",d00);
    fAnomaly = kTRUE;
    return;
  }
  fFromani = kFALSE;
  a = -a1/d00;
  b =  a3/d00;
  c = -a4/d00;
  d = -a5/d00;
  if (fIsDiagt) {
    gd  = d22 - d11;
    gd += (g*(d11 - d00))/d00;
    gd *= gd;
  }
  else {
    gd  = a*a - quatre*d;
  }
  gIdx->Set(a,b,c,d,g,gd);
  quality = gIdx->Solve(fAnist,kTRUE);
  if (quality>small) {
    PrintWhich();
    gMes->SetName("IsoToAni");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,2,"bad solution for index equation",quality);
    gMes->M(kill_this,2,"a b  : ",a,b);
    gMes->M(kill_this,2,"c d  : ",c,d);
    gMes->M(kill_this,2,"g gd : ",g,gd);
    fAnomaly = kTRUE;
    return;
  }
  for (is=0;is<4;is++) isol[is] = kTRUE;
  Short_t NumberOfTry = 0;
bigloop:
  do {
    Good1 = kFALSE;
    Good2 = kFALSE;
    NumberOfTry += 1;
    fR1->fPhases = 0;
    fT1->fPhases = 0;
    fT2->fPhases = 0;
    //We look for 2 possible solutions.
    Bool_t found = kFALSE;
    while (!found) {
      GetImin12(NumberOfTry,imin1,imin2);
      found = (isol[imin1]) && (isol[imin2]);
      if (!found) {
        NumberOfTry += 1;
        if (NumberOfTry>6) {
          gMes->SetName("IsoToAni");
          gMes->SetTitle("TPhoton");
          gMes->M(kill_this,3,"all solutions tried without success",fKphot);
          fAnomaly = kTRUE;
          return;
        }// end if (NumberOfTry>6)
      }// end if (!found)
    }// end while (!found)
    eva1 = gIdx->GetfR(imin1);
    eva2 = gIdx->GetfR(imin2);
    //We assign found value of index to the 1st transmitted wave :
    fT1->fRfI = Sqrt(gIdx->GetfZ(imin1));
    fT1->fEva = eva1;
    if (!eva1) fT1->fRfI.PureReal();
    //We assign found value of index to the 2nd transmitted wave :
    fT2->fRfI = Sqrt(gIdx->GetfZ(imin2));
    fT2->fEva = eva2;
    if (!eva2) fT2->fRfI.PureReal();
    //Fresnel or Descartes equation for the sinus of the 1st transmitted wave
    fT1->fStht = ns/fT1->fRfI;
    fT1->fCtht = gIdx->GetfD(imin1);
    //We clean very small real or imaginary parts. When presents, they are almost
    //always due to numerical imprecision. If on the contrary they were physical,
    //we at most introduce an error of wsmall in this cosine.
    if ((TMath::Abs(fT1->fCtht.Im())>wsmall) &&
      (TMath::Abs(fT1->fCtht.Re())<wsmall)) fT1->fCtht.PureIm();
    if ((TMath::Abs(fT1->fCtht.Re())>wsmall) &&
      (TMath::Abs(fT1->fCtht.Im())<wsmall)) fT1->fCtht.PureReal();
    else fT1->fEva = kTRUE;
    //Fresnel or Descartes equation for the sinus of the 2nd transmitted wave
    fT2->fStht = ns/fT2->fRfI;
    fT2->fCtht = gIdx->GetfD(imin2);
    //We clean very small real or imaginary parts. When presents, they are almost
    //always due to numerical imprecision. If on the contrary they were physical,
    //we at most introduce an error of wsmall in this cosine.
    if ((TMath::Abs(fT2->fCtht.Im())>wsmall) &&
      (TMath::Abs(fT2->fCtht.Re())<wsmall)) fT2->fCtht.PureIm();
    if ((TMath::Abs(fT2->fCtht.Re())>wsmall) &&
      (TMath::Abs(fT2->fCtht.Im())<wsmall)) fT2->fCtht.PureReal();
    else fT2->fEva = kTRUE;
    //Contrary to AniToIso, here both waves may very well be evanescent.
    //We must check that if only one wave evanescent,then it is the second
    if ((fT1->fEva) && (!fT2->fEva)) {
      TSComplex cc1,cc2,cc3;
      invers     = kTRUE;
      cc1        = fT2->fRfI;
      cc2        = fT2->fStht;
      cc3        = fT2->fCtht;
      fT2->fRfI  = fT1->fRfI;
      fT2->fStht = fT1->fStht;
      fT2->fCtht = fT1->fCtht;
      fT1->fRfI  = cc1;
      fT1->fStht = cc2;
      fT1->fCtht = cc3;
      fT1->fEva  = kFALSE;
      fT2->fEva  = kTRUE;
    }
    //
    //Determination of alfa (Salf,Calf) and delta (Sdel,Cdel) by solution of the
    //degenerate system (4), (5), (6) [see theoretical paper]. We separate the
    //real and the complex case.
    //
    //1st transmitted wave :
    //
    if (fT1->fEva) {
      TSComplex za,z00,z01,z02,z11,z12,z22;
      za  = fT1->fRfI*fT1->fCtht;
      z00 = g - d00;
      z01 = ns*za + d01;
      z02 = d02;
      z11 = za*za - d11;
      z12 = - d12;
      za  = fT1->fRfI;
      z22 = za*za - d22;
      T33CMatrix M;
      M.SetLine(0,z00,z01,z02);
      M.SetLine(1,z01,z11,z12);
      M.SetLine(2,z02,z12,z22);
      T3CVector v1,v2;
      Short_t tdeg;
      tdeg = v1.DegMatrix(M,v2,gMes->TestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,5,"1st transmitted wave : matrix M (complex) not degenerate");
        fAnomaly = kTRUE;
        return;
      case 2:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,6,"1st transmitted wave : matrix M (complex) twice degenerate");
        fAnomaly = kTRUE;
        return;
      case 3:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,7,"1st transmitted wave : matrix M (complex) is 0");
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      bdeg = v1.AlfaDelta(fT1->fStht,fT1->fCtht,
        fT1->fSalf,fT1->fCalf,fT1->fSdel,fT1->fCdel);
      //bdeg = kFALSE is not a problem
    }
    else {
      //Here, the degenarate matrix M is real !
      Double_t qa,q00,q01,q02,q11,q12,q22;
      qa  = fT1->fRfI.Re()*fT1->fCtht.Re();
      q00 = g - d00;
      q01 = ns*qa + d01;
      q02 = d02;
      q11 = qa*qa - d11;
      q12 = - d12;
      qa  = fT1->fRfI.Re();
      q22 = qa*qa - d22;
      T33Matrix M;
      M.SetLine(0,q00,q01,q02);
      M.SetLine(1,q01,q11,q12);
      M.SetLine(2,q02,q12,q22);
      T3Vector v1,v2;
      Short_t tdeg;
      tdeg = v1.DegMatrix(M,v2,gMes->TestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,8,"1st transmitted wave : matrix M (real) not degenerate");
        fAnomaly = kTRUE;
        return;
      case 2:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,9,"1st transmitted wave : matrix M (real) twice degenerate");
        fAnomaly = kTRUE;
        return;
      case 3:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,10,"1st transmitted wave : matrix M (real) is 0");
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      Double_t qsa,qca,qsd,qcd;
      bdeg = v1.AlfaDelta(fT1->fStht.Re(),fT1->fCtht.Re(),qsa,qca,qsd,qcd);
      //bdeg = kFALSE is not a problem
      fT1->fSalf = qsa;
      fT1->fCalf = qca;
      fT1->fSdel = qsd;
      fT1->fCdel = qcd;
    }
    //
    //2nd transmitted wave :
    //
    if (fT2->fEva) {
      TSComplex za,z00,z01,z02,z11,z12,z22;
      za  = fT2->fRfI*fT2->fCtht;
      z00 = g - d00;
      z01 = ns*za + d01;
      z02 = d02;
      z11 = za*za - d11;
      z12 = - d12;
      za  = fT2->fRfI;
      z22 = za*za - d22;
      T33CMatrix M;
      M.SetLine(0,z00,z01,z02);
      M.SetLine(1,z01,z11,z12);
      M.SetLine(2,z02,z12,z22);
      T3CVector v1,v2;
      Short_t tdeg;
      tdeg = v1.DegMatrix(M,v2,gMes->TestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,11,"2nd transmitted wave : matrix M (complex) not degenerate");
        fAnomaly = kTRUE;
        return;
      case 2:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,12,"2nd transmitted wave : matrix M (complex) twice degenerate");
        fAnomaly = kTRUE;
        return;
      case 3:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,13,"2nd transmitted wave : matrix M (complex) is 0");
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      bdeg = v1.AlfaDelta(fT2->fStht,fT2->fCtht,
        fT2->fSalf,fT2->fCalf,fT2->fSdel,fT2->fCdel);
      //bdeg = kFALSE is not a problem
    }
    else {
      //Here, the degenarate matrix M is real !
      Double_t qa,q00,q01,q02,q11,q12,q22;
      qa  = fT2->fRfI.Re()*fT2->fCtht.Re();
      q00 = g - d00;
      q01 = ns*qa + d01;
      q02 = d02;
      q11 = qa*qa - d11;
      q12 = - d12;
      qa  = fT2->fRfI.Re();
      q22 = qa*qa - d22;
      T33Matrix M;
      M.SetLine(0,q00,q01,q02);
      M.SetLine(1,q01,q11,q12);
      M.SetLine(2,q02,q12,q22);
      T3Vector v1,v2;
      Short_t tdeg;
      tdeg = v1.DegMatrix(M,v2,gMes->TestCalc());
      switch (tdeg) {
      case 0:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,14,"2nd transmitted wave : matrix M (real) not degenerate");
        fAnomaly = kTRUE;
        return;
      case 2:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,15,"2nd transmitted wave : matrix M (real) twice degenerate");
        fAnomaly = kTRUE;
        return;
      case 3:
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,16,"2nd transmitted wave : matrix M (real) is 0");
        fAnomaly = kTRUE;
        return;
      default:
        break;
      }
      Bool_t bdeg;
      Double_t qsa,qca,qsd,qcd;
      bdeg = v1.AlfaDelta(fT2->fStht.Re(),fT2->fCtht.Re(),qsa,qca,qsd,qcd);
      //bdeg = kFALSE is not a problem
      fT2->fSalf = qsa;
      fT2->fCalf = qca;
      fT2->fSdel = qsd;
      fT2->fCdel = qcd;
    }
    //
    //Here are the variables that we have determined until now :
    //
    //    fT1->fRfI
    //    fT1->fStht and fCtht
    //    fT1->fSalf and fCalf
    //    fT1->fSdel and fCdel
    //    fT2->fRfI
    //    fT2->fStht and fCtht
    //    fT2->fSalf and fCalf
    //    fT2->fSdel and fCdel
    //    fR1->fRfI
    //    fR1->fStht and fCtht
    //
    //Here are the variables that remain to be found :
    //
    //    fT1->fE
    //    fT2->fE
    //    fR1->fE
    //    fR1->fSalf and fCalf
    //
    //These last variables are found thanks to equations (9) (10) (11) (12)
    // (See theoretical paper)
    //
    //
    //Solution for fT1->fE and fT2->fE
    //
    zh1  = fPmui/(fI->fRfI.Re()*fPmut);
    dh1  = fI->fCtht.Re();
    fBT1 = fT1->fRfI*zh1;
    fBT2 = fT2->fRfI*zh1;
    fT1->fAlm1 =fT1->fStht*fT1->fSdel + fT1->fCtht*fT1->fCdel;
    fT2->fAlm1 =fT2->fStht*fT2->fSdel + fT2->fCtht*fT2->fCdel;
    TSComplex F12,F13,F22,F23,F32,F33,F42,F43,disc;
    F12  = fBT1*fT1->fCalf*fT1->fAlm1;
    F13  = fBT2*fT2->fCalf*fT2->fAlm1;
    F22  = zero - (fT1->fCdel*fT1->fCalf)/dh1;
    F23  = zero - (fT2->fCdel*fT2->fCalf)/dh1;
    F32  = fT1->fSalf;
    F33  = fT2->fSalf;
    F42  = zero - (fBT1*fT1->fCtht*fT1->fSalf)/dh1;
    F43  = zero - (fBT2*fT2->fCtht*fT2->fSalf)/dh1;
    disc = (F12-F22)*(F33-F43) - (F13-F23)*(F32-F42);
    //
    //We have to divide by disc, so it cannot be extremely small. If we find it
    //extremely small, we take it as a sign that we have made a wrong choice of
    //solution from TEqIndex. For instance, if by error TEqIndex has given twice
    //the same solution, disc will be 0. So we try an other pair of solution.
    //
    if (Abs(disc)<DbLim) continue;
    zh1 = (deux*fI->fE)/disc;
    fT1->fE = zh1*((F33-F43)*fI->fCalf.Re() - (F13-F23)*fI->fSalf.Re());
    fT2->fE = zh1*((F12-F22)*fI->fSalf.Re() - (F32-F42)*fI->fCalf.Re());
    //We do not accept that the real part of the module of an electric field be <0
    if (fT1->fE.Re()<zero) {
      fT1->fE    = zero - fT1->fE;
      fT1->fSalf = zero - fT1->fSalf;
      fT1->fCalf = zero - fT1->fCalf;
      F12        = zero - F12;
      F22        = zero - F22;
      F32        = zero - F32;
      F42        = zero - F42;
    }
    if (fT2->fE.Re()<zero) {
      fT2->fE    = zero - fT2->fE;
      fT2->fSalf = zero - fT2->fSalf;
      fT2->fCalf = zero - fT2->fCalf;
      F13        = zero - F13;
      F23        = zero - F23;
      F33        = zero - F33;
      F43        = zero - F43;
    }
    //
    //Only 2 variables remain to be found :
    //
    //    fR1->fE
    //    fR1->fSalf and fCalf
    //
    TSComplex rca,rsa;
    rca =  F12*fT1->fE + F13*fT2->fE - fI->fE*fI->fCalf.Re();
    rsa =  F32*fT1->fE + F33*fT2->fE - fI->fE*fI->fSalf.Re();
    fR1->fE = Sqrt(rsa*rsa+rca*rca);
    if (fR1->fE.Re()<zero) fR1->fE = zero - fR1->fE;
    fR1->fCalf = rca/fR1->fE;
    fR1->fSalf = rsa/fR1->fE;
    //
    //All the variables of system of equations (B1-B16) have now been found ! We
    //calculate the other physical values depending upon these variables
    //
    //
    //  Last calculations for wave fT1
    //
    // - module and phase of electric field :
    fT1->fE.RPhi(fT1->fAE,fT1->fPE);
    if (TMath::Abs(fT1->fPE)>wsmall) fT1->fPhases += 1;
    else fT1->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    fT1->fCalf.RPhi(fT1->fWcaf,fT1->fPcaf);
    fT1->fSalf.RPhi(fT1->fWsaf,fT1->fPsaf);
    if (TMath::Abs(fT1->fPcaf - fT1->fPsaf)>wsmall)  fT1->fPhases += 1;
    // - angle of refraction complex for the 1st transmitted wave ?
    if (TMath::Abs(fT1->fCtht.Im()) + TMath::Abs(fT1->fStht.Im())>wsmall)
      fT1->fPhases = 3;
    switch (fT1->fPhases) {
    case 0:
    case 1:
      //0 or 1 phase. No reason to choose between parallel and normal component.
      fT1->fUk.Set(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
      //The index of refraction of the 1st transmitted wave cannot be complex in the
      //case where there are only 0 or 1 phase
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
      fT1->fVk  = fT1->fAk*fT1->fUk;
      fT1->fPE += (fT1->fPcaf + fT1->fPsaf)/deux;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fT1->fSdel.Im())>small) ||
        (TMath::Abs(fT1->fCdel.Im())>small)) {
          if (invers) isol[imin2] = kFALSE;
          else        isol[imin1] = kFALSE;
          continue;
      }
      fT1->fSdel.PureReal();
      fT1->fCdel.PureReal();
      fT1->fEf.Set( - fT1->fAE*fT1->fSdel.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fCdel.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fWsaf);
      BuildBPUP(fT1->fVk,fT1->fEf,fT1->fBf,fT1->fPg,fT1->fUp,fPmut);
      //
      //Now comes the decisive test on solution chosen for fT1 : the transmitted
      //wave must go forward ! If not, we have to try an other solution for the
      //index. But we tolerate a small numerical error.
      //
      dh1 = fT1->fUp(0);
      if ((dh1<zero) && (dh1> -wsmall)) {
        dh1 = - dh1;
        fT1->fUp(0) = dh1;
      }
      if (dh1<zero) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      else Good1 = kTRUE;
      break;
    case 2:
      //2 phases. Parallel and normal component have to be calculated separatly.
      //
      //WE SHOULD NEVER HAVE COME HERE !!!
      //
      //  The splitting between parallel and normal components which occurs in
      //isotropic media when the electric field has 2 phases does not occur in
      //anisotropic media : it is replaced by a splitting into 2 different
      //transmitted waves ! To be absolutly sure about that, we still leave this
      //possibility open to the program, but we put an error message if it happens !
      //
      PrintWhich();
      gMes->SetName("IsoToAni");
      gMes->SetTitle("TPhoton");
      gMes->M(Error_M,17,"Electric field fT1->fEf has 2 phases");
      gMes->M(Info_M,17,"This should not happen in anisotropic media");
      fT1->fUk.Set(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
      //The index of refraction of the 1st transmitted wave cannot be complex in the
      //case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fT1->fRfI.Im())>wsmall) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
      fT1->fVk  = fT1->fAk*fT1->fUk;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fT1->fSdel.Im())>small) ||
        (TMath::Abs(fT1->fCdel.Im())>small)) {
          if (invers) isol[imin2] = kFALSE;
          else        isol[imin1] = kFALSE;
          continue;
      }
      fT1->fSdel.PureReal();
      fT1->fCdel.PureReal();
      fT1->fEfp.Set( -fT1->fAE*fT1->fSdel.Re()*fT1->fWcaf,
        fT1->fAE*fT1->fCdel.Re()*fT1->fWcaf,
        zero);
      fT1->fEfn.Set(zero,zero,fT1->fAE*fT1->fWsaf);
      BuildBPUP(fT1->fVk,fT1->fEfp,fT1->fBfp,fT1->fPgp,fT1->fUpp,fPmut);
      BuildBPUP(fT1->fVk,fT1->fEfn,fT1->fBfn,fT1->fPgn,fT1->fUpn,fPmut);
      //
      //Now comes the decisive test on solution chosen for fT1 : the transmitted
      //wave must go forward ! If not, we have to try an other solution for the
      //index. But we tolerate a small numerical error.
      //
      dh1 = fT1->fUpp(0);
      dh2 = fT1->fUpn(0);
      if ((dh1<zero) && (dh1> -wsmall)) {
        dh1 = - dh1;
        fT1->fUp(0) = dh1;
      }
      if ((dh2<zero) && (dh2> -wsmall)) {
        dh2 = - dh2;
        fT1->fUp(0) = dh2;
      }
      if ((dh1<zero) || (dh2<zero)) {
        if (invers) isol[imin2] = kFALSE;
        else        isol[imin1] = kFALSE;
        continue;
      }
      else Good1 = kTRUE;
      break;
    default:
      //
      //Here there are 3 phases. The 1st transmitted wave is absorbed. This is
      //perfectly allowed : this is the case in total reflection, or in the 
      //reflection on a revetment with complex index. If the wave is absorbed,
      //we have nothing to calculate any more for it !
      //
      Good1 = kTRUE;
      break;
    }//end switch
    //
    //  Last calculations for wave fT2
    //
    // - module and phase of electric field :
    fT2->fE.RPhi(fT2->fAE,fT2->fPE);
    if (TMath::Abs(fT2->fPE)>wsmall) fT2->fPhases += 1;
    else fT2->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    fT2->fCalf.RPhi(fT2->fWcaf,fT2->fPcaf);
    fT2->fSalf.RPhi(fT2->fWsaf,fT2->fPsaf);
    if (TMath::Abs(fT2->fPcaf - fT2->fPsaf)>wsmall)  fT2->fPhases += 1;
    // - angle of refraction complex for the 2nd transmitted wave ?
    if (TMath::Abs(fT2->fCtht.Im()) + TMath::Abs(fT2->fStht.Im())>wsmall)
      fT2->fPhases = 3;
    switch (fT2->fPhases) {
    case 0:
    case 1:
      //0 or 1 phase. No reason to choose between parallel and normal component.
      fT2->fUk.Set(fT2->fCtht.Re(),fT2->fStht.Re(),zero);
      //The index of refraction of the 1st transmitted wave cannot be complex in the
      //case where there are only 0 or 1 phase
      if (TMath::Abs(fT2->fRfI.Im())>wsmall) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      fT2->fAk  = (fW*fT2->fRfI.Re())/TLitPhys::Get()->C();
      fT2->fVk  = fT2->fAk*fT2->fUk;
      fT2->fPE += (fT2->fPcaf + fT2->fPsaf)/deux;
      //the angle delta of the decomposition of the electric field cannot be
      //complex in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fT2->fSdel.Im())>small) ||
        (TMath::Abs(fT2->fCdel.Im())>small)) {
          if (invers) isol[imin1] = kFALSE;
          else        isol[imin2] = kFALSE;
          continue;
      }
      fT2->fSdel.PureReal();
      fT2->fCdel.PureReal();
      fT2->fEf.Set( - fT2->fAE*fT2->fSdel.Re()*fT2->fWcaf,
        fT2->fAE*fT2->fCdel.Re()*fT2->fWcaf,
        fT2->fAE*fT2->fWsaf);
      BuildBPUP(fT2->fVk,fT2->fEf,fT2->fBf,fT2->fPg,fT2->fUp,fPmut);
      //
      //Now comes the decisive test on solution chosen for fT2 : the transmitted
      //wave must go forward ! If not, we have to try an other solution for the
      //index. But we tolerate a small numerical error.
      //
      dh1 = fT2->fUp(0);
      if ((dh1<zero) && (dh1> -wsmall)) {
        dh1 = - dh1;
        fT2->fUp(0) = dh1;
      }
      if (dh1<zero) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      else Good2 = kTRUE;
      break;
    case 2:
      //2 phases. Parallel and normal component have to be calculated separatly.
      //
      //WE SHOULD NEVER HAVE COME HERE !!!
      //
      //  The splitting between parallel and normal components which occurs in
      //isotropic media when the electric field has 2 phases does not occur in
      //anisotropic media : it is replaced by a splitting into 2 different
      //transmitted waves ! To be absolutly sure about that, we still leave this
      //possibility open to the program, but we put an error message if it happens !
      //
      PrintWhich();
      gMes->SetName("IsoToAni");
      gMes->SetTitle("TPhoton");
      gMes->M(Error_M,18,"Electric field fT2->fEf has 2 phases");
      gMes->M(Info_M,18,"This should not happen in anisotropic media");
      fT2->fUk.Set(fT2->fCtht.Re(),fT2->fStht.Re(),zero);
      //The index of refraction of the 2nd transmitted wave cannot be complex in
      //the case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fT2->fRfI.Im())>wsmall) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      fT2->fAk  = (fW*fT2->fRfI.Re())/TLitPhys::Get()->C();
      fT2->fVk  = fT2->fAk*fT2->fUk;
      //the angle delta of the decomposition of the electric field cannot be complex,
      //in the case of 0,1 or 2 phases.
      //but we must not be too stringent because of numerical imprecision
      if ((TMath::Abs(fT2->fSdel.Im())>small) ||
        (TMath::Abs(fT2->fCdel.Im())>small)) {
          if (invers) isol[imin1] = kFALSE;
          else        isol[imin2] = kFALSE;
          continue;
      }
      fT2->fSdel.PureReal();
      fT2->fCdel.PureReal();
      fT2->fEfp.Set( -fT2->fAE*fT2->fSdel.Re()*fT2->fWcaf,
        fT2->fAE*fT2->fCdel.Re()*fT2->fWcaf,
        zero);
      fT2->fEfn.Set(zero,zero,fT2->fAE*fT2->fWsaf);
      BuildBPUP(fT2->fVk,fT2->fEfp,fT2->fBfp,fT2->fPgp,fT2->fUpp,fPmut);
      BuildBPUP(fT2->fVk,fT2->fEfn,fT2->fBfn,fT2->fPgn,fT2->fUpn,fPmut);
      //
      //Now comes the decisive test on solution chosen for fT2 : the transmitted
      //wave must go forward ! If not, we have to try an other solution for the
      //index. But we tolerate a small numerical error.
      //
      dh1 = fT2->fUpp(0);
      dh2 = fT2->fUpn(0);
      if ((dh1<zero) && (dh1> -wsmall)) {
        dh1 = - dh1;
        fT2->fUp(0) = dh1;
      }
      if ((dh2<zero) && (dh2> -wsmall)) {
        dh2 = - dh2;
        fT2->fUp(0) = dh2;
      }
      if ((dh1<zero) || (dh2<zero)) {
        if (invers) isol[imin1] = kFALSE;
        else        isol[imin2] = kFALSE;
        continue;
      }
      else Good2 = kTRUE;
      break;
    default:
      //
      //Here there are 3 phases. The 2nd transmitted wave is absorbed. This is
      //perfectly allowed : this is the case in total reflection, or in the 
      //reflection on a revetment with complex index. If the wave is absorbed,
      //we have nothing to calculate any more for it !
      //
      Good2 = kTRUE;
      break;
    }

    //
    //  Last calculations for wave fR1
    //
    //As reflected wave in isotropic medium, angle delta = angle theta
    fR1->fSdel = fR1->fStht;
    fR1->fCdel = fR1->fCtht;
    // - module and phase of electric field :
    fR1->fE.RPhi(fR1->fAE,fR1->fPE);
    if (TMath::Abs(fR1->fPE)>wsmall) fR1->fPhases += 1;
    else fR1->fPE = zero;
    // - module and phase of cos(alfa) and sin(alfa)
    fR1->fCalf.RPhi(fR1->fWcaf,fR1->fPcaf);
    fR1->fSalf.RPhi(fR1->fWsaf,fR1->fPsaf);
    if (TMath::Abs(fR1->fPcaf - fR1->fPsaf)>wsmall)  fR1->fPhases += 1;
    // - angle of refraction complex for the 1st reflective wave ? No !
    if (TMath::Abs(fR1->fCtht.Im()) + TMath::Abs(fR1->fStht.Im())>wsmall)
      fR1->fPhases = 3;
    switch (fR1->fPhases) {
    case 0:
    case 1:
      //Here either no phase, or one global phase. Parallel and normal components
      //of the wave have the same phase. No reason to distinguish between the 2.
      //The reflected wave is not elliptically polarized.
      //Notice the - sign in front of fR1->fCtht.Re() due to the fact that the
      //reflection angle is defined contrary to the trigonometric sense. A reflection
      //angle EQUAL to the incidence angle implies a reflected wave having an
      //OPPOSITE x direction as the incident wave.
      fR1->fUk.Set(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
      //The index of refraction of the 1st reflected wave cannot be complex in the
      //case where there are only 0, 1 or 2 phase
      if (TMath::Abs(fR1->fRfI.Im())>wsmall) continue;
      //Module of k vector
      fR1->fAk  = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR1->fVk  = fR1->fAk*fR1->fUk;
      //phase of electric field
      fR1->fPE += (fR1->fPcaf + fR1->fPsaf)/deux;
      //3 components of electric field
      fR1->fEf.Set(- fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
        - fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
        fR1->fAE*fR1->fWsaf);
      BuildBPUP(fR1->fVk,fR1->fEf,fR1->fBf,fR1->fPg,fR1->fUp,fPmui);
      //
      //reflected wave must go backward
      //
      dh1 = fR1->fUp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR1->fUp(0) = dh1;
      }
      if (dh1>zero) {
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,19,"reflected wave going forward",dh1);
        fAnomaly = kTRUE;
        return;
      }
      //Verify that k vector and electric field are orthogonal : isotropic medium !
      if (gMes->TestCalc()) {
        vh1 = fR1->fEf;
        vh1.Unit();
        dh1 = TMath::Abs(vh1*fR1->fUk);
        if (dh1>small) {
          PrintWhich();
          gMes->SetName("IsoToAni");
          gMes->SetTitle("TPhoton");
          gMes->M(kill_this,20,"k and E not orthogonal",dh1);
          fAnomaly = kTRUE;
          return;
        }
      }
      break;
    case 2:
      //2 phases. Parallel and normal component have to be calculated separatly.
      //
      //Notice the - sign in front of fR1->fCtht due to the fact that the reflection
      //angle is defined contrary to the trigonometric sense. A reflection angle
      //EQUAL to the incidence angle implies a reflected wave having an OPPOSITE x
      //direction as the incident wave.
      //
      fR1->fUk.Set(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
      //The index of refraction of the reflected wave cannot be complex in the
      //case where there are only 0, 1 or 2 phases
      if (TMath::Abs(fR1->fRfI.Im())>wsmall) {
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,21,"Index unduly complex",fR1->fRfI);
        fAnomaly = kTRUE;
        return;
      }
      //Module of k vector
      fR1->fAk = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
      //k vector
      fR1->fVk = fR1->fAk*fR1->fUk;
      //electric field
      fR1->fEfp.Set(- fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
        - fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
        zero);
      fR1->fEfn.Set(zero,zero,fR1->fAE*fR1->fWsaf);
      BuildBPUP(fR1->fVk,fR1->fEfp,fR1->fBfp,fR1->fPgp,fR1->fUpp,fPmui);
      BuildBPUP(fR1->fVk,fR1->fEfn,fR1->fBfn,fR1->fPgn,fR1->fUpn,fPmui);
      //
      //reflected wave must go backward
      //
      dh1 = fR1->fUpp(0);
      if ((dh1>zero) && (dh1<wsmall)) {
        dh1 = - dh1;
        fR1->fUpp(0) = dh1;
      }
      dh2 = fR1->fUpn(0);
      if ((dh2>zero) && (dh2<wsmall)) {
        dh2 = - dh2;
        fR1->fUpn(0) = dh2;
      }
      if ((dh1>zero) || (dh2>zero)) {
        PrintWhich();
        gMes->SetName("IsoToAni");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,22,"reflected wave going forward",dh1,dh2);
        fAnomaly = kTRUE;
        return;
      }
      //Verify that k vector and electric field are orthogonal : isotropic medium !
      if (gMes->TestCalc()) {
        vh1 = fR1->fEfp;
        vh1.Unit();
        dh1 = TMath::Abs(vh1*fR1->fUk);
        vh1 = fR1->fEfn;
        vh1.Unit();
        dh2 = TMath::Abs(vh1*fR1->fUk);
        if ((dh1>small) || (dh2>small)) {
          PrintWhich();
          gMes->SetName("AniToIso");
          gMes->SetTitle("TPhoton");
          gMes->M(kill_this,23,"k and E not orthogonal",dh1,dh2);
          fAnomaly = kTRUE;
          return;
        }
      }
      break;
    default:
      //Here there are 3 phases so that there would be no reflected wave. This
      //is impossible.
      PrintWhich();
      gMes->SetName("IsoToAni");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,24,"Reflected wave absorbed ?");
      fAnomaly = kTRUE;
      return;
      break;
    }//end switch (fR1->fPhases)
  }// end do while
  while (((!Good1) || (!Good2)) && (NumberOfTry<6));
  //Verify that the system of 12 equations has been solved properly
  if (gMes->TestCalc()) VerifyItoA();
  if (fAnomaly) return;
  //
  //  After all these complicate calculations, we have at hand a powerful check
  //that everything is ok : we can verify that the flow of energy across the
  //boundary between the 2 media is conserved! Notice that if a transmitted
  //wave is absorbed because of a complex index of refraction of the "out" medium
  //then the energy flux is not conserved ! But if a transmitted wave is 
  //absorbed because of total reflection on an "out" medium with a real index
  //of refraction, then the energy flux is conserved !
  //
  //  We HAVE to do this test in any case, since a non-conservation of flux
  //may be the sign that we have taken a bad pair of solutions and that we
  //have to try an other one. ( We leave the "if ( 1 || gMes->TestCalc())"
  //in the case that it appears experimentally that it never happens. In
  //that case, we could render the test optional. )
  //
  if ( 1 /* gMes->TestCalc() */ ) {
    gMes->SetName("IsoToAni");
    gMes->SetTitle("TPhoton");
    Double_t outflux = 0.0;
    Double_t influx,dflux;
    influx = fI->fPg(0);
    switch (fR1->fPhases) {
    case 0:
    case 1:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 1 | fT1 : 1 | fT2 : 1
      outflux = fT1->fPg(0) + fT2->fPg(0) - fR1->fPg(0);
      break;
    case 2:
      // fR1 : 1 | fT1 : 1 | fT2 : 2   !!! theoretically impossible         !!!
      outflux = fT1->fPg(0) + fT2->fPgn(0) + fT2->fPgp(0) - fR1->fPg(0);
      break;
    default:
      // fR1 : 1 | fT1 : 1 | fT2 : 3
      //
      //  2nd transmitted wave is absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a revetment whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      outflux = fT1->fPg(0) - fR1->fPg(0);
      break;
      }//end switch (fT2->fPhases)
      break;
    case 2:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 1 | fT1 : 2 | fT2 : 1   !!! fT1 : 2 theoretically impossible !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) + fT2->fPg(0) - fR1->fPg(0);
      break;
    case 2:
      // fR1 : 1 | fT1 : 2 | fT2 : 2   !!! fT1/2:2 theoretically impossible !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) + 
        fT2->fPgn(0) + fT2->fPgp(0) - fR1->fPg(0);
      break;
    default:
      // fR1 : 1 | fT1 : 2 | fT2 : 3   !!! fT1 : 2 theoretically impossible !!!
      //
      //  2nd transmitted wave is absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a revetment whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPg(0);
      break;
      }//end switch (fT2->fPhases)
      break;
    default:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 1 | fT1 : 3 | fT2 : 1   !!! fT1 and fT2 out of order         !!!
      //
      //  1st transmitted wave is absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a revetment whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      PrintWhich();
      gMes->M(Error_M,25,"fT1 and fT2 out of order");
      outflux = fT2->fPg(0) - fR1->fPg(0);
      break;
    case 2:
      // fR1 : 1 | fT1 : 3 | fT2 : 2   !!! fT1 and fT2 out of order         !!!
      //                               !!! fT2 : 2 theoretically impossible !!!
      //
      //  1st transmitted wave is absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a revetment whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      PrintWhich();
      gMes->M(Error_M,26,"fT1 and fT2 out of order");
      outflux = fT2->fPgn(0) + fT2->fPgp(0) - fR1->fPg(0);
      break;
    default:
      // fR1 : 1 | fT1 : 3 | fT2 : 3
      //
      //  Both transmitted wave are absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a revetment whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      outflux = - fR1->fPg(0);
      break;
      }//end switch (fT2->fPhases)
      break;
      }//end switch (fT1->fPhases)
      break;
    case 2:
      switch (fT1->fPhases) {
    case 0:
    case 1:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 2 | fT1 : 1 | fT2 : 1
      outflux = fT1->fPg(0) + fT2->fPg(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    case 2:
      // fR1 : 2 | fT1 : 1 | fT2 : 2
      outflux = fT1->fPg(0) + fT2->fPgn(0) + fT2->fPgp(0) -
        fR1->fPgn(0) - fR1->fPgp(0);
      break;
    default:
      // fR1 : 2 | fT1 : 1 | fT2 : 3
      outflux = fT1->fPg(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
      }//end switch (fT2->fPhases)
      break;
    case 2:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 2 | fT1 : 2 | fT2 : 1   !!! fT1 : 2 theoretically impossible !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) + fT2->fPg(0)
        - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    case 2:
      // fR1 : 2 | fT1 : 2 | fT2 : 2   !!! fT1/2:2 theoretically impossible !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) + fT2->fPgn(0) + fT2->fPgp(0)
        - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    default:
      // fR1 : 2 | fT1 : 2 | fT2 : 3   !!! fT1 : 2 theoretically impossible !!!
      outflux = fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
      }//end switch (fT2->fPhases)
      break;
    default:
      switch (fT2->fPhases) {
    case 0:
    case 1:
      // fR1 : 2 | fT1 : 3 | fT2 : 1   !!! fT1 and fT2 out of order         !!!
      //
      //  1st transmitted wave is absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a revetment whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      PrintWhich();
      gMes->M(Error_M,27,"fT1 and fT2 out of order");
      outflux = fT2->fPg(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    case 2:
      // fR1 : 2 | fT1 : 3 | fT2 : 2   !!! fT1 and fT2 out of order         !!!
      //                               !!! fT2 : 2 theoretically impossible !!!
      PrintWhich();
      gMes->M(Error_M,28,"fT1 and fT2 out of order");
      outflux = fT2->fPgn(0) + fT2->fPgp(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    default:
      // fR1 : 2 | fT1 : 3 | fT2 : 3
      //
      //  Both transmitted wave are absorbed. Notice this strange behaviour of aniso-
      //pic media : even in the case where the index of refraction of the
      //transmitted wave is complex, the energy flux is conserved. The wave whose
      //index is complex does not transport energy. This is in contrast to the
      //case of a wave hitting a revetment whose index is complex : in that case
      //the evanescent transmitted wave DOES carry energy  and the energy flux is
      //not conserved !
      //
      outflux = - fR1->fPgn(0) - fR1->fPgp(0);
      break;
      }//end switch (fT2->fPhases)
      break;
      }//end switch (fT1->fPhases)
      break;
    default:
      // fR1 : 3  an error report for this case has already been given
      break;
    }//end switch (fR1->fPhases)
    dflux = influx - outflux;
    if (TMath::Abs(dflux)>dixm4) {
      PrintWhich();
      gMes->M(Error_M,29,"flux not conserved",dflux);
      gMes->M(Info_M,29,"This may be due to a bad choice of solutions from TEqIndex");
      gMes->M(Info_M,29,"We try an other choice");
      if (NumberOfTry<7) goto bigloop;
      else {
        gMes->M(kill_this,30,"all solutions tried without success",fKphot);
        fAnomaly = kTRUE;
        return;
      }
    }
  }//end if (gMes->TestCalc())
}
void TPhoton::IsoToIso() {
  //Calculates the transition from an isotropic medium towards an isotropic
  //medium. There is no thin slice in-between.
  const Double_t zero   = 0.0;
  const Double_t deux   = 2.0;
  const Double_t msmall = 1.0e-6;
  const Double_t small  = 1.0e-8;
  const Double_t vsmall = 1.0e-10;
  const Double_t wsmall = 1.0e-11;
  Double_t aa,bb,cc,TestIm;
  Double_t dh1,dh2;
  TSComplex a,b;
  TSComplex delta,cnum1,cnum2,cden1,cden2;
  TSComplex tsa,tca,rsa,rca;   //T*sin(alf) T*cos(alf) R*sin(alf) R*cos(alf)
  fR1->fShare  = NotTaken;    //we don't know yet whether this wave will be taken
  fR2->fShare  = NotInvolved; //no 2nd reflected wave in an isotropic medium
  fT1->fShare  = NotTaken;    //we don't know yet whether this wave will be taken
  fT2->fShare  = NotInvolved; //no 2nd transmitted wave in an isotropic medium
  fR1->fPhases = 0;           //initializes at 0 the number of phases
  fT1->fPhases = 0;           //initializes at 0 the number of phases
  fI->fRfI     = fRfIndexi;   //assigns to each wave its index of refraction
  fR1->fRfI    = fRfIndexi;
  fT1->fRfI    = fRfIndext;
  fR1->fCtht   = fI->fCtht;
  fR1->fStht   = fI->fStht;   //angle of reflection == angle of incidence : REAL
  //Fresnel or Descartes formula, between incident and transmitted
  fT1->fStht = (fI->fRfI.Re()*fI->fStht.Re())/fT1->fRfI;
  fT1->fCtht = fT1->fStht;
  //
  //  Notice this important point :
  //   The imaginary part of fT1->fCtht MUST be negative in order to have
  //absorption and not explosion of the transmitted wave. This is why CosFromSin
  //always chooses the solution with negative imaginary part.
  //
  fT1->fCtht.CosFromSin();
  //Here come the calculations involving electric fields of the waves
  //fT1->fRfI, fT1->fCtht may be complex.
  delta = (fPmui*fT1->fRfI)/(fPmut*fI->fRfI.Re());
  a     = delta*fI->fCtht.Re();
  b     = delta*fT1->fCtht;
  cnum1 = a - fT1->fCtht;
  cden1 = a + fT1->fCtht;
  cnum2 = fI->fCtht.Re() - b;
  cden2 = fI->fCtht.Re() + b;
  aa    = fI->fAE*fI->fCalf.Re();
  bb    = fI->fAE*fI->fSalf.Re();
  cc    = deux*fI->fCtht.Re();
  tca   = (aa*cc)/cden1;
  tsa   = (bb*cc)/cden2;
  rca   = (aa*cnum1)/cden1;
  rsa   = (bb*cnum2)/cden2;
  //
  //We calculate everything on the transmitted wave
  //
  fT1->fE  = Sqrt(tsa*tsa+tca*tca);
  fT1->fE.RPhi(fT1->fAE,fT1->fPE);
  if (TMath::Abs(fT1->fPE)>wsmall) fT1->fPhases += 1;
  else fT1->fPE = zero;
  fT1->fCalf = tca/fT1->fE;
  fT1->fSalf = tsa/fT1->fE;
  fT1->fCalf.RPhi(fT1->fWcaf,fT1->fPcaf);
  fT1->fSalf.RPhi(fT1->fWsaf,fT1->fPsaf);
  if (TMath::Abs(fT1->fPcaf-fT1->fPsaf)>wsmall) fT1->fPhases +=  1;
  TestIm = TMath::Abs(fT1->fCtht.Im()) + TMath::Abs(fT1->fStht.Im()) +
    TMath::Abs(fT1->fRfI.Im());
  if (TestIm>wsmall) fT1->fPhases = 3;
  switch (fT1->fPhases) {
  case 0:
  case 1:
    //Here either no phase, or one global phase. Parallel and normal components
    //of the wave have the same phase. No reason to distinguish between the 2.
    //The transmitted wave is not elliptically polarized.
    fT1->fUk.Set(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
    if (gMes->TestCalc()) {
      //If the phases are 0,1 or 2, it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>vsmall) {
        PrintWhich();
        gMes->SetName("IsoToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,1,"Index unduly complex",fT1->fRfI);
        fAnomaly = kTRUE;
        return;
      }
    }
    fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
    fT1->fVk  = fT1->fAk*fT1->fUk;
    fT1->fPE += (fT1->fPcaf + fT1->fPsaf)/deux;
    fT1->fEf.Set(- fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fWsaf);
    BuildBPUP(fT1->fVk,fT1->fEf,fT1->fBf,fT1->fPg,fT1->fUp,fPmut);
    //
    //The transmitted wave must go forward.
    //
    dh1 = fT1->fUp(0);
    if ((dh1<zero) && (dh1> -wsmall)) {
      dh1 = - dh1;
      fT1->fUp(0) = dh1;
    }
    if (dh1<zero) {
      PrintWhich();
      gMes->SetName("IsoToIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,2,"Transmitted wave going backward",dh1);
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 2:
    //  Here, there are 2 phases. Parallel and normal component have different phases,
    //so that they have to be calculated separatly. Later on, we will have to choose
    //between these 2 waves.
    fT1->fUk.Set(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
    if (gMes->TestCalc()) {
      //If there are 2 phases, it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>vsmall) {
        PrintWhich();
        gMes->SetName("IsoToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,3,"Index unduly complex",fT1->fRfI);
        fAnomaly = kTRUE;
        return;
      }
    }
    fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
    fT1->fVk  = fT1->fAk*fT1->fUk;
    fT1->fEfp.Set(-fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
      zero);
    fT1->fEfn.Set(zero,zero,fT1->fAE*fT1->fWsaf);
    BuildBPUP(fT1->fVk,fT1->fEfp,fT1->fBfp,fT1->fPgp,fT1->fUpp,fPmut);
    BuildBPUP(fT1->fVk,fT1->fEfn,fT1->fBfn,fT1->fPgn,fT1->fUpn,fPmut);
    //
    //The transmitted wave must go forward.
    //
    dh1 = fT1->fUpp(0);
    dh2 = fT1->fUpn(0);
    if ((dh1<zero) && (dh1> -wsmall)) {
      dh1 = - dh1;
      fT1->fUpp(0) = dh1;
    }
    if ((dh2<zero) && (dh2> -wsmall)) {
      dh2 = - dh2;
      fT1->fUpn(0) = dh2;
    }
    if ((dh1<zero) || (dh2<zero)) {
      PrintWhich();
      gMes->SetName("IsoToIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,4,"Transmitted wave going backward",dh1,dh2);
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 3:
    //  Here there are 3 phases so that the wave is absorbed. No reason to calculate
    //anything
    break;
  default:
    PrintWhich();
    gMes->SetName("IsoToIso");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,5,"abnormal Nb. of phases for transmitted wave",fT1->fPhases);
    fAnomaly = kTRUE;
    return;
  }
  //
  //We calculate everything on the reflected wave
  //
  fR1->fE  = Sqrt(rsa*rsa+rca*rca);
  fR1->fE.RPhi(fR1->fAE,fR1->fPE);
  if (TMath::Abs(fR1->fPE)>wsmall) fR1->fPhases += 1;
  else fR1->fPE = zero;
  fR1->fCalf = rca/fR1->fE;
  fR1->fSalf = rsa/fR1->fE;
  fR1->fCalf.RPhi(fR1->fWcaf,fR1->fPcaf);
  fR1->fSalf.RPhi(fR1->fWsaf,fR1->fPsaf);
  if (TMath::Abs(fR1->fPcaf-fR1->fPsaf)>wsmall) fR1->fPhases +=  1;
  //A reflected wave is never absorbed
  switch (fR1->fPhases) {
  case 0:
  case 1:
    //Here either no phase, or one global phase. Parallel and normal components
    //of the wave have the same phase. No reason to distinguish between the 2.
    //The reflected wave is not elliptically polarized.
    //Notice the - sign in front of fR1->fCtht.Re() due to the fact that the
    //reflection angle is defined contrary to the trigonometric sense. A reflection
    //angle EQUAL to the incidence angle implies a reflected wave having an
    //OPPOSITE x direction as the incident wave.
    fR1->fUk.Set(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
    fR1->fAk  = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
    fR1->fVk  = fR1->fAk*fR1->fUk;
    fR1->fPE += (fR1->fPcaf + fR1->fPsaf)/deux;
    fR1->fEf.Set(- fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
      - fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
      fR1->fAE*fR1->fWsaf);
    BuildBPUP(fR1->fVk,fR1->fEf,fR1->fBf,fR1->fPg,fR1->fUp,fPmui);
    //
    //The reflective wave must go backward !
    //
    dh1 = fR1->fUp(0);
    if ((dh1>zero) && (dh1<wsmall)) {
      dh1 = - dh1;
      fR1->fUp(0) = dh1;
    }
    if (dh1>zero) {
      PrintWhich();
      gMes->SetName("IsoToIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,6,"Reflected wave going forward !",dh1);
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 2:
    //  Here, there are 2 phases. Parallel and normal component have different phases,
    //so that they have to be calculated separatly. Later on, we will have to choose
    //between these 2 waves.
    fR1->fUk.Set(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
    fR1->fAk  = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
    fR1->fVk  = fR1->fAk*fR1->fUk;
    fR1->fEfp.Set(-fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
      -fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
      zero);
    fR1->fEfn.Set(zero,zero,fR1->fAE*fR1->fWsaf);
    BuildBPUP(fR1->fVk,fR1->fEfp,fR1->fBfp,fR1->fPgp,fR1->fUpp,fPmut);
    BuildBPUP(fR1->fVk,fR1->fEfn,fR1->fBfn,fR1->fPgn,fR1->fUpn,fPmut);
    //
    //The reflective wave must go backward !
    //
    dh1 = fR1->fUpp(0);
    dh2 = fR1->fUpn(0);
    if ((dh1>zero) && (dh1<wsmall)) {
      dh1 = - dh1;
      fR1->fUpp(0) = dh1;
    }
    if ((dh2>zero) && (dh2<wsmall)) {
      dh2 = - dh2;
      fR1->fUpn(0) = dh2;
    }
    if ((dh1>zero) || (dh2>zero)) {
      PrintWhich();
      gMes->SetName("IsoToIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,7,"Reflected wave going forward !",dh1,dh2);
      fAnomaly = kTRUE;
      return;
    }
    break;
  default:
    PrintWhich();
    gMes->SetName("IsoToIso");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,8,"abnormal Nb. of phases for reflected wave",fR1->fPhases);
    fAnomaly = kTRUE;
    return;
  }
  //
  //  After all these complicate calculations, we have at hand an important check
  //that everything is ok : we can verify that the flow of energy across the
  //boundary between the 2 media is conserved. Notice that if the transmitted
  //wave is absorbed because of a complex index of refraction of the "out" medium
  //then the energy flux is not conserved ! But if the transmitted wave is 
  //absorbed because of total reflection on an "out" medium with a real index
  //of refraction, then the energy flux is conserved !
  //
  if (gMes->TestCalc()) {
    Bool_t ToCheck = kTRUE;
    Double_t flux;
    Double_t dflux = 0;
    flux = fI->fPg(0);
    switch (fR1->fPhases) {
    case 0:
    case 1:
      //reflected wave not elliptically polarized
      switch (fT1->fPhases) {
    case 0:
    case 1:
      //reflected only absolut phase  --- transmitted wave only absolut phase
      dflux = flux - (fT1->fPg(0)  - fR1->fPg(0));
      break;
    case 2:
      //reflected only absolut phase  --- transmitted wave with 2 phases
      dflux = flux - (fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPg(0));
      break;
    case 3:
      //reflected only absolut phase  --- transmitted wave is absorbed
      if (TMath::Abs(fT1->fRfI.Im())<small) {
        dflux = flux + fR1->fPg(0);
      }
      else ToCheck = kFALSE;
      break;
    default:
      //impossible value of fT1->fPhases
      PrintWhich();
      gMes->SetName("IsoToIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,9,"abnormal Nb. of phases for transmitted wave",fT1->fPhases);
      fAnomaly = kTRUE;
      return;
      }
      break;
    case 2:
      //reflected wave elliptically polarized
      switch (fT1->fPhases) {
    case 0:
    case 1:
      //reflected wave with 2 phases  --- transmitted wave only absolut phase
      dflux = flux - (fT1->fPg(0)  - fR1->fPgn(0) - fR1->fPgp(0));
      break;
    case 2:
      //reflected wave with 2 phases  --- transmitted wave with 2 phases
      dflux = flux -
        (fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPgn(0) - fR1->fPgp(0));
      break;
    case 3:
      //reflected wave with 2 phases  --- transmitted wave is absorbed
      if (TMath::Abs(fT1->fRfI.Im())<small) {
        dflux = flux + fR1->fPgn(0) + fR1->fPgp(0);
      }
      else ToCheck = kFALSE;
      break;
    default:
      //impossible value of fT1->fPhases
      PrintWhich();
      gMes->SetName("IsoToIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,10,"abnormal Nb. of phases for transmitted wave",fT1->fPhases);
      fAnomaly = kTRUE;
      return;
      }
      break;
    default:
      //reflected wave absorbed ? Impossible !
      PrintWhich();
      gMes->SetName("IsoToIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,11,"reflected wave absorbed !");
      fAnomaly = kTRUE;
      return;
    }
    if (ToCheck) {
      if (TMath::Abs(dflux)>msmall) {
        PrintWhich();
        gMes->SetName("IsoToIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,12,"Energy flux not conserved",dflux);
        fAnomaly = kTRUE;
        return;
      }
    }
  }
}//end IsoToIso
void TPhoton::IsoTsIso() {
  //Calculates the transition from an isotropic medium towards an isotropic
  //medium. There IS a thin slice in-between.
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t msmall = 1.0e-6;
  const Double_t small  = 1.0e-8;
  const Double_t vsmall = 1.0e-10;
  const Double_t wsmall = 1.0e-11;
  TSComplex I(zero,un);
  Double_t aa,dh1,dh2,TestIm;      //for intermadiate calculations
  TSComplex ca,cb,cc,cd,ce,cf,cg; //for intermediate calculations
  TSComplex csthtv;  //sin of angle of refraction inside thin slice. Is always real, but may be > 1.
  TSComplex ccthtv;  //cos of angle of refraction inside thin slice. May be pure imaginary.
  Double_t thsd;    //width of thin slice in [cm]
  Double_t thsri;   //refraction index of thin slice, always real
  Double_t thsmu;   //magnetic permability of thin slice
  TSComplex betat;   //= (nt/mut) / (nc/muc). Complex if nt complex.
  Double_t betac;   //= (nc/muc) / (ni/mui). Always real.
  TSComplex phi1;    //phase inside thin slice due to width of thin slice. Imaginary if ccthtv imaginary.
  TSComplex sphi1;   //sin of phi1
  TSComplex cphi1;   //cos of phi1
  TSComplex phi2;    //phase exit of thin slice due to width of thin slice. Complex in case of fT1->fRfI or fT1->fCtht complex.
  TSComplex ephi2;   // = exp(+i*phi2)
  TSComplex cnum1,cnum2; //the 2 complex numerators
  TSComplex cden1;   //complex denominator
  TSComplex tsa,tca,rsa,rca;   //T*sin(alf) T*cos(alf) R*sin(alf) R*cos(alf)

  fR1->fShare  = NotTaken;    //we don't know yet whether this wave will be taken
  fR2->fShare  = NotInvolved; //no 2nd reflected wave in an isotropic medium
  fT1->fShare  = NotTaken;    //we don't know yet whether this wave will be taken
  fT2->fShare  = NotInvolved; //no 2nd transmitted wave in an isotropic medium
  fR1->fPhases = 0;           //initializes at 0 the number of phases
  fT1->fPhases = 0;           //initializes at 0 the number of phases
  fI->fRfI     = fRfIndexi;   //assigns to each wave its index of refraction
  fR1->fRfI    = fRfIndexi;
  fT1->fRfI    = fRfIndext;
  fR1->fCtht   = fI->fCtht;
  fR1->fStht   = fI->fStht;   //angle of reflection == angle of incidence : REAL
  //Fresnel or Descartes formula, between incident and transmitted
  fT1->fStht = (fI->fRfI.Re()*fI->fStht.Re())/fT1->fRfI;
  fT1->fCtht = fT1->fStht;
  //
  //  Notice this important point :
  //   The imaginary part of fT1->fCtht MUST be negative in order to have
  //absorption and not explosion of the transmitted wave. This is why CosFromSin
  //always chooses the solution with negative imaginary part.
  //
  fT1->fCtht.CosFromSin();
  //Fresnel or Descartes formula, between incident and thin slice. Everything on
  //the right side of = is real, so csthtv will be real, but may be >1 !
  thsri  = fThinSlice->GetIndex();
  thsmu  = fThinSlice->GetMu();
  thsd   = fThinSlice->GetWidth();
  csthtv = (fI->fRfI.Re()*fI->fStht.Re())/thsri;
  ccthtv = csthtv;
  //either ccthtv is real (if csthtv<=1) or pure imaginary (if csthtv>1)
  ccthtv.CosFromSin();
  //the 2 quotients of indices and magnetic permeability.
  betac  = (thsri*fPmui)/(fI->fRfI.Re()*thsmu);
  betat  = (fT1->fRfI*thsmu)/(thsri*fPmut);
  //  [fW] = ps-1   [thsd] = cm   [TLitPhys::Get()->C()] = cm/ps   ==> [aa] = 0
  aa     = (fW*thsd)/TLitPhys::Get()->C();
  phi1   = aa*thsri*ccthtv;
  phi2   = aa*fT1->fRfI*fT1->fCtht;
  sphi1  = Sin(phi1);
  cphi1  = Cos(phi1);
  ephi2  = Exp(I*phi2);
  //Wave inside incident plane
  ce     = betac*betat*fI->fCtht.Re();
  cf     = betac*fI->fCtht.Re()*fT1->fCtht;
  cg     = betat*ccthtv*ccthtv;
  ca     = ce - fT1->fCtht;
  cb     = cf - cg;
  cc     = ce + fT1->fCtht;
  cd     = cf + cg;
  ce     = cphi1*ccthtv;
  cf     = I*sphi1;
  cnum1  = ce*ca+cf*cb;
  cden1  = ce*cc+cf*cd;
  cnum2  = deux*fI->fCtht.Re()*ccthtv*ephi2;
  rca    = (fI->fAE*fI->fWcaf*cnum1)/cden1;
  tca    = (fI->fAE*fI->fWcaf*cnum2)/cden1;
  //Wave normal to incident plane
  ce    = betac*betat*fT1->fCtht;
  cf    = betac*ccthtv*ccthtv;
  cg    = betat*fT1->fCtht*fI->fCtht.Re();
  ca    = fI->fCtht.Re() - ce;
  cb    = cg - cf;
  cc    = fI->fCtht.Re() + ce;
  cd    = cg + cf;
  ce    = cphi1*ccthtv;
  cf    = I*sphi1;
  cnum1 = ce*ca+cf*cb;
  cden1 = ce*cc+cf*cd;
  cnum2 = deux*fI->fCtht.Re()*ccthtv*ephi2;
  rsa   = (fI->fAE*fI->fWsaf*cnum1)/cden1;
  tsa   = (fI->fAE*fI->fWsaf*cnum2)/cden1;
  //
  //We calculate everything on the transmitted wave
  //
  fT1->fE  = Sqrt(tsa*tsa+tca*tca);
  fT1->fE.RPhi(fT1->fAE,fT1->fPE);
  if (TMath::Abs(fT1->fPE)>wsmall) fT1->fPhases += 1;
  else fT1->fPE = zero;
  fT1->fCalf = tca/fT1->fE;
  fT1->fSalf = tsa/fT1->fE;
  fT1->fCalf.RPhi(fT1->fWcaf,fT1->fPcaf);
  fT1->fSalf.RPhi(fT1->fWsaf,fT1->fPsaf);
  if (TMath::Abs(fT1->fPcaf-fT1->fPsaf)>wsmall) fT1->fPhases +=  1;
  TestIm = TMath::Abs(fT1->fCtht.Im()) + TMath::Abs(fT1->fStht.Im()) +
    TMath::Abs(fT1->fRfI.Im());
  if (TestIm>wsmall) fT1->fPhases = 3;
  switch (fT1->fPhases) {
  case 0:
  case 1:
    //Here either no phase, or one global phase. Parallel and normal components
    //of the wave have the SAME phase. No reason to distinguish between the 2.
    //The transmitted wave is not elliptically polarized.
    fT1->fUk.Set(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
    if (gMes->TestCalc()) {
      //If the phases are 0 or 1, it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>vsmall) {
        PrintWhich();
        gMes->SetName("IsoTsIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,1,"Index unduly complex",fT1->fRfI);
        fAnomaly = kTRUE;
        return;
      }
    }
    fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
    fT1->fVk  = fT1->fAk*fT1->fUk;
    fT1->fPE += (fT1->fPcaf + fT1->fPsaf)/deux;
    fT1->fEf.Set(- fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fWsaf);
    BuildBPUP(fT1->fVk,fT1->fEf,fT1->fBf,fT1->fPg,fT1->fUp,fPmut);
    //
    //The transmitted wave must go forward.
    //
    dh1 = fT1->fUp(0);
    if ((dh1<zero) && (dh1> -wsmall)) {
      dh1 = - dh1;
      fT1->fUp(0) = dh1;
    }
    if (dh1<zero) {
      PrintWhich();
      gMes->SetName("IsoTsIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,2,"Transmitted wave going backward",dh1);
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 2:
    //  Here, there are 2 phases. Parallel and normal component have different phases,
    //so that they have to be calculated separatly. Later on, we will have to choose
    //between these 2 waves.
    fT1->fUk.Set(fT1->fCtht.Re(),fT1->fStht.Re(),zero);
    if (gMes->TestCalc()) {
      //If there are 2 phases, it is totally excluded that the index of refraction
      //be complex. If it were complex, the number of phases would be 3 and the wave
      //absorbed.
      if (TMath::Abs(fT1->fRfI.Im())>vsmall) {
        PrintWhich();
        gMes->SetName("IsoTsIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,3,"Index unduly complex",fT1->fRfI);
        fAnomaly = kTRUE;
        return;
      }
    }
    fT1->fAk  = (fW*fT1->fRfI.Re())/TLitPhys::Get()->C();
    fT1->fVk  = fT1->fAk*fT1->fUk;
    //electric field
    fT1->fEfp.Set(-fT1->fAE*fT1->fStht.Re()*fT1->fWcaf,
      fT1->fAE*fT1->fCtht.Re()*fT1->fWcaf,
      zero);
    fT1->fEfn.Set(zero,zero,fT1->fAE*fT1->fWsaf);
    BuildBPUP(fT1->fVk,fT1->fEfp,fT1->fBfp,fT1->fPgp,fT1->fUpp,fPmut);
    BuildBPUP(fT1->fVk,fT1->fEfn,fT1->fBfn,fT1->fPgn,fT1->fUpn,fPmut);
    //
    //The transmitted wave must go forward.
    //
    dh1 = fT1->fUpp(0);
    dh2 = fT1->fUpn(0);
    if ((dh1<zero) && (dh1> -wsmall)) {
      dh1 = - dh1;
      fT1->fUpp(0) = dh1;
    }
    if ((dh2<zero) && (dh2> -wsmall)) {
      dh2 = - dh2;
      fT1->fUpn(0) = dh2;
    }
    if ((dh1<zero) || (dh2<zero)) {
      PrintWhich();
      gMes->SetName("IsoTsIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,4,"Transmitted wave going backward",dh1,dh2);
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 3:
    //  Here there are 3 phases so that the wave is absorbed. No reason to calculate
    //anything
    break;
  default:
    PrintWhich();
    gMes->SetName("IsoTsIso");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,5,"abnormal Nb. of phases for transmitted wave",fT1->fPhases);
    fAnomaly = kTRUE;
    return;
  }
  //
  //We calculate everything on the reflected wave
  //
  fR1->fE    = Sqrt(rsa*rsa+rca*rca);
  fR1->fE.RPhi(fR1->fAE,fR1->fPE);
  if (TMath::Abs(fR1->fPE)>wsmall) fR1->fPhases += 1;
  else fR1->fPE = zero;
  fR1->fCalf = rca/fR1->fE;
  fR1->fSalf = rsa/fR1->fE;
  fR1->fCalf.RPhi(fR1->fWcaf,fR1->fPcaf);
  fR1->fSalf.RPhi(fR1->fWsaf,fR1->fPsaf);
  if (TMath::Abs(fR1->fPcaf-fR1->fPsaf)>wsmall) fR1->fPhases +=  1;
  //A reflected wave is never absorbed !
  switch (fR1->fPhases) {
  case 0:
  case 1:
    //Here either no phase, or one global phase. Parallel and normal components
    //of the wave have the same phase. No reason to distinguish between the 2.
    //The reflected wave is not elliptically polarized.
    //Notice the - sign in front of fR1->fCtht.Re() due to the fact that the
    //reflection angle is defined CONTRARY to the trigonometric sense. A reflection
    //angle EQUAL to the incidence angle implies a reflected wave having an
    //OPPOSITE x direction as the incident wave.
    fR1->fUk.Set(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
    fR1->fAk  = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
    fR1->fVk  = fR1->fAk*fR1->fUk;
    fR1->fPE += (fR1->fPcaf + fR1->fPsaf)/deux;
    fR1->fEf.Set(- fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
      - fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
      fR1->fAE*fR1->fWsaf);
    BuildBPUP(fR1->fVk,fR1->fEf,fR1->fBf,fR1->fPg,fR1->fUp,fPmui);
    //
    //The reflective wave must go backward !
    //
    dh1 = fR1->fUp(0);
    if ((dh1>zero) && (dh1<wsmall)) {
      dh1 = - dh1;
      fR1->fUp(0) = dh1;
    }
    if (dh1>zero) {
      PrintWhich();
      gMes->SetName("IsoTsIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,6,"Reflected wave going forward !",dh1);
      fAnomaly = kTRUE;
      return;
    }
    break;
  case 2:
    //  Here, there are 2 phases. Parallel and normal component have different phases,
    //so that they have to be calculated separatly. Later on, we will have to choose
    //between these 2 waves.
    fR1->fUk.Set(-fR1->fCtht.Re(),fR1->fStht.Re(),zero);
    fR1->fAk  = (fW*fR1->fRfI.Re())/TLitPhys::Get()->C();
    fR1->fVk  = fR1->fAk*fR1->fUk;
    //electric field
    fR1->fEfp.Set(-fR1->fAE*fR1->fStht.Re()*fR1->fWcaf,
      -fR1->fAE*fR1->fCtht.Re()*fR1->fWcaf,
      zero);
    fR1->fEfn.Set(zero,zero,fR1->fAE*fR1->fWsaf);
    BuildBPUP(fR1->fVk,fR1->fEfp,fR1->fBfp,fR1->fPgp,fR1->fUpp,fPmut);
    BuildBPUP(fR1->fVk,fR1->fEfn,fR1->fBfn,fR1->fPgn,fR1->fUpn,fPmut);
    //
    //The reflective wave must go backward !
    //
    dh1 = fR1->fUpp(0);
    dh2 = fR1->fUpn(0);
    if ((dh1>zero) && (dh1<wsmall)) {
      dh1 = - dh1;
      fR1->fUpp(0) = dh1;
    }
    if ((dh2>zero) && (dh2<wsmall)) {
      dh2 = - dh2;
      fR1->fUpn(0) = dh2;
    }
    if ((dh1>zero) || (dh2>zero)) {
      PrintWhich();
      gMes->SetName("IsoTsIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,7,"Reflected wave going forward !",dh1,dh2);
      fAnomaly = kTRUE;
      return;
    }
    break;
  default:
    PrintWhich();
    gMes->SetName("IsoTsIso");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,8,"abnormal Nb. of phases for reflected wave",fR1->fPhases);
    fAnomaly = kTRUE;
    return;
  }
  //
  //  After all these complicate calculations, we have at hand an important check
  //that everything is ok : we can verify that the flow of energy across the
  //boundary between the 2 media is conserved. Notice that if the transmitted
  //wave is absorbed because of a complex index of refraction of the "out" medium
  //then the energy flux is not conserved ! But if the transmitted wave is 
  //absorbed because of total reflection on an "out" medium with a real index
  //of refraction, then the energy flux is conserved !
  //
  if (gMes->TestCalc()) {
    Bool_t ToCheck = kTRUE;
    Double_t flux,dflux;
    flux  = fI->fPg(0);
    dflux = flux;
    switch (fR1->fPhases) {
    case 0:
    case 1:
      //reflected wave not elliptically polarized
      switch (fT1->fPhases) {
    case 0:
    case 1:
      //reflected only absolut phase  --- transmitted wave only absolut phase
      dflux -= fT1->fPg(0)  - fR1->fPg(0);
      break;
    case 2:
      //reflected only absolut phase  --- transmitted wave with 2 phases
      dflux -= fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPg(0);
      break;
    case 3:
      //reflected only absolut phase  --- transmitted wave is absorbed
      if (TMath::Abs(fT1->fRfI.Im())<small) {
        dflux += fR1->fPg(0);
      }
      else ToCheck = kFALSE;
      break;
    default:
      //impossible value of fT1->fPhases
      PrintWhich();
      gMes->SetName("IsoTsIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,9,"abnormal Nb. of phases for transmitted wave",fT1->fPhases);
      fAnomaly = kTRUE;
      return;
      }
      break;
    case 2:
      //reflected wave elliptically polarized
      switch (fT1->fPhases) {
    case 0:
    case 1:
      //reflected wave with 2 phases  --- transmitted wave only absolut phase
      dflux -= fT1->fPg(0)  - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    case 2:
      //reflected wave with 2 phases  --- transmitted wave with 2 phases
      dflux -= fT1->fPgn(0) + fT1->fPgp(0) - fR1->fPgn(0) - fR1->fPgp(0);
      break;
    case 3:
      //reflected wave with 2 phases  --- transmitted wave is absorbed
      if (TMath::Abs(fT1->fRfI.Im())<small) {
        dflux += fR1->fPgn(0) + fR1->fPgp(0);
      }
      else ToCheck = kFALSE;
      break;
    default:
      //impossible value of fT1->fPhases
      PrintWhich();
      gMes->SetName("IsoTsIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,10,"abnormal Nb. of phases for transmitted wave",fT1->fPhases);
      fAnomaly = kTRUE;
      return;
      }
      break;
    default:
      //reflected wave absorbed ? Impossible !
      PrintWhich();
      gMes->SetName("IsoTsIso");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,11,"reflected wave absorbed !");
      fAnomaly = kTRUE;
      return;
    }
    if (ToCheck) {
      if (TMath::Abs(dflux)>msmall) {
        PrintWhich();
        gMes->SetName("IsoTsIso");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,12,"Energy flux not conserved",dflux);
        fAnomaly = kTRUE;
        return;
      }
    }
  }
}
void TPhoton::Jump() {
  // This routine moves the photon from its current position to its next position
  //depending on which processes have been defined. The routine first checks 
  //that the photon has not exceeded TLitPhys::Get()->TooLate(). If it has the photon
  //is killed.
  // The routine then finds the next place the photon would cross the face of a
  //shape. For each possible process, an interaction length is generated. The 
  //shortest length generated out of all the process and the length to the next 
  //face is taken as the length of the next step.
  // The photon is moved to the next step inside this routine.
  const Double_t cut       = 1.0e-7;
  const Double_t verysmall = 1.0e-50;
  Int_t bin;
  KindOfDetector detkind;
  Double_t dista; //distance of the new point x1 from the edge of the face
  Double_t yLDist;
  T3Vector x1(0);
  fProcess = kNoOpt;
  //First check if photon is too late. If it is too late, do not perform Jump
  //This check is important as we have now left the possibility for materials to
  //have no absorption opening a potential infinite loop
  if (fT0>TLitPhys::Get()->TooLate()) {
    //The photon is too late
    gGs->fNpLossAny += 1;
    gGs->fNpTooLate += 1;
    bin = gGs->fHTooLate->Fill(fWvlgth);
    bin = gGs->fHAbsAny->Fill(fSl0);
    if (gCs) {
      gCs->fNpLossAny += 1;
      gCs->fNpTooLate += 1;
      bin = gCs->fHTooLate->Fill(fWvlgth);
      bin = gCs->fHAbsAny->Fill(fSl0);
    }
    fPhtLive = kFALSE;
    return;
  }
  //Find next interaction face, and the distance to travel to the face
  TFace *newface = fS->NextFace(fFace,fX0,fUpg,x1,dista,cut);
  if (!newface) newface = fS->NextFace(fFace,fX0,fUpg,x1,dista,0.0);
  if (!newface) {
    PrintWhich();
    gMes->SetName("Jump");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_this,1,"No intersection with any face !");
    gMes->M(kill_this,1,"Looking at tricky point c) in documentation");
    gMes->M(kill_this,1," about TRevetment may perhaps help");
    fAnomaly = kTRUE;
    fPhtLive = kFALSE;
  }
  fDistRidge = TMath::Abs(dista);
  fProcess = kSurfaceProcess;
  //we calculates the length to travel [cm]
  yLDist = FindProcessAndLength(x1);
  fT0   += (fAk0*yLDist*fCospk)/fW;  //Update elapsed time
  fSl0  += yLDist;                   //Update space travelled by adding last step
  if (fProcess != kAbsorption) fSkl0 += fAk0*yLDist;
  if (fProcess == kSurfaceProcess) {
    fFace = newface;
    fX0 = x1;
    //Checks whether the photon is too late : not necessary to continue with photons
    //which in any case are too late to be detected !
    if (fT0>TLitPhys::Get()->TooLate()) {
      gGs->fNpLossAny += 1;
      gGs->fNpTooLate += 1;
      bin = gGs->fHTooLate->Fill(fWvlgth);
      bin = gGs->fHAbsAny->Fill(fSl0);
      if (gCs) {
        gCs->fNpLossAny += 1;
        gCs->fNpTooLate += 1;
        bin = gCs->fHTooLate->Fill(fWvlgth);
        bin = gCs->fHAbsAny->Fill(fSl0);
      }
      fPhtLive = kFALSE;
    }// end if (fT0>TLitPhys::Get()->TooLate())
  }
  else {
    fFace = 0;
    fX0 = fX0 + yLDist*fUpg;
  }
  if (fProcess == kIsotropicScattering) {
    ++fNbOfDiffusion;
    Regenerate(kIsotropicScattering);
  }
  if (fProcess == kRayleighScattering) {
    ++fNbOfRayleigh;
    Regenerate(kRayleighScattering);
  }
  if (fProcess == kWLS) {
    ++fNbOfWLS;
    Regenerate(kWLS);
  }  
  if (fProcess == kAbsorption) {
    fPhtLive = kFALSE;
    bin = gGs->fHStopShape->Fill(fANShape);  //Record Shape in which photon stopped
    if (gCs) bin = gCs->fHStopShape->Fill(fANShape);
    if (fAbsl0<verysmall) {
      gGs->fNpAbsL0 +=1;
      if (gCs) gCs->fNpAbsL0 +=1;
    }
    else {
      //Check whether photon stopped in a sensitive material
      if (fMat0->IsSensible()) {
        //Check whether photon is too late !
        if (fT0>TLitPhys::Get()->TooLate()) {
          //The photon is too late
          gGs->fNpLossAny += 1;
          gGs->fNpTooLate += 1;
          bin = gGs->fHTooLate->Fill(fWvlgth);
          bin = gGs->fHAbsAny->Fill(fSl0);
          if (gCs) {
            gCs->fNpLossAny += 1;
            gCs->fNpTooLate += 1;
            bin = gCs->fHTooLate->Fill(fWvlgth);
            bin = gCs->fHAbsAny->Fill(fSl0);
          }
        }
        else {  //if (fT0>TLitPhys::Get()->TooLate())
          //The photon is in time
          detkind = fS->GetDetector();
          switch (detkind) {
    case none:
      // Here we handle the most simple type of volume detector, which
      //consists simply of a sensitive material. In this simple type
      //of detector, a photon is seen simply if it stops here.
      //NO break !
    case surfdet:
      // We handle here the case of a photon which has stopped inside a volume ! So if
      //this volume has a surface which is a detector, this does not concern us here !
      //NO break !
    case phototube:
      // We handle here the case of a photon which has stopped inside a volume ! So if
      //this volume has a surface which is a detector, this does not concern us here !
      //case none,surfdet and phototube handled here !
      fPhtSeen = kTRUE;
      gGs->fNpSeen += 1;
      bin = gGs->fHTimeSeen->Fill(fT0);
      bin = gGs->fHDistSeen->Fill(fSl0);
      bin = gGs->fHWvlgthSeen->Fill(fWvlgth);
      if (gCs) {
        gCs->fNpSeen += 1;
        bin = gCs->fHTimeSeen->Fill(fT0);
        bin = gCs->fHDistSeen->Fill(fSl0);
        bin = gCs->fHWvlgthSeen->Fill(fWvlgth);
      }
      break;
    case volumedet:
      //This case is handled by VolDet
      VolDet();
      break;
    case apd:
      //This case is handled by APDDet
      APDDet();
      break;
          } //switch (detkind)
        } //if (fT0>TLitPhys::Get()->TooLate())
      } //end if (fMat0->IsSensible())
      else {
        //Here the photon is lost absorbed by non-sensitive material of a shape
        gGs->fNpLossAny += 1;
        gGs->fNpAbsMat  += 1;
        bin = gGs->fHAbsAny->Fill(fSl0);
        bin = gGs->fHAbsMat->Fill(fSl0);
        if (gCs) {
          gCs->fNpLossAny += 1;
          gCs->fNpAbsMat  += 1;
          bin = gCs->fHAbsAny->Fill(fSl0);
          bin = gCs->fHAbsMat->Fill(fSl0);
        }// end if (gCs)
      }// end else of if (fMat0->IsSensible())
    } // end if (fAbsl0<verysmall)
  }// end if (fProcess == kAbsorbed)
  if (fProcess == kNoOpt) {
    gMes->SetName("Jump");
    gMes->SetTitle("TPhoton");
    gMes->M(Error_M,2,"Bad value of fProcess",(Int_t)fProcess);
  }
}
void TPhoton::Move(Int_t iface) {
  //Stirring routine moving the photon inside the setup, from birth to death
  const Int_t NPointsMax = 10000; //Max nb. of interaction on photon trajectory
  const Axis_t small = 1.0e-6;
  Int_t bin;
  Axis_t a;
  Bool_t starting = kTRUE;
  while ((fPhtLive) && (!fAnomaly)) {
    if    ((starting) && (iface>=0)) fFace = fS->GetFace(iface);
    else  {
      do Jump();
      while (fProcess == kIsotropicScattering ||
        fProcess == kRayleighScattering ||
        fProcess == kWLS);
    }
    starting = kFALSE;
    fNPoints += 1;
    if (fNPoints>NPointsMax) {
      fAnomaly = kTRUE;
      gMes->SetName("Move");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,1,"abnormal long life for photon");
    }
    if ((fPhtLive) && (!fAnomaly)) {
      OnFace();
    }
  }
  a = fNbOfDiffusion + small;
  bin = gGs->fHPoints->Fill(fNPoints);
  bin = gGs->fHDiffusion->Fill(a);
  if (fPhtSeen) {
    gLit->fSeen = kTRUE;
    gLit->fX0   = fX0(0);
    gLit->fY0   = fX0(1);
    gLit->fZ0   = fX0(2);
  }
  else gLit->fSeen = kFALSE;
  if (gCs) {
    bin = gCs->fHPoints->Fill(fNPoints);
    bin = gCs->fHDiffusion->Fill(a);
  }
  if (fAnomaly) {
    fPhtLive = kFALSE;
    gGs->fNpLossAny += 1;
    gGs->fNpAbnorm  += 1;
    bin = gGs->fHAbsAny->Fill(fSl0);
    bin = gGs->fHAbnLost->Fill(fSl0);
    if (gCs) {
      gCs->fNpLossAny += 1;
      gCs->fNpAbnorm  += 1;
      bin = gCs->fHAbsAny->Fill(fSl0);
      bin = gCs->fHAbnLost->Fill(fSl0);
    }
  }
}
void TPhoton::OnFace() {
  //OnFace is the steering routine to handle what has to be done when the
  //photon reaches a face of a shape.
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  Int_t i,bin;
  fNFace = fFace->GetNumber();
  fKFace = fNFace;
  //Does this face have subfaces ?
  fNbSubFaces = fFace->NbSubFaces();
  fSubFace    = 0;
  if (fNbSubFaces) {
    Bool_t OnSubFace = kFALSE;
    i = 0;
    while ((!OnSubFace) && (i<fNbSubFaces)) {
      fUsedFace = fFace->GetSubFace(i);
      OnSubFace = fUsedFace->Contained(fX0);
      i++;
    }
    if (OnSubFace) fSubFace = fUsedFace;
  }
  if (fSubFace) {
    fUsedFace = fSubFace;
    fNSubFace = fSubFace->GetNumber();
    fKFace    = fNSubFace;
  }
  else fUsedFace = fFace;
  //Finds the revetment covering this face
  if (fS->HasBevellings()) fRevetment = fUsedFace->GetRevetment(fDistRidge);
  else fRevetment = fUsedFace->GetRevetment();
  if (fRevetment) {
    fSinThetaMax = fRevetment->GetSinThetaMax();
    if (fRevetment->IsSlice()) fMatSl = fRevetment->GetSlice();
    else fMatSl = 0;
  }
  else {
    fSinThetaMax = un;
    fMatSl = gOutside;
  }
  bin = gGs->fHHitFace->Fill(fANShape,Axis_t(fKFace));
  if (gCs) bin = gCs->fHHitFace->Fill(fANShape,Axis_t(fKFace));
  //Is this face or subface a detector ?
  fNDet      = fUsedFace->GetDetNumber();
  if (fNDet>=0) {
    fDetKind = fUsedFace->GetDetKind();
    fAccept  = fUsedFace->GetAccAngle();
    fQE      = fUsedFace->GetQuantumEff();
  }
  else {
    fDetKind = none;
    fAccept  = zero;
    fQE      = 0;
  }
  //Is this face in contact with an other face of an other shape ?
  fHasContact = fFace->HasContact();
  if (fHasContact) fNContact = fFace->NbOfContacts();
  else  {
    fNContact = 0;
    fContactType  = nocontact;
    fContact      = 0;
    fContactFace  = 0;
    fContactShape = 0;
    fContactSuppl = 0;
    fThinSlice    = 0;
  }
  //Finds the normal to the face at point fX0, pointing outside the shape
  fPlanar = fFace->GetPlanar();
  if (fPlanar) fFN = fFace->Normal();
  else fFN = fFace->Normal(fX0);
  //Call SurfDet if the face is a detector
  if (fNDet>=0) SurfDet();
  if (fPhtLive) {
    fToOther = kFALSE;
    fAnisi   = fAnis0;
    fAniai   = fAnia0;
    fPmui    = fPmu0;
    if (fAnisi) fDzti = fDzt0;
    else fRfIndexi =fRfIndex0;
    if (fHasContact) {
      Bool_t found = kFALSE;
      fBetween = kFALSE;
      i = 0;
      while ((!found) && (i<fNContact)) {
        fContactType  = fFace->ContactType(i);
        fContactFace  = fFace->ContactFace(i);
        switch (fContactType) {
        case nocontact:
          break;
        case identical:
        case contained:
          if (fNContact != 1) {
            gMes->SetName("OnFace");
            gMes->SetTitle("TPhoton");
            gMes->M(kill_this,1,"Impossible number of contacts");
          }
          found = kTRUE;
          fBetween = kTRUE;
          fContact      = fFace->Contact(0);
          fContactShape = fFace->ContactShape(0);
          fContactSuppl = fFace->ContactSuppl(0);
          fThinSlice    = fContact->GetThinSlice();
          AgainstShape();
          break;
        case containing:
        case overlapping:
          found = fContactFace->Contained(fX0);
          if (found) {
            fBetween      = kTRUE;
            fContact      = fFace->Contact(i);
            fContactShape = fFace->ContactShape(i);
            fContactSuppl = fFace->ContactSuppl(i);
            fThinSlice    = fContact->GetThinSlice();
            AgainstShape();
          }
          break;
        }// end switch (fContactType)
        i += 1;
      }// end while ((!found) && (i<fNContact))
      if (!found) {
        fBetween = kFALSE;
        AgainstRev();
      }
    }// end if (fHasContact)
    else {
      fBetween = kFALSE;
      AgainstRev();
    }
  } //end if (fPhtlive)
}
void TPhoton::PrintWhich() const {
  //Print run number and photon number
  cout << "Run : " << fRun << "   photon : " << fKphot << endl;
  //listing
  *gMes->fListing << "Run : " << fRun << "   photon : " << fKphot << endl;
}
void TPhoton::PrintWhich(Int_t ifreq) const {
  //Print run number and photon number at a frequency ifreq
  Int_t i;
  i = fKphot % ifreq;
  if (!i) PrintWhich();
}
void TPhoton::Regenerate(OpticalProcess process) {
  // Regenerate a photon in the same position with the possibility of changing 
  //the wavelength and direction. Regenerate() should only be called for 
  //isotropic or Rayleigh scattering or wavelength shifting (WLS). 
  // WLS is the same as isotropic scattering but the photon changes wavelength. 
  // Rayleigh scattering has a preference for regeneration of photons in the 
  //forward and backward directions.
  // In isotropic scattering and wavelength shifting, the polarisation of the
  //photon is regenerated randomly.
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t deux   = 2.0;
  const Double_t small = 1.0e-6;
  Double_t x = 0.0;
  Double_t y = 0.0;
  Double_t z = 0.0;
  Double_t theta,phi,st,ct,sf,cf,s,c,test;
  T3Vector Ox,Oy;
  gMes->SetName("Regenerate");
  gMes->SetTitle("TPhoton");
  switch (process) {
  case kWLS:
    Int_t kfluo;
    fMat0->DoWLS(fWvlgth,fT0,kfluo);
    fWLSL = fMat0->GetWLSL();//Should this go in gLit:NewWavelength method?
    //No break
  case kIsotropicScattering:
    x = gRandom3->Rndm();
    y = un - deux*x;
    theta = TLitPhys::ACos(y);
    phi   = deux*TMath::Pi()*gRandom3->Rndm();
    st = TMath::Sin(theta);
    ct = TMath::Cos(theta);
    sf = TMath::Sin(phi);
    cf = TMath::Cos(phi);
    x  = st*cf;
    y  = st*sf;
    z  = ct;
    fUk0.Set(x,y,z);
    if (!fAnis0) {
      T3Vector fAxisb1(un,zero,zero);
      T3Vector fAxisb2(zero,un,zero);
      s = TMath::Abs(fUk0*fAxisb1);
      c = TMath::Abs(fUk0*fAxisb2);
      if (s<=c) Ox = fAxisb1;
      else      Ox = fAxisb2;
      Ox.MinOrthoN(fUk0,s,c);
      Oy = fUk0^Ox;
      Oy.Unit();
      if (gMes->TestCalc()) {
        test = TMath::Abs(un - Ox.Module());
        if (test>small) gMes->M(Error_M,1,"Ox not of length 1",test);
        test = TMath::Abs(Ox*Oy);
        if (test>small) gMes->M(Error_M,2,"Ox not orthogonal to Oy",test);
        test = TMath::Abs(Ox*fUk0);
        if (test>small) gMes->M(Error_M,3,"Ox not orthogonal to fUk0",test);
        test = TMath::Abs(Oy*fUk0);
        if (test>small) gMes->M(Error_M,4,"Oy not orthogonal to fUk0",test);
      }
      // Est-ce vrai que la dispersion isotrope implique que la polarisation 
      // est nouvellement aleatoire?
      phi = deux*TMath::Pi()*gRandom3->Rndm();
      s   = TMath::Sin(phi);
      c   = TMath::Cos(phi);
      fEf0   = c*Ox + s*Oy;
      if (gMes->TestCalc()) {
        test = TMath::Abs(un - fEf0.Module());
        if (test>small) gMes->M(Error_M,5,"fEf0 not of length 1",test);
        test = TMath::Abs(fEf0*fUk0);
        if (test>small) gMes->M(Error_M,6,"fEf0 not orthogonal to fUk0",test);
      }
    }
    else fEf0.Set(zero,zero,zero);
    break;
  case kRayleighScattering:
    if (!fAnis0) {
      //Find normal to direction of photon
      T3Vector fAxisb1(un,zero,zero);
      T3Vector fAxisb2(zero,un,zero);
      s = TMath::Abs(fUk0*fAxisb1);
      c = TMath::Abs(fUk0*fAxisb2);
      if (s<=c) Ox = fAxisb1;
      else      Ox = fAxisb2;
      Ox.MinOrthoN(fUk0,s,c);
      // Any normal to fUk0
      // We can do this with no loss of generality since we generate a random 
      // angle phi below.
      fUk0.Unit();
      Double_t p1, p2, p3;
      fUk0.Get(p1, p2, p3);
      Double_t ox1, ox2, ox3;
      Ox.Get(ox1, ox2, ox3);
      // Find plane of scattering
      phi = deux*TMath::Pi()*gRandom3->Rndm();
      cf = TMath::Cos(phi);
      sf = TMath::Sin(phi);
      // Normal to fUk0 in plane of scattering
      Double_t os1 = (cf+(1.-cf)*p1*p1)*ox1  
        + ((-p3*sf)+(1.-cf)*p1*p2)*ox2 
        + ((p2*sf)+(1.-cf)*p1*p3)*ox3;
      Double_t os2 = ((p3*sf)+(1.-cf)*p1*p2)*ox1  
        + (cf+(1.-cf)*p2*p2)*ox2  
        + ((-p1*sf)+(1.-cf)*p2*p3)*ox3;
      Double_t os3 = ((-p2*sf)+(1.-cf)*p1*p3)*ox1 
        + ((p1*sf)+(1.-cf)*p2*p3)*ox2  
        + (cf+(1.-cf)*p3*p3)*ox3;
      T3Vector Os(os1,os2,os3); 
      Os.Unit();
      if (gMes->TestCalc()) {
        test = TMath::Abs(Os*fUk0);
        if (test>small) gMes->M(Error_M,7,"Os not orthogonal to fUk0",test);}
      // Find polarisation wrt plane of scattering
      Double_t module = fEf0.Module();
      fEf0.Unit();
      Double_t para = fEf0.VCos(Os);
      Double_t perp = fEf0.VSin(Os);
      // Find scattering angle by acceptance rejection
      // (perp^2 + (para*ct)^2) comes from the x-section for Rayleigh Scattering
      // the factor of st comes from the infinitessimal solid angle
      theta = gRandom3->Rndm()*TMath::Pi();
      ct = TMath::Cos(theta);
      st = TMath::Sqrt(1-ct*ct);
      while (gRandom3->Rndm()> (perp*perp + para*para*ct*ct)*st) {
        theta = gRandom3->Rndm()*TMath::Pi();
        ct = TMath::Cos(theta);
        st = TMath::Sqrt(1-ct*ct);
      } 
      T3Vector oldfUk0 = fUk0;
      // Rotate the k vector
      fUk0 = fUk0*ct + Os*st;
      fUk0.Unit();
      // Rotate and give amplitude to the polarisation vector
      fEf0 = (Os*ct - oldfUk0*st);
      if (gMes->TestCalc()) {
        test = TMath::Abs(fEf0*fUk0);
        if (test>small) gMes->M(Error_M,8,"fEf0 not orthogonal to fUk0",test);
      }
      fEf0 = module*fEf0;
    }
    else {
      gMes->SetName("Regenerate");
      gMes->SetTitle("TPhoton");
      gMes->M(Error_M,1,"Rayleigh Scattering not implemented in anistropic media");
    }
    break;
  case kSurfaceProcess:
  case kNoOpt:
  case kAbsorption:
  default:
    gMes->SetName("Regenerate");
    gMes->SetTitle("TPhoton");
    gMes->M(kill_prog,1,"Regenerate called for wrong Optical Process");
  }
  StartPhoton();
}
void TPhoton::SliceToRev() {
  //  After the photon has been refracted out of the polyhedron into the slice
  //of material between face and revetment, SliceToRev looks at what happens to
  //it when it hits the revetment. Is it absorbed, diffused, or reflected ?
  //(old CRBACK)
  //  First look whether there is a "pre-absorption" before hitting revetment
  //( dust ?).
  Int_t bin;
  bin = gGs->fHHitRvt->Fill(fANShape,Axis_t(fKFace));
  if (gCs) bin = gCs->fHHitRvt->Fill(fANShape,Axis_t(fKFace));
  Double_t y;
  switch (fRevetment->IsSupplAbs()) {
  case 0:
    break;
  case 1:
    y = gRandom3->Rndm();
    if (y<fRevetment->GetSupplAbs()) fPhtAbs = kTRUE;
    break;
  case 2:
    fPhtAbs = kTRUE;
    break;
  default:
    break;
  }
  if (fPhtAbs) {
    //Photon has been absorbed before the revetment
    fPhtLive = kFALSE;
    gGs->fNpLossAny += 1;
    gGs->fNpAbsBef  += 1;
    bin = gGs->fHAbsAny->Fill(fSl0);
    bin = gGs->fHAbsBef->Fill(fSl0);
    bin = gGs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
    if (gCs) {
      gCs->fNpLossAny += 1;
      gCs->fNpAbsBef  += 1;
      bin = gCs->fHAbsAny->Fill(fSl0);
      bin = gCs->fHAbsBef->Fill(fSl0);
      bin = gCs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
    }
  }// end if (fPhtAbs)
  else {
    //"In" characteristics"
    fAnisi    = 0;
    fAniai    = 0;
    fPmui     = fMatSl->GetMu();
    fRfIndexi = fMatSl->GetIndex();
    //"Out" characteristics"
    fAnist    = 0;
    fPmut     = fRevetment->GetMu();
    fRfIndext = fRevetment->GetIndex();
    fOx       = fFN;
    //decides whether the photon is reflected or diffused
    y = gRandom3->Rndm();
    if (y>fRevetment->Diffus()) {
      //The photon is reflected
      Transition();
      if (!fAnomaly && fPhtAbs) {
        //Photon has been absorbed by the revetment
        fPhtLive = kFALSE;
        gGs->fNpLossAny += 1;
        gGs->fNpAbsRvt  += 1;
        bin = gGs->fHAbsAny->Fill(fSl0);
        bin = gGs->fHAbsRvt->Fill(fSl0);
        bin = gGs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
        if (gCs) {
          gCs->fNpLossAny += 1;
          gCs->fNpAbsRvt  += 1;
          bin = gCs->fHAbsAny->Fill(fSl0);
          bin = gCs->fHAbsRvt->Fill(fSl0);
          bin = gCs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
        }
      }// end if (!fAnomaly && fPhtAbs)
    }// end if (y>fRevetment->Diffus())
    else {
      //The photon is diffused
      Diffused();
    }// end if (y>fRevetment->Diffus())
  }// end else if (fPhtAbs)
}
void TPhoton::SliceToShape() {
  //  On return from the revetment towards the face, decides whether the photon
  //is reflected back again towards the revetment or reenters the shape.
  // (old CRBACKIN)
  const Double_t minus = -1.0;
  // "In" Characteristics :
  //The "IN" medium is the slice between convex polyhedron and revetment.
  //This slice is always isotropic.
  fAnisi    = 0;
  fAniai    = 0;
  fPmui     = fMatSl->GetMu();
  fRfIndexi = fMatSl->GetIndex();
  // "Out" characteristics
  //The OUT characteristics are those of the shape the photon has
  //just leaved to hit the revetment.
  fAnist    = fAnis0;
  fAniat    = fAnia0;
  fPmut     = fPmu0;
  if (fAnist) fDztt = fDzt0;
  else fRfIndext =fRfIndex0;
  //Because coming back, normal has to be reversed.
  fOx = minus*fFN;
  Transition();
  if (!fAnomaly) {
    fOutPoly = !fPhtOut;
  }
}
void TPhoton::StartPhoton() {
  //Finish to generate the photon
  const Double_t deux  = 2.0;
  const Double_t dixm7 = 1.0e-7;
  const Double_t dix12 = 1.0e+12;
  //calculates frequency [ps-1] and energy [Joules] of photon
  fW = (deux*TMath::Pi()*TLitPhys::Get()->C())/(dixm7*fWvlgth);
  fEphot = TLitPhys::Get()->Hbar()*fW*dix12;
  //material in which photon is
  if (fAnis0) FindDielAni();
  else FindDielIso();
  if (!fAnomaly) {
    //We generate the phase of the linearly polarized photon
    fBeta = deux*TMath::Pi()*gRandom3->Rndm();
    //Magnetic Field and Poyinting vector in WCS
    BuildBPUP(fVk0,fEf0,fBf0,fPtg,fUpg,fPmu0);
    fCospk   = fUk0*fUpg;
    fPhtLive = kTRUE;
    fOutPoly = kFALSE;
    fPhtSeen = kFALSE;
    fPhtAbs  = kFALSE;
    fPhtOut  = kFALSE;
    fToOther = kFALSE;
    fOutPoly = kFALSE;
  }
}
void TPhoton::SurfDet() {
  //Handles the case of a photon having reached a sensitive surface. It is not
  //necessary to check whether photon is too late, it has already been done by
  //method Jump().
  const Double_t un    = 1.0;
  const Double_t pideg = 180.0;
  Int_t bin;
  Double_t todeg = pideg/TMath::Pi();
  TStatSurfD *pstSurfDg;
  TStatSurfD *pstSurfDc = 0;
  fPhtLive = kFALSE;
  pstSurfDg = (TStatSurfD *)gGs->fSurfDet[fNDet];
  if (gCs)
    pstSurfDc = (TStatSurfD *)gCs->fSurfDet[fNDet];
  bin = gGs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
  if (gCs) bin = gCs->fHStopFace->Fill(fANShape,Axis_t(fKFace));
  //Is the photon within acceptance angle ?
  Double_t y1,y2,angle,angled,scalprod;
  scalprod = fUpg*fFN;
  angle  = TLitPhys::ACos(scalprod);
  angled = todeg*angle;
  bin = pstSurfDg->fHAngleAll->Fill(angled);
  if (gCs)
    bin = pstSurfDc->fHAngleAll->Fill(angled);
  if (angle<=fAccept) {
    //Photon within acceptance angle
    if (fQE) {
      //There is a quantum efficiency
      y1 = gRandom3->Rndm();
      y2 = fQE->V(fWvlgth);
      gGs->fPhweigth += y2;
      if (gCs) gCs->fPhweigth += y2;
      if (y1>y2) {
        //Photon lost because of quantum efficiency
        gGs->fNpLossAny += 1;
        gGs->fNpLossQE  += 1;
        bin = gGs->fHAbsAny->Fill(fSl0);
        pstSurfDg->fNpNotSeen +=1;
        bin = pstSurfDg->fHWvlgthNot->Fill(fWvlgth);
        bin = pstSurfDg->fHInside->Fill(fWvlgth);
        if (gCs) {
          gCs->fNpLossAny += 1;
          gCs->fNpLossQE  += 1;
          bin = gCs->fHAbsAny->Fill(fSl0);
          pstSurfDc->fNpNotSeen +=1;
          bin = pstSurfDc->fHWvlgthNot->Fill(fWvlgth);
          bin = pstSurfDc->fHInside->Fill(fWvlgth);
        }
      } // end if (y1>y2)
      else {
        //The photon is seen
        fPhtSeen = kTRUE;
        gGs->fNpSeen += 1;
        bin = gGs->fHTimeSeen->Fill(fT0);
        bin = gGs->fHDistSeen->Fill(fSl0);
        bin = gGs->fHWvlgthSeen->Fill(fWvlgth);
        pstSurfDg->fNpSeen +=1;
        bin = pstSurfDg->fHTimeSeen->Fill(fT0);
        bin = pstSurfDg->fHWvlgthSeen->Fill(fWvlgth);
        bin = pstSurfDg->fHQEff->Fill(fWvlgth);
        bin = pstSurfDg->fHInside->Fill(fWvlgth);
        bin = pstSurfDg->fHAngleAcc->Fill(angled);
        if (gCs) {
          gCs->fNpSeen += 1;
          bin = gCs->fHTimeSeen->Fill(fT0);
          bin = gCs->fHDistSeen->Fill(fSl0);
          bin = gCs->fHWvlgthSeen->Fill(fWvlgth);
          pstSurfDc->fNpSeen +=1;
          bin = pstSurfDc->fHTimeSeen->Fill(fT0);
          bin = pstSurfDc->fHWvlgthSeen->Fill(fWvlgth);
          bin = pstSurfDc->fHQEff->Fill(fWvlgth);
          bin = pstSurfDc->fHInside->Fill(fWvlgth);
          bin = pstSurfDc->fHAngleAcc->Fill(angled);
        }
      } // end else if (y1>y2)
    } // end if (fQE)
    else {
      //No quantum efficiency --> photon seen
      fPhtSeen = kTRUE;
      gGs->fNpSeen += 1;
      gGs->fPhweigth += un;
      bin = gGs->fHTimeSeen->Fill(fT0);
      bin = gGs->fHDistSeen->Fill(fSl0);
      bin = gGs->fHWvlgthSeen->Fill(fWvlgth);
      pstSurfDg->fNpSeen +=1;
      bin = pstSurfDg->fHTimeSeen->Fill(fT0);
      bin = pstSurfDg->fHWvlgthSeen->Fill(fWvlgth);
      bin = pstSurfDg->fHQEff->Fill(fWvlgth);
      bin = pstSurfDg->fHInside->Fill(fWvlgth);
      bin = pstSurfDg->fHAngleAcc->Fill(angled);
      if (gCs) {
        gCs->fNpSeen += 1;
        gCs->fPhweigth += un;
        bin = gCs->fHTimeSeen->Fill(fT0);
        bin = gCs->fHDistSeen->Fill(fSl0);
        bin = gCs->fHWvlgthSeen->Fill(fWvlgth);
        pstSurfDc->fNpSeen +=1;
        bin = pstSurfDc->fHTimeSeen->Fill(fT0);
        bin = pstSurfDc->fHWvlgthSeen->Fill(fWvlgth);
        bin = pstSurfDc->fHQEff->Fill(fWvlgth);
        bin = pstSurfDc->fHInside->Fill(fWvlgth);
        bin = pstSurfDc->fHAngleAcc->Fill(angled);
      }
    } // else if (fQE)
  } //end if (angle<=fAccept)
  else {
    //Photon not seen by detector because of acceptance angle. Photon lost.
    gGs->fNpLossAny += 1;
    gGs->fNpLossAng += 1;
    bin = gGs->fHAbsAny->Fill(fSl0);
    pstSurfDg->fNpNotSeen +=1;
    bin = pstSurfDg->fHWvlgthNot->Fill(fWvlgth);
    if (gCs) {
      gCs->fNpLossAny += 1;
      gCs->fNpLossAng += 1;
      bin = gCs->fHAbsAny->Fill(fSl0);
      pstSurfDc->fNpNotSeen +=1;
      bin = pstSurfDc->fHWvlgthNot->Fill(fWvlgth);
    }
  } //end else if (angle<=fAccept)
}
void TPhoton::ToICS() {
  //  ToICS finds the axis of the coordinate system ICS of the incident plane. It
  //also finds the transformation matrices fMn and fMt towards ICS.
  //   The ICS coordinate system is defined like this :
  // Ox is the normal to the hit face, pointing outside. Ox has to be defined
  //     before calling ToICS.
  // Oy is normal to Ox and is IN the plane containing Ox and the wave vector k
  //     of the photon. Ox,Oy build the incident plane. In addition, Oy is chosen
  //     in such a way that the scalar product of Oy with the k vector of the
  //     photon is positiv.
  // Oz is normal to Ox and Oy. Ox, Oy, Oz build a right orthonormal system
  const Double_t vsmall = 1.0e-8;
  Double_t stht_i,ctht_i;
  Double_t y;
  fOy = fUk0;
  fOy.MinOrthoN(fOx,stht_i,ctht_i);
  fOz = fOx^fOy;
  //Verify that Oy is indeed in the incident plane
  if (gMes->TestCalc()) {
    y = TMath::Abs(fUk0*fOz);
    if (y>vsmall) {
      PrintWhich();
      gMes->SetName("ToICS");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,0,"bad definition of Oy",y);
      fAnomaly = kTRUE;
      return;
    }
  }
  fI->fStht = stht_i;
  fI->fCtht = ctht_i;
  if (gMes->TestCalc()) {
    y = TMath::Abs(fOx*fOx + fOy*fOy + fOz*fOz - 3.0);
    if (y>vsmall) {
      PrintWhich();
      gMes->SetName("ToICS");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,1,"axis not orthonormal",y);
      fAnomaly = kTRUE;
      return;
    }
    y = TMath::Abs(fOx*fOy) + TMath::Abs(fOx*fOy) + TMath::Abs(fOy*fOz);
    if (y>vsmall) {
      PrintWhich();
      gMes->SetName("ToICS");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,2,"axis not orthogonal",y);
      fAnomaly = kTRUE;
      return;
    }
  }
  fMn.SetColumn(0,fOx(0),fOx(1),fOx(2));
  fMn.SetColumn(1,fOy(0),fOy(1),fOy(2));
  fMn.SetColumn(2,fOz(0),fOz(1),fOz(2));
  fMt.TransposedOf(fMn);
  if (gMes->TestCalc()) {
    T33Matrix A;
    A = fMn*fMt;
    y = A.CloseUnit();
    if (y>vsmall) {
      PrintWhich();
      gMes->SetName("ToICS");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,3,"Transformation matrix fMn bad",y);
      fAnomaly = kTRUE;
      return;
    }
  }
}
Short_t TPhoton::Toss(Short_t nchoice) {
  //Chooses randomly according to weights fWeigth among nchoive possibility
  const Short_t  huit  = 8;
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  const Double_t small = 1.0e-6;
  Short_t ichoice = -1;
  Double_t y;
  nchoice -= 1;
  nchoice %= huit;
  nchoice += 1;
  if (nchoice == 1) ichoice = 0;
  else {
    Double_t dd;
    Double_t zwhich[huit];
    Short_t k;
    Bool_t notabs = kFALSE;
    //We normalize the weigths
    dd = zero;
    for (k=0;k<nchoice;k++) dd += fWeigth[k];
    for (k=0;k<nchoice;k++) fWeigth[k] /= dd;
    //
    //  Following code is for taking into account the small error due to
    //finite precision of the machine in the case where one choice
    //corresponds to absorption and absorption is not allowed
    //( for instance in case of total reflection ). In that case,
    //fWeigth should be 0 but is not exactly 0 due to numerical imprecision,
    //and we put it exactly to 0.
    //
    for (k=0;k<nchoice;k++)
      if (fWeigth[k]<small) {
        notabs = kTRUE;
        fWeigth[k] = zero;
      }
      if (notabs) {
        dd = zero;
        for (k=0;k<nchoice;k++) dd += fWeigth[k];
        for (k=0;k<nchoice;k++) fWeigth[k] /= dd;
      }
      //Now comes the choice
      for (k=0;k<nchoice-1;k++) {
        if (k==0) zwhich[k] = fWeigth[k];
        else zwhich[k] = zwhich[k-1] + fWeigth[k];
      }
      for (k=nchoice-1;k<huit;k++) zwhich[k] = un;
      y = gRandom3->Rndm();
      if (y<=zwhich[3]) {
        if (y<=zwhich[1]) {
          if (y<=zwhich[0]) {
            ichoice = 0;
          }// if (y<=zwhich[0])
          else {
            ichoice = 1;
          }// end else if (y<=zwhich[0])
        }// if (y<=zwhich[1])
        else {
          if (y<=zwhich[2]) {
            ichoice = 2;
          }// if (y<=zwhich[2])
          else {
            ichoice = 3;
          }// end else if (y<=zwhich[2])
        }// end else if (y<=zwhich[1])
      }// if (y<=zwhich[3])
      else {
        if (y<=zwhich[5]) {
          if (y<=zwhich[4]) {
            ichoice = 4;
          }// if (y<=zwhich[4])
          else {
            ichoice = 5;
          }//end else if (y<=zwhich[4])
        }// if (y<=zwhich[5])
        else {
          if (y<=zwhich[6]) {
            ichoice = 6;
          }// if (y<=zwhich[6])
          else {
            ichoice = 7;
          }// end else if (y<=zwhich[6])
        }// end else if (y<=zwhich[5])
      }//end else if (y<=zwhich[3])
  }
  return ichoice;
}
void TPhoton::Transition() {
  //Stirring method handling transition between 2 media.
  //The fOx axis must already have been defined before calling Transition().
  //(old CRINCIPL)
  const Double_t zero   = 0.0;
  const Double_t vsmall = 1.0e-7;
  const Double_t wsmall = 1.0e-10;
  Double_t y;
  if (gMes->TestCalc()) {
    //tests that the photon is not backward
    y = fOx*fUpg;
    if (y<=zero) {
      PrintWhich();
      gMes->SetName("Transition");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,1,"Bad direction of photon",y);
      fAnomaly = kTRUE;
      return;
    }
  }
  //Creates the coordinate system of the incident plane ICS
  ToICS();
  if (fAnomaly) return;
  fIsDiag  = kFALSE;
  fIsDiagi = kFALSE;
  fIsDiagt = kFALSE;
  //Transports the dielectric tensor associated with the incident wave into ICS
  //Make it absolutly symmetric and checks whether diagonal.
  if (fAnisi) {
    fDztii = fMt*fDzti*fMn;
    y = fDztii.Symmetrize();
    if (y<vsmall) {
      fIsDiag  = kTRUE;
      fIsDiagi = kTRUE;
      fDztii.Diagonalize();
    }
  }
  //Transports the dielectric tensor associated with the transmitted wave into ICS
  //Make it absolutly symmetric and checks whether diagonal.
  if (fAnist) {
    fDztti = fMt*fDztt*fMn;
    y = fDztti.Symmetrize();
    if (y<vsmall) {
      fIsDiag  = kTRUE;
      fIsDiagt = kTRUE;
      fDztti.Diagonalize();
    }
  }
  //We transport the photon into the ICS.
  fI->fUk = fMt*fUk0;
  fI->fEf = fMt*fEf0;
  //Verify that the k vector is inside the incident plane
  if (gMes->TestCalc())
    if (TMath::Abs(fI->fUk(2))>wsmall) {
      PrintWhich();
      gMes->SetName("Transition");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,2,"k vector not in incident plane");
      fAnomaly = kTRUE;
      return;
    }
    fI->fUk(2) = zero;
    fI->fUk.Unit();
    fI->fVk = fAk0*fI->fUk;
    //We make the angular decomposition of the electric field of the incident wave
    if (fAnisi) fI->Split(kTRUE);
    else fI->Split(kFALSE);
    //Defines B field, Poyinting vector and unit Poyinting vector
    BuildBPUP(fI->fVk,fI->fEf,fI->fBf,fI->fPg,fI->fUp,fPmui);
    //Verifies that the incident wave goes forward !
    if (fI->fUp(0)<zero) {
      PrintWhich();
      if (fI->fUp(0)<-wsmall) {
        gMes->SetName("Transition");
        gMes->SetTitle("TPhoton");
        gMes->M(kill_this,3,"Incident wave going back",fI->fUp(0));
        fAnomaly = kTRUE;
        return;
      }
      else {
        fI->fUp(0) = -fI->fUp(0);
        fI->fPg(0) = -fI->fPg(0);
        gMes->SetName("Transition");
        gMes->SetTitle("TPhoton");
        gMes->M(Warning_M,4,"Incident wave slightly backward",fI->fUp(0));
      }
    }
    //List of verifications
    if (gMes->TestCalc()) {
      //Verify that the angle decomposition of the E field is ok
      fAnomaly = fI->VerifySplit();
      if (fAnomaly) return;
      //Verify, if anisotropy, that initial conditions ok
      if (fAnisi) {
        fAnomaly = fI->VerifyStart(fI->fRfI.Re(),fDztii);
        if (fAnomaly) return;
      }
    }
    //Here come all calculations about the transition !
    switch (fAnisi) {
  case 0:
    switch (fAnist) {
  case 0:
    if (fThinSlice) IsoTsIso();
    else IsoToIso();
    break;
  default:
    IsoToAni();
    break;
    }
    break;
  default:
    switch (fAnist) {
  case 0:
    AniToIso();
    break;
  default:
    AniToAni();
    break;
    }
    break;
    }
    //Now we have to choose the wave which will be taken by the photon
    Choose();
    if (fAnomaly) return;
    fPhtLive = !fPhtAbs;
    //Keep the phase between 0 and 2pi.
    Double_t d2pi = TMath::TwoPi();
    if (fBeta>d2pi) fBeta -= d2pi;
    else if (fBeta<zero) fBeta += d2pi;
    //Return from the coord. system of the incident plane (ICS) to WCS
    if (fPhtLive) ICStoWCS();
}
void TPhoton::VerifyAtoI() {
  //Verify that system of 13 equations has been correctly solved for the case
  //anisotropic to isotropic
  const Double_t small = 1.0e-5;
  Short_t k;
  Double_t dd;
  TSComplex zh1;
  TSComplex ceps[13];
  TSComplex nr2,nv2;
  TSComplex str2,ctr2,sctr,stv2,ctv2,sctv;
  Double_t d00,d01,d02,d11,d12,d22;
  d00 = fDztii(0,0);
  d01 = fDztii(0,1);
  d02 = fDztii(0,2);
  d11 = fDztii(1,1);
  d12 = fDztii(1,2);
  d22 = fDztii(2,2);
  nr2 = fR1->fRfI*fR1->fRfI;
  nv2 = fR2->fRfI*fR2->fRfI;
  str2 = fR1->fStht*fR1->fStht;
  ctr2 = fR1->fCtht*fR1->fCtht;
  sctr = fR1->fStht*fR1->fCtht;
  stv2 = fR2->fStht*fR2->fStht;
  ctv2 = fR2->fCtht*fR2->fCtht;
  sctv = fR2->fStht*fR2->fCtht;
  //Here are the 13 equations of system (E1) :
  zh1      = fI->fRfI*fI->fStht;
  ceps[0]  = fR1->fRfI*fR1->fStht - zh1;
  ceps[1]  = fR2->fRfI*fR2->fStht - zh1;
  ceps[2]  = fT1->fRfI*fT1->fStht - zh1;
  //
  ceps[3]  = (nr2*str2-d00)*fR1->fSdel*fR1->fCalf +
    (nr2*sctr-d01)*fR1->fCdel*fR1->fCalf +
    d02*fR1->fSalf;
  ceps[4]  = (nr2*sctr-d01)*fR1->fSdel*fR1->fCalf +
    (nr2*ctr2-d11)*fR1->fCdel*fR1->fCalf +
    d12*fR1->fSalf;
  ceps[5]  = d02*fR1->fSdel*fR1->fCalf + d12*fR1->fCdel*fR1->fCalf +
    (nr2-d22)*fR1->fSalf;
  //
  ceps[6]  = (nv2*stv2-d00)*fR2->fSdel*fR2->fCalf +
    (nv2*sctv-d01)*fR2->fCdel*fR2->fCalf +
    d02*fR2->fSalf;
  ceps[7]  = (nv2*sctv-d01)*fR2->fSdel*fR2->fCalf +
    (nv2*ctv2-d11)*fR2->fCdel*fR2->fCalf +
    d12*fR2->fSalf;
  ceps[8]  = d02*fR2->fSdel*fR2->fCalf + d12*fR2->fCdel*fR2->fCalf +
    (nv2-d22)*fR2->fSalf;
  //
  ceps[9]  = fBI*fI->fE*fI->fCalf*fI->fAlm1 +
    fBR1*fR1->fE*fR1->fCalf*fR1->fAlm1 +
    fBR2*fR2->fE*fR2->fCalf*fR2->fAlm1 - fT1->fE*fT1->fCalf;
  ceps[10] = fI->fE*fI->fSalf + fR1->fE*fR1->fSalf + fR2->fE*fR2->fSalf -
    fT1->fE*fT1->fSalf;
  ceps[11] = fI->fE*fI->fCdel*fI->fCalf    - fR1->fE*fR1->fCdel*fR1->fCalf -
    fR2->fE*fR2->fCdel*fR2->fCalf - fT1->fE*fT1->fCtht*fT1->fCalf;
  ceps[12] = fBI*fI->fE*fI->fSalf*fI->fCtht -
    fBR1*fR1->fE*fR1->fSalf*fR1->fCtht -
    fBR2*fR2->fE*fR2->fSalf*fR2->fCtht -
    fT1->fE*fT1->fSalf*fT1->fCtht;
  for (k=0;k<13;k++) {
    dd = Abs(ceps[k]);
    if (dd>small) {
      gMes->SetName("VerifyAtoI");
      gMes->SetTitle("TPhoton");
      PrintWhich();
      gMes->M(kill_this,1,"This equation has a bad solution",k,dd);
      fAnomaly = kTRUE;
      return;
    }
  }
}
void TPhoton::VerifyItoA() {
  //Verify that system of 13 equations has been correctly solved for the case
  //isotropic to anisotropic
  const Double_t small = 1.0e-5;
  Short_t k;
  Double_t dd;
  TSComplex ceps[12];
  TSComplex zh1,nt2,nv2,stt2,ctt2,sctt,stv2,ctv2,sctv;
  Double_t d00,d01,d02,d11,d12,d22;
  d00     = fDztti(0,0);
  d01     = fDztti(0,1);
  d02     = fDztti(0,2);
  d11     = fDztti(1,1);
  d12     = fDztti(1,2);
  d22     = fDztti(2,2);
  zh1     = fI->fRfI*fI->fStht;
  nt2     = fT1->fRfI*fT1->fRfI;
  nv2     = fT2->fRfI*fT2->fRfI;
  stt2    = fT1->fStht*fT1->fStht;
  ctt2    = fT1->fCtht*fT1->fCtht;
  sctt    = fT1->fStht*fT1->fCtht;
  stv2    = fT2->fStht*fT2->fStht;
  ctv2    = fT2->fCtht*fT2->fCtht;
  sctv    = fT2->fStht*fT2->fCtht;
  ceps[0] = fT1->fRfI*fT1->fStht - zh1;
  ceps[1] = fT2->fRfI*fT2->fStht - zh1;
  //
  ceps[2] = (nt2*stt2-d00)*fT1->fSdel*fT1->fCalf +
    (nt2*sctt+d01)*fT1->fCdel*fT1->fCalf +
    d02*fT1->fSalf;
  ceps[3] = (nt2*sctt+d01)*fT1->fSdel*fT1->fCalf +
    (nt2*ctt2-d11)*fT1->fCdel*fT1->fCalf -
    d12*fT1->fSalf;
  ceps[4] = d02*fT1->fSdel*fT1->fCalf - d12*fT1->fCdel*fT1->fCalf +
    (nt2-d22)*fT1->fSalf;
  //
  ceps[5] = (nv2*stv2-d00)*fT2->fSdel*fT2->fCalf +
    (nv2*sctv+d01)*fT2->fCdel*fT2->fCalf +
    d02*fT2->fSalf;
  ceps[6] = (nv2*sctv+d01)*fT2->fSdel*fT2->fCalf +
    (nv2*ctv2-d11)*fT2->fCdel*fT2->fCalf -
    d12*fT2->fSalf;
  ceps[7] = d02*fT2->fSdel*fT2->fCalf - d12*fT2->fCdel*fT2->fCalf +
    (nv2-d22)*fT2->fSalf;
  //
  ceps[8] = fI->fE*fI->fCalf + fR1->fE*fR1->fCalf -
    fBT1*fT1->fE*fT1->fCalf*fT1->fAlm1 -
    fBT2*fT2->fE*fT2->fCalf*fT2->fAlm1;
  ceps[9] = fI->fE*fI->fSalf   + fR1->fE*fR1->fSalf -
    fT1->fE*fT1->fSalf - fT2->fE*fT2->fSalf;
  ceps[10]= fI->fCtht*(fI->fE*fI->fCalf - fR1->fE*fR1->fCalf) -
    fT1->fE*fT1->fCdel*fT1->fCalf -
    fT2->fE*fT2->fCdel*fT2->fCalf;
  ceps[11]= fI->fCtht*(fI->fE*fI->fSalf - fR1->fE*fR1->fSalf) -
    fBT1*fT1->fE*fT1->fCtht*fT1->fSalf -
    fBT2*fT2->fE*fT2->fCtht*fT2->fSalf;
  for (k=0;k<12;k++) {
    dd = Abs(ceps[k]);
    if (dd>small) {
      gMes->SetName("VerifyItoA");
      gMes->SetTitle("TPhoton");
      gMes->M(kill_this,1,"This equation has a bad solution",k,dd);
      fAnomaly = kTRUE;
      return;
    }
  }
}
void TPhoton::VolDet() {
  //Handles the case of a photon having stopped inside a volume detector
  const Double_t zero  = 0.0;
  const Double_t un    = 1.0;
  Int_t bin;
  //Finds the number of this volume detector
  Int_t kVolD;         //Volume detector number
  kVolD = fS->GetDetNumber();
  //Finds pointer towards histograms of this volume detector
  TStatVolD *pstVolDg;
  TStatVolD *pstVolDc = 0;
  pstVolDg = (TStatVolD *)gGs->fVolDet[kVolD];
  if (gCs)
    pstVolDc = (TStatVolD *)gCs->fVolDet[kVolD];
  //Is there a quantum efficiency ?
  Bool_t isqe;
  isqe = fS->IsQE();
  Double_t y1 = zero;
  Double_t y2 = un;
  if (isqe) {
    TSplineFit *pQE;
    pQE = fS->GetQuantumEfficiency();
    y1 = gRandom3->Rndm();
    y2 = pQE->V(fWvlgth);
  }
  if (y1<=y2) {
    //Here, the photon is seen
    fPhtSeen = kTRUE;
    gGs->fNpSeen += 1;
    bin = gGs->fHTimeSeen->Fill(fT0);
    bin = gGs->fHDistSeen->Fill(fSl0);
    bin = gGs->fHWvlgthSeen->Fill(fWvlgth);
    pstVolDg->fNpSeen += 1;
    bin = pstVolDg->fHTimeSeen->Fill(fT0);
    bin = pstVolDg->fHWvlgthSeen->Fill(fWvlgth);
    bin = pstVolDg->fHQEff->Fill(fWvlgth);
    bin = pstVolDg->fHInside->Fill(fWvlgth);
    if (gCs) {
      gCs->fNpSeen += 1;
      bin = gCs->fHTimeSeen->Fill(fT0);
      bin = gCs->fHDistSeen->Fill(fSl0);
      bin = gCs->fHWvlgthSeen->Fill(fWvlgth);
      pstVolDc->fNpSeen += 1;
      bin = pstVolDc->fHTimeSeen->Fill(fT0);
      bin = pstVolDc->fHWvlgthSeen->Fill(fWvlgth);
      bin = pstVolDc->fHQEff->Fill(fWvlgth);
      bin = pstVolDc->fHInside->Fill(fWvlgth);
    }
  }
  else {
    //Here, the photon is not seen, because of quantum efficiency
    gGs->fNpLossAny += 1;
    gGs->fNpLossQE  += 1;
    bin = gGs->fHAbsAny->Fill(fSl0);
    pstVolDg->fNpNotSeen += 1;
    bin = pstVolDg->fHWvlgthNot->Fill(fWvlgth);
    bin = pstVolDg->fHInside->Fill(fWvlgth);
    if (gCs) {
      gCs->fNpLossAny += 1;
      gCs->fNpLossQE  += 1;
      bin = gCs->fHAbsAny->Fill(fSl0);
      pstVolDc->fNpNotSeen += 1;
      bin = pstVolDc->fHWvlgthNot->Fill(fWvlgth);
      bin = pstVolDc->fHInside->Fill(fWvlgth);
    }
  }
}
