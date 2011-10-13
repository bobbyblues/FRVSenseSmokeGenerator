#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <iostream>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    
	// Create menu
	// *** Main section
	QMenu* menuFichier = menuBar()->addMenu("&Fichier");
	// *** Subsection
	//  -- Fichier
	QMenu *menuExporter = menuFichier->addMenu("&Exporter");
	QAction *actionExportPBRT = menuExporter->addAction("PBRT");
	QAction *actionExportRAW = menuExporter->addAction("RAW");

        QMenu *menuImporter = menuFichier->addMenu("&Importer");
        QAction *actionImportPBRT = menuImporter->addAction("PBRT");

        QAction *actionQuitter = menuFichier->addAction("&Quitter");

	// Connect objects
	// *** Menu
	QObject::connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
	QObject::connect(actionExportPBRT, SIGNAL(triggered()), this, SLOT(exporterPBRT()));
	QObject::connect(actionExportRAW, SIGNAL(triggered()), this, SLOT(exporterRAW()));
        QObject::connect(actionImportPBRT, SIGNAL(triggered()), this, SLOT(importerPBRT()));

	// *** Other objects
	QObject::connect(ui.bGeneratePerlin, SIGNAL(clicked()), this, SLOT(launchGeneration()));
        QObject::connect(&m_PerlinGenerator, SIGNAL(progressStatus(int)), ui.pbGeneration, SLOT(setValue(int)));
        QObject::connect(ui.hsSlideSelector, SIGNAL(valueChanged(int)), this, SLOT(updateDisplay(int)));
	QObject::connect(&m_PerlinGenerator, SIGNAL(finished()), this, SLOT(newResultToDisplay()));
        QObject::connect(&m_PerlinGenerator, SIGNAL(finished()), this, SLOT(updateContraste()));
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
    std::cout << "Update Display :) " << layer << std::endl;
    if (m_PerlinGenerator.GetCurrentResult()){
        const Perlin3DObject& result = *m_PerlinGenerator.GetCurrentResult();

	QImage image = QImage(result.Size.x, result.Size.y, QImage::Format_ARGB32);
	for (int y = 0; y < result.Size.y; y++) 
	{
            QRgb* rgb = (QRgb*)image.scanLine(result.Size.y-(y+1)); // Il faut que l'image soit en ARGB32 (je pense, voir la doc)
            for (int x = 0; x < result.Size.x; x++)
            {
                float v = ceil(result.GetData(x,y,layer)*255);
                rgb[x] = qRgba(v, v, v, 255);
            }
	}
	//image = image.scaled(m_width,m_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui.lPreview->setPixmap(QPixmap::fromImage(image, Qt::AutoColor));
        ui.lPreview->setAlignment(Qt::AlignCenter);
    }
}

void mainWindow::updateContraste(){
    m_PerlinGenerator.SetScaleValue(ui.hsContraste->value() / 10.f);
    updateDisplay(ui.hsSlideSelector->value());
}

float mainWindow::getContrastedData(float data){
    float contraste = (float) ui.hsContraste->value() / 10.f;
    return std::max(std::min(data * contraste - (contraste / 2.f) + 0.5f,1.f),0.f);
}

void mainWindow::exporter(ExportersAvalaibleType t)
{
	QString fichier = QFileDialog::getSaveFileName(this,"Exporter vers");
	Exporters::Exporter(fichier.toStdString(), (ExportersAvalaibleType)t, *m_PerlinGenerator.GetCurrentResult());
}

void mainWindow::importer(ImportersAvalaibleType t)
{
        Perlin3DObject * result = m_PerlinGenerator.GetCurrentResult();
        QString fichier = QFileDialog::getOpenFileName(this,"Importer depuis");
        Importers::Importer(fichier.toStdString(), (ImportersAvalaibleType)t, *result);
        std::cout << "import done" << std::endl;

/*        // For debugging only
        std::cout << "Values : " << std::endl;
        int nz = 1;
        int ny = 2;
        int nx = 2;

        for (int z = 0; z < nz; ++z)
                for (int y = 0; y < ny; ++y)
                    for (int x = 0; x < nx; ++x){
                         std::cout << "[" << x << "," << y << "," << z << "] = " << result->GetData(x,y,z) << std::endl;
                    }



        // Mise a jour de l'interface
        ui.sbSizeX->setValue(result->Size.x);
        ui.sbSizeY->setValue(result->Size.y);
        ui.sbSizeZ->setValue(result->Size.z);
        std::cout << "plop" << std::endl;
        ui.hsSlideSelector->setMaximum(ui.sbSizeZ->value() - 1);
        ui.hsSlideSelector->setValue(0);

        std::cout << "modification ui done" << std::endl;
        updatePerlinGenerator();
        std::cout << "modification pg done" << std::endl;
        updateDisplay(0);*/
}

