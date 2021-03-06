{
//
//	BEGIN OF CMSDrc_light.C
//	Version 2.01, 2004/04/29
//
  const    Double_t zero     = 0.0;
  const    Double_t un       = 1.0;
  const    Double_t minus_un = -un;
  const    Double_t deux     = 2.0;
  const    Double_t squareroot2 = TMath::Sqrt(deux);
  const    Double_t pi       = TMath::Pi();
  const    Int_t ZmaxLR      = 24;
  const    Int_t ndatafile   = 7;
  const    Int_t ndata0      = 0;
  const    TString Tiret     = "_";
  const    TString Damagefilesprefix = "RadDamage_Kim0";
  const    TString Labsfilesprefix = "AbsorptionLength_crys8981R0";
  Bool_t   damok;
  Int_t    PrintFreq = 10000;
  Double_t calcsize;
  Int_t    iwvlgth;
  Int_t    irun;
  Int_t    idata;
  Int_t    jdata;
  Int_t    i1;
  char     ccc;
  char  *downcom;
  TString  DamageName;
  TString  Damagefile;
  TString  Labsfile;
  TString  FullName;
  TString  DamageName;
  Int_t    nbphot          = 50000;
  Int_t    typelight       = 0;
  Double_t ebeam           = 120.0;
  Double_t ebeamirrad      = 120.0;
  Double_t PhotMeV         = 100.0;
  Double_t photfac         = 0.002;
  Short_t  crystaltype     = 0;
  Double_t wvlgth          = -465.0;
  Int_t    typerevcrystal  = 0;
  Int_t    typeirrad       = 0;	// 1: flat; 2: e-beam
  Short_t  verbosity       = 1;
  Double_t efficiency[ndatafile];
  Double_t erroreff[ndatafile];
  Double_t dosis[ndatafile];
  while ((typelight<1) || (typelight>4)) {
    cout << "You have the choice between 4 types of light:" << endl;
    cout << " (1) : led     Light issued from Fibre1"   << endl;
    cout << "                wavelength fixed or spectrum of hcp200" << endl;
    cout << " (2) : laser   Light issued from a disk inside the crystal" << endl;
    cout << "                spectrum of the PbWO4" << endl;
    cout << " (3) : shower  Light generated by an e- electromagnetic shower" << endl;
    cout << "                spectrum of the PbWO4" << endl;
    cout << "                axis of shower (0,0,1)" << endl;
    cout << " (4) : shower  Light generated by an e- electromagnetic shower" << endl;
    cout << "                spectrum of the PbWO4" << endl;
    cout << "                axis of shower = axis_casc" << endl;
    cout << "What is your choice ? ";
    cin  >> typelight;
  }
  if (typelight < 3) {
    cout << "How many photons do you want ? ";
    cin  >> nbphot;
  }
  while ((crystaltype<1) || (crystaltype>17)) {
    cout << "Crystal Type for CMS are from 1 to 17" << endl;
    cout << "What is your choice ? ";
    cin  >> crystaltype;
  }
  while ((typeirrad<1) || (typeirrad>2)) {
    cout << "You can choose between 2 types of irradiation:" << endl;
    cout << " (1) - flat   : homogeneous irradiation" << endl;
    cout << " (2) - ebeam  : e- beam irradiation" << endl;
    cout << "What is your choice ? ";
    cin  >> typeirrad;
  }
  switch (typelight) {
    case 1:
      cout << "The wavelength for the led is : " << wvlgth << endl;
      cout << "Do you want to change the wavelength of the laser ? y or n ";
      cin  >> ccc;
      if (ccc=='y'){
        cout << "wavelength of the laser : ";
        cin  >> wvlgth;
      }
      iwvlgth = wvlgth;
      break;
    case 2:
      break;
    case 3:
    case 4:
      cout << "The energy of the beam is : " << ebeam << endl;
      cout << "Do you want to change the energy of the beam ? y or n ";
      cin  >> ccc;
      if (ccc=='y') {
        cout << "Energy of the beam : ";
        cin  >> ebeam;
      }
      break;
  }
  iebeam = ebeam;
  while ((typerevcrystal<1) || (typerevcrystal>7)) {
    cout << "There are 7 possible wrappings for the crystal :" << endl;
    cout << " (1) : tyvek" << endl;
    cout << " (2) : 100% absorbing" << endl;
    cout << " (3) : perfect diffusor" << endl;
    cout << " (4) : perfect reflector" << endl;
    cout << " (5) : pseudo tyvek" << endl;
    cout << " (6) : alveola" << endl;
    cout << " (7) : none" << endl;
    cout << "What is your choice ? ";
    cin  >> typerevcrystal;
  }
  switch (typeirrad) {
    case 1:	//	homogeneous irradiation
      DamageName = "flat";
      break;
    case 2:	//	ebeam irradiation
      DamageName  = "e-beam ";
      DamageName += ebeamirrad;
      DamageName += " GeV";
      break;
  }
  switch (typelight) {
    case 1:
      FullName = "led_";
      char *downcom = "led     Light issued from Fibre1";
      i1 = TMath::Abs(iwvlgth);
      break;
    case 2:
      FullName = "UV_laser_";
      char *downcom = "laser   Light issued from a disk inside the crystal";
      i1 = 0;
      break;
    case 3:
      FullName = "e_beam_";
      char *downcom = "shower  e- electromagnetic shower axis (0,0,1)";
      i1 = iebeam;
      break;
    case 4:
      FullName = "e_beam2_";
      char *downcom = "shower  e- electromagnetic shower axis axis_casc";
      i1 = iebeam;
      break;
  }
  FullName += crystaltype;
  FullName += Tiret;
  FullName += typerevcrystal;
  FullName += Tiret;
  FullName += typeirrad;
  FullName += Tiret;
  FullName += i1;
  char *name    = FullName.Data();
  char *listing = FullName.Data();
  char *upcom   = "Simulation of GIF setup of experiment CMS";
  gROOT->ProcessLine(".x InitLitrani.C(6,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
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
  for (idata = 0; idata < ndatafile; idata++) dosis[idata] = idata + 1;
  gLit->SetPrintFreq(PrintFreq);
  gROOT->ProcessLine(".x CMSDrc_setup.C");
  T3Vector zAxis(zero,zero,un);
  T3Vector start_casc(center_inter_x, center_inter_y, center_inter_z);
  T3Vector end_casc(center_exter_x, center_exter_y, center_exter_z);
  T3Vector axis_casc = end_casc - start_casc;
  axis_casc = axis_casc->Unit();
  T3Vector start_dam = start_casc;
  T3Vector axis_dam = axis_casc;
  T3Vector start_shower, axis_shower;
  switch (typelight) {
    case 1:	//	led
      TSpontan *led;
      if (iwvlgth > 0) led = new TSpontan("Led","Led","Fibre",wvlgth,1);
      else led = new TSpontan("Led","Led","Fibre",1);
      led->DefineBeam(provided,22.5,"AngularDistrib_FibreCMS");
      break;
    case 2:	//	laser
      TSpontan *laser;
      laser = new TSpontan("Laser","Laser","Crystal",center4_x,
                           center4_y,-crys_dz+0.01);
      laser->DefineCradle("Crystal",disk,fibre_r); 
      laser->DefineBeam(on4pi,180.0);
      break;
    case 3:	//	shower
      start_shower = start_casc;
      axis_shower = zAxis;
      if (verbosity >= 1) then {
        Double_t a0, b0, c0;
        start_shower->Get(a0,b0,c0);
        cout << endl << "entrance point of beam : " << a0 << " ; " << b0 << " ; " << c0 << endl;
        axis_shower->Get(a0,b0,c0);
        cout << "axis of beam : " << a0 << " ; " << b0 << " ; " << c0 << endl;
      }
      TElecCascade *beam120GeV;
      beam120GeV = new TElecCascade("Beam120GeV","Beam120GeV",ebeam,start_shower,
                                    axis_shower,"PbWO4","Crystal",kFALSE,photfac);
      break;
    case 4:	//	shower_bis
      start_shower = start_casc;
      axis_shower = axis_casc;
      if (verbosity >= 1) then {
        Double_t a0, b0, c0;
        start_shower->Get(a0,b0,c0);
        cout << endl << "entrance point of beam : " << a0 << " ; " << b0 << " ; " << c0 << endl;
        axis_shower->Get(a0,b0,c0);
        cout << "axis of beam : " << a0 << " ; " << b0 << " ; " << c0 << endl;
      }
      TElecCascade *beam120GeVb;
      beam120GeVb = new TElecCascade("Beam120GeVb","Beam120GeVb",ebeam,start_shower,
                                     axis_shower,"PbWO4","Crystal",kFALSE,photfac);
      break;
  }//end switch
  const Int_t nrun0 = 100; 
  for (idata = ndata0; idata < ndata0 + ndatafile; idata++) {
    irun = idata + nrun0;
    switch (typeirrad) {
      case 1:	//	homogeous irradiation
        if (idata > 0) { 
          Labsfile = Labsfilesprefix + (idata + 1);
          pbwo4->SetAbsLa(Labsfile.Data(), kFALSE);
        }
        cout << endl << Labsfile.Data() << endl;
        break;
      case 2:	//	e-beam irradiation
        if (idata > 0) {
          Damagefile = Damagefilesprefix + idata;
          cout << endl << Damagefile.Data() << endl;
          damok = ps->ShowerDamage(&axis_dam,&start_dam,Damagefile.Data(),ebeamirrad);
        }
        else cout << endl << Labsfile.Data() << endl;
        break;
    }//end switch (typeirrad)
    switch (typelight) {
      case 1:	//	led
        led->Gen(irun,nbphot,dosis[idata]);
        break;
      case 2:	//	laser
        laser->Gen(irun,nbphot,dosis[idata]);
        break;
      case 3:	//	shower
        beam120GeV->Gen(irun,dosis[idata]);
        break;
      case 4:	//	shower bis
        beam120GeVb->Gen(irun,dosis[idata]);
        break;
    }//end switch (typelight)
  }//end for (idata = ndata0; idata < ndata0 + ndatafile; idata++)
//  It is a good idea to make a drawing of the setup. Any geometrical mistake will
//appear as an awful drawing ! Notice however that ROOT has a very strange and
//questionable way of drawing a setup : the proportion of the 3 axis are not
//respected by ROOT ! ROOT feels free to affect different factor to the 3 axis Ox,
//Oy and Oz ! So do not be surprised by the proportions !
        gLit->BookCanvas();
	node1->Draw();
	gTwoPad->ChangePad();
  gGp->SetTitle("Absorption file");
  gGp->Summary(); //Summary() creates all summary histograms
  gGs->DoStat();
  for (idata = 0; idata < ndatafile; idata++) {
    jdata = ndata0 + idata + 1;
    efficiency[idata] = gGp->fHEfficiency->GetBinContent(jdata);
    erroreff[idata] = gGp->fHEfficiency->GetBinError(jdata);
  }
  cout << endl << "Crystal type : " << crystaltype << " ; revetment : " << crystalrevetment << endl;
  switch (typelight) {
    case 1:	//	led
      cout << "LED light injection ; wavelength : " << iwvlgth 
           << " nm ; number of photons per run : " << nbphot << endl;
      calcsize = nbphot;
      break;
    case 2:	//	laser
      cout << "Laser stimulated emission ; number of photons per run : " << nbphot << endl;
      calcsize = nbphot;
      break;
    case 3:	//	shower
      cout << "Electron shower ; energy : " << iebeam << " GeV ; photofraction : " 
           << photfac << endl;
      calcsize = photfac;
      break;
    case 4:	//	shower bis
      cout << "Electron shower 2 ; energy : " << iebeam << " GeV ; photofraction : " 
           << photfac << endl;
      calcsize = photfac;
      break;
  }//end switch (typelight)
  cout << "irradiation : " << DamageName.Data() << endl;
  cout << endl << "file;efficiency;error;n" << endl;
  for (idata = 0; idata < ndatafile; idata++) {
    jdata = ndata0 + idata + 1;
    cout << jdata << ";" << efficiency[idata] << ";" << erroreff[idata] << ";" << calcsize << endl;
  }
  cout << endl;
//  gGp->DrawEfficiency();
  gLit->CloseFiles();
}
