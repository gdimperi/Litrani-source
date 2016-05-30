// @(#)root/html:$Id: TCleanOut.cxx 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#include "Rstrstream.h"
#include "TApplication.h"
#include "TCleanOut.h"

TCleanOut *gCleanOut = 0;

ClassImp(TCleanOut)
//______________________________________________________________________________
//
//  CleanOut   class emitting standardized messages onto output and optionnally
//             inside a listing file. There are 7 kind of messages, according to
//             enum KindOfMessage:
//
// - always     labelled with 'A'. These messages are always printed. No message
//              counter can stop them.
// - debug      labelled with 'D'. These messages are in principle reserved for
//              the case where the calling program has been compiled in debug mode.
//              For messages of the method MM(), and only for this method called with
//              1st argument equal to "debug", if the 1st character of the text of
//              the message in the 3rd argument is '+', fDepth will be increased by 1
//              [advisable for message at the entry of a method].  If the 1st character
//              of the text of a message is '-', fDepth will be decreased by 1
//              [advisable for message at the exit of a method]. These 2 kinds of
//              messages will print fDepth. Always use these messages in pair.
// - info       labelled with 'I'    These messages are harmless and are there
//              only to inform about some details.
// - warning    labelled with 'W'    These are also harmless, but you should
//              take them into account because they may point to errors in your
//              code.
// - error      labelled with 'E'    They point to something which is serious,
//              but the program will continue aniway. The program may crash or
//              produce bad results.
// - killthis   labelled with 'K'    A serious problem, such as a big lack of
//              precision for a calculation, has been encountered on a particular
//              event. On result, the calling program should kill this event and
//              go to handle the next event. For instance, if the program is a
//              Monte-Carlo generating photons, like Litrani, the calling program
//              should kill this photon, add 1 to the number of bad photons and
//              go to generate the next photon. If the number of such messages
//              becomes bigger than fMaxKill, then the program is stopped.
// - fatal      labelled with 'F'    The encountered problem is so serious
//              that the program has to be and will be stopped.
//
// All message types which are provided with a counter [debug,info,warning,error,
//killthis] cease to be printed when their counter reach the maximum number
//allowed [all public variables fMax...]. These maximum can be changed freely
//by the user since they are public variables. If one wants at a certain time
//that printing restarts, then one has to call method Reset(). If one wants
//that printing restarts after a certain event of a given run, then one has to
//call method SetRestart(). But all this is working ONLY if EACH time you generate
//or handle a new event, you call RunEv().
//
// You have also the possibility that a message is published only periodically,
//each fPeriod1(/2/3) events. There are 3 periods available. If it is what you want
//for a given message, then instead of calling:
//
//        gCleanOut->Mxx(...);
//
//call:
//
//        if (gCleanOut->Period1()) gCleanOut->Mxx(...);
//
//But again, this is working ONLY if EACH time you generate or handle a new event,
//you call RunEv()! These periodic messages can be useful when doing a long job
//to verify that the program is always running.
//
// In principle, only one instance of the class TCleanOut is necessary, although
//it is not forbidden to have many. Perhaps you need 2 listings? The last
//instance of TCleanOut created is pointed to by a global pointer gCleanOut.
//You are free to make gCleanOut point to an other instance of the class.
//
// If you have used the 1st constructor [the default constructor], no listing
//will be produced unless you call method SetListing(). If you have used the
//2nd constructor, you will have a listing, in which case all messages issued by the
//class TCleanOut go to the output window AND to the listing file fListing. Notice that
//on the listing file, there are "tab" ('\t') separators, so that the listing file can
//be cleanly opened by Excel. If you want to stop writing onto the listing, call
//method CloseListing(). If you want to restart a listing, call SetListing().
//
// Through the global pointer gCleanOut, the user can also issue messages from within
//his CINT code. These messages will appear in the output AND and in the listing.
//
// The maximum length of one line of message, fMaxLength, is set to 79 characters
//by default, in order that they be well adapted to the default width of 80 of the
//"command prompt window" of Windows XP. But we strongly recommand that you change
//this default to some bigger value, (for instance 119) [1st argument of 1st constructor
//or 3rd argument of 2nd constructor or 1st argument of SetMaxLength()],
//not forgetting to ALSO change the width of the "command prompt window" of Windows XP
//to your value + 1 [for instance 120]. Be it on Windows XP or on Unix, the
//messages of TCleanOut will always look ugly if your window has a width smaller
//than the messages of TCleanOut. You can also change the length of the messages
//of TCleanOut at a later stage, by a call to SetMaxLength().
//
// Following types of messages are available, distinguished by the name of the method:
//
//
//        Message containing only one line of text
//
// ( 1) M   : pure text messages, containing only one line of text.
//
//  For the messages [2 to 6]
//     - the 2nd argument is the name of the method calling
//     - the last argument is the name of the class. If your class derive from
//       TObject, simply put ClassName() as last argument. This last argument
//       is optional.
//
//        Messages containing name of class::method calling and a text
//
// ( 2) MM  : message containing name of class::method calling and an explaining
//            text. Last argument is the optional class name.
//
//        Messages containing name of class::method calling, a text and one variable
//
// ( 3) MMT : message containing name of class::method calling, an explaining
//            text, a text defining the following text variable and a text variable.
//            Last argument is the optional class name.
// ( 4) MMB : message containing name of class::method calling, an explaining
//            text, a text defining the boolean variable and a boolean variable.
//            Last argument is the optional class name.
// ( 5) MMI : message containing name of class::method calling, an explaining
//            text, a text defining the integer variable and an integer variable.
//            Last argument is the optional class name.
// ( 5) MMR : message containing name of class::method calling, an explaining
//            text, a text defining the real variable and a real variable.
//            Last argument is the optional class name.
// ( 6) MMC : message containing name of class::method calling, an explaining
//            text, a text defining the complex variable and a complex variable.
//            Last argument is the optional class name.
//
//        Messages containing two variables
//
// ( 7) MTT : message containing a text defining the first text variable, the
//            first text variable, a text defining the second text variable,
//            the second text variable.
// ( 8) MTB : message containing a text defining a text variable, the text variable,
//            a text defining a boolean variable, the boolean variable.
// ( 9) MTI : message containing a text defining a text variable, the text variable,
//            a text defining an integer variable, the integer variable.
// (10) MTR : message containing a text defining a text variable, the text variable,
//            a text defining a real variable, the real variable.
// (11) MTC : message containing a text defining a text variable, the text variable,
//            a text defining a complex variable, the complex variable.
// (12) MBB : message containing a text defining the first boolean variable, the
//            first boolean variable, a text defining the second boolean variable,
//            the second boolean variable.
// (13) MBI : message containing a text defining a boolean variable, the boolean
//            variable, a text defining an integer variable, the integer variable.
// (14) MBR : message containing a text defining a boolean variable, the boolean
//            variable, a text defining a real variable, the real variable.
// (15) MBC : message containing a text defining a boolean variable, the boolean
//            variable, a text defining a complex variable, the complex variable.
// (16) MII : message containing a text defining the first integer variable, the
//            first integer variable, a text defining the second integer variable
//            and the second integer variable.
// (17) MIR : message containing a text defining an integer variable, the integer
//            variable, a text defining a real variable and the real variable.
// (18) MIC : message containing a text defining an integer variable, the integer
//            variable, a text defining a complex variable and the complex variable.
// (19) MRR : message containing a text defining the first real variable, the
//            first real variable, a text defining the second real variable,
//            the second real variable.
// (20) MRC : message containing a text defining a real variable, the real
//            variable, a text defining a complex variable and the complex variable.
// (17) MCC : message containing a text defining the first complex variable, the
//            first complex variable, a text defining the second complex variable,
//            the second complex variable.
//
// All messages are preceded by 'A', 'D', 'I ', 'W ', 'E ', 'K ' or 'F ' according to
//"KindOfMessage".
//
TCleanOut::TCleanOut(Short_t maxlength) {
  // Constructor not requiring a listing. Call SetListing() later, if you want
  //one.
  // - maxlength : maximum length for one line of output.
  //                minimum :  79
  //                maximum : 299
  //                default :  79
  Init(maxlength);
}
TCleanOut::TCleanOut(const char *namelis, const char *suffix, Short_t maxlength) {
  // Constructor requiring a listing, with the following arguments :
  //
  // - namelis   : name of the listing file, without suffix
  // - suffix    : suffix to be appended, without the '.'. Default is "lis"
  // - maxlength : maximum length for one line of output.
  //                minimum :  79
  //                maximum : 299
  //                default :  79
  //
  Init(maxlength);
  fWithListing = kTRUE;
  fNameListing = namelis;
  fNameListing.Append('.');
  fNameListing.Append(suffix);
  fListing.open(fNameListing.Data(), ios::out);
}
TCleanOut::~TCleanOut() {
  //destructor
  fListing.close();
  if (gCleanOut==this) gCleanOut = 0;
}
void TCleanOut::Adjust() {
  // Size of various part of output text adjusted so that the maximum length be
  //less or equal fMaxLength
  const Short_t minimdv =  9;
  const Short_t minimw  = 10;
  const Short_t diffwp  =  8;
  Short_t lengthdv;
  if (fWidth<minimw) {
    fWidth     = minimw;
    fPrecision = fWidth - diffwp;
  }
  fPrecision = fWidth - diffwp;
  lengthdv = (fMaxLength - 2*fWidth - 8)/2;
  while ((lengthdv<minimdv) && (fWidth>minimw)) {
    fWidth--;
    fPrecision = fWidth - diffwp;
    lengthdv = (fMaxLength - 2*fWidth - 8)/2;
  }
  fLengthDV   = lengthdv;
  //Provi
  if (fLengthDV>20) fLengthDV = 20;
  //Fin Provi
  lengthdv    = (fMaxLength - fWidth - (2*fWidth1C + 4) - 8)/2;
  while ((lengthdv<minimdv) && (fWidth1C>minimw)) {
    fWidth1C--;
    fPrecision1C = fWidth1C - diffwp;
    lengthdv    = (fMaxLength - fWidth - (2*fWidth1C + 4) - 8)/2;
  }
  fLength1C   = lengthdv;
  //
  lengthdv    = (fMaxLength - 2*(2*fWidth2C + 4) - 8)/2;
  while ((lengthdv<minimdv) && (fWidth2C>minimw)) {
    fWidth2C--;
    fPrecision2C = fWidth2C - diffwp;
    lengthdv    = (fMaxLength - 2*(2*fWidth2C + 4) - 8)/2;
  }
  fLength2C   = lengthdv;
  fIsAdjusted = kTRUE;
}
void TCleanOut::CloseListing() {
  // Close the listing. Cease to write onto the listing
  fListing.close();
  fWithListing = kFALSE;
}
void TCleanOut::Extract(Int_t N) {
  // Extracts string after formatting
  Int_t k;
  N = TMath::Abs(N);
  if (N>254) N=254;
  fPbuf = fOss->rdbuf();
  fCbuf = fPbuf->str();
  for (k=0;k<N;k++) fSbuf[k] = fCbuf[k];
  fSbuf[N] = '\0';
}
void TCleanOut::Init(Short_t maxlength) {
  //Initialization
  const UShort_t diffwp = 8;
  Bool_t ok;
  fPbuf        = 0;
  fOss         = 0;
  fWithListing = kFALSE;
  ok = SetMaxLength(maxlength);
  if (fMaxLength<90) {
    fWidth     = 16;
    fWidth1C   = 12;
    fWidth2C   = 10;
    fLClassMet = 20;
  }
  else {
    fWidth     = 20;
    fWidth1C   = 20;
    fWidth2C   = 16;
    fLClassMet = 34;
  }
  fPrecision   = fWidth   - diffwp;
  fPrecision1C = fWidth1C - diffwp;
  fPrecision2C = fWidth2C - diffwp;
  Adjust();
  fNameListing = "";
  fRun         = -1;
  fNev         = -1;
  fPeriod1     = 5000;
  fPeriod2     = 50000;
  fPeriod3     = 500000;
  fRunRestart  = -1;
  fNevRestart  = -1;
  fDepth       = 0;
  Reset();
  fMaxDebug    = 10000;
  fMaxInfo     = 100;
  fMaxWarning  = 100;
  fMaxError    = 100;
  fMaxKill     = 1000;
  gCleanOut    = this;
}
void TCleanOut::KillProg() {
  // Stop program after a fatal error
  char c;
  TString s = "A fatal error has been encountered, program will stop";
  cout     << s.Data() << endl;
  if (fWithListing) {
    fListing << s.Data() << endl;
    fListing.close();
  }
  cout << "Hit any character and return" << endl;
  cin  >> c;
  gApplication->Terminate(-1);
}
void TCleanOut::M(KindOfMessage kind,const char *t0) {
  // Pure text messages, containing only one line of text. Sent to output
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Int_t L;
    TString s1;
    if (fWithListing) ML(kind,t0);
    s1 = t0;
    L  = s1.Length();
    if (L>fMaxLength-2) {
      s1.Resize(fMaxLength-3);
      s1.Append('~');
    }
    fLine.Append(s1);
    cout << fLine.Data() << endl;
  }
  if (kind==fatal) KillProg();
}
void TCleanOut::ML(KindOfMessage kind,const char *t0) {
  // Pure text messages, containing only one line of text. Sent to listing
  Int_t L;
  TString s1;
  s1 = t0;
  L  = s1.Length();
  if (L>fMaxLength-2) {
    s1.Resize(fMaxLength-3);
    s1.Append('~');
  }
  fList.Append(s1);
  fListing << fList.Data() << endl;
}
void TCleanOut::MM(KindOfMessage kind,const char *method,const char *t0,const char *clas) {
  // Message containing name of class::method calling and an explaining text.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  method  : name of method calling
  //  t0      : the text of the message
  //  clas    : [optional] name of class calling, accessible by method "ClassName()"
  //
  //  Example of use:
  //
  //  gCleanOut->MM(warning,"Fill","Overflow of bin content",ClassName());
  //
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    if (!fIsAdjusted) Adjust();
    if ((kind==debug) && ((t0[0]=='+') || (t0[0]=='-'))) {
      if (t0[0]=='+') fDepth++;
      else            fDepth--;
      MMI(kind,method,t0,"depth",fDepth,clas);
    }
    else {
      Short_t k;
      Int_t L,LFree;
      TString s1;
      if (fWithListing) MML(kind,method,t0,clas);
      if (clas[0] != '\0') {
        s1 = clas;
        s1.Append("::");
        s1.Append(method);
      }
      else s1 = method;
      L     = s1.Length();
      if (L != fLClassMet-1) {
        if (L>fLClassMet-1) {
          s1.Resize(fLClassMet-2);
          s1.Append('~');
        }
        else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
      }
      s1.Append(' ');
      fLine.Append(s1);
      LFree = fMaxLength - fLClassMet - 2;
      s1 = t0;
      L  = s1.Length();
      if (L>LFree) {
        s1.Resize(LFree-1);
        s1.Append('~');
      }
      fLine.Append(s1);
      cout << fLine.Data() << endl;
    }
  }
  if (kind==fatal) KillProg();
}
void TCleanOut::MML(KindOfMessage kind,const char *method,const char *t0,const char *clas) {
  // Message containing name of class::method calling and an explaining text.
  //Sent to listing
  Short_t k;
  Int_t L,LFree;
  TString s1;
  if (clas[0] != '\0') {
    s1 = clas;
    s1.Append("::");
    s1.Append(method);
  }
  else s1 = method;
  L     = s1.Length();
  if (L != fLClassMet-1) {
    if (L>fLClassMet-1) {
      s1.Resize(fLClassMet-2);
      s1.Append('~');
    }
    else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append('\t');
  LFree = fMaxLength - fLClassMet - 2;
  s1 = t0;
  L  = s1.Length();
  if (L>LFree) {
    s1.Resize(LFree-1);
    s1.Append('~');
  }
  fList.Append(s1);
  fListing << fList.Data() << endl;
}
void TCleanOut::MMT(KindOfMessage kind,const char *method,const char *t0,const char *t1,const char *t2,const char *clas) {
  // Message containing name of class::method calling, an explaining text, a text defining
  //the following text variable and a text variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  method  : name of method calling
  //  t0      : the text of the message
  //  t1      : a short text defining the variable t2
  //  t2      : text variable associated with the message t0
  //  clas    : [optional] name of class calling, accessible by method "ClassName()"
  //
  //  Example of use:
  //
  //  gCleanOut->MMT(warning,"Fill","Overflow of bin content","histo",GetName(),ClassName());
  //
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    Int_t L,LFree;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MMTL(kind,method,t0,t1,t2,clas);
    if (clas[0] != '\0') {
      s1 = clas;
      s1.Append("::");
      s1.Append(method);
    }
    else s1 = method;
    L     = s1.Length();
    if (L != fLClassMet-1) {
      if (L>fLClassMet-1) {
        s1.Resize(fLClassMet-2);
        s1.Append('~');
      }
      else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
    }
    s1.Append(' ');
    fLine.Append(s1);
    //
    LFree = fMaxLength - fLClassMet - fLengthDV - fWidth - 2;
    s1 = t0;
    L  = s1.Length();
    if (L != LFree-1) {
      if (L>LFree-1) {
        s1.Resize(LFree-2);
        s1.Append('~');
      }
      else for (k=0;k<LFree-L-1;k++) s1.Append(' ');
    }
    s1.Append(' ');
    fLine.Append(s1);
    //
    s1 = t1;
    L  = s1.Length();
    if (L != fLengthDV-1) {
      if (L>fLengthDV-1) {
        s1.Resize(fLengthDV-2);
        s1.Append('~');
      }
      else for (k=0;k<fLengthDV-L-1;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    s1 = t2;
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    //
    cout << fLine.Data() << endl;
  }
  if (kind==fatal) KillProg();
}
void TCleanOut::MMTL(KindOfMessage kind,const char *method,const char *t0,const char *t1,const char *t2,const char *clas) {
  // Message containing name of class::method calling, an explaining text, a text defining
  //the following text variable and a text variable. Sent to listing.
  Short_t k;
  Int_t L,LFree;
  TString s1;
  if (clas[0] != '\0') {
    s1 = clas;
    s1.Append("::");
    s1.Append(method);
  }
  else s1 = method;
  L     = s1.Length();
  if (L != fLClassMet-1) {
    if (L>fLClassMet-1) {
      s1.Resize(fLClassMet-2);
      s1.Append('~');
    }
    else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append('\t');
  //
  LFree = fMaxLength - fLClassMet - fLengthDV - fWidth - 2;
  s1 = t0;
  L  = s1.Length();
  if (L != LFree-1) {
    if (L>LFree-1) {
      s1.Resize(LFree-2);
      s1.Append('~');
    }
    else for (k=0;k<LFree-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append("\t\t\t");
  //
  s1 = t1;
  L  = s1.Length();
  if (L != fLengthDV-1) {
    if (L>fLengthDV-1) {
      s1.Resize(fLengthDV-2);
      s1.Append('~');
    }
    else for (k=0;k<fLengthDV-L-1;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  s1 = t2;
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  fListing << fList.Data() << endl;
}
void TCleanOut::MMB(KindOfMessage kind,const char *method,const char *t0,const char *t1,Bool_t b,const char *clas) {
  // Message containing name of class::method calling, an explaining text, a text defining
  //the boolean variable and a boolean variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  method  : name of method calling
  //  t0      : the text of the message
  //  t1      : a short text defining the variable b
  //  b       : boolean variable associated with the message t0
  //  clas    : [optional] name of class calling, accessible by method "ClassName()"
  //
  //  Example of use:
  //
  //  gCleanOut->MMB(warning,"Fill","Statistics not required","IsStat",IsStat,ClassName());
  //
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    Int_t L,LFree;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MMBL(kind,method,t0,t1,b,clas);
    if (clas[0] != '\0') {
      s1 = clas;
      s1.Append("::");
      s1.Append(method);
    }
    else s1 = method;
    L     = s1.Length();
    if (L != fLClassMet-1) {
      if (L>fLClassMet-1) {
        s1.Resize(fLClassMet-2);
        s1.Append('~');
      }
      else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
    }
    s1.Append(' ');
    fLine.Append(s1);
    //
    LFree = fMaxLength - fLClassMet - fLengthDV - fWidth - 2;
    s1 = t0;
    L  = s1.Length();
    if (L != LFree-1) {
      if (L>LFree-1) {
        s1.Resize(LFree-2);
        s1.Append('~');
      }
      else for (k=0;k<LFree-L-1;k++) s1.Append(' ');
    }
    s1.Append(' ');
    fLine.Append(s1);
    //
    s1 = t1;
    L  = s1.Length();
    if (L != fLengthDV-1) {
      if (L>fLengthDV-1) {
        s1.Resize(fLengthDV-2);
        s1.Append('~');
      }
      else for (k=0;k<fLengthDV-L-1;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (b) s1 = "true";
    else   s1 = "false";
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    //
    cout << fLine.Data() << endl;
  }
  if (kind==fatal) KillProg();
}
void TCleanOut::MMBL(KindOfMessage kind,const char *method,const char *t0,const char *t1,Bool_t b,const char *clas) {
  // Message containing name of class::method calling, an explaining text, a text defining
  //the boolean variable and a boolean variable. Sent to listing.
  Short_t k;
  Int_t L,LFree;
  TString s1;
  if (clas[0] != '\0') {
    s1 = clas;
    s1.Append("::");
    s1.Append(method);
  }
  else s1 = method;
  L     = s1.Length();
  if (L != fLClassMet-1) {
    if (L>fLClassMet-1) {
      s1.Resize(fLClassMet-2);
      s1.Append('~');
    }
    else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append('\t');
  //
  LFree = fMaxLength - fLClassMet - fLengthDV - fWidth - 2;
  s1 = t0;
  L  = s1.Length();
  if (L != LFree-1) {
    if (L>LFree-1) {
      s1.Resize(LFree-2);
      s1.Append('~');
    }
    else for (k=0;k<LFree-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append("\t\t\t");
  //
  s1 = t1;
  L  = s1.Length();
  if (L != fLengthDV-1) {
    if (L>fLengthDV-1) {
      s1.Resize(fLengthDV-2);
      s1.Append('~');
    }
    else for (k=0;k<fLengthDV-L-1;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (b) s1 = "true";
  else   s1 = "false";
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  fListing << fList.Data() << endl;
}
void TCleanOut::MMI(KindOfMessage kind,const char *method,const char *t0,const char *t1,Long64_t I,const char *clas) {
  // Message containing name of class::method calling, an explainingtext, a text defining
  //the integer variable and an integer variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  method  : name of method calling
  //  t0      : the text of the message
  //  t1      : a short text defining the variable I
  //  I       : short, int or long64 variable associated with the message t0
  //  clas    : [optional] name of class calling, accessible by method "ClassName()"
  //
  //  Example of use:
  //
  //  gCleanOut->MMI(warning,"Fill","Overflow of bin content","bin",bin,ClassName());
  //
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    Int_t L,LFree;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MMIL(kind,method,t0,t1,I,clas);
    if (clas[0] != '\0') {
      s1 = clas;
      s1.Append("::");
      s1.Append(method);
    }
    else s1 = method;
    L     = s1.Length();
    if (L != fLClassMet-1) {
      if (L>fLClassMet-1) {
        s1.Resize(fLClassMet-2);
        s1.Append('~');
      }
      else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
    }
    s1.Append(' ');
    fLine.Append(s1);
    //
    LFree = fMaxLength - fLClassMet - fLengthDV - fWidth - 2;
    s1 = t0;
    L  = s1.Length();
    if (L != LFree-1) {
      if (L>LFree-1) {
        s1.Resize(LFree-2);
        s1.Append('~');
      }
      else for (k=0;k<LFree-L-1;k++) s1.Append(' ');
    }
    s1.Append(' ');
    fLine.Append(s1);
    //
    s1 = t1;
    L  = s1.Length();
    if (L != fLengthDV-1) {
      if (L>fLengthDV-1) {
        s1.Resize(fLengthDV-2);
        s1.Append('~');
      }
      else for (k=0;k<fLengthDV-L-1;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fOss->width(fWidth);
    *fOss << I << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    //
    cout << fLine.Data() << endl;
  }
  if (kind==fatal) KillProg();
}
void TCleanOut::MMIL(KindOfMessage kind,const char *method,const char *t0,const char *t1,Long64_t I,const char *clas) {
  // Message containing name of class::method calling, an explainingtext, a text defining
  //the integer variable and an integer variable. Sent to listing.
  Short_t k;
  Int_t L,LFree;
  TString s1;
  if (clas[0] != '\0') {
    s1 = clas;
    s1.Append("::");
    s1.Append(method);
  }
  else s1 = method;
  L     = s1.Length();
  if (L != fLClassMet-1) {
    if (L>fLClassMet-1) {
      s1.Resize(fLClassMet-2);
      s1.Append('~');
    }
    else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append('\t');
  //
  LFree = fMaxLength - fLClassMet - fLengthDV - fWidth - 2;
  s1 = t0;
  L  = s1.Length();
  if (L != LFree-1) {
    if (L>LFree-1) {
      s1.Resize(LFree-2);
      s1.Append('~');
    }
    else for (k=0;k<LFree-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append("\t\t\t");
  //
  s1 = t1;
  L  = s1.Length();
  if (L != fLengthDV-1) {
    if (L>fLengthDV-1) {
      s1.Resize(fLengthDV-2);
      s1.Append('~');
    }
    else for (k=0;k<fLengthDV-L-1;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fOss->width(fWidth);
  *fOss << I << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  fListing << fList.Data() << endl;
}
void TCleanOut::MMR(KindOfMessage kind,const char *method,const char *t0,const char *t1,Double_t r,const char *clas) {
  // message containing name of class::method calling, an explaining text, a text defining
  //the real variable and a real variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  method  : name of method calling
  //  t0      : the text of the message
  //  t1      : a short text defining the variable r
  //  r       : float or double variable associated with the message t0
  //  clas    : [optional] name of class calling, accessible by method "ClassName()"
  //
  //  Example of use:
  //
  //  gCleanOut->MMR(warning,"Fill","Standard deviation too big","sigma",sigma,ClassName());
  //
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    Int_t L,LFree;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MMRL(kind,method,t0,t1,r,clas);
    if (clas[0] != '\0') {
      s1 = clas;
      s1.Append("::");
      s1.Append(method);
    }
    else s1 = method;
    L     = s1.Length();
    if (L != fLClassMet-1) {
      if (L>fLClassMet-1) {
        s1.Resize(fLClassMet-2);
        s1.Append('~');
      }
      else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
    }
    s1.Append(' ');
    fLine.Append(s1);
    //
    LFree = fMaxLength - fLClassMet - fLengthDV - fWidth - 2;
    s1 = t0;
    L  = s1.Length();
    if (L != LFree-1) {
      if (L>LFree-1) {
        s1.Resize(LFree-2);
        s1.Append('~');
      }
      else for (k=0;k<LFree-L-1;k++) s1.Append(' ');
    }
    s1.Append(' ');
    fLine.Append(s1);
    //
    s1 = t1;
    L  = s1.Length();
    if (L != fLengthDV-1) {
      if (L>fLengthDV-1) {
        s1.Resize(fLengthDV-2);
        s1.Append('~');
      }
      else for (k=0;k<fLengthDV-L-1;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fOss->width(fWidth);
    fOss->precision(fPrecision);
    *fOss << r << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    //
    cout << fLine.Data() << endl;
  }
  if (kind==fatal) KillProg();
}
void TCleanOut::MMRL(KindOfMessage kind,const char *method,const char *t0,const char *t1,Double_t r,const char *clas) {
  // Message containing name of class::method calling, an explaining text, a text defining
  //the real variable and a real variable. Sent to listing.
  Short_t k;
  Int_t L,LFree;
  TString s1;
  if (clas[0] != '\0') {
    s1 = clas;
    s1.Append("::");
    s1.Append(method);
  }
  else s1 = method;
  L     = s1.Length();
  if (L != fLClassMet-1) {
    if (L>fLClassMet-1) {
      s1.Resize(fLClassMet-2);
      s1.Append('~');
    }
    else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append('\t');
  //
  LFree = fMaxLength - fLClassMet - fLengthDV - fWidth - 2;
  s1 = t0;
  L  = s1.Length();
  if (L != LFree-1) {
    if (L>LFree-1) {
      s1.Resize(LFree-2);
      s1.Append('~');
    }
    else for (k=0;k<LFree-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append("\t\t\t");
  //
  s1 = t1;
  L  = s1.Length();
  if (L != fLengthDV-1) {
    if (L>fLengthDV-1) {
      s1.Resize(fLengthDV-1);
      s1.Append('~');
    }
    else for (k=0;k<fLengthDV-L-1;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fOss->width(fWidth);
  fOss->precision(fPrecision);
  *fOss << r << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  fListing << fList.Data() << endl;
}
void TCleanOut::MMC(KindOfMessage kind,const char *method,const char *t0,const char *t1,TComplex z,const char *clas) {
  // Message containing name of class::method calling, an explaining text, a text defining
  //the complex variable and a complex variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  method  : name of method calling
  //  t0      : the text of the message
  //  t1      : a short text defining the variable z
  //  z       : complex variable associated with the message t0
  //  clas    : [optional] name of class calling, accessible by method "ClassName()"
  //
  //  Example of use:
  //
  //  gCleanOut->MMC(info,"GetIndex","Index of medium complex","index",index,ClassName());
  //
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    Int_t L,LFree,widthc;
    Int_t L1=0;
    Int_t L2=0;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MMCL(kind,method,t0,t1,z,clas);
    if (clas[0] != '\0') {
      s1 = clas;
      s1.Append("::");
      s1.Append(method);
    }
    else s1 = method;
    L     = s1.Length();
    if (L != fLClassMet-1) {
      if (L>fLClassMet-1) {
        s1.Resize(fLClassMet-2);
        s1.Append('~');
      }
      else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
    }
    s1.Append(' ');
    fLine.Append(s1);
    //
    widthc = 2*fWidth1C + 4;
    LFree = fMaxLength - fLClassMet - fLengthDV - widthc - 2;
    if (LFree<fLengthDV) {
      L1 = (fLengthDV+LFree)/2;
      L2 = L1;
    }
    s1 = t0;
    L  = s1.Length();
    if (L != L1-1) {
      if (L>L1-1) {
        s1.Resize(L1-2);
        s1.Append('~');
      }
      else for (k=0;k<L1-L-1;k++) s1.Append(' ');
    }
    s1.Append(' ');
    fLine.Append(s1);
    //
    s1 = t1;
    L  = s1.Length();
    if (L != L2-1) {
      if (L>L2-1) {
        s1.Resize(L2-2);
        s1.Append('~');
      }
      else for (k=0;k<L2-L-1;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    *fOss << '(';
    fOss->width(fWidth1C);
    fOss->precision(fPrecision1C);
    *fOss << z.Re() << ',';
    fOss->width(fWidth1C);
    fOss->precision(fPrecision1C);
    *fOss << z.Im() << "i)" << ends;
    Extract(2*fWidth1C+4);
    fLine.Append(fSbuf);
    //
    cout << fLine.Data() << endl;
  }
  if (kind==fatal) KillProg();
}
void TCleanOut::MMCL(KindOfMessage kind,const char *method,const char *t0,const char *t1,TComplex z,const char *clas) {
  // Message containing name of class::method calling, an explaining text, a text defining
  //the complex variable and a complex variable. Sent to listing.
  Short_t k;
  Int_t L,LFree,widthc;
  Int_t L1=0;
  Int_t L2=0;
  TString s1;
  if (clas[0] != '\0') {
    s1 = clas;
    s1.Append("::");
    s1.Append(method);
  }
  else s1 = method;
  L     = s1.Length();
  if (L != fLClassMet-1) {
    if (L>fLClassMet-1) {
      s1.Resize(fLClassMet-2);
      s1.Append('~');
    }
    else for (k=0;k<fLClassMet-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append('\t');
  //
  widthc = 2*fWidth1C + 4;
  LFree = fMaxLength - fLClassMet - fLengthDV - widthc - 2;
  if (LFree<fLengthDV) {
    L1 = (fLengthDV+LFree)/2;
    L2 = L1;
  }
  s1 = t0;
  L  = s1.Length();
  if (L != L1-1) {
    if (L>L1-1) {
      s1.Resize(L1-2);
      s1.Append('~');
    }
    else for (k=0;k<L1-L-1;k++) s1.Append(' ');
  }
  s1.Append(' ');
  fList.Append(s1);
  fList.Append("\t\t\t");
  //
  s1 = t1;
  L  = s1.Length();
  if (L != L2-1) {
    if (L>L2-1) {
      s1.Resize(L2-2);
      s1.Append('~');
    }
    else for (k=0;k<L2-L-1;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  *fOss << '(';
  fOss->width(fWidth1C);
  fOss->precision(fPrecision1C);
  *fOss << z.Re() << ',';
  fOss->width(fWidth1C);
  fOss->precision(fPrecision1C);
  *fOss << z.Im() << "i)" << ends;
  Extract(2*fWidth1C+4);
  fList.Append(fSbuf);
  fListing << fList.Data() << endl;
}
void TCleanOut::MTT(KindOfMessage kind,const char *t1,const char *v1,const char * t2,const char *v2) {
  // Message containing a text defining the first text variable, the first text variable,
  //a text defining the second text variable, the second text variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable v1
  //  v1      : 1st text variable
  //  t2      : a short text defining the variable v1
  //  v2      : 2nd text variable
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MTTL(kind,t1,v1,t2,v2);
    lengthdv1 = fLengthDV - 1;
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    s1 = v1;
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    //
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    s1 = v2;
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    cout     << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MTTL(KindOfMessage kind,const char *t1,const char *v1,const char * t2,const char *v2) {
  // Message containing a text defining the first text variable, the first text variable,
  //a text defining the second text variable, the second text variable. Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLengthDV - 1;
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  s1 = v1;
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  s1 = v2;
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  fListing << fList.Data() << endl;
}
void TCleanOut::MTB(KindOfMessage kind,const char *t1,const char *v1,const char * t2,Bool_t b) {
  // Message containing a text defining a text variable, the text variable, a text
  //defining a boolean variable, the boolean variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable v1
  //  v1      : 1st text variable
  //  t2      : a short text defining the variable b
  //  b       : 2nd variable of type boolean
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MTBL(kind,t1,v1,t2,b);
    lengthdv1 = fLengthDV - 1;
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    s1 = v1;
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (b) s1 = "true";
    else   s1 = "false";
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    cout     << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MTBL(KindOfMessage kind,const char *t1,const char *v1,const char * t2,Bool_t b) {
  // Message containing a text defining a text variable, the text variable, a text
  //defining a boolean variable, the boolean variable.Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLengthDV - 1;
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  s1 = v1;
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (b) s1 = "true";
  else   s1 = "false";
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  fListing << fList.Data() << endl;
}
void TCleanOut::MTI(KindOfMessage kind,const char *t1,const char *v1,const char * t2,Long64_t I) {
  //  Message containing a text defining a text variable, the text variable, a text
  //defining an integer variable, the integer variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable v1
  //  v1      : 1st text variable
  //  t2      : a short text defining the variable I
  //  I       : 2nd variable of type short, int or long64
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MTIL(kind,t1,v1,t2,I);
    lengthdv1 = fLengthDV - 1;
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    s1 = v1;
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fOss->width(fWidth);
    *fOss << I << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    //
    cout << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MTIL(KindOfMessage kind,const char *t1,const char *v1,const char * t2,Long64_t I) {
  //  Message containing a text defining a text variable, the text variable, a text
  //defining an integer variable, the integer variable.Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLengthDV - 1;
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  s1 = v1;
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fOss->width(fWidth);
  *fOss << I << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  //
  fListing << fList.Data() << endl;
}
void TCleanOut::MTR(KindOfMessage kind,const char *t1,const char *v1,const char * t2,Double_t r) {
  //  Message containing a text defining a text variable, the text variable, a text
  //defining a real variable, the real variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable v1
  //  v1      : 1st text variable
  //  t2      : a short text defining the variable r
  //  r       : 2nd variable of float or double
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
   if (fWithListing) MTRL(kind,t1,v1,t2,r);
    lengthdv1 = fLengthDV - 1;
     fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    s1 = v1;
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fOss->width(fWidth);
    fOss->precision(fPrecision);
    *fOss << r << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    //
    cout << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MTRL(KindOfMessage kind,const char *t1,const char *v1,const char * t2,Double_t r) {
  //  Message containing a text defining a text variable, the text variable, a text
  //defining a real variable, the real variable.Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLengthDV - 1;
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  s1 = v1;
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fOss->width(fWidth);
  fOss->precision(fPrecision);
  *fOss << r << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  //
  fListing << fList.Data() << endl;
}
void TCleanOut::MTC(KindOfMessage kind,const char *t1,const char *v1,const char * t2,TComplex z) {
  //  Message containing a text defining a text variable, the text variable, a text
  //defining a complex variable, the complex variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable v1
  //  v1      : 1st text variable
  //  t2      : a short text defining the variable z
  //  z       : 2nd variable of type complex
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MTCL(kind,t1,v1,t2,z);
    lengthdv1 = fLength1C - 1;
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    s1 = v1;
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    *fOss << '(';
    fOss->width(fWidth1C);
    fOss->precision(fPrecision1C);
    *fOss << z.Re() << ',';
    fOss->width(fWidth1C);
    fOss->precision(fPrecision1C);
    *fOss << z.Im() << "i)" << ends;
    Extract(2*fWidth1C+4);
    fLine.Append(fSbuf);
    //
    cout << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MTCL(KindOfMessage kind,const char *t1,const char *v1,const char * t2,TComplex z) {
  //  Message containing a text defining a text variable, the text variable, a text
  //defining a complex variable, the complex variable. Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLength1C - 1;
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  s1 = v1;
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  *fOss << '(';
  fOss->width(fWidth1C);
  fOss->precision(fPrecision1C);
  *fOss << z.Re() << ',';
  fOss->width(fWidth1C);
  fOss->precision(fPrecision1C);
  *fOss << z.Im() << "i)" << ends;
  Extract(2*fWidth1C+4);
  fList.Append(fSbuf);
  //
  fListing << fList.Data() << endl;
}
void TCleanOut::MBB(KindOfMessage kind,const char *t1,Bool_t b1,const char * t2,Bool_t b2) {
  // Message containing a text defining the first boolean variable, the first boolean
  //variable, a text defining the second boolean variable, the second boolean variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable b1
  //  b1      : 1st variable of type boolean
  //  t2      : a short text defining the variable b
  //  b2      : 2nd variable of type boolean
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MBBL(kind,t1,b1,t2,b2);
    lengthdv1 = fLengthDV - 1;
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (b1) s1 = "true";
    else    s1 = "false";
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    //
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (b2) s1 = "true";
    else    s1 = "false";
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    cout     << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MBBL(KindOfMessage kind,const char *t1,Bool_t b1,const char * t2,Bool_t b2) {
  // Message containing a text defining the first boolean variable, the first boolean
  //variable, a text defining the second boolean variable, the second boolean variable.
  //Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLengthDV - 1;
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (b1) s1 = "true";
  else    s1 = "false";
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  //
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (b2) s1 = "true";
  else    s1 = "false";
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  fListing << fList.Data() << endl;
}
void TCleanOut::MBI(KindOfMessage kind,const char *t1,Bool_t b1,const char * t2,Long64_t I) {
  // Message containing a text defining a boolean variable, the boolean variable, a text
  //defining an integer variable, the integer variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable b1
  //  b1      : 1st variable of type boolean
  //  t2      : a short text defining the variable I
  //  I       : 2nd variable of type short, int or long64
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MBIL(kind,t1,b1,t2,I);
    lengthdv1 = fLengthDV - 1;
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (b1) s1 = "true";
    else    s1 = "false";
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    //
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fOss->width(fWidth);
    *fOss << I << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    //
    cout     << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MBIL(KindOfMessage kind,const char *t1,Bool_t b1,const char * t2,Long64_t I) {
  // Message containing a text defining a boolean variable, the boolean variable, a text
  //defining an integer variable, the integer variable. Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLengthDV - 1;
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (b1) s1 = "true";
  else    s1 = "false";
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  //
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fOss->width(fWidth);
  *fOss << I << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  //
  fListing << fList.Data() << endl;
}
void TCleanOut::MBR(KindOfMessage kind,const char *t1,Bool_t b1,const char * t2,Double_t r) {
  // Message containing a text defining a boolean variable, the boolean variable, a text
  //defining an integer variable, the integer variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable b1
  //  b1      : 1st variable of type boolean
  //  t2      : a short text defining the variable r
  //  r       : 2nd variable of type float or double
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MBRL(kind,t1,b1,t2,r);
    lengthdv1 = fLengthDV - 1;
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (b1) s1 = "true";
    else    s1 = "false";
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    //
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fOss->width(fWidth);
    fOss->precision(fPrecision);
    *fOss << r << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    //
    cout     << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MBRL(KindOfMessage kind,const char *t1,Bool_t b1,const char * t2,Double_t r) {
  // Message containing a text defining a boolean variable, the boolean variable, a text
  //defining an integer variable, the integer variable. Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLengthDV - 1;
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (b1) s1 = "true";
  else    s1 = "false";
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  //
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fOss->width(fWidth);
  fOss->precision(fPrecision);
  *fOss << r << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  //
  fListing << fList.Data() << endl;
}
void TCleanOut::MBC(KindOfMessage kind,const char *t1,Bool_t b,const char * t2,TComplex z) {
  //  Message containing a text defining a boolean variable, the boolean variable, a text
  //defining a complex variable, the complex variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable b1
  //  b1      : 1st variable of type boolean
  //  t2      : a short text defining the variable z
  //  z       : 2nd variable of type complex
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MBCL(kind,t1,b,t2,z);
    lengthdv1 = fLength1C - 1;
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (b) s1 = "true";
    else   s1 = "false";
    L  = s1.Length();
    if (L != fWidth) {
      if (L>fWidth) {
        s1.Resize(fWidth-1);
        s1.Append('~');
      }
      else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
    }
    fLine.Append(s1);
    //
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    //
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    *fOss << '(';
    fOss->width(fWidth1C);
    fOss->precision(fPrecision1C);
    *fOss << z.Re() << ',';
    fOss->width(fWidth1C);
    fOss->precision(fPrecision1C);
    *fOss << z.Im() << "i)" << ends;
    Extract(2*fWidth1C+4);
    fLine.Append(fSbuf);
    //
    cout << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MBCL(KindOfMessage kind,const char *t1,Bool_t b,const char * t2,TComplex z) {
  //  Message containing a text defining a boolean variable, the boolean variable, a text
  //defining a complex variable, the complex variable. Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLength1C - 1;
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (b) s1 = "true";
  else   s1 = "false";
  L  = s1.Length();
  if (L != fWidth) {
    if (L>fWidth) {
      s1.Resize(fWidth-1);
      s1.Append('~');
    }
    else for (k=0;k<fWidth-L;k++) s1.Prepend(' ');
  }
  fList.Append(s1);
  //
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  //
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  *fOss << '(';
  fOss->width(fWidth1C);
  fOss->precision(fPrecision1C);
  *fOss << z.Re() << ',';
  fOss->width(fWidth1C);
  fOss->precision(fPrecision1C);
  *fOss << z.Im() << "i)" << ends;
  Extract(2*fWidth1C+4);
  fList.Append(fSbuf);
  //
  fListing << fList.Data() << endl;
}
void TCleanOut::MII(KindOfMessage kind,const char *t1,Long64_t I1,const char * t2,Long64_t I2) {
  // Message containing a text defining the first integer variable, the first integer
  //variable, a text defining the second integer variable and the second integer variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable I1
  //  I1      : 1st variable of type short, int or long64
  //  t2      : a short text defining the variable I2
  //  I2      : 2nd variable of type short, int or long64
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MIIL(kind,t1,I1,t2,I2);
    lengthdv1 = fLengthDV - 1;
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    fOss->width(fWidth);
    *fOss << I1 << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    L = fLine.Length();
//    fLine.Resize(L-1);
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fOss->width(fWidth);
    *fOss  << I2 << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    cout     << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MIIL(KindOfMessage kind,const char *t1,Long64_t I1,const char * t2,Long64_t I2) {
  // Message containing a text defining the first integer variable, the first integer
  //variable, a text defining the second integer variable and the second integer variable.
  //Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLengthDV - 1;
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  fOss->width(fWidth);
  *fOss << I1 << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  L = fList.Length();
//  fList.Resize(L-1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fOss->width(fWidth);
  *fOss  << I2 << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  fListing << fList.Data() << endl;
}
void TCleanOut::MIR(KindOfMessage kind,const char *t1,Long64_t i,const char * t2,Double_t d) {
  // Message containing a text defining an integer variable, the integer variable, a
  //text defining a real variable and the real variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable i
  //  i       : 1st variable of type short, int or long64
  //  t2      : a short text defining the variable d
  //  d       : 2nd variable of type float or double
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MIRL(kind,t1,i,t2,d);
    lengthdv1 = fLengthDV - 1;
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    fOss->width(fWidth);
    *fOss << i << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    L = fLine.Length();
//    fLine.Resize(L-1);
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fOss->width(fWidth);
    fOss->precision(fPrecision);
    *fOss  << d << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    cout     << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MIRL(KindOfMessage kind,const char *t1,Long64_t i,const char * t2,Double_t d) {
  // Message containing a text defining an integer variable, the integer variable, a
  //text defining a real variable and the real variable. Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLengthDV - 1;
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  fOss->width(fWidth);
  *fOss << i << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  L = fList.Length();
//  fList.Resize(L-1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fOss->width(fWidth);
  fOss->precision(fPrecision);
  *fOss  << d << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  fListing << fList.Data() << endl;
}
void TCleanOut::MIC(KindOfMessage kind,const char *t1,Long64_t i,const char * t2,TComplex z) {
  // message containing a text defining an integer variable, the integer variable, a text
  //defining a complex variable and the complex variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable i
  //  i       : 1st variable of type short, int or long64
  //  t2      : a short text defining the variable z
  //  z       : 2nd variable of type complex
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MICL(kind,t1,i,t2,z);
    lengthdv1 = fLength1C - 1;
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    fOss->width(fWidth);
    *fOss << i << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    L = fLine.Length();
//    fLine.Resize(L-1);
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    *fOss << '(';
    fOss->width(fWidth1C);
    fOss->precision(fPrecision1C);
    *fOss << z.Re() << ',';
    fOss->width(fWidth1C);
    fOss->precision(fPrecision1C);
    *fOss << z.Im() << "i)" << ends;
    Extract(2*fWidth1C+4);
    fLine.Append(fSbuf);
    cout << fLine.Data() << endl;
    //    cout << "Line length: " << fLine.Length() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MICL(KindOfMessage kind,const char *t1,Long64_t i,const char * t2,TComplex z) {
  // message containing a text defining an integer variable, the integer variable, a text
  //defining a complex variable and the complex variable. Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLength1C - 1;
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  fOss->width(fWidth);
  *fOss << i << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  L = fList.Length();
//  fList.Resize(L-1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  *fOss << '(';
  fOss->width(fWidth1C);
  fOss->precision(fPrecision1C);
  *fOss << z.Re() << ',';
  fOss->width(fWidth1C);
  fOss->precision(fPrecision1C);
  *fOss << z.Im() << "i)" << ends;
  Extract(2*fWidth1C+4);
  fList.Append(fSbuf);
  fListing << fList.Data() << endl;
}
void TCleanOut::MRR(KindOfMessage kind,const char *t1,Double_t d1,const char * t2,Double_t d2) {
  // Message containing a text defining the first real variable, the first real variable,
  //a text defining the second real variable, the second real variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable d1
  //  d1      : 1st variable of type float or double
  //  t2      : a short text defining the variable d2
  //  d2      : 2nd variable of type float or double
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MRRL(kind,t1,d1,t2,d2);
    lengthdv1 = fLengthDV - 1;
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    fOss->width(fWidth);
    fOss->precision(fPrecision);
    *fOss << d1 << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    L = fLine.Length();
//    fLine.Resize(L-1);
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fOss->width(fWidth);
    fOss->precision(fPrecision);
    *fOss  << d2 << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    cout     << fLine.Data() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MRRL(KindOfMessage kind,const char *t1,Double_t d1,const char * t2,Double_t d2) {
  // Message containing a text defining the first real variable, the first real variable,
  //a text defining the second real variable, the second real variable. Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLengthDV - 1;
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  fOss->width(fWidth);
  fOss->precision(fPrecision);
  *fOss << d1 << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  L = fList.Length();
//  fList.Resize(L-1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fOss->width(fWidth);
  fOss->precision(fPrecision);
  *fOss  << d2 << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  fListing << fList.Data() << endl;
}
void TCleanOut::MRC(KindOfMessage kind,const char *t1,Double_t d,const char * t2,TComplex z) {
  // Message containing a text defining a real variable, the real variable, a text defining
  //a complex variable and the complex variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable d
  //  d       : 1st variable of type float or double
  //  t2      : a short text defining the variable z
  //  z       : 2nd variable of type complex
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MRCL(kind,t1,d,t2,z);
    lengthdv1 = fLength1C - 1;
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    fOss->width(fWidth);
    fOss->precision(fPrecision);
    *fOss << d << ends;
    Extract(fWidth);
    fLine.Append(fSbuf);
    L = fLine.Length();
//    fLine.Resize(L-1);
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    *fOss << '(';
    fOss->width(fWidth1C);
    fOss->precision(fPrecision1C);
    *fOss << z.Re() << ',';
    fOss->width(fWidth1C);
    fOss->precision(fPrecision1C);
    *fOss << z.Im() << "i)" << ends;
    Extract(2*fWidth1C+4);
    fLine.Append(fSbuf);
    cout << fLine.Data() << endl;
    //    cout << "Line length: " << fLine.Length() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MRCL(KindOfMessage kind,const char *t1,Double_t d,const char * t2,TComplex z) {
  // Message containing a text defining a real variable, the real variable, a text defining
  //a complex variable and the complex variable. Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLength1C - 1;
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  fOss->width(fWidth);
  fOss->precision(fPrecision);
  *fOss << d << ends;
  Extract(fWidth);
  fList.Append(fSbuf);
  L = fList.Length();
//  fList.Resize(L-1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  *fOss << '(';
  fOss->width(fWidth1C);
  fOss->precision(fPrecision1C);
  *fOss << z.Re() << ',';
  fOss->width(fWidth1C);
  fOss->precision(fPrecision1C);
  *fOss << z.Im() << "i)" << ends;
  Extract(2*fWidth1C+4);
  fList.Append(fSbuf);
  fListing << fList.Data() << endl;
}
void TCleanOut::MCC(KindOfMessage kind,const char *t1,TComplex z1,const char * t2,TComplex z2) {
  // message containing a text defining the first complex variable, the first complex
  //variable, a text defining the second complex variable, the second complex variable.
  //
  //  Arguments:
  //
  //  kind    : {always,debug,info,warning,error,killthis,fatal}
  //  t1      : a short text defining the variable z1
  //  z1      : 1st variable of type complex
  //  t2      : a short text defining the variable z2
  //  z2      : 2nd variable of type complex
  //
  const char *sep = "  |  ";
  Bool_t ok;
  ok = SetStamp(kind);
  if (ok) {
    Short_t k;
    UShort_t lengthdv1;
    Int_t L;
    TString s1;
    if (!fIsAdjusted) Adjust();
    if (fWithListing) MCCL(kind,t1,z1,t2,z2);
    lengthdv1 = fLength2C - 1;
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    fLine.Append(' ');
    s1 = t1;
    L = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    *fOss << '(';
    fOss->width(fWidth2C);
    fOss->precision(fPrecision2C);
    *fOss << z1.Re() << ',';
    fOss->width(fWidth2C);
    fOss->precision(fPrecision2C);
    *fOss << z1.Im() << "i)" << ends;
    Extract(2*fWidth2C+4);
    fLine.Append(fSbuf);
    L = fLine.Length();
//    fLine.Resize(L-1);
    fLine.Append(sep);
    s1 = t2;
    L  = s1.Length();
    if (L != lengthdv1) {
      if (L>lengthdv1) {
        s1.Resize(lengthdv1 - 1);
        s1.Append('~');
      }
      else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
    }
    s1.Append(':');
    fLine.Append(s1);
    if (fOss)  { delete fOss; fOss = 0; }
    fOss = new ostringstream();
    *fOss << '(';
    fOss->width(fWidth2C);
    fOss->precision(fPrecision2C);
    *fOss << z2.Re() << ',';
    fOss->width(fWidth2C);
    fOss->precision(fPrecision2C);
    *fOss << z2.Im() << "i)" << ends;
    Extract(2*fWidth2C+4);
    fLine.Append(fSbuf);
    cout << fLine.Data() << endl;
    //    cout << "Line length: " << fLine.Length() << endl;
    if (kind==fatal) KillProg();
  }
}
void TCleanOut::MCCL(KindOfMessage kind,const char *t1,TComplex z1,const char * t2,TComplex z2) {
  // message containing a text defining the first complex variable, the first complex
  //variable, a text defining the second complex variable, the second complex variable.
  //Sent to listing.
  const char *sep = "  |  ";
  Short_t k;
  UShort_t lengthdv1;
  Int_t L;
  TString s1;
  lengthdv1 = fLength2C - 1;
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  fList.Append('\t');
  s1 = t1;
  L = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  *fOss << '(';
  fOss->width(fWidth2C);
  fOss->precision(fPrecision2C);
  *fOss << z1.Re() << ',';
  fOss->width(fWidth2C);
  fOss->precision(fPrecision2C);
  *fOss << z1.Im() << "i)" << ends;
  Extract(2*fWidth2C+4);
  fList.Append(fSbuf);
  L = fList.Length();
//  fList.Resize(L-1);
  fList.Append('\t');
  fList.Append(sep);
  fList.Append('\t');
  s1 = t2;
  L  = s1.Length();
  if (L != lengthdv1) {
    if (L>lengthdv1) {
      s1.Resize(lengthdv1 - 1);
      s1.Append('~');
    }
    else for (k=0;k<lengthdv1-L;k++) s1.Prepend(' ');
  }
  s1.Append(':');
  fList.Append(s1);
  fList.Append('\t');
  if (fOss)  { delete fOss; fOss = 0; }
  fOss = new ostringstream();
  *fOss << '(';
  fOss->width(fWidth2C);
  fOss->precision(fPrecision2C);
  *fOss << z2.Re() << ',';
  fOss->width(fWidth2C);
  fOss->precision(fPrecision2C);
  *fOss << z2.Im() << "i)" << ends;
  Extract(2*fWidth2C+4);
  fList.Append(fSbuf);
  fListing << fList.Data() << endl;
}
void TCleanOut::Reset() {
  // Clears all counters, so that printing restarts.
  fNbDebug     = 0;
  fNbInfo      = 0;
  fNbWarning   = 0;
  fNbError     = 0;
  fNbKill      = 0;
}
void TCleanOut::SetListing(const char *namelis, const char *suffix) {
  // Ask for a listing
  fWithListing = kTRUE;
  fNameListing = namelis;
  fNameListing.Append('.');
  fNameListing.Append(suffix);
  fListing.open(fNameListing.Data(), ios::out);
}
Bool_t TCleanOut::SetMaxLength(Short_t length) {
  // Set fMaxLength to be length. You are advised to set your window size, [the 
  //command prompt window or MSDOS window under Windows XP] to fMaxLength + 1,
  //otherwise your output will look ugly. You cannot choose a length smaller
  //than 79, nor a length bigger than 299.
  const UShort_t low  = 79;
  const UShort_t high = 299;
  Bool_t ok = kTRUE;
  if (length<low) {
    ok = kFALSE;
    length = low;
  }
  if (length>high) {
    ok = kFALSE;
    length = high;
  }
  fMaxLength  = length;
  fIsAdjusted = kFALSE;
  return ok;
}
Bool_t TCleanOut::SetStamp(KindOfMessage kind) {
  // SetStamp first decides if the message has to be printed, and if yes issues the
  //first 2 characters of the message.
  Bool_t ok = kTRUE;
  switch (kind) {
    case always:
      break;
    case debug:
      fNbDebug++;
      if (fNbDebug>fMaxDebug) ok = kFALSE;
      break;
    case info:
      fNbInfo++;
      if (fNbInfo>fMaxInfo) ok = kFALSE;
      break;
    case warning:
      fNbWarning++;
      if (fNbWarning>fMaxWarning) ok = kFALSE;
      break;
    case error:
      fNbError++;
      if (fNbError>fMaxError) ok = kFALSE;
      break;
    case killthis:
      fNbKill++;
      if (fNbKill>fMaxKill) {
        ok = kFALSE;
        TString s = "Too much messages of type killthis. We stop program";
        cout     << s.Data() << endl;
        fListing << s.Data() << endl;
        KillProg();
      }
      break;
    case fatal:
      break;
  }
  if (ok) {
    switch (kind) {
      case always:
        fLine = "A ";
        break;
      case debug:
        fLine = "D ";
        break;
      case info:
        fLine = "I ";
        break;
      case warning:
        fLine = "W ";
        break;
      case error:
        fLine = "E ";
        break;
      case killthis:
        fLine = "K ";
        break;
      case fatal:
        fLine = "F ";
        break;
    }//end switch (kind)
    if (fWithListing) {
      switch (kind) {
        case always:
          fList = "A \t";
          break;
        case debug:
          fList = "D \t";
          break;
        case info:
          fList = "I \t";
          break;
        case warning:
          fList = "W \t";
          break;
        case error:
          fList = "E \t";
          break;
        case killthis:
          fList = "K \t";
          break;
        case fatal:
          fList = "F \t";
          break;
      }//end switch (kind)
    }
  }//end if (ok)
  return ok;
}
Bool_t TCleanOut::SetWidth(Short_t width) {
  // Set width used for printing variables. Minimum 10, Maximum 22. fPrecision
  //will than be set to width - 8;
  const UShort_t low    = 10;
  const UShort_t high   = 22;
  const UShort_t diffwp = 8;
  Bool_t ok = kTRUE;
  if (width<low) {
    ok = kFALSE;
    width = low;
  }
  if (width>high) {
    ok = kFALSE;
    width = high;
  }
  fWidth = width;
  fPrecision = width - diffwp;
  fIsAdjusted = kFALSE;
  return ok;
}
