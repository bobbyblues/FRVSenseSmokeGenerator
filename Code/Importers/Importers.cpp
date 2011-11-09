#include "Importers.h"
#include <QFileDialog>

Importers::Importers(){
    this->setTitle(tr("&Import"));

    initialize();

    m_mapper = new QSignalMapper(this);
    QObject::connect(m_mapper, SIGNAL(mapped(int)), this, SLOT(Import(int)));

    for (int i = 0; i < m_importers.size(); ++i){
        QAction * temp = this->addAction(m_importers[i]->getName().c_str());
        m_mapper->setMapping(temp,i);
        QObject::connect(temp, SIGNAL(triggered()), m_mapper, SLOT(map()));
    }

}

Importers::~Importers(){
    for(int i = 0; i < m_importers.size(); ++i)
        delete m_importers[i];
}

void Importers::addImporter(Importer * imp){
    m_importers.push_back(imp);
}

void Importers::Import(int i){
    QString file = QFileDialog::getOpenFileName(this,tr("Importer from"), ".",m_importers[i]->getValidFileExtensions().c_str());
    if(!file.isEmpty()){
        m_object = m_importers[i]->Import(file.toStdString());
    }
    else
        m_object = NULL;

    emit newObject();
}











