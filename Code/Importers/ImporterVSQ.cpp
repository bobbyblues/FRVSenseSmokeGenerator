#include "ImporterVSQ.h"
#include "VSQReader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <QDialog>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QPushButton>

namespace Importers
{
namespace VSQ
{


Perlin3DObject * Importer(const std::string& path)
{

    VSQReader reader(path);
    std::cout << "import vsq done" << std::endl;

	// Creation de la boite de dialogue
	QDialog frameNbDialog;
	QVBoxLayout * layout = new QVBoxLayout;
	QSpinBox * sbNumber = new QSpinBox(&frameNbDialog);
	QPushButton * bOk = new QPushButton("Ok", &frameNbDialog);
	sbNumber->setMaximum(reader.getNbFrame());
	layout->addWidget(sbNumber);
	layout->addWidget(bOk);
	frameNbDialog.setLayout(layout);
	QObject::connect(bOk, SIGNAL(clicked()), &frameNbDialog, SLOT(accept()));

	// Affichage de celle-ci
	frameNbDialog.exec();

	// Recuperation de la valeur.
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
				//std::cout << density[i * cubeSizeSq + j * cubeSize + k] << std::endl;
                obj->SetData(density[i * cubeSizeSq + j * cubeSize + k],i,j,k);
    }

    delete[] density;

    return obj;
}

}
}
