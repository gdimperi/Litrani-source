TSplineFit* RadDamage_GzPbWO4(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RadDamage_GzPbWO4
//
//   damgz gives the possibility to give under the form of a fit
//    the z dependent part of the distribution of damages
//
//   Source : Values at random, just for test
//
//   Old f_damgz.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 25;
  const Int_t m = 3;
  Int_t i;
  TSplineFit *damage;
  Double_t x[M]= {  0.0,   1.0,   2.0,   3.0,   4.0,   5.0,   6.0,   7.0,   8.0,   9.0,
    10.0,  11.0,  12.0,  13.0,  14.0,  15.0,  16.0,  17.0,  18.0,  19.0,
    20.0,  21.0,  22.0,  23.0,  24.0 };
  Double_t y[M]= { 0.1,   0.2,   0.3,   0.4,   0.5,   0.6,   0.7,   0.8,   0.9,   0.9,
    0.8,   0.7,   0.6,   0.5,   0.4,   0.3,   0.2,   0.1,   0.1,   0.2,
    0.3,   0.4,   0.3,   0.2,   0.1 };
  Double_t s19[M];
  for (i=0;i<M;i++) s19[i] = 0.1;
  damage = new TSplineFit("RadDamage_GzPbWO4","Radiation damages in shape | g(z) PbWO4",
    15,M,m,x,y,s19,kTRUE,0.0,kTRUE,1.0,0.0,24.0,kFALSE);
  damage->SetSource("Values at random, just for test");
  damage->SetMacro("RadDamage_GzPbWO4.C");
  damage->SetXLabel("z [cm]");
  damage->SetVLabel("distribution [arbitrary]");
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
