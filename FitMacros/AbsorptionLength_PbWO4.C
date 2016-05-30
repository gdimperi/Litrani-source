TSplineFit* AbsorptionLength_PbWO4(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsorptionLength_PbWO4
//
//   Description :
//
//     Absorption length of PbWO4 ( crystal 1453 of CMS ) as a function of
//   wavelength [nm].
//
//  Source : Remy Chipeaux
//
//  Old f_abslpbwo4
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 70;
  const Int_t m = 5;
  Int_t i;
  TSplineFit *AbsLPbWO4;
  Double_t x[M]= { 300,  310,  320,  330,  340,  350,  360,  370,  380,  390, 
    400,  410,  420,  430,  440,  450,  460,  470,  480,  490,  
    500,  510,  520,  530,  540,  550,  560,  570,  580,  590,  
    600,  610,  620,  630,  640,  650,  660,  670,  680,  690,  
    700,  710,  720,  730,  740,  750,  760,  770,  780,  790,  
    800,  810,  820,  830,  840,  850,  860,  870,  880,  890,  
    900,  910,  920,  930,  940,  950,  960,  970,  980,  990 };
  Double_t y[M]= {  2.46,  2.44,  2.43,  2.43,  2.42,  2.42,  2.75,  4.11,  5.94,  8.48, 
    11.64, 15.09, 18.54, 21.98, 25.67, 29.89, 34.53, 39.01, 43.02, 46.70, 
    50.29, 54.14, 56.89, 59.09, 60.78, 62.23, 63.34, 64.83, 66.35, 67.48, 
    68.62, 69.53, 71.07, 72.56, 74.65, 76.77, 79.42, 82.17, 85.69, 89.88,
    94.29, 99.32,104.6, 111.4, 118.7, 127.2, 137.6, 147.1, 158.5, 166.4, 
    176.0, 176.4, 182.6, 185.9, 187.7, 203.9, 223.0, 246.0, 274.5, 310.5, 
    310.5, 310.5, 310.5, 310.5, 310.5, 311.0, 311.0, 311.0, 311.0, 311.0 };
  Double_t s09[M]= {  3.0,   3.0,   3.0,   3.0,   3.0,   3.0,   3.0,   3.0,   1.0,   1.0,    
    1.0,   1.0,   1.0,   1.0,   1.0,   2.0,   2.0,   2.0,   2.0,   3.0,   
    4.0,   4.0,   5.0,   5.0,   5.0,   6.0,   6.0,   6.0,   6.0,   7.0,   
    7.0,   7.0,   8.0,   8.0,   9.0,   9.0,  10.0,  10.0,  11.0,  12.0,  
    13.0,  14.0,  15.0,  17.0,  19.0,  21.0,  23.0,  26.0,  28.0,  30.0,  
    34.0,  33.0,  37.0,  37.0,  38.0,  44.0,  44.0,  44.0,  44.0,  44.0,   
    44.0,  44.0,  44.0,  44.0,  44.0,  44.0,  44.0,  44.0,  44.0,  44.0 };
  AbsLPbWO4 = new TSplineFit("AbsorptionLength_PbWO4","Absorption Length | PbWO4",
    5,M,m,x,y,s09,kTRUE,0.0,kTRUE,312.0,300.0,1000.0,kFALSE);
  AbsLPbWO4->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  AbsLPbWO4->SetMacro("AbsorptionLength_PbWO4.C");
  AbsLPbWO4->SetXLabel("wavelength [nm]");
  AbsLPbWO4->SetVLabel("absorption length [cm]");
  if (todraw) {
    AbsLPbWO4->SetDefaultLabels();
    AbsLPbWO4->DrawFit();
    AbsLPbWO4->Print();
  }
  if (infile) {
    if (firstinfile) AbsLPbWO4->UpdateFile(kTRUE);
    else             AbsLPbWO4->UpdateFile(kFALSE);
  }
  return AbsLPbWO4;
}
