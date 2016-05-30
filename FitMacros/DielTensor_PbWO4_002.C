TSplineFit* DielTensor_PbWO4_002(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   DielTensor_PbWO4_002
//
//   Description :
//
//   Largest diagonal element of dielectric tensor of PbWO4
//  in the system where it is diagonal as a function of wavelength.
//
//   Source : Calculated starting from the paper CMS TN/95-184
//            by Remi Chipaux
//
//   OLD f_epscpbwo4
//
{
Int_t k1;
Int_t k2 = -100;
k1 = TClassTable::GetID("TSplineFit");
if (k1<0) k2 = gSystem.Load("libSplineFit");
const Int_t M = 41;
const Int_t m = 8;
Int_t i;
TSplineFit *DielTensorC;
Double_t x[M]= { 300,   310,   320,   330,   340,   350,   360,   370,   380,   390,   
                 400,   420,   440,   460,   480,   500,   520,   540,   560,   580,   
                 600,   620,   640,   660,   680,   700,   720,   740,   760,   780,   
                 800,   820,   840,   860,   880,   900,   920,   940,   960,   980,   
                1000 };
Double_t y[M]= { 9.862, 8.503, 7.692, 7.154, 6.771, 6.485, 6.264, 6.088, 5.945, 5.826,
                 5.726, 5.567, 5.447, 5.353, 5.278, 5.217, 5.166, 5.123, 5.086, 5.055,
                 5.028, 5.004, 4.983, 4.965, 4.948, 4.933, 4.920, 4.908, 4.897, 4.887,
                 4.878, 4.870, 4.862, 4.855, 4.849, 4.843, 4.837, 4.832, 4.827, 4.823,
                 4.818  };
Double_t s04[M]= { 0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  
                 0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  
                 0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  
                 0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  0.02,  
                 0.02 };
DielTensorC = new TSplineFit("DielTensor_PbWO4","Element of dielectric tensor | PbWO4",
                                 2,M,m,x,y,s04,kTRUE,0.0,kTRUE,9.9,300.0,1000.0,kFALSE);
DielTensorC->BelongsToFamily(2,3.0,"Axis to associate");
DielTensorC->SetSource("Remi Chipaux: paper CMS TN/95-184");
DielTensorC->SetMacro("DielTensor_PbWO4_002.C");
DielTensorC->SetXLabel("wavelength [nm]");
DielTensorC->SetVLabel("epsilon dielectric");
  if (todraw) {
    DielTensorC->SetDefaultLabels();
    DielTensorC->DrawFit();
    DielTensorC->Print();
  }
  if (infile) {
    if (firstinfile) DielTensorC->UpdateFile(kTRUE);
    else             DielTensorC->UpdateFile(kFALSE);
  }
  return DielTensorC;
}
