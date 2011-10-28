#ifndef PERLINGENERATORBUTTON_H
#define PERLINGENERATORBUTTON_H

#include <QWidget>
#include <QThread>

#include <vector>
#include <string>

#include "glm/glm.hpp"

struct Perlin3DConfig
{
	glm::ivec3 Size;
	float Step;
	int NbOctaves;
	float Persistance;
};

class Perlin3DObject
{
public:
	// Attributes
	glm::ivec3 Size;
	float* Data;
    float Scale;

	// Constructors & Destructors
	Perlin3DObject(const glm::ivec3& size):
		Size(size),
		Data(NULL)
	{
		Data = new float[Size.x*Size.y*Size.z];
		for(int i = 0; i<Size.x*Size.y*Size.z; i++)
		{
			Data[i] = 0;
		}

                Scale = 1.f;
	}

	virtual ~Perlin3DObject()
	{
		delete[] Data;
	}

	inline float GetData(int x, int y,int z) const
        {
                float unscaledData = Data[z * Size.x * Size.y + y * Size.x + x];
                return GetScaledData(unscaledData);
	}

private:
        inline float GetScaledData(float data) const{
            return std::max(std::min(data * Scale - (Scale / 2.f) + 0.5f,1.f),0.f);
        }


};

class PerlinGenerator : public QThread
{
    Q_OBJECT

private:
    float * m_noise;
	Perlin3DObject * m_Result;
	Perlin3DConfig m_Config;
	glm::ivec3 m_SizeMax;

	// Methodes pour faire le bruit de Perlin
    float fonction_bruit3D(int x, int y, int z);
    float interpolation_cos3D(float a1, float b1, float c1, float d1, float a2, float b2, float c2, float d2, float x, float y, float z);
    float interpolation_cos2D(float a, float b, float c, float d, float x, float y);
    float interpolation_cos1D(float a, float b, float x);

	// Methodes pour faciliter 
	float getNoise(int x, int y, int z);
    float getPerlin(int x, int y, int z);

public:
    PerlinGenerator();
	virtual ~PerlinGenerator();

	void Prepare(Perlin3DConfig& config);
	void Compute() { start(); }
	Perlin3DObject * GetCurrentResult() { return m_Result; }

        void SetScaleValue(float Scale){ if (m_Result) m_Result->Scale = Scale; }
    
protected:
	virtual void run();
	

signals:
    void progressStatus(int progress);


};

#endif // PERLINGENERATORBUTTON_H
