#ifndef EKSAMENECS17_WINDOW_H
#define EKSAMENECS17_WINDOW_H
#define GLM_ENABLE_EXPERIMENTAL

#include <functional>
#include <string>
#include <SDL.h>

class Window {
  SDL_Window* window;
  SDL_GLContext context;
  bool isClosed = false;
public:
  using RenderFunction = std::function<void(float, float)>;
  Window(int width, int height, const std::string& title);
  ~Window();
  void RenderLoop(unsigned int fps, const RenderFunction& renderFn);
};

#endif
