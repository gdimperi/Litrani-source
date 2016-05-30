TSplineFit* AbsorptionLength_Silicium(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   AbsorptionLength_Silicium
//
//   Absorption length of silicium as a function of wavelength in [nm]
//
//   Source : Jean-Pierre Pansart, private communication.
//
//   Old f_abslsilicium
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 10;
  const Int_t m = 3;
  TSplineFit *AbsLSilicium;
  Double_t x[M]= { 400,       420,       450,       500,       530,
    570,       600,       630,       675,       740     };
  Double_t y[M]= { 0.000016,  0.00003,   0.00007,   0.0001,    0.00013,  
    0.00019,   0.00023,   0.0003,    0.0004,    0.00063 };
  Double_t s11[M]= { 0.0000016, 0.000003,  0.000007,  0.00001,   0.000013, 
    0.000019,  0.000023,  0.00003,   0.00004,   0.000063};
  AbsLSilicium = new TSplineFit("AbsorptionLength_Silicium","Absorption Length | Silicium",
    5,M,m,x,y,s11,kTRUE,0.0,kTRUE,0.001,300.0,1000.0,kFALSE);
  AbsLSilicium->SetSource("Jean-Pierre Pansart, private communication");
  AbsLSilicium->SetMacro("AbsorptionLength_Silicium.C");
  AbsLSilicium->SetXLabel("wavelength [nm]");
  AbsLSilicium->SetVLabel("absorption length [cm]");
  if (todraw) {
    AbsLSilicium->SetDefaultLabels();
    AbsLSilicium->DrawFit();
    AbsLSilicium->Print();
  }
  if (infile) {
    if (firstinfile) AbsLSilicium->UpdateFile(kTRUE);
    else             AbsLSilicium->UpdateFile(kFALSE);
  }
  return AbsLSilicium;
}
