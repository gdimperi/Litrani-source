{
char *fname   = "WLS";
char *listing = "WLS";
char *upcom   = "WLS Fibre Simulation";
char *downcom = "WLS Fibre Macro";
gROOT->ProcessLine(".x InitLitrani.C(5,fname,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
//
Int_t nPhot = 10000;
//
// Spectra
//
TLitSpectrum *plexiSp;
plexiSp = new TLitSpectrum("Plexi Spectrum","Plexi Emission Spectrum");
plexiSp->AddOneComponent(550.0,10.0,1.0,2.0,-1.0,1.0);
plexiSp->Normalize(); 
TLitSpectrum *spontanSp;
spontanSp = new TLitSpectrum("Fake Spectrum","Fake Emission Spectrum");
spontanSp->AddOneComponent(420.0,10.0,1.0,2.0,-1.0,1.0);
spontanSp->Normalize(); 
//spontanSp->PrintSpectrum2File("spontan.dat", 10,1);
//
// Materials
//
TOpticMaterial *plexi;
plexi = new TOpticMaterial("Plexi","Plexi",kFALSE,1.0,1000.0);
plexi->IsIsotropic(1.6);
plexi->SetWLS(plexiSp,"AbsLengthWLS_Fibre");
plexi->SetRayleighLength(40.0);
//plexi->SetDiffusionLength(40.0);
plexi->SetLitSpectrum(spontanSp);//Trick for TSpontan
TOpticMaterial *air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0,-1.0,-1.0,14.6,7.3,0.00129);
//TOpticMaterial *air = new TOpticMaterial("Air",14.6, 7.3, 0.00129);
air->IsIsotropic(1.0);
air->SetLitSpectrum(spontanSp);//Trick for TSpontan
gOutside = air;
//
// Shapes
//
//  ******* Define general properties of the setup ********** //
//
Double_t fibre_r = 0.1/2.0;  //fibre radius
Double_t fibre_h = 40.0/2.0; //fibre height
Double_t air_r   = 1.0;
TSCYL* fibre = new TSCYL("fibre","fibre","Plexi","none",fibre_r,fibre_h);
fibre->SetLineColor(2);
fibre->SetLineWidth(2);
fibre->fSuppl->SetSurfDet("Det Face", "Det Face", 1);
fibre->fSuppl->GetFace(1)->SetDetNumber(0);
TSNode* fnode = new TSNode("fnode","fnode","fibre");
fnode->SetLineColor(2);
fnode->SetLineWidth(2);
fnode->cd();
TSTUBE* airtube = new TSTUBE("airtube","airtube","Air","none",fibre_r,fibre_r+air_r,fibre_h);
airtube->SetLineColor(1);
airtube->SetLineWidth(1);
TSNode* airnode = new TSNode("airnode","airnode","airtube");
TContact* fa = new TContact("fa","fa","fibre","airtube",identical);
// gLit->CheckContactsAndOverlaps();
//
// Drawing
//
gLit->BookCanvas();
gTwoPad->SelectPad2();
fnode->Draw();
gTwoPad->ChangePad();
//
//Generate events
//
Double_t posy = fibre_r + 0.01;
TSpontan* source = new TSpontan("Source","Source","airtube",0.0,-posy,0.0);
Double_t thetabeam = 90.0;
Double_t phibeam = 90.0;
source->DefineBeam(flat,0.0,thetabeam,phibeam);
source->Gen(1, nPhot);
//
//Summary
//
gGp->Summary();
gCs->DoStat();
gLit->CloseFiles();
if (gCs->fNpSeen) cout << "Efficiency: " << (1.0*gCs->fNpSeen)/(nPhot*1.0) << endl;
}

