TSplineFit* AbsLengthTensor_PbWO4_001(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
//
// Arguments:
//
//  todraw       : true if the fit is to be drawn
//  infile       : true if the fit is to be put in file SplineFitDB.rdb
//  firstinfile  : if true, [BE CAREFULE: RISK OF LOOSING FILE SplineFitDB.rdb ]
//                 delete all fits present in file SplineFitDB.rdb and place
//                 this fit as the first fit in a new file SplineFitDB.rdb
//                 default false !!!
// All defaults for arguments correspond to the case where one calls this CINT
//macro from within a Litrani CINT macro to define a needed fit, instead of trying
//to find it in the database file SplineFitDB.rdb using TSplineFit::FindFit()
//
//   AbsLengthTensor_PbWO4_001
//
//   Description :
//
//     Part of the absorption length of PbWO4 ( crystal 1453 of CMS ) which is
//   associated with element epsb of dielectric tensor, as a function of
//   wavelength [nm]. It is assumed here that not only the dielectric constant
//   is anisotropic (is a tensor) but also that the absorption length is
//   anisotropic. See the discussion of this case in the class description
//   of TOpticMaterial.
//
//   Old f_abslpbwo4b
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 70;
  const Int_t m = 5;
  Int_t i;
  TSplineFit *AbsLengthTensorB;
  Double_t x[M]= { 300,   310,   320,   330,   340,   350,   360,   370,   380,   390,   
    400,   410,   420,   430,   440,   450,   460,   470,   480,   490,   
    500,   510,   520,   530,   540,   550,   560,   570,   580,   590,   
    600,   610,   620,   630,   640,   650,   660,   670,   680,   690,   
    700,   710,   720,   730,   740,   750,   760,   770,   780,   790,   
    800,   810,   820,   830,   840,   850,   860,   870,   880,   890,   
    900,   910,   920,   930,   940,   950,   960,   970,   980,   990  };
  Double_t y[M]= { 1.84,  1.83,  1.82,  1.82,  1.81,  1.81,  2.06,  3.08,  4.45,  6.36,  
    8.73,  11.32, 13.91, 16.48, 19.25, 22.42, 25.90, 29.26, 32.26, 35.02, 
    37.71, 40.61, 42.67, 44.32, 45.58, 46.67, 47.50, 48.62, 49.76, 50.61, 
    51.46, 52.15, 53.30, 54.42, 55.98, 57.58, 59.56, 61.63, 64.27, 67.41, 
    70.72, 74.49, 78.45, 83.55, 89.02, 95.40, 103.2, 110.3, 118.9, 124.8, 
    132.0, 132.3, 136.9, 139.4, 140.8, 152.9, 167.2, 184.5, 205.9, 232.9,
    232.9, 232.9, 232.9, 232.9, 232.9, 233.2, 233.2, 233.2, 233.2, 233.2  };
  Double_t s15[M]= { 3,     3,     3,     3,     3,     3,     3,     3,     1,     1,     
    1,     1,     1,     1,     1,     2,     2,     2,     2,     3,     
    4,     4,     5,     5,     5,     6,     6,     6,     6,     7,     
    7,     7,     8,     8,     9,     9,     10,    10,    11,    12,    
    13,    14,    15,    17,    19,    21,    23,    26,    28,    30,
    34,    33,    37,    37,    38,    44,    44,    44,    44,    44,    
    44,    44,    44,    44,    44,    44,    44,    44,    44,    44  };
  AbsLengthTensorB = new TSplineFit("AbsLengthTensor_PbWO4","Element of absorption length tensor | PbWO4",
    6,M,m,x,y,s15,kTRUE,0.0,kTRUE,233.2,300.0,1000.0,kFALSE);
  AbsLengthTensorB->BelongsToFamily(1,2.0,"Axis to associate");
  AbsLengthTensorB->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay"); 
  AbsLengthTensorB->SetMacro("AbsLengthTensor_PbWO4_001.C");
  AbsLengthTensorB->SetXLabel("wavelength [nm]");
  AbsLengthTensorB->SetVLabel("absorption length");
  if (todraw) {
    AbsLengthTensorB->SetDefaultLabels();
    AbsLengthTensorB->DrawFit();
    AbsLengthTensorB->Print();
  }
  if (infile) {
    if (firstinfile) AbsLengthTensorB->UpdateFile(kTRUE);
    else             AbsLengthTensorB->UpdateFile(kFALSE);
  }
  return AbsLengthTensorB;
}
