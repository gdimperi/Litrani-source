{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Double_t   un = 1.0;
  const Int_t      nx = 40;
  const Int_t      ny = 40;
  const Double_t xmin = 0.0;
  const Double_t xmax = 1.0;
  const Double_t ymin = 0.0;
  const Double_t ymax = 1.0;
  const Double_t sig2 = 0.15;
  const Double_t a2   = 2.0;
  const Double_t sig3 = 0.04;
  const Double_t a3   = 2.0;
  Double_t chi2;
  Double_t p1x,p1y,p2x,p2y,p3x,p3y;
  Double_t d2x,d2y,s22;
  Double_t d3x,d3y,s32;
  s22 = sig2*sig2;
  s32 = sig3*sig3;
  char c;
  Int_t i,j,k;
  TAxis *axisx, *axisy;
  Double_t x,y,v;
  TH2D *h;
  h = new TH2D("miscellaneous_example2D","miscellaneous | Example of 2D fit",nx,xmin,xmax,ny,ymin,ymax);
  axisx = h->GetXaxis();
  axisy = h->GetYaxis();
  p1x = xmin;
  p1y = ymax;
  p2x = xmin + 0.75*(xmax-xmin);
  p2y = ymin + 0.75*(ymax-ymin);
  p3x = xmin + 0.75*(xmax-xmin);
  p3y = ymin + 0.25*(ymax-ymin);
  for (j=1;j<=ny;j++) {
    y   = axisy->GetBinCenter(j);
    for (i=1;i<=nx;i++) {
      k   = h->GetBin(i,j);
      x   = axisx->GetBinCenter(i);
      v   = TMath::Exp(p1x - x  - p1y + y);
      d2x = x-p2x;
      d2y = y-p2y;
      v  += a2*TMath::Exp(-(d2x*d2x + d2y*d2y)/s22);
      d3x = x-p3x;
      d3y = y-p3y;
      v  += a3*TMath::Exp(-(d3x*d3x + d3y*d3y)/s32);
      h->SetBinContent(k,v);
      h->SetBinError(k,TMath::Sqrt(TMath::Abs(v)));
    }
  }
  TSplineFit *fit;
  fit = new TSplineFit("miscellaneous_example2D","miscellaneous | Example of 2D fit",0,3,h);
  fit->SetSource("Selfconstructed");
  fit->SetMacro("Example2D.C");
  fit->SetXLabel("x axis");
  fit->SetYLabel("y axis");
  fit->SetVLabel("z axis");
  fit->SetDefaultLabels();
  fit->DrawData();
  cout << "type a char to continue : ";
  cin >> c;
  fit->DrawFit("LEGO2",1,1);
  chi2 = fit->Chi2();
  cout << "chi2 = " << chi2 << endl;  
}
