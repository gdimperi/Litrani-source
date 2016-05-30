TSplineFit* NPLY_CaWO4(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   Non proportionnality of light yield for CaWO4
//
//   Source: Moszynski et al., Nucl. Instr. Meth. A, 553 (2005) 578-591
//
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 23;
  Int_t i;
  TSplineFit *NPLY_CaWO4;
  Double_t x[M] = { 0.0165, 0.0218, 0.0305, 0.0318, 0.0369, 0.0516, 0.0592,
    0.0628, 0.0805, 0.0837, 0.0871, 0.121,  0.258,  0.318,
    0.506,  0.564,  0.660,  0.823,  1.06,   1.17,   1.27,
    1.33,   1.76 };

  Double_t y[M] = { 0.806,  0.847,  0.889,  0.894,  0.919,  0.934,  0.942,
    0.948,  0.934,  0.941,  0.933,  0.974,  0.990,  0.994,
    0.986,  0.996,  1.00,   1.00,   1.00,   1.00,   0.995,
    1.00,   1.00 };
  NPLY_CaWO4 = new TSplineFit("NPLY_CaWO4","Non proportionality of light yield | CaWO4",17,M,x,y,0.0165, 1.76);
  NPLY_CaWO4->SetSource("Nucl. Instr. Meth. A, 553 (2005) 578-591");
  NPLY_CaWO4->SetMacro("NPLY_CaWO4.C");
  NPLY_CaWO4->SetXLabel("Gamma Energy [MeV]");
  NPLY_CaWO4->SetVLabel("Relative Light Yield/MeV");
  if (todraw) {
    NPLY_CaWO4->SetDefaultLabels();
    NPLY_CaWO4->DrawFit();
    NPLY_CaWO4->Print();
  }
  if (infile) {
    if (firstinfile) NPLY_CaWO4->UpdateFile(kTRUE);
    else             NPLY_CaWO4->UpdateFile(kFALSE);
  }
  return NPLY_CaWO4;
}
