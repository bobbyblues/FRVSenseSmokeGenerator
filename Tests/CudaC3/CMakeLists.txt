# The cuda_include_directories adds paths to only cuda compilation.
CUDA_INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES(
	${CUDA_TOOLKIT_INCLUDE}
	${CUDA_SDK_ROOT_DIR}/common/inc
)

# Set CUDA_ATTACH_VS_BUILD_RULE_TO_CUDA_FILE when you want to add the same .cu
# file to multiple targets.
set(CUDA_ATTACH_VS_BUILD_RULE_TO_CUDA_FILE OFF)

# Create Exe
CUDA_ADD_EXECUTABLE(CudaC3
	test_kernel.cu
	main.cpp
)

install(TARGETS CudaC3 RUNTIME DESTINATION ${CMAKE_BINARY_DIR})

# Add a special target to clean nvcc generated files.
CUDA_BUILD_CLEAN_TARGET()
