#include <cutil_inline.h>
#include <iostream>

int main()
{
	int width = 512;
	int height = 512;

	// Creation du device
	cutilSafeCall( cudaSetDevice( cutGetMaxGflopsDeviceId() ) );

	// Allocation des objects
	cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc(32, 0, 0, 0, cudaChannelFormatKindFloat);
    cudaArray* cu_array;
    cutilSafeCall( cudaMallocArray( &cu_array, &channelDesc, width, height )); 

	// Gestion de la taille de la grille
	dim3 dimBlock(8, 8, 1);
    dim3 dimGrid(width / dimBlock.x, height / dimBlock.y, 1);

    // warmup
    //transformKernel<<< dimGrid, dimBlock, 0 >>>( d_data, width, height, angle);

	// Close device
	cutilDeviceReset();

	return 0;
}
