#include "curveCombine.h"
#include <FL/Fl.H>

int main(int argc, char **argv) 
{
  CurveCombineUI *ccui = new CurveCombineUI;

  ccui->show(argc, argv);

  return Fl::run();
}
