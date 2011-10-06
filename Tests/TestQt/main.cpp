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



//    // Création du bouton, ayant pour parent la "fenetre"
//    QPushButton bouton("Creer image", &fenetre);
//    // Customisation du bouton
//    bouton.setCursor(Qt::PointingHandCursor);

//    // Création de générateur de Perlin
//    PerlinGenerator perlin;
//    QObject::connect(&bouton, SIGNAL(clicked()), &perlin, SLOT(generatePerlin2D()));

//    // Création de bouton pour choisir la taille de l'image
//    QSpinBox size(&fenetre);
//    size.setValue(128);
//    size.setSingleStep(1);
//    size.setMaximum(1024);
//    size.setMinimum(16);
//    size.setSuffix(" pixels");

//    QObject::connect(&size, SIGNAL(valueChanged(int)), &perlin, SLOT(setSize(int)));



    // Affichage de la fenêtre
    w.show();

    return app.exec();
}
