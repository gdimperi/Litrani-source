TSplineFit* IIndexRev_CMSalveola(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   IIndexRev_CMSalveola
//
//   Im. part of index of refraction for the revetment of the CMS alveola. The albedo of
//this revetment has been kindly provided by Paul Baillon from CERN. We have tried to
//reproduce it using the imaginary part of the index for aluminium and varying the real
//part of the index until it gives the right albedo.
//
//   Source : Paul Baillon CMS CERN
//
//   Old f_imrevalveola.C
//
{
Int_t k1;
Int_t k2 = -100;
k1 = TClassTable::GetID("TSplineFit");
if (k1<0) k2 = gSystem.Load("libSplineFit");
const Int_t M = 14;
const Int_t m = 3;
Int_t i;
Double_t sig[M];
TSplineFit *IIndexRevCaps;
Double_t x[M] = { 300,  350,  400,  450,  500,  550,  600,  650,  700,  750,
                  800,  850,  900,  950 };
Double_t y[M] = { 3.61, 4.24, 4.86, 5.46, 6.08, 6.68, 7.25, 7.79, 8.31, 8.62,
                  8.46, 8.22, 8.30, 8.88 };
for (i=0;i<M;i++) sig[i] = 0.03;
IIndexRevCaps = new TSplineFit("IIndexRev_CMSalveola","Im.  part of refraction index | CMS alveola",
                            4,M,m,x,y,sig,kTRUE,0.0,kFALSE,10.0,300.0,1000.0);
IIndexRevCaps->SetSource("Paul Baillon CMS CERN");
IIndexRevCaps->SetMacro("IIndexRev_CMSalveola.C");
IIndexRevCaps->SetXLabel("wavelength [nm]");
IIndexRevCaps->SetVLabel("imaginary part index");
  if (todraw) {
    IIndexRevCaps->SetDefaultLabels();
    IIndexRevCaps->DrawFit();
    IIndexRevCaps->Print();
  }
  if (infile) {
    if (firstinfile) IIndexRevCaps->UpdateFile(kTRUE);
    else             IIndexRevCaps->UpdateFile(kFALSE);
  }
  return IIndexRevCaps;
}
