#ifndef IMPORTERS_H
#define IMPORTERS_H

#include <string>
#include "PerlinGenerator.h"

// Liste de tous les exporters
enum ImportersAvalaibleType
{
        PBRT_IMPORTER = 0,
        VSQ_IMPORTER = 1
};

namespace Importers
{
         Perlin3DObject * Importer(const std::string& fullpath, ImportersAvalaibleType typeImp);
}



#endif // IMPORTERS_H
