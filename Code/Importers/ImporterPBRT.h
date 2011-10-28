#ifndef IMPORTERPBRT_H
#define IMPORTERPBRT_H

#include <string>
#include "PerlinGenerator.h"

namespace Importers
{
        namespace PBRT
        {
                Perlin3DObject * Importer(const std::string& path);
        }
}

#endif // IMPORTERPBRT_H
