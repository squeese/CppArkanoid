#ifndef EKSAMENECS17_VEC2_H
#define EKSAMENECS17_VEC2_H

#include "GL/glew.h"

struct Vec2 {
public:
  GLfloat x = 0.0f;
  GLfloat y = 0.0f;

  Vec2() = default;
  Vec2(GLfloat x, GLfloat y) : x(x), y(y) {}
};
#endif
