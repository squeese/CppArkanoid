#ifndef EKSAMENECS17_PRIMITIVES_H
#define EKSAMENECS17_PRIMITIVES_H

#include <GL/glew.h>
#include <cmath>
#include <iostream>
#include "./../Math/Vector.h"

struct Shape {
  ~Shape() {
    delete vertices;
    delete indices;
    delete normals;
  }
public:
  GLfloat* vertices;
  GLfloat* normals;
  GLushort* indices;
  GLuint numVertices;
  GLuint numIndices;
};

template <typename T>
void Set(T* arr, unsigned int index, T x, T y, T z) {
  unsigned int offset = index * 3;
  arr[offset] = x;
  arr[offset + 1] = y;
  arr[offset + 2] = z;
}

class Primitives {
public:
  static Shape CreateBox(float width, float height, float depth) {
    GLuint numVertices = 8;
    GLuint numIndices = 12;
    float x = abs(width / 2.0f);
    float y = abs(height / 2.0f);
    float z = abs(depth / 2.0f);
    auto vertices = new GLfloat[numVertices * 3];
    auto normals = new GLfloat[numVertices * 3];
    auto indices = new GLushort[numIndices * 3];
    Set<GLfloat>(vertices, 0, -x, +y, +z); // Front-Top-Left         0---------1
    Set<GLfloat>(vertices, 1, +x, +y, +z); // Front-Top-Right       /|        /|
    Set<GLfloat>(vertices, 2, -x, -y, +z); // Front-Bot-Left       4 | - - - 5 |
    Set<GLfloat>(vertices, 3, +x, -y, +z); // Front-Bot-Right      ; |       : |
    Set<GLfloat>(vertices, 4, -x, +y, -z); // Back-Top-Left        ; 2---------3
    Set<GLfloat>(vertices, 5, +x, +y, -z); // Back-Top-Right       ;/        :/
    Set<GLfloat>(vertices, 6, -x, -y, -z); // Back-Bot-Left        6 - - - - 7
    Set<GLfloat>(vertices, 7, +x, -y, -z); // Back-Bot-Right

    // SetNormal(normals, vertices, 0, 1, 4, 2);
    // SetNormal(normals, vertices, 1, 0, 3, 5);
    // SetNormal(normals, vertices, 2, 0, 3, 6);
    // SetNormal(normals, vertices, 3, 1, 2, 7);
    // SetNormal(normals, vertices, 4, 0, 5, 6);
    // SetNormal(normals, vertices, 5, 1, 4, 7);
    // SetNormal(normals, vertices, 6, 2, 4, 7);
    // SetNormal(normals, vertices, 7, 3, 5, 6);

    Set<GLushort>(indices, 1, 0, 2, 3);
    Set<GLushort>(indices, 0, 0, 3, 1);
    Set<GLushort>(indices, 2, 0, 4, 6);
    Set<GLushort>(indices, 3, 0, 6, 2);
    Set<GLushort>(indices, 4, 0, 1, 5);
    Set<GLushort>(indices, 5, 0, 5, 4);
    Set<GLushort>(indices, 6, 7, 3, 2);
    Set<GLushort>(indices, 7, 7, 2, 6);
    Set<GLushort>(indices, 8, 7, 6, 4);
    Set<GLushort>(indices, 9, 7, 4, 5);
    Set<GLushort>(indices, 10, 7, 5, 1);
    Set<GLushort>(indices, 11, 7, 1, 3);
    std::cout << sizeof(vertices) << std::endl;
    return Shape {
      vertices,
      indices,
      numVertices,
      numIndices
    };
  }

  static Shape CreatePyramid(float radius) { // , unsigned int subDivisions
    GLuint numVertices = 6;
    GLuint numIndices =  8;
    /*
    GLuint numEdges = 12;
    for (unsigned i = 0; i < subDivisions; i++) {
      numVertices += numIndices;
      numEdges += numIndices * 3;
      numIndices += numIndices * 2;
    }
     */
    float r = abs(radius / 2.0f);
    auto vertices = new GLfloat[numVertices * 3];
    auto indices = new GLushort[numIndices * 3];
    unsigned int vertexIndex = 0;
    unsigned int triangleIndex = 0;
    // unsigned int edgeIndex = 0;
    Set<GLfloat>(vertices, vertexIndex++, 0.0f, 0.0f, +r);
    Set<GLfloat>(vertices, vertexIndex++, 0.0f, +r,   0.0f);
    Set<GLfloat>(vertices, vertexIndex++, -r,   0.0f, 0.0f);
    Set<GLfloat>(vertices, vertexIndex++, 0.0f, -r,   0.0f);
    Set<GLfloat>(vertices, vertexIndex++, +r,   0.0f, 0.0f);
    Set<GLfloat>(vertices, vertexIndex++, 0.0f, 0.0f, -r);
    Set<GLushort>(indices, triangleIndex++, 0, 1, 2);
    Set<GLushort>(indices, triangleIndex++, 0, 2, 3);
    Set<GLushort>(indices, triangleIndex++, 0, 3, 4);
    Set<GLushort>(indices, triangleIndex++, 0, 4, 1);
    Set<GLushort>(indices, triangleIndex++, 5, 1, 4);
    Set<GLushort>(indices, triangleIndex++, 5, 4, 3);
    Set<GLushort>(indices, triangleIndex++, 5, 3, 2);
    Set<GLushort>(indices, triangleIndex++, 5, 2, 1);

    /*
    for (unsigned int s = 0; s < subDivisions; s++) {
      std::cout << "subdivision: " << (s+1) << std::endl;
      unsigned int triangleEnd = triangleIndex;

      for (unsigned int i = 0; i < triangleEnd; i++) {
        GLushort a = indices[i * 3];
        GLushort b = indices[i * 3 + 1];
        GLushort c = indices[i * 3 + 2];
        vec3 va(vertices[a * 3], vertices[a * 3 + 1], vertices[a * 3 + 2]);
        vec3 vb(vertices[b * 3], vertices[b * 3 + 1], vertices[b * 3 + 2]);
        vec3 vc(vertices[c * 3], vertices[c * 3 + 1], vertices[c * 3 + 2]);
        vec3 middle = (va + vb + vc).Normalized() * r;
        indices[i * 3] = vertexIndex;
        Set<GLushort>(indices, triangleIndex++, vertexIndex, a, b);
        Set<GLushort>(indices, triangleIndex++, vertexIndex, c, a);
        Set<GLfloat>(vertices, vertexIndex++, middle.x, middle.y, middle.z);
      }

      for (unsigned int i = 0 ; i < triangleEnd; i++) {

      }
    }
     */
    return Shape {
      vertices,
      indices,
      numVertices,
      numIndices
    };
  }
};


#endif
