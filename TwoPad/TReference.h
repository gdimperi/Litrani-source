// @(#)root/html:$Id: TReference.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
 * Copyright (C) Those valid for CNRS software.                          *
 *************************************************************************/
#ifndef LIT_TReference
#define LIT_TReference
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TReference  Class to specify references for a program                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include "TString.h"
#include "TObjArray.h"
#include "TAuthor.h"

class TReference : public TNamed {

protected:

  TString   fVersion;   //Version of program. For instance : "5_15_06"
  TString   fWebSite;   //Web address describing the program
  TString   fLanguage;  //Language in which program is written. Default: C++
  TString   fFramework; //Framework on which program is built. Default: ROOT
  TObjArray fAuthor;    //Collection of TAuthor

  void      Init();

public:


  TReference()                            { Init();                      }
  TReference(const char*,const char*);
  TReference(const char*,const char*,const char*,const char* = "");
  virtual    ~TReference();
  void        AddAuthor(TAuthor *author)  { fAuthor.AddLast(author);     }
  TObjArray  &GetCollection()             { return fAuthor;              }
  const char *GetFramework() const        { return fFramework.Data();    }
  const char *GetLanguage() const         { return fLanguage.Data();     }
  Int_t       GetNbOfAuthor() const       { return fAuthor.GetEntries(); }
  const char *GetVersion() const          { return fVersion.Data();      }
  const char *GetWebSite() const          { return fWebSite.Data();      }
  TAuthor    *GetAuthor(Int_t k) const    { return (TAuthor*)fAuthor[k]; }
  void        SetFramework(const char *f) { fFramework = f;              }
  void        SetLanguage(const char *l)  { fLanguage = l;               }
  void        SetVersion(const char *v)   { fVersion = v;                }
  void        SetWebSite(const char *w)   { fWebSite = w;                }
  ClassDef(TReference,1) //Class to specify references for a program
};
#endif
