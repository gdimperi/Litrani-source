{
  TString s;
  THtml html;
  html.SetInputDir(".");
  //  html.SetSourceDir(".");
  html.SetOutputDir("html");
  html.SetProductName("litrani");
  html.MakeAll(kTRUE);
}
