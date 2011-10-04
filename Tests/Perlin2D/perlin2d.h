#ifndef PERLIN2D_H
#define PERLIN2D_H


#include <vector>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>

class Perlin2D
{

private:
    //Tab for the noise
    int m_sizeX, m_sizeY;
    int m_sizeXMax, m_sizeYMax;
    float m_step;
    short m_nbOctave;
    float m_persistance;
    std::vector<double> m_noise;
    std::vector<double> m_perlin;

    void init_noise();
    double fonction_bruit2D(int x, int y);
    double interpolation_cos2D(double a, double b, double c, double d, double x, double y);
    double interpolation_cos1D(double a, double b, double x);
    void init_perlin();


public:
    Perlin2D(int sizeX, int sizeY, float step, short nbOctave, float m_persistance);
    ~Perlin2D();

    double noise(int i, int j);
    double getPerlin(int x, int y);
    void writePGMImage(std::string filename);



};

#endif // PERLIN2D_H
