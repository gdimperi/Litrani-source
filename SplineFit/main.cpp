#include "TRint.h"
int main(int argc, char **argv)
{
 TRint *theApp = new TRint("ROOT example", &argc, argv, NULL, 0);
 theApp->Run();
 return(0);
}
