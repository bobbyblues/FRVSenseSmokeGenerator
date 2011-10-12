#include "ImporterPBRT.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace Importers
{
namespace PBRT
{


void Importer(const std::string& path, Perlin3DObject& obj)
{

    int nx, ny, nz;
    nx = ny = nz = 0;

    std::ifstream stream(path.c_str());
    std::vector<std::string> lines;


    // We read the file
    while (stream){
        char lineStr[1024];
        stream.getline(lineStr, 1024);
        std::string line(lineStr);
        lines.push_back(line);

        //std::cout << line << std::endl;
    }

    // We find the beginning of the volume
    bool found = false;
    int startingLine = 0;
    size_t volumeStart;

    while (!found && startingLine < lines.size()){
        std::string line = lines[startingLine];
        ++startingLine;

        volumeStart = line.find("volumegrid");
        if (volumeStart != line.npos){
            found = true;
        }
    }

    // We make the assumption that ni and the ni value are on the same line

    // We find nx
    int currLine = startingLine;
    std::string line = lines[currLine];
    line = line.substr(volumeStart);
    found = false;

    while(!found && currLine < lines.size()){
        size_t location = line.find("nx");
        if (location != line.npos){
            // We found nx
            line = line.substr(location);
            location = line.find("\"");
            // We found "
            line = line.substr(location);
            location = line.find(" ");
            line = line.substr(location + 1);
            location = line.find(" ");
            line = line.substr(0, location);
            nx = atoi(line.c_str());
            found = true;
        }

        ++currLine;
        line = lines[currLine];

    }

    // We find ny
    currLine = startingLine;
    line = lines[currLine];
    line = line.substr(volumeStart);
    found = false;

    while(!found && currLine < lines.size()){
        size_t location = line.find("ny");
        if (location != line.npos){
            // We found ny
            line = line.substr(location);
            location = line.find("\"");
            // We found "
            line = line.substr(location);
            location = line.find(" ");
            line = line.substr(location + 1);
            location = line.find(" ");
            line = line.substr(0, location);
            ny = atoi(line.c_str());
            found = true;
        }

        ++currLine;
        line = lines[currLine];

    }

    // We find nz
    currLine = startingLine;
    line = lines[currLine];
    line = line.substr(volumeStart);
    found = false;

    while(!found && currLine < lines.size()){
        size_t location = line.find("nz");
        if (location != line.npos){
            // We found nz
            line = line.substr(location);
            location = line.find("\"");
            // We found "
            line = line.substr(location);
            location = line.find(" ");
            line = line.substr(location + 1);
            location = line.find(" ");
            line = line.substr(0, location);
            nz = atoi(line.c_str());
            found = true;
        }

        ++currLine;
        line = lines[currLine];

    }


    // For debugging only
    std::cout << "Size : [" << nx << " ; " << ny << " ; " << nz << "]" << std::endl;

    currLine = startingLine;
    line = lines[currLine];
    line = line.substr(volumeStart);
    found = false;
    size_t valuesStart;

    while (!found && currLine < lines.size()){
        size_t location = line.find("density");
        if (location != line.npos){
            line = line.substr(location);
            while (!found && currLine < lines.size()){
                location = line.find("[");
                if (location != line.npos){
                    found = true;
                    valuesStart = location;
                    startingLine = currLine;
                }

                ++currLine;
                line = lines[currLine];

            }
        }
        ++currLine;
        line = lines[currLine];
    }


    // We read the values making the assumption that there is no 2 spaces between 2 values
    float * Data = new float[nx * ny * nz];
    currLine = startingLine;
    line = lines[currLine];
    line = line.substr(valuesStart + 1);
    int i = 0;
    int max = nx * ny * nz;
    std::stringstream ss(line);
    while (i < max){
        if (ss.eof()){
            ++currLine;
            line = lines[currLine];
            ss.clear();
            ss << line;
        }
    float temp;
            std::cout << "plip" << std::endl;
            ss >> temp;
            std::cout << temp << std::endl;
            Data[i] = temp;
            ++i;



    }


    if (obj.Data)
        delete[] Data;
    obj.Data = Data;
    obj.Size = glm::ivec3();
    obj.Size.x = nx;
    obj.Size.y = ny;
    obj.Size.z = nz;

    // For debugging only
    std::cout << "Values : " << std::endl;
    for (int i = 0; i < nx * ny * nz ; ++i){
       // std::cout << Data[i] << " ";
    }

}

}
}
