#ifndef EXPORTERS_H
#define EXPORTERS_H

#include "PerlinGenerator.h"

#include <string>
#include <vector>
#include <sstream>

#include <QMenu>
#include <QSignalMapper>

// Exporters
#include "ExporterPBRT.h"
#include "ExporterRAW.h"

class Exporters : public QMenu{
Q_OBJECT
private:
    std::vector<Exporter *> m_exporters;
    QSignalMapper* m_mapper;
    PerlinGenerator * m_generator;

    // private function
    void addExporter(Exporter *);

public:
    Exporters(PerlinGenerator *);
    ~Exporters();

    // When creating a new exporter, add an instance of the new exporter in this function, using addExporter
    void initialize(){
        addExporter(new ExporterPBRT());
        addExporter(new ExporterRAW());
    }

public slots:
    void Export(int i); // Slot importing a file when clicking on an element of the menu


};



#endif
