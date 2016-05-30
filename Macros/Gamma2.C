{
  char *fname   = "gamma2";
  char *listing = "gamma2";
  char *upcom   = "Gammas in setup with 2 crystals";
  char *downcom = "Gammas loose energy only in fluorescent materials";
  gROOT->ProcessLine(".x InitLitrani.C(6,fname,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
  gLit->SetPrintFreq(2000);
  //
  // Gamma2
  //
  const Double_t pideg    = 180.0;
  const Int_t    nRuns    = 11;
  const Double_t gammaE   = 1.0;
  const Double_t nPhotMeV = 10000;
  const Double_t cryst_r  = 2.0;   // Crystal radius and rmin of TSTUBE refl
  const Double_t cryst_h  = 2.0;   // 1/2 Crystal height and TSTUBE refl
  const Double_t refl_r   = 2.5;   // rmax of TSTUBE refl Reflector
  const Double_t air_h    = 0.25;  // 1/2 Air gap thickness
  TResults::fgEnergyOfGamma = gammaE;
  Int_t i;
  TPhotoElecCompton *source;
  Int_t nBins = (Int_t)(gammaE*nPhotMeV/2.0);
  //
  // Materials
  //
  TOpticMaterial* cawo4 = new TOpticMaterial("CaWO4","CaWO4",kFALSE,1.0,100.0,-1,-1,288, 126, 6.06);
  cawo4->IsIsotropic(2.0);
  cawo4->FluoComponent(447.0, 25.0, 0.01, 2000.0);
  cawo4->FluoComponent(447.0, 25.0, 0.99, 320000.0);
  cawo4->SetPhotMev(nPhotMeV);
  cawo4->SetXSectnPE("PhotoEl_CaWO4");//Set the name of the TSplineFit for the photoelectric effect
  TLitPhys::Get()->SetLate(1000000000);
//
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
  TSCYL    *cryst     = new TSCYL("cryst","cryst","CaWO4","none",cryst_r,cryst_h);
  cryst->SetLineColor(2);
  cryst->SetLineWidth(2);
  TSNode   *cnode     = new TSNode("cnode","cnode","cryst");
  cnode->SetLineColor(2);
  cnode->SetLineWidth(2);
  cnode->cd();
  TSTUBE   *refl      = new TSTUBE("refl","refl","Air","TotAbs",cryst_r,refl_r,cryst_h);
  refl->SetLineColor(5);
  refl->SetLineWidth(2);
  TSNode   *rnode     = new TSNode("rnode","rnode","refl");
  TContact *CrystRefl = new TContact("CR","CR","cryst","refl",identical);
//
  TSCYL    *botcyl    = new TSCYL("Bot","Bot","refl","TotAbs",refl_r,air_h,kTRUE);
  botcyl->SetLineColor(1);
  botcyl->SetLineWidth(2);
  TSNode   *bnode     = new TSNode("bnode","bnode",botcyl, 0.0, 0.0, -cryst_h-air_h);
  TContact *BRefl     = new TContact("BR","BR","Bot","refl",containing);
  TContact *BCryst    = new TContact("BC","BC","Bot","cryst",containing);
//
  TSCYL    *topcyl    = new TSCYL("Top","Top","refl","TotAbs",refl_r,air_h,kTRUE);
  topcyl->SetLineColor(1);
  topcyl->SetLineWidth(2);
  TSNode   *tnode     = new TSNode("tnode","tnode",topcyl, 0.0, 0.0, cryst_h+air_h);
  TContact *TRefl     = new TContact("TR","TR","Top","refl",containing);
  TContact *TCryst    = new TContact("TC","TC","Top","cryst",containing);
//
  TSCYL    *cryst2    = new TSCYL("cryst2","cryst2","CaWO4","none",cryst_r,cryst_h);
  cryst2->SetLineColor(2);
  cryst2->SetLineWidth(2);
  TSNode   *cnode2    = new TSNode("cnode2","cnode2","cryst2",0.0,0.0,2*cryst_h + 2*air_h);
  TSTUBE   *refl2     = new TSTUBE("refl2","refl2","Air","TotAbs",cryst_r,refl_r,cryst_h);
  refl2->SetLineColor(5);
  refl2->SetLineWidth(2);
  TSNode   *rnode2    = new TSNode("rnode2","rnode2","refl2",0.0,0.0,2*cryst_h + 2*air_h);
  TContact *Crys2Refl = new TContact("CR2","CR2","cryst2","refl2",identical);
  TContact *TRefl2    = new TContact("TR2","TR2","Top","refl2",containing);
  TContact *TCrys2    = new TContact("TC2","TC2","Top","cryst2",containing);
//
  TSCYL    *topcyl2   = new TSCYL("Top2","Top2","refl","TotAbs",refl_r,air_h,kTRUE);
  topcyl2->SetLineColor(1);
  topcyl2->SetLineWidth(2);
  TSNode   *tnode2    = new TSNode("tnode2","tnode2",topcyl2, 0.0, 0.0, 3*cryst_h+3*air_h);
  TContact *TRefl3    = new TContact("TR3","TR3","Top2","refl2",containing);
  TContact *TCryst3   = new TContact("TC3","TC3","Top2","cryst2",containing);
//
  topcyl2->fSuppl->SetSurfDet("DetFace", "Det Face", 1);
  topcyl2->fSuppl->GetFace(1)->SetDetNumber(0);
//
//
  TH1F* spectrum = new TH1F("spectrum", "spectrum", nBins, 0, nBins*1.0);
//
//Generate events
//
  const T3Vector StartGamma(0.0, 0.0, -cryst_h - 0.5);
  T3Vector AxisGamma;
  Double_t ThetaDeg,Theta0,sTheta0,cTheta0;
  Double_t Phi0,sPhi0,cPhi0;
  Phi0 = 0.0;
  sPhi0 = TMath::Sin(Phi0);
  cPhi0 = TMath::Cos(Phi0);
  ThetaDeg = -50.0;
  Theta0 = (ThetaDeg*TMath::Pi())/pideg;
  sTheta0 = TMath::Sin(Theta0);
  cTheta0 = TMath::Cos(Theta0);
  AxisGamma.Set(sTheta0*cPhi0,sTheta0*sPhi0,cTheta0);
  source = new TPhotoElecCompton("Source","Source","cryst",gammaE,StartGamma,AxisGamma);
  //
  // Runs
  //
  for (i=1; i<=nRuns; ++i) {
    source->Gen(i,4,ThetaDeg);
    ThetaDeg += 10.0;
    Theta0 = (ThetaDeg*TMath::Pi())/pideg;
    sTheta0 = TMath::Sin(Theta0);
    cTheta0 = TMath::Cos(Theta0);
    AxisGamma.Set(sTheta0*cPhi0,sTheta0*sPhi0,cTheta0);
    source->SetGammaDirection(AxisGamma);
    if (gCs->fNpSeen) spectrum->Fill(gCs->fNpSeen);
  }
  //
  // Drawing
  //
  gLit->BookCanvas();
  gTwoPad->SelectPad2();
  cnode->Draw();
  gTwoPad->ChangePad();
  spectrum->Draw();
  gGp->SetTitle("Incident angle of gamma in degree");
  gGp->Summary();
  gGs->DoStat();
  gLit->CloseFiles();
}

