TSplineFit* RefrIndex_Nitrure(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   RefrIndex_Nitrure
//
//   Index of refraction for window of APD made of nitrure [x] = nm [y] = index
//
//   Source : Jean-Pierre Pansart ( obtained from Philips )
//
//   Old f_refrinitrure.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 7;
  const Int_t m = 4;
  Int_t i;
  Double_t sig[M];
  TSplineFit *RefrINitrure;
  Double_t x[M] = { 309.9,     354.2,     382.6,     413.3,     495.9,     619.8,     826.5 };
  Double_t y[M] = { 2.141,     2.099,     2.085,     2.066,     2.041,     2.022,     2.008 };
  for (i=0;i<M;i++) sig[i] = 0.01;
  RefrINitrure = new TSplineFit("RefrIndex_Nitrure","Index of refraction | Window of APD : nitrure",
    1,M,m,x,y,sig,kTRUE,1.0,kFALSE,10.0,300.0,1000.0);
  RefrINitrure->SetSource("Jean-Pierre Pansart ( obtained from Philips )");
  RefrINitrure->SetMacro("RefrIndex_Nitrure.C");
  RefrINitrure->SetXLabel("wavelength [nm]");
  RefrINitrure->SetVLabel("Refraction index");
  RefrINitrure->SetParameter(0.0,"Window of APD");
  if (todraw) {
    RefrINitrure->SetDefaultLabels();
    RefrINitrure->DrawFit();
    RefrINitrure->Print();
  }
  if (infile) {
    if (firstinfile) RefrINitrure->UpdateFile(kTRUE);
    else             RefrINitrure->UpdateFile(kFALSE);
  }
  return RefrINitrure;
}
