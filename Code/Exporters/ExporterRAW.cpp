#include "ExporterRAW.h"
#include <stdio.h>
#include <iostream>



void ExporterRAW::Export(const std::string& path, const Perlin3DObject& obj)
{
    FILE *fp = fopen(path.c_str(), "wb");
    if (!fp) {
        std::cout << "Error opening file '" << path << "'\n";
        return;
    }

    unsigned char * dataConv = new unsigned char[obj.Size.x*obj.Size.y*obj.Size.z];
#pragma omp parallel for
    for (int x = 0; x < obj.Size.x; ++x){
        for (int y = 0; y < obj.Size.y; ++y){
            for (int z = 0; z < obj.Size.z; ++z){
                dataConv[z * obj.Size.x * obj.Size.y + y * obj.Size.x + x] = obj.GetData(x,y,z)*255;
            }
        }
    }

    fwrite(dataConv, sizeof(unsigned char), obj.Size.x*obj.Size.y*obj.Size.z, fp);

    delete[] dataConv;
    fclose(fp);
}
