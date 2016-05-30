// @(#)root/html:$Id: TCleanOut.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TCleanOut
#define LIT_TCleanOut
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// TCleanOut  Class emitting standardized messages onto output and listing  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include "Riostream.h"
#include "TObject.h"
#include "TString.h"
#include "TComplex.h"

enum KindOfMessage {always,debug,info,warning,error,killthis,fatal};

class TCleanOut : public TObject {

protected:

  Bool_t         fWithListing; //If true, a listing is produced
  Bool_t         fIsAdjusted;  //! If true, message shorter or equal to fMaxLength
  UShort_t       fMaxLength;   //Maximum length of any message
  UShort_t       fLClassMet;   //Length for printing class::method calling
  UShort_t       fLengthDV;    //! Imposed length of a text defining a variable
  UShort_t       fLength1C;    //! Imposed length of a text defining a variable when 1 complex number is present
  UShort_t       fLength2C;    //! Imposed length of a text defining a variable when 2 complex numbers are present
  UShort_t       fWidth;       //Width used to print any variable, boolean, integer or real
  UShort_t       fPrecision;   //Precision used to print real variables. Better choose fWidth-fPrecision = 8
  UShort_t       fWidth1C;     //Width used to print a complex variable, when 1 complex number is present
  UShort_t       fPrecision1C; //Precision used to print a complex variables, when 1 complex number is present
  UShort_t       fWidth2C;     //Width used to print a complex variable, when 2 complex numbers are present
  UShort_t       fPrecision2C; //Precision used to print a complex variables, when 2 complex numbers are present
  TString        fNameListing; //Name of the listing file with suffixe
  Long64_t       fRun;         //Run number provided by RunEv
  Long64_t       fNev;         //Event number provided by RunEv
  Long64_t       fPeriod1;     //Period of publication of periodic messages
  Long64_t       fPeriod2;     //Other period of publication of periodic messages
  Long64_t       fPeriod3;     //Still an other period of publication of periodic messages
  Long64_t       fRunRestart;  //If ((fRun==fRunRestart) && (fNev==fNevRestart)) printing restart
  Long64_t       fNevRestart;  //If ((fRun==fRunRestart) && (fNev==fNevRestart)) printing restart
  Int_t          fDepth;       //Depth of method called, used by debug messages
  UInt_t         fNbDebug;     //! Number of messages of type debug printed
  UInt_t         fNbInfo;      //! Number of messages of type info printed
  UInt_t         fNbWarning;   //! Number of messages of type warning printed
  UInt_t         fNbError;     //! Number of messages of type error printed
  UInt_t         fNbKill;      //! Number of messages of type killthis printed
  ofstream       fListing;     //! Listing file
  TString        fLine;        //! Line for output
  TString        fList;        //! Line for listing
  stringbuf     *fPbuf;        //! String of fOss
  ostringstream *fOss;         //! Output into string
  char           fSbuf[256];   //! Temporary array of char
  string         fCbuf;        //! Temporary string

  void         Adjust();
  void         Extract(Int_t);
  void         Init(Short_t);
  void         ML(KindOfMessage,const char*);
  void         MML(KindOfMessage,const char*,const char*,const char * = "");
  void         MMTL(KindOfMessage,const char*,const char*,const char*,const char*,const char * = "");
  void         MMBL(KindOfMessage,const char*,const char*,const char*,Bool_t,const char * = "");
  void         MMIL(KindOfMessage,const char*,const char*,const char*,Long64_t,const char * = "");
  void         MMRL(KindOfMessage,const char*,const char*,const char*,Double_t,const char * = "");
  void         MMCL(KindOfMessage,const char*,const char*,const char*,TComplex,const char * = "");
  void         MTTL(KindOfMessage,const char*,const char*,const char *,const char*);
  void         MTBL(KindOfMessage,const char*,const char*,const char *,Bool_t);
  void         MTIL(KindOfMessage,const char*,const char*,const char *,Long64_t);
  void         MTRL(KindOfMessage,const char*,const char*,const char *,Double_t);
  void         MTCL(KindOfMessage,const char*,const char*,const char *,TComplex);
  void         MBBL(KindOfMessage,const char*,Bool_t,const char *,Bool_t);
  void         MBIL(KindOfMessage,const char*,Bool_t,const char *,Long64_t);
  void         MBRL(KindOfMessage,const char*,Bool_t,const char *,Double_t);
  void         MBCL(KindOfMessage,const char*,Bool_t,const char *,TComplex);
  void         MIIL(KindOfMessage,const char*,Long64_t,const char *,Long64_t);
  void         MIRL(KindOfMessage,const char*,Long64_t,const char*,Double_t);
  void         MICL(KindOfMessage,const char*,Long64_t,const char*,TComplex);
  void         MRRL(KindOfMessage,const char*,Double_t,const char *,Double_t);
  void         MRCL(KindOfMessage,const char*,Double_t,const char *,TComplex);
  void         MCCL(KindOfMessage,const char*,TComplex,const char*,TComplex);
  Bool_t       SetStamp(KindOfMessage);

public:

