#include "point.h"
#include "vector.h"
#include <iostream>
#include <math.h>



Vector::Vector() {
  x = 0;
  y = 0;
  z = 0;
}

Vector::~Vector() {}

Vector::Vector(float u, float v, float w) {
  x = u;
  y = v;
  z = w;
}

float Vector::dot(const Vector &v2) {
  return x * v2.gx() + y * v2.gy() + z * v2.gz();
}

Vector Vector::cross(const Vector &v2) {
  return Vector(y * v2.gz() - z * v2.gy(), z * v2.gx() - x * v2.gz(), x * v2.gy() - y * v2.gx());
}

float Vector::len() {
  return sqrt(x * x + y * y + z * z);
}

Vector Vector::norm() {
  float top = pow(fabs(x), 2.0) + pow(fabs(y), 2.0) + pow(fabs(z), 2.0);
  if (top > 1e-6) {
    float d = 1.0 / (float) sqrt(top);
    float u = x * d;
    float v = y * d;
    float w = z * d;
    return Vector(u, v, w);
  } else return Vector(x, y, z);
}

Vector Vector::reflect(Vector &d, Vector &n) {
  return d - 2.0 * d.dot(n) * n;
}

Vector operator * (const Vector &v, const double f) {
  return Vector(v.gx() * f, v.gy() * f, v.gz() * f);
}

Vector operator * (const double f, const Vector &v) {
  return Vector(v.gx() * f, v.gy() * f, v.gz() * f);
}

Vector operator / (const Vector &v, const double f) {
  return Vector(v.gx() / (double) f, v.gy() / (double) f, v.gz() / (double) f);
}

Vector operator - (const Vector &v) {
  return Vector(-v.gx(), -v.gy(), -v.gz());
}

Point operator + (const Point &p, const Vector &v) {
  return Point(p.gx() + v.gx(), p.gy() + v.gy(), p.gz() + v.gz());
}

Point operator + (const Vector &v, const Point &p) {
  return Point(p.gx() + v.gx(), p.gy() + v.gy(), p.gz() + v.gz());
}

Vector operator + (const Vector &v1, const Vector &v2) {
  return Vector(v1.gx() + v2.gx(), v1.gy() + v2.gy(), v1.gz() + v2.gz());
}

Vector operator * (const Vector &v1, const Vector &v2) {
  return Vector(v1.gx() * v2.gx(), v1.gy() * v2.gy(), v1.gz() * v2.gz());
}

Vector operator - (const Vector &v1, const Vector &v2) {
  return Vector(v1.gx() - v2.gx(), v1.gy() - v2.gy(), v1.gz() - v2.gz());
}

Vector operator - (const Point &p1, const Point &p2) {
  return Vector(p1.gx() - p2.gx(), p1.gy() - p2.gy(), p1.gz() - p2.gz());
}

std::ostream &operator << (std::ostream &os, const Vector &v) {
  return os << "<" << v.gx() << ", " << v.gy() << ", " << v.gz() << ">";
}
