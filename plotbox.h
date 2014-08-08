#ifndef _PLOT_Box_H
#define _PLOT_Box_H

#include "FL/Fl_Box.H"
#include <string>
#include <vector>
#include "point.h"

class CurveCombineUI;

class PlotBox: public Fl_Box {
  public:
    PlotBox(int x, int y, int w, int h);

    // member variables
    double m_x0, m_y0, m_xl, m_yl;
    double x0, y0, x1, y1;
    double x_rate, y_rate;
    double chooseX, chooseY; // x,y chosen using mouse
    double startX[7], endX[7];
    double xShift[7], yShift[7];
    int nLines;
    int whichLow;
    bool dataChanged;
    bool yLimitChanged;
    //std::vector<Point> line[2];
    std::vector<Point> line[7];
    int lineColor[7];
    //bool isExe;
    bool showPoint;


    // member functions
    int handle(int e);
    void addLines();
    void plotLines();
    void draw();
    void readData();
    void getNLines();
    void getXLimitsRates();
    void getYLimitsRates();
    void setXSliderValue();
    void setYSliderValue();
    void moveX(double x);
    void moveY(double y);
    void iniShiftValue();
    void iniDeleteValue();
    void iniValues();
    void resetValues();
    void deleteValue();
    void saveResult(const char* filename);

    static void cb_left(Fl_Widget*, void *userdata);
    static void cb_cross(Fl_Widget*, void *userdata);
    static void cb_right(Fl_Widget*, void *userdata);
    static void cb_clear_left(Fl_Widget*, void *userdata);
    static void cb_clear_cross(Fl_Widget*, void *userdata);
    static void cb_clear_right(Fl_Widget*, void *userdata);
};

#endif
