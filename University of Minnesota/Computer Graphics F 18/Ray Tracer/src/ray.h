#ifndef RAY_H_
#define RAY_H_

#include "point.h"
#include "vector.h"
#include <iostream>
#include <math.h>
#include <string>

class Ray {
public:
  Ray();
  virtual~Ray();
  Ray(Point orig, Vector dir);

  Ray(Point orig, Vector dir, double mint, double maxt);
  Ray(Point orig, Vector dir, double mint, double maxt, bool in );
  Ray(Point orig, Vector dir, double mint, double maxt, bool in ,
    std::string str, int num);

  double len();

  Point org() const {
    return o;
  }
  Vector dir() const {
    return d;
  }
  Point eval(double t);
  double tmax() const {
    return tmax_;
  }
  double tmin() const {
    return tmin_;
  }
  void settmax(double m) {
    tmax_ = m;
  }
  void settmin(double m) {
    tmin_ = m;
  }
  Vector reflect(Vector n);
  Vector refract(Vector n, double ni, double nr);

  bool inside;
  float riior;
  std::string shapetype;
  int shapenum;

private:
  Point o;
  Vector d;

  double tmax_;
  double tmin_;

};

std::ostream &operator << (std::ostream &os,
  const Ray &r);

#endif
