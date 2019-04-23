#ifndef EKSAMENECS17_PLANE_H
#define EKSAMENECS17_PLANE_H

#include <functional>
#include "GL/glew.h"
#include "../src/program.h"
#include "./../Math/Matrix.h"

class PlaneVertexShader : public GPU_Shader<GL_VERTEX_SHADER> {
public:
  inline static const char* SOURCE = R"(
    #version 150 core
    in vec3 vPosition;  // The vertex in the LOCAL coordinate system
    uniform mat4 mM;    // The matrix for the pose of the model
    uniform mat4 mV;    // The matrix for the pose of the camera
    uniform mat4 mP;    // The matrix for 3d->2d projection
    uniform float fTime;
    out vec4 color;

    float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
    vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
    vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

    float noise(vec3 p){
      vec3 a = floor(p);
      vec3 d = p - a;
      d = d * d * (3.0 - 2.0 * d);
      vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
      vec4 k1 = perm(b.xyxy);
      vec4 k2 = perm(k1.xyxy + b.zzww);
      vec4 c = k2 + a.zzzz;
      vec4 k3 = perm(c);
      vec4 k4 = perm(c + 1.0);
      vec4 o1 = fract(k3 * (1.0 / 41.0));
      vec4 o2 = fract(k4 * (1.0 / 41.0));
      vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
      vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);
      return o4.y * d.y + o4.x * (1.0 - d.y);
    }

    void main() {
      vec4 pos0 = mV * mM * vec4(vPosition, 1.0);
      float e = abs(pos0.x);
      pos0.y = pos0.y - noise(pos0.xyz * ((fTime * 0.2) + 100)) * 0.15;
      gl_Position = mP * pos0;
      float r = 0.5 * sin(2 * fTime) + 0.5;
      float g = 0.4 * cos(3 * (pos0.y * pos0.y * pos0.y)) + 0.6;
      float b = 0.4 * sin(3 * (pos0.x * pos0.x * pos0.x)) + 0.6;
      float a = 1 / (pos0.z / -1.75);
      color = vec4(r, g, b, clamp(a*a*a, 0, 1) * 0.25);
    }
  )";
};

class PlaneFragmentShader : public GPU_Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char* SOURCE = R"(
    #version 150 core
    in vec4 color;
    out vec4 outColor;
    void main() {
      outColor = color;
    }
  )";
};

class Plane {
  GLuint vBufferObject;
  GLuint vBufferIndices;
  GLuint vArrayObject;
  GLuint uniforms[4];
  GPU_Program<PlaneVertexShader, PlaneFragmentShader> shader;
  unsigned int numVertices;
  unsigned int numTriangles;
public:
  Plane(unsigned int resolution, float unitSize) {

    // Generate Mesh
    numVertices = resolution * resolution;
    numTriangles = (resolution - 1) * (resolution - 1) * 6;
    GLfloat vertices[numVertices * 3];
    GLushort triangles[numTriangles];
    float sx = (resolution - 1.0f) / -2.0f * unitSize;
    float sz = (resolution - 1.0f) / 2.0f * unitSize;
    unsigned int vertexIndex = 0;
    unsigned int triangleIndex = 0;
    for (unsigned int i = 0; i < numVertices; i++) {
      unsigned int y = i / resolution;
      unsigned int x = i % resolution;
      vertices[vertexIndex++] = sx + (x / (float) resolution) * resolution * unitSize;
      vertices[vertexIndex++] = 0.0f;
      vertices[vertexIndex++] = sz - (y / (float) resolution) * resolution * unitSize;
      if (x < (resolution - 1) && y < (resolution - 1)) {
        triangles[triangleIndex++] = i;
        triangles[triangleIndex++] = i + resolution + 1;
        triangles[triangleIndex++] = i + resolution;
        triangles[triangleIndex++] = i + resolution + 1;
        triangles[triangleIndex++] = i;
        triangles[triangleIndex++] = i + 1;
      }
    }

    // Create VectorArrayObject
    glGenVertexArrays(1, &vArrayObject);
    glBindVertexArray(vArrayObject);

    // Create VectorBufferObject
    glGenBuffers(1, &vBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vBufferObject);
    glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &vBufferIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vBufferIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numTriangles * sizeof(GLushort), triangles, GL_STATIC_DRAW);

    // Apply Layout/Attributes
    GLint position = glGetAttribLocation(shader.program, "vPosition");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    uniforms[0] = glGetUniformLocation(shader.program, "mM");
    uniforms[1] = glGetUniformLocation(shader.program, "mV");
    uniforms[2] = glGetUniformLocation(shader.program, "mP");
    uniforms[3] = glGetUniformLocation(shader.program, "fTime");
  }

  void Draw(const Matrix& mP, const Matrix& mV, const Matrix& mM) {
    glUseProgram(shader.program);
    glBindVertexArray(vArrayObject);
    glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, (GLfloat*) &mM);
    glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, (GLfloat*) &mV);
    glUniformMatrix4fv(uniforms[2], 1, GL_FALSE, (GLfloat*) &mP);
    glUniform1f(uniforms[3],  glfwGetTime());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_SHORT, nullptr);
  }

  ~Plane() {
    glDeleteVertexArrays(1, &vArrayObject);
    glDeleteBuffers(1, &vBufferObject);
  }
};

#endif
