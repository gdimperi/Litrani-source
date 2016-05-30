TSplineFit* AbsorptionLength_crys8981R01(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsorptionLength_crys8981R01
//
//  Description :
//
//  Longueur d'absorption PbWO4 (cristal CMS 8981 initial)
//  mesures du 19/03/2002 (avant irradiation) : état initial
//  moyenne de l'absorption suivant les deux axes, en cm
//  wavelength [nm].
//
//  Source : Remi Chipaux
//
//  Old f_absmlpwo8981_01.C
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
  Double_t x[M]= { 330,    340,    350,    360,    370,    380,    390,    400,    410,    420,    
    430,    440,    450,    460,    470,    480,    490,    500,    510,    520,
    530,    540,    550,    560,    570,    580,    590,    600,    610,    620,
    630,    640,    650,    660,    670,    680,    690,    700,    710,    720,
    730,    740,    750,    800,    850 };
  Double_t y[M]= {     2.50,     2.49,     4.36,    17.56,    67.22,   167.04,   275.66,   352.94,
    431.06,   539.46,   615.60,   726.40,   776.55,  1228.73,  1567.46,  1518.79,
    1255.57, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00, 10000.00,
    10000.00, 10000.00, 10000.00, 10000.00, 10000.00 };
  //Double_t s[M]= {    1.64,     1.63,     1.29,     0.32,     2.06,     9.57,    24.12,    39.78,
  //                   57.61,    95.07,   127.58,   179.42,   210.66,   576.40,  1184.58,  1305.00,
  //                  803.31,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,
  //                 2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,
  //                 2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,  2500.00,
  //                 2500.00,  2500.00,  2500.00,  2500.00,  2500.00 };
  AbsLPbWO4 = new TSplineFit("AbsorptionLength_crys8981R01","Absorption Length | CMS crystal 8981 before irradiation",
    5,M,x,y,320.0,800.0);
  //                           5,M,m,x,y,s,kTRUE,0.0,kTRUE,10000.0,320.0,800.0,kFALSE);
  AbsLPbWO4->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  AbsLPbWO4->SetMacro("AbsorptionLength_crys8981R01.C");
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
