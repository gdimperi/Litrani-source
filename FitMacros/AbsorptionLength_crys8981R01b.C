TSplineFit* AbsorptionLength_crys8981R01b(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsorptionLength_crys8981R01b
//
//  Description :
//
//  Longueur d'absorption PbWO4 (cristal CMS 8981 initial)
//  mesures du 19/03/2002 (avant irradiation) : état initial
//  moyenne de l'absorption suivant les deux axes, en cm
//  wavelength [nm]. Valeurs divisées par 10.
//
//  Source : Remi Chipaux
//
//  Old f_absmlpwo8981_01b.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 45;
  //const Int_t m = 2;
  Int_t i;
  TSplineFit *AbsLPbWO4;
  Double_t x[M]= {330,    340,    350,    360,    370,    380,    390,    400,
    410,    420,    430,    440,    450,    460,    470,    480,
    490,    500,    510,    520,    530,    540,    550,    560,
    570,    580,    590,    600,    610,    620,    630,    640,
    650,    660,    670,    680,    690,    700,    710,    720,
    730,    740,    750,    800,    850  };
  Double_t y[M]= {  0.250,   0.249,   0.436,   1.756,   6.722,  16.704,  27.566,  35.294,
    43.106,  53.946,  61.560,  72.640,  77.655, 122.873, 156.746, 151.879,
    125.557, 200.0,   200.0,   200.0,   200.0,   200.0,   200.0,   200.0,
    200.0,   200.0,   200.0,   200.0,   200.0,   200.0,   200.0,   200.0,
    200.0,   200.0,   200.0,   200.0,   200.0,   200.0,   200.0,   200.0,
    200.0,   200.0,   200.0,   200.0,   200.0 };
  //Double_t s[M]= {  0.164,   0.163,   0.129,   0.032,   0.206,   0.957,   2.412,   3.978,
  //                  5.761,   9.507,  12.758,  17.942,  21.066,  57.640, 118.458, 130.500,
  //                 80.331,  10.00,   10.00,   10.00,   10.00,   10.00,   10.00,   10.00, 
  //                 10.00,   10.00,   10.00,   10.00,   10.00,   10.00,   10.00,   10.00, 
  //                 10.00,   10.00,   10.00,   10.00,   10.00,   10.00,   10.00,   10.00, 
  //                 10.00,   10.00,   10.00,   10.00,   10.00   };
  AbsLPbWO4 = new TSplineFit("AbsorptionLength_crys8981R01b","Absorption Length | CMS crystal 8981 before irradiation 2nd",
    5,M,x,y,320.0,800.0);
  //                           5,M,m,x,y,s,kTRUE,0.0,kTRUE,200.0,320.0,800.0,kFALSE);
  AbsLPbWO4->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  AbsLPbWO4->SetMacro("AbsorptionLength_crys8981R01b.C");
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
