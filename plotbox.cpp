#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Float_Input.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Menu_Item.H>
#include <string>
#include <vector>
#include <algorithm>
#include "point.h"
#include "mytree.h"
#include "plotbox.h"
#include "curveCombine.h"
#include "tools.h"
#include "stdlib.h"

bool Point_bl(const Point&a, const Point &b);

const int NLINESMAX = 7;
PlotBox::PlotBox(int x, int y, int w, int h) 
  : Fl_Box(x, y, w, h) {
  double lr = 0.05;
  m_x0 = x + w*lr;
  m_y0 = y + h*(1-lr);
  m_xl = w - 2*w*lr;
  m_yl = h - 2*h*lr;
  //isExe = false;
  lineColor[0] = FL_BLACK;
  lineColor[1] = FL_RED;
  lineColor[2] = FL_GREEN;
  lineColor[3] = FL_YELLOW;
  lineColor[4] = FL_BLUE;
  lineColor[5] = FL_MAGENTA;
  lineColor[6] = FL_CYAN;

  iniValues();
}

void PlotBox::iniValues() {
  dataChanged = false;
  yLimitChanged = true;
  showPoint = false;
  nLines = 0;
  whichLow = -1;
  iniDeleteValue();
  iniShiftValue();  

  for (int i=0; i<NLINESMAX; ++i) {
    line[i].clear();
  }
}

void PlotBox::resetValues() {
  dataChanged = true;
  yLimitChanged = true;
  nLines = 0;
  whichLow = -1;
  iniDeleteValue();
  iniShiftValue();
}

void PlotBox::iniShiftValue()
{
  for (int i=0; i<NLINESMAX; ++i) {
    xShift[i] = 0.0;
    yShift[i] = 0.0;
  }
}

void PlotBox::iniDeleteValue()
{
  for (int i=0; i<NLINESMAX; ++i) {
    startX[i] = 1000000.0;
    endX[i] = -1000000.0;
  }
}

void PlotBox::deleteValue() {
  if (showPoint) {
    Mytree *tree = ((CurveCombineUI*)(parent()->user_data()))->tree;
    int whichClicked = tree->whichClicked;
    std::vector<Point>::iterator iterm1 = line[whichClicked].begin();
    std::vector<Point>::iterator iterm2 = line[whichClicked].begin();
    bool in = true; // if the data is begger than startx
    bool out = true; // if the data is bigger than endx
    std::vector<Point>::iterator iterm;
    for (iterm=line[whichClicked].begin(); iterm!=line[whichClicked].end(); ++iterm) {
      if (out && iterm->m_x + xShift[whichClicked] < endX[whichClicked]) {
        iterm1 = iterm;
        out = false;
      }

      if (in && iterm->m_x + xShift[whichClicked] < startX[whichClicked]) {
        iterm2 = iterm;
        in = false;
      }

      if (!in && !out) {
        break;
      }
    }

    if (!out && in) {
      iterm2 = iterm;
      in = false;
    }

    if (iterm1 != iterm2) {
      line[whichClicked].erase(iterm1,iterm2);
    }
  }
}

int PlotBox::handle(int e)
{
  int ret = Fl_Box::handle(e);
  char buff[255];
  int x, y;
  double dbX, dbY;
  std::string str;

  switch (e) 
  {
    case FL_DND_ENTER:
    case FL_DND_DRAG:
    case FL_DND_RELEASE:
      ret = 1;
      break;
    case FL_PASTE:
      {
        str = const_cast<char*>(Fl::event_text());
        makePath(str);
        Mytree *mytree = ((CurveCombineUI*)(parent()->user_data()))->tree;
        mytree->add(str.c_str());

        mytree->redraw();
        dataChanged = true;
        showPoint = true;
        redraw();
      }
      ret = 1;
      break;
    case FL_ENTER:
      ret = 1;
      break;
    case FL_MOVE:
      if (showPoint) {
        x = Fl::event_x();
        y = Fl::event_y();
        dbX = (x-m_x0)/x_rate + x0;
        dbY = (y-m_y0)/y_rate + y0;
        sprintf(buff, "%.2f,%.2f\0", dbX, dbY);
        CurveCombineUI *ui = (CurveCombineUI*)(parent()->user_data());
        Fl_Box *box = ui->bxXY;
        box->position(x-80,y-20);
        box->label(buff);
        redraw();
        box->show();
      }
      ret = 1;
      break;
    case FL_LEAVE:
      if (showPoint) {
        CurveCombineUI *ui = (CurveCombineUI*)(parent()->user_data());
        Fl_Box *box = ui->bxXY;
        Fl_Double_Window *win = ui->mainWindow;
        box->hide();
        redraw();
        win->redraw();
      }
      ret = 1;
      break;
    case FL_PUSH:
      if (showPoint) {
        // right mouse pushed? popup menu on right click
        if (Fl::event_button() == FL_RIGHT_MOUSE) {
          Fl_Menu_Item rclick_menu[] = {
            {"Left", 0, cb_left, (void*)this},
            //{"Cross", 0, cb_cross, (void*)this},
            {"Right", 0, cb_right, (void*)this},
            {"Clear Left", 0, cb_clear_left, (void*)this},
            //{"Clear Cross", 0, cb_clear_cross, (void*)this},
            {"Clear Right", 0, cb_clear_right, (void*)this},
            {0}
          };
          x = Fl::event_x();
          y = Fl::event_y();
          chooseX = (x-m_x0)/x_rate + x0;
          chooseY = (y-m_y0)/y_rate + y0;         
          const Fl_Menu_Item *m = rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
          if (m) m->do_callback(0, m->user_data());
          ret = 1;
        }
        if (Fl::event_button() == FL_LEFT_MOUSE) {
          Mytree *tree = ((CurveCombineUI*)(parent()->user_data()))->tree;
          startX[tree->whichClicked] = (Fl::event_x()-m_x0)/x_rate + x0;
        }
      }
      break;
    case FL_RELEASE:
      if (showPoint) {
        // Right mouse released? Mask it from PlotBox
        if (Fl::event_button() == FL_RIGHT_MOUSE) {
          ret = 1;
        }
        if (Fl::event_button() == FL_LEFT_MOUSE) {
          Mytree *tree = ((CurveCombineUI*)(parent()->user_data()))->tree;
          int whichClicked = tree->whichClicked;
          endX[whichClicked] = (Fl::event_x()-m_x0)/x_rate + x0;
          if (startX [whichClicked]> endX[whichClicked]) {
            double tmp = startX[whichClicked];
            startX[whichClicked] = endX[whichClicked];
            endX[whichClicked] = tmp;
          }

          deleteValue();
          ret = 1;
          redraw();
        }
      }
      break;
    case FL_DRAG:
      if (showPoint) {
        if (Fl::event_button() == FL_LEFT_MOUSE) {
          ret = 1;
        }
      }
      break;
  }
  return ret;
}

