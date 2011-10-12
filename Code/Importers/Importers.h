#ifndef IMPORTERS_H
#define IMPORTERS_H

#include <string>
#include "PerlinGenerator.h"

// Liste de tous les exporters
enum ImportersAvalaibleType
{
        PBRT_IMPORTER = 0
};

namespace Importers
{
        void Importer(const std::string& fullpath, ImportersAvalaibleType typeImp, Perlin3DObject& obj);
}



#endif // IMPORTERS_H
