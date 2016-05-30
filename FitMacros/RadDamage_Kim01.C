TSplineFit* RadDamage_Kim01(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  RadDamage_Kim01
//
//  Derived from AbsorptionLength_crys8981R02.
//  Describes the part induced by irradiation in the inverse of the absorption
//  length. It is after the first irradiation in GIF. Measurements of the 22/3/2002.
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
  Double_t y[M]= { 0.011387959,  0.012484213,  0.007680787,  0.002008669, -4.53981E-05, -0.000264611, 
    -0.000163547, -8.94458E-05,  2.31533E-05,  8.20370E-05, -2.12057E-05, -1.78697E-05, 
    -0.00010392,  0.000137584,  5.04369E-05,  0.000132325, -3.55624E-05,  0.000950459, 
    6.89754E-05, -1.71041E-05, -1.16677E-05, -0.000109549, -9.61841E-06, -1.48675E-06, 
    -2.39499E-05,  0.000165495, -6.87488E-05,  6.13521E-05, -3.11873E-05,  8.38857E-05, 
    -2.3761E-05,  -1.5867E-05, -1.30913E-05,  4.24976E-06, -0.000108128, -2.42743E-05, 
    2.0282E-05,  4.03277E-05,  0.000139912,  3.81853E-05,  2.91656E-05, -0.000124585, 
    0.000101347, 0.000101347,  0.000101347 };
  //Double_t s[M]= { 0.382306832,  0.383220971,  0.098346732,  0.001539450,  0.000691928,  0.000525068,
  //                 0.000492131,  0.000499495,  0.000514426,  0.000536208,  0.000565057,  0.000697772,
  //                 0.00071871),  0.001200804,  2.724819056,  0.009099771,  0.00181282),  1.340106394,
  //                 0.031126097,  0.204957418,  0.002161971,  0.002305867,  0.249742075,  0.033459539,
  //                 0.030607306,  0.004624733,  0.337334681,  0.011059595,  0.087590448,  0.235871481,
  //                 0.301218689,  0.696842191,  1.118533139,  0.000863556,  0.000675764,  0.000794584,
  //                 0.001176951,  0.001554862,  0.001139539,  0.005794866,  0.009341345,  0.001103592,
  //                 0.001002535,  0.001002535,  0.001002535 };
  Double_t s20[M];
  for (i=0;i<M;i++) s20[i] = 0.001;
  damage = new TSplineFit("RadDamage_Kim01","Radiation damages in shape | Kimax after 1st irradiation",
    15,M,x,y,320.0,850.0);
  //                           15,M,m,x,y,s20,kFALSE,0.0,kFALSE,0.0,320.0,850.0,kFALSE);
  damage->SetSource("Remi Chipaux");
  damage->SetMacro("RadDamage_Kim01.C");
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
