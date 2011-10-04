#include "perlin2d.h"

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
#include <ctime>

Perlin2D::Perlin2D(int sizeX, int sizeY, float step, short nbOctave, float persistance):
    m_sizeX(sizeX),
    m_sizeY(sizeY),
    m_step(step),
    m_nbOctave(nbOctave),
    m_persistance(persistance)
{
    init_noise();
    init_perlin();
}

Perlin2D::~Perlin2D(){

}

void Perlin2D::init_noise(){

    m_sizeXMax = (int) ceil((m_sizeX+1) * pow(2.0f, m_nbOctave  - 1)  / m_step);
    m_sizeYMax = (int) ceil((m_sizeY+1) * pow(2.0f, m_nbOctave  - 1)  / m_step);

    m_noise.clear();
    srand((unsigned int)time(NULL));

    for (int i = 0; i < m_nbOctave; ++i){
        for (int j = 0; j < m_sizeXMax * m_sizeYMax; ++j){
            m_noise.push_back((double)rand()/(double)RAND_MAX);
        }
    }
}

double Perlin2D::noise(int i, int j){
    return m_noise[i * m_sizeXMax + j];
}

double Perlin2D::getPerlin(int x, int y){
    double somme = 0;
    double p = 1;
    int f = 1;
    int i;

    for(i = 0 ; i < m_nbOctave ; ++i) {
        somme += p * fonction_bruit2D(x * f, y * f);
        p *= m_persistance;
        f *= 2;
    }
    return somme * (1 - m_persistance) / (1 - p);
}


double Perlin2D::fonction_bruit2D(int x, int y) {
   int i = (int) (x / m_step);
   int j = (int) (y / m_step);
   return interpolation_cos2D(noise(i, j), noise(i + 1, j), noise(i, j + 1), noise(i + 1, j + 1), fmod(x / m_step, 1), fmod(y / m_step, 1));
}

double Perlin2D::interpolation_cos2D(double a, double b, double c, double d, double x, double y) {
   double y1 = interpolation_cos1D(a, b, x);
   double y2 = interpolation_cos1D(c, d, x);
   return  interpolation_cos1D(y1, y2, y);
}

double Perlin2D::interpolation_cos1D(double a, double b, double x) {
   double k = (1 - cos(x * M_PI)) / 2;
    return a * (1 - k) + b * k;
}


void Perlin2D::init_perlin(){
    for (int i = 0; i < m_sizeX; ++i)
        for (int j = 0; j < m_sizeY; ++j)
            m_perlin.push_back(getPerlin(i, j));
}

void Perlin2D::writePGMImage(std::string filename){
    std::ofstream img(filename.c_str());
    img << "P2 \n " << m_sizeX << " " << m_sizeY << " 255 \n";
        for (int i = 0; i < m_sizeY * m_sizeX; ++i){
            img << (int) (m_perlin[i] * 255)<< " ";
       }

        img.close();
}









