#ifndef IMPORTERVIRTUAL_H
#define IMPORTERVIRTUAL_H

#include <string>
#include "PerlinGenerator.h"

class Importer{
private:
    std::string m_name;
    std::string m_ext;
public:
    Importer(const std::string& name, const std::string& ext):m_name(name),m_ext(ext){}
    virtual ~Importer(){}

    virtual const std::string & getName() const{return m_name;}

    virtual const std::string & getValidFileExtensions() const{return m_ext;}

    virtual Perlin3DObject * Import(const std::string& fullPath) = 0;
};

#endif // IMPORTERVIRTUAL_H
