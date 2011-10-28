#include "PerlinGenerator.h"
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <ctime>

PerlinGenerator::PerlinGenerator():
	m_noise(NULL),
	m_Result(NULL)
{
}

PerlinGenerator::~PerlinGenerator()
{
	if(m_noise)
		delete[] m_noise;

}

void PerlinGenerator::Prepare(Perlin3DConfig& config)
{
	m_Config = config;

	// Update Attributes
	m_SizeMax = glm::ivec3(ceil(config.Size.x * pow(2.0f, config.NbOctaves - 1) / config.Step),
						   ceil(config.Size.y * pow(2.0f, config.NbOctaves - 1) / config.Step),
						   ceil(config.Size.z * pow(2.0f, config.NbOctaves - 1) / config.Step));
}

void PerlinGenerator::run(){
    progressStatus(0);

	srand((unsigned int)time(NULL));
	////////////////////
	// Clear & Allocation
	////////////////////
        //std::cout << "Allocation ... " << std::endl;
	int SizeMaxTotal = m_SizeMax.x * m_SizeMax.y * m_SizeMax.z;
        //std::cout << " * Noise :  " << SizeMaxTotal*sizeof(float) << std::endl;
        //std::cout << " * Perlin :  " << m_Config.Size.x*m_Config.Size.y*m_Config.Size.z*sizeof(float) << std::endl;

	if(m_noise)
		delete[] m_noise;
	m_noise = new float[SizeMaxTotal*m_Config.NbOctaves];
	m_Result = new Perlin3DObject(m_Config.Size);

	////////////////////
	// Creation du bruit
	///////////////////
        //std::cout << "Creation Bruit ... " << std::endl;
	for (int i = 0; i < m_Config.NbOctaves; ++i){
        for (int j = 0; j < SizeMaxTotal; ++j){
			m_noise[i*SizeMaxTotal + j] = ((float)rand()/(float)RAND_MAX);
        }
		progressStatus(((float)i/ m_Config.NbOctaves)* 25);
    }

	////////////////////
	// Calcul du bruit de perlin
	////////////////////
        //std::cout << "Bruit de perlien ... " << std::endl;
	int compteur = 0;
	#pragma omp parallel for
	for (int x = 0; x < m_Result->Size.x; ++x){
		for (int y = 0; y < m_Result->Size.y; ++y){
            for (int z = 0; z < m_Result->Size.z; ++z){
                m_Result->Data[z * m_Result->Size.x * m_Result->Size.y + y * m_Result->Size.x + x] = getPerlin(x, y, z);
            }
		}
		#pragma omp critical
        compteur++;
		progressStatus(25 + 75*((float)compteur/m_Result->Size.x));
	}

        //std::cout << "Fini ! :)" << std::endl;
    progressStatus(100);
}


float PerlinGenerator::getNoise(int i, int j, int k){
    return m_noise[k * m_SizeMax.x * m_SizeMax.y + j * m_SizeMax.x + i];
}

float PerlinGenerator::getPerlin(int x, int y, int z){
    float somme = 0;
    float p = 1;
    int f = 1;
    int i;

	for(i = 0 ; i < m_Config.NbOctaves ; ++i) {
        somme += p * fonction_bruit3D(x * f, y * f, z * f);
		p *= m_Config.Persistance;
        f *= 2;
    }
    return somme * (1 - m_Config.Persistance) / (1 - p);
}

float PerlinGenerator::fonction_bruit3D(int x, int y, int z) {
   int i = (int) (x / m_Config.Step);
   int j = (int) (y / m_Config.Step);
   int k = (int) (z / m_Config.Step);
   return interpolation_cos3D(getNoise(i, j, k), getNoise(i + 1, j, k), getNoise(i, j + 1, k), getNoise(i + 1, j + 1, k), 
							  getNoise(i, j, k + 1), getNoise(i + 1, j, k + 1), getNoise(i, j + 1, k + 1), 
							  getNoise(i + 1, j + 1, k + 1),
							  fmod(x / m_Config.Step, 1), fmod(y / m_Config.Step, 1), fmod(z / m_Config.Step, 1));
}

float PerlinGenerator::interpolation_cos3D(float a1, float b1, float c1, float d1, float a2, float b2, float c2, float d2, float x, float y, float z) {
   float y1 = interpolation_cos2D(a1, b1, c1, d1, x, y);
   float y2 = interpolation_cos2D(a2, b2, c2, d2, x, y);

   return  interpolation_cos1D(y1, y2, z);
}


float PerlinGenerator::interpolation_cos2D(float a, float b, float c, float d, float x, float y) {
   float y1 = interpolation_cos1D(a, b, x);
   float y2 = interpolation_cos1D(c, d, x);
   return  interpolation_cos1D(y1, y2, y);
}

float PerlinGenerator::interpolation_cos1D(float a, float b, float x) {
   float k = (1 - cos(x * M_PI)) / 2;
    return a * (1 - k) + b * k;
}
