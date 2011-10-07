#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainForm.h"
#include "perlingeneratorbutton.h"
#include "C3Canvas.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::mainForm ui;
    PerlinGenerator m_perlin;
    C3Canvas * m_c3;
public:
    explicit mainWindow(QWidget *parent = 0);

signals:

public slots:
    void launchGeneration();
    void updateDisplay();
    void exportPBRT();

};

#endif // MAINWINDOW_H
