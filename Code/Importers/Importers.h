#ifndef IMPORTERS_H
#define IMPORTERS_H

#include "PerlinGenerator.h"

#include <string>
#include <vector>
#include <sstream>

#include <QMenu>
#include <QSignalMapper>

//Importers
#include "ImporterPBRT.h"
#include "ImporterVSQ.h"


class Importers : public QMenu{
Q_OBJECT
private:
    std::vector<Importer *> m_importers;
    QSignalMapper* m_mapper;
    Perlin3DObject * m_object;

    // private function
    void addImporter(Importer *);

public:
    Importers();
    ~Importers();

    // When creating a new importer, add an instance of the new importer in this function, using addImporter
    void initialize(){
        addImporter(new ImporterPBRT());
        addImporter(new ImporterVSQ());
    }

    Perlin3DObject* getImportedObject(){return m_object;}

signals:
    void newObject(); // Signal emmited once a new file has been imported

public slots:
    void Import(int i); // Slot importing a file when clicking on an element of the menu


};



#endif // IMPORTERS_H
