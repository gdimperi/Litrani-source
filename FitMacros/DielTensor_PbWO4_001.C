TSplineFit* DielTensor_PbWO4_001(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   DielTensor_PbWO4_001
//
//   Description :
//
//   Middle diagonal element of dielectric tensor of a pseudo PbWO4 which
//would be totally anisotropic instead of being negativ birefringent,
//in the system where it is diagonal as a function of wavelength.
//
//   Source : interpolated between Epsa_PbWO4 and Epsc_PbWO4
//
//   OLD f_epsbpbwo4
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 41;
  const Int_t m = 8;
  Int_t i;
  TSplineFit *DielTensorB;
  Double_t x[M]= { 300,    310,    320,    330,    340,    350,    360,    370,    380,    390,    
    400,    420,    440,    460,    480,    500,    520,    540,    560,    580,    
    600,    620,    640,    660,    680,    700,    720,    740,    760,    780,    
    800,    820,    840,    860,    880,    900,    920,    940,    960,    980,    
    1000 };
  Double_t y[M]= { 8.576,  7.559,  6.943,  6.530,  6.234,  6.012,  5.840,  5.702,  5.590,  5.497,  
    5.418,  5.292,  5.198,  5.123,  5.064,  5.015,  4.975,  4.940,  4.911,  4.886,  
    4.865,  4.846,  4.829,  4.814,  4.801,  4.789,  4.779,  4.769,  4.760,  4.752,
    4.745,  4.739,  4.732,  4.727,  4.722,  4.717,  4.712,  4.708,  4.704,  4.701,
    4.697 };
  Double_t s03[M]= { 0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,
    0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,
    0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,
    0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,   0.02,
    0.02 };
  DielTensorB = new TSplineFit("DielTensor_PbWO4","Element of dielectric tensor | PbWO4",
    2,M,m,x,y,s03,kTRUE,0.0,kTRUE,8.6,300.0,1000.0,kFALSE);
  DielTensorB->BelongsToFamily(1,2.0,"Axis to associate");
  DielTensorB->SetSource("Fake values interpolated from true values epsa and epsc");
  DielTensorB->SetMacro("DielTensor_PbWO4_001.C");
  DielTensorB->SetXLabel("wavelength [nm]");
  DielTensorB->SetVLabel("epsilon dielectric");
  if (todraw) {
    DielTensorB->SetDefaultLabels();
    DielTensorB->DrawFit();
    DielTensorB->Print();
  }
  if (infile) {
    if (firstinfile) DielTensorB->UpdateFile(kTRUE);
    else             DielTensorB->UpdateFile(kFALSE);
  }
  return DielTensorB;
}
