#ifndef IMPORTERPBRT_H
#define IMPORTERPBRT_H

#include "Importer.h"

class ImporterPBRT : public Importer{

public:
    ImporterPBRT():Importer("PBRT","*.pbrt"){}
    virtual ~ImporterPBRT();

    virtual Perlin3DObject * Import(const std::string& fullPath);

};

#endif // IMPORTERPBRT_H
