#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainForm.h"
#include "PerlinGenerator.h"
#include "Exporters/Exporters.h"
#include "Importers/Importers.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::mainForm ui;
    PerlinGenerator m_PerlinGenerator;
    Perlin3DObject * m_Perlin3DObject;
    Importers * m_importers;


public:
    explicit mainWindow(QWidget *parent = 0);

private:
    void exporter(ExportersAvalaibleType t);
    float getContrastedData(float data);

signals:

public slots:
    void updatePerlinGenerator();
    void launchGeneration();
    void newResultFromGeneration() {
        ui.hsSlideSelector->setValue(0);
        if (m_Perlin3DObject) delete m_Perlin3DObject;
        m_Perlin3DObject = m_PerlinGenerator.GetCurrentResult();
        updateContraste();
        updateDisplay(0);
    }

    void updateDisplay(int layer);
    void updateContraste();
    void setNewStepValue(double v) {
        if(v < std::pow(2.f, ui.sbNbOctave->value() -1))
            ui.dspStep->setValue(std::pow(2.f, ui.sbNbOctave->value() -1));
    }
    void exporterPBRT() { exporter(PBRT_EXPORTER); };
    void exporterRAW() { exporter(RAW_EXPORTER); };

    void importer();

};

#endif // MAINWINDOW_H
