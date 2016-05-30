{
//
//  GIF_setup_iso : issu de cms_setup de Xavier Gentit
//  Version 2.0, 2004/04/01
//
//  Setup_gif_iso_0.C is a description of the final setup for a crystal of the gif experiment.
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
  const Double_t radlength = 0.893;	//	radiation length of PbWO4 in cm
  const Double_t rMoliere  = 2.0;	//	Moliere radius of PbWO4 in cm
  const Double_t refindex_air = 1.0003;	//	Refractive index of air
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
  Labsfile = Labsfilesprefix + "1";
  pbwo4 = new TOpticMaterial("PbWO4","PbWO4 crystal",kFALSE,un,Labsfile.Data());
  const Double_t ns0 = 1.61744;	//	index 0 for the Sellmeier law for PbWO4
  const Double_t ns1 = 1.01999;	//	index 1 for the Sellmeier law for PbWO4
  const Double_t ls1 = 269.053;	//	1st resonant wavelength for the Sellmeier law for PbWO4
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
  pbwo4->SetCascades(radlength,rMoliere);	//	only useful when TElecCascade used
  pbwo4->dEdx("DEdx_MusInPbWO4PDB",PhotMeV,kFALSE);
  TElecCascade::fZmaxLR = ZmaxLR;
//
//  Definition of air, with magnetic permeability of 1 and absorption length
//of 10000 cm, independent of wavelength. Air is also isotropic, with index
//of refraction of 1.0 independent of wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,un,10000.0);
  air->IsIsotropic(refindex_air);
//
//  Definition of glue, with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsorptionLength_Meltmount1704".
//Glue is isotropic with index of refraction depending  upon wavelength according
//to the fit "RefrIndex_RTV3145".
	TOpticMaterial *glue;
	glue = new TOpticMaterial("Glue","Glue",kFALSE,un,"AbsorptionLength_Meltmount1704");
	glue->IsIsotropic("RefrIndex_RTV3145");
//
//  Definition of grease, with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsLGrease". Grease is isotropic with
//index of refraction depending  upon wavelength according to the fit "Refr_I_Grease".
//	TOpticMaterial *grease;
//	grease = new TOpticMaterial("Grease","Grease",kFALSE,un,"AbsLGrease");
//	grease->IsIsotropic("RefrIndex_Q23067");
//
//  Definition of silicium, defined as sensible ( i.e. if a photon dies inside, it
//is considered as detected), with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsLSilicium". Silicium is isotropic
//with index of refraction depending  upon wavelength according to the fit
//"Refr_I_Silicium".
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
//	hcp200led->FluoComponent(450.0,39.7,1.0,0.1);
//	hcp200led->FluoComponent(465.0,18.5,1.0,0.1); //LED 465 nm spectrum approx.
        TLitSpectrum *spled;
        spled = new TLitSpectrum("Spectrum_hpc200led","Emission Spectrum | hpc200led");
        spled->AddOneComponent(465.0,18.5,1.0,0.1,-1.0,1.0);
        spled->Normalize();
        hcp200led->SetLitSpectrum(spled);
//
//  Definition of the material of the mixer
//	TOpticMaterial *silica;
//	silica = new TOpticMaterial("Silica","Silica",kFALSE,un,10000.0);
//	const Double_t ns0_sio2 = 1.00000;	//	index 0 for the Sellmeier law for silica
//	const Double_t ns1_sio2 = 0.8343658;	//	index 1 for the Sellmeier law for silica
//	const Double_t ls1_sio2 = 68.4043;	//	1st resonant wavelength for the Sellmeier law
//	const Double_t ns2_sio2 = 0.6387038;	//	index 2 for the Sellmeier law for silica
//	const Double_t ls2_sio2 = 116.2414;	//	2d resonant wavelength for the Sellmeier law
//	const Double_t ns3_sio2 = 0.9473539;	//	index 31 for the Sellmeier law for silica
//	const Double_t ls3_sio2 = 9896.161;	//	3d resonant wavelength for the Sellmeier law 
//	silica->IsIsotropic(3,ns0_sio2,ns1_sio2,ls1_sio2,ns2_sio2,ls2_sio2,ns3_sio2,ls3_sio2);
//
//  Definition of the thin slice of nitrure being the window of the APD. It has a
//thickness of 125 nm and has an index of refraction varying with wavelength accor-
//ding to the fit "RefrNitrure". Look on the web at the description of class
//ThinSlice for more details.
	ThinSlice *tsAPD;
	tsAPD = new ThinSlice("WindowAPD","WindowAPD",125.0,"RefrIndex_Nitrure");
//
//  Definition of alveola's equivallent wrapping
	const Double_t PropDiffusAlveola = 0.11738;
	TRevetment *alveola;
	alveola = new TRevetment("Alveola","Alveola", "Air", PropDiffusAlveola,
                                 "RIndexRev_CMSalveola", "IIndexRev_CMSalveola", un);

//  Definition of the wrapping tyvek, with a slice of air inbetween ( a slice, not a
//thin slice ) with real part and imaginary part of index of refraction varying in
//function of wavelength according to the fits "Re_n_Tyvec" and "Im_n_Tyvec".
//The magnetic permeability is 1, independent of wavelength. There is no supplementary
//absorption. The wrapping is 99.89 % diffusing. 
//(Mean absorption 12.5%, mean diffusion 87.4%, mean specular 0.1%, 
//cf. M. Montecchi, 15 Feb. 2000)
	const Double_t PropDiffusTyvek = 874.0/875.0;
	const Double_t AbsTyvek = 0.125;
	TRevetment *tyvek;
	tyvek = new TRevetment("Tyvek","Tyvek","Air",PropDiffusTyvek,
                               "RIndexRev_Tyvek","IIndexRev_Tyvek",un);
//
	TRevetment *pseudotyvek;
	pseudotyvek = new TRevetment("PseudoTyvek","PseudoTyvek","Air",PropDiffusTyvek,
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
//according to the fits "Re_n_Tyvek" and "Im_n_Tyvek". The magnetic permeability
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
	revcaps	= new TRevetment("Revcaps","Revcaps","Glue",un,"RIndexRev_CMScapsula",
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
//      crystal which is not covered with the APD capsule.
//    - of the bottom of the alveola, which is the wrapping of the part of face 4 of the
//crystal which is not covered with the "ferule".
//  The fact that we wrongly refer to "Tyvek" does not matter, since we have put a
//supplementary absorption of 0.999999. But we have not put a supplementary absorption of 1.0,
//since we insist that there is a slice of air and that total reflection is possible.
	TRevetment *revtopbot;
	revtopbot = new TRevetment("Revtopbot","Revtopbot","Air",un,"RIndexRev_Tyvek",
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
	const Double_t iso_dx = 0.0;
	const Double_t depolishangle = 42.0;	//	max depolish angle of face 2 of isosheet
	const Double_t ferule_AF = 2.18;	//	dimension ||AF of "porte-ferule"
	Double_t x0,x1,x2,x3,x4,x5,x6,x7;	//	x coord of 8 points of crystal
	Double_t y0,y1,y2,y3,y4,y5,y6,y7;	//	y coord of 8 points of crystal
	Double_t z0,z1,z2,z3,z4,z5,z6,z7;	//	z coord of 8 points of crystal
	Double_t crys_dz;	//	half length in z of crystal of PbWO4
	Double_t crys_AF;	//	side left,  small face |_ to OZ
	Double_t crys_BF;	//	side back,  small face |_ to OZ
	Double_t crys_CF;	//	side right, small face |_ to OZ
	Double_t crys_AR;	//	side left,  large face |_ to OZ
	Double_t crys_BR;	//	side back,  large face |_ to OZ
	Double_t crys_CR;	//	side right, large face |_ to OZ
	Double_t ferule_BF;	//	dimension ||BF of "porte-ferule"
	Double_t crys_AFl;	//	crys_AF - iso_dx;
	Double_t crys_CFl;	//	crys_CF - iso_dx;
	Double_t crys_ARl;	//	crys_AR - iso_dx;
	Double_t crys_CRl;	//	crys_CR - iso_dx;
//
// The crystal is assumed to have the shape of one of the types 1-17 of the cms crystals,
//which may be built with the Litrani shape TSTRAP. The long axis of the crystal is along
//Oz. The shape "crystal" is taken as the reference shape. So that the local coordinate
//system of the shape "crystal" is at the same time the world coordinate system WCS.
// Description of the CMS crystal by the 7 parameters AF, BF, CF, AR, BR, CR and crys_dz.
//Look at "The Electromagnetic Calorimeter Project, CMS TDR4" pages 53/54. We use the
//constructor of TSTRAP which is specialized for CMS crystal. It contains the values
//for AF,BF,CF,AR,BR,CR and ferule_BF for all 17 types of CMS crystals.
//Look at the description of TSTRAP.
//
	char *crystalrevetment;
	switch (typerevcrystal) {
		case 1:
			crystalrevetment = "Tyvek";
		break;
		case 2:
			crystalrevetment = "TotAbsorbing";
		break;
		case 3:
			crystalrevetment = "PerfectDiffusor";
		break;
		case 4:
			crystalrevetment = "PerfectReflector";
		break;
		case 5:
			crystalrevetment = "PseudoTyvek";
		break;
		case 6:
			crystalrevetment = "Alveola";
		break;
		case 7:
			crystalrevetment = "none";
		break;
	}
	TSTRAP *crystal;
	crystal = new TSTRAP("Crystal","Crystal","PbWO4",crystalrevetment,crystaltype,iso_dx);
	crys_dz = crystal->Getdz();
	crystal->GetCMSdim(crystaltype,ferule_BF,crys_AFl,crys_BF,crys_CFl,crys_ARl,crys_BR,
		crys_CRl,iso_dx);
	crys_AF = crys_AFl + iso_dx;
	crys_CF = crys_CFl + iso_dx;
	crys_AR = crys_ARl + iso_dx;
	crys_CR = crys_CRl + iso_dx;

	if (verbosity >= 2) then
	{
		cout << endl;
		cout << "  Dimensions of crystal" << endl;
		cout << "    half height : " << crys_dz << endl;
		cout << "    AF : " << crys_AF << "   BF : " << crys_BF << "   CF : " << crys_CF << endl;
		cout << "    AR : " << crys_AR << "   BR : " << crys_BR << "   CR : " << crys_CR << endl;
	}
	crystal->GetPoints(x0,y0,z0,x1,y1,z1,x2,y2,z2,x3,y3,z3,
		x4,y4,z4,x5,y5,z5,x6,y6,z6,x7,y7,z7);

	Double_t center_inter_x = (x0 + x1 + x2 + x3) / 4.0;
	Double_t center_inter_y = (y0 + y1 + y2 + y3) / 4.0;
	Double_t center_inter_z = (z0 + z1 + z2 + z3) / 4.0;
	Double_t center_exter_x = (x4 + x5 + x6 + x7) / 4.0;
	Double_t center_exter_y = (y4 + y5 + y6 + y7) / 4.0;
	Double_t center_exter_z = (z4 + z5 + z6 + z7) / 4.0;

//Printing
	if (verbosity >= 3) then
	{
		cout << endl;
		cout << "  Point 0 : ";  cout.width(8);  cout << x0 << "  ";  cout.width(8);
		cout << y0 << "  ";  cout.width(8);  cout << z0 << endl;
		cout << "  Point 1 : ";  cout.width(8);  cout << x1 << "  ";  cout.width(8);
		cout << y1 << "  ";  cout.width(8);  cout << z1 << endl;
		cout << "  Point 2 : ";  cout.width(8);  cout << x2 << "  ";  cout.width(8);
		cout << y2 << "  ";  cout.width(8);  cout << z2 << endl;
		cout << "  Point 3 : ";  cout.width(8);  cout << x3 << "  ";  cout.width(8);
		cout << y3 << "  ";  cout.width(8);  cout << z3 << endl;
		cout << "  Point 4 : ";  cout.width(8);  cout << x4 << "  ";  cout.width(8);
		cout << y4 << "  ";  cout.width(8);  cout <<  z4 << endl;
		cout << "  Point 5 : ";  cout.width(8);  cout << x5 << "  ";  cout.width(8);
		cout << y5 << "  ";  cout.width(8);  cout <<  z5 << endl;
		cout << "  Point 6 : ";  cout.width(8);  cout << x6 << "  ";  cout.width(8);
		cout << y6 << "  ";  cout.width(8);  cout <<  z6 << endl;
		cout << "  Point 7 : ";  cout.width(8);  cout << x7 << "  ";  cout.width(8);
		cout << y7 << "  ";  cout.width(8);  cout <<  z7 << endl;
		cout << endl;
		cout << "  Centre face interne : " << center_inter_x << " ; " << center_inter_y << " ; " << center_inter_z << endl;
		cout << "  Centre face externe : " << center_exter_x << " ; " << center_exter_y << " ; " << center_exter_z << endl << endl;
	}
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
	const Double_t capsule_BR     = 2.3;	//	width of the capsule, parallel to BR [B. Ille]
	const Double_t capsule_CR     = 2.2;	//	width of the capsule, parallel to CR [B. Ille]
	const Double_t d_BR_caps_crys = 0.12;	//	distance from edge of capsule to edge BR [B. Ille]
	const Double_t d_CR_caps_crys = 0.12;	//	distance from edge of capsule to edge CR [B. Ille]
	Double_t x8,y8,x9,y9,x10,y10,x11,y11;	//	Points to define the rectangle of the capsule
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
	if (verbosity >= 3) then
	{
		cout << "  Rectangle of the capsule :" << endl;
		cout << "  p8 : "; p8->Print();
		cout << "  p9 : "; p9->Print();
		cout << "  p10: "; p10->Print();
		cout << "  p11: "; p11->Print();
	}
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
	Double_t d_BF_fer_crys;	//	distance from edge of ferule to edge BF [Anfreville]
	Double_t d_CF_fer_crys;	//	distance from edge of ferule to edge CF [Anfreville]
	d_BF_fer_crys = (crys_AF - ferule_AF) / deux;
	d_CF_fer_crys = (crys_BF - ferule_BF) / deux;
	if (verbosity >= 2) then
	{
		cout << "  ferule_AF     : " << ferule_AF     << "   ferule_BF     : " << ferule_BF << endl;
		cout << "  d_BF_fer_crys : " << d_BF_fer_crys << "   d_CF_fer_crys : " << d_CF_fer_crys << endl;
	}
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
	if (verbosity >= 2) then
	{
		cout << "  Rectangle of the ferule :" << endl;
		cout << "  p12: "; p12->Print();
		cout << "    p13: "; p13->Print();
		cout << "    p14: "; p14->Print();
		cout << "    p15: "; p15->Print();
	}
	ps->AddSubPoint(p12);
	ps->AddSubPoint(p13);
	ps->AddSubPoint(p14);
	ps->AddSubPoint(p15);
	TFace *face7;
	face7 = new TFace(crystal,7,4,polygonal,ps->fPoints,ps->fGCenter,"Revferule",
		12,13,14,15,kFALSE);
	ps->AddSubFace(face7);

//Getting the pointer to face 1 of crystal, which is the depolished face.
	TFace *fdepol;
	fdepol = crystal->fSuppl->GetFace(1);
	fdepol->SetDepolished(depolishangle);	//	Setting a depolish
//
//crystal has bevellings (chamfers).
	const Double_t sizechamfer = 0.05; // [M. Lebeau]
	Double_t edgechamfer = sizechamfer/squareroot2;
	crystal->fSuppl->SetBevellings(kTRUE,edgechamfer,"Revchamf");
//
//We define the center of face 5.
	const Double_t apd_BR  = 0.25;	//	half-width of APDs in direction || to BR [B. Ille]
	const Double_t apd_CR  = 0.25;	//	half-width of APds in direction || to CR [B. Ille]
	const Double_t sep_APD = 0.21;	//	half separation between the 2 APDs [B. Ille]
	const Double_t d_BR_APD_caps = 0.39;	//	distance from edge of APD1 to edge BR of capsule [B. Ille]
	const Double_t d_CR_APD_caps = 0.9;	//	distance from edge of APDs to edge CR of capsule [B. Ille]
	Double_t center5_x;	//	x of "center" of face 5
	Double_t center5_y;	//	y of "center" of face 5
	center5_x = x4 + d_BR_caps_crys + d_BR_APD_caps + deux*apd_CR + sep_APD;
	center5_y = y4 + d_CR_caps_crys + d_CR_APD_caps + apd_BR;
	if (verbosity >= 2) then
	{
		cout << "  center  face 5 : (" << center5_x << ',' << center5_y << ')' << endl;
	}
	const Double_t glue_dz = 0.0015;	//	half width of glue between crystal and APD [Montecchi]
	const Double_t apd_dz  = 0.005;	//	half width of silicium of APD
	Double_t apd1_x,apd1_y,apd1_z,apd2_x,apd2_y,apd2_z;
	apd1_x = center5_x - sep_APD - apd_CR;	//	position x center of APD 1.
	apd1_y = center5_y;	//	position y center of APD 1.
	apd1_z = crys_dz + deux*glue_dz + apd_dz;	//	position z center of APD 1.
	apd2_x = center5_x + sep_APD + apd_CR;	//	position y center of APD 2.
	apd2_y = center5_y;	//	position x center of APD 2.
	apd2_z = apd1_z;	//	position z center of APD 2.
	if (verbosity >= 2) then
	{
		cout << "  center of APD1 : (" << apd1_x << ',' << apd1_y << ',' << apd1_z << ')' << endl;
		cout << "  center of APD2 : (" << apd2_x << ',' << apd2_y << ',' << apd2_z << ')' << endl;
	}
	Double_t glue1_z = crys_dz + glue_dz;	//	position z center of glue for APD1.
	Double_t glue2_z = glue1_z;	//	position z center of glue for APD2.
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
	const Double_t airgap_dz = 0.015;	//	semi-height of air gap [Anfreville]
	const Double_t airgap_r  = 0.075;	//	radius of air gap [Anfreville]
	const Double_t fibre_dz  = 0.50;	//	semi-height of fibre [irrelevant]
	const Double_t fibre_r   = 0.01;	//	radius of fibre [Anfreville]
//
//  Position of air gap and fibre
	Double_t center4_x;	//	position x of airgap and fibre
	Double_t center4_y;	//	position y of airgap and fibre
	Double_t airgap_z;	//	position z of center of airgap
	Double_t fibre_z;	//	position z of center of fibre
	center4_x = x0 + d_BF_fer_crys + ferule_AF/deux;
	center4_y = y0 + d_CF_fer_crys + ferule_BF/deux;
	airgap_z  = - (crys_dz + airgap_dz);
	fibre_z   = airgap_z - (airgap_dz + fibre_dz);
	if (verbosity >= 2) then
	{
		cout << "  center4_x : " << center4_x << "   center4_y : " << center4_y << endl;
		cout << "  airgap_z  : " << airgap_z  << "   fibre_z   : " << fibre_z   << endl;
	}
//  Airgap
	TSCYL *airgap;
	airgap = new TSCYL("AirGap","AirGap","Air","Revferule",airgap_r,airgap_dz);
//  Fibre
	TSCYL *fibre;
	fibre = new TSCYL("Fibre","Fibre","HCP200led","TotAbsorbing",fibre_r,fibre_dz);
//use different colors
	crystal->SetLineColor(106);
	glue1->SetLineColor(102);
	glue2->SetLineColor(102);
	apd1->SetLineColor(104);
	apd2->SetLineColor(104);
	airgap->SetLineColor(149);
	fibre->SetLineColor(146);
	crystal->SetLineWidth(2);
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
	TSNode *node3 = new TSNode("node3","node3",glue1,apd1_x,apd1_y,glue1_z);
	TSNode *node4 = new TSNode("node4","node4",apd1, apd1_x,apd1_y,apd1_z);
	TSNode *node5 = new TSNode("node5","node5",glue2,apd2_x,apd2_y,glue2_z);
	TSNode *node6 = new TSNode("node6","node6",apd2, apd2_x,apd2_y,apd2_z);
	TSNode *node7 = new TSNode("node7","node7",airgap,center4_x,center4_y,airgap_z);
	TSNode *node8 = new TSNode("node8","node8",fibre, center4_x,center4_y,fibre_z);
	TContact *contactcs,*contactcg1,*contactga1,*contactcg2,*contactga2,*
		*contactca,*contactaf;
	contactcg1 = new TContact("crystal_glue1","crystal_glue1","Crystal","glue1",containing);
	contactga1 = new TContact("glue1_apd1","glue1_apd1","glue1","apd1",identical);
	contactcg2 = new TContact("crystal_glue2","crystal_glue2","Crystal","glue2",containing);
	contactga2 = new TContact("glue2_apd2","glue2_apd2","glue2","apd2",identical);
	contactca  = new TContact("crystal_air","crystal_air","Crystal","AirGap",containing);
	contactaf  = new TContact("air_fibre","air_fibre","AirGap","Fibre",containing);

}
