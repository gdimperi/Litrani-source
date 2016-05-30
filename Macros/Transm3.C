{
  char *name    = "Transm3";
  char *listing = "Transm3";
  char *upcom   = "Transmission study inside a CMS crystal with negativ birefringent material";
  char *downcom = "Beam issued from an ellipse, Detector is a phototube";
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
//  Transm3.C is still more realistic than Transm2.C :
//
//    (1) - Material PbWO4 is treated as negativ birefringent, with also an anisotropic
//           absorption length.
//    (2) - The shape of the crystal is the one of a CMS crystal.
//    (3) - "The optical axis" is parallel to Ox (move it if you will).
//    (4) - The beam of photons is polarized parallel to Ox or parallel to Oy or parallel
//           to Oz, at will.
//
  gLit->SetPrintFreq(50000);
//
//  The setup is composed of a crystal of PbWO4, of shape TS8PTS, on top of a TSBRIK made
//of air and containing a source of light. The source of light is elliptical, the axis of
//the beam is parallel to the normal to the ellipse. The aperture of the beam is 2 degrees.
//On top of the crystal is an other TSBRIK made of air having on top of it a phototube, of
//shape TSCYL.
//  This setup is intended to simulate the kind of apparatus used by opticians to measure
//transmission.
//
//  Following fits are used in this macro:
//    - "AbsLengthTensor_PbWO4__000"
//    - "AbsLengthTensor_PbWO4__001"
//    - "DielTensor_PbWO4__000"
//    - "DielTensor_PbWO4__002"
//    - "RefrIndex_Sodocal"
//    - "QuantumEff_XP2020"
//  These fits have been declared nowhere! So they will be searched and found in the
//"database" SplineFitDB.rdb
//
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
//  Definition of the material PbWO4. The magnetic permeability is declared to be 1, not
//depending upon wavelength. 2 references to fit of absorption lengths depending upon
//wavelengths are made, "AbsLengthTensor_PbWO4__000" and "AbsLengthTensor_PbWO4__001",
//which means that the absorption length is not isotropic.
//  PbWO4 is declared as negativ birefringent. We do not use the Sellmeier law to describe
//the variation of the parameters epsa and epsc with wavelength, but we use the fits
//"DielTensor_PbWO4__000","DielTensor_PbWO4__002".
//  PbWO4 being not the source of light in this example, it is not declared as fluorescent
//(no creation of TLitSpectrum) and the deposit of energy when crossed by particles is not
//defined (no call to dEdx()).
//  We preload the 2 families of fit used. This is not necessary, but more elegant.
//The fact that the absorption length tensor and the dielectric tensor are given
//here as family of fits is not important. The specific operations linked with
//family of fits are not used for these 2 families. The fact that they are grouped
//into families is also a matter of taste.
  Int_t NFamAbs,NFamDiel;
  TSplineFit *pa000, *pd000;
  pa000  = TSplineFit::FindFit("AbsLengthTensor_PbWO4__000",6);
  cout << "pa000 : " << pa000 << endl;
  NFamAbs   = pa000->LoadFamily();
  cout << "Nb. of elements in family AbsLengthTensor__PbWO4 : " << NFamAbs << endl;
  pd000 = TSplineFit::FindFit("DielTensor_PbWO4__000",2);
  NFamDiel  = pd000->LoadFamily();
  cout << "Nb. of elements in family DielTensor_PbWO4      : " << NFamDiel << endl;
//  End of preload of families
  TOpticMaterial *pbwo4;
  pbwo4 = new TOpticMaterial("PbWO4","PbWO4 crystal",kFALSE,1.0,
                             "AbsLengthTensor_PbWO4__000","AbsLengthTensor_PbWO4__001");
  pbwo4->IsUniAxialNegBirefr("DielTensor_PbWO4__000","DielTensor_PbWO4__002");
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
  const Double_t zero = 0.0;
  const Double_t deux = 2.0;
//
//  Description of the CMS crystal, type 6, by the 7 parameters AF, BF, CF, AR, BR, CR and
//dz. Look at "The Electromagnetic Calorimeter Project, CMS TDR4" pages 53/54.
  const Double_t crys_dz  = 11.5;  //half length in z of crystal of PbWO4
  const Double_t crys_AF  = 2.183; //side left,  small face |_ to OZ
  const Double_t crys_BF  = 2.260; //side back,  small face |_ to OZ
  const Double_t crys_CF  = 2.200; //side right, small face |_ to OZ
  const Double_t crys_AR  = 2.543; //side left,  large face |_ to OZ
  const Double_t crys_BR  = 2.618; //side back,  large face |_ to OZ
  const Double_t crys_CR  = 2.563; //side right, large face |_ to OZ
//
//  TSBRIK containing light source
  const Double_t laser_dx = 1.0; //half length in x of TSBRIK with light source
  const Double_t laser_dy = 1.0; //half length in y of TSBRIK with light source
  const Double_t laser_dz = 1.0; //half length in z of TSBRIK with light source
//
//  TSBRIK describing gap between crystal and PM
  const Double_t gap_dx   = 2.5; //half-length in x of air gap of type TSBRIK
  const Double_t gap_dy   = 2.5; //half-length in y of air gap of type TSBRIK
  const Double_t gap_dz   = 6.0; //half length in z of air gap of type TSBRIK
//
//  TSCYL describing the window of the phototube.
  const Double_t tube_r   = 2.2; //radius of the phototube
  const Double_t tube_dz  = 0.2; //half height of the cylinder building the window of PM
//
//  Coordinate of Points of crystal
  Double_t crys_dx,crys_dy;
  Double_t x0,x1,x2,x3,x4,x5,x6,x7;
  Double_t y0,y1,y2,y3,y4,y5,y6,y7;
  crys_dx = crys_BF/deux;
  crys_dy = crys_AF/deux;
//small face |_ to OZ
  x0 = - crys_dx;
  y0 = - crys_dy;
  x1 = x0 + crys_BF;
  y1 = y0;
  x2 = x1;
  y2 = y1 + crys_CF;
  x3 = x0;
  y3 = y0 + crys_AF;
//large face |_ to OZ  
  x5 = x1;
  y5 = y1;
  x4 = x5 - crys_BR;
  y4 = y5;
  x6 = x5;
  y6 = y5 + crys_CR;
  x7 = x4;
  y7 = y4 + crys_AR;
//Printing
  cout << endl;
  cout << "Point 0 : ";  cout.width(8);  cout << x0 << "  ";  cout.width(8);  cout << y0 << "  ";  cout.width(8);  cout << -crys_dz << endl;
  cout << "Point 1 : ";  cout.width(8);  cout << x1 << "  ";  cout.width(8);  cout << y1 << "  ";  cout.width(8);  cout << -crys_dz << endl;
  cout << "Point 2 : ";  cout.width(8);  cout << x2 << "  ";  cout.width(8);  cout << y2 << "  ";  cout.width(8);  cout << -crys_dz << endl;
  cout << "Point 3 : ";  cout.width(8);  cout << x3 << "  ";  cout.width(8);  cout << y3 << "  ";  cout.width(8);  cout << -crys_dz << endl;
  cout << "Point 4 : ";  cout.width(8);  cout << x4 << "  ";  cout.width(8);  cout << y4 << "  ";  cout.width(8);  cout <<  crys_dz << endl;
  cout << "Point 5 : ";  cout.width(8);  cout << x5 << "  ";  cout.width(8);  cout << y5 << "  ";  cout.width(8);  cout <<  crys_dz << endl;
  cout << "Point 6 : ";  cout.width(8);  cout << x6 << "  ";  cout.width(8);  cout << y6 << "  ";  cout.width(8);  cout <<  crys_dz << endl;
  cout << "Point 7 : ";  cout.width(8);  cout << x7 << "  ";  cout.width(8);  cout << y7 << "  ";  cout.width(8);  cout <<  crys_dz << endl;
  cout << endl;
  Double_t laser_z;              //translation in z of TSBRIK with light source
  Double_t gap_z;                //translation in z of air gap
  Double_t tube_z;               //translation in z of phototube
  laser_z = - crys_dz - laser_dz;
  gap_z   =   crys_dz + gap_dz;
  tube_z  =   crys_dz + deux*gap_dz + tube_dz;
//
//  The crystal is assumed to have the shape of a CMS crystal of type 6, which may be built
//with the Litrani shape TS8PTS. The long axis of the crystal is along Oz. The shape
//"crystal" is taken as the reference shape ( because of the code node1->cd(); ). So that
//the local coordinate system of the shape "crystal" is at the same time the world
//coordinate system WCS. The shape "crystal" is made of PbWO4 and is not covered by any
//revetment.  Notice that if you give to the points coordinates such that the lateral faces
//are not plane, your coordinates will be changed, as little as possible, such that lateral
//faces become plane. The new values of the coordinates will be printed.
  TS8PTS *crystal;
  crystal = new TS8PTS("crystal","crystal","PbWO4","none",crys_dz,
                x0,y0,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7);
//
//  SetOpticalAxis sets the 3 optical axis Ox', Oy' and Oz'. The 3 optical axis
//are the 3 axis of the coordinate system in which the dielectric tensor is diagonal,
//with eigenvalues :
//
//   ea   associated with Ox' axis
//   eb   associated with Oy' axis
//   ec   associated with Oz' axis
//
//   ea <  eb <  ec  for birefringent (== totally anisotropic) crystals
//   ea <  eb =  ec  for "uniaxial negativ birefringent" crystals (our case)
//   ea =  eb <  ec  for "uniaxial positiv birefringent" crystals
//
//  These 3 axis are given "a la GEANT" : give theta and phi of Ox' axis (thetax,phix)
//theta and phi of Oy' axis (thetay,phiy). It is not necessary to give theta and phi of
//Oz' : Oz' can be determined with Ox' and Oy'. This way of giving a coordinate system
//inside the local coordinate system of a shape is intuitive, but inelegant, because it is
//redundant. If you give an Oy' axis which is not orthogonal to the Ox' axis, you will be
//warned and your Oy' axis will be corrected. All angles are to be given in degree.
//  Notice that what the opticians call "the optical axis" is the axis which is associated
//with the "isolated" value of the triplet (ea,eb,ec). It has no sense in the case of
//total anisotropy, but it is ea which is "isolated" in case of negative birefringence, and
//it is ec in case of positive birefringence. In our case, it is ea. So "the optical axis"
//is the Ox' axis !
//  If all this is unclear for you, please refer to the web documentation of the classes
//TOpticMaterial and TSupplShape, to understand the construction of the dielectric tensor
//both in Local Coordinate System and in the World Coordinate system.
//  Later on we will define the beam of photons to be polarized along Ox. See the line
//laser->DefineBeam(). We give you here the choice between 3 possibilities for the setting
//of the optical axis. Uncomment one of these 3 lines : 
//(Optical axis || to polarization of photon) and (optical axis |_ to beam axis) 
  crystal->SetOpticalAxis(90,0,90,90);
//(Optical axis |_ to polarization of photon) and (optical axis |_ to beam axis)
//  crystal->SetOpticalAxis(90,90,90,180); 
//(Optical axis |_ to polarization of photon) and (optical axis || to beam axis)
//  crystal->SetOpticalAxis(0,0,90,0); 
//
//  The shape containing the source of light is assumed to be a TSBRIK. It is made of Air,
//and covered with a totally absorbing revetment, except for the face in contact with the
//crystal.
  TSBRIK *laserbrick;
  laserbrick = new TSBRIK("laserbrick","laserbrick","Air","TotAbsorbing",
                           laser_dx,laser_dy,laser_dz);
//
//  The air gap is assumed to be a TSBRIK, broader than the crystal. It is made of Air,
//and covered with a totally absorbing revetment, except for the face in contact with the
//crystal and the part of the face in contact with the window of the phototube.
  TSBRIK *gapbrick;
  gapbrick = new TSBRIK("gapbrick","gapbrick","Air","TotAbsorbing", gap_dx,gap_dy,gap_dz);
//  Definition of the TSCYL shape representing the window of the phototube. It is made
//of sodocal, covered with a totally absorbing revetment.
  TSCYL *pm;
  pm = new TSCYL("Phototube","Phototube","Sodocal","TotAbsorbing",tube_r,tube_dz);
//Declares that the shape pm is a phototube, that the upper face (1) is the photocathode,
  pm->fSuppl->SetPhotoTube("XP2020","Phototube Phillips XP2020",1,"QuantumEff_XP2020");
//
//use different colors
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
//face of laserbrick: Litrani will find it for you. The face of crystal is bigger then the
//face of the laserbrick hence "containing".
  TContact *contact12;
  contact12 = new TContact("cr_laser","cr_laser","crystal","laserbrick", containing);
//
//  There is a contact between crystal shape and air gap shape. It is face 5 of crystal
//which is in contact with face 4 of gapbrick. Face 5 of crystal is smaller than face 4 of
//gapbrick and is entirely contained into it. Hence "contained".
  TContact *contact13;
  contact13 = new TContact("cr_gap","cr_gap","crystal","gapbrick",contained);
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
//of the laserbrick. This spontaneous source emit light at the wavelength "wavelength".
  TSpontan *laser;
  laser = new TSpontan("laser","laser","laserbrick",zero,zero,zero,wavelength);
//
//  DefineCradle() is called here with the following arguments
//
//    (1) - laserbrick    : name of shape in which the cradle of the photons is
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
//We want a flat distribution with aperture 2 degrees. The beam is polarized, with its axis
//of polarization parallel to the Ox axis. It is not exactly true that the beam is pola-
//rized along the Ox axis, because the beam has an aperture of 2 degrees. In fact, the
//E vector of the photon is almost parallel to the Ox axis, but compatible with the requi-
//rement that k and E vector are orthogonal.
//  Notice that you will be only partially obeyed when you ask that the polarization of the
//photon ( the direction of its electric field E ) has a given direction ! The E field HAS
//to be orthogonal to the k vector of the photon in isotropic material. The direction you
//have required for the E field will be changed, as few as possible, so that k and E are
//orthogonal.
  laser->DefineBeam(flat,2.0,"",kTRUE,90.0,0.0);
  for (Int_t i=0;i<21;i++) {
//
//  Generation of 100000 photons of wavelength "wavelength". A run number must be greater
//than 0, hence 100+i and not i !
    laser->Gen(100+i,100000,wavelength);
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
