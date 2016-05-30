{
  gROOT->ProcessLine(".x InitLitrani.C");
//
//  This macro reads the .root file produced by Litrani and containing all the
//statistics and histograms. It is much handier to use "VisuLitrani" than
//this macro! Have a look at "VisuLitrani"!
//
  char buffer[100];
  TString sname;
  Bool_t fnd;
  Int_t irun;
  cout << "Name of .root file (without .root) : ";
  cin.get(buffer,99);
  sname = buffer;
  cout << sname.Data() << endl;
  cout << "Run you want to see : ";
  cin  >> irun;
  gLit->NameIOFile(sname.Data());
  gLit->OpenFilesR();
  fnd = gLit->SelectRun(irun);        //wants to look at run irun
  if (fnd) {
    cout << "Showing all histograms of run " << irun << endl;
    gCs->N();  //Looks at all histograms of run irun
  }
  else cout << "run " << irun << " not found" << endl;
  cout << "Showing histograms sum of all runs" << endl;
  gGs->N();           //Looks at all histograms for the sum of all runs.
  if (gGp) {
    cout << "Showing histograms of results versus run parameter" << endl;
    gGp->N();
  }
  char c;
  cout << "Do you want to close the file ? ";
  cin  >> c;
  if (c != 'n') gLit->CloseFilesR();
}
