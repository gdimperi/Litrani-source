TLitSpectrum* LitSpectrum_CsITl(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
{
//
// Spectrum of CsI doped with Tallium measured by Remi Chipaux
//
  char c;
  Bool_t ok;
  TLitSpectrum *sp;
  sp = new TLitSpectrum("Spectrum_CsITl","Emission Spectrum | CsI doped with Tallium measured by R Chipaux");
  sp->AddOneComponent(410,40.0,0.333333,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
  sp->AddOneComponent(470,30.0,0.075,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
  sp->AddOneComponent(520,40.0,0.666666,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
  sp->Normalize();
  sp->SetMacro("LitSpectrum_CsITl.C");
  sp->SetSource("Remi Chipaux DAPNIA/SPP Saclay");
  sp->fComment[0] = "Measurements of Remi Chipaux for the GLAST experiment";
  sp->fComment[1] = "Other measurements available from John E. Eldridge";
  sp->fComment[2] = "Look at Spectrum_CsITlJE";
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
