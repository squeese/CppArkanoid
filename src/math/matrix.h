#ifndef EKSAMENECS17_MAT4_H
#define EKSAMENECS17_MAT4_H

#include "GL/glew.h"
#include "vec3.h"

struct Mat4 {
  enum {
    n11, n21, n31, n41, // 0  4  8  12
    n12, n22, n32, n42, // 1  5  9  13
    n13, n23, n33, n43, // 2  6  10 14
    n14, n24, n34, n44  // 3  7  11  15
  };
  GLfloat cell[16] = {
     1.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f
  };
public:
  GLfloat& operator [](unsigned int i);
  GLfloat operator [](unsigned int i) const;
  const Vec3 operator *(const Vec3& o) const;
  const Mat4 operator *(const Mat4& o) const;
  static const Mat4 Translation(const Vec3& translation);
  static const Mat4 Translation(GLfloat x, GLfloat y, GLfloat z);
  static const Mat4 EulerRotationZXY(const Vec3& euler);
  static const Mat4 EulerRotationZXY(GLfloat x, GLfloat y, GLfloat z);
  static const Mat4 Scale(const Vec3& scalars);
  static const Mat4 Scale(GLfloat x, GLfloat y, GLfloat z);
  static const Mat4 Identity();
  static const Mat4 Transform(const Vec3& position, const Vec3& rotation, const Vec3& scale);
  static const Mat4 Perspective(GLfloat fov, GLfloat ratio, GLfloat near, GLfloat far);
  static const Mat4 LookAt(const Vec3& position, const Vec3& target, const Vec3& up);
};

#endif
