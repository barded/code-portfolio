#include "point.h"

#include <math.h>



Point::Point() {
  x = 0;
  y = 0;
  z = 0;
}

Point::~Point() {}

Point::Point(float u, float v, float w) {
  x = u;
  y = v;
  z = w;
}

Point Point::lerp(const Point &q, float alpha) {
  float u = (1.0 - alpha) * x + alpha * q.gx();
  float v = (1.0 - alpha) * y + alpha * q.gy();
  float w = (1.0 - alpha) * z + alpha * q.gz();

  return Point(u, v, w);

}

std::ostream &operator << (std::ostream &os, const Point &p) {
  return os << "(" << p.gx() << ", " << p.gy() << ", " << p.gz() << ")";
}
