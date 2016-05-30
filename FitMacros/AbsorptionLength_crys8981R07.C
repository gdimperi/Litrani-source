TSplineFit* AbsorptionLength_crys8981R07(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsorptionLength_crys8981R07
//
//  Description :
//
//  Longueur d'absorption PbWO4 (cristal CMS 8981)
//  mesures du 15/05/2002
//  moyenne de l'absorption suivant les deux axes, en cm
//  wavelength [nm].
//
//  Source : Remi Chipaux
//
//  Old f_absmlpwo8981_07.C
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
  Double_t y[M]= {    2.03,     2.61,     4.05,    16.80,    62.12,   143.23,   207.58,   244.59,
    274.67,   316.31,   339.02,   369.16,   408.69,   457.55,   509.98,   548.23,
    573.79,   647.29,   805.13,   894.96,   906.65,   930.70,   978.64,  1113.79,
    1325.71,  1543.48,  1623.51,  1791.64,  1970.12,  2168.58,  2299.67,  3403.43,
    3784.07,  5684.98,  6887.04,  8941.23, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00 };
  Double_t s12[M]= {   1.63,     1.63,     1.24,     0.33,     2.04,     8.71,    16.39,    22.88,
    31.21,    42.96,    52.48,    65.80,    77.71,   121.95,   196.05,   207.10,
    183.44,   259.00,   201.28,   223.74,   226.66,   232.68,   244.66,   278.45,
    331.43,   385.87,   405.88,   447.91,   492.53,   542.14,   574.92,   850.86,
    946.02,  1421.25,  1721.76,  2235.31,  2500.00,  2500.00,  2500.00,  2500.00,
    2500.00,  2500.00,  2500.00,  2500.00,  2500.00 };
  AbsLPbWO4 = new TSplineFit("AbsorptionLength_crys8981R07","Absorption Length | CMS crystal 8981 meas 15/05/2002",
    //   5,M,x,y,320.0,850.0);
       5,M,m,x,y,s12,kTRUE,1.0,kTRUE,10000.0,330.0,850.0,kFALSE);
  AbsLPbWO4->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  AbsLPbWO4->SetMacro("AbsorptionLength_crys8981R07.C");
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
