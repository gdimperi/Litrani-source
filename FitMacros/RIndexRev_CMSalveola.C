TSplineFit* RIndexRev_CMSalveola(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RIndexRev_CMSalveola
//
//   Real part of index of refraction for the revetment of the CMS alveola. The albedo of
//this revetment has been kindly provided by Paul Baillon from CERN. We have tried to
//reproduce it using the imaginary part of the index for aluminium and varying the real
//part of the index until it gives the right albedo.
//
//   Source : Paul Baillon CERN
//
//   Old f_realrevalveola.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 14;
  const Int_t m = 2;
  Int_t i;
  Double_t sig[M];
  TSplineFit *RIndexRevCaps;
  Double_t x[M] = { 300,    350,    400,    450,    500,    550,    600,    650,    700,    750,
    800,    850,    900,    950 };
  Double_t y[M] = { 0.568,  0.606,  0.682,  0.807,  0.974,  1.153,  1.435,  1.779,  2.180,  2.830,
    2.920,  3.048,  3.630,  5.516 };
  for (i=0;i<M;i++) sig[i] = 0.06;
  RIndexRevCaps = new TSplineFit("RIndexRev_CMSalveola","Real part of refraction index | CMS alveola",
    3,M,m,x,y,sig,kTRUE,0.0,kFALSE,10.0,300.0,1000.0);
  RIndexRevCaps->SetSource("Paul Baillon CMS CERN");
  RIndexRevCaps->SetMacro("RIndexRev_CMSalveola.C");
  RIndexRevCaps->SetXLabel("wavelength [nm]");
  RIndexRevCaps->SetVLabel("real part index");
  if (todraw) {
    RIndexRevCaps->SetDefaultLabels();
    RIndexRevCaps->DrawFit();
    RIndexRevCaps->Print();
  }
  if (infile) {
    if (firstinfile) RIndexRevCaps->UpdateFile(kTRUE);
    else             RIndexRevCaps->UpdateFile(kFALSE);
  }
  return RIndexRevCaps;
}
