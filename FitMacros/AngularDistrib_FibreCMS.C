TSplineFit* AngularDistrib_FibreCMS(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  AngularDistrib_FibreCMS
//
//  Measured angular distribution of an optical fibre of CMS used at Saclay
//  Measurements done at Saclay
//
//  OLD f_angdistfib
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 34;
  const Int_t m = 4;
  Int_t i;
  Double_t sig[M];
  TSplineFit *AngDistribFib;
  Double_t x[M] = { 0.00597,  0.01791,  0.02984,  0.04177,  0.05369,  0.06561,  0.07751,  0.08940,
    0.10127,  0.11313,  0.12497,  0.13679,  0.14859,  0.16036,  0.17211,  0.18382,
    0.19551,  0.20716,  0.21878,  0.23035,  0.24189,  0.25339,  0.26484,  0.27624,
    0.28760,  0.29890,  0.31015,  0.32134,  0.33246,  0.34353,  0.35453,  0.36547,
    0.37633,  0.38712 };
  Double_t y[M] = { 0.00597,  0.01774,  0.02877,  0.03846,  0.04637,  0.05218,  0.05574,  0.05707,
    0.05636,  0.05388,  0.05002,  0.04517,  0.03975,  0.03413,  0.02860,  0.02343,
    0.01876,  0.01469,  0.01126,  0.00845,  0.00621,  0.00447,  0.00315,  0.00218,
    0.00147,  0.00098,  0.00064,  0.00041,  0.00025,  0.00016,  0.00009,  0.00006,
    0.00003,  0.00002 };
  for (i=0;i<M;i++) sig[i] = 0.01;
  AngDistribFib = new TSplineFit("AngularDistrib_FibreCMS","Angular distribution of photons | Fibre used in CMS",
    12,M,m,x,y,sig,kTRUE,0.0,kFALSE,10.0,0.0,0.39269);
  AngDistribFib->SetSource("Measurements done in the SEDI lab at Saclay");
  AngDistribFib->SetMacro("AngularDistrib_FibreCMS.C");
  AngDistribFib->SetXLabel("radians");
  AngDistribFib->SetVLabel("amount of light");
  AngDistribFib->UseForRandom();
  if (todraw) {
    AngDistribFib->SetDefaultLabels();
    AngDistribFib->DrawFit();
    AngDistribFib->Print();
  }
  if (infile) {
    if (firstinfile) AngDistribFib->UpdateFile(kTRUE);
    else             AngDistribFib->UpdateFile(kFALSE);
  }
  return AngDistribFib;
}
