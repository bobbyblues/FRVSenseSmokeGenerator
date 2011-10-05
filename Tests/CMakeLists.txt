SUBDIRS(
	Perlin2D
	Perlin3D
)

IF(FOUND_CUDA)
	SUBDIRS(Cuda
		CudaC3
		CudaC3Texture)
ENDIF()