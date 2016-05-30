{
  TSplineFit *fit;
  fit = TSplineFit::FindFit("AbsorptionLength_crys8981R01",5);
  fit = TSplineFit::FindFit("AngularDistrib_FibreCMS",12);
  fit = TSplineFit::FindFit("DEdx_MusInCsITlNoL",11);
  fit = TSplineFit::FindFit("IIndexRev_Aluminium",4);
  //  fit = TSplineFit::FindFit();
  TSplineFit::InitTwoPad();
}
