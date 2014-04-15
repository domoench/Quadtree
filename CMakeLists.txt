cmake_minimum_required (VERSION 2.8)
project (Quadtree)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(SRCS
  main.cpp
  bb.cpp
  geometry.cpp
  qtnode.cpp
  scene.cpp
  shader.cpp
)

add_executable(quadtree ${SRCS})

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW)
find_package(GLM REQUIRED)
find_package(GLFW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})

target_link_libraries(quadtree ${OPENGL_LIBRARY})
target_link_libraries(quadtree ${GLFW_LIBRARY})
target_link_libraries(quadtree ${GLEW_LIBRARY})
 
