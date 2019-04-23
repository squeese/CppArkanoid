#include "window.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Window::Window(int width, int height, const std::string& title) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
  context = SDL_GL_CreateContext(window);
  glewExperimental = GL_TRUE;
  GLenum status = glewInit();
  if (status != GLEW_OK) {
    std::cerr << "GLEW failed to initialize" << std::endl;
    exit(EXIT_FAILURE);
  }
}

bool Window::HandleInput() {
  SDL_GL_SwapWindow(window);
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_KEYDOWN: {
        if (e.key.keysym.sym == SDLK_ESCAPE) return false;
        if (e.key.repeat == 1) break;
        switch (e.key.keysym.scancode) {
          case SDL_SCANCODE_W: KEY_W = true; break;
          case SDL_SCANCODE_A: KEY_A = true; break;
          case SDL_SCANCODE_S: KEY_S = true; break;
          case SDL_SCANCODE_D: KEY_D = true; break;
        }
        break;
      }
      case SDL_KEYUP:
        switch (e.key.keysym.scancode) {
          case SDL_SCANCODE_W: KEY_W = false; break;
          case SDL_SCANCODE_A: KEY_A = false; break;
          case SDL_SCANCODE_S: KEY_S = false; break;
          case SDL_SCANCODE_D: KEY_D = false; break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        break;
      case SDL_MOUSEBUTTONUP:
        break;
      case SDL_MOUSEMOTION:
        break;
      case SDL_QUIT:
        return false;
    }
  }
  return true;
}

Window::~Window() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  context = nullptr;
  window = nullptr;
}
