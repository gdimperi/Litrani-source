{
  char *name    = "OxCryst";
  char *listing = "OxCryst";
  char *upcom   = "Crystal Module Simulation";
  char *downcom = "Variation of Collection Efficiency with Generated x position";
  gROOT->ProcessLine(".x InitLitrani.C(8,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
//
// arguments of InitLitrani:
//
// size     = 0 | 1 | 2 | 3 according to the size you want for the canvas
// name     name of the produced ROOT file (without ".root")
// listing  name of the listing file (without ".lis")
// upcom    name of the comment appearing on top    left of canvas
// downcom  name of the comment appearing on bottom left of canvas
// otherseq true if one wants an other sequence of random numbers at each new launching
// indebug  true if one wants to run Litrani2 in debug mode
// WithDate if true, appends date and time to name of root file and name of listing
//
//
// OxCryst Test
//
Int_t nPhot = 10000;
//
// Materials
//
TOpticMaterial *pbwo4 = 
  new TOpticMaterial("PbWO4", "PbWO4 crystal", kFALSE, 1.0,
		     "AbsLengthTensor_PbWO4__000", "AbsLengthTensor_PbWO4__002");
pbwo4->IsUniAxialNegBirefr("DielTensor_PbWO4__000","DielTensor_PbWO4__002");
TOpticMaterial *air = new TOpticMaterial("Air", "Air", kFALSE, 1.0, 10000.0);
air->IsIsotropic(1.0);
TOpticMaterial *silicium = new TOpticMaterial("Silicium","Silicium detector",
					      kTRUE, 1.0, "AbsorptionLength_Silicium");
silicium->IsIsotropic("RefrIndex_Silicium");
TRevetment *totabsorbing = new TRevetment("TotAbsorbing",
					  "Totally absorbing revetment",
					  "none", 0.0, 2.0, 6.0, 1.0, 1.0);
TRevetment *aluminium = new TRevetment("Aluminium", "Aluminium", "none", 0.0,
				       "RIndexRev_Aluminium", "IIndexRev_Aluminium",1.0);
//
// Shapes
//
TSCYL  *det=0;
TSTUBE *airdet    = 0;
TSCYL  *crystal   = 0;
TSCYL  *airgap    = 0;
TSCYL  *airtop    = 0;
TSTUBE *airaround = 0;
//
// Nodes
//
TSNode *node1=0; // detector
TSNode *node2=0; // air around detector
TSNode *node3=0; // airgap top
TSNode *node4=0; // crystal
TSNode *node5=0; // air around
TSNode *node6=0; // air top
//
//Contact Faces
//
TContact *det_airdet           = 0;
TContact *det_airgap           = 0;
TContact *airdet_airgap        = 0;
TContact *airgap_crystal       = 0;
TContact *airgap_airaround     = 0;
TContact *airaround_crystal    = 0;
TContact *airaround_airtop     = 0;
TContact *crystal_airtop       = 0;
//
// Source
//
TSpontan *source =0;
//
// Geometry
//
Double_t crystal_r = 2.0;                                   // crystal radius
Double_t crystal_h = 2.1;                                   // crystal height
Double_t det_r     = 2.0;                                   // start detector radius
Double_t det_h     = 0.05;                                  // detector thickness
Double_t airtop_h  = 0.07;
Double_t airsize   = 0.2;                                   // vertical air sizes
Double_t airthick  = 0.5;                                   // start air around thickness
Double_t top_r     = airthick + crystal_r;                  // start overall radius

Double_t d_r       = det_r;                                 // detector radius
Double_t ag_thick  = airthick;                              // air around thickness
Double_t ag_h      = airsize/2.0;                           // airgap height
Double_t ag_z      = ag_h + det_h;                          // airgap size
Double_t cr_z      = crystal_h + airsize + det_h;           // position of crystal
Double_t at_z      = ag_h +2.0*crystal_h + airsize + det_h; // air top position
Double_t t_r       = top_r;                                 // overall radius
Int_t    run       = 0;                                     // run counter over height
Int_t    lastrun   = 19;
Double_t gen_x     = 0.0;
Double_t gen_z     = 0.0;
Double_t wvl       = 420;
Double_t theta     = 0;
Double_t phi       = 0;
while (run < lastrun) { // airgap cryst-det
  // Geometry
  ag_z = ag_h + det_h;                               // airgap size
  cr_z = crystal_h + 2.0*ag_h + det_h;               // position of crystal
  at_z = airtop_h +2.0*crystal_h + 2.0*ag_h + det_h; // air top position
  t_r = ag_thick + crystal_r;                        // airgap cryst-refl
  // Shapes and nodes
  det = new TSCYL("det", "detector", "Silicium", "TotAbsorbing", d_r, det_h);
  det->fSuppl->SetSurfDet("Det Face", "Det Face", 0);
  det->fSuppl->GetFace(1)->SetDetNumber(0);
  node1 = new TSNode("node1", "node1", det);
  node1->SetLineColor(3);
  node1->cd();
  airdet = new TSTUBE("airdet", "air around detector", "Air", 
		      "none", d_r, t_r, det_h);
  node2 = new TSNode("node2", "node2", airdet, 0, 0, 0);
  airgap =  new TSCYL("airgap", "airgap", "airdet", 
		      "Aluminium", t_r, ag_h, kTRUE);
  node3 = new TSNode("node3", "node3", airgap, 0.0, 0.0, ag_z);
  crystal = new TSCYL("crystal", "scintillator crystal",
		      "PbWO4", "none", crystal_r, crystal_h);
  crystal->SetOpticalAxis(90+theta, phi, 90, 90+phi);
  node4 = new TSNode("node4", "node4", crystal, 0.0, 0.0, cr_z);
  airaround = new TSTUBE("airaround", "airaround", "airgap", 
			 "Aluminium", crystal_r, t_r, crystal_h, kTRUE);
  node5 = new TSNode("node5", "node5", airaround, 0.0, 0.0, cr_z);
  airtop = new TSCYL("airtop", "airtop", "Air", "Aluminium", 
		     t_r, airtop_h, kFALSE);
  airtop->fSuppl->GetFace(1)->ChangeRevetment("TotAbsorbing");
  node6 = new TSNode("node6", "node6", airtop, 0.0, 0.0, at_z);
  cout << "Finished nodes and shapes" << endl;
  // Contact faces
  //Contained: face F1 of shape 1 is contained inside face F2 of shape 2
  //Containing: face F1 of shape 1 contains entirely face F2 of shape 2
  det_airdet = new TContact("det_airdet", "det_airdet", 
			    "det", "airdet", identical);
  det_airgap = new TContact("det_airgap", "det_airgap", 
			    "det", "airgap", contained);
  airdet_airgap = new TContact("airdet_airgap", "airdet_airgap", 
			       "airdet", "airgap", contained);
  airgap_crystal = new TContact("airgap_crystal", "airgap_crystal", 
				"airgap", "crystal", containing);
  airgap_airaround = new TContact("airgap_airaround", "airgap_airaround", 
				  "airgap", "airaround", containing);
  airaround_crystal = new TContact("airaround_crystal", "airaround_crystal", 
				   "airaround", "crystal", identical);
  airaround_airtop = new TContact("airaround_airtop", "airaround_airtop", 
				  "airaround", "airtop", contained);
  crystal_airtop = new TContact("crystal_airtop", "crystal_airtop", 
				"crystal", "airtop", contained);
  cout << "Finished contacts" << endl;
  // Generate events
  source = new TSpontan("Spontan", "Source inside crystal", "crystal",
			gen_x, 0, gen_z, wvl);
  if (!run) source->Gen(1+run, 1, gen_x);
  else source->Gen(100*run, nPhot, gen_x);
  cout << "Finished generation" << endl;
  // Make histograms right
  if (!run) {
    gGs->fHAbsAny->SetBins(50, 0, 20);
    gGs->fHAbsMat->SetBins(50, 0, 20);
    gGs->fHAbsBef->SetBins(50, 0, 20);
    gGs->fHAbsRvt->SetBins(50, 0, 20);
    gGs->fHOutSide->SetBins(50, 0, 20);
    gGs->fHAbnLost->SetBins(50, 0, 20);
    gGs->fHTooLate->SetBins(50, 0, 20);
    gGs->fHDiffusion->SetBins(50, 0, 20);
    gGs->fHPoints->SetBins(50, 0, 20);}
  if (run != lastrun-1){
    // Clean up for next run
    delete source;
    source =0; 
    if (det_airdet) delete det_airdet;
    det_airdet = 0;
    if (det_airgap) delete det_airgap;
    det_airgap = 0;
    if (airdet_airgap) delete airdet_airgap;
    airdet_airgap = 0;
    if (airgap_crystal) delete airgap_crystal;
    airgap_crystal = 0;
    if (airgap_airaround) delete airgap_airaround;
    airgap_airaround= 0;
    if (airaround_crystal) delete airaround_crystal;
    airaround_crystal= 0;
    if (airaround_airtop) delete airaround_airtop;
    airaround_airtop= 0;
    if (crystal_airtop) delete crystal_airtop;
    crystal_airtop= 0;
    if (node6) delete node6;
    node6 = 0;
    if (node5) delete node5;
    node5 = 0;
    if (node4) delete node4;
    node4 = 0;
    if (node3) delete node3;
    node3 = 0;
    if (node2) delete node2;
    node2 = 0;
    if (airtop) delete airtop;
    airtop = 0;
    if (airaround) delete airaround;
    airaround = 0;
    if (crystal) delete crystal;
    crystal = 0;
    if (airgap) delete airgap;
    airgap = 0;
    if (airdet) delete airdet;
    airdet =0;
    gLit->fDetector.RemoveAt(0);
    gLit->fNDetector =0; // Needed otherwise error in TResults::CInitStatDet()
    if(det) delete det;
    det =0;}
  cout << "fNpSeen:    " << gCs->fNpSeen    << endl;
  cout << "fNpLossAny: " << gCs->fNpLossAny << endl;
  cout << "fNpAbsMat:  " << gCs->fNpAbsMat  << endl;
  cout << "fNpAbsBef:  " << gCs->fNpAbsBef  << endl;
  cout << "fNpAbsRvt:  " << gCs->fNpAbsRvt  << endl;
  cout << "fNpOutSide: " << gCs->fNpOutSide << endl;
  cout << "fNpAbnorm:  " << gCs->fNpAbnorm  << endl;
  cout << "fNpTooLate: " << gCs->fNpTooLate << endl;
  cout << "fNpTooFew:  " << gCs->fNpTooFew  << endl;
  cout << "fNpLossAng: " << gCs->fNpLossAng << endl;
  cout << "fNpLossQE:  " << gCs->fNpLossQE  << endl;
  gen_x+=0.1;
  run++;
}
//
// Draw geometry
//
crystal->SetLineColor(2);
node4->SetLineColor(2);
det->SetLineColor(3);
gLit->BookCanvas();
gTwoPad->SelectPad2();
node1->Draw();
gTwoPad->ChangePad();
//
// Draw histograms
//
gGp->Summary();
gTwoPad->CommentTL("GenX");
gGs->DoStat();
gLit->CloseFiles();
}
