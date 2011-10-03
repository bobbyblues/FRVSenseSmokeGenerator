PROJECT(FRVSenseSmokeGenerator)

# ---------------
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Config ${CMAKE_MODULE_PATH})

# ---------------
MESSAGE("Dependancies search ... ")
FIND_PACKAGE(OpenGL)
IF(OPENGL_FOUND)
  MESSAGE(STATUS "Find OpenGL")
ELSE(OPENGL_FOUND)
  MESSAGE(FATAL_ERROR "You must install OpenGL")
ENDIF(OPENGL_FOUND)

# ---------------
SUBDIRS(Tests
		Code
		Ressources)

