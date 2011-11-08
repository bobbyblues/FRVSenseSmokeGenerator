#ifndef IMPORTERVSQ_H
#define IMPORTERVSQ_H

#include "Importer.h"

class ImporterVSQ : public Importer{

public:
    ImporterVSQ():Importer("VSQ","*.vsq"){}
    virtual ~ImporterVSQ();

    virtual Perlin3DObject * Import(const std::string& fullPath);

};

#endif // IMPORTERVSQ_H
