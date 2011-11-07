#include "Importers.h"

#include <iostream>

// Includes des differents importers
#include "ImporterPBRT.h"
#include "ImporterVSQ.h"

namespace Importers
{
        Perlin3DObject * Importer(const std::string& fullpath, ImportersAvalaibleType typeImp)
        {
                switch(typeImp)
                {
                case PBRT_IMPORTER:
                        return PBRT::Importer(fullpath);
                        break;
                case VSQ_IMPORTER:
                        return VSQ::Importer(fullpath);
                        break;
                default:
                        // TODO: Lever une exception
                        break;
                }
				return NULL;
        }
}
