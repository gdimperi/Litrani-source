TSplineFit* RefrIndex_Sodocal(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RefrIndex_Sodocal
//
//
//   Refraction index for sodocalcic glass ( window of PM )
//
//   Source : Schott optical glass. Used for window of PM 2262B
//            given by Marie Geleoc
//
//   OLD f_refrisodocal
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 29;
  const Int_t m = 5;
  Int_t i;
  Double_t sig[M];
  TSplineFit *RefrISodocal;
  Double_t x[M] = { 300,   325,   350,   375,   400,   425,   450,   475,   500,   525,   
    550,   575,   600,   625,   650,   675,   700,   725,   750,   775,   
    800,   825,   850,   875,   900,   925,   950,   975,   1000  };
  Double_t y[M] = { 1.563, 1.554, 1.547, 1.542, 1.538, 1.535, 1.532, 1.530, 1.528, 1.526, 
    1.524, 1.523, 1.522, 1.521, 1.520, 1.519, 1.518, 1.518, 1.517, 1.517, 
    1.516, 1.516, 1.515, 1.515, 1.514, 1.514, 1.513, 1.513, 1.513  };
  for (i=0;i<M;i++) sig[i] = 0.04;
  RefrISodocal = new TSplineFit("RefrIndex_Sodocal","Index of refraction | Sodocalcic glass (PM window)",
    1,M,m,x,y,sig,kTRUE,1.0,kFALSE,1.6,300.0,1000.0);
  RefrISodocal->SetSource("Marie Geleoc: Schott optical glass used for window of PM 2262B");
  RefrISodocal->SetMacro("RefrIndex_Sodocal.C");
  RefrISodocal->SetXLabel("wavelength [nm]");
  RefrISodocal->SetVLabel("Refraction index");
  if (todraw) {
    RefrISodocal->SetDefaultLabels();
    RefrISodocal->DrawFit();
    RefrISodocal->Print();
  }
  if (infile) {
    if (firstinfile) RefrISodocal->UpdateFile(kTRUE);
    else             RefrISodocal->UpdateFile(kFALSE);
  }
  return RefrISodocal;
}
