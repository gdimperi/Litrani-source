TSplineFit* AbsLengthTensor_PbWO4_000(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
//
// Arguments:
//
//  todraw       : true if the fit is to be drawn
//  infile       : true if the fit is to be put in file SplineFitDB.rdb
//  firstinfile  : if true, [BE CAREFUL: RISK OF LOOSING FILE SplineFitDB.rdb ]
//                 delete all fits present in file SplineFitDB.rdb and place
//                 this fit as the first fit in a new file SplineFitDB.rdb
//                 default false !!!
// All defaults for arguments correspond to the case where one calls this CINT
//macro from within a Litrani CINT macro to define a needed fit, instead of trying
//to find it in the database file SplineFitDB.rdb using TSplineFit::FindFit()
//
//   AbsLengthTensor_PbWO4_000
//
//   Description :
//
//     Part of the absorption length of PbWO4 ( crystal 1453 of CMS ) which is
//   associated with element epsa of dielectric tensor, as a function of
//   wavelength [nm]. It is assumed here that not only the dielectric constant
//   is anisotropic (is a tensor) but also that the absorption length is
//   anisotropic. See the discussion of this case in the class description
//   of TOpticMaterial.
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 70;
  const Int_t m = 5;
  Int_t i;
  TSplineFit *AbsLengthTensorA;
  Double_t x[M]= { 300,  310,  320,  330,  340,  350,  360,  370,  380,  390,  
    400,  410,  420,  430,  440,  450,  460,  470,  480,  490,  
    500,  510,  520,  530,  540,  550,  560,  570,  580,  590,  
    600,  610,  620,  630,  640,  650,  660,  670,  680,  690,  
    700,  710,  720,  730,  740,  750,  760,  770,  780,  790,  
    800,  810,  820,  830,  840,  850,  860,  870,  880,  890,  
    900,  910,  920,  930,  940,  950,  960,  970,  980,  990  };
  Double_t y[M]= {  2.46, 2.44, 2.43, 2.43, 2.42, 2.42, 2.75, 4.11, 5.94, 8.48, 
    11.64,15.09,18.54,21.98,25.67,29.89,34.53,39.01,43.02,46.70,
    50.29,54.14,56.89,59.09,60.78,62.23,63.34,64.83,66.35,67.48,
    68.62,69.53,71.07,72.56,74.65,76.77,79.42,82.17,85.69,89.88,
    94.29,99.32,104.6,111.4,118.7,127.2,137.6,147.1,158.5,166.4,
    176.0,176.4,182.6,185.9,187.7,203.9,223.0,246.0,274.5,310.5,
    310.5,310.5,310.5,310.5,310.5,311.0,311.0,311.0,311.0,311.0  };
  Double_t s14[M]= { 3,   3,   3,   3,   3,   3,   3,   3,   1,   1,   
    1,   1,   1,   1,   1,   2,   2,   2,   2,   3,   
    4,   4,   5,   5,   5,   6,   6,   6,   6,   7,   
    7,   7,   8,   8,   9,   9,   10,  10,  11,  12,  
    13,  14,  15,  17,  19,  21,  23,  26,  28,  30,  
    34,  33,  37,  37,  38,  44,  44,  44,  44,  44,  
    44,  44,  44,  44,  44,  44,  44,  44,  44,  44  };
  AbsLengthTensorA = new TSplineFit("AbsLengthTensor_PbWO4","Element of absorption length tensor | PbWO4",
    6,M,m,x,y,s14,kTRUE,0.0,kTRUE,312.0,300.0,1000.0,kFALSE);
  AbsLengthTensorA->BelongsToFamily(0,1.0,"Axis to associate");
  AbsLengthTensorA->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay"); 
  AbsLengthTensorA->SetMacro("AbsLengthTensor_PbWO4_000.C");
  AbsLengthTensorA->SetXLabel("wavelength [nm]");
  AbsLengthTensorA->SetVLabel("absorption length");
  if (todraw) {
    AbsLengthTensorA->SetDefaultLabels();
    AbsLengthTensorA->DrawFit();
    AbsLengthTensorA->Print();
  }
  if (infile) {
    if (firstinfile) AbsLengthTensorA->UpdateFile(kTRUE);
    else             AbsLengthTensorA->UpdateFile(kFALSE);
  }
  return AbsLengthTensorA;
}
