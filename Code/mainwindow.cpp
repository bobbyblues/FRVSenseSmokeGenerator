#include "mainwindow.h"

#include "Importers/Importers.h"
#include "Exporters/Exporters.h"

#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <iostream>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_Perlin3DObject(NULL)
{
    ui.setupUi(this);
    
	// Create menu
	// *** Main section
        QMenu* menuFichier = menuBar()->addMenu(tr("&File"));
	// *** Subsection
        //  -- File
        //   \--Import
        m_importers = new Importers();
        menuFichier->addMenu(m_importers);
        QObject::connect(m_importers, SIGNAL(newObject()), this, SLOT(importer()));
        //   \--Export
        menuFichier->addMenu(new Exporters(&m_PerlinGenerator));

	// *** Other objects
	QObject::connect(ui.bGeneratePerlin, SIGNAL(clicked()), this, SLOT(launchGeneration()));
        QObject::connect(&m_PerlinGenerator, SIGNAL(progressStatus(int)), ui.pbGeneration, SLOT(setValue(int)));
        QObject::connect(ui.hsSlideSelector, SIGNAL(valueChanged(int)), this, SLOT(updateDisplay(int)));
        QObject::connect(&m_PerlinGenerator, SIGNAL(finished()), this, SLOT(newResultFromGeneration()));
	QObject::connect(ui.dspStep,SIGNAL(valueChanged(double)), this, SLOT(setNewStepValue(double)));
        QObject::connect(ui.hsContraste, SIGNAL(valueChanged(int)), this, SLOT(updateContraste()));
}



void mainWindow::updatePerlinGenerator(){
    if (!m_PerlinGenerator.isRunning()){
                Perlin3DConfig config;
                config.Size = glm::ivec3(ui.sbSizeX->value(),
                                                                 ui.sbSizeY->value(),
                                                                 ui.sbSizeZ->value());

                ui.hsSlideSelector->setMaximum(ui.sbSizeZ->value() - 1);

                config.NbOctaves = ui.sbNbOctave->value();
                config.Step = ui.dspStep->value();
                config.Persistance = ui.dspPersistance->value();

                m_PerlinGenerator.Prepare(config);


    }
}

void mainWindow::launchGeneration(){
    if (!m_PerlinGenerator.isRunning()){
                updatePerlinGenerator();
		m_PerlinGenerator.Compute();

    }
}

void mainWindow::updateDisplay(int layer){
    if (m_Perlin3DObject){

        QImage image = QImage(m_Perlin3DObject->Size.x, m_Perlin3DObject->Size.y, QImage::Format_ARGB32);
        for (int y = 0; y < m_Perlin3DObject->Size.y; y++)
	{
            QRgb* rgb = (QRgb*)image.scanLine(m_Perlin3DObject->Size.y-(y+1)); // Il faut que l'image soit en ARGB32 (je pense, voir la doc)
            for (int x = 0; x < m_Perlin3DObject->Size.x; x++)
            {
                float v = ceil(m_Perlin3DObject->GetData(x,y,layer)*255);
                rgb[x] = qRgba(v, v, v, 255);
            }
	}
	ui.lPreview->setPixmap(QPixmap::fromImage(image, Qt::AutoColor));
        ui.lPreview->setAlignment(Qt::AlignCenter);
    }
}

void mainWindow::updateContraste(){
    m_Perlin3DObject->Scale = ui.hsContraste->value() / 10.f;
    updateDisplay(ui.hsSlideSelector->value());
}

float mainWindow::getContrastedData(float data){
    float contraste = (float) ui.hsContraste->value() / 10.f;
    return std::max(std::min(data * contraste - (contraste / 2.f) + 0.5f,1.f),0.f);
}

void mainWindow::importer()
{

    Perlin3DObject *  temp = m_importers->getImportedObject();
    if(temp){
        if (m_Perlin3DObject) delete m_Perlin3DObject;
        m_Perlin3DObject = temp;

        // Interface Update
        ui.sbSizeX->setValue(m_Perlin3DObject->Size.x);
        ui.sbSizeY->setValue(m_Perlin3DObject->Size.y);
        ui.sbSizeZ->setValue(m_Perlin3DObject->Size.z);
        ui.hsSlideSelector->setMaximum(ui.sbSizeZ->value() - 1);
        ui.hsSlideSelector->setValue(0);
        ui.hsContraste->setValue(1);
    }

    updateDisplay(0);

}

