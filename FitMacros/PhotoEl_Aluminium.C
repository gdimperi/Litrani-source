TSplineFit* PhotoEl_Aluminium(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
//
// Arguments:
//
//  todraw       : true if the fit is to be drawn
//  infile       : true if the fit is to be put in file SplineFitDB.rdb
//  firstinfile  : if true, [BE CAREFUL: RISK OF LOOSING FILE SplineFitDB.rdb ]
//                 delete all fits present in file SplineFitDB.rdb and place
//                 this fit as the first fit in a new file SplineFitDB.rdb
//                 default false !!!
//  All defaults for arguments correspond to the case where one calls this CINT
// macro from within a Litrani CINT macro to define a needed fit, instead of trying
// to find it in the database file SplineFitDB.rdb using TSplineFit::FindFit()
//
//   Photo-Electric Effect Cross Section for Aluminium
//
//   Source: NIST http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 31;
  const Int_t m = 5;
  Int_t i;
  TSplineFit *PE_Aluminium;
  Double_t x[M] = {   0.001,  0.0015,      0.001559,   0.00156,      0.002,      0.003,
		      0.004,   0.005,         0.006,     0.008,       0.01,      0.015,
		      0.02,     0.03,          0.04,      0.05,       0.06,       0.08,
		      0.1,      0.15,           0.2,       0.3,        0.4,        0.5,
                      0.6,       0.8,           1.0,     1.022,       1.25,        1.5,
                      2.0 };

  Double_t y[M] = { 52843.0,   17913.0,     16122.0,  177340.0,   101210.0,    35244.0,
                    16077.0,    8598.3,      5105.2,    2216.7,     1146.4,     336.32,
		    138.83,      39.05,      15.674,    7.7026,     4.2812,     1.6928,
		    0.824,     0.22347,    0.089565,  0.025705,   0.011106,  0.0060009,
                    0.0037617, 0.0019033, 0.0011823, 0.0011151, 0.00075683, 0.00054635,
                    0.00034169 };
  Double_t s[M];
  for (k1=0;k1<M;k1++) s[k1] = 0.1*y[k1];
  PE_Aluminium = new TSplineFit("PhotoEl_Aluminium","Photo-Electric Cross Section | Aluminium",
                                 18,M,x,y,0.001,2.0);
  //                                 18,M,x,y);
  //  PE_Aluminium = new TSplineFit("PhotoEl_Aluminium","Photo-Electric Cross Section | Aluminium",
  //				18,M,m,x,y,s,kTRUE,0.0001,kTRUE,60000.0);
  PE_Aluminium->SetSource("http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html");
  PE_Aluminium->SetMacro("PhotoEl_Aluminium.C");
  PE_Aluminium->SetXLabel("Gamma Energy [MeV]");
  PE_Aluminium->SetVLabel("Cross Section x10+24 cm2");
  if (todraw) {
    PE_Aluminium->SetDefaultLabels();
    PE_Aluminium->DrawFit();
    gTwoPad->SetPadLogX(1);
    PE_Aluminium->Print();
  }
  if (infile) {
    if (firstinfile) PE_Aluminium->UpdateFile(kTRUE);
    else             PE_Aluminium->UpdateFile(kFALSE);
  }
  return PE_Aluminium;
}