  UInt_t       fMaxDebug;    //maximum number of messages of type debug to print
  UInt_t       fMaxInfo;     //maximum number of messages of type info to print
  UInt_t       fMaxWarning;  //maximum number of messages of type warning to print
  UInt_t       fMaxError;    //maximum number of messages of type error to print
  UInt_t       fMaxKill;     //maximum number of messages of type killthis to print

  TCleanOut(Short_t = 79);
  TCleanOut(const char*,const char* = "lis",Short_t = 79);
  virtual     ~TCleanOut();
  void         CloseListing();
  void         KillProg();
  void         M(KindOfMessage,const char*);
  void         MM(KindOfMessage,const char*,const char*,const char * = "");
  void         MMT(KindOfMessage,const char*,const char*,const char*,const char*,const char * = "");
  void         MMB(KindOfMessage,const char*,const char*,const char*,Bool_t,const char * = "");
  void         MMI(KindOfMessage,const char*,const char*,const char*,Long64_t,const char * = "");
  void         MMR(KindOfMessage,const char*,const char*,const char*,Double_t,const char * = "");
  void         MMC(KindOfMessage,const char*,const char*,const char*,TComplex,const char * = "");
  void         MTT(KindOfMessage,const char*,const char*,const char *,const char*);
  void         MTB(KindOfMessage,const char*,const char*,const char *,Bool_t);
  void         MTI(KindOfMessage,const char*,const char*,const char *,Long64_t);
  void         MTR(KindOfMessage,const char*,const char*,const char *,Double_t);
  void         MTC(KindOfMessage,const char*,const char*,const char *,TComplex);
  void         MBB(KindOfMessage,const char*,Bool_t,const char *,Bool_t);
  void         MBI(KindOfMessage,const char*,Bool_t,const char *,Long64_t);
  void         MBR(KindOfMessage,const char*,Bool_t,const char *,Double_t);
  void         MBC(KindOfMessage,const char*,Bool_t,const char *,TComplex);
  void         MII(KindOfMessage,const char*,Long64_t,const char *,Long64_t);
  void         MIR(KindOfMessage,const char*,Long64_t,const char *,Double_t);
  void         MIC(KindOfMessage,const char*,Long64_t,const char *,TComplex);
  void         MRR(KindOfMessage,const char*,Double_t,const char *,Double_t);
  void         MRC(KindOfMessage,const char*,Double_t,const char *,TComplex);
  void         MCC(KindOfMessage,const char*,TComplex,const char *,TComplex);
  Bool_t       Period1() const                        { return (!(fNev%fPeriod1));           }
  Bool_t       Period2() const                        { return (!(fNev%fPeriod2));           }
  Bool_t       Period3() const                        { return (!(fNev%fPeriod3));           }
  void         Reset();
  inline void  RunEv(Long64_t,Long64_t);
  void         SetListing(const char*,const char* = "lis");
  Bool_t       SetMaxLength(Short_t);
  void         SetPeriod1(Long64_t period)            { fPeriod1 = period;                   }
  void         SetPeriod2(Long64_t period)            { fPeriod2 = period;                   }
  void         SetPeriod3(Long64_t period)            { fPeriod3 = period;                   }
  void         SetRestart(Long64_t run,Long64_t nev)  { fRunRestart=run; fNevRestart = nev;  }
  Bool_t       SetWidth(Short_t);
  ClassDef(TCleanOut,1)//Class emitting standardized messages onto output and listing
};
inline void  TCleanOut::RunEv(Long64_t run,Long64_t nev) {
  // Inform TCleanOut of run number and ev number
  fRun = run;
  fNev = nev;
  if ((fRun==fRunRestart) && (fNev==fNevRestart)) {
    fNbDebug     = 0;
    fNbInfo      = 0;
    fNbWarning   = 0;
    fNbError     = 0;
    fNbKill      = 0;
  }
}
R__EXTERN TCleanOut *gCleanOut;
#endif
