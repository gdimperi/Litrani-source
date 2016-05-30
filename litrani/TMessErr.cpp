// @(#)root/html:$Id: TMessErr.cpp 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TString.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TSComplex.h"
#include "T3Vector.h"
#include "TMessErr.h"

TMessErr* gMes = 0;

ClassImp(TMessErr)

//______________________________________________________________________________
//
// Class handling warnings or error messages going to output AND listing.
//Notice that there are 5 level of severity for messages :
//
// - Info_M       labelled with 'I'    These messages are harmless and are there
//              only to inform you about some details.
// - Warning_M    labelled with 'W'    These are also harmless, but you should
//              take them into account because they may point to errors in
//              your macro commands.
// - Error_M      labelled with 'E'    They point to something which is serious,
//              but the program will continue aniway. If this "serious thing"
//              is an error in your macro command, the program may crash. If
//              it was a test on some result which is considered as non
//              precise enough, the program may continue, but produce bad
//              results for the entity (for example the photon) responsible
//              for this error message.
// - kill_this  labelled with 'K'    A serious problem, such as a big lack of
//              precision for a calculation, has been encountered. On result,
//              the responsible photon will be killed and the program will
//              continue with an other photon. The number of photon abnormaly
//              killed will be registered in the statistics on the run (See
//              class TResults) and can be subtracted from the number of
//              generated photons. Hopefully, this number of abnormal photons
//              will be 0.
// - kill_prog  labelled with 'F'    The encountered problem is so serious
//              that the program has to be stopped : it would die aniway.
//
// All messages issued by the class TMessErr go to the output window AND to
//the listing of Litrani, which is called by default litrani.lis, if the
//user has not changed its name.
// All messages coming from the internal code of Litrani use the methods M().
//Using the methods M(), each message is accompanied with:
//   - level of severity of message
//   - name of the class  having issued the message (stored into the title
//              of the class TMessErr)
//   - name of the method having issued the message (stored into the name
//              of the class TMessErr)
//   - a message number (a single method can issue many messages)
// Through the global pointer gMes, the user can also issue messages from
//within his CINT code. These messages will appear in the output AND and
//in the listing litrani.lis. It will only work after the call to the
//constructor of TLitGlob and before the call to gLit->CloseFiles().
// The user is advised to use the methods W() instead of the method M()
//if he does not want his message to be accompanied with a name of a
//class and a name of a method. Example:
//  gMes->W("A user comment");
// If he wants his message to be accompanied with severity level, name of a
//class, name of a method and number of the message, then he can use the
//methods M(), after having defined name and title. For instance:
//  gMes->SetName("UserMethodName");
//  gMes->SetTitle("UserClassName");
//  gMes->M(Warning_M,1,"A user comment");
//
TMessErr::TMessErr(const char *name,const char *title,const char *namelis):TNamed(name,title) {
  //constructor, with the following arguments :
  //
  // - name    : name of the calling routine
  // - title   : name of the calling class
  // - namelis : name of the listing of litrani, without suffix. Default : litrani
  //
  TString s;
  Init();
  s = namelis;
  s.Append(".lis");
  fListing = new ofstream(s.Data(), ios::out);
}
TMessErr::~TMessErr() {
  fListing->close();
  delete fListing;
}
void TMessErr::Init() {
  fTestCalc = kFALSE;
  fListing  = 0;
}
void TMessErr::KillProg() const {
  //stop program after a fatal error
  char c;
  TApplication *theApp;
  cout << "A fatal error has been encountered, program will stop" << endl;
  *fListing << "A fatal error has been encountered, program will stop" << endl;
  fListing->close();
  cout << "Please read carefully the last error message marqued with F" << endl;
  cin  >> c;
  theApp = gROOT->GetApplication();
  theApp->Terminate(-1);
}
void TMessErr::PrintCplx(const TSComplex &z) const {
  //Integer
  cout.width(16);
  cout.precision(8);
  fListing->width(16);
  fListing->precision(8);
  cout     << z;
  *fListing << z;
}
void TMessErr::PrintCR() const {
  //Print carriage return
  cout     << endl;
  *fListing << endl;
}
void TMessErr::PrintHeader(KindOfMes K,Int_t n) {
  //Prints the beginning of the message
  SetfC(K);
  TString s1,s2;
  n %= 10000;
  switch (K) {
  case Error_M:
    cout     << "!!! ERROR  !!!" << endl;
    *fListing << "!!! ERROR  !!!" << endl;
    break;
  case kill_this:
    cout     << "!!! KILLED !!!" << endl;
    *fListing << "!!! KILLED !!!" << endl;
    break;
  default:
    break;
  }
  s1 = GetTitle();
  s2 = GetName();
  s1.Resize(16);
  s2.Resize(16);
  if (n<10) {
    cout     << fC << "  " << n << ' ' << s1.Data() << "::" << s2.Data();
    *fListing << fC << "  " << n << ' ' << s1.Data() << "::" << s2.Data();
  }
  else {
    cout     << fC << ' ' << n << ' ' << s1.Data() << "::" << s2.Data();
    *fListing << fC << ' ' << n << ' ' << s1.Data() << "::" << s2.Data();
  }
}
void TMessErr::PrintInt(Int_t i) const {
  //Integer
  cout.width(16);
  fListing->width(16);
  cout     << i;
  *fListing << i;
}
void TMessErr::PrintReal(Double_t d) const {
  //Integer
  cout.width(20);
  cout.precision(12);
  fListing->width(20);
  fListing->precision(12);
  cout     << d;
  *fListing << d;
}
void TMessErr::PrintSpace() const {
  //Print space
  cout     << "    ";
  *fListing << "    ";
}
void TMessErr::PrintStr(const TString &s) const {
  TString s1(s);
  s1.Resize(16);
  cout     << s1.Data();
  *fListing << s1.Data();
}
void TMessErr::PrintText(const char *t) const {
  //Prints the text of the message.
  cout     << "     " << t << endl;
  *fListing << "     " << t << endl;
}
void TMessErr::PrintVal() const {
  //Only if there is a value to print
  cout     << "    value(s) : ";
  *fListing << "    value(s) : ";
}
void TMessErr::PrintVec(const T3Vector &v) const {
  //Integer
  cout.width(16);
  cout.precision(8);
  fListing->width(16);
  fListing->precision(8);
  cout     << v;
  *fListing << v;
}
void TMessErr::SetfC(KindOfMes K) {
  //Defines header according to type
  switch (K) {
  case Info_M:
    fC = 'I';
    break;
  case Warning_M:
    fC = 'W';
    break;
  case Error_M:
    fC = 'E';
    break;
  case kill_this:
    fC = 'K';
    break;
  case kill_prog:
    fC = 'F';
    break;
  }
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t) {
  //Prints only a text message
  PrintHeader(K,n);
  PrintText(t);
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t) const {
  //Prints only a text message
  PrintText(t);
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t,Int_t i) {
  //Prints a text message + an integer
  PrintHeader(K,n);
  PrintText(t);
  PrintVal();
  PrintInt(i);
  PrintCR();
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t,Int_t i) const{
  //Prints a text message + an integer
  PrintText(t);
  PrintVal();
  PrintInt(i);
  PrintCR();
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t,Int_t i,Int_t j) {
  //Prints a text message + 2 integers
  PrintHeader(K,n);
  PrintText(t);
  PrintVal();
  PrintInt(i);
  PrintSpace();
  PrintInt(j);
  PrintCR();
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t,Int_t i,Int_t j) const {
  //Prints a text message + 2 integers
  PrintText(t);
  PrintVal();
  PrintInt(i);
  PrintSpace();
  PrintInt(j);
  PrintCR();
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t,Int_t i,Int_t j,Int_t k) {
  //Prints a text message + 3 integers
  PrintHeader(K,n);
  PrintText(t);
  PrintVal();
  PrintInt(i);
  PrintSpace();
  PrintInt(j);
  PrintSpace();
  PrintInt(k);
  PrintCR();
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t,Int_t i,Int_t j,Int_t k) const {
  //Prints a text message + 3 integers
  PrintText(t);
  PrintVal();
  PrintInt(i);
  PrintSpace();
  PrintInt(j);
  PrintSpace();
  PrintInt(k);
  PrintCR();
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t,Int_t i,Double_t d) {
  //Prints a text message + an integer + a double
  PrintHeader(K,n);
  PrintText(t);
  PrintVal();
  PrintInt(i);
  PrintSpace();
  PrintReal(d);
  PrintCR();
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t,Int_t i,Double_t d) const {
  //Prints a text message + an integer + a double
  PrintText(t);
  PrintVal();
  PrintInt(i);
  PrintSpace();
  PrintReal(d);
  PrintCR();
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t,Double_t d) {
  //Prints a text message + a double
  PrintHeader(K,n);
  PrintText(t);
  PrintVal();
  PrintReal(d);
  PrintCR();
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t,Double_t d) const {
  //Prints a text message + a double
  PrintText(t);
  PrintVal();
  PrintReal(d);
  PrintCR();
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t,Double_t d1,Double_t d2) {
  //Prints a text message + 2 doubles
  PrintHeader(K,n);
  PrintText(t);
  PrintVal();
  PrintReal(d1);
  PrintReal(d2);
  PrintCR();
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t,Double_t d1,Double_t d2) const {
  //Prints a text message + 2 doubles
  PrintText(t);
  PrintVal();
  PrintReal(d1);
  PrintReal(d2);
  PrintCR();
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t,const TSComplex &z) {
  //Prints a text message + a complex
  PrintHeader(K,n);
  PrintText(t);
  PrintVal();
  PrintCplx(z);
  PrintCR();
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t,const TSComplex &z) const {
  //Prints a text message + a complex
  PrintText(t);
  PrintVal();
  PrintCplx(z);
  PrintCR();
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t,const T3Vector &v) {
  //Prints a text message + a complex
  PrintHeader(K,n);
  PrintText(t);
  PrintVal();
  PrintVec(v);
  PrintCR();
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t,const T3Vector &v) const {
  //Prints a text message + a complex
  PrintText(t);
  PrintVal();
  PrintVec(v);
  PrintCR();
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t,const TString &s) {
  //Prints a text message + a string
  PrintHeader(K,n);
  PrintText(t);
  PrintVal();
  PrintStr(s);
  PrintCR();
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t,const TString &s) const {
  //Prints a text message + a string
  PrintText(t);
  PrintVal();
  PrintStr(s);
  PrintCR();
}
void TMessErr::M(KindOfMes K,Int_t n,const char *t,const TString &s1,const TString &s2) {
  //Prints a text message + 2 strings
  PrintHeader(K,n);
  PrintText(t);
  PrintVal();
  PrintStr(s1);
  PrintSpace();
  PrintStr(s2);
  PrintCR();
  if (K==kill_prog) KillProg();
}
void TMessErr::W(const char *t,const TString &s1,const TString &s2) const {
  //Prints a text message + 2 strings
  PrintText(t);
  PrintVal();
  PrintStr(s1);
  PrintSpace();
  PrintStr(s2);
  PrintCR();
}
