{
  char *name    = "DamAnibm2";
  char *listing = "DamAnibm2";
  char *upcom   = "Setup with beam of muons and APD, damaged crystal";
  char *downcom = "Birefringent material and anisotropic absorption length";
  gROOT->ProcessLine(".x InitLitrani.C(3,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
//
//   DAMANIBM2
//
//  DamAnibm2.C is identical to DamAnibm1.C, except that the damages have not been
//done by electromagnetic shoxers. So we do not use ShowerDamage(), but
//RadiationDamage().
//  DamAnibm2.C is an example of a setup with a crystal of PbWO4 having the shape
//of a rectangular parallelipiped. All TSplineFit fits needed by this macro will be
//searched for in the "database" root file SplineFitDB.rdb. They are:
//
// - "AbsLengthTensor_PbWO4__000"
// - "AbsLengthTensor_PbWO4__001"
// - "DielTensor_PbWO4__000"
// - "DielTensor_PbWO4__002"
// - "DEdx_MusInPbWO4__000"
// - "DEdx_MusInPbWO4__001"
// - "AbsorptionLength_Meltmount1704"
// - "RefrIndex_Meltmount"
// - "AbsorptionLength_Silicium"
// - "RefrIndex_Silicium"
// - "RefrIndex_Nitrure"
// - "RIndexRev_Aluminium"
// - "IIndexRev_Aluminium"
// - "GainProfile_CMSAPD"
// - "RadDamage_KimaxPbWO4"
//
// In this example, the material PbWO4 is negativ birefringent.
//i.e. if ea, eb, ec are the values of the diagonal elements of the dielectric
//tensor in the system Ox'y'z' in which it is diagonal, we have ea < eb = ec. "The 3
//optical axis" are Ox' associated with ea, Oy' associated with eb and
//Oz' associated with ec. In that case, what the opticians call "The optical
//axis" [not to be confused with "The 3 optical axis"] is the axis Ox', asso-
//ciated with the "isolated" value ea. We have also taken PbWO4 has having an
//anisotropic absorption length. See the description of these features in the class
//TOpticMaterial.
//  On the upper small face of the PbWO4 crystal is an APD having the shape of a thin
//cylinder. This APD is glued onto the crystal. The slice of glue has also
//the shape of a cylinder. So there are 3 shapes in this setup : a TSBRIK (crystal of
//PbWO4) a TSCYL (glue) and an other TSCYL (APD made of silicium). The only material
//which is anisotropic (negativ birefringent) in our setup is the PbWO4. It is also
//the only material which is fluorescent, i.e. able to emit light when crossed by
//particles. The other materials are the glue, the silicium and air. To understand
//the following code affecting materials, look on the web at the description of the
//class TOPticMaterial.
//  The crystal of PbWO4 is wrapped with aluminium. Between the crystal and the
//aluminium foil is a slice of air. This is why we had to introduce material air.
//The cylinder made of glue and the cylinder made of silicium (APD) are considered
//to be wrapped with a totally absorbing revetment which is called "totabsorbing".
//To understand all the following code concerning revetments, look on the web
//at the description of the class TRevetment.
//  Between the glue and the silicium of the APD, there is a thin slice of nitrure
//which is the window of the APD. This is a true thin slice in the optical meaning
//of the word, i.e. a slice which is so thin that its thickness is of the order
//of the wavelengths, so that the normal Fresnel formulae do not apply. Litrani
//knows how to handle optical thin slices. See on the web the description of the
//class ThinSlice.
//  A beam of muons is defined which crosses the crystal of PbWO4. For that, a
//Landau distribution of dE/dx deposition has been defined for the optical
//material PbWO4. The muons have been defined has having an exponentially decreasing
//momentum distribution. See on the web the description of the class TSParticle.
//  The beam originates from an ellipse, placed on the left of the crystal, and is displa-
//ced upwards from run to run. The axis of the beam is perpendicular
//to the ellipse in direction of the crystal. The aperture of the beam is
//5 degrees. The beam crosses the crystal orthogonally to the long axis
//of the crystal. To understand the code concerning the beam, look on the
//web to the description of the class TBeam.
//
//  BEGIN OF DAMANIBM2.C
// 
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
//  DEdx_MusInPbWO4 is a family of 2 distributions.
//  The 1st one "DEdx_MusInPbWO4__000" is the Landau distribution of energy deposited
//in 1 cm of PbWO4 by muons of less than 10 Gev.
//  The 2nd one "DEdx_MusInPbWO4__001" is the Landau distribution of energy deposited
//in 1 cm of PbWO4 by muons of more than 10 Gev.
//  These 2 distributions will be used, according to the energy of the muon, to
//generate the energy deposited by a call to dedx->GetRandom(M,energy), where M
//is the number of elements in the family. Look on the web at the description
//of the class TSplineFit.
//  Definition of the material PbWO4. The magnetic permeability is declared
//to be 1, not depending upon wavelength. 2 references to fit of absorption
//lengths depending upon wavelengths are made, "AbsLengthTensor_PbWO4__000"
//and "AbsLengthTensor_PbWO4__001", which means that the absorption length
//is not isotropic.
//  PbWO4 is declared as negativ birefringent, the fits giving the parameters
//epsa and epsc as a function of wavelength are "DielTensor_PbWO4__000",
//"DielTensor_PbWO4__002".
//  PbWO4 is declared fluorescent, with one component at 420 nm and another
//one at 500 nm. These 2 components are splitted into 3 possible decay times.
//  A call to dEdx indicates that PbWO4 is able to emit light when crossed
//by particles.
  TOpticMaterial *pbwo4;
  pbwo4 = new TOpticMaterial("PbWO4","PbWO4 crystal",kFALSE,1.0,
                             "AbsLengthTensor_PbWO4__000","AbsLengthTensor_PbWO4__001");
  pbwo4->IsUniAxialNegBirefr("DielTensor_PbWO4__000","DielTensor_PbWO4__002");
//  pbwo4->FluoComponent(420.0,40.0,0.2925,5.0);
//  pbwo4->FluoComponent(420.0,40.0,0.45  ,15.0);
//  pbwo4->FluoComponent(420.0,40.0,0.0075,100.0);
//  pbwo4->FluoComponent(500.0,50.0,0.0975,5.0);
//  pbwo4->FluoComponent(500.0,50.0,0.15  ,15.0);
//  pbwo4->FluoComponent(500.0,50.0,0.0025,100.0);
  TLitSpectrum *sp;
  sp = new TLitSpectrum("Spectrum_PbWO4g2","Emission Spectrum | PbWO4 old spectrum with 2 gaussians");
  sp->AddOneComponent(420.0,40.0,0.75,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->AddOneComponent(500.0,50.0,0.25,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->Normalize();
  pbwo4->SetLitSpectrum(sp);
  pbwo4->dEdx("DEdx_MusInPbWO4__000",100.0);
//
//  Definition of air, with magnetic permeability of 1 and absorption length
//of 10000 cm, independent of wavelength. Air is also isotropic, with index
//of refraction of 1.0 independent of wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0);
  air->IsIsotropic(1.0);
//
//  Definition of glue, with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsorptionLength_Meltmount1704".
//Glue is isotropic with index of refraction depending  upon wavelength according
//to the fit "RefrIndex_Meltmount".
  TOpticMaterial *glue;
  glue = new TOpticMaterial("Glue","Glue",kFALSE,1.0,"AbsorptionLength_Meltmount1704");
  glue->IsIsotropic("RefrIndex_Meltmount");
//
//  Definition of silicium, defined as sensible ( i.e. if a photon dies inside, it
//is considered as detected), with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsorptionLength_Silicium".
//Silicium is isotropic with index of refraction depending  upon wavelength according
//to the fit "RefrIndex_Silicium".
  TOpticMaterial *Silicium;
  Silicium = new TOpticMaterial("Silicium","Silicium of APD",kTRUE,1.0,"AbsorptionLength_Silicium");
  Silicium->IsIsotropic("RefrIndex_Silicium");
//
//  Definition of the thin slice of nitrure being the window of the APD. It has a
//thickness of 125 nm and has an index of refraction varying with wavelength accor-
//ding to the fit "RefrIndex_Nitrure". Look on the web at the description of class
//ThinSlice for more details.
  ThinSlice *tsAPD;
  tsAPD = new ThinSlice("WindowAPD","WindowAPD",125.0,"RefrIndex_Nitrure");
//
//  Definition of the revetment aluminium, with a slice of air in-between ( a slice,
//not a thin slice ) with real part and imaginary part of index of refraction
//varying in function of wavelength according to the fits "RIndexRev_Aluminium" and 
//"IIndexRev_Aluminium". The magnetic permeability is 1, independent of wavelength.
//There is no supplementary absorption. The revetment is 100% reflecting, 0%
//diffusing.
  TRevetment *aluminium;
  aluminium = new TRevetment("Aluminium","Aluminium","Air",0.0,
                             "RIndexRev_Aluminium","IIndexRev_Aluminium",1.0);
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
  totabsorbing = new TRevetment("TotAbsorbing","Totally absorbing revetment","none",0.0,2.0,6.0,1.0,1.0);
//
//  Now comes the definition of the geometry. The crystal is assumed to have the
//shape of a rectangular parallelipiped ( a TSBRIK ) with semi-axis dx1, dy1, dz1.
//The long axis of the crystal is along Oz. The shape "crystal" is taken as the
//reference shape ( because of the code node1->cd(); ). So that the local
//coordinate system of the shape "crystal" is at the same time the world coordinate
//system. The shape "crystal" is made of PbWO4 and is covered with Aluminium,
//except for the parts of the shape which are in contact with other shapes.
//Look on the web at the description of the classes TSBRIK, TSupplShape and
//TFace  for a description of all possible features.
  Double_t dx1 = 1.0;       //half width of crystal in Ox direction
  Double_t dy1 = 1.0;       //half width of crystal in Oy direction
  Double_t dz1 = 12.0;      //half length of crystal axis, put here along Oz
  Double_t dz2 = 0.0025;    //half width of glue between crystal and APD
  Double_t dz3 = 0.005;     //half width of silicium of APD
  Double_t z2  = dz1 + dz2; //position, in WCS, of the center of the glue shape
  Double_t z3;
  z3 = z2  + dz2 + dz3;     //position, in WCS, of the center of the APD.
  Double_t r = 0.4;         //radius of APD and glue shape.
  const Double_t zmean = 5.0;
  const Double_t zsig  = 2.0;
  const Double_t rsig  = 1.0;
  Bool_t damok;
  TSupplShape *scrys;
  TSBRIK *crystal;
  crystal = new TSBRIK("crystal","CMS crystal","PbWO4","Aluminium",dx1,dy1,dz1);
  T3Vector damaxis(zero,zero,un);
  T3Vector damstart(zero,zero,-dz1);
  scrys      = crystal->fSuppl;
  damok = scrys->RadiationDamage(&damaxis,&damstart,"RadDamage_KimaxPbWO4","RadDamage_GzPbWO4",rsig);
  scrys->RecordDamages(40,2.0*dz1,40,1.5); //remove in case of prod! time consuming!
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
//  To understand the construction of the dielectric tensor both in Local Coordinate
//System and in the World Coordinate system, read the general description of the
//class TSupplShape.
//  We choose here to put "The optical axis" Ox', in an arbitrary position (thetax
//= 60 degree, phix = 30 degree). You can verify that with (thetay = 90 degree,
//phiy = 120 degree) we have an Oy' axis which is orthogonal to the Ox' axis. If all
//this is unclear for you, please refer to the web documentation of the classes
//TOpticMaterial and TSupplShape.
  crystal->SetOpticalAxis(60,30,90,120);
//
//Definition of the cylindrical glue shape.
  TSCYL *glueshape;
  glueshape = new TSCYL("glueshape","glueshape","Glue","TotAbsorbing",r,dz2);
//
//Definition of the cylindrical APD
  TSCYL *APD;
  APD = new TSCYL("APD","Shape of APD","Silicium","TotAbsorbing",r,dz3);
//
//Declares that this shape is an APD. Look at the web documentation about class
//TSCYL and TSupplShape if you do not understand the appearance of the pointer
//fSuppl in the following line. The entry face of the APD is face 0 of TSCYL and
//the end face of the APD is face 1. If you do not understand why, look at the web
//documentation about the class TSCYL, in particular how faces are defined for a
//TSCYL. The gain profile of the APD is defined by the TSplineFit interpolation
//"GainProfile_CMSAPD". If you do not know what the gain profile
//of an APD is, look at the web documentation of the class TSupplShape, method
//SetAPD().
  APD->fSuppl->SetAPD("hamamatsu","hamamatsu",0,1,"GainProfile_CMSAPD");
//
//use different colors for the 3 shapes
  crystal->SetLineColor(1);
  crystal->SetLineWidth(2);
  glueshape->SetLineColor(4);
  glueshape->SetLineWidth(2);
  APD->SetLineColor(3);
  APD->SetLineWidth(2);
//
// TSNode gives the position of a shape with respect to the node currently
//selected, by mean of a translation vector and a rotation matrix. No node
//being actually selected, node1 will be the main node and will define the
//WCS (world coordinate system). As no translation vector is given, the
//crystal will not be translated (translation vector 0 assumed). As no
//rotation matrix is given, the crystal will not be rotated and will remain
//with its axis parallel to Oz.
  TSNode *node1 = new TSNode("node1","node1",crystal);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);
//
//node1, the crystal, is selected and will remain the reference for all other
//nodes.
  node1->cd();
//
//node2 will give the position of the glue shape with respect to the node
//currently selected, the crystal node. A translation vector (0,0,z2) will
//put the glue shape exactly on top of the crystal. No rotation matrix
//being given, the cylindrical glue shape will stay with its axis parallel
//to the Oz axis.
  TSNode *node2 = new TSNode("node2","node2",glueshape,0.0,0.0,z2);
//
//node3 will give the position of the APD with respect to the node
//currently selected, the crystal node. A translation vector (0,0,z3) will
//put the APD exactly on top of the glue shape. No rotation matrix
//being given, the cylindrical APD will stay with its axis parallel
//to the Oz axis.
  TSNode *node3 = new TSNode("node3","node3",APD,0.0,0.0,z3);
//
//  It remains to tell Litrani that there is a contact between the crystal
//and the glue. Notice this nice property that you do not have to tell
//which face of the crystal is in contact with which face of the glue shape:
//Litrani will find it for you. The face of the crystal in contact with
//the glue face does contains it entirely, hence "containing". This contact is
//declared as having no thin slice. Look at the web documentation of the class
//TContact for more detailed explanations.
  TContact *contact12;
  contact12 = new TContact("PbWO4-Glue","PbWO4-Glue","crystal","glueshape",containing);
//
//  It remains to tell Litrani that there is a contact between the glue shape
//and the APD. The face of the glue shape in contact is identical with the
//APD entrance, hence "identical". Notice that there is a thin slice
//"WindowAPD" between the glue and the silicium of the APD. "WindowAPD" has
//been defined above. Look at the web documentation of the class TContact for
// more detailed explanations.
  TContact *contact23;
  contact23 = new TContact("Glue-APD","Glue-APD","glueshape","APD",identical,"WindowAPD");
//
//
//  Defines a particle of type muon, with an exponential energy distribution. The
//first numerical parameter, 1.0 ( in principle mean energy ) is not used in the
//case of an exponential momentum distribution. The minimum and maximum momentum
//values are 0.5 and 10 Gev. The next value of 1.0 ( in principle the sigma of the
//distribution ) is not used  in the case of an exponential momentum distribution.
//The last parameter, 1.5, is the slope of the exponential distribution. Look at
//the web documentation about the class TSParticle for more details. Notice that
//as the mass of the particle is not given, the default : mass of the muon, will
//be taken.
  TSParticle *part = new TSParticle("muon","muon",exponential,1.0,0.5,10.0,1.0,1.5);
  const Double_t x = 0.0;
  const Double_t y = -2.0;
  Double_t z = -10;
  Double_t dz = 4;
  TBeam *pbeam;
//T3Vector is the class defining 3 vectors in Litrani.
  T3Vector center(x,y,z);
//
//  Loop displacing the beam upwards along z
  for (Int_t i=0;i<6;i++) {
//
//  Defines a beam of particles part (muons). Because "crystal" is given as the
//4th argument of the constructor, all coordinates used for defining the beam
//are in the local coordinate system of the crystal. The beam is coming from
//a surface which is defined as an ellipse, centered at (0,-2,z). The constructor
//invoked here is the second constructor, the one best suited when the 2 axis of
//the emitting surface of the beam (here the 2 axis of the ellipse) are orthogonal.
//0.8 and 0.5 are the lengths of the 2 semi-axis of the ellipse. The axis
//normal to the ellipse ( let us define it as Oz" ) has the 2 angles (theta3=90,
//phi3=90), so that it points parallel to the Oy axis, towards the crystal, crossing
//the crystal orthogonally to the long axis of the crystal. The large axis of the
//ellipse ( let us define it as Ox" ) has the 2 angles (theta1=90, phi1=0) so that
//it points along the Ox axis. The small axis of the ellipse, Oy", need not be
//defined because (Ox", Oy", Oz") have to build an orthonormal system. This implies
//here that Oy" = - Oz. Look at the web documentation of the class TBeam if something
//is unclear to you.
    pbeam = new TBeam("mu_beam","mu_beam",part,"crystal",center,ellipse,
                   0.8,0.5,90.0,90.0,90.0,0.0);
//
//  The instance of the method Beam() called here implies that the direction of the
//axis of emission of the beam is parallel to the axis normal to the emitting surface
//of the beam (the ellipse). If it would not have been the case, we ought to have
//called the other instance of the method Beam(), where direction of emission of the
//beam has to be given. The distribution of emission of the beam is flat, i.e.
//sin(theta)*dtheta*dphi within the aperture of 5 degrees.
    pbeam->Beam(flat,5.0);
//
// Start run 100, generating 5 particle (feel free to generate less or more !). Recording of
//the separate statistics and histograms for this run is asked. The histograms of the
//preceeding run are cleaned from memory before starting this new run, but they are saved
//into the ROOT file "DamAnibm2.root". z is xparam, the parameter which varies from run to
//run and will be used as x axis in the histograms done by the class TPublication.
    pbeam->Gen(100+i,5,z,kTRUE,kFALSE);
    delete pbeam;
    z += dz;
//displaces the beam
    center.Set(x,y,z);
  }
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
//power of ROOT ! Look at:
// http://gentit.home.cern.ch/gentit/intro/intro4.html
//for a general description of statistics and histograms in Litrani
// Look at the web description of the classes Tresults and TPublication for more
//details. You will learn there that for instance gGp is a pointer towards the
//summary histograms of Litrani. Giving to the title of the class TPublication
//pointed to by gGp "z coordinate of beam" will have for result that all the
//summary histograms will contain this in their title. For instance, the histogram
//on efficiency that we plot here will have as title :
//  "Efficiency versus z coordinate of beam".
//
  gGp->SetTitle("z coordinate of beam");
//Summary() creates all summary histograms
  gGp->Summary();
//Draws one of the available histogram.
  scrys->ShowDamage();
  char cc;
  cout << "Please enter any character to continue : ";
  cin  >> cc;
  gGs->DoStat();
  gLit->CloseFiles();
}
