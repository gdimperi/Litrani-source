TSplineFit* AbsorptionLength_CsITl(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//   AbsorptionLength_CsITl
//
//   Absorption length of CsI(Tl) as a function of wavelength in [nm]
//
//   Source : measures of a Crismatec Csi/Tl cristal, 20 cm long, 20 Jan. 2000, Remi Chipaux
//
//   Old f_abslcsitl.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 108;
  const Int_t m = 6;
  Int_t i;
  TSplineFit *AbsLCsITl;
  Double_t x[M]= { 300,  320,  330,  335,  340,  345,  350,  355,  360,  365,
    370,  375,  380,  385,  390,  395,  400,  405,  410,  415,
    420,  425,  430,  435,  440,  445,  450,  455,  460,  465,
    470,  475,  480,  485,  490,  495,  500,  505,  510,  515,
    520,  525,  530,  535,  540,  545,  550,  555,  560,  565,
    570,  575,  580,  585,  590,  595,  600,  605,  610,  615,
    620,  625,  630,  635,  640,  645,  650,  655,  660,  665,
    670,  675,  680,  685,  690,  695,  700,  705,  710,  715,
    720,  725,  730,  735,  740,  745,  750,  755,  760,  765,
    770,  775,  780,  785,  790,  795,  800,  805,  810,  815,
    820,  825,  830,  835,  840,  845,  850,  1000 };
  Double_t y[M]= {    0.2,    0.2,    4.9,    7.8,   25.0,   69.5,  160.9,  293.8,  486.6,  652.9,
    670.7, 1007.8,  696.6,  751.8,  704.8,  644.7,  751.7,  724.4,  702.0,  694.4,
    637.1,  594.8,  574.1,  560.5,  558.1,  533.2,  516.9,  491.2,  475.9,  468.6,
    444.5,  458.7,  468.8,  462.6,  464.9,  458.4,  462.1,  445.5,  443.5,  444.8,
    447.6,  448.9,  453.4,  446.8,  467.6,  460.2,  466.4,  449.6,  446.2,  454.7,
    457.9,  460.1,  449.6,  452.4,  442.6,  441.9,  452.2,  460.6,  458.7,  451.4,
    471.7,  434.8,  468.0,  439.1,  433.8,  453.5,  422.6,  435.6,  422.4,  444.0,
    426.1,  426.9,  412.7,  430.0,  436.8,  439.4,  431.2,  427.1,  461.6,  454.3,
    432.6,  436.3,  432.7,  402.1,  423.6,  443.3,  463.9,  456.3,  463.5,  477.0,
    495.6,  451.5,  454.5,  467.5,  449.6,  453.6,  452.9,  444.4,  461.2,  409.2,
    432.9,  474.2,  440.2,  467.3,  526.6,  515.5,  412.6,  420.0  };
  Double_t s06[M]= {    0.1,    0.1,   30.2,    2.3,    1.6,    6.2,   28.0,   87.6,  232.2,  409.1,
    424.9,  948.5,  447.5,  516.4,  450.3,  373.3,  503.0,  462.8,  431.6,  419.7,
    350.9,  304.0,  281.7,  267.4,  263.8,  239.8,  224.3,  201.8,  188.7,  182.1,
    163.2,  173.0,  180.2,  174.5,  175.4,  169.7,  171.7,  159.0,  157.2,  157.7,
    159.2,  159.7,  162.4,  157.3,  171.9,  166.0,  170.2,  157.7,  154.9,  160.6,
    162.4,  163.7,  156.1,  157.7,  150.5,  149.8,  156.4,  162.2,  160.4,  155.1,
    169.1,  143.5,  165.9,  145.9,  142.2,  155.2,  134.6,  142.9,  134.0,  148.0,
    136.1,  136.4,  127.4,  138.2,  142.2,  143.8,  138.1,  135.6,  158.1,  153.0,
    138.6,  140.7,  138.2,  119.3,  132.4,  144.8,  158.2,  152.9,  157.8,  167.1,
    180.2,  149.4,  151.4,  159.6,  147.8,  150.7,  149.8,  144.2,  155.3,  122.2,
    136.6,  163.5,  141.1,  158.1,  201.4,  192.8,  123.0,  100.0 };
  AbsLCsITl = new TSplineFit("AbsorptionLength_CsITl","Absorption Length | CsI(Tl) Cesium iodide doped with Tallium",
    5,M,m,x,y,s06,kTRUE,0.0,kTRUE,1010.0,300.0,1000.0,kFALSE);
  AbsLCsITl->SetSource("measures of a Crismatec Csi/Tl cristal, 20 cm long, 20 Jan. 2000, Remi Chipaux");
  AbsLCsITl->SetMacro("AbsorptionLength_CsITl.C");
  AbsLCsITl->SetXLabel("wavelength [nm]");
  AbsLCsITl->SetVLabel("absorption length [cm]");
  if (todraw) {
    AbsLCsITl->SetDefaultLabels();
    AbsLCsITl->DrawFit();
    AbsLCsITl->Print();
  }
  if (infile) {
    if (firstinfile) AbsLCsITl->UpdateFile(kTRUE);
    else             AbsLCsITl->UpdateFile(kFALSE);
  }
  return AbsLCsITl;
}
