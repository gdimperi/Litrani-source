{
//
//   cms_muons
//
//  A beam of muons is defined which crosses the crystal of PbWO4. The fit
//"DEdx_MusInPbWO4PDB" in SplineFitDB.rdb, coming from the CINT Macro of the
//same name, gives the value of the deposited dE/dx per cm as a function of
//the energy of the muon. Landau fluctuations are not taken into account. The muons have
//been defined has having a fixed momentum of 10 Gev.
//  The beam originates from an ellipse, placed on the left of the crystal, and is displa-
//ced upwards from run to run. The axis of the beam is perpendicular to the ellipse in
//direction to the crystal. The aperture of the beam is 5 degrees. The beam crosses the
//crystal orthogonally to the long axis of the crystal.
//
//  BEGIN OF cms1.C
//
  char ccc;
  Int_t kkk;
  Bool_t perpendicular = kTRUE;
  TString name   = "muons";
  char *upcom    = "Simulation of CMS crystal hit by muon";
  char *downcom1 = "Muons perpendicular to long axis of crystal";
  char *downcom2 = "Muons parallel to long axis of crystal";
  Int_t NPart;
  Bool_t isopbwo4 = kFALSE;
  Double_t diffusionlength = 40.0;
  Double_t depolishangle = 42.0;  //max depolish angle of face 2 of isosheet
  cout << endl;
  cout << "PbWO4 is treated as birefringent. Do you want" << endl;
  cout << "an isotropic PbWO4, in order to speed up the program ?" << endl;
  cout << "Type y if yes : ";
  cin  >>ccc;
  if (ccc == 'y') {
    isopbwo4 = kTRUE;
    name.Append("_I");
    cout << "PbWO4 treated as isotropic" << endl;
  }
  else {
    name.Append("_B");
    cout << "PbWO4 treated as birefringent" << endl;
  }
  cout << endl;
  cout << "Depolish angle for depolished face is : " << depolishangle << endl;
  cout << "Do you want to change it? [y/n] :";
  cin  >> ccc;
  if (ccc=='y') {
    cout << "Enter depolish angle : ";
    cin  >> depolishangle;
  }
  name.Append("_D");
  kkk = (Int_t)depolishangle;
  name += kkk;
  cout << endl;
  cout << "Diffusion length is set to " << diffusionlength << endl;
  cout << "Do you want to change it? [y/n]";
  cin  >> ccc;
  if (ccc=='y') {
    cout << "[Put it negativ if you do not want diffusion]" << endl;
    cout << "Enter diffusion length : ";
    cin  >> difusionlength;
  }
  if (diffusionlength>0.0) {
    name.Append("_diff");
    kkk = (Int_t)diffusionlength;
    name += kkk;
  }
  cout << endl;
  cout << "How many muons do you want to generate : ";
  cin  >> NPart;
  cout << "Do you want the muon beam perpendicular" << endl;
  cout << "  to the long axis of the crystal? (y/n) ";
  cin  >> ccc;
  if (ccc=='y') {
    perpendicular = kTRUE;
    cout << "Muon beam perpendicular to long axis of crystal" << endl;
    gROOT->ProcessLine(".x InitLitrani.C(7,name.Data(),name.Data(),upcom,downcom1,kFALSE,kFALSE,kTRUE)");
  }
  else {
    perpendicular = kFALSE;
    cout << "Muon beam parallel to long axis of crystal" << endl;
    gROOT->ProcessLine(".x InitLitrani.C(1,name.Data(),name.Data(),upcom,downcom2,kFALSE,kFALSE,kTRUE)");
  }
  Int_t    PrintFreq = 50000;
  gLit->SetPrintFreq(PrintFreq);
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
  gROOT->ProcessLine(".x CMS1_setup.C");
//  Defines a particle of type muon, with a fixed energy distribution of 2 Gev. Notice
//that as the mass of the particle is not given, the default : mass of the muon, will
//be taken.
  TSParticle *part = new TSParticle("muon","muon",singlevalued,2.0);
  Double_t x;
  const Double_t y  = zero;
  const Double_t dx = 0.1;
  const Double_t dz = 2;
  Double_t z;
  if (perpendicular) {
    x = -deux;
    z = -10.0;
  }
  else {
    x = -0.5;
    z = -13.0;
  }
  TBeam *pbeam;
  T3Vector center(x,y,z);
//
//  Loop displacing the beam upwards along z
  Bool_t Cerenkov = kTRUE;
  cout << "Do you want Cerenkov light ? (y/n)";
  cin  >> ccc;
  if (ccc=='n') Cerenkov = kFALSE;
  for (Int_t i=0;i<10;i++) {
//
//  Defines a beam of particles part (muons). Because "crystal" is given as the
//4th argument of the constructor, all coordinates used for defining the beam
//are in the local coordinate system of the crystal. The beam is coming from
//a surface which is defined as an ellipse, centered at (x,y,z). The constructor
//invoked here is the second constructor, the one best suited when the 2 axis of
//the emitting surface of the beam (here the 2 axis of the ellipse) are orthogonal.
//0.8 and 0.5 are the lengths of the 2 semi-axis of the ellipse. The axis
//normal to the ellipse ( let us define it as Oz" ) has the 2 angles (theta3=90,
//phi3=0), so that it points parallel to the Ox axis, towards the crystal, crossing
//the crystal orthogonally to the long axis of the crystal. The large axis of the
//ellipse ( let us define it as Ox" ) has the 2 angles (theta1=90, phi1=90) so that
//it points along the Oy axis. The small axis of the ellipse, Oy", need not be
//defined because (Ox", Oy", Oz") have to build an orthonormal system. This implies
//here that Oy" = + Oz. Look at the web documentation of the class TBeam if something
//is unclear to you.
    if (perpendicular) {
      pbeam = new TBeam("mu_beam","mu_beam",part,"crystal",center,ellipse,
                   0.8,0.5,90.0,0.0,90.0,90.0);
    }
    else {
      pbeam = new TBeam("mu_beam","mu_beam",part,"crystal",center,ellipse,
                   0.08,0.05,0.0,0.0,90.0,90.0);
    }
//
//  The instance of the method Beam() called here implies that the direction of the
//axis of emission of the beam is parallel to the axis normal to the emitting surface
//of the beam (the ellipse). If it would not have been the case, we ought to have
//called the other instance of the method Beam(), where direction of emission of the
//beam has to be given. The distribution of emission of the beam is flat, i.e.
//sin(theta)*dtheta*dphi within the aperture of 5 degrees.
    pbeam->Beam(flat,5.0);
    pbeam->SetCerenkov(Cerenkov);
//
// Start run 100, generating 1 particles (feel free to generate more !). Recording of
//the separate statistics and histograms for this run is asked. The histograms of the
//preceeding run are cleaned from memory before starting this new run, but they are saved
//into a file named "mu_beam_nnn.root", where nnn is the run number. As the last parameter,
//step, is not given, the default value of 0.5 cm will be used. z is xparam, the parameter
//which varies from run to run and will be used as x axis in the histograms done by the
//class TPublication.
    if (perpendicular) {
      pbeam->Gen(100+i,NPart,z,kTRUE,kFALSE);
    }
    else {
      pbeam->Gen(100+i,NPart,x,kTRUE,kFALSE);
    }
    delete pbeam;
    if (perpendicular) {
      z += dz;
    }
    else {
      x += dx;
    }
//displaces the beam
    center.Set(x,y,z);
  }
//
//  It is a good idea to make a drawing of the setup. Any geometrical mistake will
//appear as an awful drawing ! Notice however that ROOT has a very strange and
//questionable way of drawing a setup : the proportion of the 3 axis are not
//respected by ROOT ! ROOT feels free to affect different factor to the 3 axis Ox,
//Oy and Oz ! So do not be surprised by the proportions !
  gLit->BookCanvas();
  node1->Draw();
  gTwoPad->ChangePad();
//  Here the generation of photons is finished. It remains to analyze the results.
//For that, you have at hand the 2 classes TResults and TPublication, and all the
//power of ROOT ! Look at the web description of the classes TResults and
//TPublication where you will find a full description of all types of histograms
//available at the end of a production of Litrani. You will learn there that
//for instance gGp is a pointer towards the summary histograms of Litrani. Giving
//to the title of the class TPublication pointed to by gGp "z coordinate of beam"
//will have for result that all the summary histograms will contain this in their
//title. For instance, the histogram on efficiency that we plot here will have as
//title : "Efficiency versus z coordinate of beam".
  if (perpendicular) {
    gGp->SetTitle("z coordinate of beam");
  }
  else {
    gGp->SetTitle("x coordinate of beam");
  }
//Summary() creates all summary histograms
  gGp->Summary();
//Draws one of the available histogram.
  gStyle->SetOptFit(1111);
  gGp->fHEfficiency->Fit("pol1","V");
  gGs->DoStat();
  gLit->CloseFiles();
}
