#ifndef IMPORTERVSQ_H
#define IMPORTERVSQ_H

#include <string>
#include "PerlinGenerator.h"

namespace Importers
{
        namespace VSQ
        {
			Perlin3DObject * Importer(const std::string& path);
        }
}
#endif // IMPORTERVSQ_H
