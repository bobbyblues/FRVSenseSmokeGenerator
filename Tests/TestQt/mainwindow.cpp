#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>

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
    QObject::connect(&m_perlin, SIGNAL(progressStatus(int)), ui.pbGeneration, SLOT(setValue(int)));
    QObject::connect(ui.hsSlideSelector, SIGNAL(valueChanged(int)), this, SLOT(updateDisplay()));
}

void mainWindow::launchGeneration(){
    if (!m_perlin.isRunning()){
    m_perlin.setSizeX(ui.sbSizeX->value());
    m_perlin.setSizeY(ui.sbSizeY->value());
    m_perlin.setSizeZ(ui.sbSizeZ->value());

    ui.hsSlideSelector->setMaximum(ui.sbSizeZ->value() - 1);

    m_perlin.setNbOctave(ui.sbNbOctave->value());
    m_perlin.setStep(ui.dspStep->value());
    m_perlin.setPersistance(ui.dspPersistance->value());

    m_perlin.run();

    updateDisplay();
    }
}

void mainWindow::updateDisplay(){
    m_perlin.writePGMImage("temp.pgm",ui.hsSlideSelector->value());
    ui.lPreview->setPixmap(QPixmap("temp.pgm"));
}

void mainWindow::exporter(ExportersAvalaibleType t)
{
	QString fichier = QFileDialog::getSaveFileName(this,"Exporter vers");
	Exporters::Exporter(fichier.toStdString(), (ExportersAvalaibleType)t);
}
