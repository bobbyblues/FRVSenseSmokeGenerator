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

FIND_PACKAGE(OpenMP REQUIRED)
if(OPENMP_FOUND)
	#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}) 
	#set(CCMAKE_C_FLAGS ${CCMAKE_C_FLAGS} ${OpenMP_C_FLAGS})
	#set(${CMAKE_EXE_LINKER_FLAGS} -lgomp)
endif()

# ---------------
SUBDIRS(Tests
		Code
		Ressources
		Subprojects)

