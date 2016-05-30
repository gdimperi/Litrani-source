TSplineFit* DielTensor_PbWO4_000(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   DielTensor_PbWO4_000
//
//   Description :
//
//   Smallest diagonal element of dielectric tensor of PbWO4
//  in the system where it is diagonal as a function of wavelength.
//
//   Source : Calculated starting from the paper CMS TN/95-184
//            updated in december 1997 by Remi Chipaux
//
//   OLD f_epsapbwo4
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 41;
  const Int_t m = 8;
  Int_t i;
  TSplineFit *DielTensorA;
  Double_t x[M]= { 300,   310,   320,   330,   340,   350,   360,   370,   380,   390,   
    400,   420,   440,   460,   480,   500,   520,   540,   560,   580,   
    600,   620,   640,   660,   680,   700,   720,   740,   760,   780,   
    800,   820,   840,   860,   880,   900,   920,   940,   960,   980,   
    1000 };
  Double_t y[M]= { 7.291, 6.615, 6.194, 5.907, 5.698, 5.540, 5.416, 5.317, 5.235, 5.168, 
    5.110, 5.018, 4.949, 4.894, 4.850, 4.814, 4.784, 4.758, 4.737, 4.718, 
    4.702, 4.688, 4.676, 4.664, 4.655, 4.646, 4.638, 4.631, 4.624, 4.618, 
    4.613, 4.608, 4.603, 4.599, 4.595, 4.592, 4.588, 4.585, 4.582, 4.580, 
    4.577  };
  Double_t s02[M]= { 0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  
    0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  
    0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  
    0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  
    0.02 };
  DielTensorA = new TSplineFit("DielTensor_PbWO4","Element of dielectric tensor | PbWO4",
    2,M,m,x,y,s02,kTRUE,0.0,kTRUE,7.3,300.0,1000.0,kFALSE);
  DielTensorA->BelongsToFamily(0,1.0,"Axis to associate");
  DielTensorA->SetSource("Remi Chipaux: paper CMS TN/95-184");
  DielTensorA->SetMacro("DielTensor_PbWO4_000.C");
  DielTensorA->SetXLabel("wavelength [nm]");
  DielTensorA->SetVLabel("epsilon dielectric");
  if (todraw) {
    DielTensorA->SetDefaultLabels();
    DielTensorA->DrawFit();
    DielTensorA->Print();
  }
  if (infile) {
    if (firstinfile) DielTensorA->UpdateFile(kTRUE);
    else             DielTensorA->UpdateFile(kFALSE);
  }
  return DielTensorA;
}
