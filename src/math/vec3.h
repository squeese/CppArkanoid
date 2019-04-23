#ifndef EKSAMENECS17_VEC3_H
#define EKSAMENECS17_VEC3_H

#include "GL/glew.h"
#include <cmath>

struct Vec3 {
public:
  GLfloat x = 0.0f;
  GLfloat y = 0.0f;
  GLfloat z = 0.0f;

  Vec3() = default;
  Vec3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}

  static const Vec3 ZERO() { return Vec3(0.0f, 0.0f, 0.0f); }
  static const Vec3 UP() { return Vec3(0.0f, 1.0f, 0.0f); }
  static const Vec3 Cross(const Vec3& a, const Vec3&b);
  static GLfloat Dot(const Vec3& a, const Vec3& b);

  void operator +=(const Vec3& o);
  void operator -=(const Vec3& o);
  const Vec3 operator +(const Vec3& o) const;
  const Vec3 operator -(const Vec3& o) const;
  const Vec3 operator -() const;
  const Vec3 operator *(GLfloat s) const;
  const Vec3 operator /(GLfloat s) const;
  const Vec3 Cross(const Vec3& o) const;
  GLfloat MagnitudeSQ() const;
  GLfloat Magnitude() const;
  const Vec3 Normalized() const;
  void Normalize();
};

#endif
