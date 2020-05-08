#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "color.h"

class Material{
public:
  Material(){};
  Material(float ar, float ag, float ab, float dr, float dg, float db,
    float sr, float sg, float sb, double n, float tr, float tg, float tb, float i){
    a = Color(ar, ag, ab);
    d = Color(dr, dg, db);
    s = Color(sr, sg, sb);
    ns = n;
    t = Color(tr, tg, tb);
    ior = i;
  }

  Color a;
  Color d;
  Color s;
  double ns;
  Color t;
  float ior;



};


#endif
