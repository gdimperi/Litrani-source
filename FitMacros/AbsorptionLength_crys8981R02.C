TSplineFit* AbsorptionLength_crys8981R02(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsorptionLength_crys8981R02
//
//  Description :
//
//  Longueur d'absorption PbWO4 (cristal CMS 8981 apres 1ere irrad)
//  mesures du 22/03/2002
//  moyenne de l'absorption suivant les deux axes, en cm
//  wavelength [nm].
//
//  Source : Remi Chipaux
//
//  Old f_absmlpwo8981_02.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 46;
  //const Int_t m = 1;
  Int_t i;
  TSplineFit *AbsLPbWO4;
  Double_t x[M]= {320,     330,     340,     350,     360,     370,     380,     390,
    400,     410,     420,     430,     440,     450,     460,     470,
    480,     490,     500,     510,     520,     530,     540,     550,
    560,     570,     580,     590,     600,     610,     620,     630,
    640,     650,     660,     670,     680,     690,     700,     710,
    720,     730,     740,     750,     800,     850 };
  Double_t y[M]= {    2.50,     2.43,     2.42,     4.22,    16.96,    67.42,   174.77,   288.68,
    364.44,   426.80,   516.60,   623.75,   735.95,   844.72,  1051.05,  1452.61,
    1264.63,  1314.25,  2071.33, 10000.00, 10000.00, 10000.00, 10000.00,  4480.75,
    7530.78, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00 };
  //Double_t s[M]= {   1.64,     1.64,     1.63,     1.27,     0.32,     2.37,    12.15,    31.34,
  //                  51.01,    74.78,   113.47,   176.57,   330.02,   448.18,  1257.70,   384.20,
  //                 316.16,   328.56,   517.83,  2500.00,  2500.00,  2500.00,  2500.00,  1120.19,
  //                1882.69,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,
  //                2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,
  //                2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00 };
  AbsLPbWO4 = new TSplineFit("AbsorptionLength_crys8981R02","Absorption Length | CMS crystal 8981 after 1st irradiation",
    5,M,x,y,320.0,850.0);
  //                           5,M,m,x,y,s,kTRUE,2.4,kTRUE,10000.0,320.0,850.0,kFALSE);
  AbsLPbWO4->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  AbsLPbWO4->SetMacro("AbsorptionLength_crys8981R02.C");
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
