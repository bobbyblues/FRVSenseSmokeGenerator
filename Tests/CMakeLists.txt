SUBDIRS(
	Perlin2D
	Perlin3D
	TestQt
)

IF(FOUND_CUDA)
	SUBDIRS(Cuda
		CudaC3
		CudaC3Texture)
ENDIF()