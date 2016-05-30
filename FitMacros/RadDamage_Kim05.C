TSplineFit* RadDamage_Kim05(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
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
//  RadDamage_Kim05
//
//  Derived from AbsorptionLength_crys8981R06.
//  Describes the part induced by irradiation in the inverse of the absorption
//  length. It is after the 5th irradiation in GIF. Measurements of the 30/4/2002.
//
//  Source : Remi Chipaux
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 45;
  const Int_t m = 3;
  Int_t i;
  TSplineFit *damage;
  Double_t x[M]= { 330,          340,          350,          360,          370,          380,
    390,          400,          410,          420,          430,          440,
    450,          460,          470,          480,          490,          500,
    510,          520,          530,          540,          550,          560,
    570,          580,          590,          600,          610,          620,
    630,          640,          650,          660,          670,          680,
    690,          700,          710,          720,          730,          740,
    750,          800,          850 };
  Double_t y[M]= { -0.007363246, -0.002788877,  0.031100289,  0.005602002,  0.000851245,  0.000130297,
    0.000145448,  0.000206724,  0.000273538,  0.000375261,  0.000200412,  0.000314387,
    0.000290788,  0.000475612,  0.000410518,  0.000392068,  0.000171737,  0.001284649,
    0.000653549,   0.00059502,  0.000456834,  0.000421961,  0.000346565,  0.000325117,
    0.000242016,  0.000393037,  0.000313298,  0.000252798,  0.000235979,  0.000393796,
    0.000331555,  0.000323389,  0.000335338,  0.000373868,  0.000362355,  0.000475756,
    0.000466199,  0.000498366,  0.000686102,  0.000623346,  0.000576152,  0.000609058,
    0.00076792,   0.00076792,   0.00076792  };
  //Double_t s[M]= { 0.395305385, 0.400511539, 0.156940686, 0.002539992, 0.001055344, 0.000839395,
  //                 0.000772965, 0.00080759 , 0.000939753, 0.000902046, 0.001184224, 0.001710793,
  //                 0.001384759, 0.009745962, 6.320978466, 6.345057564, 5.389794483, 3.837625514,
  //                 1.570859678, 1.181564726, 1.175686249, 1.728859112, 1.623879617, 1.241554255,
  //                 0.340406244, 0.337372095, 0.258412244, 0.194251833, 0.217015729, 0.158402505,
  //                 0.129948547, 0.000916544, 0.070859122, 0.317648595, 0.697857058, 0.754251657,
  //                 0.806652463, 0.731889722, 0.786018665, 0.613352134, 0.956593017, 1.331831626,
  //                 2.353932824, 2.353932824, 2.353932824  };
  Double_t s24[M];
  for (i=0;i<M;i++) s24[i] = 0.001;
  damage = new TSplineFit("RadDamage_Kim05","Radiation damages in shape | Kimax after 5th irradiation",
    15,M,x,y,320.0,850.0);
  //                           15,M,m,x,y,s24,kFALSE,0.0,kFALSE,0.0,320.0,850.0,kFALSE);
  damage->SetSource("Remi Chipaux");
  damage->SetMacro("RadDamage_Kim05.C");
  damage->SetXLabel("wavelength [nm]");
  damage->SetVLabel("damage factor kimax [cm-1]");
  if (todraw) {
    damage->SetDefaultLabels();
    damage->DrawFit();
    damage->Print();
  }
  if (infile) {
    if (firstinfile) damage->UpdateFile(kTRUE);
    else             damage->UpdateFile(kFALSE);
  }
  return damage;
}
