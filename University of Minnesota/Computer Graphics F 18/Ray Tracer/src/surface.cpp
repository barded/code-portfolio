

#include "point.h"
#include "vector.h"
#include "surface.h"
#include <string>


Surface::Surface(): material_(-1), type_("none") {}
Surface::Surface(int material, string type) {
  material_ = material;
  type_ = type;
}

Surface::~Surface() {}

Sphere::Sphere(Point center, double radius, int material): Surface(material, "sphere"), center_(center), radius_(radius) {

}

Sphere::~Sphere() {}

Triangle::Triangle() {}
Triangle::~Triangle() {}

Triangle::Triangle(Point v1, Point v2, Point v3, int material): Surface(material, "triangle") {
  verts[0] = v1;
  verts[1] = v2;
  verts[2] = v3;
  n[0] = (v1 - v2).cross(v3 - v2).norm();
  n[1] = n[0];
  n[2] = n[0];
  diff = false;
}

Triangle::Triangle(Point v1, Point v2, Point v3, Vector n1, Vector n2, Vector n3, int material): Surface(material, "triangle") {
  verts[0] = v1;
  verts[1] = v2;
  verts[2] = v3;
  n[0] = n1;
  n[1] = n2;
  n[2] = n3;
  diff = true;
}

std::ostream &operator << (std::ostream &os,
  const Triangle &t) {
  return os << t.verts[0] << t.verts[1] << t.verts[2] << t.n[0] << t.n[1] << t.n[2];

}

Plane::Plane() {}
Plane::~Plane() {}

Plane::Plane(Point pos, Vector n, int material): Surface(material, "plane"), pos_(pos), n_(n) {}
