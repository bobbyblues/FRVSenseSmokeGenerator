#include "mainwindow.h"

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
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
