#ifndef _MYTREE_H_
#define _MYTREE_H_ value

#include <FL/Fl_Tree.H>
#include <FL/Enumerations.H>
#include <FL/Fl.H>

class CurveWindow;

class Mytree : public Fl_Tree
{
public:
  Mytree (int x, int y, int w, int h);
  int handle(int e);
  
  int whichClicked;

private:
  /* data */
};

#endif
