TSplineFit* AbsorptionLength_Q23067(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   AbsorptionLength_Q23067
//
//   Description :
//
//   Absorption length for the optical grease Q2-3067
//
//   Source : Remi Chipeaux
//
//   Old f_abslgrease
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 34;
  const Int_t m = 5;
  Int_t i;
  TSplineFit *AbsLGrease;
  Double_t x[M]= { 320,    340,    360,    380,    400,    420,    440,    460,    480,    500,
    520,    540,    560,    580,    600,    620,    640,    660,    680,    700,
    720,    740,    760,    780,    800,    820,    840,    860,    880,    900,
    920,    940,    960,    980 };
  Double_t y[M]= {  8.5,    9.0,    9.5,   10.5,   12.0,   13.0,   14.0,   15.0,   16.0,   17.0,
    18.0,   19.0,   20.0,   21.0,   22.0,   23.0,   24.0,   25.0,   26.0,   27.0,
    28.0,   29.0,   30.0,   31.0,   32.0,   33.0,   34.0,   35.0,   36.0,   37.0,
    38.0,   39.0,   40.0,   41.0 };
  Double_t s10[M];
  for (i=0;i<M;i++) s10[i] = 0.1;
  AbsLGrease = new TSplineFit("AbsorptionLength_Q23067","Absorption Length | optical grease Q2-3067",
    5,M,m,x,y,s10,kTRUE,8.0,kTRUE,42.0,300.0,1000.0,kFALSE);
  AbsLGrease->SetSource("Remi Chipaux DSM/DAPNIA/SEDI CEA Saclay");
  AbsLGrease->SetMacro("AbsorptionLength_Q23067.C");
  AbsLGrease->SetXLabel("wavelength [nm]");
  AbsLGrease->SetVLabel("absorption length [cm]");
  if (todraw) {
    AbsLGrease->SetDefaultLabels();
    AbsLGrease->DrawFit();
    AbsLGrease->Print();
  }
  if (infile) {
    if (firstinfile) AbsLGrease->UpdateFile(kTRUE);
    else             AbsLGrease->UpdateFile(kFALSE);
  }
  return AbsLGrease;
}
