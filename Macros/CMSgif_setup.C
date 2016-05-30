{
//
//  GIF_setup_iso : issu de cms_setup de Xavier Gentit
//
//  setup_gif_iso_0.C is a description of the final setup for a crystal of the gif experiment.
//The material PbWO4 of the crystal is treated as isotropic 
//with index niso = sqr ((2nord^2 + next^2)/3). 
//On the upper face of
//the PbWO4 crystal is the APD capsule which requires this upper face to be divided into
//face 5, the part not covered by the APD capsule, and subface 6, the part covered by the
//APD capsule. On this subface 6 are 2 rectangular APDs of 25 mm2. The capsule and the
//APDs are glued onto face 6 with the same glue. On the lower face of the PbWO4 crystal
//is the so called "ferule", a piece of white PBT containing the optical fibre. The
//optical fibre is not in contact with the crystal, there is a cylindrical air gap
//in-between.
//So there are 8 shapes in this setup :
// - (a) a TSTRAP (CMS crystal made of isotropic PbWO4)
// - (c) 1 TSBRIK (glue of the APDs)
// - (d) 1 TSBRIK (APD made of silicium).
// - (e) 1 TSCYL  (air gap between optical fibre and crystal)
// - (f) 1 TSCYL  (the optical fibre)
//  PbWO4 is the only material which is fluorescent, i.e. able to emit light when
//crossed by particles. The other materials are air, glue, silicium and silice/polymere
//hcp200 of the fibre.
//  The CMS crystal is put inside tyvek. Between the crystal and the tyvek is a slice of air.
//  The TSBRIKs made of glue and the TSBRIKs made of silicium are considered to be wrapped
//with a totally absorbing wrapping which is called "totabsorbing". Between the glue and
//the silicium of the APD, there is a thin slice of nitrure which is the window of the
//APD. This is a true thin slice in the optical meaning of the word, i.e. a slice which
//is so thin that its thickness is of the order of the wavelengths, so that the normal
//Fresnel formulae do not apply. Litrani knows how to handle optical thin slices.
//
//  BEGIN OF setup_gif_iso0.C
//
  const Double_t radlength = 0.893; //radiation length of PbWO4 in cm
  const Double_t rMoliere  = 2.0;   //Moliere radius of PbWO4 in cm
//
//  Definition of the material PbWO4. The magnetic permeability is declared to be 1, not
//depending upon wavelength. PbWO4 is declared as isotropic, the index being given by a 
//Sellmeir law with the following parameters extracted form the expressions of the ordinary 
//and extraordinary indices :
//ns0 = sqr ((2 * ns0ord^2 + ns0ext^2) / 3)
//ns1 = sqr ((2 * ns1ord^2 + nsxext^2) / 3)
//ls1 = (ls1ord + ls1ext) / 2
//
//The absorption length is taken as the one of crystal 8981.
//  PbWO4 is declared fluorescent, with one component at 420 nm and another one at 500 nm.
//These 2 components are splitted into 3 possible decay times.
//  A call to dEdx indicates that PbWO4 is able to emit light when crossed
//by particles.
  TOpticMaterial *pbwo4;
  pbwo4 = new TOpticMaterial("PbWO4","PbWO4 crystal",kFALSE,un,"AbsorptionLength_crys8981R01");
  const Double_t ns0 = 1.61744;   //index 0 for the Sellmeier law for PbWO4
  const Double_t ns1 = 1.01999;   //index 1 for the Sellmeier law for PbWO4
  const Double_t ls1 = 269.053;   //1st resonant wavelength for the Sellmeier law for PbWO4
  pbwo4->IsIsotropic(1,ns0,ns1,ls1);
//  pbwo4->FluoComponent(420.0,40.0,0.2925,5.0);
//  pbwo4->FluoComponent(420.0,40.0,0.4500,15.0);
//  pbwo4->FluoComponent(420.0,40.0,0.0075,100.0);
//  pbwo4->FluoComponent(500.0,50.0,0.0975,5.0);
//  pbwo4->FluoComponent(500.0,50.0,0.1500,15.0);
//  pbwo4->FluoComponent(500.0,50.0,0.0025,100.0);
  TLitSpectrum *sp;
  sp = new TLitSpectrum("Spectrum_PbWO4g2","Emission Spectrum | PbWO4 old spectrum with 2 gaussians");
  sp->AddOneComponent(420.0,40.0,0.75,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->AddOneComponent(500.0,50.0,0.25,5.0,-1.0,0.39,15.0,-1.0,0.6,100.0,-1.0,0.01);
  sp->Normalize();
  pbwo4->SetLitSpectrum(sp);
  pbwo4->SetCascades(radlength,rMoliere); //only useful when TElecCascade used
  pbwo4->dEdx("DEdx_MusInPbWO4PDB",100.0,kFALSE);
//
//  Definition of air, with magnetic permeability of 1 and absorption length
//of 10000 cm, independent of wavelength. Air is also isotropic, with index
//of refraction of 1.0 independent of wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,un,10000.0);
  air->IsIsotropic(1.0003);
//
//  Definition of glue, with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsorptionLength_Meltmount1704". Glue is isotropic with
//index of refraction depending  upon wavelength according to the fit "RefrIndex_RTV3145".
  TOpticMaterial *glue;
  glue = new TOpticMaterial("Glue","Glue",kFALSE,un,"AbsorptionLength_Meltmount1704");
  glue->IsIsotropic("RefrIndex_RTV3145");
//
//  Definition of grease, with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsorptionLength_Q23067". Grease is isotropic with
//index of refraction depending  upon wavelength according to the fit "RefrIndex_Q23067".
  TOpticMaterial *grease;
  grease = new TOpticMaterial("Grease","Grease",kFALSE,un,"AbsorptionLength_Q23067");
  grease->IsIsotropic("RefrIndex_Q23067");
//
//  Definition of silicium, defined as sensible ( i.e. if a photon dies inside, it
//is considered as detected), with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsorptionLength_Silicium". Silicium is isotropic
//with index of refraction depending  upon wavelength according to the fit
//"RefrIndex_Silicium".
  TOpticMaterial *silicium;
  silicium = new TOpticMaterial("Silicium","Silicium of APD",kTRUE,un,"AbsorptionLength_Silicium");
  silicium->IsIsotropic("RefrIndex_Silicium");
//
//  Definition of the material of the fibre, généric
  TOpticMaterial *hcp200;
  hcp200 = new TOpticMaterial("HCP200","HCP200",kFALSE,un,zero);
  hcp200->IsIsotropic("RefrIndex_hcp200");
//  Definition of the material of the fibre, led emission
  TOpticMaterial *hcp200led;
  hcp200led = new TOpticMaterial("HCP200led","HCP200led",kFALSE,un,zero);
  hcp200led->IsIsotropic("RefrIndex_hcp200");
//  hcp200led->FluoComponent(450.0,39.7,1.0,0.1);
//  hcp200led->FluoComponent(465.0,18.5,1.0,0.1); //LED 465 nm spectrum approx.
  TLitSpectrum *spled;
  spled = new TLitSpectrum("Spectrum_hpc200led","Emission Spectrum | hpc200led");
  spled->AddOneComponent(465.0,18.5,1.0,0.1,-1.0,1.0);
  spled->Normalize();
  hcp200led->SetLitSpectrum(spled);
//
//  Definition of the material of the mixer
  TOpticMaterial *silica;
  silica = new TOpticMaterial("Silica","Silica",kFALSE,un,10000.0);
  const Double_t ns0_sio2 = 1.00000;   //index 0 for the Sellmeier law for silica
  const Double_t ns1_sio2 = 0.8343658; //index 1 for the Sellmeier law for silica
  const Double_t ls1_sio2 = 68.4043;   //1st resonant wavelength for the Sellmeier law
  const Double_t ns2_sio2 = 0.6387038; //index 2 for the Sellmeier law for silica
  const Double_t ls2_sio2 = 116.2414;  //2d resonant wavelength for the Sellmeier law
  const Double_t ns3_sio2 = 0.9473539; //index 31 for the Sellmeier law for silica
  const Double_t ls3_sio2 = 9896.161;  //3d resonant wavelength for the Sellmeier law 
  silica->IsIsotropic(3,ns0_sio2,ns1_sio2,ls1_sio2,ns2_sio2,ls2_sio2,ns3_sio2,ls3_sio2);
//
//  Definition of the thin slice of nitrure being the window of the APD. It has a
//thickness of 125 nm and has an index of refraction varying with wavelength accor-
//ding to the fit "RefrNitrure". Look on the web at the description of class
//ThinSlice for more details.
  ThinSlice *tsAPD;
  tsAPD = new ThinSlice("WindowAPD","WindowAPD",125.0,"RefrIndex_Nitrure");
//
//  Definition of the wrapping tyvek, with a slice of air inbetween ( a slice, not a
//thin slice ) with real part and imaginary part of index of refraction varying in
//function of wavelength according to the fits "Re_n_Tyvec" and "Im_n_Tyvec".
//The magnetic permeability is 1, independent of wavelength. There is no supplementary
//absorption. The wrapping is 99.89 % diffusing. 
//(Mean absorption 12.5%, mean diffusion 87.4%, mean specular 0.1%, 
//cf. M. Montecchi, 15 Feb. 2000)
  const Double_t PropDiffus = 874.0/875.0;
  const Double_t AbsTyvek = 0.125;
  TRevetment *tyvek;
  tyvek = new TRevetment("Tyvek","Tyvek","Air",PropDiffus,"RIndexRev_Tyvek","IIndexRev_Tyvek",un);
//
  TRevetment *pseudotyvek;
  pseudotyvek = new TRevetment("PseudoTyvek","PseudoTyvek","Air",PropDiffus,
                zero,minus_un,un,AbsTyvek);
//
//  Definition of a perfect reflector
  TRevetment *perfectreflector;
  perfectreflector = new TRevetment("PerfectReflector","PerfectReflector","Air",zero,
                     zero,minus_un,un,zero);
//
//  Definition of a perfect diffusor
  TRevetment *perfectdiffusor;
  perfectdiffusor = new TRevetment("PerfectDiffusor", "PerfectDiffusor","Air",un,
                    zero,minus_un ,un,zero);
//
//  Definition of the wrapping "Revchamf", without slice of air inbetween, with real
//part and imaginary part of index of refraction varying in function of wavelength
//according to the fits "RIndexRev_Tyvek" and "IIndexRev_Tyvek". The magnetic permeability
//is 1, independent of wavelength. There is a supplementary absorption of 0.05. The
//wrapping is 0% reflecting, 100% diffusing, to simulate a depolish. The maximum angle
//theta for a diffused photon is 70 degree.
  TRevetment *revchamf;
  revchamf = new TRevetment("Revchamf","Revchamf","none",un,"RIndexRev_Tyvek",
             "IIndexRev_Tyvek",un,0.05,70.0);
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
//supplementary absorption of 0.999999. But we have not put a supplementary absorption of 1.0,
//since we insist that there is a slice of air and that total reflection is possible.
  TRevetment *revtopbot;
  revtopbot  = new TRevetment("Revtopbot","Revtopbot","Air",un,"RIndexRev_Tyvek",
               "IIndexRev_Tyvek",un,0.999999,80.0);
//
//  Definition of the wrapping "TotAbsorbing", giving only the 1st 2 arguments. When the
//other arguments 3 to 8 are not given, following default values are assumed :
//
// (3) - nameMat  = "none" : no slice material between face and wrapping.
// (4) - diffus = 0.0    : Reflecting, not diffusing. Irrelevant because of (8).
// (5) - Rindex = 2.0    : Real part of index. Irrelevant because of (8).
// (6) - Iindex = 6.0    : Imaginary part of Index. Irrelevant because of (8).
// (7) - Mu = 1.0    : Magnetic permeability. Irrelevant because of (8).
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
//    z0 = z1 = z2 = z3 = - crys_dz
//    z4 = z5 = z6 = z7 = + crys_dz
//  The 8 points of the TSTRAP representing the crystal. The crystal is then centered
//in the sense that we impose:
//    x0+x1+x2+x3+x4+x5+x6+x7 = 0
//    y0+y1+y2+y3+y4+y5+y6+y7 = 0
//  The center of the crystal is then (0,0,0)
//
  const Double_t iso_dx        = 0.002; //width in y of sheet of isotropic PbWO4.
  const Double_t depolishangle = 42.0;  //max depolish angle of face 2 of isosheet
  const Double_t ferule_AF     = 0.4;   //2.18;  //dimension ||AF of "porte-ferule"
  Double_t x0,x1,x2,x3,x4,x5,x6,x7;     //x coord of 8 points of crystal
  Double_t y0,y1,y2,y3,y4,y5,y6,y7;     //y coord of 8 points of crystal
  Double_t z0,z1,z2,z3,z4,z5,z6,z7;     //z coord of 8 points of crystal
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
//
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
//  Int_t typerevcrystal = 4; //1:Tyvek,2:TotAbsorbing,3:PerfectDiffusor,4:PerfectReflector,5:PseudoTyvek
  TSTRAP *crystal;
  switch (typerevcrystal) {
    case 1:
      crystal = new TSTRAP("Crystal","Crystal","PbWO4","Tyvek",crystaltype,iso_dx);
    break;
    case 2:
      crystal = new TSTRAP("Crystal","Crystal","PbWO4","TotAbsorbing",crystaltype,iso_dx);
    break;
    case 3:
      crystal = new TSTRAP("Crystal","Crystal","PbWO4","PerfectDiffusor",crystaltype,iso_dx);
    break;
    case 4:
      crystal = new TSTRAP("Crystal","Crystal","PbWO4","PerfectReflector",crystaltype,iso_dx);
    break;
    case 5:
      crystal = new TSTRAP("Crystal","Crystal","PbWO4","PseudoTyvek",crystaltype,iso_dx);
    break;
    case 6:
      crystal = new TSTRAP("Crystal","Crystal","PbWO4","none",crystaltype,iso_dx);
    break;
  }
//
  crys_dz = crystal->Getdz();
  crystal->GetCMSdim(crystaltype,ferule_BF,crys_AFl,crys_BF,crys_CFl,crys_ARl,crys_BR,
           crys_CRl,iso_dx);
  crys_AF = crys_AFl + iso_dx;
  crys_CF = crys_CFl + iso_dx;
  crys_AR = crys_ARl + iso_dx;
  crys_CR = crys_CRl + iso_dx;
  ferule_BF = 0.4;

  cout << endl;
  cout << "    Dimensions of crystal" << endl;
  cout << endl;
  cout << "half height : " << crys_dz << endl;
  cout << "AF : " << crys_AF << "   BF : " << crys_BF << "   CF : " << crys_CF << endl;
  cout << "AR : " << crys_AR << "   BR : " << crys_BR << "   CR : " << crys_CR << endl;
  crystal->GetPoints(x0,y0,z0,x1,y1,z1,x2,y2,z2,x3,y3,z3,
                     x4,y4,z4,x5,y5,z5,x6,y6,z6,x7,y7,z7);
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
//Getting the pointer to face 1 of crystal, which is the depolished face.
  TFace *fdepol;
  fdepol = crystal->fSuppl->GetFace(1);
  fdepol->SetDepolished(depolishangle);  //Setting a depolish
//
//crystal has bevellings (chamfers).
  const Double_t squareroot2 = TMath::Sqrt(deux);
  const Double_t sizechamfer = 0.05; // [M. Lebeau]
  Double_t edgechamfer = sizechamfer/squareroot2;
  crystal->fSuppl->SetBevellings(kTRUE,edgechamfer,"Revchamf");
//
//We define the center of face 5.
  const Double_t apd_BR = 0.25;       //half-width of APDs in direction || to BR [B. Ille]
  const Double_t apd_CR = 0.25;       //half-width of APds in direction || to CR [B. Ille]
  Double_t center5_x = ( x4 + x5 + x6 + x7) / 4.0;       //x of "center" of face 5
  Double_t center5_y = ( y4 + y5 + y6 + y7) / 4.0;       //y of "center" of face 5
  cout << "center  face 5 : (" << center5_x << ',' << center5_y << ')' << endl;
  const Double_t grease_dz = 0.0015;  //half width of glue between crystal and APD [Montecchi]
  const Double_t apd_dz = 0.005;      //half width of silicium of APD
  Double_t apd_x,apd_y,apd_z;         //position x and y centers of APD.
  apd_x = center5_x ;
  apd_y = center5_y ;
  apd_z = crys_dz + deux*grease_dz + apd_dz;   //position z center of APD.
  cout << "center of APD : (" << apd_x << ',' << apd_y << ',' << apd_z << ')' << endl;
  Double_t grease_z = crys_dz + grease_dz;     //position z center of grease for APD1.
//
//  Grease for APD
  TSBRIK *grease1;
  grease1 = new TSBRIK("grease1","grease1","Grease","TotAbsorbing",apd_CR,apd_BR,grease_dz);
//
//  APD
  TSBRIK *apd;
  apd = new TSBRIK("Apd","Apd","Silicium","TotAbsorbing",apd_CR,apd_BR,apd_dz);
  apd->fSuppl->SetAPD("hamamatsu1","hamamatsu1",4,5,"GainProfile_CMSAPD");
//
//  Dimensions of air gap of mixer and mixer
  const Double_t airgapmixer_dz = 0.015; //semi-height of air gap
  const Double_t airgapmixer_r  = 0.96; //radius of air gap
  const Double_t mixer_dz  = 0.5;   //semi-height of mixer
  const Double_t mixer_r   = 0.95;  //radius of mixer
  const Double_t mixer_dm = 0.5;  //semi-length of mixer "meplat"
  const Int_t mixer_nsa = 2;
  const Int_t mixer_nsb = 1;
  Double_t mixer_dx, mixer_x1, mixer_x2, mixer_x3, mixer_x4, mixer_x5, mixer_x6, mixer_x7;
  Double_t mixer_x8, mixer_x9, mixer_x10, mixer_x11, mixer_x12, mixer_x13, mixer_x14;
  Double_t mixer_dy, mixer_y1, mixer_y2, mixer_y3, mixer_y4, mixer_y5, mixer_y6, mixer_y7;
  Double_t mixer_y8, mixer_y9, mixer_y10, mixer_y11, mixer_y12, mixer_y13, mixer_y14;
  Double_t mixer_theta1 = asin(mixer_dm/mixer_r);
  Double_t mixer_dthetaa = ( pi - 2.0 * mixer_theta1 ) / ( 2 * mixer_nsa + 1);
  Double_t mixer_dthetab =  2.0 * mixer_theta1 / ( 2 * mixer_nsb + 1);
  mixer_dx = mixer_r * cos(mixer_theta1);
  mixer_dy = mixer_dm;
  mixer_x1 = mixer_dx;
  mixer_y1 = mixer_dy;
  mixer_x2 = mixer_r * cos(mixer_theta1 + mixer_dthetaa);
  mixer_y2 = mixer_r * sin(mixer_theta1 + mixer_dthetaa);
  mixer_x3 = mixer_r * cos(mixer_theta1 + 2.0 * mixer_dthetaa);
  mixer_y3 = mixer_r * sin(mixer_theta1 + 2.0 * mixer_dthetaa);
  mixer_x4 = - mixer_x3;
  mixer_y4 = mixer_y3;
  mixer_x5 = - mixer_x2;
  mixer_y5 = mixer_y2;
  mixer_x6 = - mixer_x1;
  mixer_y6 = mixer_y1;
  mixer_x7 = - mixer_r * cos(mixer_theta1 - mixer_dthetab);
  mixer_y7 = mixer_r * sin(mixer_theta1 - mixer_dthetab);
  mixer_x8 = mixer_x7;
  mixer_y8 = - mixer_y7;
  mixer_x9 = mixer_x6;
  mixer_y9 = - mixer_y6;
  mixer_x10 = mixer_x5;
  mixer_y10 = - mixer_y5;
  mixer_x11 = mixer_x4;
  mixer_y11 = - mixer_y4;
  mixer_x12 = mixer_x3;
  mixer_y12 = - mixer_y3;
  mixer_x13 = mixer_x2;
  mixer_y13 = - mixer_y2;
  mixer_x14 = mixer_x1;
  mixer_y14 = - mixer_y1;
//
  Double_t d_BF_mixer_crys = crys_AF - 1.59; //distance from center of mixer to edge BF
  Double_t d_CF_mixer_crys = crys_BF - 1.05; //distance from edge of mixer to edge CF
  Double_t mixer_x;                          //position x of mixer center
  Double_t mixer_y;                          //position y of mixer center
  Double_t mixer_z;                          //position z of of mixer center
  Double_t airgapmixer_x;                    //position x of airgapmixer center
  Double_t airgapmixer_y;                    //position y of airgapmixer center
  Double_t airgapmixer_z;                    //position z of airgapmixer center
//
//  Positions of air gapmixer and mixer
  mixer_x = x0 + d_BF_mixer_crys;
  mixer_y = y0 + d_CF_mixer_crys;
//  mixer_x = ( x0 + x1 + x2 + x3 ) / 4.0;
//  mixer_y = ( y0 + y1 + y2 + y3 ) / 4.0;
  airgapmixer_x = mixer_x;
  airgapmixer_y = mixer_y;
  airgapmixer_z  = - crys_dz - airgapmixer_dz;
  mixer_z   = airgapmixer_z - airgapmixer_dz - mixer_dz;
  cout << "mixer_x : " << mixer_x << "   mixer_y : " << mixer_y;
  cout << "  airgapmixer_z  : " << airgapmixer_z  << "   mixer_z   : " << mixer_z   << endl;
//
//  Airgap mixer
  TSCYL *airgapmixer;
  airgapmixer = new TSCYL("AirGapMixer","AirGapMixer","Air","Tyvek",airgapmixer_r,airgapmixer_dz);
//
//  Mixer
//  TSCYL *mixer;
//  mixer = new TSCYL("Mixer","Mixer","Silica","Tyvek",mixer_r,mixer_dz);
  TSBRIK *mixer;
  mixer = new TSBRIK("Mixer","Mixer","Silica","Tyvek",mixer_dx,mixer_dy,mixer_dz);
  cout << "mixer_dx : " << mixer_dx << "   mixer_dy : " << mixer_dy << endl;
//
//  Mixerb1
  Double_t mixerb1_tx,mixerb1_ty;
  TSTRAP *mixerb1;
  mixerb1 = new TSTRAP("MixerB1","MixerB1","Mixer","Tyvek",mixer_dz,mixer_y9,mixer_y6,
            mixer_y7,mixer_y8,mixer_y9,mixer_y6,mixer_y7,mixer_y8,-mixer_x6,-mixer_x8,
            -mixer_x6,-mixer_x8,mixerb1_tx,mixerb1_ty,kTRUE);
  mixerb1_ty = ( mixer_y9 + mixer_y6 + mixer_y7 + mixer_y8) / 4.0;
  mixerb1_tx = - ( mixer_x6 + mixer_x8 ) / 2.0;  
  cout << "mixerB1_tx : " << mixerb1_tx << "   ty : " << mixerb1_ty << endl;
//
//  Mixera1
  Double_t mixera1_tx,mixera1_ty;
  TSTRAP *mixera1;
  mixera1 = new TSTRAP("MixerA1","MixerA1","Mixer","Tyvek",mixer_dz,mixer_x6,mixer_x1,
            mixer_x2,mixer_x5,mixer_x6,mixer_x1,mixer_x2,mixer_x5,mixer_y6,mixer_y2,
            mixer_y6,mixer_y2,mixera1_tx,mixera1_ty,kTRUE);
  mixera1_tx = ( mixer_x6 + mixer_x1 + mixer_x2 + mixer_x5) / 4.0;
  mixera1_ty = ( mixer_y6 + mixer_y2 ) / 2.0;  
  cout << "mixerA1_tx : " << mixera1_tx << "   ty : " << mixera1_ty << endl;
//
//  Mixera2
  Double_t mixera2_tx,mixera2_ty;
  TSTRAP *mixera2;
  mixera2 = new TSTRAP("MixerA2","MixerA2","Mixer","Tyvek",mixer_dz,mixer_x5,mixer_x2,
            mixer_x3,mixer_x4,mixer_x5,mixer_x2,mixer_x3,mixer_x4,mixer_y5,mixer_y3,
            mixer_y5,mixer_y3,mixera2_tx,mixera2_ty,kTRUE);
  mixera2_tx = ( mixer_x5 + mixer_x2 + mixer_x3 + mixer_x4) / 4.0;
  mixera2_ty = ( mixer_y5 + mixer_y3 ) / 2.0;  
  cout << "mixerA2_tx : " << mixera2_tx << "   ty : " << mixera2_ty << endl;
//
//  Mixerc1
  Double_t mixerc1_tx,mixerc1_ty;
  TSTRAP *mixerc1;
  mixerc1 = new TSTRAP("MixerC1","MixerC1","Mixer","Tyvek",mixer_dz,mixer_x14,mixer_x9,
            mixer_x10,mixer_x13,mixer_x14,mixer_x9,mixer_x10,mixer_x13,mixer_y14,
            mixer_y10,mixer_y14,mixer_y10,mixerc1_tx,mixerc1_ty,kTRUE);
  mixerc1_tx = ( mixer_x14 + mixer_x9 + mixer_x10 + mixer_x13 ) / 4.0;
  mixerc1_ty = ( mixer_y10 + mixer_y14 ) / 2.0;  
  cout << "mixerC1_tx : " << mixerc1_tx << "   ty : " << mixerc1_ty << endl;
//
//  Mixerc2
  Double_t mixerc2_tx,mixerc2_ty;
  TSTRAP *mixerc2;
  mixerc2 = new TSTRAP("MixerC2","MixerC2","Mixer","Tyvek",mixer_dz,mixer_x13,mixer_x10,
            mixer_x11,mixer_x12,mixer_x13,mixer_x10,mixer_x11,mixer_x12,mixer_y13,
            mixer_y11,mixer_y13,mixer_y11,mixerc2_tx,mixerc2_ty,kTRUE);
  mixerc2_tx = ( mixer_x13 + mixer_x10 + mixer_x11 + mixer_x12 ) / 4.0;
  mixerc2_ty = ( mixer_y11 + mixer_y13 ) / 2.0;  
  cout << "mixerC2_tx : " << mixerc2_tx << "   ty : " << mixerc2_ty << endl;
//
//  Dimensions of air gapfibre1 and fibre1
  const Double_t airgapfibre1_dz = 0.015; //semi-height of air gap [Anfreville]
  const Double_t airgapfibre1_r  = 0.075; //radius of air gap [Anfreville]
  const Double_t fibre1_dz  = 0.5;   //semi-height of fibre [irrelevant]
  const Double_t fibre1_r   = 0.01;  //radius of fibre [Anfreville]
//
//  Position of air gapfibre1 and fibre1
  Double_t centerfibre1_x;  //position x of airgap and fibre
  Double_t centerfibre1_y;  //position y of airgap and fibre
  Double_t airgapfibre1_z;   //position z of center of airgap
  Double_t d_BF_fibre1_crys; //distance from fibre1 to edge BF
  Double_t d_CF_fibre1_crys; //distance from fibre1 to edge CF
  Double_t fibre1_z;    //position z of center of fibre
  d_BF_fibre1_crys = crys_AF - 1.10;
  d_CF_fibre1_crys = crys_BF - 0.45;
  centerfibre1_x = x0 + d_BF_fibre1_crys;
  centerfibre1_y = y0 + d_CF_fibre1_crys;
  airgapfibre1_z  = mixer_z - mixer_dz - airgapfibre1_dz;
  fibre1_z   = airgapfibre1_z - airgapfibre1_dz - fibre1_dz;
  cout << "centerfibre1_x : " << centerfibre1_x << "   centerfibre1_y : " << centerfibre1_y;
  cout << "  airgapfibre1_z  : " << airgapfibre1_z << "   fibre1_z   : " << fibre1_z   << 
       endl;
//  Airgap fibre 1
  TSCYL *airgapfibre1;
  airgapfibre1 = new TSCYL("AirGapfibre1","AirGapfibre1","Air","TotAbsorbing",airgapfibre1_r,
                 airgapfibre1_dz);
//  Fibre
  TSCYL *fibre1;
  fibre1 = new TSCYL("Fibre1","Fibre1","HCP200led","TotAbsorbing",fibre1_r,fibre1_dz);
//
//  Dimensions of air gapfibre2 and fibre2
  const Double_t airgapfibre2_dz = 0.015; //semi-height of air gap [Anfreville]
  const Double_t airgapfibre2_r  = 0.075; //radius of air gap [Anfreville]
  const Double_t fibre2_dz  = 0.5;        //semi-height of fibre [irrelevant]
  const Double_t fibre2_r   = 0.01;       //radius of fibre [Anfreville]
//
//  Position of air gapfibre2 and fibre2
  Double_t centerfibre2_x;                //position x of airgap and fibre
  Double_t centerfibre2_y;                //position y of airgap and fibre
  Double_t airgapfibre2_z;                //position z of center of airgap
  Double_t d_BF_fibre2_crys;              //distance from fibre2 to edge BF
  Double_t d_CF_fibre2_crys;              //distance from fibre2 to edge CF
  Double_t fibre2_z;                      //position z of center of fibre
  d_BF_fibre2_crys = crys_AF - 1.10;
  d_CF_fibre2_crys = crys_BF - 1.05;
  centerfibre2_x = x0 + d_BF_fibre2_crys;
  centerfibre2_y = y0 + d_CF_fibre2_crys;
  airgapfibre2_z  = mixer_z - mixer_dz - airgapfibre2_dz;
  fibre2_z   = airgapfibre2_z - airgapfibre2_dz - fibre2_dz;
  cout << "centerfibre2_x : " << centerfibre2_x << "   centerfibre2_y : " << centerfibre2_y;
  cout << "  airgapfibre2_z  : " << airgapfibre2_z  << "   fibre2_z   : " << fibre2_z   << 
       endl;
//  Airgap
  TSCYL *airgapfibre2;
  airgapfibre2 = new TSCYL("AirGapfibre2","AirGapfibre2","Air","TotAbsorbing",airgapfibre2_r,
                 airgapfibre2_dz);
//  Fibre
  TSCYL *fibre2;
  fibre2 = new TSCYL("Fibre2","Fibre2","HCP200","TotAbsorbing",fibre2_r,fibre2_dz);
//
//  Dimensions of air gapfibre3 and fibre3
  const Double_t airgapfibre3_dz = 0.015; //semi-height of air gap [Anfreville]
  const Double_t airgapfibre3_r  = 0.075; //radius of air gap [Anfreville]
  const Double_t fibre3_dz  = 0.5;        //semi-height of fibre [irrelevant]
  const Double_t fibre3_r   = 0.01;       //radius of fibre [Anfreville]
//
//  Position of air gapfibre3 and fibre3
  Double_t centerfibre3_x;                //position x of airgap and fibre
  Double_t centerfibre3_y;                //position y of airgap and fibre
  Double_t airgapfibre3_z;                //position z of center of airgap
  Double_t d_BF_fibre3_crys;              //distance from fibre3 to edge BF
  Double_t d_CF_fibre3_crys;              //distance from fibre3 to edge CF
  Double_t fibre3_z;                      //position z of center of fibre
  d_BF_fibre3_crys = crys_AF - 1.10;
  d_CF_fibre3_crys = crys_BF - 1.65;
  centerfibre3_x = x0 + d_BF_fibre3_crys;
  centerfibre3_y = y0 + d_CF_fibre3_crys;
  airgapfibre3_z  = mixer_z - mixer_dz - airgapfibre3_dz;
  fibre3_z   = airgapfibre3_z - airgapfibre3_dz - fibre3_dz;
  cout << "centerfibre3_x : " << centerfibre3_x << "   centerfibre3_y : " << centerfibre3_y;


  cout << "  airgapfibre3_z  : " << airgapfibre3_z  << "   fibre3_z   : " << fibre3_z   << 
       endl;
//  Airgap
  TSCYL *airgapfibre3;
  airgapfibre3 = new TSCYL("AirGapfibre3","AirGapfibre3","Air","TotAbsorbing",airgapfibre3_r,
                 airgapfibre3_dz);
//  Fibre
  TSCYL *fibre3;
  fibre3 = new TSCYL("Fibre3","Fibre3","HCP200","TotAbsorbing",fibre3_r,fibre3_dz);
//
//use different colors
  crystal->SetLineColor(106);
  crystal->SetLineWidth(2);
  grease1->SetLineColor(102);
  grease1->SetLineWidth(2);
  apd->SetLineWidth(2);
  apd->SetLineColor(104);
  airgapmixer->SetLineColor(150);
  airgapmixer->SetLineWidth(2);
  mixer->SetLineColor(147);
  mixer->SetLineWidth(2);
  mixera1->SetLineColor(147);
  mixera1->SetLineWidth(2);
  mixerc1->SetLineColor(147);
  mixerc1->SetLineWidth(2);
  mixera2->SetLineColor(147);
  mixera2->SetLineWidth(2);
  mixerc2->SetLineColor(147);
  mixerc2->SetLineWidth(2);
  mixerb1->SetLineColor(147);
  mixerb1->SetLineWidth(2);
  airgapfibre1->SetLineColor(150);
  airgapfibre1->SetLineWidth(2);
  fibre1->SetLineColor(104);
  fibre1->SetLineWidth(2);
  airgapfibre2->SetLineColor(150);
  airgapfibre2->SetLineWidth(2);
  fibre2->SetLineColor(103);
  fibre2->SetLineWidth(2);
  airgapfibre3->SetLineColor(150);
  airgapfibre3->SetLineWidth(2);
  fibre3->SetLineColor(103);
  fibre3->SetLineWidth(2);

  TSNode *node1 = new TSNode("node1","node1",crystal);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);
  node1->cd();
  TSNode *node3 = new TSNode("node3","node3",grease1,apd_x,apd_y,grease_z);
  TSNode *node4 = new TSNode("node4","node4",apd, apd_x,apd_y,apd_z);
  TSNode *node5 = new TSNode("node5","node5",airgapmixer,airgapmixer_x,airgapmixer_y,
                  airgapmixer_z);
  TSNode *node6 = new TSNode("node6","node6",mixer,mixer_x,mixer_y,mixer_z);
  TSNode *node71 = new TSNode("node71","node71",airgapfibre1,centerfibre1_x,centerfibre1_y,
                   airgapfibre1_z);
  TSNode *node81 = new TSNode("node81","node81",fibre1, centerfibre1_x,centerfibre1_y,
                   fibre1_z);
  TSNode *node72 = new TSNode("node72","node72",airgapfibre2,centerfibre2_x,centerfibre2_y,
                   airgapfibre2_z);
  TSNode *node82 = new TSNode("node82","node82",fibre2, centerfibre2_x,centerfibre2_y,
                   fibre2_z);
  TSNode *node73 = new TSNode("node73","node73",airgapfibre3,centerfibre3_x,centerfibre3_y,
                   airgapfibre3_z);
  TSNode *node83 = new TSNode("node83","node83",fibre3, centerfibre3_x,centerfibre3_y,
                   fibre3_z);
  TSNode *node6a1 = new TSNode("node6a1","node6a1",mixera1,mixer_x+mixera1_tx,
                    mixer_y+mixera1_ty,mixer_z);
  TSNode *node6c1 = new TSNode("node6c1","node6c1",mixerc1,mixer_x+mixerc1_tx,
                    mixer_y+mixerc1_ty,mixer_z);
  TSNode *node6a2 = new TSNode("node6a2","node6a2",mixera2,mixer_x+mixera2_tx,
                    mixer_y+mixera2_ty,mixer_z);
  TSNode *node6c2 = new TSNode("node6c2","node6c2",mixerc2,mixer_x+mixerc2_tx,
                    mixer_y+mixerc2_ty,mixer_z);
  TRotMatrix *rot90degOz;
  rot90degOz = new TRotMatrix("Rot90degOz","Rot90degOz",90.0,-90.0,90.0,0.0,0.0,0.0);
  TSNode *node6b1 = new TSNode("node6b1","node6b1",mixerb1,mixer_x+mixerb1_tx,
                    mixer_y+mixerb1_ty,mixer_z,rot90degOz);
//
  TContact *contactcg1,*contactga1,*contactcam,*contactam,*contactma1,*contactaf1,
           *contactma2,*contactaf2,*contactma3,*contactaf3,*contactmia1,*contactcmia1,
           *contactmic1,*contactcmic1,*contactmia2,*contactcmia2,*contactmic2,
           *contactcmic2,*contactmib1,*contactcmib1;
  contactcg1 = new TContact("crystal_grease1","crystal_grease1","Crystal","grease1",
               containing);
  contactga1 = new TContact("grease1_apd","grease1_apd","grease1","Apd",identical);
  contactcam = new TContact("crystal_airgapmixer","crystal_airgapmixer","Crystal",
               "AirGapMixer",overlapping); 
  contactam = new TContact("airgap_mixer","airgap_mixer","AirGapMixer","Mixer",containing); 
  contactma1  = new TContact("mixer_airfibre1","mixer_airfibre1","MixerA1","AirGapfibre1",
                containing);
  contactaf1  = new TContact("air_fibre1","air_fibre1","AirGapfibre1","Fibre1",containing);
  contactma2  = new TContact("mixer_airfibre2","mixer_airfibre2","Mixer","AirGapfibre2",
                containing);
  contactaf2  = new TContact("air_fibre2","air_fibre2","AirGapfibre2","Fibre2",containing);
  contactma3  = new TContact("mixer_airfibre3","mixer_airfibre3","MixerC1","AirGapfibre3",
                containing);
  contactaf3  = new TContact("air_fibre3","air_fibre3","AirGapfibre3","Fibre3",containing);
  contactmia1 = new TContact("mixer_mixera1","mixer_mixera1","Mixer","MixerA1",identical);
  contactcmia1 = new TContact("airgapmixer_mixera1","airgapmixer_mixera1","AirGapMixer",
                 "MixerA1",containing);
  contactmic1 = new TContact("mixer_mixerc1","mixer_mixerc1","Mixer","MixerC1",identical);
  contactcmic1 = new TContact("airgapmixer_mixerc1","airgapmixer_mixerc1","AirGapMixer",
                 "MixerC1",containing);
  contactmia2 = new TContact("mixera1_mixera2","ixera1_mixera2","MixerA1","MixerA2",
                identical);
  contactcmia2 = new TContact("airgapmixer_mixera2","airgapmixer_mixera2","AirGapMixer",
                 "MixerA2",containing);
  contactmic2 = new TContact("mixerc1_mixerc2","mixerc1_mixerc2","MixerC1","MixerC2",
                identical);
  contactcmic2 = new TContact("airgapmixer_mixerc2","airgapmixer_mixerc2",
                 "AirGapMixer","MixerC2",containing);
  contactmib1 = new TContact("mixer_mixerb1","mixer_mixerb1","Mixer","MixerB1",identical);
  contactcmib1 = new TContact("airgapmixer_mixerb1","airgapmixer_mixerb1","AirGapMixer",
                 "MixerB1",containing);
//
}
