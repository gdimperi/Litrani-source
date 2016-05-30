TSplineFit* PhotoEl_Air(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   Photo-Electric Effect Cross Section for Air
//
//   Source: NIST http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 55;
  const Int_t m = 5;
  Int_t i;
  TSplineFit *PE_Air;
  Double_t x[M] = { 1.00E-03,        1.25E-03,        1.50E-03,        1.75E-03,        2.00E-03,
                    2.50E-03,        3.00E-03,        3.50E-03,        4.00E-03,        4.50E-03,
                    5.00E-03,        5.50E-03,        6.00E-03,        7.00E-03,        8.00E-03,
                    9.00E-03,        1.00E-02,        1.25E-02,        1.50E-02,        1.75E-02,
                    2.00E-02,        2.50E-02,        3.00E-02,        3.50E-02,        4.00E-02,
                    4.50E-02,        5.00E-02,        5.50E-02,        6.00E-02,        7.00E-02,
                    8.00E-02,        9.00E-02,        1.00E-01,        1.25E-01,        1.50E-01,
                    1.75E-01,        2.00E-01,        2.50E-01,        3.00E-01,        3.50E-01,
                    4.00E-01,        4.50E-01,        5.00E-01,        5.50E-01,        6.00E-01,
                    7.00E-01,        8.00E-01,        9.00E-01,        1.00E+00,        1.01E+00,
                    1.02E+00,        1.14E+00,        1.25E+00,        1.50E+00,        2.00E+00 };

  Double_t y[M] = { 3.6754E+05,      2.0120E+05,      1.2152E+05,      7.8887E+04,      5.3886E+04,
                    2.8288E+04,      1.6534E+04,      1.0459E+04,      7.0122E+03,      4.9105E+03,
                    3.5659E+03,      2.6595E+03,      2.0319E+03,      1.2650E+03,      8.3370E+02,
                    5.7572E+02,      4.1336E+02,      2.0319E+02,      1.1355E+02,      6.9126E+01,
                    4.4822E+01,      2.1714E+01,      1.1953E+01,      7.1915E+00,      4.6316E+00,
                    3.1376E+00,      2.2212E+00,      1.6136E+00,      1.2152E+00,      7.2712E-01,
                    4.6715E-01,      3.1674E-01,      2.2312E-01,      1.0658E-01,      5.8867E-02,
                    3.5659E-02,      2.3208E-02,      1.1455E-02,      6.5540E-03,      4.1336E-03,
                    2.7989E-03,      2.0120E-03,      1.5140E-03,      1.1753E-03,      9.4027E-04,
                    6.3449E-04,      4.7412E-04,      3.8448E-04,      2.9483E-04,      2.8388E-04,
                    2.7391E-04,      2.1415E-04,      1.8626E-04,      1.3546E-04,      8.4963E-05 };
  Double_t s[M];
  for (k1=0;k1<M;k1++) s[k1] = 0.1*y[k1];
  PE_Air = new TSplineFit("PhotoEl_Air","Photo-Electric Cross Section | Air",
                                 18,M,x,y,0.001,2.0);
  //                                 18,M,x,y);
  //  PE_Air = new TSplineFit("PhotoEl_Air","Photo-Electric Cross Section | Air",
  //				18,M,m,x,y,s,kTRUE,0.0001,kTRUE,60000.0);
  PE_Air->SetSource("http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html");
  PE_Air->SetMacro("PhotoEl_Air.C");
  PE_Air->SetXLabel("Gamma Energy [MeV]");
  PE_Air->SetVLabel("Cross Section x10+24 cm2");
  if (todraw) {
    PE_Air->SetDefaultLabels();
    PE_Air->DrawFit();
    gTwoPad->SetPadLogX(1);
    PE_Air->Print();
  }
  if (infile) {
    if (firstinfile) PE_Air->UpdateFile(kTRUE);
    else             PE_Air->UpdateFile(kFALSE);
  }
  return PE_Air;
}
