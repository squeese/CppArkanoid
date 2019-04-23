#ifndef EKSAMENECS17_MESH_H
#define EKSAMENECS17_MESH_H

#include <memory>
#include "GL/glew.h"
#include "shapes.h"

class Mesh {
public:
  virtual GLuint Use() = 0;
};

enum MeshBufferTypes { NORMAL, UV, ELEMENTS };

template<MeshBufferTypes... BufferTypes>
class ArrayMesh : public Mesh {
public:
  GLuint VAO = 0;
  GLuint VBO[sizeof...(BufferTypes) + 1] = { 0 };
  GLuint count = 0;

  ArrayMesh(const std::shared_ptr<Shape>& shape) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(sizeof...(BufferTypes) + 1, &VBO[0]);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, shape->numVertices * 3 * sizeof(GLfloat), shape->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    count = shape->numVertices;
    BindBuffer<1, 1, BufferTypes...>(shape);
    glBindVertexArray(0);
  }

  template <size_t VBOIndex, size_t ATTRIBIndex>
  void BindBuffer(const std::shared_ptr<Shape>& shape) {}

  template <size_t VBOIndex, size_t ATTRIBIndex, MeshBufferTypes BufferType, MeshBufferTypes... Rest>
  void BindBuffer(const std::shared_ptr<Shape>& shape) {
    switch (BufferType) {
      case UV: {
        glBindBuffer(GL_ARRAY_BUFFER, VBO[VBOIndex]);
        glBufferData(GL_ARRAY_BUFFER, shape->numVertices * 2 * sizeof(GLfloat), shape->uvs, GL_STATIC_DRAW);
        glEnableVertexAttribArray(ATTRIBIndex);
        glVertexAttribPointer(ATTRIBIndex, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        return BindBuffer<VBOIndex + 1, ATTRIBIndex + 1, Rest...>(shape);
      }
      case NORMAL: {
        glBindBuffer(GL_ARRAY_BUFFER, VBO[VBOIndex]);
        glBufferData(GL_ARRAY_BUFFER, shape->numVertices * 3 * sizeof(GLfloat), shape->normals, GL_STATIC_DRAW);
        glEnableVertexAttribArray(ATTRIBIndex);
        glVertexAttribPointer(ATTRIBIndex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        return BindBuffer<VBOIndex + 1, ATTRIBIndex + 1, Rest...>(shape);
      }
      case ELEMENTS: {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[VBOIndex]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape->numTriangles * 3 * sizeof(GLushort), shape->triangles,
                     GL_STATIC_DRAW);
        count = shape->numTriangles * 3;
        return BindBuffer<VBOIndex + 1, ATTRIBIndex, Rest...>(shape);
      }
    }
  }

  GLuint Use() override {
    glBindVertexArray(VAO);
    return count;
  }

  ~ArrayMesh() {
    glDeleteBuffers(sizeof...(BufferTypes) + 1, &VBO[0]);
    glDeleteVertexArrays(1, &VAO);
  }
};

#endif
