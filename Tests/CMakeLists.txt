SUBDIRS(
	Perlin2D
	Perlin3D
	TestQt
)

IF(CUDA_FOUND)
	SUBDIRS(Cuda
		CudaC3
		CudaC3Texture)
ENDIF()