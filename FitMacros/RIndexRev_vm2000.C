TSplineFit* RIndexRev_vm2000(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RIndexRev_vm2000
//
//   Real part of index of refraction for vm2000 [x] = nm [y] = index
//
//   SOurce : Values adjusted to give the albedo of vm2000, according to paper SED Glast N5600038PA of Remi Chipaux.
//
//   Old f_realvm2000.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 29;
  Int_t i;
  TSplineFit *RIndexRevVM2000;
  Double_t x[M] = {  300,   325,   350,   375,   400,   425,   450,   475,   500,   525,
    550,   575,   600,   625,   650,   675,   700,   725,   750,   775,
    800,   825,   850,   875,   900,   925,   950,   975,  1000 };
  Double_t y[M] = {   8.0,   8.0,   8.0,   8.0,   0.6, 0.178,  0.18, 0.595,  0.87,  0.25,
    0.24,  0.19,  0.11,  0.24,   1.3,  0.26,  0.17,  0.23,  0.40,   1.2,
    0.6,   0.1,  0.08,  0.08,  0.08,  0.08,  0.08,  0.08,  0.08 };
  RIndexRevVM2000 = new TSplineFit("RIndexRev_vm2000","Real part of refraction index | VM2000",
    3,M,x,y,300.0,1000.0);
  RIndexRevVM2000->SetSource("Values adjusted to give the albedo of vm2000, according to paper SED Glast N5600038PA of Remi Chipaux.");
  RIndexRevVM2000->SetMacro("RIndexRev_vm2000.C");
  RIndexRevVM2000->SetXLabel("wavelength [nm]");
  RIndexRevVM2000->SetVLabel("real part index");
  if (todraw) {
    RIndexRevVM2000->SetDefaultLabels();
    RIndexRevVM2000->DrawFit();
    RIndexRevVM2000->Print();
  }
  if (infile) {
    if (firstinfile) RIndexRevVM2000->UpdateFile(kTRUE);
    else             RIndexRevVM2000->UpdateFile(kFALSE);
  }
  return RIndexRevVM2000;
}
