TSplineFit* RIndexRev_Tyvek(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RIndexRev_Tyvek
//
//   Real part of index of refraction for Tyvek [x] = nm [y] = index
//
//   Source : A paper from Casaccia ( 6 november 95 ) give the albedo
// of Tyvek L1057D as a function of wavelength. We have taken for the
// imaginary part of the index of tyvek the same values as for aluminium
// and we have recalculated the real part of the index such that it
// gives the albedo in the Casaccia paper. The Casaccia paper give values
// only between 350 and 650 nm. Outside this range, we have taken the
// values for aluminium.
//
//   Old f_realtyvek.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 19;
  const Int_t m = 4;
  Int_t i;
  Double_t sig[M];
  TSplineFit *RIndexRevTyvek;
  Double_t x[M] = {   300,   350,   360,   370,   380,   390,   400,   450,   500,   550,
    600,   650,   700,   750,   800,   850,   900,   950,   992 };
  Double_t y[M] = { 0.276,   0.4,  0.38, 0.382,  0.41, 0.433, 0.456, 0.578,  0.72, 0.907,
    1.13,  1.32,   1.8,   2.4,   2.7,  2.61,  2.06,  1.49,  1.37 };
  for (i=0;i<M;i++) sig[i] = 0.03;
  RIndexRevTyvek = new TSplineFit("RIndexRev_Tyvek","Real part of refraction index | Tyvek",
    3,M,m,x,y,sig,kTRUE,0.0,kFALSE,10.0,300.0,1000.0);
  RIndexRevTyvek->SetSource("A paper from Casaccia in CMS");
  RIndexRevTyvek->SetMacro("RIndexRev_Tyvek.C");
  RIndexRevTyvek->SetXLabel("wavelength [nm]");
  RIndexRevTyvek->SetVLabel("real part index");
  if (todraw) {
    RIndexRevTyvek->SetDefaultLabels();
    RIndexRevTyvek->DrawFit();
    RIndexRevTyvek->Print();
  }
  if (infile) {
    if (firstinfile) RIndexRevTyvek->UpdateFile(kTRUE);
    else             RIndexRevTyvek->UpdateFile(kFALSE);
  }
  return RIndexRevTyvek;
}
