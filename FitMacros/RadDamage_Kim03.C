TSplineFit* RadDamage_Kim03(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  RadDamage_Kim03
//
//  Derived from AbsorptionLength_crys8981R04.
//  Describes the part induced by irradiation in the inverse of the absorption
//  length. It is after the 3rd irradiation in GIF. Measurements of the 10/4/2002.
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
  Double_t y[M]= {  0.022347886, -0.017664528,  0.003424816,  0.000722685,  5.02153E-07, -5.44225E-05,
    -6.74372E-05,  5.56224E-05,  0.000141317,  0.000242843,  0.000155648,  0.000185662,
    0.000127905,  0.000182158,  0.000113472,  0.000239197,  7.00538E-05,  0.000989134,
    9.83369E-05, -1.75342E-05,  9.62417E-05, -9.88554E-06, -1.07059E-05,   3.7277E-05,
    0.000147581,  0.000103148,  7.28219E-06, -9.17719E-06, -4.18882E-05,  0.000131081,
    -6.91392E-05,  9.82163E-05,  2.77298E-05, -2.08395E-05,  7.34503E-05,  9.27571E-05,
    2.06449E-05,  9.44385E-05,  0.000188958,  6.44657E-05, -2.54848E-05, -6.88474E-05,
    0.000203716, 0.000203716,  0.000203716 };
  //Double_t s[M]= {  0.392877707, 0.3559779  , 0.097076363, 0.00149607 , 0.000665437, 0.00051246 ,
  //                  0.000469172, 0.000475485, 0.000501252, 0.000511296, 0.000507707, 0.000524797,
  //                  0.000601361, 0.000735594, 0.003738048, 0.001034634, 0.000842929, 0.005396623,
  //                  0.008594928, 0.206126352, 0.068227922, 0.110362468, 0.246655974, 0.086116258,
  //                  0.098693743, 0.060406726, 0.132677054, 0.093802534, 0.106587913, 0.123286624,
  //                  0.611401921, 0.445498633, 0.920064646, 0.000863556, 0.000675764, 0.000794584,
  //                  0.001176951, 0.001554862, 0.001139539, 0.005794866, 0.009341345, 0.001103592,
  //                  0.001002535, 0.001002535, 0.001002535 };
  Double_t s22[M];
  for (i=0;i<M;i++) s22[i] = 0.001;
  damage = new TSplineFit("RadDamage_Kim03","Radiation damages in shape | Kimax after 3rd irradiation",
    15,M,x,y,320.0,850.0);
  //                           15,M,m,x,y,s22,kFALSE,0.0,kFALSE,0.0,320.0,850.0,kFALSE);
  damage->SetSource("Remi Chipaux");
  damage->SetMacro("RadDamage_Kim03.C");
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
