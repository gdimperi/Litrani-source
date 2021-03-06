{
//
//	BEGIN OF CMSEcalEBAPD.C
//
  const TString Tiret     = "_";
  const Short_t verbosity = 3;           //0-3
  Int_t    iwvlgth;
  Int_t    i1;
  char     ccc;
  char    *downcom;
  TString  FullName;
  Int_t    nbphot          = 50000;
  Int_t    typelight       = 0;
  Double_t ebeam           = 120.0;
  Int_t    iebeam;
  Double_t wvlgth          = 440.0;
  Short_t  DamageIntensity = 0; //[0-6] intensity of radiation damages in this shape
  TString  DamageFile = "RadDamage_Kim0";
  Bool_t isopbwo4 = kFALSE;
  cout << endl;
  cout << "PbWO4 is treated as birefringent. Do you want" << endl;
  cout << "an isotropic PbWO4, in order to speed up the program ?" << endl;
  cout << "Type y if yes : ";
  cin  >>ccc;
  if (ccc == 'y') {
    isopbwo4 = kTRUE;
    cout << "PbWO4 treated as isotropic" << endl;
  }
  else cout << "PbWO4 treated as birefringent" << endl;
  Short_t  kOptAxis = 0;  //Position of optical axis
  if (!isopbwo4) {
    while ((kOptAxis<1) || (kOptAxis>2)) {
      cout << endl;
      cout << "You have the choice between 2 positionning for" << endl;
      cout << " the optical axis:" << endl;
      cout << " (1) - like the russian crystals" << endl;
      cout << " (2) - like the chinese crystals" << endl;
      cout << "  Your choice : ";
      cin  >> kOptAxis;
    }
  }
  while ((typelight<1) || (typelight>2)) {
    cout << "You have the choice between 2 types of light emission" << endl;
    cout << " (1) : Light issued from Fibre"   << endl;
    cout << " (2) : Light generated by an e- electromagnetic shower" << endl;
    cout << "What is your choice ? ";
    cin  >> typelight;
  }
  if (typelight == 1) {
    cout << endl;
    cout << "Unit is 1000 photons (kilo-photons!)" << endl;
    cout << "How many kilo-photons do you want ? ";
    cin  >> nbphot;
    nbphot *= 1000;
    cout << endl;
    cout << "The wavelength for the led is : " << wvlgth << endl;
    cout << "Do you want to change the wavelength of the laser ? y or n ";
    cin  >> ccc;
    if (ccc=='y'){
      cout << "wavelength of the laser : ";
      cin  >> wvlgth;
    }
    iwvlgth = wvlgth;
  }
  else {
    cout << "The energy of the beam is : " << ebeam << endl;
    cout << "Do you want to change the energy of the beam ? y or n ";
    cin  >> ccc;
    if (ccc=='y') {
      cout << "Energy of the beam : ";
      cin  >> ebeam;
    }
  }
  iebeam = ebeam;
  cout << endl;
  cout << "You have the choice between 7 levels of irradiation damages" << endl;
  cout << "The level actually chosen is 0 : no radiation damage" << endl;
  cout << "Do you want to change it ? ";
  cin  >> ccc;
  if (ccc=='y') {
    DamageIntensity = -1;
    while ((DamageIntensity<0) || (DamageIntensity>6)) {
      cout << "Enter damage intensity [0-6] : ";
      cin  >> DamageIntensity;
    }
  }
  DamageFile += DamageIntensity;
  switch (typelight) {
    case 1:
      FullName = "EBlaser_";
      char *downcom = "Light issued from Fibre";
      i1 = TMath::Abs(iwvlgth);
      break;
    case 2:
      FullName = "EBebeam_";
      char *downcom = "e- electromagnetic shower";
      i1 = iebeam;
      break;
  }
  if (isopbwo4) FullName.Append('A');
  else {
    if (kOptAxis==1) FullName.Append('B');
    else             FullName.Append('C');
  }
  FullName += Tiret;
  FullName += i1;
  FullName += Tiret;
  FullName.Append("dam");
  FullName += DamageIntensity;
  if (verbosity>=2) {
    cout << endl;
    cout << "Root file: " << FullName.Data() << endl;
    cout << endl;
  }
  char *name    = FullName.Data();
  char *listing = FullName.Data();
  char *upcom   = "Simulation of CMS Endcap crystal";
  gROOT->ProcessLine(".x InitLitrani.C(6,name,listing,upcom,downcom,kTRUE,kFALSE,kTRUE)");
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
  gROOT->ProcessLine(".x CMSEcalEBAPD_setup.C");
//
// End setting setup
//
  Int_t    irun;
  Double_t photfac   = 0.05;
  Int_t    PrintFreq = 50000;
  gLit->SetPrintFreq(PrintFreq);
//
// Generation of photons
//
  Double_t xparam;
  Int_t nbrun;
  switch (typelight) {
   case 1:
     TSpontan *laser;
     laser = new TSpontan("laser","laser","Fibre",wvlgth,1);
     laser->DefineBeam(provided,MaxApert,"AngularDistrib_FibreCMS");
     cout << endl;
     cout << "How many IDENTICAL runs do you want : ";
     cin  >> nbrun;
     for (irun=1;irun<=nbrun;irun++) {
       xparam = irun;
       laser->Gen(irun,nbphot,xparam);
     }
     break;
   case 2:
      if (verbosity >= 1) then {
        cout << endl;
        cout << "  Entry point of beam : " << endl;
        start_casc->Print();
        cout << "Axis of beam" << endl;
        axis_casc->Print();
        cout << endl;
      }
      TElecCascade *beam;
      beam = new TElecCascade("Beam","Beam",ebeam,start_casc,axis_casc,"PbWO4","crystal",
                              kFALSE,photfac);
      cout << endl;
      cout << "How many electromagnetic showers do you want to generate: ";
      cin  >> nbrun;
      for (irun=1;irun<=nbrun;irun++) {
        xparam = irun;
        beam->Gen(irun,xparam);
      }
     break;
  }
//  It is a good idea to make a drawing of the setup. Any geometrical mistake will
//appear as an awful drawing ! Notice however that ROOT has a very strange and
//questionable way of drawing a setup : the proportion of the 3 axis are not
//respected by ROOT ! ROOT feels free to affect different factor to the 3 axis Ox,
//Oy and Oz ! So do not be surprised by the proportions !
  gLit->BookCanvas();
  node1->Draw();
  gTwoPad->ChangePad();
  switch (typelight) {
   case 1:
     gGp->SetTitle("run number");
     break;
   case 2:
     gGp->SetTitle("generated Cascade");
     break;
  }
  gGp->Summary(); //Summary() creates all summary histograms
  gGs->DoStat();
  gLit->CloseFiles();
}
