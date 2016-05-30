TSplineFit* RefrIndex_RTV3145(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RefrIndex_RTV3145
//
//   Refraction index of Glue RTV 3145 a function of wavelength in [nm]
//
//   Source : Marco Montecchi            Phone: +39 06 3048 3587
//            Research physicist INN/TEC/IRR
//            ENEA Casaccia, Via Anguillarese 301
//            00060 S. Maria di Galeria (Roma) 
//            ITALY
//
//   OLD f_refrirtv3145.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 18;
  const Int_t m = 4;
  Int_t i;
  TSplineFit *RefrIRTV3145;
  Double_t x[M] = {300.0,    325.1,    350.0,    375.0,    400.8,    425.1,    450.6,    475.1,
    500.0,    525.0,    549.7,    576.9,    600.0,    625.0,    652.2,    677.4,
    700.0,    999.0 };
  Double_t y[M] = {1.701,    1.647,    1.598,    1.558,    1.523,    1.494,    1.472,    1.448,
    1.436,    1.424,    1.413,    1.405,    1.401,    1.394,    1.386,    1.383,
    1.380,    1.300  };
  Double_t s01[M] = {0.0117,   0.0109,   0.0108,   0.0109,   0.0111,   0.0114,   0.0117,   0.0121,
    0.0123,   0.0125,   0.0127,   0.0129,   0.0130,   0.0132,   0.0135,   0.0135,
    0.0136,   0.0136  };
  RefrIRTV3145 = new TSplineFit("RefrIndex_RTV3145","Index of refraction | Glue RTV3145",
    1,M,m,x,y,s01,kTRUE,1.0,kFALSE,1.701,300.0,1000.0);
  RefrIRTV3145->SetSource("Marco Montecchi Research physicist INN/TEC/IRR");
  RefrIRTV3145->SetMacro("RefrIndex_RTV3145.C");
  RefrIRTV3145->SetXLabel("wavelength [nm]");
  RefrIRTV3145->SetVLabel("Refraction index");
  if (todraw) {
    RefrIRTV3145->SetDefaultLabels();
    RefrIRTV3145->DrawFit();
    RefrIRTV3145->Print();
  }
  if (infile) {
    if (firstinfile) RefrIRTV3145->UpdateFile(kTRUE);
    else             RefrIRTV3145->UpdateFile(kFALSE);
  }
  return RefrIRTV3145;
}
