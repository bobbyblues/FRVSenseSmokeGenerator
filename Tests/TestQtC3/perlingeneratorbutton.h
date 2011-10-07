#ifndef PERLINGENERATORBUTTON_H
#define PERLINGENERATORBUTTON_H

#include <QWidget>
#include <QThread>

#include <vector>
#include <string>

class PerlinGenerator : public QThread
{
    Q_OBJECT

private:
    int m_sizeX, m_sizeY, m_sizeZ;
    int m_sizeXMax, m_sizeYMax, m_sizeZMax;
    float m_step;
    int m_nbOctave;
    float m_persistance;
    std::vector<double> m_noise;
    std::vector<double> m_perlin;


    double fonction_bruit3D(int x, int y, int z);
    double interpolation_cos3D(double a1, double b1, double c1, double d1, double a2, double b2, double c2, double d2, double x, double y, double z);
    double interpolation_cos2D(double a, double b, double c, double d, double x, double y);
    double interpolation_cos1D(double a, double b, double x);



public:
    PerlinGenerator(int sizeX = 512, int sizeY = 512, float step = 128, int nbOctave = 8, float persistance = 0.5);

    int sizeX(){return m_sizeX;}
    int sizeY(){return m_sizeY;}
    int sizeZ(){return m_sizeZ;}

    float step(){return m_step;}
    int nbOctave(){return m_nbOctave;}
    float persistance(){return m_persistance;}

    void setSize(int size){m_sizeX = m_sizeY = size;}
    void setSizeX(int sizeX){m_sizeX = sizeX;}
    void setSizeY(int sizeY){m_sizeY = sizeY;}
    void setSizeZ(int sizeZ){m_sizeZ = sizeZ;}
    void setStep(float step){m_step = step;}
    void setNbOctave(int nbOctave){m_nbOctave = nbOctave;}
    void setPersistance(float persistance){m_persistance = persistance;}

    double noise(int i, int j, int k);
    double getPerlin(int x, int y, int z);
    void writePGMImage(std::string filename, int tranche = 0);
    void writePBRTFile(std::string filename);

    virtual void run();

signals:
    void progressStatus(int progress);


};

#endif // PERLINGENERATORBUTTON_H
