{
//  miscellaneous_poly3
//
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Double_t zero      = 0.0;
  const Int_t    NbSamp    = 10;
  const Int_t    Skipped   = 3;
  const Int_t    m         = 2;
  const Double_t StartTime = 0.0;
  const Double_t StepTime  = 25.0;
  const Double_t startrise = 0.1;
  const Double_t stoprise  = 0.9;
  Double_t StopTime,StartFit,StopFit,tstart;
  StopTime = StartTime + NbSamp*StepTime;
  StartFit = StartTime + (Skipped + 0.5)*StepTime;
  StopFit  = StopTime  - 0.5*StepTime;
  tstart   = StartFit;
  Bool_t ok1,ok2;
  Double_t S,ped,xStart,yStart,xStop,yStop,yCheck;
  Bool_t ismin,ismax;
  Double_t xMin,yMin,xMax,yMax;
  Int_t i;
  TSplineFit *poly3;
  Double_t x[NbSamp-Skipped];
  for (i=Skipped;i<NbSamp;i++) x[i-Skipped] = tstart + (i-Skipped)*StepTime;
  Double_t ys[Skipped]       = { 171,    170,    170 };
  Double_t y[NbSamp-Skipped] = { 170,    238,   1430,   2271,   2239,   1842,   1397 };
  Double_t ss[NbSamp-Skipped];
  for (i=Skipped;i<NbSamp;i++) ss[i-Skipped] = TMath::Sqrt(y[i-Skipped]);
  //  for (i=Skipped;i<NbSamp;i++) ss[i-Skipped] = 1.0;
  poly3 = new TSplineFit("miscellaneous_poly4","miscellaneous | other test of class TPoly3",
                           0,NbSamp-Skipped,m,x,y,ss,kFALSE,0.0,kFALSE,1500.0,StartFit,StopFit);
  poly3->SetSource("F.X.Gentit");
  poly3->SetMacro("miscellaneous_poly4");
  poly3->SetXLabel("time sample");
  poly3->SetVLabel("ADC value");
  poly3->SetDefaultLabels();
  poly3->DrawFit();
  poly3->Print();
  S =  poly3->Integral(StartFit,StopFit);
  cout << "Integral: " << S << endl;
  ped = zero;
  for (i=0;i<Skipped;i++) ped += ys[i];
  ped /= Skipped;
  cout << "pedestal: " << ped << endl;
  poly3->MinMax(ismin,xMin,yMin,ismax,xMax,yMax);
  cout << "Minimum: " << ismin << "    xMin: " << xMin << "    yMin: " << yMin << endl;
  cout << "Maximum: " << ismax << "    xMax: " << xMax << "    yMax: " << yMax << endl;
  yStart = ped + startrise*(yMax-ped);
  yStop  = ped + stoprise*(yMax-ped);
  ok1 = poly3->SolveLeft(xStart,yStart);
  ok2 = poly3->SolveLeft(xStop,yStop);
  if (ok1) {
    yCheck = poly3->V(xStart);
    cout << "xStart: " << xStart << "    yStart: " << yStart << "    yCheck: " << yCheck << endl;
  }
  else cout << "No start of pulse found" << endl;
  if (ok2) {
    yCheck = poly3->V(xStop);
    cout << "xStop : " << xStop  << "    yStop : " << yStop  << "    yCheck: " << yCheck << endl;
  }
  else cout << "No end of pulse found" << endl;
}
