#include "Importers.h"

#include <iostream>

// Includes des differents importers
#include "ImporterPBRT.h"

namespace Importers
{
        void Importer(const std::string& fullpath, ImportersAvalaibleType typeImp, Perlin3DObject& obj)
        {
                switch(typeImp)
                {
                case PBRT_IMPORTER:
                        PBRT::Importer(fullpath, obj);
                        break;
                default:
                        // TODO: Lever une exception
                        break;
                }
        }
}
