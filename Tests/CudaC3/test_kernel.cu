
#include <cuda.h>

inline __global__ void KernelTest(float * data, int width, int height)
{
	// calculate normalized texture coordinates
    unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y*blockDim.y + threadIdx.y;

	return;
}
