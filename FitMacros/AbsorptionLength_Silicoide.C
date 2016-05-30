TSplineFit* AbsorptionLength_Silicoide(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   AbsorptionLength_Silicoide
//
//   Absorption length for the silicoide: the pastilla before the PM
//
//   Source : none, value taken at random
//
//   Old f_abslsilicoide
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 3;
  TSplineFit *AbsLSilicoide;
  Double_t x[M]= { 300.0, 600.0, 1000.0 };
  Double_t y[M]= {  50.0, 100.0,   50.0 };
  AbsLSilicoide = new TSplineFit("AbsorptionLength_Silicoide","Absorption Length | Silicoide",
    5,M,x,y,300.0,1000.0);
  AbsLSilicoide->SetSource("none, value taken at random");
  AbsLSilicoide->SetMacro("AbsorptionLength_Silicoide.C");
  AbsLSilicoide->SetXLabel("wavelength [nm]");
  AbsLSilicoide->SetVLabel("absorption length [cm]");
  if (todraw) {
    AbsLSilicoide->SetDefaultLabels();
    AbsLSilicoide->DrawFit();
    AbsLSilicoide->Print();
  }
  if (infile) {
    if (firstinfile) AbsLSilicoide->UpdateFile(kTRUE);
    else             AbsLSilicoide->UpdateFile(kFALSE);
  }
  return AbsLSilicoide;
}
