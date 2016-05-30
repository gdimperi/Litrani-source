TSplineFit* RadDamage_Kimax02(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  RadDamage_Kimax02
//
//  Derived from AbsorptionLength_crys8981R02. See the EXCEL file RadDamages.xls.
//  Describes the part induced by irradiation in the inverse of the absorption
//  length. It is after the first irradiation in GIF. Measurements of the 22/3/2002.
//  As the measurements give the mean value of the absorption length along the
//  path of the photons, and as what is needed for Kimax is not the mean value
//  but the absorption length at the point where the damages are maximal, we
//  introduce a factor "meantomax", by which we multiply all the measurements.
//
//  Source : Remi Chipaux
//
{
  const Double_t meantomax = 2.0;
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 46;
  //const Int_t m = 2;
  Int_t i;
  TSplineFit *damage;
  Double_t x[M]= {320,    330,    340,    350,    360,    370,    380,    390,    400,    410,
    420,    430,    440,    450,    460,    470,    480,    490,    500,    510,
    520,    530,    540,    550,    560,    570,    580,    590,    600,    610,
    620,    630,    640,    650,    660,    670,    680,    690,    700,    710,
    720,    730,    740,    750,    800,    850 };
  Double_t y[M]= {           0,   0.011522634,   0.011616715,  0.007609026,  0.002014656,  -4.41309E-05,
    -0.000264784,  -0.000163614,  -8.94069E-05,  2.31551E-05,  8.20281E-05,  -2.1225E-05,
    -1.7864E-05,   -0.000103923,   0.000137581,  5.04412E-05,  0.000132326,  -3.55608E-05,
    0.000382782,             0,             0,            0,            0,   0.000123177,
    3.27884E-05,             0,             0,            0,            0,             0,
    0,             0,             0,            0,            0,             0,
    0,             0,             0,            0,            0,             0,
    0,             0,             0,            0  };
  Double_t s[M];
  for (i=0;i<M;i++) s[i] = 0.001;
  for (i=0;i<M;i++) y[i] *= meantomax;
  damage = new TSplineFit("RadDamage_Kimax02","Radiation damages in shape | Kimax after 1st irradiation",
    15,M,x,y,320.0,850.0);
  //                           15,M,m,x,y,s,kFALSE,0.0,kFALSE,0.0,320.0,850.0,kFALSE);
  damage->SetSource("Remi Chipaux");
  damage->SetMacro("RadDamage_Kimax02.C");
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
