

#include "point.h"
#include "vector.h"
#include "color.h"
#include "light.h"
#include <string>
#include <cstdlib>


Light::Light(): type_("ambient"){
  color_ = Color(0,0,0);

}
Light::Light(Color color, string type){
  color_ = color;
  type_ = type;
}

Light::Light(Color color){
  color_ = color;
  type_ = "ambient";
}

Light::~Light(){
}

Color Light::intensity(){
  return color_;
}

PL::PL(Color color, Point pos) : Light(color,"point"), pos_(pos) {
	// Initial Intensity variables
	Io_R = color.r;
	Io_G = color.g;
	Io_B = color.b;

	//(Attenuation factors)
	k_c = 0;
	k_l = 0;
	k_q = 1;
}

PL::PL(Color color, Point pos, float c, float l, float q) : Light(color,"point"), pos_(pos) {
	// Initial Intensity variables
	Io_R = color.r;
	Io_G = color.g;
	Io_B = color.b;

	//(Attenuation factors)
	k_c = c;
	k_l = l;
	k_q = q;
}

PL::~PL(){}

Color PL::intensity(Point pos) {
	// Distance
	float d = sqrt(pow((pos.u()-pos_.u()), 2.0) + pow((pos.v()-pos_.v()), 2.0) + pow((pos.w()-pos_.w()), 2.0));

	float rI_atten = Io_R / (k_c + (d * k_l) + (k_q * d * d));
	float gI_atten = Io_G / (k_c + (d * k_l) + (k_q * d * d));
	float bI_atten = Io_B / (k_c + (d * k_l) + (k_q * d * d));

	return Color(rI_atten, gI_atten, bI_atten);
}


Directional::Directional(Color color, Vector dir) : Light(color, "directional") {
  dir_=dir.norm();

  Io_R = color.r;
  Io_G = color.g;
  Io_B = color.b;
}

Directional::~Directional(){}

Color Directional::intensity() {
	return Color(Io_R, Io_G, Io_B);
}

Spot::Spot(Color color, Point pos, Vector dir, float angle1, float angle2) : Light(color,"spot"),   pos_(pos), angle1_(angle1 * M_PI / 180.0f), angle2_(angle2 * M_PI / 180.0f) {
  dir_ = dir.norm();
  Io_R = color.r;
  Io_G = color.g;
  Io_B = color.b;
  k_c = 0;
	k_l = 0;
	k_q = 1;

}

Spot::~Spot(){}

Color Spot::intensity(Point pos, Vector l) {
//  a . b= |a||b|cos(t)
double a = acos(dir_.dot(-l));

// std::cout << "l"<<l<<" a "<<a <<" a1 "<<angle1_<<" a2 "<<angle2_ << '\n';
  if (a > angle2_) {
    return Color(0, 0, 0);
  } else
  {
    float d = sqrt(pow((pos.u()-pos_.u()), 2.0) + pow((pos.v()-pos_.v()), 2.0) + pow((pos.w()-pos_.w()), 2.0));

  	float rI_atten = Io_R / (k_c + (d * k_l) + (k_q * d * d));
  	float gI_atten = Io_G / (k_c + (d * k_l) + (k_q * d * d));
  	float bI_atten = Io_B / (k_c + (d * k_l) + (k_q * d * d));

  	Color intensity = Color(rI_atten, gI_atten, bI_atten);
    if (a < angle1_) {
      return intensity;
    } else {   // between  a1 & a2.   polynomial approx
      double x = fabs((a-angle2_) / (angle2_-angle1_));
      double m = .0866889,  a = 1.13041, D = 1.12511, b = 0, c = 2.54632, f = 5.21602, g = -9.10455;
      double y = m * x + b + a * pow(x, 2.0) + c * pow(x, 3.0) + D * pow(x, 4.0) + g * pow(x, 5.0) + f * pow(x,6.0);
      return y * intensity;
    }
  }
}


AL::AL(Color color, Point pos, Vector h, Vector v) : Light(color,"area"), pos_(pos) {

  horiz = h;
  vertical = v;

	// Initial Intensity variables
	Io_R = color.r;
	Io_G = color.g;
	Io_B = color.b;

	//(Attenuation factors)
	k_c = 0;
	k_l = 0;
	k_q = 1;
}

AL::~AL(){}

Color AL::intensity(Point pos) {
	// Distance
	float d = sqrt(pow((pos.u()-pos_.u()), 2.0) + pow((pos.v()-pos_.v()), 2.0) + pow((pos.w()-pos_.w()), 2.0));

	float rI_atten = Io_R / (k_c + (d * k_l) + (k_q * d * d));
	float gI_atten = Io_G / (k_c + (d * k_l) + (k_q * d * d));
	float bI_atten = Io_B / (k_c + (d * k_l) + (k_q * d * d));

	return Color(rI_atten, gI_atten, bI_atten);
}


Point AL::randpos(){
  float r1 = (float)rand() / (float)(RAND_MAX);
  float r2 = (float)rand() / (float)(RAND_MAX);

  return pos_ + (horiz * r1 + vertical * r2);
}
