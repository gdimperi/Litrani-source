TSplineFit* IIndexRev_vm2000(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   IIndexRev_vm2000
//
//   Imaginary part of index of refraction for vm2000 [x] = nm [y] = index
//
//   Im parts taken from aluminium. Real parts adjusted to give the albedo of vm2000, see paper SED Glast N5600038PA of Remi Chipaux.
//
//   Old f_imvm2000.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 29;
  const Int_t m = 4;
  Int_t i;
  Double_t sig[M];
  TSplineFit *IIndexRevVM2000;
  Double_t x[M] = {   300, 317.9, 335.1,   350, 364.7, 387.5, 413.3, 442.8, 459.2, 495.9,
    506.1, 527.6,   550, 563.6, 590.4, 604.8, 635.8, 652.6, 688.8, 708.5,
    750, 774.9,   825,   850,   875,   900,   925,   950, 991.9 };
  Double_t y[M] = {  3.61,  3.84,  4.06,  4.24,  4.43,  4.71,  5.02,  5.38,  5.58,  6.03,
    6.15,  6.42,  6.69,  6.85,  7.15,  7.31,  7.65,  7.82,  8.21,  8.39,
    8.62,  8.60,  8.31,  8.22,  8.18,  8.30,  8.49,  8.88,  9.49 };
  for (i=0;i<M;i++) sig[i] = 0.03;
  IIndexRevVM2000 = new TSplineFit("IIndexRev_vm2000","Im.  part of refraction index | VM2000",
    4,M,m,x,y,sig,kTRUE,0.0,kFALSE,10.0,300.0,1000.0);
  IIndexRevVM2000->SetSource("Im parts taken from aluminium. Real parts adjusted to give the albedo of vm2000, see paper SED Glast N5600038PA of Remi Chipaux.");
  IIndexRevVM2000->SetMacro("IIndexRev_vm2000.C");
  IIndexRevVM2000->SetXLabel("wavelength [nm]");
  IIndexRevVM2000->SetVLabel("imaginary part index");
  if (todraw) {
    IIndexRevVM2000->SetDefaultLabels();
    IIndexRevVM2000->DrawFit();
    IIndexRevVM2000->Print();
  }
  if (infile) {
    if (firstinfile) IIndexRevVM2000->UpdateFile(kTRUE);
    else             IIndexRevVM2000->UpdateFile(kFALSE);
  }
  return IIndexRevVM2000;
}
