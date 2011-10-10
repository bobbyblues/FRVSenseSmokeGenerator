#ifndef EXPORTRAW_H
#define EXPORTRAW_H

#include <string>
#include "PerlinGenerator.h"

namespace Exporters
{
	namespace RAW
	{
		void Exporter(const std::string& path, const Perlin3DObject& obj);
	}
}

#endif