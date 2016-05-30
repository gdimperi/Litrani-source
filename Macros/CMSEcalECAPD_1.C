{
//
//  CMSEcalECAPD_1.C is a description of the final setup for an End-Cap crystal of the 
//CMS Ecal detector, except that here the VPT has been replaced by an APD.
// The material PbWO4 of the crystal is treated as birefringent or not.
//On the upper face of the PbWO4 crystal are the 2 APDs AND the optical fibre
//(contrary to the Barrel crystals which have the fibre on the opposit small end
//face of the crystal).
//  So there are 7 shapes in this setup :
//
// - (a) 1 TSTRAP : CMS crystal of type 18 (Endcap) made of PbWO4
// - (c) 2 TSBRIK : glue of the APDs
// - (d) 2 TSBRIK : the APDs made of silicium
// - (e) 1 TSBRIK : air gap between optical fibre and crystal
// - (f) 1 TSCYL  : the optical fibre
//
//  PbWO4 is the only material which is fluorescent, i.e. able to emit light when
//crossed by particles. The other materials are air, glue, silicium and
//silice/polymere hcp200 of the fibre.
//  The CMS EndCap crystal has its lateral faces wrapped with a black (absorbing)
//revetment (carbon fibres). Between this black revetment and the crystal is a slice
//of air, so that total reflection remain possible. The small end face of the crystal
//is covered with tyvek, also with a slice of air. The big end face contains the
//APDs and the fibre. The part neither covered by the glue of the APDs nor by the
//air junction of the fibre is covered with tyvek with a slice of air.
//
//  BEGIN OF CMSEcalEC_setup.C
//
//
//  Physical constants
//
  const Double_t zero            = 0.0;
  const Double_t un              = 1.0;
  const Double_t deux            = 2.0;
  const Double_t squareroot2     = TMath::Sqrt(deux);
  const Double_t RadLPbWO4       = 0.893;       //radiation length of PbWO4 in cm
  const Double_t RMolierePbWO4   = 2.0;         //Moliere radius of PbWO4 in cm
  const Double_t PhotMev         = 100.0;       //Nb. of photons generated per Mev of deposited energy
  const Int_t    ZmaxLR          = 24;          //maximal length of cascade in radiation length.
  const Double_t AbsLAir         = 10000.0;     //Absorption length of air
  const Double_t RefrIAir        = 1.0003;      //Refractive index of air
  const Double_t PropDiffusTyvek = 874.0/875.0; //For tyvek, proportion of diffusion versus reflexion
  const Double_t DamageE0        = 100.0;       //Energy of shower having caused radiation damage
  const Double_t MaxApert        = 22.5;        //Maximal aperture angle of the optical fibre
//
//  Geometrical constants
//
  const Double_t d_BR_VPTedge    = 0.12;	//distance from edge of VPT to edge BR
  const Double_t d_CR_VPTedge    = 0.12;	//distance from edge of VPT to edge CR
  const Double_t VPT_radius      = 1.0;         //radius of VPT
  const Double_t VPT_dz          = 0.5;         //semi height of VPT
  const Double_t sizechamfer     = 0.05;        // [M. Lebeau]
  const Double_t glue_dz         = 0.0015;	//half width of glue between crystal and APD [Montecchi]
  const Double_t airgap_dz       = 0.015;	//semi-height of air gap [Anfreville]
  const Double_t airgap_dx       = 0.075;	//half-width of air gap [Anfreville]
  const Double_t airgap_dy       = 0.075;	//half-width of air gap [Anfreville]
  const Double_t fibre_dz        = 0.50;	//semi-height of fibre [irrelevant]
  const Double_t fibre_r         = 0.01;	//radius of fibre [Anfreville]
  const Double_t FibSupSize      = 0.25;        //Size of the fibre support in x and in y (square)
  const Double_t d_BR_FibSup     = 0.12;        //distance from edge of FibSup to edge BR [Verify!] (pos 1)
  const Double_t d_AR_FibSup     = 0.12;        //distance from edge of FibSup to edge AR [Verify!]
  const Double_t d_DR_FibSup     = 0.12;        //distance from edge of FibSup to edge DR [Verify!] (pos 2)
  Double_t ferule_BF;                           //not used but given by TSTRAP::GetCMSdim()
//
//  Definition of the material PbWO4. The magnetic permeability is declared to be 1, not
//depending upon wavelength. PbWO4 is declared as uniaxial negativ birefringent
//The absorption length is taken as the one of crystal 8981 before irradiation.
//PbWO4 is declared fluorescent, with one component at 420 nm and another one at 500 nm.
//These 2 components are splitted into 3 possible decay times. A call to SetPhotMev sets
//the number of photons emitted per Mev of deposited energy by the electromagnetic shower.
//A call to SetCascades gives the values of radiation length and Moliere radius.
//
  Bool_t HasDiffusion = kFALSE; //if true, adds diffusion length
  Double_t diffL = 50;          //diffusion length
  cout << endl;
  cout << "By default, setup without diffusion length" << endl;
  cout << "Do you want a diffusion length ? ";
  cin  >> ccc;
  if (ccc=='y') {
    HasDiffusion = kTRUE;
    cout << "Diffusion length is : " << diffL << endl;
    cout << "Do you want to change it ? ";
    cin  >> ccc;
    if (ccc=='y') {
      cout << "Enter new diffusion length [cm] : ";
      cin  >> diffL;
    }
  }
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
  pbwo4->SetCascades(RadLPbWO4,RMolierePbWO4);
  pbwo4->SetPhotMev(PhotMev);
  TElecCascade::fZmaxLR = ZmaxLR;
  if (HasDiffusion) pbwo4->SetDiffusionLength(diffL,kTRUE);
//
//  Definition of air, with magnetic permeability of 1 and absorption length
//of 10000 cm, independent of wavelength. Air is also isotropic, with index
//of refraction of 1.0003 independent of wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,un,AbsLAir);
  air->IsIsotropic(RefrIAir);
//
//  Definition of glue, with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsorptionLength_Meltmount1704".
//Glue is isotropic with index of refraction depending  upon wavelength according
//to the fit "RefrIndex_RTV3145".
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
//  Definition of the material of the fibre, généric
  TOpticMaterial *hcp200;
  hcp200 = new TOpticMaterial("HCP200","HCP200",kFALSE,un,zero);
  hcp200->IsIsotropic("RefrIndex_hcp200");
//  Definition of alveola's equivallent wrapping. For the endcap crystals,it
//seems that in fact the revetment is essentially black, it is why we put a supplementary
//absorption of 0.99. By that, "RIndexRev_CMSalveola", "IIndexRev_CMSalveola" play
//almost no role. But total reflexion on slice of air remains possible.
  const Double_t PropDiffusAlveola = 0.11738;
  TRevetment *alveola;
  alveola = new TRevetment("Alveola","Alveola", "Air", PropDiffusAlveola,
                           "RIndexRev_CMSalveola", "IIndexRev_CMSalveola", un,0.99);
//
//  Definition of the wrapping "Revcaps", describing the surface of the capsule containing
//the APD, which is in contact with face 5 of the crystal.
  TRevetment *revcaps;
  revcaps  = new TRevetment("Revcaps","Revcaps","Glue",un,"RIndexRev_CMScapsula",
                            "IIndexRev_CMScapsula",un,0.2,85.0);
//  Definition of the wrapping tyvek, with a slice of air inbetween ( a slice, not a
//thin slice ) with real part and imaginary part of index of refraction varying in
//function of wavelength according to the fits "RIndexRev_Tyvek" and "IIndexRev_Tyvek".
//The magnetic permeability is 1, independent of wavelength. There is no supplementary
//absorption. The wrapping is 99.89 % diffusing. 
//(Mean absorption 12.5%, mean diffusion 87.4%, mean specular 0.1%, 
//cf. M. Montecchi, 15 Feb. 2000)
  TRevetment *tyvek;
  tyvek = new TRevetment("Tyvek","Tyvek","Air",PropDiffusTyvek,"RIndexRev_Tyvek","IIndexRev_Tyvek",un);
//
//  Definition of the wrapping "Revchamf", without slice of air inbetween, with real
//part and imaginary part of index of refraction varying in function of wavelength
//according to the fits "RIndexRev_CMSalveola" and "IIndexRev_CMSalveola". The
//magnetic permeability is 1, independent of wavelength. There is a supplementary
//absorption of 0.05. The wrapping is 0% reflecting, 100% diffusing, to simulate a
//depolish. The maximum angle theta for a diffused photon is 70 degree.
  TRevetment *revchamf;
  revchamf = new TRevetment("Revchamf","Revchamf","none",un,"RIndexRev_CMSalveola",
                            "IIndexRev_CMSalveola",un,0.05,70.0);
//
//  Definition of the wrapping "Revtop", describing the surface of the top of the alveola,
//which is the wrapping of the part of face 5 of the crystal which is not covered
//The fact that we wrongly refer to "Tyvek" does not matter, since we have put a
//supplementary absorption of 0.99. But we have not put a supplementary absorption of 1.0,
//since we insist that there is a slice of air and that total reflection is possible.
  TRevetment *revtop;
  revtop = new TRevetment("Revtop","Revtop","Air",un,"RIndexRev_Tyvek",
		             "IIndexRev_Tyvek",un,0.99,80.0);
//
//  Definition of the wrapping "TotAbsorbing", giving only the 1st 2 arguments. When the
//other arguments 3 to 8 are not given, following default values are assumed :
//
// (3) - nameMat = "none" : no slice material between face and wrapping.
// (4) - diffus  = 0.0    : Reflecting, not diffusing. Irrelevant because of (8).
// (5) - Rindex  = 2.0    : Real part of index. Irrelevant because of (8).
// (6) - Iindex  = 6.0    : Imaginary part of Index. Irrelevant because of (8).
// (7) - Mu      = 1.0    : Magnetic permeability. Irrelevant because of (8).
// (8) - supplAbs = 1.0   : supplementary absorption of 100%.
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
//
//    (x0,y0,z0),(x1,y1,z1),(x2,y2,z2),(x3,y3,z3),
//    (x4,y4,z4),(x5,y5,z5),(x6,y6,z6),(x7,y7,z7)
//    z0 = z1 = z2 = z3 = - crys_dz
//    z4 = z5 = z6 = z7 = + crys_dz
//
//  The 8 points of the TSTRAP representing the crystal. The crystal is then centered
//in the sense that we impose:
//
//    x0+x1+x2+x3+x4+x5+x6+x7 = 0
//    y0+y1+y2+y3+y4+y5+y6+y7 = 0
//
//  The center of the crystal is then (0,0,0)
//
  const Double_t iso_dx = zero;
  const Short_t  crystaltype = 18;
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
//
// The crystal is of type 18, i.e. it is an endcap crystal, which may be built with the
//Litrani shape TSTRAP. The long axis of the crystal is along Oz. The shape "crystal" is
//taken as the reference shape. So that the local coordinate system of the shape "crystal"
//is at the same time the world coordinate system WCS.
// Description of the CMS crystal by the 7 parameters AF, BF, CF, AR, BR, CR and crys_dz.
//Look at "The Electromagnetic Calorimeter Project, CMS TDR4" pages 53/54. We use the
//constructor of TSTRAP which is specialized for CMS crystal. It contains the values
//for AF,BF,CF,AR,BR,CR and ferule_BF for all 18 types of CMS crystals.
//Look at the description of TSTRAP.
//
  TSTRAP *crystal;
  crystal = new TSTRAP("Crystal","Crystal","PbWO4","Alveola",crystaltype,iso_dx);
  crys_dz = crystal->Getdz();
  crystal->GetCMSdim(crystaltype,ferule_BF,crys_AF,crys_BF,crys_CF,crys_AR,crys_BR,crys_CR,iso_dx);
  if (verbosity >= 2) then {
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
  if (verbosity >= 3) {
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
    cout << "  Center small endface : " << center_inter_x << " ; " << center_inter_y << " ; " << center_inter_z << endl;
    cout << "  Centre big   endface : " << center_exter_x << " ; " << center_exter_y << " ; " << center_exter_z << endl << endl;
  }
//We change the wrapping of faces 4 and 5.
  TFace *face4, *face5;
  TSupplShape *ps = crystal->fSuppl;
  face4 = ps->GetFace(4);
  face5 = ps->GetFace(5);
  face4->ChangeRevetment("Tyvek");
  face5->ChangeRevetment("Revtop");
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
//  We propose 2 positions for the "optical axis" (The Ox' axis of "the 3 optical
//axis", 
// - Ox' associated with epsilon a
// - Oy' associated with epsilon b
// - Oz' associated with epsilon c
//with epsilon b = epsilon c, because PbWO4 is uniaxial negative birefringent.
//epsilon is the dielectric tensor)
//
// (1) - 1st positioning: like Bogorodisk crystals, "optical axis
//        parallell to CR (or CF)
// (2) - 2nd positioning: like Chinese crystals, "optical axis
//        parallel to axis of crystal, axis Oz
//
  if (!isopbwo4) {
    if (kOptAxis==1) crystal->SetOpticalAxis(90.0,0.0,90.0,90.0);
    else             crystal->SetOpticalAxis(0.0,0.0,90.0,0.0);
  }
//crystal has bevellings (chamfers).
  Double_t edgechamfer = sizechamfer/squareroot2;
  ps->SetBevellings(kTRUE,edgechamfer,"Revchamf");
//Radiation damages
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
  Double_t ShiftX; //shift in x between the 2 positions
  Double_t x12,y12,x13,y13,x14,y14,x15,y15;
  x12 = x7 + d_BR_FibSup;
  x15 = x12;
  x13 = x12 + FibSupSize;
  x14 = x13;
  y15 = y7 - d_AR_FibSup;
  y14 = y15;
  y12 = y15 - FibSupSize;
  y13 = y12;
  if (WhereFib == 2) {
    ShiftX = x6 - d_DR_FibSup - FibSupSize - x12;
    x12 += ShiftX;
    x13 += ShiftX;
    x14 += ShiftX;
    x15 += ShiftX;
  }
  Double_t centerX_FibSup = (x12+x13)/deux;
  Double_t centerY_FibSup = (y12+y15)/deux;
//Position of air gap and fibre
  Double_t centerZ_FibSup;	//position z of center of airgap
  Double_t centerZ_Fib;	        //position z of center of fibre
  centerZ_FibSup = crys_dz + airgap_dz;
  centerZ_Fib    = centerZ_FibSup + airgap_dz + fibre_dz;
  if (verbosity >= 2) {
    cout << "  centerX_FibSup : " << centerX_FibSup << "   centerY_FibSup : " << centerY_FibSup << endl;
    cout << "  centerZ_FibSup  : " << centerZ_FibSup  << "   centerZ_Fib   : " << centerZ_Fib   << endl;
  }
//  Airgap
  if (verbosity >= 2) {
    cout << endl;
    cout << "TSBRIK AirGap for fibre" << endl;
    cout << endl;
    cout << "  airgap_dx : " << airgap_dx << endl;
    cout << "  airgap_dy : " << airgap_dy << endl;
    cout << "  airgap_dz : " << airgap_dz << endl;
  }
  TSBRIK *airgap;
  airgap = new TSBRIK("AirGap","AirGap","Air","TotAbsorbing",airgap_dx,airgap_dy,airgap_dz);
  if (verbosity >= 2) {
    cout << endl;
    cout << "TSCYL Fibre" << endl;
    cout << endl;
    cout << "  fibre_r   : " << fibre_r << endl;
    cout << "  fibre_dz  : " << fibre_dz << endl;
  }
//  Fibre
  TSCYL *fibre;
  fibre = new TSCYL("Fibre","Fibre","HCP200","TotAbsorbing",fibre_r,fibre_dz);
//
//We define the center of face 5, the point just between the 2 APDs.
  const Double_t apd_BR  = 0.25;       //half-width of APDs in direction || to BR [B. Ille]
  const Double_t apd_CR  = 0.25;       //half-width of APds in direction || to CR [B. Ille]
  const Double_t sep_APD = 0.21;       //half separation between the 2 APDs [B. Ille]
  //  const Double_t d_BR_APD_caps = 0.39; //distance from edge of APD1 to edge BR of capsule [B. Ille]
  const Double_t d_BR_APD_caps = 0.6; //distance from edge of APD1 to edge BR of capsule [B. Ille]
  const Double_t d_CR_APD_caps = 0.9;  //distance from edge of APDs to edge CR of capsule [B. Ille]
  Double_t center5_x;                  //x of "center" of face 5
  Double_t center5_y;                  //y of "center" of face 5
  center5_x = x4 + d_BR_caps_crys + d_BR_APD_caps + deux*apd_CR + sep_APD;
  center5_y = y4 + d_CR_caps_crys + d_CR_APD_caps + apd_BR;
  cout << "  center  face 5 : (" << center5_x << ',' << center5_y << ')' << endl;
  const Double_t glue_dz = 0.0015;     //half width of glue between crystal and APD [Montecchi]
  const Double_t apd_dz  = 0.005;      //half width of silicium of APD
  Double_t apd1_x,apd1_y,apd1_z,apd2_x,apd2_y,apd2_z;
  apd1_x = center5_x - sep_APD - apd_CR;    //position x center of APD 1.
  apd1_y = center5_y;                       //position y center of APD 1.
  apd1_z = crys_dz + deux*glue_dz + apd_dz; //position z center of APD 1.
  apd2_x = center5_x + sep_APD + apd_CR;    //position y center of APD 2.
  apd2_y = center5_y;                       //position x center of APD 2.
  apd2_z = apd1_z;                          //position z center of APD 2.
  cout << "  center of APD1 : (" << apd1_x << ',' << apd1_y << ',' << apd1_z << ')' << endl;
  cout << "  center of APD2 : (" << apd2_x << ',' << apd2_y << ',' << apd2_z << ')' << endl;
  Double_t glue1_z = crys_dz + glue_dz;     //position z center of glue for APD1.
  Double_t glue2_z = glue1_z;               //position z center of glue for APD2.
//
//  Glue for APD1
  if (verbosity >= 2) {
    cout << endl;
    cout << "TSBRIK glue1" << endl;
    cout << endl;
    cout << "  apd_CR    : " << apd_CR  << endl;
    cout << "  apd_BR    : " << apd_BR  << endl;
    cout << "  glue_dz   : " << glue_dz << endl;
  }
  TSBRIK *glue1;
  glue1 = new TSBRIK("glue1","glue1","Glue","TotAbsorbing",apd_CR,apd_BR,glue_dz);
//  APD1
  if (verbosity >= 2) {
    cout << endl;
    cout << "TSBRIK apd1" << endl;
    cout << endl;
    cout << "  apd_CR    : " << apd_CR  << endl;
    cout << "  apd_BR    : " << apd_BR  << endl;
    cout << "  apd_dz    : " << apd_dz  << endl;
  }
  TSBRIK *apd1;
  apd1 = new TSBRIK("apd1","apd1","Silicium","TotAbsorbing",apd_CR,apd_BR,apd_dz);
  apd1->fSuppl->SetAPD("hamamatsu1","hamamatsu1",4,5,"GainProfile_CMSAPD");
//  Glue for APD2
  if (verbosity >= 2) {
    cout << endl;
    cout << "TSBRIK glue2" << endl;
    cout << endl;
    cout << "  apd_CR    : " << apd_CR  << endl;
    cout << "  apd_BR    : " << apd_BR  << endl;
    cout << "  glue_dz   : " << glue_dz << endl;
  }
  TSBRIK *glue2;
  glue2 = new TSBRIK("glue2","glue2","Glue","TotAbsorbing",apd_CR,apd_BR,glue_dz);
//  APD2
  if (verbosity >= 2) {
    cout << endl;
    cout << "TSBRIK apd2" << endl;
    cout << endl;
    cout << "  apd_CR    : " << apd_CR  << endl;
    cout << "  apd_BR    : " << apd_BR  << endl;
    cout << "  apd_dz    : " << apd_dz  << endl;
  }
  TSBRIK *apd2;
  apd2 = new TSBRIK("apd2","apd2","Silicium","TotAbsorbing",apd_CR,apd_BR,apd_dz);
  apd2->fSuppl->SetAPD("hamamatsu2","hamamatsu2",4,5,"GainProfile_CMSAPD");
//use different colors
  crystal->SetLineColor(106);
  airgap->SetLineColor(149);
  fibre->SetLineColor(146);
  crystal->SetLineWidth(2);
  airgap->SetLineWidth(2);
  fibre->SetLineWidth(2);
  glue1->SetLineColor(102);
  glue2->SetLineColor(102);
  apd1->SetLineColor(104);
  apd2->SetLineColor(104);
  glue1->SetLineWidth(2);
  glue2->SetLineWidth(2);
  apd1->SetLineWidth(2);
  apd2->SetLineWidth(2);
//Position in space
  TSNode *node1 = new TSNode("node1","node1",crystal);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);
  node1->cd();
  if (verbosity >= 2) {
    cout << endl;
    cout << "node3 glue1" << endl;
    cout << endl;
    cout << "  apd1_x    : " << apd1_x  << endl;
    cout << "  apd1_y    : " << apd1_y  << endl;
    cout << "  glue1_z   : " << glue1_z << endl;
  }
  TSNode *node3 = new TSNode("node3","node3",glue1,apd1_x,apd1_y,glue1_z);
  if (verbosity >= 2) {
    cout << endl;
    cout << "node4 apd1" << endl;
    cout << endl;
    cout << "  apd1_x    : " << apd1_x  << endl;
    cout << "  apd1_y    : " << apd1_y  << endl;
    cout << "  apd1_z    : " << apd1_z  << endl;
  }
  TSNode *node4 = new TSNode("node4","node4",apd1, apd1_x,apd1_y,apd1_z);
  if (verbosity >= 2) {
    cout << endl;
    cout << "node5 glue2" << endl;
    cout << endl;
    cout << "  apd2_x    : " << apd2_x  << endl;
    cout << "  apd2_y    : " << apd2_y  << endl;
    cout << "  glue2_z   : " << glue2_z << endl;
  }
  TSNode *node5 = new TSNode("node5","node5",glue2,apd2_x,apd2_y,glue2_z);
  if (verbosity >= 2) {
    cout << endl;
    cout << "node6 apd2" << endl;
    cout << endl;
    cout << "  apd2_x    : " << apd2_x  << endl;
    cout << "  apd2_y    : " << apd2_y  << endl;
    cout << "  apd2_z    : " << apd2_z  << endl;
  }
  TSNode *node6 = new TSNode("node6","node6",apd2, apd2_x,apd2_y,apd2_z);
  if (verbosity >= 2) {
    cout << endl;
    cout << "node7 airgap" << endl;
    cout << endl;
    cout << "  centerX_FibSup    : " << centerX_FibSup  << endl;
    cout << "  centerY_FibSup    : " << centerY_FibSup  << endl;
    cout << "  centerZ_FibSup    : " << centerZ_FibSup  << endl;
  }
  TSNode *node7 = new TSNode("node7","node7",airgap,centerX_FibSup,centerY_FibSup,centerZ_FibSup);
  if (verbosity >= 2) {
    cout << endl;
    cout << "node8 fibre" << endl;
    cout << endl;
    cout << "  centerX_FibSup    : " << centerX_FibSup  << endl;
    cout << "  centerY_FibSup    : " << centerY_FibSup  << endl;
    cout << "  centerZ_Fib       : " << centerZ_Fib     << endl;
  }
  TSNode *node8 = new TSNode("node8","node8",fibre, centerX_FibSup,centerY_FibSup,centerZ_Fib);
  TContact *CrystalAir,*AirFibre,*contactcg1,*contactga1,*contactcg2,,*contactga2;
  CrystalAir = new TContact("crystal_air","crystal_air","Crystal","AirGap",containing);
  AirFibre   = new TContact("air_fibre","air_fibre","AirGap","Fibre",containing);
  contactcg1 = new TContact("crystal_glue1","crystal_glue1","Crystal","glue1",containing);
  contactga1 = new TContact("glue1_apd1","glue1_apd1","glue1","apd1",identical);
  contactcg2 = new TContact("crystal_glue2","crystal_glue2","Crystal","glue2",containing);
  contactga2 = new TContact("glue2_apd2","glue2_apd2","glue2","apd2",identical);
}
