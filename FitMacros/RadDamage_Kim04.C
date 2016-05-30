TSplineFit* RadDamage_Kim04(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  RadDamage_Kim04
//
//  Derived from AbsorptionLength_crys8981R05.
//  Describes the part induced by irradiation in the inverse of the absorption
//  length. It is after the 4th irradiation in GIF. Measurements of the 12/4/2002.
//
//  Source : Remi Chipaux
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 45;
  const Int_t m = 3;
  Int_t i;
  TSplineFit *damage;
  Double_t x[M]= { 330,          340,          350,          360,          370,          380,
    390,          400,          410,          420,          430,          440,
    450,          460,          470,          480,          490,          500,
    510,          520,          530,          540,          550,          560,
    570,          580,          590,          600,          610,          620,
    630,          640,          650,          660,          670,          680,
    690,          700,          710,          720,          730,          740,
    750,          800,          850 };
  Double_t y[M]= { -0.03074688,  0.027794641,  0.003602412,  0.001045303,  5.72557E-05,  0.000182739,
    0.000298442,  0.000280647,  0.000472589,  0.000483592,  0.000336718,  0.000581542,
    0.000245905,  0.000393171,  0.000196081,  0.000296724,  0.000173127,  0.001060392,
    0.000165839,  8.13703E-05,  0.000100651,  6.90803E-05,  6.61006E-05,  8.03832E-05,
    0.000150656,  0.000151699,  7.61465E-05,  7.06933E-05,  3.98429E-05,  0.000125874,
    4.04618E-05,   6.2058E-05,  1.08666E-05,  4.57403E-05,  3.55307E-05,  1.72251E-05,
    2.67541E-05,  0.000131857,  0.000219778, -3.05512E-05,  2.49932E-05,   -0.0001063,
    2.80687E-05,  2.80687E-05,  2.80687E-05 };
  //Double_t s[M]= {  0.370756597, 0.437796827, 0.133141652, 0.002279023, 0.000994332, 0.000762989,
  //                    0.000744135, 0.00074357 , 0.000800851, 0.000909084, 0.001105658, 0.001138277,
  //                    0.001603387, 0.004399463, 3.99984412 , 5.245545018, 0.0202191  , 2.019946767,
  //                    0.009954972, 0.029586282, 0.074956745, 0.310682366, 0.512182723, 0.173371762,
  //                    0.104283844, 0.011522964, 0.028190618, 0.006167655, 0.008730695, 0.133944989,
  //                    0.220497549, 0.593866928, 1.415797776, 2.247374168, 3.120477687, 0.000794584,
  //                    0.001176951, 3.003166827, 3.473034186, 4.956696209, 4.596097803, 7.198379295,
  //                    0.001002535, 0.001002535, 0.001002535 };
  Double_t s23[M];
  for (i=0;i<M;i++) s23[i] = 0.001;
  damage = new TSplineFit("RadDamage_Kim04","Radiation damages in shape | Kimax after 4th irradiation",
    15,M,x,y,320.0,850.0);
  //                           15,M,m,x,y,s23,kFALSE,0.0,kFALSE,0.0,320.0,850.0,kFALSE);
  damage->SetSource("Remi Chipaux");
  damage->SetMacro("RadDamage_Kim04.C");
  damage->SetXLabel("wavelength [nm]");
  damage->SetVLabel("damage factor kimax [cm-1]");
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
