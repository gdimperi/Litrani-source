{
//
// This macro has been produced using the file "Spectrum.C" produced by the macro
//"GaussDecomp_In6WOTb.C", introducing later on the decay times components. For
//decay time, we have arbitrarily put 1000.0 ns, as we don't know it!
//
  TLitSpectrum *sp;
  sp = new TLitSpectrum("Spectrum_In6WOTb","Emission Spectrum | indium tungstate doped with terbium");
  sp->AddOneComponent(        389.78820368,       12.6337447462,       106802.646052,300.0,150.0,1.0);
  sp->AddOneComponent(       411.608797192,       12.6435656992,       81420.8729246,300.0,150.0,1.0);
  sp->AddOneComponent(        363.80978922,       13.6487947972,       106208.950595,300.0,150.0,1.0);
  sp->AddOneComponent(       339.056056358,       14.7397044141,       107503.524542,300.0,150.0,1.0);
  sp->AddOneComponent(       430.656970431,        13.232517416,        75313.535433,300.0,150.0,1.0);
  sp->AddOneComponent(       447.079352858,       14.8870990161,       66379.0024627,300.0,150.0,1.0);
  sp->AddOneComponent(       462.786324292,       13.8509348981,        59411.121266,300.0,150.0,1.0);
  sp->AddOneComponent(       483.554480002,       14.1334138862,       51830.4177368,300.0,150.0,1.0);
  sp->AddOneComponent(       499.417886254,       16.5296183087,       49641.1205897,300.0,150.0,1.0);
  sp->AddOneComponent(       317.251250395,       15.4200769941,       63803.6166247,300.0,150.0,1.0);
  sp->AddOneComponent(       290.321527059,       14.4555732922,        59638.524801,300.0,150.0,1.0);
  sp->AddOneComponent(       527.396148869,       21.1419962136,       24339.9272936,300.0,150.0,1.0);
  sp->AddOneComponent(       263.973737032,       16.4865110835,       23314.0116928,300.0,150.0,1.0);
  sp->AddOneComponent(       232.251167204,       17.9439737266,       14476.8227214,300.0,150.0,1.0);
  sp->AddOneComponent(       519.695509261,        14.032725551,       26546.0182691,300.0,150.0,1.0);
  sp->AddOneComponent(       557.466406285,       34.6851973463,       51439.5621411,300.0,150.0,1.0);
  sp->AddOneComponent(       578.744749553,       84.5939009946,        35143.127799,300.0,150.0,1.0);
  sp->Normalize();
  Double_t wavelength,decaytime;
  TH1D *hd = new TH1D("DecayTime","Decay time",1000,0.0,2000.0);
  TH1D *hw = new TH1D("WaveLength","WaveLength",500,200.0,1000.0);
  Int_t k;
  for (k=0;k<1000000;k++) {
    sp->GenWaveLAndDecayT(wavelength,decaytime);
    hd->Fill(decaytime);
    hw->Fill(wavelength);
  }
//  hd->Draw();
  hw->Draw();
}
