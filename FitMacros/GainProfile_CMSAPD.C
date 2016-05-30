TSplineFit* GainProfile_CMSAPD(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  GainProfile_CMSAPD
//
//  Look at the web documentation of class TSupplShape, method SetAPD() to know what the
//   gain profile of an APD is and how to define it.
//
//  Source : Jean-Pierre Pansart DAPNIA/SPP CEA Saclay
//
//  OLD f_profileapd
//
{
Int_t k1;
Int_t k2 = -100;
k1 = TClassTable::GetID("TSplineFit");
if (k1<0) k2 = gSystem.Load("libSplineFit");
TString su1,su2,su3,su4;
const Int_t M = 7;
TSplineFit *GainProfileAPD;
Double_t x[M] = { 0.0,      200.0,    250.0,    2500.0,   5000.0,   90000.0,  100000.0 };
Double_t y[M] = { 0.0,        0.1,     58.0,      58.0,      2.0,       2.0 ,      0.0 };
GainProfileAPD = new TSplineFit("GainProfile_CMSAPD","Gain profile of APD or PIN | APD used in CMS",
                                 13,M,x,y,0.0,100000.0);
GainProfileAPD->SetSource("Jean-Pierre Pansart DAPNIA/SPP CEA Saclay");
GainProfileAPD->SetMacro("GainProfile_CMSAPD.C");
GainProfileAPD->SetXLabel("depth [nm]");
GainProfileAPD->SetVLabel("gain");
  if (todraw) {
    GainProfileAPD->SetDefaultLabels();
    GainProfileAPD->DrawFit();
    GainProfileAPD->Print();
  }
  if (infile) {
    if (firstinfile) GainProfileAPD->UpdateFile(kTRUE);
    else             GainProfileAPD->UpdateFile(kFALSE);
  }
  return GainProfileAPD;
}
