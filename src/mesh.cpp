#include "mesh.h"
/*

Mesh::Mesh(const std::shared_ptr<Shape>& shape) {
  count = shape->numTriangles * 3;
  // 1) Create the VAO object and the VBO objects
  glGenVertexArrays(1, &vertexArrayObject);
  glGenBuffers(4, &vertexBufferObjects[0]);
  // 2) Bind to the VAO object as we begin to describe the data 'going to be used'
  glBindVertexArray(vertexArrayObject);

  // 3.1) Move the vertex position data to the VBO[0] buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
  glBufferData(GL_ARRAY_BUFFER, shape->numVertices * 3 * sizeof(GLfloat), shape->vertices, GL_STATIC_DRAW);
  // 3.2) Describe the data to => layout (location = 0) in vec3 position;
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // 5.1) Move the uv data to the VBO[2] buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
  glBufferData(GL_ARRAY_BUFFER, shape->numVertices * 2 * sizeof(GLfloat), shape->uvs, GL_STATIC_DRAW);
  // 5.2) Describe the data to => layout (location = 1) in vec2 uv;
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  // 4.1) Move the vertex normals data to the VBO[1] buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[2]);
  glBufferData(GL_ARRAY_BUFFER, shape->numVertices * 3 * sizeof(GLfloat), shape->normals, GL_STATIC_DRAW);
  // 4.2) Describe the data to => layout (location = 1) in vec3 normal;
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


  // 6) Move the triangle indices data to the VBO[3] buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferObjects[3]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape->numTriangles * 3 * sizeof(GLushort), shape->triangles, GL_STATIC_DRAW);
  // ---) Instanced. data etc...
  // glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[2]);
  // glBufferData(GL_ARRAY_BUFFER, numMatrices * sizeof(Matrix), matrices, GL_STATIC_DRAW);
  // for (unsigned int i = 0; i < 4; i++) {
  //   glEnableVertexAttribArray(1 + i);
  //   glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (void*)(sizeof(GLfloat) * i * 4));
  //   glVertexAttribDivisor(1 + i, 1);
  // }
  // 5) Release the VAO and VBO's
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint Mesh::Use() {
  glBindVertexArray(vertexArrayObject);
  return count;
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vertexArrayObject);
  glDeleteBuffers(4, &vertexBufferObjects[0]);
}
 */
