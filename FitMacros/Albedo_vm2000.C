TSplineFit* Albedo_vm2000(Bool_t todraw = kTRUE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   Albedo_vm2000
//
//   Albedo of wrapping vm2000. Using class TLitAlbedo, the complex index of refraction
//    of vm2000 will be produced using the albedo.
//
//   Source : NIM A 547 (2005) 368-388
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 39;
  const Int_t m = 4;
  Int_t i;
  Double_t sig[M];
  TSplineFit *AlbedoVM2000;
  Double_t x[M] = { 300.0, 310.0, 320.0, 330.0, 340.0, 350.0, 360.0, 370.0, 380.0, 390.0,
                    400.0, 410.0, 420.0, 430.0, 440.0, 450.0, 460.0, 470.0, 480.0, 490.0,
                    500.0, 510.0, 520.0, 530.0, 540.0, 550.0, 560.0, 570.0, 580.0, 590.0,
                    600.0, 650.0, 700.0, 750.0, 800.0, 850.0, 900.0, 950.0, 1000.0 };
  Double_t y[M] = { 0.13,  0.13,  0.11,  0.1,   0.1,   0.1,   0.11,  0.12,  0.3,   0.94,
                    0.97,  0.975, 0.978, 0.979, 0.98,  0.982, 0.985, 0.987, 0.99,  0.99,
                    0.99,  0.99,  0.99,  0.99,  0.99,  0.99,  0.99,  0.99,  0.99,  0.99,
                    0.99,  0.99,  0.99,  0.99,  0.99,  0.99,  0.99,  0.99,  0.99   };
  for (i=0;i<M;i++) sig[i] = 0.01;
  //  AlbedoVM2000 = new TSplineFit("Albedo_vm2000","Albedo of wrapping | vm2000",
  //    21,M,m,x,y,sig,kTRUE,0.0,kTRUE,1.0,300.0,1000.0);
  AlbedoVM2000 = new TSplineFit("Albedo_vm2000","Albedo of wrapping | vm2000",
    21,M,x,y,300.0,1000.0);
  AlbedoVM2000->SetSource("NIM A 547 (2005) 368-388");
  AlbedoVM2000->SetMacro("Albedo_vm2000.C");
  AlbedoVM2000->SetXLabel("wavelength [nm]");
  AlbedoVM2000->SetVLabel("albedo [0-1]");
  if (todraw) {
    AlbedoVM2000->SetDefaultLabels();
    AlbedoVM2000->DrawFit();
    AlbedoVM2000->Print();
  }
  if (infile) {
    if (firstinfile) AlbedoVM2000->UpdateFile(kTRUE);
    else             AlbedoVM2000->UpdateFile(kFALSE);
  }
  return AlbedoVM2000;
}
