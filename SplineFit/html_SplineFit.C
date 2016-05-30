{
  THtml html;
  html.SetInputDir(".");
  //  html.SetSourceDir(".");
  html.SetOutputDir("./html");
  html.SetProductName("SplineFit");
  html.MakeAll(kTRUE);
}
