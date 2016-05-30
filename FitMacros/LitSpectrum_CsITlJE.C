TLitSpectrum* LitSpectrum_CsITlJE(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
{
//
// Spectrum of CsI doped with Tallium measured by J.Elridge
//
  char c;
  Bool_t ok;
  TLitSpectrum *sp;
  sp = new TLitSpectrum("Spectrum_CsITlJE","Emission Spectrum | CsI doped with Tallium measured by J.Elridge");
  sp->AddOneComponent(410,40.0,0.333333,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
  sp->AddOneComponent(470,30.0,0.075,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
  sp->AddOneComponent(520,40.0,0.666666,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
  sp->Normalize();
  sp->SetMacro("LitSpectrum_CsITlJE.C");
  sp->SetSource("Jon E. Elridge");
  sp->fComment[0] = "Measurements by John E. Elridge";
  sp->fComment[1] = "Other measurements available from Remi Chipaux";
  sp->fComment[2] = "Look at Spectrum_CsITl";
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
