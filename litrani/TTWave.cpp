// @(#)root/html:$Id: TTWave.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "TMath.h"
#include "TMessErr.h"
#include "TTWave.h"

ClassImp(TTWave)
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
//  TTWave contains all information about one wave in the ICS ( coordinate system
//of incidence ). This wave may be the incident wave, the 1st or the 2nd
//reflected wave, the 1st or the 2nd transmitted wave. The name "name" identifies
//the kind of wave.
//  fStht and fCtht are the sin and the cos of angle between k vector and normal
//to surface separating the 2 media. It is the angle of incidence for the
//incident wave, the angle of reflection for a reflected wave, the angle of
//transmission for a transmitted wave. It may be complex in all cases, except
//for the incident wave.
//  delta and alpha angle are a way of representing the E field :
//
//  For the incident or transmitted wave :
//
//    fEf(0) = - fAE*fSdel*fCalf
//    fEf(1) = + fAE*fCdel*fCalf
//    fEf(2) = + fAE*fSalf
//
//
//  For the reflected wave :
//
//    fEf(0) = - fAE*fSdel*fCalf
//    fEf(1) = - fAE*fCdel*fCalf
//    fEf(2) = + fAE*fSalf
//
//  Note concerning fPhases:  Number of phases for this wave :
//
//      0 : nothing to add to the absolute phase of the wave
//      1 : it will have to be simply added to fBeta, the irrelevant absolute phase of the wave
//      2 : the parallel and normal components have different phases,
//           wave elliptically polarized. We will have to choose between the 2
//      3 : if the number of phases is 3, the wave is absorbed.
//
TTWave::TTWave(const char *name,const char *title): TNamed(name,title) {
  //Constructor
  fEva = kFALSE;
}
void TTWave::Split(Bool_t isanisotropic) {
  //Split the Electric field according to the decomposition :
  //
  //    fEf(0) => - fAE*fSdel*fCalf
  //    fEf(1) => + fAE*fCdel*fCalf
  //    fEf(2) => + fAE*fSalf
  //
  //This has to be done only for the incident wave. For the other waves, things go
  //the other way, we first find the angles, and then go to the field components.
  //For the incident wave, all variables are real !
  const Double_t zero   = 0.0;
  const Double_t un     = 1.0;
  const Double_t vsmall = 1.0e-12;
  Double_t a,b,c,d,f,g;
  Double_t salf,calf,sdel,cdel,sde2,cde2,stht,ctht;
  T3Vector v1;
  stht    = fStht.Re();
  ctht    = fCtht.Re();
  fAE     = fEf.Module();
  v1      = fEf;
  v1.Unit();
  a       = v1(0);
  b       = v1(1);
  calf    = TMath::Sqrt(a*a+b*b);
  salf    = v1(2);
  if (isanisotropic) {
    //We have 2 possibilities for the sin and cos of delta, because
    //we are free to choose the sign of calf. We choose the solution
    //for which (sdel,cdel) is closest to (fStht,fCtht). This
    //choice is natural, because the 2 are equal in isotropic case.
    if (calf>vsmall) {
      Double_t da,db;
      g    = TMath::ATan2(-a,b);
      sdel = TMath::Sin(g);
      cdel = TMath::Cos(g);
      sde2 = -sdel;
      cde2 = -cdel;
      da   = TMath::Abs(sdel-stht) + TMath::Abs(cdel-ctht);
      db   = TMath::Abs(sde2-stht) + TMath::Abs(cde2-ctht);
      if (db<da) {
        sdel = sde2;
        cdel = cde2;
      }
      //What is the sign of calf ?
      //   It is equal to the sign of c = -v1(0)*sdel  if c is not 0
      //   It is equal to the sign of d =  v1(1)*cdel  if d is not 0
      //   If both c and d are 0 we cannot be here
      c = -a*sdel;
      d =  b*cdel;
      if (TMath::Abs(c)>= TMath::Abs(d)) f = c;
      else f = d;
      calf = TMath::Sign(calf,f);
    }
    else {
      //If the 1st 2 components are 0, it means that calf is 0. In that case, sdel and
      //cdel can have any value, it does not matter. We choose to assign to sdel and
      //cdel the values of fStht and fCtht.
      calf = zero;
      salf = TMath::Sign(un,salf);
      sdel = stht;
      cdel = ctht;
    }
  }
  else {
    //Isotropic case
    sdel = stht;
    cdel = ctht;
    if (calf>vsmall) {
      c = -a*sdel;
      d =  b*cdel;
      if (TMath::Abs(c)>= TMath::Abs(d)) f = c;
      else f = d;
      calf = TMath::Sign(calf,f);
    }
    else {
      calf = zero;
      salf = TMath::Sign(un,salf);
    }
  }
  //Record the result
  fE    = fAE;
  fSdel = sdel;
  fCdel = cdel;
  fSalf = salf;
  fWsaf = salf;
  fPsaf = zero;
  fCalf = calf;
  fWcaf = calf;
  fPcaf = zero;
}
Bool_t TTWave::VerifySplit() const {
  //Verifies the work done by Split
  const Double_t small = 1.0e-6;
  Bool_t bad = kFALSE;
  Double_t a,b,c;
  a = fEf(0) + fAE*fSdel.Re()*fCalf.Re();
  b = fEf(1) - fAE*fCdel.Re()*fCalf.Re();
  c = fEf(2) - fAE*fSalf.Re();
  a = TMath::Abs(a) + TMath::Abs(b) + TMath::Abs(c);
  if (a>small) {
    bad = kTRUE;
    gMes->SetName("VerifySplit");
    gMes->SetTitle("TTWave");
    gMes->M(Error_M,1,"Bad decomposition of the Electric field",a);
  }
  return bad;
}
Bool_t TTWave::VerifyStart(Double_t n, const T33Matrix &m) const {
  //Verify starting conditions of incident wave in case of anisotropic medium
  const Double_t small = 1.0e-4;
  Bool_t bad = kFALSE;
  Double_t a,b,c,bb;
  T3Vector v;
  T33Matrix aam;
  bb = n*n;
  a = fUk(0);   b = fUk(1);   c = fUk(2);
  aam(0,0) = bb*(b*b + c*c) - m(0,0);
  aam(0,1) = - (bb*a*b + m(0,1));
  aam(0,2) = - (bb*a*c + m(0,2));
  aam(1,0) = - (bb*a*b + m(1,0));
  aam(1,1) = bb*(a*a + c*c) - m(1,1);
  aam(1,2) = - (bb*b*c + m(1,2));
  aam(2,0) = - (bb*a*c + m(2,0));
  aam(2,1) = - (bb*b*c + m(2,1));
  aam(2,2) = bb*(a*a + b*b) - m(2,2);
  v  = aam*fEf;
  bb = v.Module();
  if (bb>small) {
    bad = kTRUE;
    gMes->SetName("VerifyStart");
    gMes->SetTitle("TTWave");
    gMes->M(Error_M,1,"Bad initial condition",bb);
  }
  return bad;
}