void PlotBox::cb_left(Fl_Widget*, void *userdata) {
  PlotBox *pb = (PlotBox*)userdata;
  CurveCombineUI *w = (CurveCombineUI*)(pb->parent()->user_data());
  setEditValue(w->edtXLeft, pb->chooseX);
  setEditValue(w->edtYLeft, pb->chooseY);
}

void PlotBox::cb_cross(Fl_Widget*, void *userdata) {
  PlotBox *pb = (PlotBox*)userdata;
  CurveCombineUI *w = (CurveCombineUI*)(pb->parent()->user_data());
  setEditValue(w->edtXCross, pb->chooseX);
  setEditValue(w->edtYCross, pb->chooseY);
}

void PlotBox::cb_right(Fl_Widget*, void *userdata) {
  PlotBox *pb = (PlotBox*)userdata;
  CurveCombineUI *w = (CurveCombineUI*)(pb->parent()->user_data());
  setEditValue(w->edtXRight, pb->chooseX);
  setEditValue(w->edtYRight, pb->chooseY);
}

void PlotBox::cb_clear_left(Fl_Widget*, void *userdata) {
  PlotBox *pb = (PlotBox*)userdata;
  CurveCombineUI *w = (CurveCombineUI*)(pb->parent()->user_data());
  w->edtXLeft->value("");
  w->edtYLeft->value("");
}

void PlotBox::cb_clear_cross(Fl_Widget*, void *userdata) {
  PlotBox *pb = (PlotBox*)userdata;
  CurveCombineUI *w = (CurveCombineUI*)(pb->parent()->user_data());
  w->edtXCross->value("");
  w->edtYCross->value("");
}

void PlotBox::cb_clear_right(Fl_Widget*, void *userdata) {
  PlotBox *pb = (PlotBox*)userdata;
  CurveCombineUI *w = (CurveCombineUI*)(pb->parent()->user_data());
  w->edtXRight->value("");
  w->edtYRight->value("");
}

void PlotBox::readData()
{
  CurveCombineUI *w = (CurveCombineUI*)(parent()->user_data());
  std::string tmpFilename;
  for (int i=0; i<NLINESMAX; ++i) {
    line[i].clear();
  }
  int i = 0;
  for (Fl_Tree_Item *item = w->tree->first(); item && i < NLINESMAX; item = w->tree->next(item), ++i)
  {
    tmpFilename = const_cast<char*>(item->label());
    if (tmpFilename == "ROOT") continue;
    else
      readfile(tmpFilename.c_str(), line[i-1]);
  }
}

void PlotBox::getNLines()
{
  int i;
  for (i=6; i>=0; --i) {
    if (!line[i].empty()) {
      nLines = i+1;
      break;
    }
  }
  
  if (i == -1) {
    nLines = 0;
  }
}

void PlotBox::getXLimitsRates()
{
  std::vector<double> xs;
  for (int i=0; i<7; ++i) {
    if (!line[i].empty()) {
      xs.push_back(line[i].begin()->m_x);
      xs.push_back((line[i].end()-1)->m_x);
    }
    else
      break;
  }
  std::sort(xs.begin(),xs.end());
  
  x1 = *(xs.end()-1);
  x0 = *(xs.begin());
  x_rate = m_xl/(x1 - x0);

}

