#include <string>
#include <vector>
#include "FL/Fl_Float_Input.H"

void makePath(std::string &str);
void readfile(const char* filename, std::vector<Point> &line);
double getEditValue(Fl_Float_Input *in);
void setEditValue(Fl_Float_Input *in, double value);
