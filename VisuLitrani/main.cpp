//#include "TRint.h"
#include "TMath.h"
#include "TRint.h"
#include "TGClient.h"
#include "TVisuLit.h"

int main(int argc, char **argv)
{
  UInt_t w,h;
  Double_t h1,h2;
  TVisuLit *mainWin;
  TRint theApp("App", &argc, argv);
  w  = gClient->GetDisplayWidth();
  h  = gClient->GetDisplayHeight();
  w -= 200;
  h1 = h;
  h2 = ((TMath::Sqrt(5.0) - 1.0)/2.0)*w;
  h  = (UInt_t)TMath::Min(h1,h2);
  mainWin = new TVisuLit(gClient->GetRoot(), w, h);
  theApp.Run();
  return 0;
}
