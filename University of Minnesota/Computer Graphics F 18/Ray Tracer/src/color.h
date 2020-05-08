#ifndef COLOR_H_
#define COLOR_H_


class Color{
public:
  Color();
  virtual ~Color();

  Color(float rx, float gx, float bx);
  float lum() {
    return .33 * r + .59 * g + .11 * b;
  }

  float r;
  float g;
  float b;


};


Color operator+ (const Color &p, const Color &q);

Color operator* (const Color &p, const Color &q);


Color operator* (const Color &p, double f);
Color operator* (double f, const Color &p);
Color operator/ (const Color &p, double f);







#endif
