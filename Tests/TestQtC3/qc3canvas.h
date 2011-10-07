#ifndef QC3CANVAS_H
#define QC3CANVAS_H

#include <QWidget>
#include <C3/Window.h>
#include <Qt/qwidget.h>
#include <Qt/qtimer.h>

class QC3Canvas : public QWidget, public C3::Window
{
    Q_OBJECT

private:
    QTimer m_Timer;
    bool   m_Initialized;

public:
    explicit QC3Canvas(QWidget *parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 0);

    virtual ~QC3Canvas(){}

private:
    virtual void OnInit() = 0;

    virtual void OnUpdate() = 0;

    virtual void showEvent(QShowEvent*);

    virtual void paintEvent(QPaintEvent*);


signals:

public slots:

};

#endif // QC3CANVAS_H
