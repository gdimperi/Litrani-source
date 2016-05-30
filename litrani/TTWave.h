// @(#)root/html:$Id: TTWave.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TTWave
#define LITR_TTWave
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//   TTWave    characteristics of incident, reflected or transmitted    //
//            wave. All values in the incidence coordinate system ICS   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TSComplex.h"
#include "T3Vector.h"

enum Sharing { NotInvolved,NotTaken,Taken,NormalTaken,ParallelTaken };

class TTWave:public TNamed {

  friend class TPhoton;

protected:

  Bool_t    fEva;     //True if wave evanescent
  Sharing   fShare;   //How this wave participates to process
  Short_t   fPhases;  //Number of phases for this wave
  // 0 : nothing to add to the absolute phase of the wave
  // 1 : it will have to be simply added to fBeta, the irrelevant absolute phase of the wave
  // 2 : the parallel and normal components have different phases, wave elliptically polarized. We will have to choose between the 2
  // 3 : if the number of phases is 3, the wave is absorbed.
  TSComplex fRfI;     //Refraction index associated with this wave
  Double_t  fDiel;    //Dielectric constant epsilon
  TSComplex fCtht;    //cos of angle between k vector and normal to surface separating the 2 media. tht is the angle of incidence, reflection or transmission according to the kind of wave.
  TSComplex fStht;    //sin of angle between k vector and normal to surface separating the 2 media. tht is the angle of incidence, reflection or transmission according to the kind of wave.
  Double_t  fAk;      //module of k vector
  T3Vector  fUk;      //unit k vector in ICS
  T3Vector  fVk;      //k vector in ICS
  TSComplex fE;       //module of electric field
  Double_t  fAE;      //absolute value of fE
  Double_t  fPE;      //phase of fE
  TSComplex fSdel;    //sin of delta angle of electric field
  TSComplex fCdel;    //cos of delta angle of electric field
  TSComplex fAlm1;    // = Stht*Sdel + Ctht*Cdel
  TSComplex fSalf;    //sin of alpha angle of electric field
  Double_t  fWsaf;    //absolute value of fSalf
  Double_t  fPsaf;    //phase of fSalf
  TSComplex fCalf;    //cos of alpha angle of electric field
  Double_t  fWcaf;    //absolute value of fCalf
  Double_t  fPcaf;    //phase of fCalf
  T3Vector  fEf;      //electric field E in ICS [MKSA]
  T3Vector  fEfn;     //E field normal to incident plane
  T3Vector  fEfp;     //E field inside the incident plane
  T3Vector  fBf;      //magnetic field B in ICS [MKSA]
  T3Vector  fBfn;     //magnetic field associated with fEfn
  T3Vector  fBfp;     //magnetic field associated with fEfp
  T3Vector  fPg;      //Poyinting vector P in ICS [MKSA]
  T3Vector  fPgn;     //Poyinting vector P in ICS [MKSA] associated with fEfn
  T3Vector  fPgp;     //Poyinting vector P in ICS [MKSA] associated with fEfp
  T3Vector  fUp;      //Unit Poyinting vector in ICS
  T3Vector  fUpn;     //Unit Poyinting vector in ICS associated with fEfn
  T3Vector  fUpp;     //Unit Poyinting vector in ICS associated with fEfp


public:

  TTWave() { ; }
  TTWave(const char*, const char*);
  void      Split(Bool_t);
  Bool_t    VerifySplit() const;
  Bool_t    VerifyStart(Double_t, const T33Matrix&) const;
  ClassDef(TTWave,1)//characteristics of el-magn. wave
};
#endif
