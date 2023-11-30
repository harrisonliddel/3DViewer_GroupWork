cmake_minimum_required(VERSION 3.17)
project(code)

set(CMAKE_CXX_STANDARD 14)

# Find OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
endif(NOT OPENGL_FOUND)

# Find GLUT
find_package(GLUT REQUIRED)
if(NOT GLUT_FOUND)
    message(FATAL_ERROR "GLUT not found")
endif(NOT GLUT_FOUND)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_executable(code main.cpp
)

# Link against OpenGL and GLUT libraries
target_link_libraries(code assimp::assimp glfw GLEW::GLEW)
