{
  char *name    = "Transm2";
  char *listing = "Transm2";
  char *upcom   = "Transmission study inside a TSBRIK";
  char *downcom = "Beam issued from an ellipse, Detector is a phototube";
  gROOT->ProcessLine(".x InitLitrani.C(6,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
//
// arguments of InitLitrani:
//
// size     size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
// name     name of the produced ROOT file (without ".root")
// listing  name of the listing file (without ".lis")
// upcom    name of the comment appearing on top    left of canvas
// downcom  name of the comment appearing on bottom left of canvas
// otherseq true if one wants an other sequence of random numbers at each new launching
// indebug  true if one wants to run Litrani2 in debug mode
// WithDate if true, appends date and time to name of root file and name of listing
//
//  Transm2.C is more realistic than Transm1.C :
//
//    (1) - The beam is issued from an ellipse, not from a point.
//    (2) - The aperture of the Beam is 2 degree
//    (3) - The detector is a true phototube, with a real quantum efficiency.
//
//  One point remains over-simplified :
//
//  (4) - Material PbWO4 is treated as isotropic and not negative birefringent.
//
//  Look at Transm3.C to have PbWO4 treated as negative birefringent.
//  The setup is composed of a crystal of PbWO4, of shape TSBRIK, on top of an other TSBRIK
//made of air and containing a source of light. On top of the crystal is an other TSBRIK
//made of air having on top of it a phototube. This setup is intended to simulate the kind
//of apparatus used by opticians to measure transmission.
//
//  Following fits are used in this macro:
//    - "AbsorptionLength_PbWO4"
//    - "RefrIndex_Sodocal"
//    - "QuantumEff_XP2020"
//  These fits have been declared nowhere! So they will be searched and found in the
//"database" SplineFitDB.rdb
//  To know how to define your own fits, please read the documentation about SplineFit at:
//
//  ---> http://gentit.home.cern.ch/gentit/splinefit/
//
//
//  BEGIN of code
//
//  Material "Air" is declared, with magnetic permeability fixed at 1.0, absorption length
//fixed at 10000 cm, index of refraction fixed at 1.0. Nothing depends upon wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0);
  air->IsIsotropic(1.0);
//
//  Definition of the material PbWO4. The magnetic permeability is declared
//to be 1, not depending upon wavelength. Only 1 fit of absorption length versus wavelength
//is called, meaning that absorption length is isotropic.
//  IsIsotropic() is called with 4 arguments, meaning that the material is treated as
//isotropic, the variation of the index of refraction with wavelength being described by
//the Sellmeier law with 3 parameters : ns0, ns1, ls1.
  TOpticMaterial *pbwo4;
  char cc = 'n';
  cout << " The following question is just to show you how to put" << endl;
  cout << "a TSplineFit in the collection TSplineFit::fgFits, avoiding" << endl;
  cout << "the use of the database file SplineFitDB.rdb for this fit" << endl;
  cout << "Look at the code of Transm2.C after this question!" << endl;
  cout << " Do you want that the fit AbsorptionLength_PbWO4 be found" << endl;
  cout << "inside the collection TSplineFit::fgFits, and not inside the" << endl;
  cout << "database file SplineFitDB.rdb? If yes type y and <CR>, else" << endl;
  cout << "type 'n' and <CR>:";
  cin >> cc;
  if (cc == 'y') {
    // The following code is just to show you how to put a TSplineFit in the collection
    //TSplineFit::fgFits, avoiding the use of the database file SplineFitDB.rdb for this fit
    //You can do the same for all fits that you have not yet introduced inside
    //the database file SplineFitDB.rdb
    // This line executes the CINT macro AbsorptionLength_PbWO4.C, which
    //has the wanted effect
    gROOT->ProcessLine(".x AbsorptionLength_PbWO4.C();");
  }
  pbwo4 = new TOpticMaterial("PbWO4","Material PbWO4",kFALSE,1.0,"AbsorptionLength_PbWO4");
  const Double_t ns0 = 1.5861;   //index 0 for the Sellmeier law for PbWO4
  const Double_t ns1 = 1.1062;   //index 1 for the Sellmeier law for PbWO4
  const Double_t ls1 = 270.63;   //1st resonant wavelength for the Sellmeier law for PbWO4
  pbwo4->IsIsotropic(1,ns0,ns1,ls1);
//
//  Definition of the material sodocal, the window of the Phototube. It has an absorption
//length of 1000 cm (we have not been able to find the true values for the absorption
//length of sodocal), a magnetic permeability of 1.0. It is optically isotropic, with an
//index of refraction varying as a function of wavelength by the fit "RefrIndex_Sodocal"
  TOpticMaterial *sodocal;
  sodocal = new TOpticMaterial("Sodocal","Sodocal",kFALSE,1.0,1000.0);
  sodocal->IsIsotropic("RefrIndex_Sodocal");
//
//  Definition of the revetment "TotAbsorbing", giving only the 1st 2 arguments. When the
//other arguments 3 to 8 are not given, following default values are assumed :
//
// (3) - nameMat  = "none" : no slice material between face and revetment.
// (4) - diffus   = 0.0    : Reflecting, not diffusing. Irrelevant because of (8).
// (5) - Rindex   = 2.0    : Real part of index. Irrelevant because of (8).
// (6) - Iindex   = 6.0    : Imaginary part of Index. Irrelevant because of (8).
// (7) - Mu       = 1.0    : Magnetic permeability. Irrelevant because of (8).
// (8) - supplAbs = 1.0    : supplementary absorption of 100%.
//
// When a photon reaches such a revetment, it is absorbed in any case, it has no chance
//neither of making a total reflection ( there is no slice of material between shape and
//revetment ) nor of being reflected by the revetment (because the revetment has a
//supplementary absorption of 100%).
  TRevetment *totabsorbing;
  totabsorbing = new TRevetment("TotAbsorbing","Totally absorbing revetment");
//
//  Geometry
//
  const Double_t zero     = 0.0;
  const Double_t crys_dx  = 1.0; //half length in x of crystal of PbWO4
  const Double_t crys_dy  = 1.0; //half length in y of crystal of PbWO4
  const Double_t crys_dz  = 6.0; //half length in z of crystal of PbWO4
  const Double_t laser_dz = 1.0; //half length in z of TSBRIK with light source
  const Double_t gap_dx   = 3.0; //half-length in x of air gap of type TSBRIK
  const Double_t gap_dy   = 3.0; //half-length in y of air gap of type TSBRIK
  const Double_t gap_dz   = 6.0; //half length in z of air gap of type TSBRIK
  const Double_t tube_r   = 2.2; //radius of the phototube
  const Double_t tube_dz  = 0.2; //half-length of the cylinder building the window of PM
  Double_t laser_z;              //translation in z of TSBRIK with light source
  Double_t gap_z;                //translation in z of air gap
  Double_t tube_z;               //translation in z of phototube
  laser_z = - crys_dz - laser_dz;
  gap_z   =   crys_dz + gap_dz;
  tube_z  =   crys_dz + 2*gap_dz + tube_dz;
//
//  The crystal is assumed to have the shape of a rectangular parallelipiped ( a TSBRIK )
//with semi-axis crys_dx, crys_dy, crys_dz. The long axis of the crystal is along Oz. The
//shape "crystal" is taken as the reference shape ( because of the code node1->cd(); ).
//So that the local coordinate system of the shape "crystal" is at the same time the world
//coordinate system WCS. The shape "crystal" is made of PbWO4 and is not covered by any
//revetment.
  TSBRIK *crystal;
  crystal = new TSBRIK("crystal","crystal","PbWO4","none",crys_dx,crys_dy,crys_dz);
//
//  The shape containing the source of light is assumed to be a TSBRIK. It is made of Air,
//and covered with a totally absorbing revetment, except for the face in contact with the
//crystal.
  TSBRIK *laserbrick;
  laserbrick = new TSBRIK("laserbrick","laserbrick","Air","TotAbsorbing",
                           crys_dx,crys_dy,laser_dz);
//
//  The air gap is assumed to be a TSBRIK, broader than the crystal. It is made of Air,
//and covered with a totally absorbing revetment, except for the face in contact with the
//crystal and the part of the face in contact with the window of the phototube.
  TSBRIK *gapbrick;
  gapbrick = new TSBRIK("gapbrick","Brik with the gaptor","Air","TotAbsorbing",
                           gap_dx,gap_dy,gap_dz);
//  Definition of the TSCYL shape representing the window of the phototube. It is made
//of sodocal, covered with a totally absorbing revetment.
  TSCYL *pm;
  pm = new TSCYL("Phototube","Phototube","Sodocal","TotAbsorbing",tube_r,tube_dz);
//Declares that the shape pm is a phototube, that the upper face (1) is the photocathode,
  pm->fSuppl->SetPhotoTube("XP2020","Phototube Phillips XP2020",1,"QuantumEff_XP2020");
//
//use different colors for the 3 shapes
  laserbrick->SetLineColor(4);
  laserbrick->SetLineWidth(2);
  gapbrick->SetLineColor(2);
  gapbrick->SetLineWidth(2);
  pm->SetLineColor(28);
  pm->SetLineWidth(2);
//
//  TSNode gives the position of a shape with respect to the node currently
//selected, by mean of a translation vector and a rotation matrix. No node
//being actually selected, node1 will be the main node and will define the
//WCS (world coordinate system). As no translation vector is given, the
//crystal will not be translated (translation vector 0 assumed). As no
//rotation matrix is given, the crystal will not be rotated and will remain
//with its axis parallel to Oz. Node1, the crystal, is selected and will remain the
//reference for all other nodes.
  TSNode *node1;
  node1 = new TSNode("node1","node1",crystal);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);  
  node1->cd();
//
//  Node2 will give the position of the laserbrick shape with respect to the node
//currently selected, the crystal node. A translation vector (0,0,laser_z) will
//put the shape exactly under the crystal. No rotation matrix being given, the laserbrick
//shape will stay with its axis parallel to the Oz axis.
  TSNode *node2;
  node2 = new TSNode("node2","node2",laserbrick,zero,zero,laser_z);
//
//  Node3 will give the position of the gapbrick shape with respect to the node
//currently selected, the crystal node. A translation vector (0,0,gap_z) will
//put the gapbrick shape exactly above the crystal. No rotation matrix being given, the
//shape will stay with its axis parallel to the Oz axis.
  TSNode *node3;
  node3 = new TSNode("node3","node3",gapbrick,zero,zero,gap_z);
//
//  Node4 will give the position of the pm shape with respect to the node
//currently selected, the crystal node. A translation vector (0,0,tube_z) will
//put the phototube shape exactly above the air gap. No rotation matrix being given, the
//shape will stay with its axis parallel to the Oz axis.
  TSNode *node4;
  node4 = new TSNode("node4","node4",pm,zero,zero,tube_z);
//
//  There is a contact between crystal shape and laserbrick shape. Notice this nice
//property that you do not have to tell which face of crystal is in contact with which
//face of laserbrick: Litrani will find it for you. In fact, it is face 4 of crystal which
//is in contact with face 5 of laserbrick. Look at the web documentation of TSBRIK to
//understand how faces are numbered for a TSBRIK. The face of crystal in contact with the
//face of laserbrick are identical hence "identical".
  TContact *contact12;
  contact12 = new TContact("crystal-laser","crystal-laser","crystal","laserbrick",
                            identical);
//
//  There is a contact between crystal shape and air gap shape. It is face 5 of crystal
//which is in contact with face 4 of gapbrick. Face 5 of crystal is smaller than face 4 of
//gapbrick and is entirely contained into it. Hence "contained".
  TContact *contact13;
  contact13 = new TContact("crystal-gap","crystal-gap","crystal","gapbrick",contained);
//
//  There is a contact between air gap shape and pm shape. It is face 5 of air gap which
//is in contact with face 0 of pm. Face 5 of air gap is bigger than face 0 of pm  and
//contains it entirely. Hence "containing".
  TContact *contact34;
  contact34 = new TContact("gap-pm","gap-pm","gapbrick","Phototube",containing);
//
  const Double_t WvlgthStart = 300.0;  //starting value for wavelength
  const Double_t WvlgthStep  = 25.0;   //step in wavelength
  Double_t wavelength = WvlgthStart;
//
//  Declaration of a spontaneous source of light inside the shape "laserbrick", at point
//(0,0,0) in the local coordinate system of the laserbrick, i.e. exactly in the middle
//of the laserbrick. This spontaneous source will emit light at the wavelength "wavelength".
  TSpontan *laser;
  laser = new TSpontan("laser","laser","laserbrick",zero,zero,zero,wavelength);
//
//  DefineCradle() is called here with the following arguments
//
//    (1) - "laserbrick"  : name of shape in which the cradle of the photons is
//    (2) - ellipse       : kind of cradle : {point, disk, ellipse, parallelogram,
//                                            parallelipiped}
//    (3) - a1     = 0.1  : large semi-axis of ellipse
//    (4) - a2     = 0.05 : small semi-axis of elipse
//    (5) - a3     = 0.0  : Irrelevant for an ellipse
//    (6) - theta3 = 0.0  : angle theta (degree, in LCS) of the normal to the ellipse
//    (7) - phi3   = 0.0  : angle phi   (degree, in LCS) of the normal to the ellipse
//    (8) - theta1 = 90.0 : angle theta (degree, in LCS) of the large axis of the ellipse
//    (9) - phi1   = 0.0  : angle phi   (degree, in LCS) of the large axis of the ellipse
//
  laser->DefineCradle("laserbrick",ellipse,0.1,0.05,0.0,0.0,0.0,90.0,0.0);
//
//We want a flat distribution with aperture 2 degrees. The beam is not polarized.
  laser->DefineBeam(flat,2.0);
  for (Int_t i=0;i<21;i++) {
//
//  Generation of 10000 photons of wavelength "wavelength". A run number must be greater
//than 0, hence 100+i and not i !
    laser->Gen(100+i,10000,wavelength);
    wavelength += WvlgthStep;
//Changes the wavelength
    laser->SetWvlgth(wavelength);
  }
//  It is a good idea to make a drawing of the setup. Any geometrical mistake will
//appear as an awful drawing ! Notice however that ROOT has a very strange and
//questionable way of drawing a setup : the proportion of the 3 axis are not
//respected by ROOT ! ROOT feels free to affect different factor to the 3 axis Ox,
//Oy and Oz ! So do not be surprised by the proportions !
  gLit->BookCanvas();
  node1->Draw();
  gTwoPad->ChangePad();
//  Here all runs of generation of photons are finished. It remains to analyze the results.
//For that, you have at hand the 2 classes TResults and TPublication, and all the
//power of ROOT ! Look at the web description of the classes Tresults and
//TPublication where you will find a full description of all types of histograms
//available at the end of a production of Litrani. You will learn there that
//for instance gGp is a pointer towards the summary histograms of Litrani. Naming
//the title of the class TPublication (pointed to by gGp) "wavelength of laser"
//will have for result that all the summary histograms will contain this in their
//title. For instance, the histogram on efficiency that we plot here will have as
//title : "Efficiency versus wavelength of laser ".
  gGp->SetTitle("wavelength of laser");
//Summary() creates all summary histograms
  gGp->Summary();
  gGs->DoStat();
  gLit->CloseFiles();
}
