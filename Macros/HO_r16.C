 {
 // We call gROOT->Reset() if bit 16 not set. See explanation in InitLitrani.C
   if (!gROOT->TestBit(65536)) {
    gROOT->Reset();
    cout << endl;
    cout << "  --> gROOT->Reset() called" << endl;
   }
   char *name    = "HO_r15";
   char *listing = "HO_r15";
   char *upcom   = "Transmission study for HO light guides";
   char *downcom = "HO_r15";
   gROOT->ProcessLine(".x InitLitrani.C(1,name,listing,upcom,downcom,kFALSE,kFALSE,kFALSE)");

   TOpticMaterial *plexi;
   plexi = new TOpticMaterial("Plexi","Plexi",kFALSE,1.0,0.001);
   plexi->IsIsotropic(1.5);
   TOpticMaterial *air;
   air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0);
   air->IsIsotropic(1.0);
//------------------------------------------------------------------------------------------
// Definition of the revetment aluminum
   TRevetment *aluminium;
   aluminium = new TRevetment("Aluminium","Aluminium","Air",0.0,"RIndexRev_Aluminium","IIndexRev_Aluminium",1.0);
//------------------------------------------------------------------------------------------
//  External environment is explicitly defined to be Air:
   delete gOutside;
   gOutside = new TOpticMaterial("Outside","Air taken as outside material",kFALSE,1.0,10000.0); 
   gOutside->IsIsotropic(1.0);     
   TRevetment *totabsorbing;
   totabsorbing = new TRevetment("TotAbsorbing","Totally absorbing revetment");
//
//------------------------------------------------------------------------------------------
//  Geometry
//
  const Double_t zero   = 0.0;
  const Double_t detec_dz = 0.005;   //half length in z of TSCYL with surface detector
  const Double_t detec_dx = 0.1;
  const Double_t detec_dy = 0.1;
  const Double_t detec_r = 0.141;
  const Double_t trap_dx1 = 0.15;
  const Double_t trap_dx2 = 0.1;
  const Double_t trap_dy1 = 0.15;
  const Double_t trap_dy2 = 0.1;
  const Double_t trap_dz = 0.025;
  const Double_t lg_dz = 0.05;
  //  const Double_t lg_dz = 2.0;
  const Double_t lg_r1 = 0.15;
  const Double_t lg_r2 = 0.141;
  const Double_t fiber_dz = 0.5;
  const Double_t fiber_r = 0.047;
  //  const Double_t fiber_r = 0.0047;
  Double_t z_source;               //position in z of point source in TSCYL
  Double_t x_source;
  Double_t y_source;
  //  x_source = 0.1;
  x_source = 0.0;
  y_source = 0.0;
  z_source = 0.0;
  TOpticMaterial *coremat;
  coremat = new TOpticMaterial("coremat","coremat",kFALSE,1.0,10000);
  coremat->IsIsotropic(1.59);
  TOpticMaterial *cladmat;
  cladmat = new TOpticMaterial("cladmat","cladmat",kFALSE,1.0,1000);
  cladmat->IsIsotropic(1.39);
  TOpticMaterial *lgfill;
  lgfill = new TOpticMaterial("lgfill","lgfill",kFALSE,1.0,1000);
  lgfill->IsIsotropic(1.59);
  //lgfill->IsIsotropic(1.0);
  TSCYL *fiber;
  fiber = new TSCYL("fiber","fiber","Plexi","TotAbsorbing",fiber_r,fiber_dz);
  fiber->SetLineColor(1);
  fiber->SetLineWidth(2);
  TSCONE *lg;
  lg = new TSCONE("lg","lg","lgfill","Aluminium",lg_r1,lg_r2,lg_dz);
  lg->SetLineColor(2);
  lg->SetLineWidth(2);
  TSCYL *detecbrick;
  detecbrick = new TSCYL("detecbrick","detecbrick","Air","Aluminium",detec_r,detec_dz);
  detecbrick->fSuppl->SetSurfDet("perfect","perfect detector",1);
  detecbrick->SetLineColor(3);
  detecbrick->SetLineWidth(2);
  TSNode *node1;
  node1 = new TSNode("node1","node1",fiber);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);
  TSNode *node3;
  node3 = new TSNode("node3","node3",lg,x_source,y_source,fiber_dz+lg_dz);
  TSNode *node5;
  node5 = new TSNode("node5","node5",detecbrick,x_source,y_source,fiber_dz+2*lg_dz+detec_dz);
  TContact *contact13;
  contact13 = new TContact("fiber-lg","fiber-lg","fiber","lg",contained);
  TContact *contact35;
  contact35 = new TContact("lg_detecbrick","lg_detecbrick","lg","detecbrick",identical);
  gLit->fMainPad2->cd();
  gLit->fUsePad2 = kTRUE;
  node1->Draw();
  gLit->ChangePad();
  laser = new TSpontan("laser","laser","fiber",520,1);
  laser->DefineBeam(provided,30.0,"AngularDistrib_FibreCMS");
  laser->Gen(2,100000);
  gGp->SetTitle("wavelength of laser");
  gGp->Summary();
  //  gGp->fHEfficiency->Fit("pol4","Q");
  gGs->DoStat();
  gLit->CloseFiles();
  gLit->HistosManager();
}

 
