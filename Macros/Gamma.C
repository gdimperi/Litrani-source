{
  char *fname   = "gamma";
  char *listing = "gamma";
  char *upcom   = "Crystal Module Simulation";
  char *downcom = "Crystal Development Macro";
  gROOT->ProcessLine(".x InitLitrani.C(5,fname,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
  gLit->SetPrintFreq(2000);
  //
  // Source option
  // default = TSpontan
  // 1 = TPhotoElecCompton
  TPhotoElecCompton *source;
  Int_t nRuns = 10;
  //Histos and stuff
  Double_t gammaE = 0.200;
  Double_t nPhotMeV = 10000;
  Int_t nBins = (int)(gammaE*nPhotMeV/2.0);
  //
  // Materials
  //
  TOpticMaterial* cawo4 = new TOpticMaterial("CaWO4","CaWO4",kFALSE,1.0,100.0,-1,-1,288, 126, 6.06);
  cawo4->IsIsotropic(2.0);
  cawo4->FluoComponent(447.0, 25.0, 0.01, 2000.0);
  cawo4->FluoComponent(447.0, 25.0, 0.99, 320000.0);
  cawo4->SetPhotMev(nPhotMeV);
  cawo4->SetXSectnPE("PhotoEl_CaWO4");
  TLitPhys::Get()->SetLate(1000000000);
  TOpticMaterial* air = new TOpticMaterial("Air", "Air", kFALSE, 1.0, 10000.0);
  air->IsIsotropic(1.0);
  TRevetment* totabsorbing = new TRevetment("TotAbs",
    "Totally absorbing revetment",
    "none", 0.0, 2.0, 6.0, 1.0, 1.0);
  TRevetment* aluminium = new TRevetment("Aluminium", "Aluminium", "none", 0.0,
    "RIndexRev_Aluminium", 
    "IIndexRev_Aluminium",1.0);
  //
  // Shapes
  //
  //  ******* Define general properties of the setup ********** //
  Double_t cryst_r = 4.0/2.0;  // Crystal radius
  Double_t cryst_h = 4.0/2.0;  // Crystal height
  Double_t refl_r  = 5.0/2.0;  // Reflector position
  Double_t air_h   = 0.5/2.0;  // Air gap thickness
  TSCYL* cryst = new TSCYL("cryst","cryst","CaWO4","none",cryst_r,cryst_h);
  TSNode* cnode = new TSNode("cnode","cnode","cryst");
  cnode->SetLineColor(1);
  cnode->SetLineWidth(2);
  cnode->cd();
  TSTUBE* refl = new TSTUBE("refl","refl","Air","TotAbs",cryst_r,refl_r,cryst_h);
  TSNode* rnode = new TSNode("rnode","rnode","refl");
  TContact* CrystRefl = new TContact("CR","CR","cryst","refl",identical);
  TSCYL* botcyl = new TSCYL("Bot","Bot","refl","TotAbs",refl_r,air_h,kTRUE);
  TSNode* bnode = new TSNode("bnode","bnode",botcyl, 0.0, 0.0, -cryst_h-air_h);
  TContact* BRefl = new TContact("BR","BR","Bot","refl",containing);
  TContact* BCryst = new TContact("BC","BC","Bot","cryst",containing);
  TSCYL* topcyl = new TSCYL("Top","Top","refl","TotAbs",refl_r,air_h,kTRUE);
  TSNode* tnode = new TSNode("tnode","tnode",topcyl, 0.0, 0.0, cryst_h+air_h);
  TContact* TRefl = new TContact("TR","TR","Top","refl",containing);
  TContact* TCryst = new TContact("TC","TC","Top","cryst",containing);
  topcyl->fSuppl->SetSurfDet("Det Face", "Det Face", 1);
  topcyl->fSuppl->GetFace(1)->SetDetNumber(0);
  //Generate events
  T3Vector StartCasc(0.0, 0.0, -cryst_h - 0.5);
  const Double_t Theta0 = 0.01;
  Double_t sTheta0,cTheta0;
  sTheta0 = TMath::Sin(Theta0);
  cTheta0 = TMath::Cos(Theta0);
  const Double_t Phi0 = 0.0;
  Double_t sPhi0,cPhi0;
  sPhi0 = TMath::Sin(Phi0);
  cPhi0 = TMath::Cos(Phi0);
  T3Vector AxisCasc(sTheta0*cPhi0,sTheta0*sPhi0,cTheta0);
  source = new TPhotoElecCompton("Source","Source","cryst",gammaE,StartCasc,AxisCasc);
  //
  // Results
  //
  TH1F* spectrum = new TH1F("spectrum", "spectrum", nBins, 0, nBins*1.0);
  //
  // Runs
  //
  for (Int_t i=0; i<nRuns; ++i){
    //cout << "Run number: " << i << " " << endl;
    source->Gen(101+i,4);
    // PMT spectrum
    if (gCs->fNpSeen) spectrum->Fill(gCs->fNpSeen);
    // Generation of photons
  }
  //
  // Drawing
  //
  gLit->BookCanvas();
  gTwoPad->SelectPad2();
  cnode->Draw();
  gTwoPad->ChangePad();
  spectrum->Draw();
  gGp->SetTitle("Run number");
  gGp->Summary();
  gGs->DoStat();
  gLit->CloseFiles();
}
