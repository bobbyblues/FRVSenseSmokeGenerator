#include "ExporterPBRT.h"

#include <fstream>
#include <iostream>

namespace Exporters
{
namespace PBRT
{
	

void Exporter(const std::string& path, const Perlin3DObject& obj)
{
	std::ofstream pbrt(path.c_str());
    pbrt << "Volume \"volumegrid\" \"integer nx\" " << obj.Size.x
		 << " \"integer ny\" " << obj.Size.y 
		 << " \" integer nz\" " << obj.Size.z << std::endl;
    pbrt << "\"point p0\" [ 0 0 0 ] \" point p1\" [ 1 1 1 ]" << std::endl;
    pbrt << "\"float density\" [" << std::endl;

    for (int x = 0; x < obj.Size.x; ++x){
        for (int y = 0; y < obj.Size.y; ++y){
            for (int z = 0; z < obj.Size.z; ++z){
                float value = obj.GetData(x,y,z);
                value *= 2.f;
                value -= 1.f;
                if (value > 1.f)
                    value = 1.f;
                if (value < 0.f)
                    value = 0.f;
                pbrt << value << " " ;
                //pbrt << obj.GetData(x,y,z) << " ";
            }
            pbrt << std::endl;
        }
    }
    pbrt << "\n]"<< std::endl;
    pbrt.close();
    std::cout << "done" << std::endl;
}

}
}
