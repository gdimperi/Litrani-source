TSplineFit* PhotoEl_Strontium(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   Photo-Electric Effect Cross Section for Strontium
//
//   Source: NIST http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 38;
  const Int_t m = 5;
  Int_t i;
  TSplineFit *PE_Strontium;
  Double_t x[M] = { 1.00E-03,        1.50E-03,        1.94E-03,        1.94E-03,        2.00E-03,
                    2.01E-03,        2.01E-03,        2.11E-03,        2.22E-03,        2.22E-03,
                    3.00E-03,        4.00E-03,        5.00E-03,        6.00E-03,        8.00E-03,
                    1.00E-02,        1.50E-02,        1.61E-02,        1.61E-02,        2.00E-02,
                    3.00E-02,        4.00E-02,        5.00E-02,        6.00E-02,        8.00E-02,
                    1.00E-01,        1.50E-01,        2.00E-01,        3.00E-01,        4.00E-01,
                    5.00E-01,        6.00E-01,        8.00E-01,        1.00E+00,        1.02E+00,
                    1.25E+00,        1.50E+00,        2.00E+00 };

  Double_t y[M] = { 5.0754E+05,      1.9487E+05,      4.1592E+05,      4.1592E+05,      3.7520E+05,
                    5.1918E+05,      5.1918E+05,      4.6246E+05,      4.1301E+05,      4.1301E+05,
                    2.2105E+05,      1.0558E+05,      5.8462E+04,      3.5775E+04,      1.6288E+04,
                    8.8274E+03,      2.8358E+03,      2.3123E+03,      2.3123E+03,      9.1474E+03,
                    3.0540E+03,      1.3656E+03,      7.2277E+02,      4.2610E+02,      1.8324E+02,
                    9.5110E+01,      2.8504E+01,      1.2129E+01,      3.7520E+00,      1.6870E+00,
                    9.3655E-01,      5.9480E-01,      3.0540E-01,      1.9051E-01,      1.8033E-01,
                    1.2201E-01,      8.7693E-02,      5.3663E-02 };
  Double_t s[M];
  for (k1=0;k1<M;k1++) s[k1] = 0.1*y[k1];
  PE_Strontium = new TSplineFit("PhotoEl_Strontium","Photo-Electric Cross Section | Strontium",
                                 18,M,x,y,0.001,2.0);
  //                                 18,M,x,y);
  //  PE_Strontium = new TSplineFit("PhotoEl_Strontium","Photo-Electric Cross Section | Strontium",
  //				18,M,m,x,y,s,kTRUE,0.0001,kTRUE,60000.0);
  PE_Strontium->SetSource("http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html");
  PE_Strontium->SetMacro("PhotoEl_Strontium.C");
  PE_Strontium->SetXLabel("Gamma Energy [MeV]");
  PE_Strontium->SetVLabel("Cross Section x10+24 cm2");
  if (todraw) {
    PE_Strontium->SetDefaultLabels();
    PE_Strontium->DrawFit();
    gTwoPad->SetPadLogX(1);
    PE_Strontium->Print();
  }
  if (infile) {
    if (firstinfile) PE_Strontium->UpdateFile(kTRUE);
    else             PE_Strontium->UpdateFile(kFALSE);
  }
  return PE_Strontium;
}
