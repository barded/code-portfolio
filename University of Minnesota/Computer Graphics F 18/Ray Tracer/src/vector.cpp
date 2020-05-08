#include "point.h"
#include "vector.h"
#include <iostream>
#include <math.h>


Vector::Vector() {
  u_ = 0;
  v_ = 0;
  w_ = 0;
}

Vector::~Vector() {}

Vector::Vector(double u, double v, double w) {
  u_ = u;
  v_ = v;
  w_ = w;
}

double Vector::dot(const Vector &v2) {
  return u_ * v2.u() + v_ * v2.v() + w_ * v2.w();
}

Vector Vector::cross(const Vector &v2) {
  return Vector(v_ * v2.w() - w_ * v2.v(), w_ * v2.u() - u_ * v2.w(), u_ * v2.v() - v_ * v2.u());
}

double Vector::len() {
  return sqrt(u_ * u_ + v_ * v_ + w_ * w_);
}

Vector Vector::norm() {
  double top = pow(fabs(u_), 2.0) + pow(fabs(v_), 2.0) + pow(fabs(w_), 2.0);
  if (top > 1e-6) {
    double d = 1.0 / (double) sqrt(top);
    double u = u_ * d;
    double v = v_ * d;
    double w = w_ * d;
    return Vector(u, v, w);
  } else return Vector(u_, v_, w_);
}

Vector operator * (const Vector &v,
  const double f) {
  return Vector(v.u() * f, v.v() * f, v.w() * f);
}

Vector operator * (const double f,
  const Vector &v) {
  return Vector(v.u() * f, v.v() * f, v.w() * f);
}

Vector operator / (const Vector &v,
  const double f) {
  return Vector(v.u() / (double) f, v.v() / (double) f, v.w() / (double) f);
}

Vector operator - (const Vector &v) {
  return Vector(-v.u(), -v.v(), -v.w());
}

Point operator + (const Point &p,
  const Vector &v) {
  return Point(p.u() + v.u(), p.v() + v.v(), p.w() + v.w());
}

Point operator + (const Vector &v,
  const Point &p) {
  return Point(p.u() + v.u(), p.v() + v.v(), p.w() + v.w());
}

Vector operator + (const Vector &v1,
  const Vector &v2) {
  return Vector(v1.u() + v2.u(), v1.v() + v2.v(), v1.w() + v2.w());
}

Vector operator * (const Vector &v1,
  const Vector &v2) {
  return Vector(v1.u() * v2.u(), v1.v() * v2.v(), v1.w() * v2.w());
}

Vector operator - (const Vector &v1,
  const Vector &v2) {
  return Vector(v1.u() - v2.u(), v1.v() - v2.v(), v1.w() - v2.w());
}

Vector operator - (const Point &p1,
  const Point &p2) {
  return Vector(p1.u() - p2.u(), p1.v() - p2.v(), p1.w() - p2.w());
}

std::ostream &operator << (std::ostream &os,
  const Vector &v) {
  return os << "<" << v.u() << ", " << v.v() << ", " << v.w() << ">";
}
