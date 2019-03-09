#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/gl.h>
#include "saslibrary_global.h"

class SASLIBRARYSHARED_EXPORT GLWidget : public QGLWidget
{
    Q_OBJECT
    MAKE_TESTABLE(GLWidget);
public:
    explicit GLWidget(QWidget *parent = nullptr);

    void initializeGL();
    void paintGL();
    void resizeGL(int w,int h);

    void checkNode(bool bXFixity,bool bYFixity,bool bZFixity,double dbXStiffness,double dbYStiffness,double dbZStiffness,double dbXCoordinate,double dbYCoordinate);
    void clearGL();
    void LineDraw(double xCoor1,double yCoor1,double xCoor2,double yCoor2);
    void nodeDraw(double xCoor1,double yCoor1);
    void pinSupport(double xCoor1,double yCoor1);
    void fixedSupport(double xCoor1,double yCoor1);
    void fixedRollerSupport(double xCoor1,double yCoor1,int rotation);
    void rollerSupport(double xCoor1,double yCoor1,int rotation);
    void rotZeroSupport(double xCoor1,double yCoor1);
    void xSpring(double xCoor1,double yCoor1);
    void ySpring(double xCoor1,double yCoor1);
    void zSpring(float xCoor1, float yCoor1, float r, int num_segments);
    void sizeUpdateGL(int maxW,int maxH);
    void setActivated(){activated = true;}
    bool getActivated(){return activated;}
    void drawGrids();
private:
    QTimer timer;
    int maxW=10;
    int maxH=5;
    bool activated = false;
    qreal aspectratio = qreal(600)/qreal(400);
};

#endif // GLWIDGET_H
