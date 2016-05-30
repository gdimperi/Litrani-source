TSplineFit* AbsorptionLength_crys8981R06(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsorptionLength_crys8981R06
//
//  Description :
//
//  Longueur d'absorption PbWO4 (cristal CMS 8981)
//  mesures du 30/04/2002
//  moyenne de l'absorption suivant les deux axes, en cm
//  wavelength [nm].
//
//  Source : Remi Chipaux
//
//  Old f_absmlpwo8981_06.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 45;
  const Int_t m = 2;
  Int_t i;
  TSplineFit *AbsLPbWO4;
  Double_t x[M]= {330,     340,     350,     360,     370,     380,     390,     400,
    410,     420,     430,     440,     450,     460,     470,     480,
    490,     500,     510,     520,     530,     540,     550,     560,
    570,     580,     590,     600,     610,     620,     630,     640,
    650,     660,     670,     680,     690,     700,     710,     720,
    730,     740,     750,     800,     850 };
  Double_t y[M]= {    2.55,     2.51,     3.84,    15.99,    63.58,   163.48,   265.03,   328.94,
    385.59,   448.64,   548.00,   591.35,   633.50,   775.52,   953.75,   951.94,
    1032.86,  1224.03,  1669.46,  1846.80,  1986.93,  1823.65,  1881.68,  2281.20,
    3272.70,  3465.51,  3667.48,  3841.44,  4018.59,  4099.19,  4822.44,  8210.73,
    20139.67, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00 };
  Double_t s[M]= {   1.91,     1.90,     2.09,     0.59,     3.85,    20.48,    49.51,    80.26,
                   131.90,   169.23,   340.95,   586.32,   537.76,   193.88,   238.44,   237.98,
                   258.21,   306.01,   417.37,   461.70,   496.73,   455.91,   470.42,   570.30,
                   818.17,   866.38,   916.87,   960.36,  1004.65,  1024.80,  1205.61,  2052.68,
                  5034.92,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,
                  2500.00,  2500.00,  2500.00,  2500.00,  2500.00 };
  AbsLPbWO4 = new TSplineFit("AbsorptionLength_crys8981R06","Absorption Length | CMS crystal 8981 meas 30/04/2002",
  //  5,M,x,y,320.0,,850.0);
  5,M,m,x,y,s,kTRUE,1.0,kTRUE,21000.0,320.0,850.0,kFALSE);
  AbsLPbWO4->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  AbsLPbWO4->SetMacro("AbsorptionLength_crys8981R06.C");
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
