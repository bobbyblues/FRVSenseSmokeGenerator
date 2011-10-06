PROJECT(FRVSenseSmokeGenerator)

# ---------------
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Config ${CMAKE_MODULE_PATH})

# ---------------
MESSAGE("Dependancies search ... ")
FIND_PACKAGE(OpenGL)
IF(OPENGL_FOUND)
  MESSAGE(STATUS "Find OpenGL")
ELSE()
  MESSAGE(FATAL_ERROR "You must install OpenGL")
ENDIF()

find_package(CUDA)
IF(CUDA_FOUND)
	MESSAGE(STATUS "Find CUDA")
ELSE()
    MESSAGE(STATUS "NOT Find CUDA")
ENDIF()
find_package(GLEW REQUIRED)
find_package(Qt4 REQUIRED QtCore QtGui)

# ---------------
SUBDIRS(Tests
		Code
		Ressources
		Subprojects)

