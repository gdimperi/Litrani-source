{
  char *name    = "GLASTWithPMs";
  char *listing = "GLASTWithPMs";
  char *upcom   = "Simulation of a GLAST CsI(Tl) crystal with phototubes";
  char *downcom = "Source of light : beam of muons. Phototube on both ends";
  gROOT->ProcessLine(".x InitLitrani.C(7,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
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
//   GLASTWITHPMS
//
//  GLASTWithPMs.C is an example of a setup with a crystal of CsI(Tl) having the shape of a
//rectangular parallelipiped. All the necessary fits or interpolations are searched on
//the ROOT database file SplineFitDB.rdb. In this example, the material CsI(Tl) is
//optically isotropic.
//  On the upper small face of the crystal is a phototube (shape TSCYL). The contact
//between the crystal and the phototube is by mean of a silicoide pastille (shape
//TSCYL).
//  On the lower small face of the crystal is an other phototube (shape TSCYL). The contact
//between the crystal and the phototube is by mean of a silicoide pastille (shape
//TSCYL). So there are 5 shapes in all for this setup.
//  CsI(Tl) is the only material which is fluorescent, i.e. able to emit light when
//crossed by particles. The other materials are silicoide, dodocal, air.
//  The crystal of CsITl is wrapped with tyvek. The tyvek revetment is well described
//by the fits "RIndexRev_Tyvek" and "IIndexRev_Tyvek".
//  Between the crystal and the tyvek foil is a slice of air. This is why we had to
//introduce material air.
//  The TSCYL made of silicoide is considered to be wrapped with a totally absorbing
//revetment which is called "totabsorbing". Idem for the TSCYL representing the PM.
//  A beam of muons is defined which crosses the crystal of CsITl perpendicularly to the
//long axis of the crystal. The fit "DEdx_MusInCsITlNoL" gives the value
//of the deposited dE/dx per cm as a function of the energy of the muon. Landau
//fluctuations are not taken into account (you can, if you will). The muons have been
//defined has having a fixed momentum of 10 Gev.
//  The beam originates from an ellipse, placed on the left of the crystal, and is displa-
//ced upwards from run to run. The axis of the beam is perpendicular to the ellipse in di-
//rection of the crystal. The aperture of the beam is 5 degrees. The beam crosses the
//crystal orthogonally to the long axis of the crystal.
//  CsITl is slow, so it is a good idea to change the default value of TooLate. According
//to Arache Djannati of the GLAST experiment, the time of the gate of the GLAST experiment
//is 2500 ns, which is quite short for CsI(Tl) ! There will be many photons killed because
//too late !
  TLitPhys::Get()->SetLate(2500);
//  Definition of the material CsITl. The magnetic permeability is declared to be 1, not
//depending upon wavelength. The absorpion length is given by the fit "AbsorptionLength_CsITl".
//CsITl is declared as optically isotropic, the fits giving the variation of the index
//of refraction as a function of wavelength is "RefrIndex_CsITl",
//  The fluorescent characteristics of CsI are taken from :
//     Measurements done by Remi Chipaux from Saclay
//     Measurements done by John E. Eldridge
//  Please choose the one you want.
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
//  Definition of silicoide, with magnetic permeability of 1 and absorption length depending
//upon wavelength according to the fit "AbsorptionLength_Silicoide".
  TOpticMaterial *silicoide;
  silicoide = new TOpticMaterial("Silicoide","Silicoide",kFALSE,1.0,"AbsorptionLength_Silicoide");
  silicoide->IsIsotropic("RefrIndex_Silicoide");
//
//  Definition of the material sodocal, the window of the Phototube. It has an absorption
//length of 1000 cm (we have not been able to find the true values for the absorption
//length of sodocal), a magnetic permeability of 1.0. It is optically isotropic, with an
//index of refraction varying as a function of wavelength by the fit "RefrIndex_Sodocal"
  TOpticMaterial *sodocal;
  sodocal = new TOpticMaterial("Sodocal","Sodocal",kFALSE,1.0,1000.0);
  sodocal->IsIsotropic("RefrIndex_Sodocal");
//
//  Definition of the revetment tyvek, with a slice of Air in-between ( a slice, not a
//thin slice ) with real part and imaginary part of index of refraction varying in
//function of wavelength according to the fits "RIndexRev_Tyvek" and "IIndexRev_Tyvek".
//The magnetic permeability is 1, independent of wavelength. There is no supplementary
//absorption.  The revetment is 98.0% diffusing.
  const Double_t PropDiffus = 0.98;
  TRevetment *tyvek;
  tyvek = new TRevetment("tyvek","tyvek","Air",PropDiffus,
                           "RIndexRev_Tyvek", "IIndexRev_Tyvek",1.0);
//  Bad tyvek is for the revetment covering the chamfers! We give to this
//part of the revetment a supplementary absorption of 5%!
  TRevetment *badtyvek;
  badtyvek = new TRevetment("badtyvek","badtyvek","Air",PropDiffus,
                           "RIndexRev_Tyvek", "IIndexRev_Tyvek",1.0,0.05);
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
//covered with "tyvek", except for the parts of the shape which are in contact with other
//shapes.
  const Double_t zero = 0.0;
  const Double_t crystal_dx = 1.335;  //half width of crystal in Ox direction
  const Double_t crystal_dy = 0.995;  //half width of crystal in Oy direction
  const Double_t crystal_dz = 16.3;   //half length of crystal axis, put here along Oz
  const Double_t silico_dz  = 0.25; //half width of both pastilles in Oz direction.
  const Double_t tube_r     = 2.2;  //radius for both phototubes
  const Double_t tube_dz    = 0.2;  //half width of both phototube windows in Oz direction.
  Double_t silico1_z =   crystal_dz + silico_dz; //shift in z for 1st pastille
  Double_t tube1_z   = silico1_z + silico_dz + tube_dz;
  Double_t silico2_z = - crystal_dz - silico_dz; //shift in z for 2nd pastille
  Double_t tube2_z   = silico2_z - silico_dz - tube_dz;
//
  TSBRIK *crystal;
  crystal = new TSBRIK("crystal","GLAST crystal","CsITl","tyvek",
                        crystal_dx,crystal_dy,crystal_dz);
//Setting depolish and bevellings
  const Double_t ThetaDepls   = 31.48;
  const Double_t ThetaDeplsbv = 50.0;
  TSupplShape *ps = crystal->fSuppl;
  ps->SetBevellings(kTRUE,0.1,"badtyvek",ThetaDeplsbv);
  TFace *q0,*q1,*q2,*q3;
  q0 = ps->GetFace(0);
  q1 = ps->GetFace(1);
  q2 = ps->GetFace(2);
  q3 = ps->GetFace(3);
  q0->SetDepolished(ThetaDepls);
  q1->SetDepolished(ThetaDepls);
  q2->SetDepolished(ThetaDepls);
  q3->SetDepolished(ThetaDepls);
//
//Definition of the 2 pastilles
  TSCYL *psilic1;
  psilic1 = new TSCYL("Pastille1","Pastille1","Silicoide","TotAbsorbing",tube_r,silico_dz);
  TSCYL *psilic2;
  psilic2 = new TSCYL("Pastille2","Pastille2","Silicoide","TotAbsorbing",tube_r,silico_dz);
//
//Definition of the window of the 2 PMs
  TSCYL *pm1;
  pm1 = new TSCYL("Phototube1","Phototube1","Sodocal","TotAbsorbing",tube_r,tube_dz);
  TSCYL *pm2;
  pm2 = new TSCYL("Phototube2","Phototube2","Sodocal","TotAbsorbing",tube_r,tube_dz);
//Declares that the shapes pm1 and pm2 are a phototubes.
//  that the upper face (1) is the photocathode for pm1,
//  that the lower face (0) is the photocathode for pm2.
  pm1->fSuppl->SetPhotoTube("XP2020_1","Phototube Phillips XP2020",1,"QuantumEff_XP2020");
  pm2->fSuppl->SetPhotoTube("XP2020_2","Phototube Phillips XP2020",0,"QuantumEff_XP2020");
//use different colors
  crystal->SetLineColor(4);
  crystal->SetLineWidth(2);
  psilic1->SetLineColor(2);
  psilic1->SetLineWidth(2);
  psilic2->SetLineColor(2);
  psilic2->SetLineWidth(2);
  pm1->SetLineColor(28);
  pm1->SetLineWidth(2);
  pm2->SetLineColor(28);
  pm2->SetLineWidth(2);
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
//the 2 pastilles
  TSNode *node2;
  node2 = new TSNode("node2","node2",psilic1,zero,zero,silico1_z);
  TSNode *node4;
  node4 = new TSNode("node4","node4",psilic2,zero,zero,silico2_z);
//the 2 windows of PM
  TSNode *node3;
  node3 = new TSNode("node3","node4",pm1,zero,zero,tube1_z);
  TSNode *node5;
  node5 = new TSNode("node5","node5",pm2,zero,zero,tube2_z);
//
//  Definitions of all contacts criystal-silicoide
//
//  There is a contact between crystal shape and up pastille. It is face 5 of crystal
//which is in contact with face 0 of the pastille. Face 5 of crystal is smaller than face 0 of
//pastille and is entirely contained into it. Hence "contained".
  TContact *contact12;
  contact12 = new TContact("cr_pas1","cr_pas1","crystal","Pastille1",contained);
//
//  There is a contact between pastille1 and pm1 shape. It is face 1 of pastille which
//is in contact with face 0 of pm1. Face 1 of pastille is of the same size as face 0
//of pm1. Hence "identical".
  TContact *contact23;
  contact23 = new TContact("past1_pm1","past1_pm1","Pastille1","Phototube1",identical);
//
//  There is a contact between crystal shape and down pastille. It is face 4 of crystal
//which is in contact with face 1 of the pastille. Face 4 of crystal is smaller than face 1 of
//pastille and is entirely contained into it. Hence "contained".
  TContact *contact14;
  contact14 = new TContact("cr_pas2","cr_pas2","crystal","Pastille2",contained);
//
//  There is a contact between pastille2 and pm2 shape. It is face 0 of pastille2 which
//is in contact with face 1 of pm2. Face 0 of pastille2 is of the same size as face 1
//of pm2. Hence "identical".
  TContact *contact45;
  contact45 = new TContact("past2_pm2","past2_pm2","Pastille2","Phototube2",identical);
//
//  Defines a particle of type muon, with a fixed momentum distribution : all muons have
//the fixed momentum of 10 Gev/c. Notice that as the mass of the particle is not given,
//the default : mass of the muon, will be taken.
  TSParticle *part = new TSParticle("muon","muon",singlevalued,10.0);
  const Double_t beam_dz = 3;
  const Double_t beam_x = zero;
  const Double_t beam_y = -2.0;
  Double_t       beam_z = -15.0;
  TBeam *pbeam;
  T3Vector center(beam_x,beam_y,beam_z);
//
//  Loop displacing the beam upwards along z
  gLit->SetPrintFreq(2000); //set print frequency on new photons
  for (Int_t i=0;i<11;i++) {
//
//  Defines a beam of particles part (muons). Because "crystal" is given as the 4th argu-
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
// Start run 100, generating 1 particle (feel free to generate more !). As the last parameter,
//step, is not given, the default value of 0.5 cm will be used. beam_z is xparam, the
//parameter which varies from run to run and will be used as x axis in the histograms
//done by the class TPublication.
    pbeam->Gen(100+i,1,beam_z);
    delete pbeam;
//displaces the beam
    beam_z += beam_dz;
    center.Set(beam_x,beam_y,beam_z);
  }
//
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
