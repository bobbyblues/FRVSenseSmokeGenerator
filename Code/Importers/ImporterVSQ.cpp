#include "ImporterVSQ.h"
#include "VSQReader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace Importers
{
namespace VSQ
{


Perlin3DObject * Importer(const std::string& path)
{

    VSQReader reader(path);
    std::cout << "import vsq done" << std::endl;

    // We need to ask the user which frame he wants
    // For now we assume it is the first one
    int frameNumber = 5;

    int cubeSize = reader.getSize();
    float * density = reader.readFrame(frameNumber);

    glm::ivec3 Size = glm::ivec3();
    Size.x = cubeSize;
    Size.y = cubeSize;
    Size.z = cubeSize;
    Perlin3DObject * obj = new Perlin3DObject(Size);

    int cubeSizeSq = cubeSize * cubeSize;

    for (int i = 0; i << cubeSize; ++i)
        for (int j = 0; j << cubeSize; ++j)
            for (int k = 0; k << cubeSize; ++k){
                obj->SetData(density[i * cubeSizeSq + j * cubeSize + k],i,j,k);
    }

    delete[] density;

    return obj;



}

}
}
