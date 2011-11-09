#ifndef EXPORTPBRT_H
#define EXPORTPBRT_H

#include "Exporter.h"

class ExporterPBRT : public Exporter{

public:
    ExporterPBRT():Exporter("PBRT","*.pbrt"){}
    virtual ~ExporterPBRT(){}

    virtual void Export(const std::string& fullPath, const Perlin3DObject& obj);

};

#endif
