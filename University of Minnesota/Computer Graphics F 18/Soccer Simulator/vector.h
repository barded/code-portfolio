#ifndef VECTOR_H_
#define VECTOR_H_

#include "point.h"

#include <math.h>
#include <iostream>


class Vector{
public:

  Vector();
  virtual ~Vector();
  Vector(float u, float v, float w);

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

  float dot(const Vector &v2);
  Vector cross(const Vector &v2);
  float len();
  Vector norm();
  Vector reflect(Vector &d, Vector &n);

  float x;
  float y;
  float z;
};

Vector operator * (const Vector &v, const double f);
Vector operator * (const Vector &v1, const Vector &v2);

Vector operator * (const double f, const Vector &v2);
Vector operator / (const Vector &v, const double f);
Vector operator - (const Vector &v);
Point operator + (const Point &p, const Vector &v);
Point operator + (const Vector &v, const Point &p);
Vector operator + (const Vector &v1, const Vector &v2);
Vector operator - (const Vector &v1, const Vector &v2);
Vector operator - (const Point &p1, const Point &p2);
std::ostream &operator << (std::ostream &os, const Vector &v);
#endif
