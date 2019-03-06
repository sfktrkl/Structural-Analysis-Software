#include "glwidget3d.h"
#include "mainwindow.h"
#include "member.h"

GLWidget3D::GLWidget3D(QWidget *parent) : QGLWidget (parent)
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
    timer.start(50);
    // finds max x coordinate to orient the camera
    xMax = 0;
    for (unsigned int i = 0;i < Member::numOfMembers;i++){
        if (MainWindow::members[i].memberNodes[0].GetXCoordinate() > xMax){
            xMax = MainWindow::members[i].memberNodes[0].GetXCoordinate();
        }
        else if(MainWindow::members[i].memberNodes[1].GetXCoordinate() > xMax){
            xMax = MainWindow::members[i].memberNodes[1].GetXCoordinate();
        }
    }
}

void GLWidget3D::initializeGL(){
    glClearColor(GLfloat(0.2),GLfloat(0.2),GLfloat(0.2),1);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
}
void GLWidget3D::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

    glRotatef(GLfloat(this->CameraAngle), 0, 1, 0);
    this->CameraAngle = this->CameraAngle -0.5;
    double xCoor,yCoor,xCoor2,yCoor2,height,width,cos,sin;
    for (unsigned int i = 0;i < Member::numOfMembers;i++){
        xCoor = MainWindow::members[i].memberNodes[0].GetXCoordinate();
        yCoor = MainWindow::members[i].memberNodes[0].GetYCoordinate();
        xCoor2 = MainWindow::members[i].memberNodes[1].GetXCoordinate();
        yCoor2 = MainWindow::members[i].memberNodes[1].GetYCoordinate();
        height = MainWindow::members[i].memberMaterial.height();
        width = MainWindow::members[i].memberMaterial.width();
        cos = MainWindow::members[i].GetCos();
        sin = MainWindow::members[i].GetSin();
        drawMember(xCoor,yCoor,xCoor2,yCoor2,height,width,cos,sin);
    }

    glLoadIdentity();
    glTranslatef(GLfloat(-xMax/2), 0, GLfloat(-xMax*3));

}

void GLWidget3D::resizeGL(int w,int h){
    qreal aspectratio = qreal(w)/qreal(h);
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,static_cast<double>(aspectratio),0.01,500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

// drawing members using GL_QUADS
// http://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_examples.html
void GLWidget3D::drawMember(double xCoor,double yCoor,double xCoor2,double yCoor2,double height,double width,double cos,double sin)
{
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
        // Top face
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f( GLfloat(xCoor2-sin*height/2), GLfloat(yCoor2+height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor-sin*height/2), GLfloat(yCoor+height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor-sin*height/2), GLfloat(yCoor+height/2*cos),  GLfloat(width/2));
        glVertex3f( GLfloat(xCoor2-sin*height/2), GLfloat(yCoor2+height/2*cos),  GLfloat(width/2));

        // Bottom face
        glColor3f(1.0f, 0.5f, 0.0f);     // Orange
        glVertex3f( GLfloat(xCoor2+sin*height/2), GLfloat(yCoor2-height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor+sin*height/2), GLfloat(yCoor-height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor+sin*height/2), GLfloat(yCoor-height/2*cos),  GLfloat(width/2));
        glVertex3f( GLfloat(xCoor2+sin*height/2), GLfloat(yCoor2-height/2*cos),  GLfloat(width/2));

        // Front face
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f( GLfloat(xCoor2-sin*height/2), GLfloat(yCoor2+height/2*cos),  GLfloat(width/2));
        glVertex3f( GLfloat(xCoor-sin*height/2), GLfloat(yCoor+height/2*cos),  GLfloat(width/2));
        glVertex3f( GLfloat(xCoor+sin*height/2), GLfloat(yCoor-height/2*cos),  GLfloat(width/2));
        glVertex3f( GLfloat(xCoor2+sin*height/2), GLfloat(yCoor2-height/2*cos),  GLfloat(width/2));

        // Back face
        glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f( GLfloat(xCoor2-sin*height/2), GLfloat(yCoor2+height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor-sin*height/2), GLfloat(yCoor+height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor+sin*height/2), GLfloat(yCoor-height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor2+sin*height/2), GLfloat(yCoor2-height/2*cos),  GLfloat(-width/2));

        // Left face
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f( GLfloat(xCoor-sin*height/2), GLfloat(yCoor+height/2*cos),  GLfloat(width/2));
        glVertex3f( GLfloat(xCoor-sin*height/2), GLfloat(yCoor+height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor+sin*height/2), GLfloat(yCoor-height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor+sin*height/2), GLfloat(yCoor-height/2*cos),  GLfloat(width/2));

        // Right face
        glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f( GLfloat(xCoor2-sin*height/2), GLfloat(yCoor2+height/2*cos),  GLfloat(width/2));
        glVertex3f( GLfloat(xCoor2-sin*height/2), GLfloat(yCoor2+height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor2+sin*height/2), GLfloat(yCoor2-height/2*cos),  GLfloat(-width/2));
        glVertex3f( GLfloat(xCoor2+sin*height/2), GLfloat(yCoor2-height/2*cos),  GLfloat(width/2));
    glEnd();  // End of drawing color-cube

}

/*
void GLWidget3D::rotateGL()
{

}

void GLWidget3D::clearGL(){

}
*/


