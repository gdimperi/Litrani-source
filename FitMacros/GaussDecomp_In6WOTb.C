{
//
// Decomposition into gaussians of the emission spectrum of In6WO doped with terbium
// This spectrum has previously been entered into SplineFitDB.rdb has a TSplineFit
//of category 16, with name:
//   "Spectrum_In6WOTb" and title
//   "Emission Spectrum | indium tungstate doped with terbium"
// We give the same name and title to the TLitSpectrum created here.
//
// We use here the method TLitSpectrum::FindPeaks()" which has to be used when nothing
//is known in the litterature about the position of the individual peaks, only a graph
//of the emission is available. The ROOT class TSpectrum will find a possible decomposition
//into gaussian peaks.
//
// Notice that the call to TLitSpectrum::FindPeaks() produce a file "Spectrum.C",
//which you can use later on for a full definition of this TLitSpectrum, with
//decay times information. The macro produced with this file "Spectrum.C", after
//addition of the decay times, is called "LitSpectrum_In6WOTb.C"
//
  Bool_t ok;
  TLitSpectrum *sp;
  sp = new TLitSpectrum("Spectrum_In6WOTb","Emission Spectrum | indium tungstate doped with terbium",kTRUE);
  ok = sp->FindPeaks(75,0.001,10.0,16.0,kFALSE,kTRUE);
}
