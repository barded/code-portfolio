#ifndef SCENE_H_
#define SCENE_H_


#include "point.h"
#include "vector.h"
#include "ray.h"

class Scene{
public:
  int w;
  int h;
  Color background;
  Point eye;
  Vector up;
  Vector view;
  Vector right;
  Point ul;
  Point ur;
  Point bl;
  Point br;
  double ha;
  double wa;
  int max_depth;

};

#endif
