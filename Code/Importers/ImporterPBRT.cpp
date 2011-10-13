#include "ImporterPBRT.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace Importers
{
namespace PBRT
{


Perlin3DObject * Importer(const std::string& path)
{
    std::cout << "debut import" << std::endl;
    int nx, ny, nz;
    nx = ny = nz = 0;

    std::ifstream stream(path.c_str());
    std::vector<std::string> lines;


    // We read the file
    while (stream){
        //char lineStr[1024];
        //stream.getline(lineStr, 1024);
        //std::string line(lineStr);
        std::string line = "";
        std::getline(stream,line);
//        stream >> line;
        lines.push_back(line);

        //std::cout << line << std::endl;
    }

    // We find the beginning of the volume
    bool found = false;
    int startingLine = 0;
    size_t volumeStart;

    while (!found && startingLine < lines.size()){
        std::string line = lines[startingLine];


        volumeStart = line.find("volumegrid");
        if (volumeStart != line.npos){
            found = true;
            continue;
        }
        ++startingLine;
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

    glm::ivec3 Size = glm::ivec3();
    Size.x = nx;
    Size.y = ny;
    Size.z = nz;
    Perlin3DObject * obj = new Perlin3DObject(Size);

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
    currLine = startingLine;
    line = lines[currLine];
    line = line.substr(valuesStart + 1);
    std::stringstream ss(line);
    for (int z = 0; z < nz; ++z)
        for (int y = 0; y < ny; ++y)
            for (int x = 0; x < nx; ++x){
                if (ss.eof()){
                    ++currLine;
                    line = lines[currLine];
                    ss.clear();
                    ss << line;
                }
                float temp;
                ss >> temp;
                while (ss.fail() || ss.bad()){
                    ++currLine;
                    line = lines[currLine];
                    ss.clear();
                    ss << line;
                    ss >> temp;
                }

                obj->SetData(temp, x, y, z);
    }
    std::cout << "blublu" << std::endl;
            return obj;
}

}
}
