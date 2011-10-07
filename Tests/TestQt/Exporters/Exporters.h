#ifndef EXPORTERS_H
#define EXPORTERS_H

#include <string>

// Liste de tous les exporters
enum ExportersAvalaibleType
{
	PBRT_EXPORTER = 0,
	RAW_EXPORTER = 1
};

namespace Exporters
{
	void Exporter(const std::string& fullpath, ExportersAvalaibleType typeExp);
}

#endif