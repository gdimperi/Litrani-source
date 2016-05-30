TSplineFit* RIndexRev_CMSferule(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RIndexRev_CMSferule
//
//   Real part of index of refraction for the piece fixing the optical fibre ("porte
// ferule") in function of wavelength. Knowing nothing about it, for the time being, we
// take the characteristics of aluminium.
//
//   Source : No Source yet. Values for aluminium taken
//
//   Old f_realferule.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 29;
  const Int_t m = 4;
  Int_t i;
  Double_t sig[M];
  TSplineFit *RIndexRevFerule;
  Double_t x[M] = { 300.0,   317.9,   335.1,   350.0,   364.7,   387.5,   413.3,   442.8,   459.2,
    495.9,   506.1,   527.6,   550.0,   563.6,   590.4,   604.8,   635.8,   652.6,
    688.8,   708.5,   750.0,   774.9,   825.0,   850.0,   875.0,   900.0,   925.0,
    950.0,   991.9 };
  Double_t y[M] = { 0.276,   0.310,   0.344,   0.375,   0.407,   0.460,   0.523,   0.598,   0.644,
    0.755,   0.789,   0.867,   0.958,   1.02,    1.15,    1.22,    1.39,    1.49,
    1.74,    1.91,    2.40,    2.63,    2.75,    2.61,    2.38,    2.06,    1.77,
    1.49,    1.37 };
  for (i=0;i<M;i++) sig[i] = 0.03;
  RIndexRevFerule = new TSplineFit("RIndexRev_CMSferule","Real part of refraction index | CMS ferule",
    3,M,m,x,y,sig,kTRUE,0.0,kFALSE,10.0,300.0,1000.0);
  RIndexRevFerule->SetSource("No Source yet. Values for aluminium taken");
  RIndexRevFerule->SetMacro("RIndexRev_CMSferule.C");
  RIndexRevFerule->SetXLabel("wavelength [nm]");
  RIndexRevFerule->SetVLabel("real part index");
  if (todraw) {
    RIndexRevFerule->SetDefaultLabels();
    RIndexRevFerule->DrawFit();
    RIndexRevFerule->Print();
  }
  if (infile) {
    if (firstinfile) RIndexRevFerule->UpdateFile(kTRUE);
    else             RIndexRevFerule->UpdateFile(kFALSE);
  }
  return RIndexRevFerule;
}
