#ifndef EXPORTERS_H
#define EXPORTERS_H

#include <string>
#include "PerlinGenerator.h"

// Liste de tous les exporters
enum ExportersAvalaibleType
{
	PBRT_EXPORTER = 0,
	RAW_EXPORTER = 1
};

namespace Exporters
{
	void Exporter(const std::string& fullpath, ExportersAvalaibleType typeExp, const Perlin3DObject& obj);
}

#endif