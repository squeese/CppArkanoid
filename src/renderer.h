#ifndef EKSAMENECS17_RENDERER_H
#define EKSAMENECS17_RENDERER_H

#include <functional>
#include "GL/glew.h"
#include "program.h"
#include "mesh.h"

class GenericSceneVertexShader : public Shader<GL_VERTEX_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 vPosition;
    layout (location = 1) in vec2 vUv;
    layout (location = 2) in vec3 vNormal;
    uniform mat4 mP;
    uniform mat4 mV;
    uniform mat4 mM;
    uniform vec3 lightAmbient;
    uniform vec3 lightPosition;
    out vec3 fPosition;
    out vec2 fUv;
    out vec3 fNormal;
    void main() {
      fNormal = mat3(mM) * vNormal;
      vec4 mPosition = mM * vec4(vPosition, 1.0);
      fPosition = mPosition.xyz;
      fUv = vUv;
      gl_Position = mP * mV * mPosition;
    }
  )";
};

class GenericSceneFragmentShader : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) out vec4 FragColor;
    layout (location = 1) out vec4 EmitColor;
    in vec3 fPosition;
    in vec3 fNormal;
    in vec2 fUv;
    uniform vec3 lightAmbient;
    uniform vec3 lightPosition;
    const float eThickness = 2.0;
    const float eSharpness = 6.0;
    const float eSubtract = 0.2;
    const float eStrength = 10.0f;
    void main() {
      vec3 lv = normalize(lightPosition - fPosition);
      float b = clamp(dot(lv, fNormal), 0, 1);
      FragColor = vec4(b, b, b, 1.0);
      vec2 uv = abs(fUv - 0.5) * eThickness;
      uv = pow(uv, vec2(eSharpness)) - eSubtract;
      float c = clamp(uv.x + uv.y, 0.0, 1.0) * eStrength;
      EmitColor = vec4(c, c, c, 1.0);
    }
  )";
};

class GenericSceneVertexShader2 : public Shader<GL_VERTEX_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 vPosition;
    layout (location = 1) in vec2 vUv;
    layout (location = 2) in vec3 vNormal;
    uniform mat4 mP;
    uniform mat4 mV;
    uniform mat4 mM;
    uniform vec3 lightAmbient;
    uniform vec3 lightPosition;
    out vec3 fPosition;
    out vec2 fUv;
    out vec3 fNormal;
    void main() {
      fNormal = mat3(mM) * vNormal;
      vec4 mPosition = mM * vec4(vPosition, 1.0);
      fPosition = mPosition.xyz;
      fUv = vUv;
      gl_Position = mP * mV * mPosition;
    }
  )";
};

class GenericSceneFragmentShader2 : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    // layout (location = 0) out vec4 FragColor;
    // layout (location = 1) out vec4 EmitColor;
    out vec4 FragColor;
    in vec3 fPosition;
    in vec3 fNormal;
    in vec2 fUv;
    uniform vec3 lightAmbient;
    uniform vec3 lightPosition;
    void main() {
      FragColor = vec4(1.0, 1.0, 1.0, 1.0);
      // EmitColor = vec4(1.0, 1.0, 1.0, 1.0);
      // vec3 lv = normalize(lightPosition - fPosition);
      // float b = clamp(dot(lv, fNormal), 0, 1);
      // FragColor = vec4(b, b, b, 1.0);
      // vec2 uv = abs(fUv - 0.5) * eThickness;
      // uv = pow(uv, vec2(eSharpness)) - eSubtract;
      // float c = clamp(uv.x + uv.y, 0.0, 1.0) * eStrength;
      // EmitColor = vec4(c, c, c, 1.0);
    }
  )";
};

class QuadVertexShader : public Shader<GL_VERTEX_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 vPos;
    layout (location = 1) in vec2 vUv;
    out vec2 fUv;
    void main() {
      gl_Position = vec4(vPos, 1.0);
      fUv = vUv;
    }
  )";
};

class QuadFragmentShader : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    in vec2 fUv;
    uniform sampler2D image;
    out vec4 FragColor;
    void main() {
      FragColor = texture(image, fUv);
    }
  )";
};

