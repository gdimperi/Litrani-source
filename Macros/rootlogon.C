{
   cout << endl << "Welcome to the ROOT framewok" << endl << endl;
   cout << "Type \".x benchmarks.C\" to check Root" << endl;
   cout << "Type \".x LitDemos.C\"   to check Litrani" << endl << endl;
   ((TRint*)gROOT->GetApplication())->SetPrompt("==> [%d] ");
}

