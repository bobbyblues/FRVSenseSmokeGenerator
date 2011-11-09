#include "ImporterVSQ.h"
#include "VSQReader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <QDialog>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QPushButton>

ImporterVSQ::~ImporterVSQ(){

}


Perlin3DObject * ImporterVSQ::Import(const std::string& path)
{

    VSQReader reader(path);

    // Dialog box creation
    QDialog frameNbDialog;
    QVBoxLayout * layout = new QVBoxLayout;
    QSpinBox * sbNumber = new QSpinBox(&frameNbDialog);
    QPushButton * bOk = new QPushButton("Ok", &frameNbDialog);
    sbNumber->setMaximum(reader.getNbFrame());
    layout->addWidget(sbNumber);
    layout->addWidget(bOk);
    frameNbDialog.setLayout(layout);
    QObject::connect(bOk, SIGNAL(clicked()), &frameNbDialog, SLOT(accept()));

    // We display the box
    frameNbDialog.exec();

    // We get back the value
    int frameNumber = sbNumber->value();

    int cubeSize = reader.getSize();
    float * density = reader.readFrame(frameNumber);

    glm::ivec3 Size = glm::ivec3();
    Size.x = cubeSize;
    Size.y = cubeSize;
    Size.z = cubeSize;
    Perlin3DObject * obj = new Perlin3DObject(Size);

    int cubeSizeSq = cubeSize * cubeSize;

    for (int i = 0; i < cubeSize; ++i)
        for (int j = 0; j < cubeSize; ++j)
            for (int k = 0; k < cubeSize; ++k){
                obj->SetData(density[i * cubeSizeSq + j * cubeSize + k],j,i,k);
            }

    delete[] density;

    return obj;
}

