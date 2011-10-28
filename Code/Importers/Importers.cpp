#include "Importers.h"

#include <iostream>

// Includes des differents importers
#include "ImporterPBRT.h"

namespace Importers
{
        Perlin3DObject * Importer(const std::string& fullpath, ImportersAvalaibleType typeImp)
        {
                switch(typeImp)
                {
                case PBRT_IMPORTER:
                        PBRT::Importer(fullpath);
                        break;
                default:
                        // TODO: Lever une exception
                        break;
                }
        }
}
