#ifndef GLWIDGET3D_H
#define GLWIDGET3D_H

#include <QGLWidget>
#include <QTimer>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/gl.h>
#include "saslibrary_global.h"

class SASLIBRARYSHARED_EXPORT GLWidget3D : public QGLWidget
{
    Q_OBJECT
    MAKE_TESTABLE(GLWidget3D);

public:
    explicit GLWidget3D(QWidget *parent = nullptr);

    void initializeGL();
    void paintGL();
    void resizeGL(int w,int h);

    void drawMember(double xCoor,double yCoor,double xCoor2,double yCoor2,double height,double width,double cos,double sin);
    unsigned int checkNode(bool bXFixity,bool bYFixity,bool bZFixity,double dbXStiffness,double dbYStiffness,double dbZStiffness,double xCoor,double yCoor,double dbOffset);
    void drawCuboid(double xCoor,double yCoor,double xCoor2,double yCoor2,double heightCos,double heightSin,double weight);
    void drawPyramid();
    void drawPinSupport(double xCoor,double yCoor);
    void drawFixedSupport(double xCoor,double yCoor);
    //void rotateGL();
    //void clearGL();
private:

    QTimer timer;
    double CameraAngle = 5;
    double xMax;

};

#endif // GLWIDGET3D_H
