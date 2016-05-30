{
//
// Decomposition into gaussians of the emission spectrum of PbWO4 doped with lanthane
// This spectrum has previously been entered into SplineFitDB.rdb has a TSplineFit
//of category 16, with name:
//   "Spectrum_PbWO4La" and title
//   "Emission Spectrum | Lead tungstate PbWO4 doped with lanthane"
// We give the same name and title to the TLitSpectrum created here.
//
// In the litterature, it is said that PbWO4(La) has 2 "components", one at 420nm
//and the other at 500 nm. But trying to fit the spectrum with only 2 gaussians
//peaks at these values give a bad fit. This shows that what is called "component"
//or "emission band" is in fact not gaussian. Since it is impossible to know what
//is the real shape of these "components" or "emission bands", we restrict Litrani
//to the use of gaussian peaks. But nothing prevents you to use more gaussian peaks
//than the number of "emission bands" quoted in the litterature. Here we get a good
//fit using 4 gaussian peaks, the first 2 being fixed at the values 420 and 500
//given in the litterature, the 3rd peak being a complementary peak of the one at 420,
//with a position adjustable by the fit, and the 4th peak being a complementary peak
//of the one at 500 nm, also with a position adjustable by the fit. On the contrary,
//the position of peaks 1 and 2 are fixed.
//
// Notice that the call to TLitSpectrum::BetterPeaks() produce a file "Spectrum.C",
//which you can use later on for a full definition of this TLitSpectrum, with
//decay times information. The macro produced with this file "Spectrum.C", after
//addition of the decay times, is called "LitSpectrum_PbWO4La.C"
//
  Bool_t ok;
  TLitSpectrum *sp;
  const Int_t Npeaks = 4;
  Bool_t MeanFixed = kFALSE;
  Double_t *mean, *sigma, *weight;
  Short_t *meanfixed;
  mean      = new Double_t [Npeaks];
  sigma     = new Double_t [Npeaks];
  weight    = new Double_t [Npeaks];
  meanfixed = new Short_t  [Npeaks];
  sp = new TLitSpectrum("Spectrum_PbWO4La","Emission Spectrum | Lead tungstate PbWO4 doped with lanthane",kTRUE);
  mean[0]      = 420.0;
  mean[1]      = 500.0;
  mean[2]      = 400.0;
  mean[3]      = 520.0;
  sigma[0]     = 50.0;
  sigma[1]     = 50.0;
  sigma[2]     = 10.0;
  sigma[3]     = 10.0;
  weight[0]    = 400000.0;
  weight[1]    = 400000.0;
  weight[2]    = 100000.0;
  weight[3]    = 100000.0;
  meanfixed[0] = 1;
  meanfixed[1] = 1;
  meanfixed[2] = 0;
  meanfixed[3] = 0;
  ok = sp->BetterPeaks(Npeaks,mean,sigma,weight,meanfixed,kTRUE);
}
