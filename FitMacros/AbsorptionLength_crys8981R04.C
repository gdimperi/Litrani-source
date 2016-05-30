TSplineFit* AbsorptionLength_crys8981R04(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsorptionLength_crys8981R04
//
//  Description :
//
//  Longueur d'absorption PbWO4 (cristal CMS 8981)
//  mesures du 10/04/2002
//  moyenne de l'absorption suivant les deux axes, en cm
//  wavelength [nm].
//
//  Source : Remi Chipaux
//
//  Old f_absmlpwo8981_04.C
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
  Double_t x[M]= {330,     340,     350,     360,     370,     380,     390,     400,
    410,     420,     430,     440,     450,     460,     470,     480,
    490,     500,     510,     520,     530,     540,     550,     560,
    570,     580,     590,     600,     610,     620,     630,     640,
    650,     660,     670,     680,     690,     700,     710,     720,
    730,     740,     750,     800,     850 };
  Double_t y[M]= {    2.37,     2.61,     4.30,    17.34,    67.22,   168.57,   280.88,   346.14,
    406.31,   476.97,   561.78,   640.08,   706.39,  1004.01,  1330.76,  1114.06,
    1154.06,  1917.71, 10000.00, 10000.00, 10916.70,  8583.46,  5741.55,  9716.93,
    9076.68, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00 };
  //Double_t s[M]= {   1.64,      1.63,      1.28,      0.32,      2.19,     10.82,     27.26,     42.21,
  //                  65.02,     89.48,    119.94,    163.77,    244.25,    251.00,    332.69,    278.52,
  //                 288.51,    479.43,   2500.00,   2500.00,   2729.17,   2145.86,   1435.39,   2429.23,
  //                2269.17,   2500.00,   2500.00,   2500.00,   2500.00,   2500.00,   2500.00,   2500.00,
  //                2500.00,   2500.00,   2500.00,   2500.00,   2500.00,   2500.00,   2500.00,   2500.00,
  //                2500.00,   2500.00,   2500.00,   2500.00,   2500.00 };
  AbsLPbWO4 = new TSplineFit("AbsorptionLength_crys8981R04","Absorption Length | CMS crystal 8981 meas 10/04/2002",
    5,M,x,y,320.0,850.0);
  //                           5,M,m,x,y,s,kTRUE,2.0,kTRUE,11000.0,320.0,850.0,kFALSE);
  AbsLPbWO4->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  AbsLPbWO4->SetMacro("AbsorptionLength_crys8981R04.C");
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
