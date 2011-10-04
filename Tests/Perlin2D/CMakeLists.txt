add_executable(Perlin2D
	main.cpp
	perlin2d.cpp
)

install(TARGETS Perlin2D RUNTIME DESTINATION ${CMAKE_BINARY_DIR})
