#ifndef VECTOR_H_
#define VECTOR_H_

#include "point.h"

#include <math.h>
#include <iostream>


class Vector{
public:
  Vector();
  virtual~Vector();
  Vector(double u, double v, double w);

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

  double dot(const Vector &v2);
  Vector cross(const Vector &v2);
  double len();
  Vector norm();

private:
  double u_;
  double v_;
  double w_;
  };

Vector operator*(const Vector&v, const double f);
Vector operator*(const Vector&v1, const Vector &v2);

Vector operator*(const double f,const Vector&v2);
Vector operator/(const Vector&v, const double f);
Vector operator-(const Vector&v);
Point operator+(const Point &p, const Vector& v);
Point operator+(const Vector& v, const Point& p);
Vector operator+(const Vector& v1, const Vector& v2);
Vector operator-(const Vector& v1, const Vector& v2);
Vector operator-(const Point& p1, const Point& p2);
std::ostream &operator<< ( std::ostream &os, const Vector &v);



#endif
