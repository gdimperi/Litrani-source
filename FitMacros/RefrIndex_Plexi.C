TSplineFit* RefrIndex_Plexi(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RefrIndex_Plexi
//
//   Refraction index of plexi, taken as material of an optical fibre.
//
//   Source : R. Chipeaux from a document from :
//
//   Old f_refriplexi.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 23;
  const Int_t m = 6;
  Int_t i;
  Double_t sig[M];
  TSplineFit *RefrIPlexi;
  Double_t x[M] = { 302.1,   330.2,   334.1,   340.3,   346.6,   361.1,   365.0,   404.6,   435.8,   467.8,
    486.1,   508.6,   546.1,   577.0,   579.1,   587.5,   589.3,   643.8,   656.3,   667.8,
    706.5,   852.1,   894.3 };
  Double_t y[M] = { 1.487,   1.480,   1.480,   1.479,   1.477,   1.475,   1.475,   1.470,   1.467,   1.464,
    1.463,   1.462,   1.460,   1.459,   1.459,   1.458,   1.458,   1.457,   1.456,   1.456,
    1.455,   1.452,   1.452 };
  for (i=0;i<M;i++) sig[i] = 0.04;
  RefrIPlexi = new TSplineFit("RefrIndex_Plexi","Index of refraction | Plexi of optical fibre",
    1,M,m,x,y,sig,kTRUE,1.0,kFALSE,10.0,300.0,1000.0);
  RefrIPlexi->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  RefrIPlexi->SetMacro("RefrIndex_Plexi.C");
  RefrIPlexi->SetXLabel("wavelength [nm]");
  RefrIPlexi->SetVLabel("Refraction index");
  if (todraw) {
    RefrIPlexi->SetDefaultLabels();
    RefrIPlexi->DrawFit();
    RefrIPlexi->Print();
  }
  if (infile) {
    if (firstinfile) RefrIPlexi->UpdateFile(kTRUE);
    else             RefrIPlexi->UpdateFile(kFALSE);
  }
  return RefrIPlexi;
}
