{
  char *name    = "GLASTWithPINs";
  char *listing = "GLASTWithPINs";
  char *upcom   = "Simulation of a GLAST CsI(Tl) crystal with 4 PINs";
  char *downcom = "Source of light : beam of muons. 4 PIN diodes as detectors";
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
//   GLASTWITHPINS
//
//  GLASTWithPINs.C is an example of a setup with a crystal of CsI(Tl) having the shape of a
//rectangular parallelipiped. All the necessary fits or interpolations are searched on
//the ROOT database file SplineFitDB.rdb. In this example, the material CsI(Tl) is optically
//isotropic.
//  On the upper small face and on the lower small face of the CsI(Tl) crystal are 2 PIN
//diodes having the shape of a TSBRIK. So there are 4 PIN diodes in all. These PIN diodes
//are GLUED onto the crystal. The slice of glue has also the shape of a TSBRIK. The PIN
//diodes have a window made of epoxy. These windows have also the shape of TSBRIKs. So
//there are 13 shapes in this setup :
//  -  1 TSBRIK  (crystal of CsITl)
//  -  4 TSBRIKs (glue)
//  -  4 TSBRIKs (windows of epoxy)
//  -  4 TSBRIKs (PIN diode made of silicium)
//  CsI(Tl) is the only material which is fluorescent, i.e. able to emit light when
//crossed by particles. The other materials are the glue, the epoxy, the silicium and air.
//  The crystal of CsITl is wrapped with vm2000, it is the reason why we call the
//revetment "vm2000". The vm2000 revetment is well described by the fits
//"RIndexRev_vm2000" and "IIndexRev_vm2000". Values for the real part of the index have
//been carefully adjusted so that we obtain the values of the albedo of the revetment
//vm2000 that have been measured in the GLAST experiment. The revetment "vm2000" is mainly
//diffusive, but has a small proportion of 2.0% of photons reflected. This also has been
//measured and is described properly by Litrani. Between the crystal and the vm2000 foil
//is a slice of air. This is why we had to introduce material air.
//  The TSBRIKs made of glue, the TSBRIKS made of epoxy and the TSBRIKs made of silicium
//(PIN diode) are considered to be wrapped with a totally absorbing revetment which is
//called "totabsorbing".
//  We have yet to find the optical characteristics of the epoxy. For the time being, we
//put arbitrary values.
//  A beam of muons is defined which crosses the crystal of CsITl perpendicularly to the
//long axis of the crystal. The fit "DEdx_MusInCsITlNoL" gives the value
//of the deposited dE/dx per cm as a function of the energy of the muon. Landau
//fluctuations are not taken into account (you can, if you will). The muons have been
//defined has having a fixed momentum of 10 Gev.
//  The beam originates from an ellipse, placed on the left of the crystal, and is displa-
//ced upwards from run to run. The axis of the beam is perpendicular to the ellipse in di-
//rection of the crystal. The aperture of the beam is 5 degrees. The beam crosses the
//crystal orthogonally to the long axis of the crystal.
//
//  CsITl is slow, so it is a good idea to change the default value of TooLate. According
//to Arache Djannati of the GLAST experiment, the time of the gate of the GLAST experiment
//is 2500 ns, which is quite short for CsI(Tl) ! There will be many photons killed because
//too late !
  TLitPhys::Get()->SetLate(2500);
//  Definition of the material CsITl. The magnetic permeability is declared to be 1, not
//depending upon wavelength. The absorpion length is given by the fit "AbsorptionLength_CsITl" in the
//file f_abslcsitl.C. CsITl is declared as optically isotropic, the fits giving the
//variation of the index of refraction as a function of wavelength is "RefrIndex_CsITl",
//defined in the file f_refricsitl.C.
// The fluorescent characteristics of CsI are taken from :
//     Measurements done by Remi Chipaux from Saclay
//     Measurements done by John E. Eldridge
// Please choose the one you want.
//  A call to dEdx indicates that CsITl is able to emit light when crossed by particles.
//The interpolation "DEdx_MusInCsITlNoL" gives the value of dE/dx [Mev] deposited by a muon
//as a function of its energy [Gev]. There are 50000 photons produced per Mev of deposited
//energy. We do not allow Landau fluctuations.
  TOpticMaterial *CsITl;
  CsITl = new TOpticMaterial("CsITl","CsITl",kFALSE,1.0,"AbsorptionLength_CsITl");
  CsITl->IsIsotropic("RefrIndex_CsITl");
  TLitSpectrum *sp;
  Int_t itype;
  cout << endl;
  cout << "You have the choice between 2 fluorescent spectra for CsITl :" << endl;
  cout << "   (1) - Measurements of Remi Chipaux" << endl;
  cout << "   (2) - Measurements of John E. Eldridge" << endl;
  cout << endl;
  cout << "Please enter your choice : ";
  cin  >> itype;
  if (itype != 1) itype = 2;
  switch (itype) {
  case 1:
//    CsITl->FluoComponent(410.0,40.0,0.222222,600.0);
//    CsITl->FluoComponent(410.0,40.0,0.111111,3500.0);
//    CsITl->FluoComponent(470.0,30.0,0.05,600.0);
//    CsITl->FluoComponent(470.0,30.0,0.025,3500.0);
//    CsITl->FluoComponent(520.0,40.0,0.444444,600.0);
//    CsITl->FluoComponent(520.0,40.0,0.222222,3500.0);
    sp = new TLitSpectrum("Spectrum_CsITl","Emission Spectrum | CsI doped with Tallium measured by R Chipaux");
    sp->AddOneComponent(410,40.0,0.333333,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
    sp->AddOneComponent(470,30.0,0.075,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
    sp->AddOneComponent(520,40.0,0.666666,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
    sp->Normalize();
    break;
  default:
//    CsITl->FluoComponent(551.25,51.73,0.4870625,600.0);
//    CsITl->FluoComponent(551.25,51.73,0.2922375,3500.0);
//    CsITl->FluoComponent(428.40,49.78,0.0615,600.0);
//    CsITl->FluoComponent(428.40,49.78,0.0369,3500.0);
//    CsITl->FluoComponent(653.64,39.48,0.0764375,600.0);
//    CsITl->FluoComponent(653.64,39.48,0.0458625,3500.0);
    sp = new TLitSpectrum("Spectrum_CsITlJE","Emission Spectrum | CsI doped with Tallium measured by J.E Elridge");
    sp->AddOneComponent(551.25,51.73,0.7793,600.0,-1.0,0.625,3500.0,-1.0,0.375);
    sp->AddOneComponent(428.40,49.78,0.0984,600.0,-1.0,0.625,3500.0,-1.0,0.375);
    sp->AddOneComponent(653.64,39.48,0.1223,600.0,-1.0,0.625,3500.0,-1.0,0.375);
    sp->Normalize();
    break;
  }
  CsITl->SetLitSpectrum(sp);
//Here 1000 should be 50000 !
  CsITl->dEdx("DEdx_MusInCsITlNoL",1000.0,kFALSE);
//
//  Definition of air, with magnetic permeability of 1 and absorption length of 10000 cm,
//independent of wavelength. Air is also isotropic, with index of refraction of 1.0 inde-
//pendent of wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0);
  air->IsIsotropic(1.0);
//
//  Definition of glue, with magnetic permeability of 1 and absorption length depending
//upon wavelength according to the fit "AbsorptionLength_DC93500". We do not know yet what kind of glue
//will be chosen by GLAST, so we put a fixed index of 1.45 for the time being.
  TOpticMaterial *glue;
  glue = new TOpticMaterial("Glue","Glue",kFALSE,1.0,"AbsorptionLength_DC93500");
// Refractive index of 1.4124 found in DOW CORNING 93-500 documentation, no wave length
//dependency provided.
  glue->IsIsotropic(1.4124);
//
//  Definition of epoxy, with magnetic permeability of 1, absorption length of 0.22 cm,
//refraction index of 1.52, both not depending upon wavelength. These values extrapolated
//from indications given by Hammamatsu.
  TOpticMaterial *epoxy;
  epoxy = new TOpticMaterial("Epoxy","Epoxy",kFALSE,1.0,0.22);
  epoxy->IsIsotropic(1.52);
//
//  Definition of silicium, defined as sensible ( i.e. if a photon dies inside, it may be
//detected), with magnetic permeability of 1 and absorption length depending upon wave-
//length according to the fit "AbsorptionLength_Silicium" in file f_abslsilicium.. Silicium is isotro-
//pic with index of refraction depending  upon wavelength according to the fit
//"RefrIndex_Silicium" in file f_refrisilicium.
  TOpticMaterial *Silicium;
  Silicium = new TOpticMaterial("Silicium","Silicium",kTRUE,1.0,"AbsorptionLength_Silicium");
  Silicium->IsIsotropic("RefrIndex_Silicium");
//
//  Definition of the revetment vm2000, with a slice of Air in-between ( a slice, not a
//thin slice ) with real part and imaginary part of index of refraction varying in
//function of wavelength according to the fits "RIndexRev_vm2000" and "IIndexRev_vm2000".
//The magnetic permeability is 1, independent of wavelength. There is no supplementary
//absorption.  The revetment is 98.0% diffusing.
  const Double_t PropDiffus = 0.05;
  TRevetment *vm2000;
  vm2000 = new TRevetment("vm2000","vm2000","Air",PropDiffus,
                           "RIndexRev_vm2000", "IIndexRev_vm2000",1.0);
//  Bad vm2000 is for the revetment covering the chamfers! We give to this
//part of the revetment a supplementary absorption of 5%!
  TRevetment *vm2000;
  vm2000 = new TRevetment("badvm2000","badvm2000","Air",PropDiffus,
                           "RIndexRev_vm2000", "IIndexRev_vm2000",1.0,0.05);
//
//  Definition of the wrapping "Revcaps", describing the surface of the capsule containing
//the PINs, which is in contact with face 4/5 of the crystal.
  TRevetment *revcaps;
  revcaps  = new TRevetment("Revcaps","Revcaps","Glue",1.0,"RIndexRev_CMScapsula",
                            "IIndexRev_CMScapsula",1.0,0.1);
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
//  Now comes the definition of the geometry. The crystal is assumed to have the shape of
//a rectangular parallelipiped ( a TSBRIK ) with semi-axis crystal_dx/dy/dz. The long axis
//of the crystal is along Oz. The shape "crystal" is taken as the reference shape ( because
//of the code node1->cd(); ). So that the local coordinate system of the shape "crystal" is
//at the same time the world coordinate system. The shape "crystal" is made of CsITl and is
//covered with "vm2000", except for the parts of the shape which are in contact with other
//shapes. There are 2 types of PINs a big one (bpin) and a small one (spin).
  const Double_t zero = 0.0;
  const Double_t crystal_dx = 1.335;  //half width of crystal in Ox direction
  const Double_t crystal_dy = 0.995;  //half width of crystal in Oy direction
  const Double_t crystal_dz = 16.3;   //half length of crystal axis, put here along Oz
  const Double_t bpin_dx    = 0.525;  //half width of big PINs in Ox direction
  const Double_t bpin_dy    = 0.725;  //half width of big PINs in Oy direction
//
//  According to a mail of Traudl Kozanecka of the GLAST experiment, Si Thickness 280
//microns, active depth 180 microns. We take 180 microns.
  const Double_t pin_dz     = 0.009;  //half width of all PINs in Oz direction
  const Double_t spin_dx    = 0.12;   //half width of small PIN in Ox direction
  const Double_t spin_dy    = 0.725;  //half width of small PIN in Oy direction
  const Double_t bpin_x     =-0.255;  //center in x of big PIN
  const Double_t spin_x     =+0.86;   //center in x of small PIN
  const Double_t pin_y      =+0.09;   //center in y of both  PINs
  const Double_t glue_dz    = 0.0025; //half width of all 4 glue shapes in Oz direction.
  const Double_t epoxy_dz   = 0.015;  //half width of all 4 epoxy windows in Oz.
  Double_t glue_z  = crystal_dz + glue_dz; //shift in z for glue
  Double_t epoxy_z = glue_z + glue_dz + epoxy_dz;
  Double_t pin_z   = epoxy_z + epoxy_dz + pin_dz;
//
  TSBRIK *crystal;
  crystal = new TSBRIK("crystal","GLAST crystal","CsITl","vm2000",
                        crystal_dx,crystal_dy,crystal_dz);
//Setting depolish and bevellings, and frame of PINs as revetment for faces 4/5
  const Double_t ThetaDepls   = 31.48;
  const Double_t ThetaDeplsbv = 50.0;
  TSupplShape *ps = crystal->fSuppl;
  ps->SetBevellings(kTRUE,0.1,"badvm2000",ThetaDeplsbv);
  TFace *q0,*q1,*q2,*q3,*q4,*q5;
  q0 = ps->GetFace(0);
  q1 = ps->GetFace(1);
  q2 = ps->GetFace(2);
  q3 = ps->GetFace(3);
  q4 = ps->GetFace(4);
  q5 = ps->GetFace(5);
  q0->SetDepolished(ThetaDepls);
  q1->SetDepolished(ThetaDepls);
  q2->SetDepolished(ThetaDepls);
  q3->SetDepolished(ThetaDepls);
  q4->ChangeRevetment("Revcaps");
  q5->ChangeRevetment("Revcaps");
//
//Definition of the 4 glue shapes
  TSBRIK *glue_ul,*glue_ur,*glue_dl,*glue_dr;
  glue_ul = new TSBRIK("glue_ul","glue_ul","Glue","TotAbsorbing",bpin_dx,bpin_dy,glue_dz);
  glue_ul->SetLineColor(2);
  glue_ul->SetLineWidth(2);
  glue_ur = new TSBRIK("glue_ur","glue_ur","Glue","TotAbsorbing",spin_dx,spin_dy,glue_dz);
  glue_ur->SetLineColor(2);
  glue_ur->SetLineWidth(2);
  glue_dl = new TSBRIK("glue_dl","glue_dl","Glue","TotAbsorbing",bpin_dx,bpin_dy,glue_dz);
  glue_dl->SetLineColor(2);
  glue_dl->SetLineWidth(2);
  glue_dr = new TSBRIK("glue_dr","glue_dr","Glue","TotAbsorbing",spin_dx,spin_dy,glue_dz);
  glue_dr->SetLineColor(2);
  glue_dr->SetLineWidth(2);
//
//Definition of the 4 windows shapes
  TSBRIK *epoxy_ul,*epoxy_ur,*epoxy_dl,*epoxy_dr;
  epoxy_ul = new TSBRIK("epoxy_ul","epoxy_ul","Epoxy","TotAbsorbing",bpin_dx,bpin_dy,epoxy_dz);
  epoxy_ul->SetLineColor(4);
  epoxy_ul->SetLineWidth(2);
  epoxy_ur = new TSBRIK("epoxy_ur","epoxy_ur","Epoxy","TotAbsorbing",spin_dx,spin_dy,epoxy_dz);
  epoxy_ur->SetLineColor(4);
  epoxy_ur->SetLineWidth(2);
  epoxy_dl = new TSBRIK("epoxy_dl","epoxy_dl","Epoxy","TotAbsorbing",bpin_dx,bpin_dy,epoxy_dz);
  epoxy_dl->SetLineColor(4);
  epoxy_dl->SetLineWidth(2);
  epoxy_dr = new TSBRIK("epoxy_dr","epoxy_dr","Epoxy","TotAbsorbing",spin_dx,spin_dy,epoxy_dz);
  epoxy_dr->SetLineColor(4);
  epoxy_dr->SetLineWidth(2);
//
//Definition of the 4 PIN diodes
  TSBRIK *pin_ul,*pin_ur,*pin_dl,*pin_dr;
  pin_ul = new TSBRIK("pin_ul","pin_ul","Silicium","TotAbsorbing",bpin_dx,bpin_dy,pin_dz);
  pin_ul->SetLineColor(49);
  pin_ul->SetLineWidth(2);
  pin_ur = new TSBRIK("pin_ur","pin_ur","Silicium","TotAbsorbing",spin_dx,spin_dy,pin_dz);
  pin_ur->SetLineColor(49);
  pin_ur->SetLineWidth(2);
  pin_dl = new TSBRIK("pin_dl","pin_dl","Silicium","TotAbsorbing",bpin_dx,bpin_dy,pin_dz);
  pin_dl->SetLineColor(49);
  pin_dl->SetLineWidth(2);
  pin_dr = new TSBRIK("pin_dr","pin_dr","Silicium","TotAbsorbing",spin_dx,spin_dy,pin_dz);
  pin_dr->SetLineColor(49);
  pin_dr->SetLineWidth(2);
//
//Declares that these shapes are PIN diodes. PIN diodes are for Litrani a special case of
//APDs, where profile of the APD is such that there is never more than 1 electron produced.
//Be careful, there is a trap here : for the PIN diodes on the top face of the crystal, the
//entry face of PIN is face 4 of TSBRIK and the exit face of the PIN is face 5, but for the
//PIN diodes on the bottom face of the crystal, the entry face of PIN is face 5 and the
//exit face is face 4! The gain profile of the PIN has already been defined by the inter-
//polation "GainProfile_GLASTpin" of the file f_profilepin.C. If you do not know what the gain profi-
//le of a PIN diode or of an APD is, look at the web documentation of the class TSuppl-
//Shape, method SetAPD().
  pin_ul->fSuppl->SetAPD("pin_up_left","pin_up_left",4,5,"GainProfile_GLASTpin",0.5);
  pin_ur->fSuppl->SetAPD("pin_up_right","pin_up_right",4,5,"GainProfile_GLASTpin",0.5);
  pin_dl->fSuppl->SetAPD("pin_down_left","pin_down_left",5,4,"GainProfile_GLASTpin",0.5);
  pin_dr->fSuppl->SetAPD("pin_down_right","pin_down_right",5,4,"GainProfile_GLASTpin",0.5);
//
//TSNode gives the position of a shape with respect to the node currently selected, by
//mean of a translation vector and a rotation matrix. No node being actually selected,
//node1 will be the main node and will define the WCS (world coordinate system). As no
//translation vector is given, the crystal will not be translated (translation vector 0
//assumed). As no rotation matrix is given, the crystal will not be rotated and will
//remain with its axis parallel to Oz. node1, the crystal, is selected and will remain the
//reference for all other nodes. For all other nodes, the proper translation vector is
//given. No shape is rotated, no rotation matrix is given.
  TSNode *node1 = new TSNode("node1","node1",crystal);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);
  node1->cd();
//glue
  TSNode *nodeg1 = new TSNode("nodeg1","glue up left",   glue_ul,bpin_x,pin_y,glue_z);
  TSNode *nodeg2 = new TSNode("nodeg2","glue up right",  glue_ur,spin_x,pin_y,glue_z);
  TSNode *nodeg3 = new TSNode("nodeg3","glue down left", glue_dl,bpin_x,pin_y,-glue_z);
  TSNode *nodeg4 = new TSNode("nodeg4","glue down right",glue_dr,spin_x,pin_y,-glue_z);
//windows
  TSNode *nodew1 = new TSNode("nodew1","epoxy up left",   epoxy_ul,bpin_x,pin_y,epoxy_z);
  TSNode *nodew2 = new TSNode("nodew2","epoxy up right",  epoxy_ur,spin_x,pin_y,epoxy_z);
  TSNode *nodew3 = new TSNode("nodew3","epoxy down left", epoxy_dl,bpin_x,pin_y,-epoxy_z);
  TSNode *nodew4 = new TSNode("nodew4","epoxy down right",epoxy_dr,spin_x,pin_y,-epoxy_z);
//PINs
  TSNode *nodep1 = new TSNode("nodep1","pin up left",   pin_ul,bpin_x,pin_y,pin_z);
  TSNode *nodep2 = new TSNode("nodep2","pin up right",  pin_ur,spin_x,pin_y,pin_z);
  TSNode *nodep3 = new TSNode("nodep3","pin down left", pin_dl,bpin_x,pin_y,-pin_z);
  TSNode *nodep4 = new TSNode("nodep4","pin down right",pin_dr,spin_x,pin_y,-pin_z);
//
//  Definitions of all contacts criystal-glue
  TContact *ct_cg1,*ct_cg2,*ct_cg3,*ct_cg4;
  ct_cg1 = new TContact("CsITl_Glue_ul","CsITl_Glue_ul","crystal","glue_ul",containing);
  ct_cg2 = new TContact("CsITl_Glue_ur","CsITl_Glue_ur","crystal","glue_ur",containing);
  ct_cg3 = new TContact("CsITl_Glue_dl","CsITl_Glue_dl","crystal","glue_dl",containing);
  ct_cg4 = new TContact("CsITl_Glue_dr","CsITl_Glue_dr","crystal","glue_dr",containing);
//  Definitions of all contacts glue-window
  TContact *ct_gw1,*ct_gw2,*ct_gw3,*ct_gw4;
  ct_gw1=new TContact("Glue_Window_ul","Glue_Window_ul","glue_ul","epoxy_ul",identical);
  ct_gw2=new TContact("Glue_Window_ur","Glue_Window_ur","glue_ur","epoxy_ur",identical);
  ct_gw3=new TContact("Glue_Window_dl","Glue_Window_dl","glue_dl","epoxy_dl",identical);
  ct_gw4=new TContact("Glue_Window_dr","Glue_Window_dr","glue_dr","epoxy_dr",identical);
//  Definitions of all contacts window-PIN
  TContact *ct_wp1,*ct_wp2,*ct_wp3,*ct_wp4;
  ct_wp1=new TContact("Window_pin_ul","Window_pin_ul","epoxy_ul","pin_ul",identical);
  ct_wp2=new TContact("Window_pin_ur","Window_pin_ur","epoxy_ur","pin_ur",identical);
  ct_wp3=new TContact("Window_pin_dl","Window_pin_dl","epoxy_dl","pin_dl",identical);
  ct_wp4=new TContact("Window_pin_dr","Window_pin_dr","epoxy_dr","pin_dr",identical);
//
//  Defines a particle of type muon, with a fixed momentum distribution : all muons have
//the fixed momentum of 10 Gev/c. Notice that as the mass of the particle is not given,
//the default : mass of the muon, will be taken.
  TSParticle *part = new TSParticle("muon","muon",singlevalued,10.0);
  const Double_t beam_dz =   3.0;
  const Double_t beam_x  = zero;
  const Double_t beam_y  =  -2.0;
  Double_t       beam_z  = -15.0;
  TBeam *pbeam;
  T3Vector center(beam_x,beam_y,beam_z);
//
//  Loop displacing the beam upwards along z
  for (Int_t i=0;i<11;i++) {
    //  for (Int_t i=0;i<1;i++) {
//
//  Defines a beam of particles (muons). Because "crystal" is given as the 4th argu-
//ment of the constructor, all coordinates used for defining the beam are in the local
//coordinate system of the crystal. The beam is coming from a surface which is defined as
//an ellipse, centered at (beam_x,beam_y,beam_z). The constructor invoked here is the
//second constructor, the one best suited when the 2 axis of the emitting surface of the
//beam (here the 2 axis of the ellipse) are orthogonal. 0.8 and 0.4 are the lengths of the
//2 semi-axis of the ellipse. The axis normal to the ellipse ( let us define it as Oz" )
//has the 2 angles (theta3=90,phi3=90), so that it points parallel to the Oy axis, towards
//the crystal, crossing the crystal orthogonally to the long axis of the crystal. The
//large axis of the ellipse ( let us define it as Ox" ) has the 2 angles (theta1=90,
//phi1=0) so that it points along the Ox axis. The small axis of the ellipse, Oy", need
//not be defined because (Ox", Oy", Oz") have to build an orthonormal system. This implies
//here that Oy" = - Oz.
    pbeam = new TBeam("mu_beam","mu_beam",part,"crystal",center,ellipse,
                   0.8,0.4,90.0,90.0,90.0,0.0);
//
//  The instance of the method Beam() called here implies that the direction of the axis
//of emission of the beam is parallel to the axis normal to the emitting surface of the
//beam (the ellipse). If it would not have been the case, we ought to have called the
//other instance of the method Beam(), where direction of emission of the beam has to be
//given. The distribution of emission of the beam is flat, i.e. sin(theta)*dtheta*dphi
//within the aperture of 5 degrees.
    pbeam->Beam(flat,5.0);
//  For CsITl, emission of Cerenkov photons is negligible as compared with dE/dx photons.
//We cancel it.
    pbeam->SetCerenkov(kFALSE);
//
// Start run 100, generating 1 particle (feel free to generate more !).As the last parameter,
//step, is not given, the default value of 0.5 cm will be used. beam_z is xparam, the
//parameter which varies from run to run and will be used as x axis in the histograms
//done by the class TPublication.
    pbeam->Gen(100+i,1,beam_z);
    delete pbeam;
//displaces the beam
    beam_z += beam_dz;
    center.Set(beam_x,beam_y,beam_z);
  }
  gLit->BookCanvas();
  gTwoPad->SelectPad2();
  node1->Draw();
  gTwoPad->ChangePad();
//  Here the generation of photons is finished. It remains to analyze the results.
//For that, you have at hand the 2 classes TResults and TPublication, and all the
//power of ROOT! Look at the web description of the classes Tresults and
//TPublication where you will find a full description of all types of histograms
//available at the end of a production of Litrani. You will learn there that
//for instance gGp is a pointer towards the summary histograms of Litrani. Giving
//to the title of the class TPublication pointed to by gGp "z coordinate of beam"
//will have for result that all the summary histograms will contain this in their
//title. For instance, the histogram on efficiency that we plot here will have as
//title : "Efficiency versus z coordinate of beam".
  gGp->SetTitle("z coordinate of beam");
//Summary() creates all summary histograms
  gGp->Summary();
//Draws one of the available histogram.
  gStyle->SetOptFit(1111);
  gGs->DoStat();
  gLit->CloseFiles();
}
