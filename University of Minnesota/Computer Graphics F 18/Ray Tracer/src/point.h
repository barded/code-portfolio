#ifndef POINT_H_
#define POINT_H_

#include <iostream>


class Point {
  public:

    Point();
  virtual~Point();

  Point(double u, double v, double w);
  // Point& operator=(const Point& p);
  double u() const {
    return u_;
  }
  double v() const {
    return v_;
  }
  double w() const {
    return w_;
  }
  void set_u(const double &u) {
    u_ = u;
  }
  void set_v(const double &v) {
    v_ = v;
  }
  void set_w(const double &w) {
    w_ = w;
  }

  Point lerp(const Point &q, float alpha);

private:
  double u_;
  double v_;
  double w_;

};

std::ostream &operator << (std::ostream &os,
  const Point &p);

#endif
