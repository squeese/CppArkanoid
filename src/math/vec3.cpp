#include "vec3.h"

void Vec3::operator +=(const Vec3& o) {
  x += o.x;
  y += o.y;
  x += o.z;
}

void Vec3::operator -=(const Vec3& o) {
  x -= o.x;
  y -= o.y;
  x -= o.z;
}

const Vec3 Vec3::operator +(const Vec3& o) const {
  return Vec3(x + o.x, y + o.y, z + o.z);
}

const Vec3 Vec3::operator -(const Vec3& o) const {
  return Vec3(x - o.x, y - o.y, z - o.z);
}

const Vec3 Vec3::operator *(GLfloat s) const {
  return Vec3(x * s, y * s, z * s);
}

const Vec3 Vec3::operator /(GLfloat s) const {
  return Vec3(x / s, y / s, z / s);
}

const Vec3 Vec3::Cross(const Vec3& o) const {
  return Vec3(
    y * o.z - z * o.y,
    z * o.x - x * o.z,
    x * o.y - y * o.x);
}

const Vec3 Vec3::Cross(const Vec3& a, const Vec3& b) {
  return Vec3(
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x);
}

GLfloat Vec3::MagnitudeSQ() const {
  return x*x + y*y + z*z;
}

GLfloat Vec3::Magnitude() const {
  return sqrt(MagnitudeSQ());
}

const Vec3 Vec3::Normalized() const {
  const GLfloat m = Magnitude();
  if (m == 0.0f) return Vec3::ZERO();
  return Vec3(x / m, y / m, z / m);
}

void Vec3::Normalize() {
  const GLfloat m = Magnitude();
  if (m == 0.0f) return;
  x /= m;
  y /= m;
  z /= m;
}

