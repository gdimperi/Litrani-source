TSplineFit* QuantumEff_XP2020(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   QuantumEff_XP2020
//
//   Quantum efficiency of the photocathode of the Phototube XP2020
//
//   Source : Document Philips
//
//   OLD f_effcathode
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 61;
  const Int_t m = 5;
  Int_t i;
  Double_t sig[M];
  TSplineFit *PhotoTube;
  Double_t x[M] = { 300,  310,  320,  330,  340,  350,  360,  370,  380,  390,  
    400,  410,  420,  430,  440,  450,  460,  470,  480,  490,  
    500,  510,  520,  530,  540,  550,  560,  570,  580,  590,  
    600,  610,  620,  630,  640,  650,  660,  670,  680,  690,  
    700,  710,  720,  730,  740,  750,  760,  770,  780,  790,  
    800,  820,  840,  860,  880,  900,  920,  940,  960,  980,  
    1000 };
  Double_t y[M] = { 0.1364,0.1600,0.1938,0.2255,0.2553,0.2657,0.2687,0.2681,0.2741,0.2703,
    0.2666,0.2601,0.2510,0.2393,0.2255,0.2149,0.2049,0.1952,0.1757,0.1645,
    0.1538,0.1435,0.1312,0.1170,0.1033,0.0879,0.0775,0.0653,0.0470,0.0378,
    0.0269,0.0224,0.0180,0.0118,0.0078,0.0055,0.0038,0.0024,0.0013,0.0009,
    0.0005,0.0003,0.0002,0.0001,0.0,   0.0,   0.0,   0.0,   0.0,   0.0,
    0.0    0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   
    ,0.0 };
  for (i=0;i<M;i++) sig[i] = 0.005;
  PhotoTube = new TSplineFit("QuantumEff_XP2020","Quantum efficiency | Philips XP2020",
    10,M,m,x,y,sig,kTRUE,0.0,kFALSE,0.275,300.0,1000.0);
  PhotoTube->SetSource("Philips documentation");
  PhotoTube->SetMacro("QuantumEff_XP2020.C");
  PhotoTube->SetXLabel("wavelength [nm]");
  PhotoTube->SetVLabel("quantum efficiency");
  if (todraw) {
    PhotoTube->SetDefaultLabels();
    PhotoTube->DrawFit();
    PhotoTube->Print();
  }
  if (infile) {
    if (firstinfile) PhotoTube->UpdateFile(kTRUE);
    else             PhotoTube->UpdateFile(kFALSE);
  }
  return PhotoTube;
}
