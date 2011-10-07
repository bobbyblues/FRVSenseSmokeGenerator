#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainForm.h"
#include "perlingeneratorbutton.h"
#include "Exporters/Exporters.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::mainForm ui;
    PerlinGenerator m_perlin;

public:
    explicit mainWindow(QWidget *parent = 0);

signals:

public slots:
    void launchGeneration();
    void updateDisplay();
	void exporter(ExportersAvalaibleType t);
};

#endif // MAINWINDOW_H
