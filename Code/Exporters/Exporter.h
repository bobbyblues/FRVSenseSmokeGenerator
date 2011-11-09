#ifndef EXPORTER_H
#define EXPORTER_H

#include <string>
#include "PerlinGenerator.h"

class Exporter{
private:
    std::string m_name;
    std::string m_ext;
public:
    Exporter(const std::string& name, const std::string& ext):m_name(name),m_ext(ext){}
    virtual ~Exporter(){}

    virtual const std::string & getName() const{return m_name;}

    virtual const std::string & getValidFileExtensions() const{return m_ext;}

    virtual void Export(const std::string& fullPath, const Perlin3DObject& obj) = 0;
};

#endif // EXPORTER_H