void PlotBox::getYLimitsRates() {
  CurveCombineUI *ui = (CurveCombineUI*)(parent()->user_data());
  y0 = getEditValue(ui->edtYLow);
  y1 = getEditValue(ui->edtYHigh);

  y_rate = -1.0*m_yl/(y1 - y0);
}

void PlotBox::setXSliderValue()
{
  CurveCombineUI *ui = (CurveCombineUI*)(parent()->user_data());
  ui->sldX->bounds(-(int)(x1-x0)*2/3,(int)(x1-x0)*2/3);
  ui->sldX->step((int)(x1-x0)/100.0);

  for (int i=0; i<NLINESMAX; ++i) {
    xShift[i] = 0.0;
  }

  ui->sldX->value(0.0);
  ui->sldX->activate();
}

void PlotBox::setYSliderValue()
{
  CurveCombineUI *ui = (CurveCombineUI*)(parent()->user_data());

  ui->sldY->bounds(-(int)(y1-y0)/2,(int)(y1-y0)/2);
  ui->sldY->step((int)(y1-y0)/200.0);

  for (int i=0; i<NLINESMAX; ++i) {
    yShift[i] = 0.0;
  }

  ui->sldY->value(0.0);

  ui->sldY->activate();
}

void PlotBox::addLines() {
  CurveCombineUI *ui = (CurveCombineUI*)(parent()->user_data());

  int whichClicked = ui->tree->whichClicked;
  double x0, y0, xc, yc, x1, y1;
  double step = 0.5;
  Point p;
    
  x0 = getEditValue(ui->edtXLeft);
  y0 = getEditValue(ui->edtYLeft);
  //xc = getEditValue(ui->edtXCross);
  //yc = getEditValue(ui->edtYCross);
  x1 = getEditValue(ui->edtXRight);
  y1 = getEditValue(ui->edtYRight);

  for (double i = x0+step; i < x1; i += step) {
    p.m_x = i;
    p.m_y = (i-x1)*(y0-y1)/(x0-x1) + y1;
    line[whichClicked].push_back(p);
  }
}

void PlotBox::draw()
{
  Fl_Box::draw();

  // prepare for plots
  if (dataChanged) {
    readData();
    getNLines();
    getXLimitsRates();
    setXSliderValue();
    dataChanged = false;
  }
  
  if (yLimitChanged) {
    getYLimitsRates();
    setYSliderValue();
    yLimitChanged = false;
  }

  plotLines();
}

void PlotBox::plotLines()
{
  /*if (isExe == true)*/
  //{
    //resetLines(cw);
    //isExe = false;
  /*}*/
  
  // plot axis
  CurveCombineUI *ui = (CurveCombineUI*)(parent()->user_data());

  int whichClicked = ui->tree->whichClicked;

  fl_color(FL_BLACK);

  fl_line(m_x0, m_y0, m_x0 + m_xl, m_y0);
  fl_line(m_x0, m_y0, m_x0, m_y0 - m_yl);
  
  double radius;
  for (int i=0; i<nLines; ++i) {
    for (std::vector<Point>::iterator iterm=line[i].begin();
        iterm!=line[i].end(); ++iterm)
    {
      fl_color(lineColor[i]);
      if (i == whichClicked)
        radius = 2;
      else
        radius = 1;
      fl_circle(
          m_x0 + (iterm->m_x + xShift[i] - x0)*x_rate,
          m_y0 + (iterm->m_y + yShift[i] - y0)*y_rate,radius
          );
    }
  }
}

void PlotBox::moveX(double x)
{
  CurveCombineUI *ui = (CurveCombineUI*)(parent()->user_data());

  int whichClicked = ui->tree->whichClicked;
  xShift[whichClicked] = x;
}

void PlotBox::moveY(double y)
{
  CurveCombineUI *ui = (CurveCombineUI*)(parent()->user_data());

  int whichClicked = ui->tree->whichClicked;
  yShift[whichClicked] = y;
}

void PlotBox::saveResult(const char* filename) {
  FILE *file = fl_fopen(filename, "w");
  if (file) {
    std::vector<Point> result;
    for (int i=0; i<nLines; ++i) {
      for (std::vector<Point>::iterator iterm=line[i].begin(); iterm!=line[i].end(); ++iterm) {
        result.push_back(Point((iterm)->m_x + xShift[i], (iterm)->m_y + yShift[i]));
      }
    }
    std::sort(result.begin(),result.end(),Point_bl);

    fprintf(file, "%s,%s,%s\n","id","x","y");

    double nId = 0.0;

    for (std::vector<Point>::iterator iterm=result.begin(); iterm!=result.end(); ++iterm) {
        nId += 1;
        fprintf(file, "%f,%f,%f\n",nId,iterm->m_x,iterm->m_y);
    }
  }
  else {
    printf("Open save file error\n");
  }
  fclose(file);
}
