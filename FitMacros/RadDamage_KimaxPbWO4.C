TSplineFit* RadDamage_KimaxPbWO4(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RadDamage_KimaxPbWO4
//
//   If Limin(wavelength [cm] is the wavelength depending contribution to the
// absorption length of the damages at the point where the damages are maximal,
// we define Kimax(wavelength) == 1/Limin(wavelength) [cm-1]. This fit defines
// Kimax as a function of wavelength
//
//   Source : Values at random, just for test
//
//   Old f_dampbwo4
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 16;
  const Int_t m = 3;
  Int_t i;
  TSplineFit *damage;
  Double_t x[M]= {   300,   350,   400,   420,   450,   500,   530,   570,   600,   630,
    675,   740,   800,   900,   950,  1000 };
  Double_t y[M]= { 0.011, 0.015, 0.022, 0.025, 0.033, 0.042, 0.045, 0.035, 0.033, 0.029,
    0.028, 0.025, 0.024, 0.023, 0.022, 0.021 };
  Double_t s27[M];
  for (i=0;i<M;i++) s27[i] = 0.001;
  damage = new TSplineFit("RadDamage_KimaxPbWO4","Radiation damages in shape | Kimax PbWO4",
    15,M,m,x,y,s27,kTRUE,0.0,kFALSE,0.0,300.0,1000.0,kFALSE);
  damage->SetSource("Values at random, just for test");
  damage->SetMacro("RadDamage_KimaxPbWO4.C");
  damage->SetXLabel("wavelength [nm]");
  damage->SetVLabel("damage factor [cm-1]");
  if (todraw) {
    damage->SetDefaultLabels();
    damage->DrawFit();
    damage->Print();
  }
  if (infile) {
    if (firstinfile) damage->UpdateFile(kTRUE);
    else             damage->UpdateFile(kFALSE);
  }
  return damage;
}
