// @(#)root/html:$Id: TAuthor.cxx 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#include "Riostream.h"
#include "TAuthor.h"

ClassImp(TAuthor)
//______________________________________________________________________________
//
//  Class giving info on one author of a program. This info concerns:
//
//  name       : Name of the author
//  title      : Firstname of the author
//  fAddress   : Address of the laboratory or private address of the author
//  fMail      : Mail address of author
//  fWebSite   : Address of web site of author
//  fCountry   : Country of author
//  fPhone     : Phone number of author
//
TAuthor::TAuthor(const char *name, const char *title):TNamed(name,title) {
  // Constructor
  //  name      : Name of the author
  //  title     : Firstname of the author
  Int_t k,L;
  Ssiz_t k1,k2;
  TString s;
  Init();
  s = title;
  fInitials.Append(s[0]);
  fInitials.Append('.');
  L = s.Length();
  k1 = s.Index("-",1);
  k2 = s.Index(" ",1);
  k  = TMath::Max(k1,k2);
  if ((k>0) && (k<L-1)) {
    fInitials.Append(s[k+1]);
    fInitials.Append(". ");
  }
}
TAuthor::TAuthor(const char *name, const char *title,const char *address,
  const char *mail,const char *web,const char *country,const char *phone):TNamed(name,title) {
  // Constructor
  //  name      : Name of the author
  //  title     : Firstname of the author
  //  address   : Address of the laboratory or the private address of the author
  //  mail      : Mail address of author
  //  web       : Address of web site of author
  //  country   : Country of author
  //  phone     : Phone number of author
  Int_t k,L;
  Ssiz_t k1,k2;
  TString s = title;
  fAddress  = address;
  fMail     = mail;
  fWebSite  = web;
  fCountry  = country;
  fPhone    = phone;
  fInitials = "";
  fInitials.Append(s[0]);
  fInitials.Append('.');
  L = s.Length();
  k1 = s.Index("-",1);
  k2 = s.Index(" ",1);
  k  = TMath::Max(k1,k2);
  if ((k>0) && (k<L-1)) {
    fInitials.Append(s[k+1]);
    fInitials.Append('.');
  }
}
TAuthor::~TAuthor() {
  // Destructor
}
void TAuthor::Init() {
  // TString to ""
  fInitials = "";
  fAddress  = "";
  fMail     = "";
  fWebSite  = "";
  fCountry  = "";
  fPhone    = "";
}
void TAuthor::Print() const {
  // Prints everything
  cout << "Name     : " << GetName() << endl;
  cout << "Address  : " << GetTitle() << endl;
  if (fMail.Length() > 0)    cout << "Mail     : " << fMail.Data() << endl;
  if (fWebSite.Length() > 0) cout << "Web site : " << fWebSite.Data() << endl;
  if (fCountry.Length() > 0) cout << "Country  : " << fCountry.Data() << endl;
  if (fPhone.Length() > 0)   cout << "Phone    : " << fPhone.Data() << endl;
}
