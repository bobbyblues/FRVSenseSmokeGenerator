#ifndef EXPORTPBRT_H
#define EXPORTPBRT_H

#include <string>
#include "PerlinGenerator.h"

namespace Exporters
{
	namespace PBRT
	{
		void Exporter(const std::string& path, const Perlin3DObject& obj);
	}
}

#endif