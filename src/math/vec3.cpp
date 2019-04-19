#include "vec3.h"

void vec3::operator +=(const vec3& o) {
  x += o.x;
  y += o.y;
  x += o.z;
}

void vec3::operator -=(const vec3& o) {
  x -= o.x;
  y -= o.y;
  x -= o.z;
}

const vec3 vec3::operator +(const vec3& o) const {
  return vec3(x + o.x, y + o.y, z + o.z);
}

const vec3 vec3::operator -(const vec3& o) const {
  return vec3(x - o.x, y - o.y, z - o.z);
}

const vec3 vec3::operator *(GLfloat s) const {
  return vec3(x * s, y * s, z * s);
}

const vec3 vec3::operator /(GLfloat s) const {
  return vec3(x / s, y / s, z / s);
}

const vec3 vec3::Cross(const vec3& o) const {
  return vec3(
    y * o.z - z * o.y,
    z * o.x - x * o.z,
    x * o.y - y * o.x);
}

const vec3 vec3::Cross(const vec3& a, const vec3& b) {
  return vec3(
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x);
}

GLfloat vec3::MagnitudeSQ() const {
  return x*x + y*y + z*z;
}

GLfloat vec3::Magnitude() const {
  return sqrt(MagnitudeSQ());
}

const vec3 vec3::Normalized() const {
  const GLfloat m = Magnitude();
  if (m == 0.0f) return vec3::vec3();
  return vec3(x / m, y / m, z / m);
}

void vec3::Normalize() {
  const GLfloat m = Magnitude();
  if (m == 0.0f) return;
  x /= m;
  y /= m;
  z /= m;
}

