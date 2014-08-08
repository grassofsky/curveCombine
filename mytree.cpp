#include "mytree.h"
//#include "plotbox.h"
#include "FL/Enumerations.H"
#include "curveCombine.h"

Mytree::Mytree (int x, int y, int w, int h) 
  : Fl_Tree(x,y,w,h)
{
  whichClicked = 0;
}

int Mytree::handle(int e) {
  CurveCombineUI *ui = (CurveCombineUI*)(parent()->user_data());
  if (e == FL_KEYBOARD)
  {
    if (Fl::event_key() == FL_Delete)
    {
      Fl_Tree_Item *item = this->callback_item();
      this->remove(item);
      this->redraw();
      ui->plotBox->dataChanged = true;
      ui->plotBox->redraw();
    }
  }
  
  return (Fl_Tree::handle(e));
}

