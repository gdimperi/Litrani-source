TSplineFit* RefrIndex_Silicium(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RefrIndex_Silicium
//
//   Refraction index of Silicium as a function of wavelength in [nm]
//   Source : Landolt-Bernstein volume 17
//
//   Old f_refrisilicium.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 35;
  const Int_t m = 2;
  Int_t i;
  Double_t sig[M];
  TSplineFit *RefrISilicium;
  Double_t x[M] = { 305, 310, 320, 330, 340, 350, 360, 370, 380, 390, 
    400, 425, 450, 475, 500, 525, 550, 575, 600, 625, 
    650, 675, 700, 725, 750, 775, 800, 825, 850, 875, 
    900, 925, 950, 975, 995  };
  Double_t y[M] = { 4.8,   4.9,   5.0,   5.0,   5.2,   5.4,   6.5,   7.0,   6.3,   5.8,   
    5.2,   4.95,  4.7,   4.45,  4.2,   4.125, 4.05,  3.975, 3.9,   3.875, 
    3.85,  3.825, 3.8,   3.7875,3.775, 3.7625,3.75,  3.725, 3.7,   3.675, 
    3.65,  3.625, 3.6,   3.575, 3.55 };
  for (i=0;i<M;i++) sig[i] = 0.2;
  RefrISilicium = new TSplineFit("RefrIndex_Silicium","Index of refraction | Silicium of APD",
    1,M,m,x,y,sig,kTRUE,1.0,kFALSE,10.0,300.0,1000.0);
  RefrISilicium->SetSource("Landolt-Bernstein volume 17");
  RefrISilicium->SetMacro("RefrIndex_Silicium.C");
  RefrISilicium->SetXLabel("wavelength [nm]");
  RefrISilicium->SetVLabel("Refraction index");
  if (todraw) {
    RefrISilicium->SetDefaultLabels();
    RefrISilicium->DrawFit();
    RefrISilicium->Print();
  }
  if (infile) {
    if (firstinfile) RefrISilicium->UpdateFile(kTRUE);
    else             RefrISilicium->UpdateFile(kFALSE);
  }
  return RefrISilicium;
}
