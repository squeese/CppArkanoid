cmake_minimum_required(VERSION 3.13)
project(EksamenECS17)

set(CMAKE_CXX_STANDARD 17)

# add_executable(EksamenECS17 main.cpp src/ecs/manager.h src/ecs/entity.cpp src/ecs/entity.h src/ecs/component.h src/ecs/system.h src/window.cpp src/window.h src/program.h src/mesh.h src/math/vec3.h src/math/matrix.h Graphics/Plane.h Graphics/FrameBuffer.h src/shapes.h src/math/matrix.cpp src/math/vec3.cpp src/shapes.cpp src/mesh.cpp src/program.cpp src/math/vec2.cpp src/math/vec2.h src/framebuffer.h src/renderer.h src/gamestate.h src/test.cpp)
add_executable(EksamenECS17 test.cpp src/ecs/manager.h src/ecs/entity.cpp src/ecs/entity.h src/ecs/component.h src/ecs/system.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${EksamenECS17_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
target_link_libraries(EksamenECS17 ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
target_link_libraries(EksamenECS17 "-framework OpenGL" GLEW GLFW)
