cmake_minimum_required(VERSION 3.13)
project(EksamenECS17)

set(CMAKE_CXX_STANDARD 17)

add_executable(EksamenECS17 main.cpp src/ECS/ECS.h src/ECS/Entity.cpp src/ECS/Entity.h src/ECS/Component.h src/ECS/System.h Graphics/Display.cpp Graphics/Display.h Graphics/Shader.h Graphics/Mesh.h src/math/vec3.h src/math/matrix.h Graphics/Plane.h Graphics/FrameBuffer.h Graphics/Primitives.cpp Graphics/Primitives.h src/math/matrix.cpp src/math/vec3.cpp)

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL -lGLEW")
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${EksamenECS17_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
target_link_libraries(EksamenECS17 ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
target_link_libraries(EksamenECS17 "-framework OpenGL" GLEW GLFW)
