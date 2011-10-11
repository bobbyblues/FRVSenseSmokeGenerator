 #include <QApplication>
#include <QPushButton>
 #include <QSpinBox>

//Includes perso
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Création d'un widget qui servira de fenêtre

    mainWindow w;


    // Affichage de la fenêtre
    w.show();

    return app.exec();
}
