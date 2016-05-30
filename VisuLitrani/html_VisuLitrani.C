{
  gSystem.Load("libVisuLitrani");
  THtml html;
  html.SetInputDir(".");
  //  html.SetSourceDir(".");
  html.SetOutputDir("html");
  html.SetProductName("VisuLitrani");
  html.MakeAll(kTRUE);
}
