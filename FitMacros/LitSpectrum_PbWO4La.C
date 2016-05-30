TLitSpectrum* LitSpectrum_PbWO4La(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
{
//
// This macro has been produced using the file "Spectrum.C" produced by the macro
//"GaussDecomp_PbWO4La.C", introducing later on the decay times components
//
  char c;
  Bool_t ok;
  TLitSpectrum *sp;
  sp = new TLitSpectrum("Spectrum_PbWO4La","Emission Spectrum | Lead tungstate PbWO4 doped with lanthane");
  sp->AddOneComponent(420,35.4938722588,598715.663229,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->AddOneComponent(500,67.7989582402,251867.894271,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->AddOneComponent(437.620987064,19.5870778123,53885.7693666,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->AddOneComponent(492.366250454,27.1662871551,100601.517916,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->Normalize();
  sp->SetMacro("LitSpectrum_PbWO4La.C");
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
