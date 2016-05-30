TSplineFit* RadDamage_Kim06(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  RadDamage_Kim06
//
//  Derived from AbsorptionLength_crys8981R07.
//  Describes the part induced by irradiation in the inverse of the absorption
//  length. It is after the 6th irradiation in GIF. Measurements of the 15/5/2002.
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
  Double_t x[M]= { 330,         340,         350,         360,         370,         380,
    390,         400,         410,         420,         430,         440,
    450,         460,         470,         480,         490,         500,
    510,         520,         530,         540,         550,         560,
    570,         580,         590,         600,         610,         620,
    630,         640,         650,         660,         670,         680,
    690,         700,         710,         720,         730,         740,
    750,         800,         850 };
  Double_t y[M]= { 0.091448505, -0.01763043, 0.017739785, 0.002569199, 0.001219667, 0.000995242,
    0.001189745, 0.001255161, 0.001320893, 0.001307761, 0.001325286, 0.001332219,
    0.001159105, 0.001371721, 0.001322879, 0.001165642, 0.000946349, 0.002012579,
    0.001372888, 0.001259055, 0.001107599,  0.00094807, 0.000836952, 0.000832202,
    0.00079172, 0.000837228, 0.000750979, 0.000656382, 0.000580195, 0.000664309,
    0.000639982, 0.000564206, 0.000638835,  0.00062551, 0.000716354,  0.00075658,
    0.000833443,  0.00075308, 0.000990102, 0.000896786, 0.000827465, 0.000896416,
    0.001188338, 0.001188338, 0.001188338  };
  //Double_t s[M]= { 0.474489195, 0.35614499 , 0.101553222, 0.001562109, 0.000698283, 0.000545717,
  //                 0.000495483, 0.000498234, 0.000517004, 0.000539521, 0.000567267, 0.000590587,
  //                 0.000581822, 0.000696486, 0.000894812, 0.000891559, 0.000755049, 0.001298756,
  //                 0.001726791, 0.002711761, 0.001134527, 0.001392067, 0.000734445, 0.001283887,
  //                 0.003192761, 0.000800477, 0.000987214, 0.00061394 , 0.000411503, 0.000316379,
  //                 0.000359162, 0.001141322, 0.010577541, 0.000863595, 0.000675809, 0.000794587,
  //                 0.001176951, 0.001646213, 0.001139539, 0.005794866, 0.009341366, 0.001104203,
  //                 0.001049246, 0.001049246, 0.001049246  };
  Double_t s25[M];
  for (i=0;i<M;i++) s25[i] = 0.001;
  damage = new TSplineFit("RadDamage_Kim06","Radiation damages in shape | Kimax after 6th irradiation",
    15,M,x,y,320.0,850.0);
  //                           15,M,m,x,y,s25,kFALSE,0.0,kFALSE,0.0,320.0,850.0,kFALSE);
  damage->SetSource("Remi Chipaux");
  damage->SetMacro("RadDamage_Kim06.C");
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