class BlurFragmentShader : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    out vec4 FragColor;
    in vec2 fUv;
    uniform sampler2D imageEmitColor;
    uniform bool horizontal;
    uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
    void main() {
      vec2 tex_offset = 1.0 / textureSize(imageEmitColor, 0);
      vec3 result = texture(imageEmitColor, fUv).rgb * weight[0];
      if(horizontal) {
        for(int i = 1; i < 5; ++i) {
          result += texture(imageEmitColor, fUv + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
          result += texture(imageEmitColor, fUv - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
      } else {
        for(int i = 1; i < 5; ++i) {
          result += texture(imageEmitColor, fUv + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
          result += texture(imageEmitColor, fUv - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
      }
      FragColor = vec4(result, 1.0);
    }
  )";
};

class BloomFragmentShader : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    out vec4 FragColor;
    in vec2 fUv;
    uniform sampler2D imageSceneColor;
    uniform sampler2D imageBlurColor;
    void main() {
      vec3 scene = texture(imageSceneColor, fUv).rgb;
      vec3 blur = texture(imageBlurColor, fUv).rgb;
      FragColor = vec4(scene * 0.5 + blur * 0.5, 1.0);
    }
  )";
};

// Mesh boxMeshFlat(ShapeBuilder::BoxFlat(0.15f, 0.15f, 0.15f));
// Mesh boxMeshRound(ShapeBuilder::BoxFlat(0.15f, 0.15f, 0.15f));
// Mesh dot(ShapeBuilder::BoxRound(0.15f, 0.15f, 0.15f));
// Mesh bigBoxRound(ShapeBuilder::BoxRound(1.0f, 1.0f, 1.0f));
// Mesh bigBoxFlat(ShapeBuilder::BoxFlat(1.0f, 1.0f, 1.0f));

class Renderer {
  GLuint MainFBO = 0;
  GLuint MainRBO = 0;
  GLuint MainCBO[2] = { 0 };
  GLuint BlurFBO[2] = { 0 };
  GLuint BlurCBO[2] = { 0 };

public:
  // Program<GenericSceneVertexShader, GenericSceneFragmentShader> sceneShader;
  Program<GenericSceneVertexShader, GenericSceneFragmentShader> sceneShader;
  Program<QuadVertexShader, BlurFragmentShader> blurShader;
  Program<QuadVertexShader, BloomFragmentShader> bloomShader;
  Program<QuadVertexShader, QuadFragmentShader> quadShader;
  ArrayMesh<MeshBufferTypes::UV> quad;

  explicit Renderer(GLint width, GLint height) : quad(ShapeBuilder::Quad(2.0f, 2.0f, 0)) {

    // glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenFramebuffers(1, &MainFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, MainFBO);

    glGenTextures(2, MainCBO);
    for (unsigned int i = 0; i < 2; i++) {
      glBindTexture(GL_TEXTURE_2D, MainCBO[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, MainCBO[i], 0);
    }

    glGenRenderbuffers(1, &MainRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, MainRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, MainRBO);
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(2, BlurFBO);
    glGenTextures(2, BlurCBO);
    for (unsigned int i = 0; i < 2; i++) {
      glBindFramebuffer(GL_FRAMEBUFFER, BlurFBO[i]);
      glBindTexture(GL_TEXTURE_2D, BlurCBO[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BlurCBO[i], 0);
    }

    blurShader.Use();
    blurShader.SetUniformInt("imageEmitColor", 0);
    bloomShader.Use();
    bloomShader.SetUniformInt("imageSceneColor", 0);
    bloomShader.SetUniformInt("imageBlurColor", 1);
    // quadShader.Use();
    // quadShader.SetUniformInt("image", 0);
  }

  void BeginFrame() {
    // glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, MainFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void FinalizeFrame() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    quadShader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, MainCBO[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, quad.Use());
    glBindVertexArray(0);


    /*
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    blurShader.Use();
    glActiveTexture(GL_TEXTURE0);
    for (unsigned int i = 0; i < 10; i++) {
      unsigned int horizontal = i % 2;
      glBindFramebuffer(GL_FRAMEBUFFER, BlurFBO[horizontal]);
      blurShader.SetUniformInt("horizontal", horizontal);
      glBindTexture(GL_TEXTURE_2D, (i == 0) ? MainCBO[1] : BlurCBO[1 - horizontal]);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, quad.Use());
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bloomShader.Use();
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, MainCBO[0]);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, BlurCBO[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, quad.Use());

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
     */
  }

  ~Renderer() {
    glDeleteTextures(2, MainCBO);
    glDeleteTextures(2, BlurCBO);
    glDeleteRenderbuffers(1, &MainRBO);
    glDeleteFramebuffers(2, BlurFBO);
    glDeleteFramebuffers(1, &MainFBO);
  }
};

#endif
