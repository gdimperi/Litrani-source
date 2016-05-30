// This macro rebuilds the file SplineFitDB.rdb
//
{
  Int_t k,error,nbfit;
  Bool_t ok;
  TSplineFit *fit = 0;
  nbfit = 0;
  //
  k = gROOT->LoadMacro("RefrIndex_CsITl.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RefrIndex_CsITl(kFALSE,kTRUE,kTRUE);
  nbfit++;
  cout << nbfit << "   RefrIndex_CsITl" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RefrIndex_hcp200.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RefrIndex_hcp200(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RefrIndex_hcp200" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RefrIndex_Meltmount.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RefrIndex_Meltmount(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RefrIndex_Meltmount" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RefrIndex_Nitrure.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RefrIndex_Nitrure(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RefrIndex_Nitrure" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RefrIndex_Plexi.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RefrIndex_Plexi(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RefrIndex_Plexi" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RefrIndex_Q23067.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RefrIndex_Q23067(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RefrIndex_Q23067" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RefrIndex_RTV3145.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RefrIndex_RTV3145(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RefrIndex_RTV3145" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RefrIndex_Silicium.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RefrIndex_Silicium(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RefrIndex_Silicium" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RefrIndex_Silicoide.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RefrIndex_Silicoide(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RefrIndex_Silicoide" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RefrIndex_Sodocal.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RefrIndex_Sodocal(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RefrIndex_Sodocal" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("DielTensor_PbWO4_000.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = DielTensor_PbWO4_000(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   DielTensor_PbWO4_000" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("DielTensor_PbWO4_001.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = DielTensor_PbWO4_001(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   DielTensor_PbWO4_001" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("DielTensor_PbWO4_002.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = DielTensor_PbWO4_002(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   DielTensor_PbWO4_002" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RIndexRev_Aluminium.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RIndexRev_Aluminium(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RIndexRev_Aluminium" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RIndexRev_CMSalveola.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RIndexRev_CMSalveola(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RIndexRev_CMSalveola" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RIndexRev_CMScapsula.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RIndexRev_CMScapsula(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RIndexRev_CMScapsula" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RIndexRev_CMSferule.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RIndexRev_CMSferule(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RIndexRev_CMSferule" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RIndexRev_Tyvek.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RIndexRev_Tyvek(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RIndexRev_Tyvek" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RIndexRev_vm2000.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RIndexRev_vm2000(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RIndexRev_vm2000" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("IIndexRev_Aluminium.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = IIndexRev_Aluminium(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   IIndexRev_Aluminium" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("IIndexRev_CMSalveola.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = IIndexRev_CMSalveola(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   IIndexRev_CMSalveola" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("IIndexRev_CMScapsula.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = IIndexRev_CMScapsula(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   IIndexRev_CMScapsula" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("IIndexRev_CMSferule.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = IIndexRev_CMSferule(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   IIndexRev_CMSferule" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("IIndexRev_Tyvek.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = IIndexRev_Tyvek(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   IIndexRev_Tyvek" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("IIndexRev_vm2000.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = IIndexRev_vm2000(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   IIndexRev_vm2000" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_crys8981R01.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_crys8981R01(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_crys8981R01" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_crys8981R01b.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_crys8981R01b(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_crys8981R01b" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_crys8981R02.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_crys8981R02(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_crys8981R02" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_crys8981R03.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_crys8981R03(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_crys8981R03" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_crys8981R04.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_crys8981R04(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_crys8981R04" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_crys8981R05.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_crys8981R05(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_crys8981R05" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_crys8981R06.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_crys8981R06(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_crys8981R06" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_crys8981R07.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_crys8981R07(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_crys8981R07" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_CsITl.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_CsITl(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_CsITl" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_DC93500.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_DC93500(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_DC93500" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_Meltmount1704.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_Meltmount1704(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_Meltmount1704" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_PbWO4.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_PbWO4(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_PbWO4" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_Q23067.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_Q23067(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_Q23067" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_Silicium.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_Silicium(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_Silicium" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsorptionLength_Silicoide.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsorptionLength_Silicoide(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsorptionLength_Silicoide" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsLengthTensor_PbWO4_000.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsLengthTensor_PbWO4_000(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsLengthTensor_PbWO4_000" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsLengthTensor_PbWO4_001.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsLengthTensor_PbWO4_001(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsLengthTensor_PbWO4_001" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsLengthTensor_PbWO4_002.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsLengthTensor_PbWO4_002(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsLengthTensor_PbWO4_002" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("QuantumEff_HVPT.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = QuantumEff_HVPT(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   QuantumEff_HVPT" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("QuantumEff_XP2020.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = QuantumEff_XP2020(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   QuantumEff_XP2020" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("DEdx_MusInCsITlNoL.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = DEdx_MusInCsITlNoL(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   DEdx_MusInCsITlNoL" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("DEdx_MusInPbWO4_000.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = DEdx_MusInPbWO4_000(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   DEdx_MusInPbWO4_000" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("DEdx_MusInPbWO4_001.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = DEdx_MusInPbWO4_001(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   DEdx_MusInPbWO4_001" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("DEdx_MusInPbWO4NoL.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = DEdx_MusInPbWO4NoL(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   DEdx_MusInPbWO4NoL" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("DEdx_MusInPbWO4PDB.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = DEdx_MusInPbWO4PDB(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   DEdx_MusInPbWO4PDB" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AngularDistrib_FibreCMS.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AngularDistrib_FibreCMS(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AngularDistrib_FibreCMS" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("GainProfile_CMSAPD.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = GainProfile_CMSAPD(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   GainProfile_CMSAPD" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("GainProfile_GLASTpin.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = GainProfile_GLASTpin(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   GainProfile_GLASTpin" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RadDamage_GzPbWO4.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RadDamage_GzPbWO4(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RadDamage_GzPbWO4" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RadDamage_Kim01.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RadDamage_Kim01(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RadDamage_Kim01" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RadDamage_Kim02.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RadDamage_Kim02(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RadDamage_Kim02" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RadDamage_Kim03.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RadDamage_Kim03(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RadDamage_Kim03" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RadDamage_Kim04.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RadDamage_Kim04(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RadDamage_Kim04" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RadDamage_Kim05.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RadDamage_Kim05(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RadDamage_Kim05" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RadDamage_Kim06.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RadDamage_Kim06(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RadDamage_Kim06" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RadDamage_Kimax02.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RadDamage_Kimax02(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RadDamage_Kimax02" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("RadDamage_KimaxPbWO4.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = RadDamage_KimaxPbWO4(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   RadDamage_KimaxPbWO4" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("Spectrum_In6WOTb.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = Spectrum_In6WOTb(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   Spectrum_In6WOTb" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("Spectrum_PbWO4La.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = Spectrum_PbWO4La(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   Spectrum_PbWO4La" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("Spectrum_PbWO4Nb.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = Spectrum_PbWO4Nb(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   Spectrum_PbWO4Nb" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("NPLY_CaWO4.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = NPLY_CaWO4(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   NPLY_CaWO4" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("PhotoEl_CaWO4.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = PhotoEl_CaWO4(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   PhotoEl_CaWO4" << " in SplineFitDB.rdb" << endl;
  delete fit;
  //
  k = gROOT->LoadMacro("AbsLengthWLS_Fibre.C",&error,kFALSE);
  if (k) gApplication->Terminate();
  fit = AbsLengthWLS_Fibre(kFALSE,kTRUE,kFALSE);
  nbfit++;
  cout << nbfit << "   AbsLengthWLS_Fibre" << " in SplineFitDB.rdb" << endl;
  delete fit;
  cout << "End of entering fits in file" << endl;
  ok = TSplineFit::OrderFile(kTRUE);
  if (ok) cout << "SplineFitDB.rdb has been correctly ordered" << endl;
  else    cout << "Problem in ordering SplineFitDB.rdb" << endl;
}
