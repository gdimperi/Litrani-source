// This macro rebuilds the file SpectraDB.rdb
//
{
  Int_t k,error1,nbspectrum;
  Bool_t ok;
  TLitSpectrum *spectrum = 0;
  nbspectrum = 0;
  //
  k = gROOT->LoadMacro("LitSpectrum_CsITl.C",&error1,kFALSE);
  if (k) gApplication->Terminate();
  spectrum = LitSpectrum_CsITl(kFALSE,kTRUE,kTRUE);
  nbspectrum++;
  cout << nbspectrum << "   LitSpectrum_CsITl" << " in SpectraDB.rdb" << endl;
  delete spectrum;
  //
  k = gROOT->LoadMacro("LitSpectrum_CsITlJE.C",&error1,kFALSE);
  if (k) gApplication->Terminate();
  spectrum = LitSpectrum_CsITlJE(kFALSE,kTRUE,kFALSE);
  nbspectrum++;
  cout << nbspectrum << "   LitSpectrum_CsITlJE" << " in SpectraDB.rdb" << endl;
  delete spectrum;
  //
  k = gROOT->LoadMacro("LitSpectrum_In6WOTb.C",&error1,kFALSE);
  if (k) gApplication->Terminate();
  spectrum = LitSpectrum_In6WOTb(kFALSE,kTRUE,kFALSE);
  nbspectrum++;
  cout << nbspectrum << "   LitSpectrum_In6WOTb" << " in SpectraDB.rdb" << endl;
  delete spectrum;
  //
  k = gROOT->LoadMacro("LitSpectrum_PbWO4La.C",&error1,kFALSE);
  if (k) gApplication->Terminate();
  spectrum = LitSpectrum_PbWO4La(kFALSE,kTRUE,kFALSE);
  nbspectrum++;
  cout << nbspectrum << "   LitSpectrum_PbWO4La" << " in SpectraDB.rdb" << endl;
  delete spectrum;
  //
  k = gROOT->LoadMacro("LitSpectrum_PbWO4Nb.C",&error1,kFALSE);
  if (k) gApplication->Terminate();
  spectrum = LitSpectrum_PbWO4Nb(kFALSE,kTRUE,kFALSE);
  nbspectrum++;
  cout << nbspectrum << "   LitSpectrum_PbWO4Nb" << " in SpectraDB.rdb" << endl;
  delete spectrum;
  //
  cout << "End of entering spectra in file" << endl;
  ok = TLitSpectrum::OrderFile(kTRUE);
  if (ok) cout << "SpectraDB.rdb has been correctly ordered" << endl;
  else    cout << "Problem in ordering SpectraDB.rdb" << endl;
}
