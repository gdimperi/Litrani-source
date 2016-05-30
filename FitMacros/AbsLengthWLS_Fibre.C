TSplineFit* AbsLengthWLS_Fibre(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AbsLengthWLS_Fibre
//
//  Absorption length (or diffusion length,as you like) of Plastic Fibre with wavelength shifting
//
//  Source : David Wahl
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 10;
  const Int_t m = 1;
  Int_t i;
  TSplineFit *WLSLength;
  Double_t x[M]= { 200, 300, 400, 450, 500,  600,  700, 800, 900, 990 };
  Double_t y[M]= { 0.0, 0.1, 0.1, 0.1, 1.E6, 1.E6, 0.0, 0.0, 0.0, 0.0 };
  WLSLength = new TSplineFit("AbsLengthWLS_Fibre","Absorption Length for WLS | Fibre with WLS",20, M, x, y, 200.0, 990.0);
  WLSLength->SetSource("Not real");
  WLSLength->SetMacro("AbsLengthWLS_Fibre.C");
  WLSLength->SetXLabel("Wavelength [nm]");
  WLSLength->SetVLabel("Absorption length with WLS [cm]");
  if (todraw) {
    WLSLength->SetDefaultLabels();
    WLSLength->DrawFit();
    WLSLength->Print();
  }
  if (infile) {
    if (firstinfile) WLSLength->UpdateFile(kTRUE);
    else             WLSLength->UpdateFile(kFALSE);
  }
  return WLSLength;
}
