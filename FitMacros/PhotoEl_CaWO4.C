TSplineFit* PhotoEl_CaWO4(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   Photo-Electric Effect Cross Section for CaWO4
//
//   Source: NIST http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 27;
  Int_t i;
  TSplineFit *PE_CaWO4;
  Double_t x[M] = { 0.001, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 
    0.09,  0.1,  0.15, 0.2,  0.3,  0.4,  0.5,  0.6,  0.7, 
    0.8,   0.9,  1.0,  1.02, 1.1,  1.2,  1.25, 1.3,  1.4};

  Double_t y[M] = { 1940000, 35100, 20421, 6823, 3114, 1677, 1009, 3272,
    2297,    1687,  1280,  430,  197,  66.8, 31.8, 18.4,
    11.9,    8.41,  6.30,  4.90, 3.95, 3.78, 3.26, 2.75,
    2.54,    2.37,  2.06};
  PE_CaWO4 = new TSplineFit("PhotoEl_CaWO4","Photo-Electric Cross Section | CaWO4",18,M,x,y,0.001, 1.4);
  PE_CaWO4->SetSource("http://physics.nist.gov/PhysRefData/Xcom/Text/XCOM.html");
  PE_CaWO4->SetMacro("PhotoEl_CaWO4.C");
  PE_CaWO4->SetXLabel("Gamma Energy [MeV]");
  PE_CaWO4->SetVLabel("Cross Section x10+24 cm2");
  if (todraw) {
    PE_CaWO4->SetDefaultLabels();
    PE_CaWO4->DrawFit();
    gTwoPad->SetPadLogX(1);
    PE_CaWO4->Print();
  }
  if (infile) {
    if (firstinfile) PE_CaWO4->UpdateFile(kTRUE);
    else             PE_CaWO4->UpdateFile(kFALSE);
  }
  return PE_CaWO4;
}
