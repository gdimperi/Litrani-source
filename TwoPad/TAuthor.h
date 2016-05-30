// @(#)root/html:$Id: TAuthor.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TAuthor
#define LIT_TAuthor
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TAuthor    Class giving info on author of program                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TString.h"

class TAuthor : public TNamed {

protected:

  TString fInitials; //Initials of Firstname
  TString fAddress;  //Address of the laboratory or private address of the author
  TString fMail;     //Mail address of author
  TString fWebSite;  //Address of web site of author
  TString fCountry;  //Country of author
  TString fPhone;    //Phone number of author


  void      Init();

public:

  TAuthor()  { ; }
  TAuthor(const char*,const char*);
  TAuthor(const char*,const char*,const char*,const char*,const char* = "",const char* = "",const char* = "");
  virtual    ~TAuthor();
  void        AddAddress(const char *addr)    { fAddress = addr;         }
  void        AddCountry(const char *country) { fCountry = country;      }
  void        AddMail(const char *mail)       { fMail = mail;            }
  void        AddPhone(const char *phone )    { fPhone = phone;          }
  void        AddWebSite(const char *web)     { fWebSite = web;          }
  const char *GetAddress() const              { return fAddress;         }
  const char *GetCountry() const              { return fCountry.Data();  }
  const char *GetInitials() const             { return fInitials.Data(); }
  const char *GetMail() const                 { return fMail.Data();     }
  const char *GetPhone() const                { return fPhone.Data();    }
  const char *GetWebSite() const              { return fWebSite.Data();  }
  void        Print() const;
  ClassDef(TAuthor,1) //Class giving info on one author of program
};
#endif
