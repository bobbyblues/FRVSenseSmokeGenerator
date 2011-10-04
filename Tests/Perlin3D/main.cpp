#include "perlin3d.h"

#include <iostream>
#include <fstream>
#include <sstream>

int main(){
    Perlin3D p(100, 100, 100, 128, 8, 0.5);
    std::cout << "ecriture" << std::endl;
    for (int i =  0; i < 10; ++i){
        std::string filename;
        std::stringstream stream;
        stream << "test" << i << ".pgm";
        stream >> filename;
        stream.clear();

        p.writePGMImage(filename.c_str(), i);
    }

    p.writePBRTFile("smoke.pbrt");


return 0;
}
