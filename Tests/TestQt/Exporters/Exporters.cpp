#include "Exporters.h"

#include <iostream>

// Includes des differents exporters
#include "ExporterPBRT.h"

namespace Exporters
{
	void Exporter(const std::string& fullpath, ExportersAvalaibleType typeExp)
	{
		switch(typeExp)
		{
		case PBRT_EXPORTER:
			std::cout << "Export de " << fullpath << " en PBRT\n";
			break;
		case RAW_EXPORTER:
			std::cout << "Export de " << fullpath << " en RAW\n";
			break;
		default:
			// TODO: Lever une exception
			break;
		}
	}
}