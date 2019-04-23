#ifndef EKSAMENECS17_PROGRAM_H
#define EKSAMENECS17_PROGRAM_H

#include <iostream>
#include "math/matrix.h"

template<GLenum TYPE>
class Shader {
public:
  static GLuint Compile(GLint *success, const char *source) {
    GLuint shader = glCreateShader(TYPE);
    if (shader == 0) {
      std::cout << "Error: glCreateShader failed" << std::endl;
      *success = GL_FALSE;
      return shader;
    }
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, success);
    if (*success == GL_TRUE) return shader;
    GLchar error[512];
    glGetShaderInfoLog(shader, sizeof(error), nullptr, error);
    std::cout << "Error: glCompileShader failed: " << error << std::endl;
    return shader;
  }
};

template<class... Shaders>
class Program {
  template<size_t i>
  bool CompileShaders() {
    return true;
  }
  template<size_t i, class Shader, class... REST>
  bool CompileShaders() {
    GLint success;
    GLuint shader = Shader::Compile(&success, Shader::SOURCE);
    if (success == GL_TRUE) {
      shaders[i] = shader;
      // fn(i, shader);
      return CompileShaders<i + 1, REST...>();
    }
    return false;
  }
public:
  GLint success;
  GLuint program;
  GLuint shaders[sizeof...(Shaders)];
  Program() {
    CompileShaders<0, Shaders...>();
    program = glCreateProgram();
    for (unsigned int i = 0; i < sizeof...(Shaders); i++)
      glAttachShader(program, shaders[i]);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
      GLchar error[512];
      glGetProgramInfoLog(program, sizeof(error), NULL, error);
      std::cout << "Error: glLinkProgram failed: " << error << std::endl;
    }
    glValidateProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
      GLchar error[512];
      glGetProgramInfoLog(program, sizeof(error), NULL, error);
      std::cout << "Error: glValidateProgram failed: " << error << std::endl;
    }
    for (unsigned int i = 0; i < sizeof...(Shaders); i++)
      glDeleteShader(shaders[i]);
  }
  ~Program() {
    glDeleteProgram(program);
  }
  void Use() {
    glUseProgram(program);
  }
  void SetUniformInt(const std::string &key, int value) const {
    glUniform1i(glGetUniformLocation(program, key.c_str()), value);
  }
  void SetUniformFloat(const std::string &key, float value) const {
    glUniform1f(glGetUniformLocation(program, key.c_str()), value);
  }
  void SetUniformVec3(const std::string &name, const Vec3 &value) const {
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value.x);
  }
  void SetUniformMat4(const std::string &name, const Mat4 &matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, (GLfloat *) &matrix);
  }
};

#endif

