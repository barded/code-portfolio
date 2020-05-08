#include "point.h"

#include <math.h>


Point::Point() {
  u_ = 0;
  v_ = 0;
  w_ = 0;
}

Point::~Point() {}

Point::Point(double u, double v, double w) {
  u_ = u;
  v_ = v;
  w_ = w;
}

Point Point::lerp(const Point &q, float alpha) {
  float u = (1.0 - alpha) * u_ + alpha * q.u();
  float v = (1.0 - alpha) * v_ + alpha * q.v();
  float w = (1.0 - alpha) * w_ + alpha * q.w();

  return Point(u, v, w);

}

std::ostream &operator << (std::ostream &os,
  const Point &p) {
  return os << "(" << p.u() << ", " << p.v() << ", " << p.w() << ")";
}
