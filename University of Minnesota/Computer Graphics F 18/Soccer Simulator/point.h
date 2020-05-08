
#ifndef POINT_H_
#define POINT_H_

#include <iostream>


class Point{
public:

  Point();
  virtual ~Point();

  Point(float u, float v, float w);
   // Point& operator=(const Point& p);
  float gx() const {
   return x;
  }
  float gy() const {
   return y;
  }
  float gz() const {
   return z;
  }
  void set_x(const float &u) {
   x = u;
  }
  void set_y(const float &v) {
   y = v;
  }
  void set_z(const float &w) {
   z = w;
  }

  Point lerp(const Point &q, float alpha);

  float x;
  float y;
  float z;

};



std::ostream &operator << ( std::ostream &os, const Point &p);






#endif
