TSplineFit* DEdx_MusInPbWO4_001(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  DEdx_MusInPbWO4_001
//
//  Landau distribution of dE/dx of muons of more than 10 Gev in PbWO4.
//   Values in Mev
//   Calculated with GEANT by A.Givernaud
//   (values are the same as in DEdx_MusInPbWO4_000 ! by lazyness!)
//
//  Old f_dedx_20.C
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 120;
  const Int_t m = 4;
  Int_t i;
  TSplineFit *DEdx1;
  Double_t x[M]= {  0.25,     0.75,     1.25,     1.75,     2.25,     2.75,     3.25,     3.75,     4.25,     4.75,
    5.25,     5.75,     6.25,     6.75,     7.25,     7.75,     8.25,     8.75,     9.25,     9.75,
    10.25,    10.75,    11.25,    11.75,    12.25,    12.75,    13.25,    13.75,    14.25,    14.75,
    15.25,    15.75,    16.25,    16.75,    17.25,    17.75,    18.25,    18.75,    19.25,    19.75,
    20.25,    20.75,    21.25,    21.75,    22.25,    22.75,    23.25,    23.75,    24.25,    24.75,
    25.25,    25.75,    26.25,    26.75,    27.25,    27.75,    28.25,    28.75,    29.25,    29.75,
    30.25,    30.75,    31.25,    31.75,    32.25,    32.75,    33.25,    33.75,    34.25,    34.75,
    35.25,    35.75,    36.25,    36.75,    37.25,    37.75,    38.25,    38.75,    39.25,    39.75,
    40.25,    40.75,    41.25,    41.75,    42.25,    42.75,    43.25,    43.75,    44.25,    44.75,
    45.25,    45.75,    46.25,    46.75,    47.25,    47.75,    48.25,    48.75,    49.25,    49.75,
    50.25,    50.75,    51.25,    51.75,    52.25,    52.75,    53.25,    53.75,    54.25,    54.75,
    55.25,    55.75,    56.25,    56.75,    57.25,    57.75,    58.25,    58.75,    59.25,    59.75 };
  Double_t y[M]= { 10.0,     50.0,     80.0,    120.0,    200.0,    300.0,    500.0,    650.0,    800.0,   1250.0,
    2100.0,   3500.0,   5900.0,   9000.0,  11000.0,  12600.0,  13100.0,  11100.0,   7300.0,   4600.0,
    2900.0,   2100.0,   1400.0,   1200.0,    950.0,    900.0,    800.0,    740.0,    730.0,    710.0,
    700.0,    693.2,    686.4,    679.6,    672.8,    665.9,    659.1,    652.3,    645.5,    638.7,
    631.9,    625.1,    618.3,    611.4,    604.6,    597.8,    591.0,    584.2,    577.4,    570.6,
    563.8,    557.0,    550.1,    543.3,    536.5,    529.7,    522.9,    516.1,    509.3,    502.5,
    495.6,    488.8,    482.0,    475.2,    468.4,    461.6,    454.8,    448.0,    441.2,    434.3,
    427.5,    420.7,    413.9,    407.1,    400.3,    393.5,    386.7,    379.9,    373.0,    366.2,
    359.4,    352.6,    345.8,    339.0,    332.2,    325.4,    318.6,    311.7,    304.9,    298.1,
    291.3,    284.5,    277.7,    270.9,    264.1,    257.2,    250.4,    243.6,    236.8,    230.0,
    223.2,    216.4,    209.6,    202.8,    195.9,    189.1,    182.3,    175.5,    168.7,    161.9,
    155.1,    148.3,    141.4,    134.6,    127.8,    121.0,    114.2,    107.4,    100.6,     93.8 };
  Double_t s18[M];
  for (i=0;i<M;i++) s18[i] = TMath::Sqrt(y[i]);
  DEdx1 = new TSplineFit("DEdx_MusInPbWO4","Energy deposited by crossing | Muons in PbWO4",
    11,M,m,x,y,s18,kTRUE,0.0,kFALSE,100000.0,0.0,60.0,kFALSE);
  DEdx1->BelongsToFamily(1,20.0,"Energy of muon");
  DEdx1->SetSource("Calculated with GEANT by A.Givernaud DSM/DAPNIS/SPP CEA Saclay");
  DEdx1->SetMacro("DEdx_MusInPbWO4_001.C");
  DEdx1->SetXLabel("deposited energy [Mev]");
  DEdx1->SetVLabel("Nb. of hits according to GEANT 3");
  DEdx1->UseForRandom();
  if (todraw) {
    DEdx1->SetDefaultLabels();
    DEdx1->DrawFit();
    DEdx1->Print();
  }
  if (infile) {
    if (firstinfile) DEdx1->UpdateFile(kTRUE);
    else             DEdx1->UpdateFile(kFALSE);
  }
  return DEdx1;
}
