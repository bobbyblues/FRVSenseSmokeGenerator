
#include <cuda.h>

inline __global__ void KernelTest(int * a, int * b, int * res, int size)
{
	// Calcul de l'indice du tableau
    unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
	res[x] = a[x] + b[x];
}

extern "C"
void runKernel(int * a, int * b, int * res, int size)
{
	dim3 dimBlock(64, 1, 1);
    dim3 dimGrid(size / dimBlock.x, 1, 1);
    
	KernelTest<<< dimGrid, dimBlock, 0 >>>( a, b, res, size);
}
