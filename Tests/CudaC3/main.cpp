#include <cutil_inline.h>
#include <iostream>

// Pour lancer le kernel
extern "C" 
void runKernel(int * a, int * b, int * res, int size);


int main()
{
	int width = 512;
	int height = 512;

	// Creation du device
	cutilSafeCall( cudaSetDevice( cutGetMaxGflopsDeviceId() ) );

	// Creation des buffers sur CPU
	int * a = new int[width*height];
	int * b = new int[width*height];
	int * res = new int[width*height];
	for(int i = 0; i < width*height; i++)
	{
		a[i] = (int)ceil(((double)rand()/ (double)RAND_MAX)*100);
		b[i] = (int)ceil(((double)rand()/ (double)RAND_MAX)*100);
	}

	// Allocation des objects on the device
	// *** data
	unsigned int size = width * height * sizeof(int);
	std::cout << "Allocation d'un buffer de taille " << width*height << "\n";
	int* d_a = NULL;
	int* d_b = NULL;
	int* d_res = NULL;
    cutilSafeCall( cudaMalloc( (void**) &d_a, size));
	cutilSafeCall( cudaMalloc( (void**) &d_b, size));
	cutilSafeCall( cudaMalloc( (void**) &d_res, size));

	// Copy des donnees
	cutilSafeCall( cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice));
	cutilSafeCall( cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice));

    // Lancer le calcul
	std::cout << "Lancer le kernel ... \n";
	runKernel(d_a, d_b, d_res, width*height);
	cutilSafeCall( cutilDeviceSynchronize() );

	// Copie DeviceHost
	cutilSafeCall( cudaMemcpy(res, d_res, size, cudaMemcpyDeviceToHost));

	// Verification du test
	int i = 0;
	for(;i < width*height;i++)
		if(res[i] != a[i] + b[i])
			std::cout << "Error : [" << i << "] " << res[i] << " != " << a[i] << " + " << b[i] << std::endl;


	// Liberation des ressources
	// *** Device
	cutilSafeCall(cudaFree(d_a));
	cutilSafeCall(cudaFree(d_b));
	cutilSafeCall(cudaFree(d_res));
	// *** CPU
	delete[] res;
	delete[] a;
	delete[] b;

	// Close device
	cutilDeviceReset();
	std::cout << "Test result : " << ((i == width*height) ? "Succes" : "Error" ) <<  std::endl;
	std::cout.flush();

	return 0;
}
