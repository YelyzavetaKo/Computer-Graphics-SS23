

#include "Color.hpp"

Color::Color() {}

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

Color &Color::operator*=(double i) {
  r *= i;
  g *= i;
  b *= i;
  return *this;
}

Color &Color::operator/=(double i) {
  double frac = 1.0 / i;
  r *= frac;
  g *= frac;
  b *= frac;
  return *this;
}

Color &Color::operator+=(const Color &rhs) {
  r += rhs.r;
  g += rhs.g;
  b += rhs.b;

  return *this;
}

/*
bool operator==(const Color &color1, const Color &color2) {
  return (color1.r == color2.r) && (color1.g == color2.g) && (color1.b == color2.b);
}
 
Color &Color::operator==(const Color &rhs) {
  return (r == rhs.r) && (g == rhs.g) && (b == rhs.b);
}
*/