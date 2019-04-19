#ifndef EKSAMENECS17_MESH_H
#define EKSAMENECS17_MESH_H

#include <functional>
#include "GL/glew.h"
#include "./Shader.h"
#include "./../Math/Matrix.h"
#include "./Primitives.h"

class StaticMesh {
public:
  GLuint vertexArrayObject;
  GLuint vertexBufferObjects[3];
  GLuint count;
  StaticMesh(Shape&& shape, Matrix* matrices, unsigned int numMatrices) {
    // Create the ArrayObject and BufferObjects
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(3, &vertexBufferObjects[0]);
    // Begin describing the data used
    glBindVertexArray(vertexArrayObject);

    // The vertex data
    // layout (location = 0) in vec3 position;
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
    glBufferData(GL_ARRAY_BUFFER, shape.numVertices * 3 * sizeof(GLfloat), shape.vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // layout (location = 1) in mat4 model;
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
    glBufferData(GL_ARRAY_BUFFER, numMatrices * sizeof(Matrix), matrices, GL_STATIC_DRAW);
    for (unsigned int i = 0; i < 4; i++) {
      glEnableVertexAttribArray(1 + i);
      glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (void*)(sizeof(GLfloat) * i * 4));
      glVertexAttribDivisor(1 + i, 1);
    }

    // The vertex face indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferObjects[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.numIndices * 3 * sizeof(GLushort), shape.indices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, nullptr);
    // layout (location = 1) in vec3 color;
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(sizeof(GLfloat) * 3));
    // Release etc..
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    count = shape.numIndices * 3;
  }
  GLuint Use() {
    glBindVertexArray(vertexArrayObject);
    return count;
  }
  ~StaticMesh() {
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(3, &vertexBufferObjects[0]);
  }
};

#endif
