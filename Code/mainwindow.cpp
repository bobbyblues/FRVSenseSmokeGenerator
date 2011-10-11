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
	QAction *actionQuitter = menuFichier->addAction("&Quitter");

	// Connect objects
	// *** Menu
	QObject::connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
	QObject::connect(actionExportPBRT, SIGNAL(triggered()), this, SLOT(exporterPBRT()));
	QObject::connect(actionExportRAW, SIGNAL(triggered()), this, SLOT(exporterRAW()));

	// *** Other objects
	QObject::connect(ui.bGeneratePerlin, SIGNAL(clicked()), this, SLOT(launchGeneration()));
        QObject::connect(&m_PerlinGenerator, SIGNAL(progressStatus(int)), ui.pbGeneration, SLOT(setValue(int)));
        QObject::connect(ui.hsSlideSelector, SIGNAL(valueChanged(int)), this, SLOT(updateDisplay(int)));
	QObject::connect(&m_PerlinGenerator, SIGNAL(finished()), this, SLOT(newResultToDisplay()));
	QObject::connect(ui.dspStep,SIGNAL(valueChanged(double)), this, SLOT(setNewStepValue(double)));
        QObject::connect(ui.hsContraste, SIGNAL(valueChanged(int)), this, SLOT(updateContraste(int)));
}

void mainWindow::launchGeneration(){
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
		m_PerlinGenerator.Compute();
    }
}

void mainWindow::updateDisplay(int layer){
	std::cout << "Update Display :) " << layer << std::endl;
	const Perlin3DObject& result = *m_PerlinGenerator.GetCurrentResult();

	QImage image = QImage(result.Size.x, result.Size.y, QImage::Format_ARGB32);
	for (int y = 0; y < result.Size.y; y++) 
	{
		QRgb* rgb = (QRgb*)image.scanLine(result.Size.y-(y+1)); // Il faut que l'image soit en ARGB32 (je pense, voir la doc)
		for (int x = 0; x < result.Size.x; x++) 
		{
                        float v = ceil(getContrastedData(result.GetData(x,y,layer))*255);
			rgb[x] = qRgba(v, v, v, 255);
		}
	}
	//image = image.scaled(m_width,m_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui.lPreview->setPixmap(QPixmap::fromImage(image, Qt::AutoColor));
	ui.lPreview->setAlignment(Qt::AlignCenter);
}

void mainWindow::updateContraste(int value){
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
