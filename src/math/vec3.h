#ifndef EKSAMENECS17_VEC3_H
#define EKSAMENECS17_VEC3_H

#include "GL/glew.h"
#include <cmath>

struct vec3 {
public:
  GLfloat x = 0.0f;
  GLfloat y = 0.0f;
  GLfloat z = 0.0f;

  vec3() = default;
  vec3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}

  static const vec3 ZERO() { return vec3(0.0f, 0.0f, 0.0f); }
  static const vec3 UP() { return vec3(0.0f, 1.0f, 0.0f); }
  static const vec3 Cross(const vec3& a, const vec3&b);

  void operator +=(const vec3& o);
  void operator -=(const vec3& o);
  const vec3 operator +(const vec3& o) const;
  const vec3 operator -(const vec3& o) const;
  const vec3 operator *(GLfloat s) const;
  const vec3 operator /(GLfloat s) const;
  const vec3 Cross(const vec3& o) const;
  GLfloat MagnitudeSQ() const;
  GLfloat Magnitude() const;
  const vec3 Normalized() const;
  void Normalize();
};

#endif
