{
//
// Macro to try many Litrani test macros
//
  const Bool_t litdebug = kTRUE;
  Int_t kClass,kLoad;
  //  gROOT->Reset();
  gROOT->SetBit(131072);  //to tell Litrani that LitDemos has been called
  kClass = TClassTable::GetID("TLitGlob");
  if (kClass<0) {
    kLoad = gSystem->Load("libTwoPad","",kTRUE);
    kLoad = gSystem->Load("libSplineFit","",kTRUE);
    kLoad = gSystem->Load("libLitrani","",kTRUE);
    if (litdebug) cout << "Start from ROOT, libLitrani to be loaded" << endl;
    switch (kLoad) {
    case -1:
      cout << "ERROR: Shared library libLitrani not found" << endl;
      break;
    case 0:
      if (litdebug) cout << "libLitrani correctly loaded" << endl;
      break;
    case 1:
      cout << "ERROR: Contradictory results from TClassTable::GetID()" << endl;
      cout << "                         and from gSystem->Load()" << endl;
      break;
    }
  }
  else {
    if (litdebug) cout << "Start from Litrani, libLitrani is already loaded" << endl;
    if (gLit) {
      delete gLit;
      gLit = 0;
      cout << "gLit deleted" << endl;
    }
  }
  gLit = new TLitGlob(5,30);
  gLit->BookCanvas();
  gTwoPad->CanDelete(kFALSE);
  delete gLit;
  gLit = 0;
  gTwoPad->SetButtonFast(12,"Transm1",        ".x Transm1.C");
  gTwoPad->SetButtonFast(13,"DamTransm1",     ".x DamTransm1.C");
  gTwoPad->SetButtonFast(14,"TransCone1",     ".x TransCone1.C");
  gTwoPad->SetButtonFast(15,"Transm2",        ".x Transm2.C");
  gTwoPad->SetButtonFast(16,"Transm3",        ".x Transm3.C");
  gTwoPad->SetButtonFast(17,"Anibm1",         ".x Anibm1.C");
  gTwoPad->SetButtonFast(18,"DamAnibm1",      ".x DamAnibm1.C");
  gTwoPad->SetButtonFast(19,"DamAnibm2",      ".x DamAnibm2.C");
  gTwoPad->SetButtonFast(20,"AniFibre",       ".x AniFibre.C");
  gTwoPad->SetButtonFast(21,"FibreWLS",       ".x FibreWLS.C");
  gTwoPad->SetButtonFast(22,"Gamma2",         ".x Gamma2.C");
  gTwoPad->SetButtonFast(23,"GLASTWithPINs",  ".x GLASTWithPINs.C");
  gTwoPad->SetButtonFast(24,"GLASTWithPMs",   ".x GLASTWithPMs.C");
  gTwoPad->SetButtonFast(25,"OxCryst",        ".x OxCryst.C");
  gTwoPad->SetButtonFast(26,"CMS1_muons",     ".x CMS1_muons.C");
  gTwoPad->SetButtonFast(27,"CMSgif_light",   ".x CMSgif_light.C");
  gTwoPad->SetButtonFast(28,"CMSEcalEBAPD",   ".x CMSEcalEBAPD.C");
  gTwoPad->SetButtonFast(29,"CMSEcalECVPT",   ".x CMSEcalECVPT.C");
  gTwoPad->UpdateCanvas();
}

