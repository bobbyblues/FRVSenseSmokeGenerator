#include "qc3canvas.h"

QC3Canvas::QC3Canvas(QWidget *parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime) :
    QWidget(parent),
    m_Initialized(false)
{

    // Mise en place de quelques options pour autoriser le rendu direct dans le widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    // Définition de la position et de la taille du widget
    move(Position);
    resize(Size);

    // Préparation du timer
    m_Timer.setInterval(FrameTime);
}



#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

void QC3Canvas::showEvent(QShowEvent*)
{
    if (!m_Initialized)
    {
        // Sous X11, il faut valider les commandes qui ont été envoyées au serveur
        // afin de s'assurer que C3 aura une vision à jour de la fenêtre
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // On crée la fenêtre C3 avec l'identificateur du widget
        Create(winId());

        // On laisse la classe dérivée s'initialiser si besoin
        OnInit();

        // On paramètre le timer de sorte qu'il génère un rafraîchissement à la fréquence souhaitée
        connect(&m_Timer, SIGNAL(timeout()), this, SLOT(repaint()));
        m_Timer.start();

        m_Initialized = true;
    }
}

void QC3Canvas::paintEvent(QPaintEvent*)
{
    // On laisse la classe dérivée faire sa tambouille
    OnUpdate();

    // On rafraîchit le widget
    C3::Window::Display();
}

