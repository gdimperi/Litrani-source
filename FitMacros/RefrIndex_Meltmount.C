TSplineFit* RefrIndex_Meltmount(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RefrIndex_Meltmount
//
//   Description :
//
//   Refraction index of Glue as a function of wavelength in [nm]
//
//   Source : R. Chipeaux from a document from :
//            Cargille Labs : Melt Mount Code 5870
//
//   OLD f_refriglue
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 12;
  const Int_t m = 4;
  Int_t i;
  Double_t sig[M];
  TSplineFit *RefrIMeltmount;
  Double_t x[M] = { 400.0,  404.7,  450.0,  480.0,  486.1,  546.1,  589.3,  632.8,  643.9,  656.3,
    694.3,  840.0  };
  Double_t y[M] = { 1.7590, 1.7565, 1.7366, 1.7268, 1.7250, 1.7112, 1.7040, 1.6983, 1.6971, 1.6957,
    1.6921, 1.6828 };
  for (i=0;i<M;i++) sig[i] = 0.01;
  RefrIMeltmount = new TSplineFit("RefrIndex_Meltmount","Index of refraction | Glue Meltmount",
    1,M,m,x,y,sig,kTRUE,1.0,kFALSE,2.0,300.0,1000.0);

  RefrIMeltmount->SetSource("Cargille Labs : Melt Mount Code 5870");
  RefrIMeltmount->SetMacro("RefrIndex_Meltmount.C");
  RefrIMeltmount->SetXLabel("wavelength [nm]");
  RefrIMeltmount->SetVLabel("Refraction index");
  if (todraw) {
    RefrIMeltmount->SetDefaultLabels();
    RefrIMeltmount->DrawFit();
    RefrIMeltmount->Print();
  }
  if (infile) {
    if (firstinfile) RefrIMeltmount->UpdateFile(kTRUE);
    else             RefrIMeltmount->UpdateFile(kFALSE);
  }
  return RefrIMeltmount;
}
