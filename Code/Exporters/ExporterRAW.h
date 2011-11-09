#ifndef EXPORTRAW_H
#define EXPORTRAW_H

#include "Exporter.h"

class ExporterRAW : public Exporter{

public:
    ExporterRAW():Exporter("RAW","*.raw"){}
    virtual ~ExporterRAW(){}

    virtual void Export(const std::string& fullPath, const Perlin3DObject& obj);

};

#endif
