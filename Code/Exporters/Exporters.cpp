#include "Exporters.h"

#include <iostream>

// Includes des differents exporters
#include "ExporterPBRT.h"
#include "ExporterRAW.h"

namespace Exporters
{
	void Exporter(const std::string& fullpath, ExportersAvalaibleType typeExp, const Perlin3DObject& obj)
	{
		switch(typeExp)
		{
		case PBRT_EXPORTER:
			PBRT::Exporter(fullpath, obj);
			break;
		case RAW_EXPORTER:
			RAW::Exporter(fullpath, obj);
			break;
		default:
			// TODO: Lever une exception
			break;
		}
	}
}