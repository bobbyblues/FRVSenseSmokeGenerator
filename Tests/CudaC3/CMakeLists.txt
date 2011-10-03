add_executable(CudaC3 
	main.cpp
)

install(TARGETS CudaC3 RUNTIME DESTINATION ${CMAKE_BINARY_DIR})
