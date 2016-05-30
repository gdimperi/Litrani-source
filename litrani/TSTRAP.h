// @(#)root/html:$Id: TSTRAP.h 2008-06-11
// Author: F.X. Gentit <http://gentit.home.cern.ch/gentit/>

/*************************************************************************
* Copyright (C) Those valid for CNRS software.                          *
*************************************************************************/
#ifndef LITR_TSTRAP
#define LITR_TSTRAP
///////////////////////////////////////////////////////////////////////////
//                                                                       //
// TSTRAP  Inherits from TTRAP and add all informations asked by Litrani //
//                                                                       //
///////////////////////////////////////////////////////////////////////////
#include "TTRAP.h"
#include "TObjArray.h"

class TSupplShape;
class T3Vector;

class TSTRAP : public TTRAP {

protected:

  Double_t  fDdz;     //half length in z but in double precision.
  Double_t  fDtheta;  //half length in x but in double precision. DEGREE
  Double_t  fDphi;    //half length in y but in double precision. DEGREE
  Double_t  fDh1;     //half length in y at low z
  Double_t  fDbl1;    //half length in x at low z and y low edge
  Double_t  fDtl1;    //half length in x at low z and y high edge
  Double_t  fDalpha1; //angle w.r.t. the y axis.  DEGREE
  Double_t  fDh2;     //half length in y at high z
  Double_t  fDbl2;    //half length in x at high z and y low edge
  Double_t  fDtl2;    //half length in x at high z and y high edge
  Double_t  fDalpha2; //angle w.r.t. the y axis.  DEGREE
  T3Vector *fV0;      //point 0 of the TSTRAP
  T3Vector *fV1;      //point 1 of the TSTRAP
  T3Vector *fV2;      //point 2 of the TSTRAP
  T3Vector *fV3;      //point 3 of the TSTRAP
  T3Vector *fV4;      //point 4 of the TSTRAP
  T3Vector *fV5;      //point 5 of the TSTRAP
  T3Vector *fV6;      //point 6 of the TSTRAP
  T3Vector *fV7;      //point 7 of the TSTRAP

  void      DelPoints();
  void      InitP();
  Double_t  MakePlanar();
  void      ToParam(Double_t&, Double_t&);
  void      ToPoints();

public:

  TSupplShape *fSuppl;  //! suppl. information added to each TSSHAPE

  TSTRAP() { InitP(); }
  TSTRAP(const char*,const char*,const char*,const char*,Double_t,Double_t,Double_t,
    Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,
    Double_t,Bool_t = kFALSE);
  TSTRAP(const char*,const char*,const char*,const char*,Double_t,Double_t,Double_t,
    Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,
    Double_t,Double_t,Double_t,Double_t&,Double_t&,Bool_t = kFALSE);
  TSTRAP(const char*,const char*,const char*,const char*,Double_t,Double_t,Double_t,
    Double_t,Double_t,Double_t,Double_t,Bool_t = kFALSE);
  TSTRAP(const char*,const char*,const char*,const char*,Short_t,Double_t=0.0,Bool_t=kFALSE);
  virtual  ~TSTRAP();
  Double_t  Getalpha1() const { return fDalpha1; }
  Double_t  Getalpha2() const { return fDalpha2; }
  Double_t  Getbl1() const    { return fDbl1;    }
  Double_t  Getbl2() const    { return fDbl2;    }
  void      GetCMSdim(Short_t,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
    Double_t&,Double_t=0.0) const;
  Double_t  Getdz() const     { return fDdz;     }
  T3Vector* GetfV(Int_t) const;
  Double_t  Geth1() const     { return fDh1;     }
  Double_t  Geth2() const     { return fDh2;     }
  Double_t  Getphi() const    { return fDphi;    }
  void      GetPoints(Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
    Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
    Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,
    Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&) const;
  Double_t  Gettl1() const    { return fDtl1;    }
  Double_t  Gettl2() const    { return fDtl2;    }
  Double_t  Gettheta() const  { return fDtheta;  }
  void      Print() const;
  void      PrintPts() const;
  void      SetOpticalAxis(Double_t thetax, Double_t phix, Double_t thetay, Double_t phiy) const;
  ClassDef(TSTRAP,0) //TTRAP with more information
};
#endif
