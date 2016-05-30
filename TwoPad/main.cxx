#include <TSystem.h>
#include "TRint.h"
#include "TGRootIDE.h"
int main(int argc, char **argv)
{
  TString *fname = 0;
  Int_t i;
  for (i=0;i<argc;i++) {
    if ((i > 0) && (!gSystem->AccessPathName(argv[i]))) {
      fname = new TString(argv[i]);
      // don't pass filename arg to TRint (avoid processing file)
      argv[i] = 0;
      argc--;
    }
  }
  TRint *theApp = new TRint("ROOT example", &argc, argv, NULL, 0);
  if (fname) new TGRootIDE(fname->Data());
  theApp->Run();
  return(0);
}
