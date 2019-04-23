#ifndef EKSAMENECS17_FRAMEBUFFER_H
#define EKSAMENECS17_FRAMEBUFFER_H

#include <iostream>
#include "GL/glew.h"

class FrameBuffer {
public:
  GLuint frameBufferObject = 0;
  GLuint colorBuffers[2] = { 0 };
  GLuint renderBuffer = 0;
  GLuint second = 0;
  GLuint secondTexture = 0;
  FrameBuffer(GLfloat width, GLfloat height) {
    glGenFramebuffers(1, &frameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
    // create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
    glGenTextures(2, &colorBuffers[0]);
    for (unsigned int i = 0; i < 2; i++) {
      // glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorBuffers[i]);
      // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
      glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      // glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      /// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      // glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, colorBuffers[i], 0);
      // glBindTexture(GL_TEXTURE_2D, 0);
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    // create and attach depth buffer (renderbuffer)
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, width);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &second);
    glBindFramebuffer(GL_FRAMEBUFFER, second);
    glGenTextures(1, &secondTexture);
    glBindTexture(GL_TEXTURE_2D, secondTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, secondTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  ~FrameBuffer() {
    glDeleteRenderbuffers(1, &renderBuffer);
    glDeleteTextures(2, &colorBuffers[2]);
    glDeleteFramebuffers(1, &frameBufferObject);
    glDeleteTextures(1, &secondTexture);
    glDeleteFramebuffers(1, &second);
  }
};

class BloomBuffer {
public:
  GLuint frameBufferObjects[2] = { 0 };
  GLuint colorBuffers[2] = { 2 };
  BloomBuffer(GLfloat width, GLfloat height) {
    glGenFramebuffers(2, &frameBufferObjects[0]);
    glGenTextures(2, &colorBuffers[0]);
    for (unsigned int i = 0; i < 2; i++) {
      glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjects[i]);
      // glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorBuffers[i]);
      // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
      glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      // glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      // glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffers[i], 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorBuffers[i], 0);
      // glBindTexture(GL_TEXTURE_2D, 0);
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  ~BloomBuffer() {
    glDeleteTextures(2, &colorBuffers[0]);
    glDeleteFramebuffers(2, &frameBufferObjects[0]);
  }
};

#endif
