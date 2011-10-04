#ifndef PERLIN2D_H
#define PERLIN2D_H


#include <vector>
#include <string>

class Perlin3D
{

private:
    //Tab for the noise
    int m_sizeX, m_sizeY, m_sizeZ;
    int m_sizeXMax, m_sizeYMax, m_sizeZMax;
    float m_step;
    short m_nbOctave;
    float m_persistance;
    std::vector<double> m_noise;
    std::vector<double> m_perlin;

    void init_noise();
    double fonction_bruit3D(int x, int y, int z);
    double interpolation_cos3D(double a1, double b1, double c1, double d1, double a2, double b2, double c2, double d2, double x, double y, double z);
    double interpolation_cos2D(double a, double b, double c, double d, double x, double y);
    double interpolation_cos1D(double a, double b, double x);
    void init_perlin();


public:
    Perlin3D(int sizeX, int sizeY, int sizeZ, float step, short nbOctave, float m_persistance);
    ~Perlin3D();

    double noise(int i, int j, int k);
    double getPerlin(int x, int y, int z);
    void writePGMImage(std::string filename, int tranche = 0);
    void writePBRTFile(std::string filename);


};

#endif // PERLIN2D_H
