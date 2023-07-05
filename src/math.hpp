#pragma once

#include <array>
#include <vector>
#include <limits>
#include <memory>

#include "GLMatrix.hpp"
#include "GLPoint.hpp"
#include "GLVector.hpp"

inline GLVector operator*(const GLVector &lhs, double scale) {
  return GLVector(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLVector operator*(double scale, const GLVector &rhs) {
  return GLVector(rhs(0) * scale, rhs(1) * scale, rhs(2) * scale);
}

inline GLPoint operator*(const GLPoint &lhs, double scale) {
  return GLPoint(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLPoint operator*(double scale, const GLPoint &rhs) {
  return GLPoint(rhs(0) * scale, rhs(1) * scale, rhs(2) * scale);
}

inline GLPoint operator+(const GLPoint &p1, const GLVector &p2) {
  return GLPoint(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLPoint operator+(const GLPoint &p1, const GLPoint &p2) {
  return GLPoint(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLVector operator+(const GLVector &p1, const GLVector &p2) {
  return GLVector(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLVector operator-(const GLPoint &p1, const GLPoint &p2) {
  return GLVector(p1(0) - p2(0), p1(1) - p2(1), p1(2) - p2(2));
}

inline GLVector operator-(const GLVector &p1, const GLVector &p2) {
  return GLVector(p1(0) - p2(0), p1(1) - p2(1), p1(2) - p2(2));
}

inline GLVector crossProduct(const GLVector &lhs, const GLVector &rhs) {
  return GLVector(lhs(1) * rhs(2) - lhs(2) * rhs(1),
                  lhs(2) * rhs(0) - lhs(0) * rhs(2),
                  lhs(0) * rhs(1) - lhs(1) * rhs(0));
}

inline double dotProduct(const GLVector &lhs, const GLVector &rhs) {
  return lhs(0) * rhs(0) + lhs(1) * rhs(1) + lhs(2) * rhs(2);
}

inline void barycentricCoordinates(GLPoint p, GLPoint a, GLPoint b, GLPoint c, float &u, float &v, float &w) {
  GLVector v0 = b - a, v1 = c - a, v2 = p - a;
  float d00 = dotProduct(v0, v0);
  float d01 = dotProduct(v0, v1);
  float d11 = dotProduct(v1, v1);
  float d20 = dotProduct(v2, v0);
  float d21 = dotProduct(v2, v1);
  float denom = d00 * d11 - d01 * d01;
  v = (d11 * d20 - d01 * d21) / denom;
  w = (d00 * d21 - d01 * d20) / denom;
  u = 1.0f - v - w;
}

inline int sgn(int x) { return (x > 0) ? 1 : (x < 0) ? -1 : 0; }

/** Aufgabenblatt 2, Aufgabe 2 **/

inline GLVector operator*(const GLMatrix &lhs, const GLVector &rhs) {
  return GLVector(lhs(0, 0) * rhs(0) + lhs(0, 1) * rhs(1) + lhs(0, 2) * rhs(2),
                  lhs(1, 0) * rhs(0) + lhs(1, 1) * rhs(1) + lhs(1, 2) * rhs(2),
                  lhs(2, 0) * rhs(0) + lhs(2, 1) * rhs(1) + lhs(2, 2) * rhs(2));
}

inline GLPoint operator*(const GLMatrix &lhs, const GLPoint &rhs) {
  return GLPoint(lhs(0, 0) * rhs(0) + lhs(0, 1) * rhs(1) + lhs(0, 2) * rhs(2) + lhs(0, 3),
                  lhs(1, 0) * rhs(0) + lhs(1, 1) * rhs(1) + lhs(1, 2) * rhs(2) + lhs(1, 3),
                  lhs(2, 0) * rhs(0) + lhs(2, 1) * rhs(1) + lhs(2, 2) * rhs(2) + lhs(2, 3));
}

inline GLMatrix operator*(const GLMatrix &lhs, const GLMatrix &rhs) {
  GLMatrix matrix = GLMatrix();
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      float entry = lhs(r, 0) * rhs(0, c) + lhs(r, 1) * rhs(1, c) + lhs(r, 2) * rhs(2, c) + lhs(r, 3) * rhs(3, c);
      matrix.setValue(r, c, entry);
    }
  }
  return matrix;
}
