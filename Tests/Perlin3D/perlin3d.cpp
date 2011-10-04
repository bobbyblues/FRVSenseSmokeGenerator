#include "perlin3d.h"

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
#include <ctime>

Perlin3D::Perlin3D(int sizeX, int sizeY, int sizeZ, float step, short nbOctave, float persistance):
    m_sizeX(sizeX),
    m_sizeY(sizeY),
    m_sizeZ(sizeZ),
    m_step(step),
    m_nbOctave(nbOctave),
    m_persistance(persistance)
{
    init_noise();
    init_perlin();
}

Perlin3D::~Perlin3D(){

}

void Perlin3D::init_noise(){

    m_sizeXMax = (int) ceil(m_sizeX * pow(2.0f, m_nbOctave - 1) / m_step);
    m_sizeYMax = (int) ceil(m_sizeY * pow(2.0f, m_nbOctave - 1) / m_step);
    m_sizeZMax = (int) ceil(m_sizeZ * pow(2.0f, m_nbOctave - 1) / m_step);

    m_noise.clear();
    srand((unsigned int)time(NULL));

    for (int i = 0; i < m_nbOctave; ++i){
        for (int j = 0; j < m_sizeXMax * m_sizeYMax * m_sizeZMax; ++j){
            m_noise.push_back((double)rand()/(double)RAND_MAX);
        }
    }
}

double Perlin3D::noise(int i, int j, int k){
    return m_noise[k * m_sizeXMax * m_sizeYMax + j * m_sizeXMax + i];
}

double Perlin3D::getPerlin(int x, int y, int z){
    double somme = 0;
    double p = 1;
    int f = 1;
    int i;

    for(i = 0 ; i < m_nbOctave ; ++i) {
        somme += p * fonction_bruit3D(x * f, y * f, z * f);
        p *= m_persistance;
        f *= 2;
    }
    return somme * (1 - m_persistance) / (1 - p);
}


double Perlin3D::fonction_bruit3D(int x, int y, int z) {
   int i = (int) (x / m_step);
   int j = (int) (y / m_step);
   int k = (int) (z / m_step);
   return interpolation_cos3D(noise(i, j, k), noise(i + 1, j, k), noise(i, j + 1, k), noise(i + 1, j + 1, k), noise(i, j, k + 1), noise(i + 1, j, k + 1), noise(i, j + 1, k + 1), noise(i + 1, j + 1, k + 1),fmod(x / m_step, 1), fmod(y / m_step, 1), fmod(z / m_step, 1));
}

double Perlin3D::interpolation_cos3D(double a1, double b1, double c1, double d1, double a2, double b2, double c2, double d2, double x, double y, double z) {
   double y1 = interpolation_cos2D(a1, b1, c1, d1, x, y);
   double y2 = interpolation_cos2D(a2, b2, c2, d2, x, y);

   return  interpolation_cos1D(y1, y2, z);
}


double Perlin3D::interpolation_cos2D(double a, double b, double c, double d, double x, double y) {
   double y1 = interpolation_cos1D(a, b, x);
   double y2 = interpolation_cos1D(c, d, x);
   return  interpolation_cos1D(y1, y2, y);
}

double Perlin3D::interpolation_cos1D(double a, double b, double x) {
   double k = (1 - cos(x * M_PI)) / 2;
    return a * (1 - k) + b * k;
}


void Perlin3D::init_perlin(){
    for (int k = 0; k < m_sizeZ; ++k)
        for (int i = 0; i < m_sizeX; ++i)
            for (int j = 0; j < m_sizeY; ++j)
                m_perlin.push_back(getPerlin(i, j, k));
}

void Perlin3D::writePGMImage(std::string filename, int tranche){
    std::ofstream img(filename.c_str());
    img << "P2 \n " << m_sizeX << " " << m_sizeY << " 255 \n";
    int init = tranche * m_sizeX * m_sizeY;
    int end = init + m_sizeX * m_sizeY;
    for (int i = init; i < end; i+=1){
        img << (int)(m_perlin[i] * 255)<< " ";
    }

        img.close();
}

void Perlin3D::writePBRTFile(std::string filename){
    std::ofstream pbrt(filename.c_str());
    pbrt << "Volume \"volumegrid\" \"integer nx\" " << m_sizeX << " \"integer ny\" " << m_sizeY << " \" integer nz\" " << m_sizeZ << std::endl;
    pbrt << "\"point p0\" [ 0 0 0 ] \" point p1\" [ 1 1 1 ]" << std::endl;
    pbrt << "\"float density\" [" << std::endl;

    for (int k = 0; k < m_sizeZ; ++k){
        int init = k * m_sizeX * m_sizeY;
        int end = init + m_sizeX * m_sizeY;
        for (int i = init; i < end; i+=1){
            pbrt << m_perlin[i]<< " ";
            if (i % m_sizeX == 0)
                pbrt << std::endl;
        }
    }
    pbrt << "]"<< std::endl;
    pbrt.close();
}





