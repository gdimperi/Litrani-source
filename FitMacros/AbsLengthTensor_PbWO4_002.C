TSplineFit* AbsLengthTensor_PbWO4_002(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   AbsLengthTensor_PbWO4_002
//
//   Description :
//
//     Part of the absorption length of PbWO4 ( crystal 1453 of CMS ) which is
//   associated with element epsc of dielectric tensor, as a function of
//   wavelength [nm]. It is assumed here that not only the dielectric constant
//   is anisotropic (is a tensor) but also that the absorption length is
//   anisotropic. See the discussion of this case in the class description
//   of TOpticMaterial.
//
//   Old f_abslpbwo4c
//
{
Int_t k1;
Int_t k2 = -100;
k1 = TClassTable::GetID("TSplineFit");
if (k1<0) k2 = gSystem.Load("libSplineFit");
const Int_t M = 70;
const Int_t m = 5;
Int_t i;
TSplineFit *AbsLengthTensorC;
Double_t x[M]= { 300,   310,   320,   330,   340,   350,   360,   370,   380,   390,   
                 400,   410,   420,   430,   440,   450,   460,   470,   480,   490,   
                 500,   510,   520,   530,   540,   550,   560,   570,   580,   590,   
                 600,   610,   620,   630,   640,   650,   660,   670,   680,   690,   
                 700,   710,   720,   730,   740,   750,   760,   770,   780,   790,   
                 800,   810,   820,   830,   840,   850,   860,   870,   880,   890,   
                 900,   910,   920,   930,   940,   950,   960,   970,   980,   990 };
Double_t y[M]= { 1.38,  1.37,  1.36,  1.36,  1.36,  1.36,  1.54,  2.31,  3.34,  4.77,  
                 6.55,  8.49,  10.43, 12.36, 14.44, 16.81, 19.42, 21.94, 24.19, 26.26, 
                 28.28, 30.46, 32.00, 33.24, 34.18, 35.00, 35.62, 36.46, 37.32, 37.96, 
                 38.59, 39.11, 39.97, 40.81, 41.98, 43.18, 44.67, 46.22, 48.20, 50.56, 
                 53.04, 55.87, 58.84, 62.66, 66.76, 71.55, 77.40, 82.72, 89.17, 93.60, 
                 99.0 , 99.22, 102.7, 104.5, 105.6, 114.7, 125.4, 138.4, 154.4, 174.7, 
                 174.7, 174.7, 174.7, 174.7, 174.7, 174.9, 174.9, 174.9, 174.9, 174.9 };
Double_t s16[M]= { 3,     3,     3,     3,     3,     3,     3,     3,     1,     1,     
                 1,     1,     1,     1,     1,     2,     2,     2,     2,     3,     
                 4,     4,     5,     5,     5,     6,     6,     6,     6,     7,     
                 7,     7,     8,     8,     9,     9,     10,    10,    11,    12,    
                 13,    14,    15,    17,    19,    21,    23,    26,    28,    30,    
                 34,    33,    37,    37,    38,    44,    44,    44,    44,    44,    
                 44,    44,    44,    44,    44,    44,    44,    44,    44,    44 };
AbsLengthTensorC = new TSplineFit("AbsLengthTensor_PbWO4","Element of absorption length tensor | PbWO4",
                                 6,M,m,x,y,s16,kTRUE,0.0,kTRUE,175.0,300.0,1000.0,kFALSE);
AbsLengthTensorC->BelongsToFamily(2,3.0,"Axis to associate");
AbsLengthTensorC->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
AbsLengthTensorC->SetMacro("AbsLengthTensor_PbWO4_002.C");
AbsLengthTensorC->SetXLabel("wavelength [nm]");
AbsLengthTensorC->SetVLabel("absorption length");
  if (todraw) {
    AbsLengthTensorC->SetDefaultLabels();
    AbsLengthTensorC->DrawFit();
    AbsLengthTensorC->Print();
  }
  if (infile) {
    if (firstinfile) AbsLengthTensorC->UpdateFile(kTRUE);
    else             AbsLengthTensorC->UpdateFile(kFALSE);
  }
  return AbsLengthTensorC;
}
