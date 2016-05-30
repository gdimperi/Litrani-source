// @(#)root/html:$Id: TReference.cxx 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TReference.h"

ClassImp(TReference)
//______________________________________________________________________________
//
//  Class to specify references for a program, concerning
//   (1) - name of program
//   (2) - short description of program
//   (3) - version number of program
//   (4) - web site describing program
//   (5) - language in which program is written [ default C++ ]
//   (6) - framework on which program is based  [ default ROOT ]
//   (7) - list of authors of program. See class TAuthor
//
TReference::TReference(const char *name, const char *title):TNamed(name,title) {
  //  Constructor
  //
  //  name   : is the name of the program to be referenced
  //  title  : is a short description of the program to be referenced
  //
  Init();
}
TReference::TReference(const char *name, const char *title,const char *version,
  const char *web):TNamed(name,title) {
  //  Constructor
  //
  //  name    : is the name of the program to be referenced
  //  title   : is a short description of the program to be referenced
  //  version : version number of program
  //  web     : web site describing program
  //
  //  Use the methods SetLanguage() and SetFramework() to add the missing
  // info in case default is wrong. Define each author in turn, using class
  // TAuthor, and use method AddAuthor()
  //
  Init();
  fVersion = version;
  fWebSite = web;
}
TReference::~TReference() {
  // Destructor
}
void TReference::Init() {
  // Initialization of TString
  TString s;
  fVersion     = "";
  fWebSite     = "";
  fLanguage    = "C++";
  fFramework   = "ROOT";
}
