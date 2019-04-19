#ifndef EKSAMENECS17_MATRIX_H
#define EKSAMENECS17_MATRIX_H

#include "GL/glew.h"
#include "vec3.h"

struct matrix {
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
  const vec3 operator *(const vec3& o) const;
  const matrix operator *(const matrix& o) const;
  static const matrix Translation(const vec3& translation);
  static const matrix Translation(GLfloat x, GLfloat y, GLfloat z);
  static const matrix EulerRotationZXY(const vec3& euler);
  static const matrix EulerRotationZXY(GLfloat x, GLfloat y, GLfloat z);
  static const matrix Scale(const vec3& scalars);
  static const matrix Scale(GLfloat x, GLfloat y, GLfloat z);
  static const matrix Identity();
  static const matrix Transform(const vec3& position, const vec3& rotation, const vec3& scale);
  static const matrix Perspective(GLfloat fov, GLfloat ratio, GLfloat near, GLfloat far);
  static const matrix LookAt(const vec3& position, const vec3& target, const vec3& up);
};

#endif
