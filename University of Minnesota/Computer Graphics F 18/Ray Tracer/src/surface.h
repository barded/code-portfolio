#ifndef SURFACE_H_
#define SURFACE_H_


#include "point.h"
#include "vector.h"
#include <string>
using namespace std;
class Surface {
public:
  Surface();
  Surface(int material, string type);
  virtual~Surface();

  int material() {
    return material_;
  }

  string type() {
    return type_;
  }
private:
  int material_;
  string type_;

};

class Sphere: public Surface {
public:
  Sphere(Point center, double radius, int material);
  virtual~Sphere();

  Point get_c() {
    return center_;
  }
  double get_r() {
    return radius_;
  }

private:
  Point center_;
  double radius_;

};

class Triangle: public Surface {
public:
  Triangle();
  Triangle(Point v1, Point v2, Point v3, int material);
  Triangle(Point v1, Point v2, Point v3, Vector n1, Vector n2, Vector n3, int material);

  virtual~Triangle();

  void setnormals(Vector nn) {
    n[0] = nn;
    n[1] = nn;
    n[2] = nn;
  }

  Point verts[3];
  Vector n[3];
  bool diff;
  Vector side1;
  Vector side2;

};

class Plane: public Surface {
public:
  Plane();
  Plane(Point pos, Vector n, int material);
  virtual~Plane();

  Point pos() {
    return pos_;
  }
  Vector n() {
    return n_;
  }

private:
  Point pos_;
  Vector n_;

};

std::ostream &operator << (std::ostream &os, const Triangle &t);


#endif
