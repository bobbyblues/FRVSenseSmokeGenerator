#ifndef IMPORTERPBRT_H
#define IMPORTERPBRT_H

#include <string>
#include "PerlinGenerator.h"

namespace Importers
{
        namespace PBRT
        {
                void Importer(const std::string& path, Perlin3DObject& obj);
        }
}

#endif // IMPORTERPBRT_H
