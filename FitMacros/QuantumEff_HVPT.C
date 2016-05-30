TSplineFit* QuantumEff_HVPT(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   QuantumEff_HVPT
//
//   Quantum efficiency of the photocathode of the Hamamatsu Vacuum Phototriode HVPT
//
//   Source : CMS NOTE 2003/011 D.Britton
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 29;
  const Int_t m = 2;
  Int_t i;
  Double_t sig[M];
  TSplineFit *hvpt;
  Double_t x[M] = { 300.0, 325.0, 350.0, 375.0, 400.0, 425.0, 450.0, 475.0, 500.0, 525.0,
    550.0, 575.0, 600.0, 625.0, 650.0, 675.0, 700.0, 725.0, 750.0, 775.0,
    800.0, 825.0, 850.0, 875.0, 900.0, 925.0, 950.0, 975.0, 1000.0  };
  Double_t y[M] = { 0.21,  0.21,  0.22,  0.21,  0.21,  0.20,  0.20,  0.17,  0.15,  0.11,
    0.07,  0.04,  0.01,  0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005,
    0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005 };
  for (i=0;i<M;i++) sig[i] = 0.01;
  hvpt = new TSplineFit("QuantumEff_HVPT","Quantum efficiency | Hamamatsu HVPT",
    10,M,m,x,y,sig,kTRUE,0.0,kFALSE,0.22,300.0,1000.0);
  hvpt->SetSource("CMS NOTE 2003/011 D.Britton");
  hvpt->SetMacro("QuantumEff_HVPT.C");
  hvpt->SetXLabel("wavelength [nm]");
  hvpt->SetVLabel("quantum efficiency");
  if (todraw) {
    hvpt->SetDefaultLabels();
    hvpt->DrawFit();
    hvpt->Print();
  }
  if (infile) {
    if (firstinfile) hvpt->UpdateFile(kTRUE);
    else             hvpt->UpdateFile(kFALSE);
  }
  return hvpt;
}
