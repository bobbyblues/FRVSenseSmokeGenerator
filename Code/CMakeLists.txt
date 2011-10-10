INCLUDE(
	${QT_USE_FILE}
)

INCLUDE_DIRECTORIES(
	 ${QT_INCLUDES}
	 ${CMAKE_CURRENT_SOURCE_DIR}
	 ${CMAKE_CURRENT_BINARY_DIR}
#	 ${CMAKE_CURRENT_SOURCE_DIR}/../Perlin2D
)

SET(PERLIN_SRCS
    main.cpp
    PerlinGenerator.cpp
    PerlinGenerator.h
	mainwindow.cpp
	mainwindow.h
	Exporters/Exporters.h
	Exporters/Exporters.cpp

	Exporters/ExporterPBRT.cpp
	Exporters/ExporterPBRT.h
	Exporters/ExporterRAW.cpp
	Exporters/ExporterRAW.h
)

QT4_WRAP_CPP(PERLIN_SRCS 
	     PerlinGenerator.h
	     mainwindow.h
	     )

QT4_WRAP_UI(PERLIN_SRCS 
	    mainForm.ui
)

ADD_EXECUTABLE(
	FRVSenseSmokeGenerator
	${PERLIN_SRCS}
#	${CMAKE_CURRENT_SOURCE_DIR}/../Perlin2D/perlin2d.cpp
)

TARGET_LINK_LIBRARIES(FRVSenseSmokeGenerator
	${QT_LIBRARIES}
)

INSTALL(TARGETS FRVSenseSmokeGenerator RUNTIME DESTINATION ${CMAKE_BINARY_DIR})
