TSplineFit* RefrIndex_Silicoide(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   FIT RefrIndex_Silicoide
//
//   Refraction index of Silicoide as a function of wavelength in [nm]
//
//   Source : Value found in old fortran Litrani, without dependency upon wavelength
//
//   Old f_refrisilicoide
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 3;
  TSplineFit *RefrISilicoide;
  Double_t x[M] = { 300.0,   600.0,   1000.0 };
  Double_t y[M] = { 1.413,   1.413,    1.413 };
  RefrISilicoide = new TSplineFit("RefrIndex_Silicoide","Index of refraction | Silicoide",
    1,M,x,y,300.0,1000.0);
  RefrISilicoide->SetSource("Value found in old fortran Litrani, without dependency upon wavelength");
  RefrISilicoide->SetMacro("RefrIndex_Silicoide.C");
  RefrISilicoide->SetXLabel("wavelength [nm]");
  RefrISilicoide->SetVLabel("Refraction index");
  if (todraw) {
    RefrISilicoide->SetDefaultLabels();
    RefrISilicoide->DrawFit();
    RefrISilicoide->Print();
  }
  if (infile) {
    if (firstinfile) RefrISilicoide->UpdateFile(kTRUE);
    else             RefrISilicoide->UpdateFile(kFALSE);
  }
  return RefrISilicoide;
}
