{
  char *name    = "AniFibre";
  char *listing = "AniFibre";
  char *upcom   = "Simulation of a fibre with varying incident angle";
  char *downcom = "Crystal of PbWO4 in truncated pyramidal form. APD as detector";
  gROOT->ProcessLine(".x InitLitrani.C(4,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
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
//    ANIFIBRE
//
//  AniFibre.C is an example of a setup with a crystal of PbWO4 having the shape
//of a truncated pyramid, type TSTRD2. All TSplineFit fits used in this example
//will be searched on the database ROOT file SplineFitDB.rdb.
// In this example, the material PbWO4 is treated as totally
//anisotropic, which is not the case in reality. In reality, PbWO4 is negativ
//birefringent. But we treat it as totally anisotropic to exercise this feature
//of Litrani. Totally anisotropic means that if ea, eb, ec are the values of the
//diagonal elements of the dielectric tensor in the system Ox'y'z' in which it is
//diagonal, we have ea < eb < ec. "The 3 optical axis" are Ox' associated with ea,
//Oy' associated with eb and Oz' associated with ec. In that case, what the opticians
//call "The optical axis" [not to be confused with "The 3 optical axis"] is not
//defined : it exists only in case of negativ or positiv birefringence. We have also
//taken PbWO4 has having an anisotropic absorption length. See the description of
//these features in the class TOpticMaterial.
//  We warn the user that in this example, something is complicated : the geometry,
//so that this example can be considered as a good example for those willing to
//learn the geometry in ROOT. The geometry in ROOT is similar to that of GEANT, so
//that what we consider here as complicated may appear triviality for an oldtimer of
//GEANT. Something which can help very much the user to understand the geometry of
//the complicated form TSTRAP is to look not only at the web description of TSTRAP
//but also at the source code of TSTRAP, method SetPoints(). Let us now explain the
//geometry of this example :
//  The crystal has the shape of a truncated pyramid, TSTRD2. On the lower face of
//the PbWO4 crystal is an APD having the shape of a thin cylinder, type TSCYL. This
//APD is glued onto the crystal. The thin slice of glue has also the shape of a thin
//cylinder. On the upper part of of the PbWO4 crystal is an optical fibre, having the
//shape of a lean cylinder, always of type TSCYL, which is the source of light.
//Between the crystal and the optical fibre, there is a shape made of grease. We want
//to study in this example the effect of the optical fibre being tilted with respect
//to the crystal, so that the axis of the fibre is not aligned with the axis of the
//crystal. We tilt the fibre in the Oyz plane of the crystal. It implies that the
//shape of the grease has a lower plane parallel to the upper plane of the crystal
//(perpendicular to Oz) and an upper plane having a normal in the Oyz plane, making
//an angle delta with the Oz axis. It can be realized by giving to the grease the
//shape of a TSTRAP, on the condition of making face 3 of the TSTRAP orthogonal
//to the Ox axis of the TSTRAP and face 1 of the TSTRAP making an angle delta
//with the face 3. (Look at the web description of TSTRAP to know which is face 3
//and which is face 1). After that, the TSTRAP has to be rotated and translated in
//such a way that face 3 comes exactly on top of the crystal. Besides this, the TSTRAP
//must fulfill a list of conditions which will be given below.
//  So there are 5 shapes in this setup : a TSTRD2 (crystal of PbWO4), a TSCYL (glue),
//an other TSCYL (APD made of silicium), a TSTRAP (grease) and a TSCYL (fibre). The
//only material which is anisotropic is the PbWO4. We have not defined PbWO4 as
//fluorescent, since the source of light in this example is the optical fibre.
//The other materials are the glue, the silicium, the grease, plexi (the material of
//the optical fibre) and air. To understand everything concerning materials, look on the
//web at the description of the class TOpticMaterial. Notice that the absorption length
//of the plexi is put to 0. To understand why, look at the very detailed explanations in
//the class description of TSpontan, point (B) "Simulating an optical fibre". On the
//conrary, we have to give the right value of the refractive index of the plexi, since
//photons have to cross from plexi to grease.
//  The crystal of PbWO4 is wrapped with aluminium. Between the crystal and the
//aluminium foil is a slice of air. This is why we had to introduce material air.
//The cylinder made of glue, the cylinder made of silicium (APD), the TSTRAP made of
//grease and the cylinder of the fibre are considered to be wrapped with a totally
//absorbing revetment which is called "totabsorbing". To understand all the following
//code concerning revetments, look on the web at the description of the class TRevetment.
//  Between the glue and the silicium of the APD, there is a thin slice of nitrure
//which is the window of the APD. This is a true thin slice in the optical meaning
//of the word, i.e. a slice which is so thin that its thickness is of the order
//of the wavelengths, so that the normal Fresnel formulae do not apply. Litrani
//knows how to handle optical thin slices. See on the web the description of the
//class ThinSlice.
//  We make 9 runs, changing the tilt of the fibre by 5 degrees from -20 to +20 degree.
//
//  BEGIN OF ANIFIBRE.C
//
  Bool_t RealDimensions = kTRUE;
  char c;
  cout << endl;
  cout << "    You can have a run without generation of photons and with" << endl;
  cout << "  geometrical dimensions of small objects changed in such a" << endl;
  cout << "  way that they appear clearly on the drawing of the setup." << endl;
  cout << "    Or you can have a run with generation of photons and with" << endl;
  cout << "  true dimensions of all objects" << endl;
  cout << endl;
  cout << "Do you want generation of photons ? (y/n) ";
  cin  >> c;
  if (c=='n') RealDimensions = kFALSE;
//
//  Definition of the material PbWO4. The magnetic permeability is declared
//to be 1, not depending upon wavelength. 3 references to fit of absorption
//lengths depending upon wavelengths are made, "AbsLengthTensor_PbWO4__000/1/2", which means that
//the absorption length is totally anisotropic. The fact that these 3 fits are grouped
//into a family of fits does not play any role here.
//  PbWO4 is declared as birefringent (which means totally anisotropic, neither negativ
//birefringent, nor positiv birefringent, nor isotropic), the fits giving the parameters
//epsa/b/c as a function of wavelength are "DielTensor_PbWO4__000","DielTensor_PbWO4__001",
//"DielTensor_PbWO4__002". The fact that these 3 fits are grouped into a family of fits
//does not play any role here.
  TOpticMaterial *pbwo4;
  pbwo4 = new TOpticMaterial("PbWO4","PbWO4 crystal",kFALSE,1.0,
    "AbsLengthTensor_PbWO4__000","AbsLengthTensor_PbWO4__001","AbsLengthTensor_PbWO4__002");
  pbwo4->IsBirefr("DielTensor_PbWO4__000","DielTensor_PbWO4__001","DielTensor_PbWO4__002");
//
//  Definition of air, with magnetic permeability of 1 and absorption length
//of 10000 cm, independent of wavelength. Air is also isotropic, with index
//of refraction of 1.0 independent of wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0);
  air->IsIsotropic(1.0);
//
//  Definition of glue, with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsorptionLength_Meltmount1704". Glue is isotropic with
//index of refraction depending  upon wavelength according to the fit "RefrIndex_Meltmount".
  TOpticMaterial *glue;
  glue = new TOpticMaterial("Glue","Glue",kFALSE,1.0,"AbsorptionLength_Meltmount1704");
  glue->IsIsotropic("RefrIndex_Meltmount");
//
//  Definition of silicium, defined as sensible ( i.e. if a photon dies inside, it
//is considered as detected), with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsorptionLength_Silicium". Silicium is isotropic
//with index of refraction depending  upon wavelength according to the fit
//"RefrIndex_Silicium".
  TOpticMaterial *Silicium;
  Silicium = new TOpticMaterial("Silicium","Silicium of APD",kTRUE,1.0,"AbsorptionLength_Silicium");
  Silicium->IsIsotropic("RefrIndex_Silicium");
//
//  Definition of grease, with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsorptionLength_Q23067". Grease is isotropic with
//index of refraction depending  upon wavelength according to the fit "RefrIndex_Q23067".
  TOpticMaterial *grease;
  grease = new TOpticMaterial("Grease","Grease",kFALSE,1.0,"AbsorptionLength_Q23067");
  grease->IsIsotropic("RefrIndex_Q23067");
//
//  Definition of plexi, the material of the fibre, with magnetic permeability of 1 and
//absorption length of 0 cm. Plexi is isotropic with index of refraction depending  upon
//wavelength according to the fit "RefrIndex_Plexi".
//  Why an absorption length of 0 for plexi ? There is a good reason for that : material
//plexi is used here for an optical fibre. If instead of coming out of the fibre by
//refraction, a photon is reflected inside the fibre, you do not want it to be followed
//by Litrani. You want it to be considered as lost. It will be lost immediatly if its
//absorption length is 0. Besides this, in the special case of photons absorbed in a
//material of absorption length 0, photons are counted apart from the other losses, and
//do not contribute to the generated photons. They are considered as having not been
//generated, so that they do not falsify the calculation of efficiency, for instance.
//For more explanations, look at the class description of TSpontan, point (B).
  TOpticMaterial *plexi;
  plexi = new TOpticMaterial("Plexi","Plexi",kFALSE,1.0,0.0);
  plexi->IsIsotropic("RefrIndex_Plexi");
//
//  Definition of the revetment aluminium, with a slice of Air inbetween ( a slice,
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
  totabsorbing = new TRevetment("TotAbsorbing","Totally absorbing revetment");
//
//  Definition of the thin slice of nitrure being the window of the APD. It has a
//thickness of 125 nm and has an index of refraction varying with wavelength accor-
//ding to the fit "RefrIndex_Nitrure". Look on the web at the description of class
//ThinSlice for more details.
  ThinSlice *tsAPD;
  tsAPD = new ThinSlice("WindowAPD","WindowAPD",125.0,"RefrIndex_Nitrure");
//
//    GEOMETRY
//
//  The crystal is assumed to have the shape of a truncated pyramid, shape TSTRD2. The
//long axis of the crystal is along Oz.The shape "crystal" is taken as the reference shape
//( because of the code node1->cd(); ). So that the local coordinate system of the shape
//"crystal" is at the same time the world coordinate system. The shape "crystal" is made
//of PbWO4 and is covered with Aluminium, except for the parts of the shape which are in
//contact with other shapes. Look on the web at the description of the classes TSTRD2,
//TSupplShape and TFace  for a description of all possible features. Notice that in this
//code, there are "true" and "false" dimensions for the geometry ! The false dimensions
//are there only to produce a nice drawing without objects so small that they are
//scarcely visible.
  const Double_t crystal_dx1 = 2.5;    //half x length at -dz in cm
  const Double_t crystal_dx2 = 2.0;    //half x length at +dz in cm
  const Double_t crystal_dy1 = 2.5;    //half y length at -dz in cm
  const Double_t crystal_dy2 = 2.0;    //half y length at +dz in cm
  const Double_t crystal_dz  = 6.0;    //half-length along the z-axis in cm
  const Double_t glue_r      = 0.4;    //radius of glue shape.
  const Double_t R_glue_dz   = 0.0025; //true  half width of glue between crystal and APD
  const Double_t F_glue_dz   = 0.5;    //false half width of glue between crystal and APD
  Double_t glue_dz;
  if (RealDimensions) glue_dz = R_glue_dz;
  else glue_dz = F_glue_dz;
  const Double_t apd_r       = glue_r; //radius of APD and glue shape.
  const Double_t R_apd_dz    = 0.005;  //true  half width of silicium of APD
  const Double_t F_apd_dz    = 0.5;    //false half width of silicium of APD
  Double_t apd_dz;
  if (RealDimensions) apd_dz = R_apd_dz;
  else apd_dz = F_apd_dz;
  Double_t glue_center = - crystal_dz - glue_dz;         //center of the glue in WCS
  Double_t apd_center  = glue_center - glue_dz - apd_dz; //center of the APD in WCS
  TSTRD2 *crystal;
  crystal = new TSTRD2("crystal","CMS crystal","PbWO4","Aluminium",crystal_dx1,
                        crystal_dx2,crystal_dy1,crystal_dy2,crystal_dz);
//
//  SetOpticalAxis sets the 3 optical axis Ox', Oy' and Oz'. The 3 optical axis
//are the 3 axis of the coordinate system in which the dielectric tensor is diagonal,
//with eigenvalues :
//
//   ea   associated with Ox' axis
//   eb   associated with Oy' axis
//   ec   associated with Oz' axis
//
//  These 3 axis are given "a la GEANT" : give theta and phi of Ox' axis (thetax,
//phix) theta and phi of Oy' axis (thetay,phiy). It is not necessary to give theta
//and phi of Oz' : Oz' can be determined with Ox' and Oy'. This way of
//giving a coordinate system inside the local coordinate system of a shape is
//intuitive, but inelegant, because it is redundant. If you give an Oy' axis which
//is not orthogonal to the Ox' axis, you will be warned and your Oy' axis will be
//corrected. All angles are to be given in degree.
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
  glueshape = new TSCYL("glueshape","glueshape","Glue","TotAbsorbing",glue_r,glue_dz);
//
//Definition of the cylindrical APD
  TSCYL *APD;
  APD = new TSCYL("APD","Shape of APD","Silicium","TotAbsorbing",apd_r,apd_dz);
//
//Declares that this shape is an APD. Look at the web documentation about class
//TSCYL and TSupplShape if you do not understand the appearance of the pointer
//fSuppl in the following line. The entry face of the APD is face 1 of TSCYL and
//the end face of the APD is face 0. If you do not understand why, look at the web
//documentation about the class TSCYL, in particular how faces are defined for a
//TSCYL. The gain profile of the APD has already been defined by the interpolation
//"GainProfile_CMSAPD" of the file f_profileapd.C. If you do not know what the gain profile
//of an APD is, look at the web documentation of the class TSupplShape, method
//SetAPD().
  APD->fSuppl->SetAPD("hamamatsu","hamamatsu",1,0,"GainProfile_CMSAPD");
//
//use different colors for the 3 shapes
  crystal->SetLineColor(1);
  crystal->SetLineWidth(2);
  glueshape->SetLineColor(4);
  glueshape->SetLineWidth(2);
  APD->SetLineColor(3);
  APD->SetLineWidth(2);
//
//TSNode gives the position of a shape with respect to the node currently
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
//
//node2 will give the position of the glue shape with respect to the node
//currently selected, the crystal node. A translation vector (0,0,glue_center) will
//put the glue shape exactly under the crystal. No rotation matrix
//being given, the cylindrical glue shape will stay with its axis parallel
//to the Oz axis.
  TSNode *node2 = new TSNode("node2","node2",glueshape,0.0,0.0,glue_center);
//
//node3 will give the position of the APD with respect to the node currently selected,
//the crystal node. A translation vector (0,0,apd_center) will put the APD exactly under
//the glue shape. No rotation matrix being given, the cylindrical APD will stay with its
//axis parallel to the Oz axis.
  TSNode *node3 = new TSNode("node3","node3",APD,0.0,0.0,apd_center);
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
//  We will give the name "bellows" to the TSTRAP shape of the grease, because
//its shape looks a little bit as a pair of bellows. The following code will only become
//clear to you if you look at the documentation about TSTRAP on the web. Look also at
//the source code, method SetPoints() ! We use here the variable delta defined above
//(tilt angle of the fibre, angle between face 3 and face 1 of the TSTRAP) and D, the mean
//width of the TSTRAP along the Ox axis of the TSTRAP (along the Oz axis of the crystal).
//  Notice that the TSTRAP that we need here has the following properties :
//
//  theta =0       phi=0      h1=h2      bl1=bl2      tl1=tl2
//  alpha1=alpha2  and        alpha1 such that points 0, 3, 4 and 7 have the same x
//                                   coordinate, which implies :
//
//  tth1 = tg(alpha1) = (bl1-tl1)/h1
//
//  delta is the tilt angle of the fibre. So by definition, it is the angle between
//face 3 (composed of points 3047) and face 1 (composed of points 1265). This implies :
//
//  cos(delta) = h1 / Sqrt((bl1-tl1)^2 + h1^2)        From there, one obtains :
//  bl1 = (D + h1*tg(delta))/2       tl1 = (D - h1*tg(delta))/2
//
//  D is the full mean width along Ox. It is the distance between the middle of the
//segment (0,3) and the middle of the segment (1,2). This implies :
//
//  D = bl1 + tl1
//
//  The mimimal value of the width is the distance between point 0 and point 1, if delta
//is positiv, and is the distance between point 2 and 3 if delta is negativ. This
//minimal value of width cannot be negativ, which implies :
//
//  D >= Abs(h1*tg(delta))
//
//  We leave to the user the demonstration of all these formulae, which is an excellent
//exercise in geometry.
//
  const Double_t zero = 0.0;
  const Double_t z05 = 0.5;
  const Double_t pideg = 180.0;
  const Double_t R_Dmin = 0.01;      //real  width of the bellows, if possible.
  const Double_t F_Dmin = 1.0;       //false width of the bellows, if possible.
  Double_t Dmin;
  if (RealDimensions) Dmin = R_Dmin;
  else Dmin = F_Dmin;
  const Double_t deltaDstart = -20.0;//starting value for delta in degree
  const Double_t deltaDstep = 5.0;   //step for delta in degree
  const Double_t R_bow_dz = 0.015;   //real  half-length of the TSTRAP along its z axis in cm
  const Double_t F_bow_dz = 0.8;     //false half-length of the TSTRAP along its z axis in cm
  Double_t bow_dz;
  if (RealDimensions) bow_dz = R_bow_dz;
  else bow_dz = F_bow_dz;
  const Double_t R_bow_h1 = 0.015;   //real  half-length along y of the face at -DZ
  const Double_t F_bow_h1 = 0.8;     //false half-length along y of the face at -DZ
  Double_t bow_h1;
  if (RealDimensions) bow_h1 = R_bow_h1;
  else bow_h1 = F_bow_h1;
  const Double_t R_fib_r = 0.01;     //real  radius of the fibre
  const Double_t F_fib_r = 0.6;      //false radius of the fibre
  Double_t fib_r;
  if (RealDimensions) fib_r = R_fib_r;
  else fib_r = F_fib_r;
  const Double_t fib_dz = 1.0;       //length of the fibre (irrelevant)
  Double_t fib_x;                    //x of the center of the fibre in WCS
  Double_t fib_y;                    //y of the center of the fibre in WCS
  Double_t fib_z;                    //z of the center of the fibre in WCS
  Double_t bow_alf1;                 //alpha1 = alpha2 of the TSTRAP
  Double_t deltastart;               //starting value for delta in radian
  Double_t deltastep;                //step for delta in radian
  Double_t torad = TMath::Pi()/pideg;
  Double_t D;                        //width of the bellows (=Dmin if possible)
  Double_t bow_center;               //shift in z to put bowel in place
  Double_t deltaD;                   //angle delta in degree
  Double_t delta;                    //angle delta in radian
  Double_t tgd;                      //tangent of angle delta
  Double_t htgd;                     //bow_h1*tgd
  Double_t bl1;                      //half-length along x of the side at -H1 in y of the
                                     // face at -DZ in z (of the TSTRAP)
  Double_t tl1;                      //half-length along x of the side at +H1 in y of the
                                     // face at -DZ in z (of the TSTRAP)
  deltastart = torad*deltaDstart;
  deltastep  = torad*deltaDstep;
  delta      = deltastart;
  deltaD     = deltaDstart;
  TSTRAP *bowels;
  TRotMatrix *matrixbow;
  matrixbow = new TRotMatrix("MatrixBow", "MatrixBow",180,0,90,90,90,0);
  TSNode *node4;
  TSCYL *fibre;
  TRotMatrix *matrixfib;
  TSNode *node5;
  TSpontan *laser;
  TContact *contact54;
  TContact *contact41;
  for (Int_t run=96;run<105;run++) {
    tgd  = TMath::Tan(delta);
    htgd = bow_h1*tgd;
    D    = TMath::Max(Dmin,TMath::Abs(htgd));
    bl1  = z05*(D + htgd);
    tl1  = z05*(D - htgd);
    bow_center = crystal_dz + z05*D;
    bow_alf1 = TMath::ATan((z05*(bl1-tl1))/bow_h1)/torad;
    bowels = new TSTRAP("bowels","bowels","Grease","TotAbsorbing",bow_dz,zero,zero,
                         bow_h1,bl1,tl1,bow_alf1,bow_h1,bl1,tl1,bow_alf1);
    bowels->SetLineColor(28);
    bowels->SetLineWidth(2);
    node4 = new TSNode("node4","node4",bowels,zero,zero,bow_center,matrixbow);
    fibre = new TSCYL("fibre","fibre","Plexi","TotAbsorbing",fib_r,fib_dz);
    fibre->SetLineColor(28);
    fibre->SetLineWidth(2);
    fib_x = zero;
    fib_y = fib_dz*TMath::Sin(delta);
    fib_z = crystal_dz + D + fib_dz*TMath::Cos(delta);
    cout << "deltaD : " << deltaD << endl;
    matrixfib = new TRotMatrix("Matrixfib","Matrixfib",90.0,zero,deltaD+90.0,90.0,
      deltaD,90.0);
    node5 = new TSNode("node5","node5",fibre,fib_x,fib_y,fib_z,matrixfib);
//
//  It remains to tell Litrani that there is a contact between the fibre and the grease.
//The face of the fibre in contact with the grease face is contained in it entirely, hence
//"contained". This contact is declared as having no thin slice.
    contact54 = new TContact("Fibre-Grease","Fibre-Grease","fibre","bowels",contained);
//
//  There is a contact between the grease and the crystal. The face of the grease in
//contact with the crystal face is contained in it entirely, hence "contained". 
    contact41 = new TContact("Grease-crystal","Grease-crystal","bowels","crystal",contained);
//
//  It is a good idea to make a drawing of the setup. Any geometrical mistake will
//appear as an awful drawing ! Notice however that ROOT has a very strange and
//questionable way of drawing a setup : the proportion of the 3 axis are not
//respected by ROOT ! ROOT feels free to affect different factor to the 3 axis Ox,
//Oy and Oz ! So do not be surprised by the proportions !
    if (run==96) {
      gLit->BookCanvas();
      gTwoPad->SetStateOfPads(OnePad);
      node1->Draw();
    }
//
//  Generation of photons of 500 nm, issuing from face 0 of the fibre.  Look at the
//class description of TSpontan and of the methods DefineBeam() and Gen() for more
//explanations. gLit is a global pointer towards class TLitGlob, a class handling the
//pads and canvas of Litrani, and storing Litrani collection of objects.
    if (RealDimensions) {
      laser = new TSpontan("laser","laser","fibre",500,0);
      laser->DefineBeam(provided,22.5,"AngularDistrib_FibreCMS");
      laser->Gen(run,10000,deltaD,kTRUE,kFALSE);
    }
    gTwoPad->UpdatePad1();
    if (!RealDimensions) {
      gTwoPad->UpdatePad1();
      cout << "Type any character to continue ";
      cin >> c;
    }
    else {
      delete laser;
      laser = 0;
    }
    delete contact41;
    contact41 = 0;
    delete contact54;
    contact54 = 0;
    delete node5;
    node5 = 0;
//this line to be removed when ROOT corrected :
//    gGeometry->GetListOfMatrices()->Remove(matrixfib);
    delete matrixfib;
    matrixfib=0;
    delete fibre;
    fibre = 0;
    delete node4;
    node4 = 0;
    delete bowels;
    bowels = 0;
    delta  += deltastep;
    deltaD += deltaDstep;
  }
//
//end of loop
//
  gTwoPad->ChangePad();
//  Here all runs of generation of photons are finished. It remains to analyze the results.
//For that, you have at hand the 2 classes TResults and TPublication, and all the
//power of ROOT ! Look at the web description of the classes Tresults and
//TPublication where you will find a full description of all types of histograms
//available at the end of a production of Litrani. You will learn there that
//for instance gGp is a pointer towards the summary histograms of Litrani. Naming
//the title of the class TPublication (pointed to by gGp) "tilt angle of the fibre"
//will have for result that all the summary histograms will contain this in their
//title. For instance, the histogram on efficiency that we plot here will have as
//title : "Efficiency versus tilt angle of the fibre".
  if (RealDimensions) {
    gGp->SetTitle("tilt angle of the fibre");
//Summary() creates all summary histograms
    gGp->Summary();
//Draws one of the available histogram.
    gGs->DoStat();
    gMes->W("End of AniFibre");
    gLit->CloseFiles();
  }
}
