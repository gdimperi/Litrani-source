TSplineFit* Spectrum_PbWO4Nb(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  Spectrum_PbWO4Nb.C
//
//  Description :
//
//  spectrum of PbWO4 doped with Niobium
//
//  Source : CMS ECAL Calorimeter TDR
//
{
Int_t k1;
Int_t k2 = -100;
k1 = TClassTable::GetID("TSplineFit");
if (k1<0) k2 = gSystem.Load("libSplineFit");
const Int_t M = 81;
const Int_t m = 6;
const Double_t zero  = 0.0;
const Double_t z05   = 0.5;
const Double_t Nphot = 1.0e+6;
Int_t i;
Double_t x[M]= { 300.0,  305.0,  310.0,  315.0,  320.0,  325.0,  330.0,  335.0,  340.0,  345.0,
                 350.0,  355.0,  360.0,  365.0,  370.0,  375.0,  380.0,  385.0,  390.0,  395.0,
                 400.0,  405.0,  410.0,  415.0,  420.0,  425.0,  430.0,  435.0,  440.0,  445.0,
                 450.0,  455.0,  460.0,  465.0,  470.0,  475.0,  480.0,  485.0,  490.0,  495.0,
                 500.0,  505.0,  510.0,  515.0,  520.0,  525.0,  530.0,  535.0,  540.0,  545.0,
                 550.0,  555.0,  560.0,  565.0,  570.0,  575.0,  580.0,  585.0,  590.0,  595.0,
                 600.0,  605.0,  610.0,  615.0,  620.0,  625.0,  630.0,  635.0,  640.0,  645.0,
                 650.0,  655.0,  660.0,  665.0,  670.0,  675.0,  680.0,  685.0,  690.0,  695.0,
                 700.0 };
Double_t y[M]= { 0.001,  0.0012, 0.0013, 0.0014, 0.0015, 0.002,  0.0025, 0.003,  0.004,  0.005,
                 0.01,   0.02,   0.06,   0.11,   0.19,   0.25,   0.33,   0.4,    0.48,   0.57,
                 0.64,   0.71,   0.76,   0.82,   0.885,  0.945,  0.99,   1.015,  1.02,   1.02,
                 1.012,  0.98,   0.95,   0.93,   0.895,  0.85,   0.8,    0.74,   0.7,    0.66,
                 0.6,    0.56,   0.5,    0.47,   0.43,   0.38,   0.345,  0.32,   0.27,   0.235,
                 0.2,    0.17,   0.15,   0.125,  0.1,    0.09,   0.075,  0.05,   0.03,   0.02,
                 0.01,   0.009,  0.008,  0.007,  0.006,  0.005,  0.004,  0.003,  0.002,  0.001,
                 0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,
                 0.001 };
Double_t s[M];
for (i=0;i<M;i++) s[i] = 0.01;
//
//     Normalization of the spectrum
//
// We normalize y[i] and s[i] in such a way that the surface under the curve be equal to Nphot,
//i.e coresponds to Nphot photons. This is not necessary, but it is a nice feature when we will
//be at the stage of representing the curve as a sum of gaussians.
//
Double_t S = zero;
for (i=0;i<M-1;i++) {
  S += z05*(x[i+1] - x[i])*(y[i] + y[i+1]);
}
cout << "Old Surface: " << S << endl;
for (i=0;i<M;i++) {
  y[i] *= Nphot/S;
  s[i] *= Nphot/S;
}
S = zero;
for (i=0;i<M-1;i++) {
  S += z05*(x[i+1] - x[i])*(y[i] + y[i+1]);
}
cout << "New Surface: " << S    << endl;
cout << "New error  : " << s[0] << endl;
SpecPbWO4Nb = new TSplineFit("Spectrum_PbWO4Nb","Emission Spectrum | Lead tungstate PbWO4 doped with niobium",
                           16,M,m,x,y,s,kTRUE,0.0,kFALSE,0.0,300.0,700.0,kFALSE);
SpecPbWO4Nb->SetSource("CMS ECAL Calorimeter TDR");
SpecPbWO4Nb->SetMacro("Spectrum_PbWO4Nb.C");
SpecPbWO4Nb->SetXLabel("wavelength [nm]");
SpecPbWO4Nb->SetVLabel("Intensity");
  if (todraw) {
    SpecPbWO4Nb->SetDefaultLabels();
    SpecPbWO4Nb->DrawFit();
    SpecPbWO4Nb->Print();
  }
  if (infile) {
    if (firstinfile) SpecPbWO4Nb->UpdateFile(kTRUE);
    else             SpecPbWO4Nb->UpdateFile(kFALSE);
  }
  return SpecPbWO4Nb;
}
