# Copyright (c) 2012 Marwan Abdellah <abdellah.marwan@gmail.com>

# Minimum required CMake version 
cmake_minimum_required(VERSION 2.6)

# Basic_Collision
PROJECT(Basic_Collision)

# Add CMake modules 
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

# OpenGL & GLU & GLUT  
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(SDL REQUIRED)

IF(OPENGL_FOUND)
  MESSAGE(STATUS "OpenGL found...")
  MESSAGE("-- OpenGL include directory : ${OPENGL_INCLUDE_DIR}")
  MESSAGE("-- OpenGL library directory : ${OPENGL_LIBRARY}")
  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR}/Headers)
  LINK_LIBRARIES(${OPENGL_LIBRARY})
ELSE(OPENGL_FOUND)
  MESSAGE(FATAL_ERROR "OpenGL not found")
ENDIF(OPENGL_FOUND)

IF(GLUT_FOUND)
  MESSAGE(STATUS  "GLUT found...")
  MESSAGE("GLUT include directory : ${GLUT_INCLUDE_DIR}")
  MESSAGE("GLUT library directory : ${GLUT_LIBRARY}")
  INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR}/Headers)
  LINK_LIBRARIES(${GLUT_LIBRARY})
ELSE(GLUT_FOUND)
  MESSAGE(FATAL_ERROR "GLUT not found")
ENDIF(GLUT_FOUND)

IF(SDL_FOUND)
  MESSAGE(STATUS "SDL found...")
  MESSAGE("-- SDL include directory : ${SDL_INCLUDE_DIR}")
  MESSAGE("-- SDL library directory : ${SDL_LIBRARY}")
  INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
  LINK_LIBRARIES(${SDL_LIBRARY})
ELSE(SDL_FOUND)
  MESSAGE(FATAL_ERROR "SDL not found")
ENDIF(SDL_FOUND)

# Add the heade files to the include directories
INCLUDE_DIRECTORIES("${OPENGL_INCLUDE_DIR}")

# Generate the executable 
ADD_EXECUTABLE(basic_collision Basic_Collision.cpp)

