#ifndef C3CANVAS_H
#define C3CANVAS_H
#include "qc3canvas.h"
#include <GL/glew.h>


class C3Canvas : public QC3Canvas
{
public :

    C3Canvas(QWidget* Parent, const QPoint& Position, const QSize& Size) :
    QC3Canvas(Parent, Position, Size)
    {

    }

private :

    virtual void OnInit()
    {
        glewInit();
    }

    virtual void OnUpdate()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

};

#endif // C3CANVAS_H
