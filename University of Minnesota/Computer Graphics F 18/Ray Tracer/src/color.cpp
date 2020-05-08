#include "color.h"
Color::Color(): r(0), g(0), b(0) {}

Color::~Color() {}

Color::Color(float rx, float gx, float bx): r(rx), g(gx), b(bx) {}

Color operator + (const Color &p, const Color &q) {
  return Color(p.r + q.r, p.g + q.g, p.b + q.b);
}

Color operator * (const Color &p, const Color &q) {
  return Color(p.r * q.r, p.g * q.g, p.b * q.b);
}

Color operator * (const Color &p, double f) {
  return Color(p.r * f, p.g * f, p.b * f);
}

Color operator * (double f, const Color &p) {
  return Color(p.r * f, p.g * f, p.b * f);
}

Color operator / (const Color &p, double f) {
  return Color(p.r / f, p.g / f, p.b / f);
}
