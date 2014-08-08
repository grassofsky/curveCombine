#ifndef _POINT_H
#define _POINT_H

class Point
{
public:
  Point();
  Point(double x, double y);

  double m_x;
  double m_y;
};

inline bool Point_bl(const Point &a, const Point &b)
{
  return a.m_x > b.m_x;
}
#endif
