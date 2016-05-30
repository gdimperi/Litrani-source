TSplineFit* AbsorptionLength_Meltmount1704(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   AbsorptionLength_Meltmount1704
//
//   Description :
//
//   Absorption length of Glue Meltmount 1.704 as a function of wavelength in [nm]
//
//   Source : Remi Chipeaux : Meltmount 1.704
//
//   Old f_abslglue or f_abslrtv3145 (identical!)
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 34;
  const Int_t m = 5;
  Int_t i;
  TSplineFit *AbsLMeltmount;
  Double_t x[M]= { 320,   340,   360,   380,   400,   420,   440,   460,   480,   500,
    520,   540,   560,   580,   600,   620,   640,   660,   680,   700,
    720,   740,   760,   780,   800,   820,   840,   860,   880,   900,
    920,   940,   960,   980 };
  Double_t y[M]= { 0.000527, 0.000830, 0.001958, 0.011993, 0.041164, 0.061999, 0.076422, 0.082832, 0.086455, 0.090408,
    0.094737, 0.099499, 0.102063, 0.104762, 0.110610, 0.113784, 0.117146, 0.120711, 0.124499, 0.128532,
    0.132832, 0.137430, 0.142356, 0.147647, 0.153345, 0.159499, 0.166166, 0.173412, 0.181318, 0.189975,
    0.199499, 0.210025, 0.221722, 0.234794 };
  Double_t s08[M]= { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
    0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
    0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
    0.001, 0.001, 0.001, 0.001 };
  AbsLMeltmount = new TSplineFit("AbsorptionLength_Meltmount1704","Absorption Length | Glue Meltmount 1.704",
    5,M,m,x,y,s08,kTRUE,0.0,kTRUE,0.24,300.0,1000.0,kFALSE);
  AbsLMeltmount->SetSource("Meltmount"); 
  AbsLMeltmount->SetMacro("AbsorptionLength_Meltmount1704.C");
  AbsLMeltmount->SetXLabel("wavelength [nm]");
  AbsLMeltmount->SetVLabel("absorption length");
  if (todraw) {
    AbsLMeltmount->SetDefaultLabels();
    AbsLMeltmount->DrawFit();
    AbsLMeltmount->Print();
  }
  if (infile) {
    if (firstinfile) AbsLMeltmount->UpdateFile(kTRUE);
    else             AbsLMeltmount->UpdateFile(kFALSE);
  }
  return AbsLMeltmount;
}
