TSplineFit* RefrIndex_CsITl(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  RefrIndex_CsITl
//
//  Refraction index of CsI as a function of wavelength in [nm]
//
//  Source : Handbook of Optical Constants
//  Old f_refricsitl.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 51;
  const Int_t m = 5;
  Int_t i;
  Double_t sig[1000];
  TSplineFit *RefrICsITl;
  Double_t x[1000] = { 300.0, 310.0, 320.0, 330.0, 340.0, 350.0, 360.0, 370.0, 380.0, 390.0,
    400.0, 410.0, 420.0, 430.0, 440.0, 450.0, 460.0, 470.0, 480.0, 490.0,
    500.0, 510.0, 520.0, 530.0, 540.0, 550.0, 560.0, 570.0, 580.0, 590.0,
    600.0, 620.0, 640.0, 660.0, 680.0, 700.0, 720.0, 740.0, 760.0, 780.0,
    800.0, 820.0, 840.0, 860.0, 880.0, 900.0, 920.0, 940.0, 960.0, 980.0,
    1000.0 };
  Double_t y[1000] = { 1.979, 1.956, 1.937, 1.921, 1.907, 1.894, 1.884, 1.874, 1.865, 1.858,
    1.851, 1.844, 1.839, 1.833, 1.829, 1.824, 1.820, 1.816, 1.813, 1.810,
    1.807, 1.804, 1.801, 1.799, 1.797, 1.795, 1.793, 1.791, 1.789, 1.787,
    1.786, 1.783, 1.780, 1.778, 1.776, 1.774, 1.772, 1.770, 1.769, 1.767,
    1.766, 1.765, 1.764, 1.763, 1.762, 1.761, 1.760, 1.760, 1.759, 1.758,
    1.757 };
  for (i=0;i<M;i++) sig[i] = 0.1;
  RefrICsITl = new TSplineFit("RefrIndex_CsITl","Index of refraction | CsI(Tl)",
    1,M,m,x,y,sig,kTRUE,1.0,kFALSE,10.0,300.0,1000.0);
  RefrICsITl->SetSource("Handbook of Optical Constants");
  RefrICsITl->SetMacro("RefrIndex_CsITl.C");
  RefrICsITl->SetXLabel("wavelength [nm]");
  RefrICsITl->SetVLabel("Refraction index");
  if (todraw) {
    RefrICsITl->SetDefaultLabels();
    RefrICsITl->DrawFit();
    RefrICsITl->Print();
  }
  if (infile) {
    if (firstinfile) RefrICsITl->UpdateFile(kTRUE);
    else             RefrICsITl->UpdateFile(kFALSE);
  }
  return RefrICsITl;
}
