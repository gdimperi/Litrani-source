TSplineFit* RadDamage_Kim02(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  RadDamage_Kim02
//
//  Derived from AbsorptionLength_crys8981R03.
//  Describes the part induced by irradiation in the inverse of the absorption
//  length. It is after the 2nd irradiation in GIF. Measurements of the 2/4/2002.
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
  Double_t y[M]= {  0.008953072,  0.036658184,  0.005267399,  0.001261783, -0.000255693, -3.61691E-05,
    5.35254E-05,  0.000235154,  0.000244864,  0.000296013,  0.000242575,  0.000249018,
    0.000203777,  0.000268011,  0.000223700,  0.000230337,  0.000176392,  0.001075064,
    0.000141282,  0.000167489,  0.000184641,  0.000197779,  5.43631E-05,  0.000217112,
    0.000199392,  0.000248693,  0.000129031,  7.15174E-05,  9.59507E-05,  0.000218467,
    2.52504E-05,  0.000151667,  8.45120E-05,  0.000103500,  0.000127338,  7.13212E-05,
    -1.10482E-05,  7.48015E-05,  0.000217360,  0.000228382,  7.96582E-05, -4.54715E-05,
    0.000265583,  0.000265583,  0.000265583 };
  //Double_t s21[M]= {  0.379860883,  0.408153390,  0.097396314,  0.001522869,  0.000684734,  0.000533817,
  //                  0.000489241,  0.000493036,  0.000527100,  0.000563422,  0.000599735,  0.000704460,
  //                  0.000663741,  0.001551995,  0.047073650,  4.541610641,  0.001171856,  2.121145259,
  //                  0.000885458,  0.005411403,  0.263453173,  0.604181830,  0.465378061,  0.650054755,
  //                  0.213343916,  0.028640058,  0.000294583,  0.005803458,  0.004429112,  0.010396178,
  //                  0.263035491,  0.265250415,  0.678885852,  0.000863556,  0.000675764,  0.000794584,
  //                  0.001176951,  0.001554862,  0.001139539,  0.005794866,  0.009341345,  0.001103592,
  //                  0.001002535,  0.001002535,  0.001002535 };
  Double_t s21[M];
  for (i=0;i<M;i++) s21[i] = 0.001;
  damage = new TSplineFit("RadDamage_Kim02","Radiation damages in shape | Kimax after 2nd irradiation",
    15,M,x,y,320.0,850.0);
  //                           15,M,m,x,y,s21,kFALSE,0.0,kFALSE,0.0,320.0,850.0,kFALSE);
  damage->SetSource("Remi Chipaux");
  damage->SetMacro("RadDamage_Kim02.C");
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
