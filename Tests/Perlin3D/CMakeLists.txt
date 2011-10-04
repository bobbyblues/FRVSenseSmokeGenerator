add_executable(Perlin3D
	main.cpp
	perlin3d.cpp
)

install(TARGETS Perlin3D RUNTIME DESTINATION ${CMAKE_BINARY_DIR})
