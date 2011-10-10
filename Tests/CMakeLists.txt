SUBDIRS(
	Perlin2D
	Perlin3D
	TestQtC3
)

IF(CUDA_FOUND)
	SUBDIRS(Cuda
		CudaC3
		CudaC3Texture
		CudaVolumeRender
	)
ENDIF()