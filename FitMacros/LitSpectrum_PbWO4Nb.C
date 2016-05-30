TLitSpectrum* LitSpectrum_PbWO4Nb(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
{
//
// This macro has been produced using the file "Spectrum.C" produced by the macro
//"GaussDecomp_PbWO4Nb.C", introducing later on the decay times components
//
  char c;
  Bool_t ok;
  TLitSpectrum *sp;
  sp = new TLitSpectrum("Spectrum_PbWO4Nb","Emission Spectrum | Lead tungstate PbWO4 doped with niobium");
  sp->AddOneComponent(420,28.7662742323,453926.961802,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->AddOneComponent(500,40.3573207994,359351.517305,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->AddOneComponent(473.885930763,21.5692430907,129328.597403,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->AddOneComponent(449.194375232,15.5756853742,61281.5231796,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->Normalize();
  sp->SetMacro("LitSpectrum_PbWO4Nb.C");
  sp->fComment[0] = "Although PbWO4 is said to have 2 emission bands, one at 420 nm";
  sp->fComment[1] = "and the other at 500 nm, we have found that the global emission";
  sp->fComment[2] = "spectrum cannot be fitted well with only 2 gaussians. So we have";
  sp->fComment[3] = "made a fit with 4 gaussians, the 1st 2 peaks with mean values fixed";
  sp->fComment[4] = "at 420 and 500 nm, the other 2 auxiliary peaks having free mean";
  sp->fComment[5] = "values adjusted by the fit";
  if (todraw) {
    if (!gTwoPad) {
      gLit = new TLitGlob(5);
      gLit->BookCanvas();
    }
    sp->DrawDecayTimes();
    sp->DrawSpectrum();
    sp->Print();
  }
  if (infile) {
    if (firstinfile) sp->UpdateFile(kTRUE);
    else             sp->UpdateFile(kFALSE);
  }
  return sp;
}
