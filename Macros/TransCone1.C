{
  char *name    = "TransCone1";
  char *listing = "TransCone1";
  char *upcom   = "Transmission study inside a conical shape";
  char *downcom = "Using TSCYL and TSCONE";
  gROOT->ProcessLine(".x InitLitrani.C(5,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
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
//  This example is to illustrate the case of a transmission measurement inside TSCYL and
//TSCONE.
//  The setup is composed of a crystal of PbWO4, of shape TSCONE, on top of a TSCYL
//made of air and containing a source of light, and below an other TSCYL also made of air
//and whose upper face is a perfect detector.
//
//  BEGIN of code
//
  gLit->SetPrintFreq(50000);
//  Material "Air" is declared, with magnetic permeability fixed at 1.0, absorption length
//fixed at 10000 cm, index of refraction fixed at 1.0. Nothing depends upon wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0);
  air->IsIsotropic(1.0);
//

//------------------------------------------------------------------------------------------
// Definition of the revetment aluminium
 TRevetment *aluminium;
 aluminium = new TRevetment("Aluminium","Aluminium","Air",0.0,
                            "RIndexRev_Aluminium","IIndexRev_Aluminium",1.0);

//------------------------------------------------------------------------------------------
//  External environment is explicitly defined to be Air:
delete gOutside;
gOutside = new TOpticMaterial("Outside","Air taken as outside material",kFALSE,1.0,10000.0); 
gOutside->IsIsotropic(1.0);     

//------------------------------------------------------------------------------------------
//  Definition of the material PbWO4. The magnetic permeability is declared
//to be 1, not depending upon wavelength. Only 1 fit of absorption length versus wavelength
//is called, meaning that absorption length is isotropic.
//  IsIsotropic() is called with 4 arguments, meaning that the material is treated as
//isotropic, the variation of the index of refraction with wavelength being described by
//the Sellmeier law with 3 parameters : ns0, ns1, ls1.
  TOpticMaterial *pbwo4;
  pbwo4 = new TOpticMaterial("PbWO4","Material PbWO4",kFALSE,1.0,"AbsorptionLength_PbWO4");
  const Double_t ns0 = 1.5861;   //index 0 for the Sellmeier law for PbWO4
  const Double_t ns1 = 1.1062;   //index 1 for the Sellmeier law for PbWO4
  const Double_t ls1 = 270.63;   //1st resonant wavelength for the Sellmeier law for PbWO4
  pbwo4->IsIsotropic(1,ns0,ns1,ls1);
//
//------------------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------------------
//  Geometry
//
  const Double_t zero     = 0.0;
  const Double_t crys_r1  = 4.0;   //half length in x of crystal of PbWO4
  const Double_t crys_r2  = 1.0;   //half length in y of crystal of PbWO4
  const Double_t crys_dz  = 1.0;   //half length in z of crystal of PbWO4
  const Double_t laser_dz = 1.0;   //half length in z of TSCYL with light source
  const Double_t detec_dz = 1.0;   //half length in z of TSCYL with surface detector
  Double_t laser_z;                //translation in z of TSCYL with light source
  Double_t detec_z;                //translation in z of TSCYL with surface detector
  laser_z = - crys_dz - laser_dz;
  detec_z =   crys_dz + detec_dz;
//
//------------------------------------------------------------------------------------------
//  The shape containing the source of light is assumed to be a TSCYL. It is made of Air,
//and covered with a totally absorbing revetment, except for the face in contact with the
//crystal.
  TSCYL *laserbrick;
  laserbrick = new TSCYL("laserbrick","laserbrick","Air","none",crys_r1,laser_dz);
  laserbrick->SetLineColor(2);
  laserbrick->SetLineWidth(2);
//
//------------------------------------------------------------------------------------------
//  The crystal is assumed to have the shape of TSCONE.
  TSCONE *crystal;
  crystal    = new TSCONE("crystal","crystal","PbWO4","none",crys_r1,crys_r2,crys_dz);
//  TSTUBECONIC *crystal;
//  crystal    = new TSTUBECONIC("crystal","crystal","PbWO4","none",crys_dz,0.,crys_r1,0.,crys_r2);
  crystal->SetLineColor(1);
  crystal->SetLineWidth(2);
//
//------------------------------------------------------------------------------------------
//  The shape containing the surface detector is assumed to be a TSCYL. It is made of Air,
//and covered with a totally absorbing revetment, except for the face in contact with the
//crystal.
  TSCYL *detecbrick;
//  detecbrick = new TSCYL("detecbrick","Brick with the detector","Air","none",crys_r1,detec_dz);
  detecbrick = new TSCYL("detecbrick","Brick with the detector","Air","none",crys_r2,detec_dz);
  detecbrick->SetLineColor(4);
  detecbrick->SetLineWidth(2);
//declares that the upper face (5) of detecbrick is a perfect detector, with quantum effi-
//ciency of 1 and acceptance angle of 90 degrees. Look at the web documentation about the
//TSCYL and TSupplShape to understand the numbering of the faces of a TSCYL, and the
//appearance of the pointer fSuppl here.
  detecbrick->fSuppl->SetSurfDet("perfect","perfect detector",1);
//
//
//------------------------------------------------------------------------------------------
//TSNode gives the position of a shape with respect to the node currently
//selected, by mean of a translation vector and a rotation matrix. No node
//being actually selected, node1 will be the main node and will define the
//WCS (world coordinate system). As no translation vector is given, the
//crystal will not be translated (translation vector 0 assumed). As no
//rotation matrix is given, the laser brik will not be rotated and will remain
//with its axis parallel to Oz. Node1, the crystal, is selected and will remain the
//reference for all other nodes.
  TSNode *node1;
  node1 = new TSNode("node1","node1",laserbrick);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);
  node1->cd();
//------------------------------------------------------------------------------------------
//node2 will give the position of the laserbrick shape with respect to the node
//currently selected, the crystal node. A translation vector (0,0,laser_z) will
//put the shape exactly under the crystal. No rotation matrix being given, the laserbrick
//shape will stay with its axis parallel to the Oz axis.
  TSNode *node2;
  node2 = new TSNode("node2","node2",crystal,zero,zero,laser_dz+crys_dz);
//
//------------------------------------------------------------------------------------------
//node3 will give the position of the detecbrick shape with respect to the node
//currently selected, the crystal node. A translation vector (0,0,detec_z) will
//put the detecbrick shape exactly above the crystal. No rotation matrix being given, the
//shape will stay with its axis parallel to the Oz axis.
  TSNode *node3;
  node3 = new TSNode("node3","node3",detecbrick,zero,zero,laser_dz+2.*crys_dz+detec_dz);
//
//------------------------------------------------------------------------------------------
//  There is a contact between crystal shape and laserbrick shape. Notice this nice
//property that you do not have to tell which face of crystal is in contact with which
//face of laserbrick: Litrani will find it for you. In fact, it is face 4 of crystal which
//is in contact with face 5 of laserbrick. Look at the web documentation of TSCYL to
//understand how faces are numbered for a TSCYL. The face of crystal in contact with the
//face of laserbrick are identical hence "identical".
  TContact *contact12;
  contact12 = new TContact("crystal-laser","crystal-laser","crystal","laserbrick",
                            identical);
//
///------------------------------------------------------------------------------------------
  //  There is a contact between crystal shape and detecbrick shape. Notice this nice
//property that you do not have to tell which face of crystal is in contact with which
//face of laserbrick: Litrani will find it for you. In fact, it is face 5 of crystal which
//is in contact with face 4 of detecbrick.
  TContact *contact13;
  contact13 = new TContact("crystal-detec","crystal-detec","crystal","detecbrick",   
                           identical);
//
  const Double_t WvlgthStart = 300.0;  //starting value for wavelength
  const Double_t WvlgthStep  = 25.0;   //step in wavelength
  Double_t wavelength = WvlgthStart;
//
//------------------------------------------------------------------------------------------
//  Declaration of a spontaneous source of light inside the shape "laserbrick", at point
//(0,0,0) in the local coordinate system of the laserbrick, i.e. exactly in the middle
//of the laserbrick. This spontaneous source will emit light at the wavelength "wavelength".
  TSpontan *laser;
  laser = new TSpontan("laser","laser","laserbrick",zero,zero,zero,wavelength);
//
//  DefineCradle() is called here with only the 1st 2 arguments :
//
//    (1) - "laserbrick"  : name of shape in which the cradle of the photons is
//    (2) - point         : kind of cradle : {point, disk, ellipse, parallelogram,
//                                            parallelipiped}
//  The remaining 3->9 arguments have the following default values :
//
//    (3) - a1     = 1.0 : Irrelevant, because of (2).
//    (4) - a2     = 1.0 : Irrelevant, because of (2).
//    (5) - a3     = 1.0 : Irrelevant, because of (2).
//    (6) - theta3 = 0  : angle theta (degree, in LCS) of the Oz axis of the beam
//    (7) - phi3   = 0  : angle phi   (degree, in LCS) of the Oz axis of the beam
//    (8) - theta1 = 90 : angle theta (degree, in LCS) of the Ox axis of the beam
//    (9) - phi1   = 0  : angle phi   (degree, in LCS) of the Ox axis of the beam
//
//  Because the beam cradle is a point, the arguments 3,4,5 of DefineCradle are irrelevant.
//The default values of the arguments 6 and 7 are 0.0 and 0.0, which is exactly what we
//want, since we want a beam parallel to the Oz axis. The beam cradle being a point, the
//complementary axis Ox of the beam coordinate system, is irrelevant and we can leave the
//default values for the arguments (8) and (9). But notice that we have to call
//DefineCradle(), because by default in the constructor of TSpontan, the emission of the
//beam is of the type "on4pi", for which the axis of the beam coordinate system are irre-
//levant, but we want here an emission of the beam of type "flat", for which the direction
//of the beam coordinate system matters ! So this call to DefineCradle is there essentially
//to insure that the direction of the beam is along the Oz axis! Notice that when the
//beam cradle is a point, THE AXIS 
// - {fAxis1, fAxis2, fAxis3 }, coordinate system of the beam cradle shape (disk, ellipse,
//                              parallelogram, parallelipiped} and THE AXIS
// - {fAxisb1,fAxisb2,fAxisb3}, coordinate system of the beam, ARE IDENTICAL.
//  laser->DefineCradle("laserbrick",point);
//
//  We want a flat distribution with aperture 0, which means that all photons will be
//emitted parallel to the Oz axis in the local coordinate system of laserbrick.
//  laser->DefineBeam(flat,zero);
//  laser->DefineBeam(on4pi,zero);
  for (Int_t i=0;i<10;i++) {
//
//  Generation of 1000 photons of wavelength "wavelength". A run number must be greater
//than 0, hence 100+i and not i !
    laser->Gen(100+i,100000,wavelength);
    wavelength += WvlgthStep;
//Changes the wavelength
    laser->SetWvlgth(wavelength);
  }
//------------------------------------------------------------------------------------------
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
//Draws one of the available histogram, after having made a fit of a polynom of degree 7
  gGp->fHEfficiency->Fit("pol4","Q");
  gGs->DoStat();
  gLit->CloseFiles();
  //  gROOT->ResetBit(65536);
}
