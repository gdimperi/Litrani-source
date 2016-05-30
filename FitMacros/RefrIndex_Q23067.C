TSplineFit* RefrIndex_Q23067(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RefrIndex_Q23067
//
//   Description :
//
//   Refraction index for the grease
//
//   Source : J.P. Peigneux : n=1.468 a 589.3 nm 
//                           et pente p = -0.000043216804
//
//   OLD f_refrigrease
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 11;
  const Int_t m = 6;
  Int_t i;
  Double_t sig[M];
  TSplineFit *RefrIQ23067;
  Double_t x[M] = { 300.0,    350.0,    400.0,    450.0,    500.0,    589.3,    600.0,     700.0,
    800.0,    900.0,    1000.0 };
  Double_t y[M] = { 1.480466, 1.478319, 1.476170, 1.474017, 1.47186,  1.468,    1.467537,  1.4632,
    1.458850, 1.454486, 1.450108 };
  for (i=0;i<M;i++) sig[i] = 0.1;
  RefrIQ23067 = new TSplineFit("RefrIndex_Q23067","Index of refraction | optical grease Q23067",
    1,M,m,x,y,sig,kTRUE,1.0,kFALSE,2.0,300.0,1000.0);
  RefrIQ23067->SetSource("Jean-Pierre Peigneux LAPP Annecy");
  RefrIQ23067->SetMacro("RefrIndex_Q23067.C");
  RefrIQ23067->SetXLabel("wavelength [nm]");
  RefrIQ23067->SetVLabel("Refraction index");
  RefrIQ23067->SetParameter(0.0,"Grease used by CMS");
  if (todraw) {
    RefrIQ23067->SetDefaultLabels();
    RefrIQ23067->DrawFit();
    RefrIQ23067->Print();
  }
  if (infile) {
    if (firstinfile) RefrIQ23067->UpdateFile(kTRUE);
    else             RefrIQ23067->UpdateFile(kFALSE);
  }
  return RefrIQ23067;
}
