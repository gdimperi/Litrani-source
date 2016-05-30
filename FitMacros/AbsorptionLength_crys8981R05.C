TSplineFit* AbsorptionLength_crys8981R05(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsorptionLength_crys8981R05
//
//  Description :
//
//  Longueur d'absorption PbWO4 (cristal CMS 8981)
//  mesures du 12/04/2002
//  moyenne de l'absorption suivant les deux axes, en cm
//  wavelength [nm].
//
//  Source : Remi Chipaux
//
//  Old f_absmlpwo8981_05.C
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
  Double_t y[M]= {    2.71,     2.33,     4.29,    17.24,    66.96,   162.09,   254.71,   321.13,
    358.11,   427.84,   509.91,   510.67,   652.04,   828.49,  1198.96,  1046.96,
    1031.38,  1687.16, 10000.00, 10000.00, 10000.00,  5115.90,  3984.45,  4973.59,
    8830.20, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00 };
  Double_t s13[M]= {   1.92,     1.90,     2.11,     0.60,     3.96,    17.91,    43.66,    69.25,
    94.69,   155.29,   273.83,   283.29,   163.01,   207.12,   299.74,   261.74,
    257.84,   421.79,  2500.00,  2500.00,  2500.00,  1278.98,   996.11,  1243.40,
    2207.55,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,
    2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,
    2500.00,  2500.00,  2500.00,  2500.00,  2500.00 };
  AbsLPbWO4 = new TSplineFit("AbsorptionLength_crys8981R05","Absorption Length | CMS crystal 8981 meas 12/04/2002",
    //                           5,M,x,y,320.0,850.0);
    5,M,m,x,y,s13,kTRUE,1.0,kTRUE,10000.0,320.0,850.0,kFALSE);
  AbsLPbWO4->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  AbsLPbWO4->SetMacro("AbsorptionLength_crys8981R05.C");
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
