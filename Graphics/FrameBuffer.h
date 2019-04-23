#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#ifndef EKSAMENECS17_FRAMEBUFFER_H
#define EKSAMENECS17_FRAMEBUFFER_H

#include "GL/glew.h"
#include "./../Math/Vector.h"
#include "../src/program.h"
#include <iostream>
#include <vector>
#include <functional>

class BlurVertexShader : public GPU_Shader<GL_VERTEX_SHADER> {
public:
  inline static const char* SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoords;
    out vec2 TexCoords;
    void main() {
      TexCoords = aTexCoords;
      gl_Position = vec4(aPos, 1.0);
    }
  )";
};

class BloomVertexShader : public GPU_Shader<GL_VERTEX_SHADER> {
public:
  inline static const char* SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoords;
    out vec2 TexCoords;
    void main() {
      TexCoords = aTexCoords;
      gl_Position = vec4(aPos, 1.0);
    }
  )";
};

class BlurFragmentShader : public GPU_Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char* SOURCE = R"(
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoords;
    uniform sampler2D image;
    uniform bool horizontal;
    uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
    void main() {
      vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
      vec3 result = texture(image, TexCoords).rgb * weight[0];
      if(horizontal) {
        for(int i = 1; i < 5; ++i) {
          result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
          result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
      } else {
        for(int i = 1; i < 5; ++i) {
        result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
      }
      FragColor = vec4(result, 1.0);
    }
  )";
};

class BloomFragmentShader : public GPU_Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char* SOURCE = R"(
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoords;
    uniform sampler2D scene;
    uniform sampler2D bloomBlur;
    uniform bool bloom;
    uniform float exposure;
    void main() {
      const float gamma = 2.2;
      vec3 hdrColor = texture(scene, TexCoords).rgb;
      vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
      if(bloom)
          hdrColor += bloomColor; // additive blending
      // tone mapping
      vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
      // also gamma correct while we're at it
      result = pow(result, vec3(1.0 / gamma));
      FragColor = vec4(result, 1.0);
    }
  )";
};



class FrameBuffer {
  GLuint fBufferObject;
  GLuint colorBuffers[2];
  GLuint rDepthBuffer;
  GLuint attachments[2];
  GLuint pingpongFBO[2];
  GLuint pingpongColors[2];
  GLuint quadVAO = 0;
  GLuint quadVBO;
  GPU_Program<BlurVertexShader, BlurFragmentShader> blurShader;
  GPU_Program<BloomVertexShader, BloomFragmentShader> bloomShader;
public:
  FrameBuffer() {
    // Create Frame Buffer Object
    glGenFramebuffers(1, &fBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, fBufferObject);

    // Create two texture buffers
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++) {
      glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 640.0f, 480.0f, 0, GL_RGB, GL_FLOAT, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }

    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering

    // Create and attach depth buffer
    glGenRenderbuffers(1, &rDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, rDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 640.0f, 480.0f);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rDepthBuffer);
    attachments[0] = GL_COLOR_ATTACHMENT0;
    attachments[1] = GL_COLOR_ATTACHMENT1;
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColors);
    for (unsigned int i = 0; i < 2; i++) {
      glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
      glBindTexture(GL_TEXTURE_2D, pingpongColors[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 640.0f, 480.0f, 0, GL_RGB, GL_FLOAT, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColors[i], 0);
      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    }
  }

  void Capture(std::function<void()> fn) {
    blurShader.Use();
    blurShader.SetUniformInt("image", 0);
    bloomShader.Use();
    bloomShader.SetUniformInt("scene", 0);
    bloomShader.SetUniformInt("bloomBlur", 1);
    glBindFramebuffer(GL_FRAMEBUFFER, fBufferObject);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fn();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    bool horizontal = true;
    bool first_iteration = true;
    blurShader.Use();
    unsigned int amount = 10;
    for (unsigned int i = 0; i < amount; i++) {
      glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
      blurShader.SetUniformInt("horizontal", horizontal);
      glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColors[!horizontal]);
      RenderQuad();
      horizontal = !horizontal;
      if (first_iteration) first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    bloomShader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongColors[!horizontal]);

    bloomShader.SetUniformInt("bloom", true);
    bloomShader.SetUniformFloat("exposure", 1.0f);
    RenderQuad();
  }


  void RenderQuad() {
    if (quadVAO == 0) {
      float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
      };
      // setup plane VAO
      glGenVertexArrays(1, &quadVAO);
      glGenBuffers(1, &quadVBO);
      glBindVertexArray(quadVAO);
      glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
  }

  ~FrameBuffer() {
    // glDeleteFramebuffers(1, &fBufferObject);
    // glDeleteTextures(1, &textureObject);
  }
};

#endif

#pragma clang diagnostic pop


/*
std::vector<vec3> lightPositions;
lightPositions.emplace_back(vec3 {  0.0f,  0.5f,  1.5f });
lightPositions.emplace_back(vec3 { -4.0f,  0.5f, -3.0f });
lightPositions.emplace_back(vec3 {  3.0f,  0.5f,  1.0f });
lightPositions.emplace_back(vec3 {  0.0f,  5.0f, -1.0f });

std::vector<vec3> lightColors;
lightColors.emplace_back(vec3 {  5.0f,  5.0f,  5.0f });
lightColors.emplace_back(vec3 { 10.0f,  0.0f,  0.0f });
lightColors.emplace_back(vec3 {  3.0f,  0.0f, 15.0f });
lightColors.emplace_back(vec3 {  0.0f,  5.0f,  0.0f });
 */
