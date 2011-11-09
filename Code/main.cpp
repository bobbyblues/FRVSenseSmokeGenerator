#include <QApplication>
#include <QPushButton>
#include <QSpinBox>

//Includes perso
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Handlign of the language
    /*QString locale = QLocale::system().name().section('_', 0, 0);

    QTranslator translator;
    translator.load(QString("FRVSenseSmokeGenerator_") + locale);
    app.installTranslator(&translator);*/


    // Creation of a widget acting as a window
    mainWindow w;


    // We display the window
    w.show();

    return app.exec();
}
