TSplineFit* GainProfile_GLASTpin(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  GainProfile_GLASTpin
//
//  Look at the web documentation of class TSupplShape, method SetAPD() to know what the
//   gain profile of an APD or a PIN diode is and how to define it.
//
//  Source : Philippe Bourgeois DAPNIA/SEDI CEA Saclay
//
//  OLD f_profilepin
//
{
Int_t k1;
Int_t k2 = -100;
k1 = TClassTable::GetID("TSplineFit");
if (k1<0) k2 = gSystem.Load("libSplineFit");
TString sv1,sv2,sv3,sv4;
const Int_t M = 7;
TSplineFit *GainProfilePIN;
Double_t x[M] = { 0.0,      200.0,    250.0,    2500.0,   5000.0,   90000.0,  100000.0 };
Double_t y[M] = { 0.0,        0.2,      1.0,       1.0,      1.0,       1.0 ,      0.0 };
GainProfilePIN = new TSplineFit("GainProfile_GLASTpin","Gain profile of APD or PIN | PIN used in GLAST",
                                 13,M,x,y,0.0,100000.0);
GainProfilePIN->SetSource("Philippe Bourgeois DAPNIA/SEDI CEA Saclay");
GainProfilePIN->SetMacro("GainProfile_GLASTpin.C");
GainProfilePIN->SetXLabel("depth [nm]");
GainProfilePIN->SetVLabel("gain");
  if (todraw) {
    GainProfilePIN->SetDefaultLabels();
    GainProfilePIN->DrawFit();
    GainProfilePIN->Print();
  }
  if (infile) {
    if (firstinfile) GainProfilePIN->UpdateFile(kTRUE);
    else             GainProfilePIN->UpdateFile(kFALSE);
  }
  return GainProfilePIN;
}
