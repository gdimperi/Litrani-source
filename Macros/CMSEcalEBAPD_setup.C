{
//
//  CMSEcalEBAPD_setup.C
//
// CMSEcalEBAPD_setup.C is a description of the final setup for a crystal of the CMS experiment.
//The material PbWO4 of the crystal is correctly treated as negativ birefringent. In order
//to be able to declare as depolished face 1 of the crystal, it has been necessary to add
//a very thin shape TSTRAP in contact with face 1 of the TSTRAP shape representing the
//crystal, but made of isotropic PbWO4. This is because Litrani is not able, for the time
//being, to handle a depolished surface on a non-isotropic material. On the upper face of
//the PbWO4 crystal is the APD capsule which requires this upper face to be divided into
//face 5, the part not covered by the APD capsule, and subface 6, the part covered by the
//APD capsule. On this subface 6 are 2 rectangular APDs of 25 mm2. The capsule and the
//APDs are glued onto face 6 with the same glue. On the lower face of the PbWO4 crystal
//is the so called "ferule", a piece of white PBT containing the optical fibre. The
//optical fibre is not in contact with the crystal, there is a cylindrical air gap
//in-between.
//So there are 8 shapes in this setup :
// - (a) a TSTRAP (CMS crystal made of negativ birefringent PbWO4)
// - (b) a TSTRAP (very thin parallelipiped made of isotropic PbWO4, in contact with
//                 face 1 of (a)).
// - (c) 2 TSBRIK (glue of the APDs)
// - (d) 2 TSBRIK (APD made of silicium).
// - (e) 1 TSCYL  (air gap between optical fibre and crystal)
// - (f) 1 TSCYL  (the optical fibre)
//  The only material which is anisotropic (negativ birefringent) in our setup is the
//PbWO4. It is also the only material which is fluorescent, i.e. able to emit light when
//crossed by particles. The other materials are air, glue, silicium and silice/polymere
//hcp200 of the fibre.
//  The CMS crystal is put inside an alveola, it is the reason why we call the wrapping
//"Alveola". To describe properly the wrapping of the alveola, we have taken for the
//imaginary part of the index the one of aluminium and we have varied the real part of the
//index until it gives the right albedo of the alveola (which has been measured). We have
//done that as a function of wavelength. The wrapping "alveola" is mainly reflective, but
//has a small proportion of 11.738% of photons diffused. This also has been measured and
//is described properly by Litrani. Between the crystal and the alveola is a slice of air.
//  The TSBRIKs made of glue and the TSBRIKs made of silicium are considered to be wrapped
//with a totally absorbing wrapping which is called "totabsorbing". Between the glue and
//the silicium of the APD, there is a thin slice of nitrure which is the window of the
//APD. This is a true thin slice in the optical meaning of the word, i.e. a slice which
//is so thin that its thickness is of the order of the wavelengths, so that the normal
//Fresnel formulae do not apply. Litrani knows how to handle optical thin slices.
//
//  BEGIN OF CMSEcalEBAPD_setup.C
//
  const Double_t zero = 0.0;
  const Double_t un   = 1.0;
  const Double_t deux = 2.0;
  const Double_t radlength = 0.893; //radiation length of PbWO4 in cm
  const Double_t rMoliere  = 2.0;   //Moliere radius of PbWO4 in cm
//
//  Definition of the material PbWO4. The magnetic permeability is declared to be 1, not
//depending upon wavelength. PbWO4 is declared as negativ birefringent, the fits giving
//the parameters epsa and epsc as a function of wavelength are "Epsa_PbWO4","Epsc_PbWO4".
//The absorption length is taken as the one of crystal 1854.
//  PbWO4 is declared fluorescent, with one component at 420 nm and another one at 500 nm.
//These 2 components are splitted into 3 possible decay times.
//  A call to dEdx indicates that PbWO4 is able to emit light when crossed
//by particles.
  const Double_t PhotMev = 100.0;
  const Double_t DamageE0        = 100.0;       //Energy of shower having caused radiation damage
  const Double_t MaxApert        = 22.5;        //Maximal aperture angle of the optical fibre
  TOpticMaterial *pbwo4;
  pbwo4 = new TOpticMaterial("PbWO4","PbWO4 crystal",kFALSE,un,"AbsorptionLength_crys8981R01");
  if (isopbwo4) {
    const Double_t ns0 = 1.5861;   //index 0 for the Sellmeier law for PbWO4
    const Double_t ns1 = 1.1062;   //index 1 for the Sellmeier law for PbWO4
    const Double_t ls1 = 270.63;   //1st resonant wavelength for the Sellmeier law for PbWO4
    pbwo4->IsIsotropic(1,ns0,ns1,ls1);
  }
  else pbwo4->IsUniAxialNegBirefr("DielTensor_PbWO4__000","DielTensor_PbWO4__002");
  pbwo4->FindSpectrum("Spectrum_PbWO4La");
  pbwo4->SetCascades(radlength,rMoliere); //only useful when TElecCascade used
  pbwo4->SetPhotMev(PhotMev);
//
//  Definition of isotropic PbWO4 for the thin sheet on face 1, in order to be able to
//define a depolished surface. Litrani is not yet able to handle depolished surface when
//the material is anisotropic.
//  IsIsotropic() is called with 4 arguments, meaning that the material is treated as
//isotropic, the variation of the index of refraction with wavelength being described by
//the Sellmeier law with 3 parameters : ns0, ns1, ls1.
  TOpticMaterial *pbwo4iso;
  pbwo4iso = new TOpticMaterial("PbWO4iso","PbWO4iso",kFALSE,un,"AbsorptionLength_crys8981R01");
  const Double_t ns0 = 1.5861;   //index 0 for the Sellmeier law for PbWO4
  const Double_t ns1 = 1.1062;   //index 1 for the Sellmeier law for PbWO4
  const Double_t ls1 = 270.63;   //1st resonant wavelength for the Sellmeier law for PbWO4
  pbwo4iso->IsIsotropic(1,ns0,ns1,ls1);
//
//  Definition of air, with magnetic permeability of 1 and absorption length
//of 10000 cm, independent of wavelength. Air is also isotropic, with index
//of refraction of 1.0 independent of wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,un,10000.0);
  air->IsIsotropic(1.0003);
//
//  Definition of glue, with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsLRtv3145". Glue is isotropic with
//index of refraction depending  upon wavelength according to the fit "Refr_I_Rtv3145".
  TOpticMaterial *glue;
  glue = new TOpticMaterial("Glue","Glue",kFALSE,un,"AbsorptionLength_Meltmount1704");
  glue->IsIsotropic("RefrIndex_RTV3145");
//
//  Definition of silicium, defined as sensible ( i.e. if a photon dies inside, it
//is considered as detected), with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsLSilicium". Silicium is isotropic
//with index of refraction depending  upon wavelength according to the fit
//"Refr_I_Silicium".
  TOpticMaterial *Silicium;
  Silicium = new TOpticMaterial("Silicium","Silicium of APD",kTRUE,un,"AbsorptionLength_Silicium");
  Silicium->IsIsotropic("RefrIndex_Silicium");
//
//  Definition of the material of the fibre
  TOpticMaterial *hcp200;
  hcp200 = new TOpticMaterial("HCP200","HCP200",kFALSE,un,zero);
  hcp200->IsIsotropic("RefrIndex_hcp200");
//
//  Definition of the thin slice of nitrure being the window of the APD. It has a
//thickness of 125 nm and has an index of refraction varying with wavelength accor-
//ding to the fit "RefrNitrure". Look on the web at the description of class
//ThinSlice for more details.
  ThinSlice *tsAPD;
  tsAPD = new ThinSlice("WindowAPD","WindowAPD",125.0,"RefrIndex_Nitrure");
//
//  Definition of the wrapping alveola, with a slice of Air inbetween ( a slice, not a
//thin slice ) with real part and imaginary part of index of refraction varying in
//function of wavelength according to the fits "RIndexRev_CMSalveola" and "IIndexRev_CMSalveola".
//The magnetic permeability is 1, independent of wavelength. There is no supplementary
//absorption. The wrapping is 11.738% diffusing.
  const Double_t PropDiffus = 0.11738;
  TRevetment *alveola;
  alveola = new TRevetment("Alveola","Alveola","Air",PropDiffus,
                           "RIndexRev_CMSalveola", "IIndexRev_CMSalveola",un);
//
//  Definition of the wrapping "Revchamf", without  slice of air inbetween, with real
//part and imaginary part of index of refraction varying in function of wavelength
//according to the fits "Re_n_Revalveola" and "Im_n_Revalveola". The magnetic permeability
//is 1, independent of wavelength. There is a supplementary absorption of 0.05. The
//wrapping is 0% reflecting, 100% diffusing, to simulate a depolish. The maximum angle
//theta for a diffused photon is 70 degree.
  TRevetment *revchamf;
  revchamf = new TRevetment("Revchamf","Revchamf","none",un,"RIndexRev_CMSalveola",
                            "IIndexRev_CMSalveola",un,0.05,70.0);
//
//  Definition of the wrapping "Revcaps", describing the surface of the capsule containing
//the APD, which is in contact with face 5 of the crystal.
  TRevetment *revcaps;
  revcaps  = new TRevetment("Revcaps","Revcaps","Glue",un,"RIndexRev_CMScapsula",
                            "IIndexRev_CMScapsula",un,0.2,85.0);
//
//  Definition of the wrapping "Revferule", describing the surface of the piece fixing
//the optical fibre, which is in contact with face 4 of the crystal.
  TRevetment *revferule;
  revferule  = new TRevetment("Revferule","Revferule","Air",un,"RIndexRev_CMSferule",
                            "IIndexRev_CMSferule",un,0.2,85.0);
//
//  Definition of the wrapping "Revtopbot", describing the surface
//    - of the top of the alveola, which is the wrapping of the part of face 5 of the
//       crystal which is not covered with the APD capsule.
//    - of the bottom of the alveola, which is the wrapping of the part of face 4 of the
//crystal which is not covered with the "ferule".
//  The fact that we wrongly refer to "Tyvek" does not matter, since we have put a
//supplementary absorption of 0.99. But we have not put a supplementary absorption of 1.0,
//since we insist that there is a slice of air and that total reflection is possible.
  TRevetment *revtopbot;
  revtopbot  = new TRevetment("Revtopbot","Revtopbot","Air",un,"RIndexRev_Tyvek",
                            "IIndexRev_Tyvek",un,0.99,80.0);
//
//  Definition of the wrapping "TotAbsorbing", giving only the 1st 2 arguments. When the
//other arguments 3 to 8 are not given, following default values are assumed :
//
// (3) - nameMat  = "none" : no slice material between face and wrapping.
// (4) - diffus   = 0.0    : Reflecting, not diffusing. Irrelevant because of (8).
// (5) - Rindex   = 2.0    : Real part of index. Irrelevant because of (8).
// (6) - Iindex   = 6.0    : Imaginary part of Index. Irrelevant because of (8).
// (7) - Mu       = 1.0    : Magnetic permeability. Irrelevant because of (8).
// (8) - supplAbs = 1.0    : supplementary absorption of 100%.
//
// When a photon reaches such a wrapping, it is absorbed in any case, it has no chance
//neither of making a total reflection ( there is no slice of material between shape and
//wrapping ) nor of being reflected by the wrapping (because the wrapping has a
//supplementary absorption of 100%).
  TRevetment *totabsorbing;
  totabsorbing = new TRevetment("TotAbsorbing","Totally absorbing wrapping");
//
//  Geometry
//
//  Let us name 
//    (x0,y0,z0),(x1,y1,z1),(x2,y2,z2),(x3,y3,z3),
//    (x4,y4,z4),(x5,y5,z5),(x6,y6,z6),(x7,y7,z7)
//    z0 = z1 = z2 = z3 = -crys_dz
//    z4 = z5 = z6 = z7 = +crys_dz
//  The 8 points of the TSTRAP representing the crystal. The crystal is then centered
//in the sense that we impose:
//    x0+x1+x2+x3+x4+x5+x6+x7 = 0
//    y0+y1+y2+y3+y4+y5+y6+y7 = 0
//  The center of the crystal is then (0,0,0)
//
  const Double_t iso_dx        = 0.002; //width in y of sheet of isotropic PbWO4.
  const Double_t depolishangle = 42.0;  //max depolish angle of face 2 of isosheet
  const Double_t ferule_AF     = 2.18;  //dimension ||AF of "porte-ferule"
  Short_t crystaltype;     //1-17
  Double_t x0,x1,x2,x3,x4,x5,x6,x7;  //x coord of 8 points of crystal
  Double_t y0,y1,y2,y3,y4,y5,y6,y7;  //y coord of 8 points of crystal
  Double_t z0,z1,z2,z3,z4,z5,z6,z7;  //z coord of 8 points of crystal
  Double_t crys_dz;        //half length in z of crystal of PbWO4
  Double_t crys_AF;        //side left,  small face |_ to OZ
  Double_t crys_BF;        //side back,  small face |_ to OZ
  Double_t crys_CF;        //side right, small face |_ to OZ
  Double_t crys_AR;        //side left,  large face |_ to OZ
  Double_t crys_BR;        //side back,  large face |_ to OZ
  Double_t crys_CR;        //side right, large face |_ to OZ
  Double_t ferule_BF;      //dimension ||BF of "porte-ferule"
  Double_t crys_AFl;       //crys_AF - iso_dx;
  Double_t crys_CFl;       //crys_CF - iso_dx;
  Double_t crys_ARl;       //crys_AR - iso_dx;
  Double_t crys_CRl;       //crys_CR - iso_dx;
  Bool_t ok = kFALSE;
  while (!ok) {
    cout << endl;
    cout << "Please choose cms crystal type between 1 and 17" << endl;
    cout << endl;
    cout << "Crystal type : ";
    cin  >> crystaltype;
    ok = ((crystaltype>=1) && (crystaltype<=17));
    if (!ok) cout << "Type of crystal must lie between 1 and 17" << endl;
  }
//
// The crystal is assumed to have the shape of one of the types 1-17 of the cms crystals,
//which may be built with the Litrani shape TSTRAP. The long axis of the crystal is along
//Oz. The shape "crystal" is taken as the reference shape. So that the local coordinate
//system of the shape "crystal" is at the same time the world coordinate system WCS.
// Description of the CMS crystal by the 7 parameters AF, BF, CF, AR, BR, CR and crys_dz.
//Look at "The Electromagnetic Calorimeter Project, CMS TDR4" pages 53/54. We use the
//constructor of TSTRAP which is specialized for CMS crystal. It contains the values
//for AF,BF,CF,AR,BR,CR and ferule_BF for all 17 types of CMS crystals.
//Look at the desciption of TSTRAP.
//
  TSTRAP *crystal;
  crystal = new TSTRAP("crystal","crystal","PbWO4","Alveola",crystaltype,iso_dx);
  crys_dz = crystal->Getdz();
  crystal->GetCMSdim(crystaltype,ferule_BF,
         crys_AFl,crys_BF,crys_CFl,crys_ARl,crys_BR,crys_CRl,iso_dx);
  crys_AF = crys_AFl + iso_dx;
  crys_CF = crys_CFl + iso_dx;
  crys_AR = crys_ARl + iso_dx;
  crys_CR = crys_CRl + iso_dx;
  cout << endl;
  cout << "    Dimensions of crystal" << endl;
  cout << endl;
  cout << "half height : " << crys_dz << endl;
  cout << "AF : " << crys_AF << "   BF : " << crys_BF << "   CF : " << crys_CF << endl;
  cout << "AR : " << crys_AR << "   BR : " << crys_BR << "   CR : " << crys_CR << endl;
  crystal->GetPoints(x0,y0,z0,x1,y1,z1,x2,y2,z2,x3,y3,z3,
                     x4,y4,z4,x5,y5,z5,x6,y6,z6,x7,y7,z7);
  Double_t center_inter_x = (x0 + x1 + x2 + x3) / 4.0;
  Double_t center_inter_y = (y0 + y1 + y2 + y3) / 4.0;
  Double_t center_inter_z = (z0 + z1 + z2 + z3) / 4.0;
  Double_t center_exter_x = (x4 + x5 + x6 + x7) / 4.0;
  Double_t center_exter_y = (y4 + y5 + y6 + y7) / 4.0;
  Double_t center_exter_z = (z4 + z5 + z6 + z7) / 4.0;
//Printing
  cout << endl;
  cout << "Point 0 : ";  cout.width(8);  cout << x0 << "  ";  cout.width(8);
    cout << y0 << "  ";  cout.width(8);  cout << z0 << endl;
  cout << "Point 1 : ";  cout.width(8);  cout << x1 << "  ";  cout.width(8);
    cout << y1 << "  ";  cout.width(8);  cout << z1 << endl;
  cout << "Point 2 : ";  cout.width(8);  cout << x2 << "  ";  cout.width(8);
    cout << y2 << "  ";  cout.width(8);  cout << z2 << endl;
  cout << "Point 3 : ";  cout.width(8);  cout << x3 << "  ";  cout.width(8);
    cout << y3 << "  ";  cout.width(8);  cout << z3 << endl;
  cout << "Point 4 : ";  cout.width(8);  cout << x4 << "  ";  cout.width(8);
    cout << y4 << "  ";  cout.width(8);  cout <<  z4 << endl;
  cout << "Point 5 : ";  cout.width(8);  cout << x5 << "  ";  cout.width(8);
    cout << y5 << "  ";  cout.width(8);  cout <<  z5 << endl;
  cout << "Point 6 : ";  cout.width(8);  cout << x6 << "  ";  cout.width(8);
    cout << y6 << "  ";  cout.width(8);  cout <<  z6 << endl;
  cout << "Point 7 : ";  cout.width(8);  cout << x7 << "  ";  cout.width(8);
    cout << y7 << "  ";  cout.width(8);  cout <<  z7 << endl;
  cout << endl;
//
//  Face 4 and 5, parts not covered neither by APD capsule nor by "ferule" have a
//wrapping which has to be considered as black, but with a slice of air. We change the
//wrapping of faces 4 and 5.
  TFace *face4, *face5;
  TSupplShape *ps = crystal->fSuppl;
  face4 = ps->GetFace(4);
  face5 = ps->GetFace(5);
  face4->ChangeRevetment("Revtopbot");
  face5->ChangeRevetment("Revtopbot");
//
//  We create a subface on face 5 of crystal, which is the portion of the face which is
//in contact with the capsule of the APDs, with a wrapping "Revcaps".
  const Double_t capsule_BR     = 2.3;  //width of the capsule, parallel to BR [B. Ille]
  const Double_t capsule_CR     = 2.2;  //width of the capsule, parallel to CR [B. Ille]
  const Double_t d_BR_caps_crys = 0.12; //distance from edge of capsule to edge BR [B. Ille]
  const Double_t d_CR_caps_crys = 0.12; //distance from edge of capsule to edge CR [B. Ille]
  Double_t x8,y8,x9,y9,x10,y10,x11,y11; //Points to define the rectangle of the capsule
  x8  = x4  + d_BR_caps_crys;
  y8  = y4  + d_CR_caps_crys;
  x9  = x8  + capsule_CR;
  y9  = y8;
  x11 = x8;
  y11 = y8  + capsule_BR;
  x10 = x11 + capsule_CR;
  y10 = y11;
  T3Vector *p8,*p9,*p10,*p11;
  p8  = new T3Vector(x8,y8,crys_dz);
  p9  = new T3Vector(x9,y9,crys_dz);
  p10 = new T3Vector(x10,y10,crys_dz);
  p11 = new T3Vector(x11,y11,crys_dz);
  cout << "rectangle of the capsule :" << endl;
  cout << "  p8 : "; p8->Print();
  cout << "  p9 : "; p9->Print();
  cout << "  p10: "; p10->Print();
  cout << "  p11: "; p11->Print();
  ps->AddSubPoint(p8);
  ps->AddSubPoint(p9);
  ps->AddSubPoint(p10);
  ps->AddSubPoint(p11);
  TFace *face6;
  face6 = new TFace(crystal,6,5,polygonal,ps->fPoints,ps->fGCenter,"Revcaps",
    8,9,10,11,kFALSE);
  ps->AddSubFace(face6);
//
//  We create a subface on face 4 of crystal, which is the portion of the face which is
//in contact with the "porte-ferule", the piece fixing the optical fibre, with a wrapping
//"Revferule".
  Double_t d_BF_fer_crys; //distance from edge of ferule to edge BF [Anfreville]
  Double_t d_CF_fer_crys; //distance from edge of ferule to edge CF [Anfreville]
  d_BF_fer_crys = (crys_AF - ferule_AF)/deux;
  d_CF_fer_crys = (crys_BF - ferule_BF)/deux;
  cout << "ferule_AF     : " << ferule_AF     << "   ferule_BF     : " << ferule_BF << endl;
  cout << "d_BF_fer_crys : " << d_BF_fer_crys << "   d_CF_fer_crys : " << d_CF_fer_crys << endl;
  Double_t x12,y12,x13,y13,x14,y14,x15,y15; //Points to define the rectangle of the ferule
  x12 = x0 + d_BF_fer_crys;
  y12 = y0 + d_CF_fer_crys;
  x13 = x12 + ferule_AF,
  y13 = y12;
  x15 = x12;
  y15 = y12 + ferule_BF;
  x14 = x15 + ferule_AF;
  y14 = y15;
  T3Vector *p12,*p13,*p14,*p15;
  p12 = new T3Vector(x12,y12,-crys_dz);
  p13 = new T3Vector(x13,y13,-crys_dz);
  p14 = new T3Vector(x14,y14,-crys_dz);
  p15 = new T3Vector(x15,y15,-crys_dz);
  cout << "rectangle of the ferule :" << endl;
  cout << "  p12: "; p12->Print();
  cout << "  p13: "; p13->Print();
  cout << "  p14: "; p14->Print();
  cout << "  p15: "; p15->Print();
  ps->AddSubPoint(p12);
  ps->AddSubPoint(p13);
  ps->AddSubPoint(p14);
  ps->AddSubPoint(p15);
  TFace *face7;
  face7 = new TFace(crystal,7,4,polygonal,ps->fPoints,ps->fGCenter,"Revferule",
                    12,13,14,15,kFALSE);
  ps->AddSubFace(face7);
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
  if (!isopbwo4) {
    if (kOptAxis==1) crystal->SetOpticalAxis(90.0,0.0,90.0,90.0);
    else             crystal->SetOpticalAxis(0.0,0.0,90.0,0.0);
  }
//crystal has bevellings (chamfers).
  const Double_t squareroot2 = TMath::Sqrt(deux);
  const Double_t sizechamfer = 0.05; // [M. Lebeau]
  Double_t edgechamfer = sizechamfer/squareroot2;
  crystal->fSuppl->SetBevellings(kTRUE,edgechamfer,"Revchamf");
//
// Radiation damages
//
  T3Vector start_casc(center_inter_x, center_inter_y, center_inter_z);
  T3Vector end_casc(center_exter_x, center_exter_y, center_exter_z);
  T3Vector axis_casc = end_casc - start_casc;
  axis_casc = axis_casc.Unit();
  if (verbosity >= 3) {
    cout << "  starting point of cascade :" << endl;
    cout << "  ";
    start_casc.Print();
    cout << "  end point of cascade:" << endl;
    cout << "  ";
    end_casc.Print();
    cout << "  axis of cascade:" << endl;
    cout << "  ";
    axis_casc.Print();
  }
  if (DamageIntensity>0) {
    ps->ShowerDamage(&axis_casc,&start_casc,DamageFile.Data(),DamageE0,kFALSE,0.2,4.0);
  }
//Positions of isosheet
  Double_t u0,u1,u2,u3,u4,u5,u6,u7;
  Double_t v0,v1,v2,v3,v4,v5,v6,v7,tx,ty;
  u0 = x1;
  v0 = y1;
  u1 = x1 + iso_dx;
  v1 = y1;
  u3 = x2;
  v3 = y2;
  u2 = x2 + iso_dx;
  v2 = y2;
  u4 = x5;
  v4 = y5;
  u5 = x5 + iso_dx;
  v5 = y5;
  u7 = x6;
  v7 = y6;
  u6 = x6 + iso_dx;
  v6 = y6;
  TSTRAP *isosheet;
  isosheet = new TSTRAP("isosheet","isosheet","PbWO4iso","Alveola",crys_dz,
     u0,u1,u2,u3,u4,u5,u6,u7,v0,v2,v4,v6,tx,ty);
//
//  Faces 4 and 5 have a different wrapping.
  TSupplShape *psiso = isosheet->fSuppl;
  face4 = psiso->GetFace(4);
  face5 = psiso->GetFace(5);
  face4->ChangeRevetment("Revtopbot");
  face5->ChangeRevetment("Revtopbot");
//isosheet has bevellings (chamfers).
  isosheet->fSuppl->SetBevellings(kTRUE,edgechamfer,"Revchamf",60.0);
//Getting the pointer to face 1 of isosheet, which is the depolished face.
  TFace *fdepol;
  fdepol = isosheet->fSuppl->GetFace(1);
  fdepol->SetDepolished(depolishangle);  //Setting a depolish
//
//We define the center of face 5, the point just between the 2 APDs.
  const Double_t apd_BR  = 0.25;       //half-width of APDs in direction || to BR [B. Ille]
  const Double_t apd_CR  = 0.25;       //half-width of APds in direction || to CR [B. Ille]
  const Double_t sep_APD = 0.21;       //half separation between the 2 APDs [B. Ille]
  const Double_t d_BR_APD_caps = 0.39; //distance from edge of APD1 to edge BR of capsule [B. Ille]
  const Double_t d_CR_APD_caps = 0.9;  //distance from edge of APDs to edge CR of capsule [B. Ille]
  Double_t center5_x;                  //x of "center" of face 5
  Double_t center5_y;                  //y of "center" of face 5
  center5_x = x4 + d_BR_caps_crys + d_BR_APD_caps + deux*apd_CR + sep_APD;
  center5_y = y4 + d_CR_caps_crys + d_CR_APD_caps + apd_BR;
  cout << "center  face 5 : (" << center5_x << ',' << center5_y << ')' << endl;
  const Double_t glue_dz = 0.0015;     //half width of glue between crystal and APD [Montecchi]
  const Double_t apd_dz  = 0.005;      //half width of silicium of APD
  Double_t apd1_x,apd1_y,apd1_z,apd2_x,apd2_y,apd2_z;
  apd1_x = center5_x - sep_APD - apd_CR;    //position x center of APD 1.
  apd1_y = center5_y;                       //position y center of APD 1.
  apd1_z = crys_dz + deux*glue_dz + apd_dz; //position z center of APD 1.
  apd2_x = center5_x + sep_APD + apd_CR;    //position y center of APD 2.
  apd2_y = center5_y;                       //position x center of APD 2.
  apd2_z = apd1_z;                          //position z center of APD 2.
  cout << "center of APD1 : (" << apd1_x << ',' << apd1_y << ',' << apd1_z << ')' << endl;
  cout << "center of APD2 : (" << apd2_x << ',' << apd2_y << ',' << apd2_z << ')' << endl;
  Double_t glue1_z = crys_dz + glue_dz;     //position z center of glue for APD1.
  Double_t glue2_z = glue1_z;               //position z center of glue for APD2.
//
//  Glue for APD1
  TSBRIK *glue1;
  glue1 = new TSBRIK("glue1","glue1","Glue","TotAbsorbing",apd_CR,apd_BR,glue_dz);
//  APD1
  TSBRIK *apd1;
  apd1 = new TSBRIK("apd1","apd1","Silicium","TotAbsorbing",apd_CR,apd_BR,apd_dz);
  apd1->fSuppl->SetAPD("hamamatsu1","hamamatsu1",4,5,"GainProfile_CMSAPD");
//  Glue for APD2
  TSBRIK *glue2;
  glue2 = new TSBRIK("glue2","glue2","Glue","TotAbsorbing",apd_CR,apd_BR,glue_dz);
//  APD2
  TSBRIK *apd2;
  apd2 = new TSBRIK("apd2","apd2","Silicium","TotAbsorbing",apd_CR,apd_BR,apd_dz);
  apd2->fSuppl->SetAPD("hamamatsu2","hamamatsu2",4,5,"GainProfile_CMSAPD");
//
//  Dimensions of air gap and fibre
  const Double_t airgap_dz = 0.015; //semi-height of air gap [Anfreville]
  const Double_t airgap_r  = 0.075; //radius of air gap [Anfreville]
  const Double_t fibre_dz  = 0.5;   //semi-height of fibre [irrelevant]
  const Double_t fibre_r   = 0.01;  //radius of fibre [Anfreville]
//
//  Position of air gap and fibre
  Double_t center4_x;  //position x of airgap and fibre
  Double_t center4_y;  //position y of airgap and fibre
  Double_t airgap_z;   //position z of center of airgap
  Double_t fibre_z;    //position z of center of fibre
  center4_x = x0 + d_BF_fer_crys + ferule_AF/deux;
  center4_y = y0 + d_CF_fer_crys + ferule_BF/deux;
  airgap_z  = - (crys_dz + airgap_dz);
  fibre_z   = airgap_z - (airgap_dz + fibre_dz);
  cout << "center4_x : " << center4_x << "   center4_y : " << center4_y << endl;
  cout << "airgap_z  : " << airgap_z  << "   fibre_z   : " << fibre_z   << endl;
//  Airgap
  TSCYL *airgap;
  airgap = new TSCYL("AirGap","AirGap","Air","Revferule",airgap_r,airgap_dz);
//  Fibre
  TSCYL *fibre;
  fibre = new TSCYL("Fibre","Fibre","HCP200","TotAbsorbing",fibre_r,fibre_dz);
//use different colors
  crystal->SetLineColor(106);
  isosheet->SetLineColor(146);
  glue1->SetLineColor(102);
  glue2->SetLineColor(102);
  apd1->SetLineColor(104);
  apd2->SetLineColor(104);
  airgap->SetLineColor(149);
  fibre->SetLineColor(146);
  crystal->SetLineWidth(2);
  isosheet->SetLineWidth(2);
  glue1->SetLineWidth(2);
  glue2->SetLineWidth(2);
  apd1->SetLineWidth(2);
  apd2->SetLineWidth(2);
  airgap->SetLineWidth(2);
  fibre->SetLineWidth(2);
  TSNode *node1 = new TSNode("node1","node1",crystal);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);
  node1->cd();
  TSNode *node2 = new TSNode("node2","node2",isosheet,-tx,-ty,zero);
  TSNode *node3 = new TSNode("node3","node3",glue1,apd1_x,apd1_y,glue1_z);
  TSNode *node4 = new TSNode("node4","node4",apd1, apd1_x,apd1_y,apd1_z);
  TSNode *node5 = new TSNode("node5","node5",glue2,apd2_x,apd2_y,glue2_z);
  TSNode *node6 = new TSNode("node6","node6",apd2, apd2_x,apd2_y,apd2_z);
  TSNode *node7 = new TSNode("node7","node7",airgap,center4_x,center4_y,airgap_z);
  TSNode *node8 = new TSNode("node8","node8",fibre, center4_x,center4_y,fibre_z);
  TContact *contactcs,*contactcg1,*contactga1,*contactcg2,*contactga2,*
           *contactca,*contactaf;
  contactcs  = new TContact("crystal_iso","crystal_iso","crystal","isosheet",identical);
  contactcg1 = new TContact("crystal_glue1","crystal_glue1","crystal","glue1",containing);
  contactga1 = new TContact("glue1_apd1","glue1_apd1","glue1","apd1",identical);
  contactcg2 = new TContact("crystal_glue2","crystal_glue2","crystal","glue2",containing);
  contactga2 = new TContact("glue2_apd2","glue2_apd2","glue2","apd2",identical);
  contactca  = new TContact("crystal_air","crystal_air","crystal","AirGap",containing);
  contactaf  = new TContact("air_fibre","air_fibre","AirGap","Fibre",containing);
}
