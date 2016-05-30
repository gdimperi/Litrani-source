TSplineFit* AbsorptionLength_crys8981R03(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsorptionLength_crys8981R03
//
//  Description :
//
//  Longueur d'absorption PbWO4 (cristal CMS 8981)
//  mesures du 02/04/2002
//  moyenne de l'absorption suivant les deux axes, en cm
//  wavelength [nm].
//
//  Source : Remi Chipaux
//
//  Old f_absmlpwo8981_03.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 45;
  //const Int_t m = 1;
  Int_t i;
  TSplineFit *AbsLPbWO4;
  Double_t x[M]= {330,    340,    350,    360,    370,    380,    390,    400,
    410,    420,    430,    440,    450,    460,    470,    480,
    490,    500,    510,    520,    530,    540,    550,    560,
    570,    580,    590,    600,    610,    620,    630,    640,
    650,    660,    670,    680,    690,    700,    710,    720,
    730,    740,    750,    800,    850 };
  Double_t y[M]= {    2.44,     2.28,     4.26,    17.18,    68.39,   168.06,   271.65,   325.89,
    389.90,   465.18,   535.62,   615.13,   670.45,   924.34,  1160.53,  1125.17,
    1027.91,  1646.40,  8051.71,  9071.95,  5555.48,  3084.82,  3532.09,  3536.72,
    4430.71,  7462.98,  8148.24, 10000.00,  8192.47, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00 };
  //Double_t s[M]= {   1.64,     1.63,     1.27,     0.33,     2.39,    11.56,    27.47,    39.89,
  //                  64.80,    99.33,   142.39,   233.45,   253.69,   231.08,   290.13,   281.29,
  //                 256.98,   411.60,  2012.93,  2267.99,  1388.87,   771.20,   883.02,   884.18,
  //                1107.68,  1865.74,  2037.06,  2500.00,  2048.12,  2500.00,  2500.00,  2500.00,
  //                2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,
  //                2500.00,  2500.00,  2500.00,  2500.00,  2500.00 };
  AbsLPbWO4 = new TSplineFit("AbsorptionLength_crys8981R03","Absorption Length | CMS crystal 8981 meas 2/4/2002",
    5,M,x,y,320.0,850.0);
  //                           5,M,m,x,y,s,kTRUE,2.0,kTRUE,10000.0,320.0,850.0,kFALSE);
  AbsLPbWO4->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  AbsLPbWO4->SetMacro("AbsorptionLength_crys8981R03.C");
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
