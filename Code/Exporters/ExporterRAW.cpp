#include "ExporterRAW.h"
#include <stdio.h>
#include <iostream>
namespace Exporters
{
	namespace RAW
	{
		void Exporter(const std::string& path, const Perlin3DObject& obj)
		{
			FILE *fp = fopen(path.c_str(), "wb");
			if (!fp) {
				std::cout << "Error opening file '" << path << "'\n";
				return;
			}
			fwrite(obj.Data, sizeof(float), obj.Size.x*obj.Size.y*obj.Size.z, fp);
			fclose(fp);
		}
	}
}

