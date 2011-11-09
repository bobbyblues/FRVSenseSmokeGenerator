#include "Exporters.h"
#include <QFileDialog>
#include <iostream>

Exporters::Exporters(PerlinGenerator * generator):
    m_generator(generator){
    this->setTitle(tr("&Export"));

    initialize();

    m_mapper = new QSignalMapper(this);
    QObject::connect(m_mapper, SIGNAL(mapped(int)), this, SLOT(Export(int)));

    for (int i = 0; i < m_exporters.size(); ++i){
        QAction * temp = this->addAction(m_exporters[i]->getName().c_str());
        m_mapper->setMapping(temp,i);
        QObject::connect(temp, SIGNAL(triggered()), m_mapper, SLOT(map()));
    }

}

Exporters::~Exporters(){
    for(int i = 0; i < m_exporters.size(); ++i)
        delete m_exporters[i];
}

void Exporters::addExporter(Exporter * exp){
    m_exporters.push_back(exp);
}

void Exporters::Export(int i){
    QString file = QFileDialog::getSaveFileName(this,tr("Export to"), ".",m_exporters[i]->getValidFileExtensions().c_str());
    if(!file.isEmpty()){
        m_exporters[i]->Export(file.toStdString(), *m_generator->GetCurrentResult());
    }

}
