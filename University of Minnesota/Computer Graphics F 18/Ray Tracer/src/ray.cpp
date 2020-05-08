#include "vector.h"
#include "ray.h"
#include "point.h"
#include <iostream>
#include <math.h>
#include <string>


Ray::Ray() {
  o = Point(0, 0, 0);
  d = Vector(0, 0, -1);
  tmax_ = 99999;
  tmin_ = 0.0;
  inside = false;
  shapenum = -1;
  shapetype = "";
}

Ray::~Ray() {}

Ray::Ray(Point orig, Vector dir): o(orig), d(dir), tmin_(0), tmax_(9999), inside(false), shapenum(-1), shapetype("") {}

Ray::Ray(Point orig, Vector dir, double mint, double maxt): o(orig), d(dir), tmin_(mint), tmax_(maxt), inside(false), shapenum(-1), shapetype("") {}

Ray::Ray(Point orig, Vector dir, double mint, double maxt, bool in ): o(orig), d(dir), tmin_(mint), tmax_(maxt), inside( in ), shapenum(-1), shapetype("") {}

Ray::Ray(Point orig, Vector dir, double mint, double maxt, bool in , std::string str, int num): o(orig), d(dir), tmin_(mint), tmax_(maxt), inside( in ), shapenum(num), shapetype(str) {}

Vector Ray::reflect(Vector n) {
  return (d - 2.0 * d.dot(n) * n).norm();
}

Vector Ray::refract(Vector n, double ni, double nr) {

  double ninr = ni / nr;

  double ai = acos((-d).dot(n));
  double ar = asin(ninr * sin(ai));
  // std::cout << "in refract ni:"<<ni<<" nr "<<nr<<" ninr "<<ninr<<" normal " <<n<<" -d "<<-d<<" ai "<<ai<<" ar "<<ar<< '\n';
  return ((ninr * cos(ai) - cos(ar)) * n - ninr * (-d)).norm();
}

double Ray::len() {
  return d.len();
}

Point Ray::eval(double t) {
  return o + t * d.norm();
}

std::ostream &operator << (std::ostream &os,
  const Ray &r) {
  return os << r.org() << " " << r.dir() << r.inside << r.shapetype << r.shapenum;
}
