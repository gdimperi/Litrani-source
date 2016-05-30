TSplineFit* RefrIndex_hcp200(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RefrIndex_hcp200
//
//   Refraction index of Silice/Polymere HCP 200 done by Spectranb, material of the
//optical fibre used for the crystal of CMS. For the time being, we only know that the
//index is 1.46 at 632 nm. We will try to find more information.
//
//   Source : M. Anfreville
//
//   Old f_refrihcp200.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 3;
  TSplineFit *RefrIhcp200;
  Double_t x[M] = { 300.0,   632.0,   1000.0 };
  Double_t y[M] = {   1.46,    1.46,     1.46 };
  RefrIhcp200 = new TSplineFit("RefrIndex_hcp200","Index of refraction | hcp200",
    1,M,x,y,300.0,1000.0);
  RefrIhcp200->SetSource("M. Anfreville DAPNIA CEA Saclay");
  RefrIhcp200->SetMacro("RefrIndex_hcp200.C");
  RefrIhcp200->SetXLabel("wavelength [nm]");
  RefrIhcp200->SetVLabel("Refraction index");
  if (todraw) {
    RefrIhcp200->SetDefaultLabels();
    RefrIhcp200->DrawFit();
    RefrIhcp200->Print();
  }
  if (infile) {
    if (firstinfile) RefrIhcp200->UpdateFile(kTRUE);
    else             RefrIhcp200->UpdateFile(kFALSE);
  }
  return RefrIhcp200;
}
