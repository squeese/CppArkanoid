#ifndef EKSAMENECS17_MESH_H
#define EKSAMENECS17_MESH_H

#include <memory>
#include "GL/glew.h"
#include "shapes.h"

class Mesh {
public:
  GLuint vertexArrayObject = 0;
  GLuint vertexBufferObjects[2] = { 0 };
  GLuint count;
  explicit Mesh(const std::shared_ptr<Shape>& shape);
  ~Mesh();
  GLuint Use();
};

#endif
