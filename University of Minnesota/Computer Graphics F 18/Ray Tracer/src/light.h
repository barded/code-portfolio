#ifndef LIGHT_H_
#define LIGHT_H_

#include "color.h"
#include "point.h"
#include "vector.h"
#include <string>
using namespace std;

class Light{
public:
  Light();
  Light(Color color);
  Light(Color color, string type);
  virtual ~Light();

  Color get_color() {
    return color_;
  }

  Color intensity();

  string type() {
    return type_;
  }

private:
  Color color_;
  string type_;


};


class PL : public Light{
public:
  PL();
  PL(Color color, Point pos);

  PL(Color color, Point pos, float c, float l, float q);
  virtual ~PL();

  Color intensity(Point pos);

   Point pos() {
     return pos_;
   }
   // float* atten();

private:
  Point pos_;

  // Channel intensities
  float Io_R;
  float Io_G;
  float Io_B;

  // Attenuation constants
  float k_c;
  float k_l;
  float k_q;
};


class Directional : public Light{
public:
  Directional();
  Directional(Color color, Vector dir);
  virtual ~Directional();

  Color intensity();

  Vector dir() {
   return dir_;
  }

private:
  Vector dir_;
  float Io_R;
  float Io_G;
  float Io_B;

};


class Spot : public Light{
public:
  Spot();
  Spot(Color color, Point pos, Vector dir, float angle1, float angle2);
  virtual ~Spot();
  Point pos() {
    return pos_;
  }
  Vector dir() {
    return dir_;
  }
  float angle1() {
    return angle1_;
  }
  float angle2() {
    return angle2_;
  }
  Color intensity(Point pos, Vector l);


private:
  Point pos_;
  Vector dir_;
  float angle1_;
  float angle2_;

  // Channel intensities
  float Io_R;
  float Io_G;
  float Io_B;

  // Attenuation constants
  float k_c;
  float k_l;
  float k_q;


};

class AL : public Light{
public:
  AL(Color color, Point pos, Vector h, Vector v);

  virtual ~AL();

  Color intensity(Point pos);

  Point pos() {
    return pos_;
  }

  Point randpos();
  Point center() {
    return pos_ + (horiz * .5 + vertical * .5);
  }

private:
  Point pos_;

  // Channel intensities
  float Io_R;
  float Io_G;
  float Io_B;

  // Attenuation constants
  float k_c;
  float k_l;
  float k_q;

  Vector horiz;
  Vector vertical;



};



#endif
