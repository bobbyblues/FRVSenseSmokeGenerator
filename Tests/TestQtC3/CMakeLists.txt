INCLUDE(
	${QT_USE_FILE}
)

INCLUDE_DIRECTORIES(
	 ${QT_INCLUDES}
	 ${CMAKE_CURRENT_SOURCE_DIR}
	 ${CMAKE_CURRENT_BINARY_DIR}
	 ${CMAKE_CURRENT_SOURCE_DIR}/../../Subprojects/C3/Source
)

SET(PERLIN_SRCS
    main.cpp
    perlingeneratorbutton.cpp
    mainwindow.cpp
    qc3canvas.cpp
)

QT4_WRAP_CPP(PERLIN_SRCS 
	     perlingeneratorbutton.h
	     mainwindow.h
	     qc3canvas.h
	     C3Canvas.h
	     )

QT4_WRAP_UI(PERLIN_SRCS 
	    mainForm.ui
)

ADD_EXECUTABLE(TestQtC3
	${PERLIN_SRCS}
)

TARGET_LINK_LIBRARIES(TestQtC3
	C3
	${GLEW_LIBRARY}
	${QT_LIBRARIES}
)

INSTALL(TARGETS TestQtC3 RUNTIME DESTINATION ${CMAKE_BINARY_DIR})
