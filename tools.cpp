#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "stdlib.h"
#include "point.h"
#include <FL/Fl.H>
#include <FL/Fl_Float_Input.H>

double getEditValue(Fl_Float_Input *in)
{
  const char *buff;
  buff = in->value();
  double d = strtod(buff, '\0');
  return d;
}

void setEditValue(Fl_Float_Input *in, double value) {
  char buff[200];
  sprintf(buff,"%.1f",value);
  in->value(buff);
}

void makePath(std::string &str) {
  std::string tmp;
  int pass = 0;
  for (std::string::iterator it=str.begin(); it!=str.end(); ++it) {
      pass++;
      if (pass <= 7) continue;
      if (*it == '/') {
          tmp = tmp + "\\/";
      } else if (*it == '\n') {
          break;
      } else {
          tmp = tmp + *it;
      }
  }
  str = tmp;
}

void readfile(const char* filename, std::vector<Point> &points) {
  double x;
  double y;
  double dTmp;
  FILE* in = fopen(filename, "r");
  if (in) {
    char buff[256];
    fgets(buff, 256, in);
    
    while ((fscanf(in, "%lf,%lf,%lf", &dTmp, &x, &y)) != EOF) {
      fgets(buff, 256, in);
      Point pTmp(x,y);
      points.push_back(pTmp);
    }
  } else {
    printf("file open error in readfile\n");
  }
  fclose(in);
}

/*void savefile(std::string str, std::vector<Point> &points) {*/
  //std::ofstream ou;
  //ou.open(str.c_str(), std::ofstream::out);
  //if (ou.is_open()) {
    //ou << "x,y\n";
    //for (std::vector<Point>::iterator iterm=points.begin(); iterm!=points.end(); ++iterm) {
      //ou << iterm->m_x << "," << iterm->m_y << "\n";
    //}
    //ou << "\n";
  //}
  //ou.close();
//}

//void data_process(std::vector<Point> &input, std::vector<Point> &output) {
  //if (output.empty()) {
    //output = input;
  //}
  //else {
    //output.insert(output.end(), input.begin(), input.end());
    //std::sort(output.begin(), output.end());
    //for (int i=0; i<3; ++i) {
      //std::vector<Point>::iterator pre = output.begin();
      //for (std::vector<Point>::iterator iterm=output.begin()+1; iterm != output.end() - 3; ++iterm) {
        //if ((pre->m_y < iterm->m_y) && 
            //( (iterm->m_y - pre->m_y > 2) || 
              //((iterm+1)->m_y - pre->m_y > 2) ||
              //((iterm+2)->m_y - pre->m_y > 2) || 
              //((iterm+3)->m_y - pre->m_y > 2)
              //)) {
          //output.erase(pre);
        //}
        //[>else if ((pre->m_x > iterm->m_x) && (pre->m_x - iterm->m_x > 2)) {<]
          ////output.erase(iterm);
        //[>}<]
        //pre = iterm;
      //}
    //}
    //[>double x1i = (input.end()-1)->m_x;<]
    ////double x0i = (input.begin())->m_x;
    
    ////double x1o = (output.end()-1)->m_x;
    ////double x0o = (output.begin())->m_x;
    
    ////if (x0i < x0o) {
      ////if (x1i < x0o) {
        ////output.insert(output.begin(), input.begin(), input.end());
      ////} else if (x1i > x0o && x1i < x1o) {
        
      ////}
    ////}

  //}
/*}*/
