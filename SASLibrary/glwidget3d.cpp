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
        MainWindow::nodes[MainWindow::members[i].memberNodes[0].getNodeId()].setOffsetOpenGL(MainWindow::members[i].memberMaterial.height()/2);
        MainWindow::nodes[MainWindow::members[i].memberNodes[1].getNodeId()].setOffsetOpenGL(MainWindow::members[i].memberMaterial.height()/2);
        drawMember(xCoor,yCoor,xCoor2,yCoor2,height,width,cos,sin);
    }
    double dbXStiffness,dbYStiffness,dbZStiffness, dbOffset;
    bool bXFixity,bYFixity,bZFixity;
    for (unsigned int i =0 ; i < Node::numOfNodes;i++){
        xCoor = MainWindow::nodes[i].GetXCoordinate();
        yCoor = MainWindow::nodes[i].GetYCoordinate();
        dbXStiffness = MainWindow::nodes[i].GetStiffness()[0];
        dbYStiffness = MainWindow::nodes[i].GetStiffness()[1];
        dbZStiffness = MainWindow::nodes[i].GetStiffness()[2];
        bXFixity = MainWindow::nodes[i].GetNodeFixity()[0];
        bYFixity = MainWindow::nodes[i].GetNodeFixity()[1];
        bZFixity = MainWindow::nodes[i].GetNodeFixity()[2];
        dbOffset = MainWindow::nodes[i].GetOffset();
        checkNode(bXFixity,  bYFixity,  bZFixity,  dbXStiffness,  dbYStiffness,  dbZStiffness, xCoor, yCoor,dbOffset);
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

void GLWidget3D::drawCuboid(double xCoor, double yCoor, double xCoor2, double yCoor2, double heightCos, double heightSin, double width)
{
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
        // Top face
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(width));

        // Bottom face
        glColor3f(1.0f, 0.5f, 0.0f);     // Orange
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(width));

        // Front face
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(width));

        // Back face
        glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(-width));

        // Left face
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(width));

        // Right face
        glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(width));
        glEnd();  // End of drawing color-cube
}

// drawing members using GL_QUADS
// http://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_examples.html
void GLWidget3D::drawMember(double xCoor,double yCoor,double xCoor2,double yCoor2,double height,double width,double cos,double sin)
{
    drawCuboid(xCoor,yCoor,xCoor2,yCoor2,height/2*cos,height/2*sin,width/2);
}

unsigned int GLWidget3D::checkNode(bool bXFixity, bool bYFixity, bool bZFixity, double dbXStiffness, double dbYStiffness, double dbZStiffness,double xCoor,double yCoor,double dbOffset)
{
    unsigned int type;

    if (bXFixity == true && bYFixity == true && bZFixity == false){
        drawPinSupport(xCoor,yCoor-dbOffset);
    }
    else if (bXFixity == true && bYFixity == true && bZFixity == true){
        type = 2;
    }
    else if (bXFixity == true && bYFixity == false && bZFixity == true){
        type = 3;
    }
    else if (bXFixity == true && bYFixity == false && bZFixity == false){
        type = 4;
    }
    else if (bXFixity == false && bYFixity == true && bZFixity == true){
        type = 5;
    }
    else if (bXFixity == false && bYFixity == true && bZFixity == false){
        type = 6;
    }
    else if (bXFixity == false && bYFixity == false && bZFixity == true){
        type = 7;
    }
    else if (bXFixity == false && bYFixity == false && bZFixity == false){
        type = 8;
    }
    else{
        type = 0;
    }


    return type;
}



void GLWidget3D::drawPinSupport(double xCoor, double yCoor)
{
    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
        // Front
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
        glVertex3f( GLfloat(xCoor-0.5), GLfloat(yCoor-1), GLfloat(+0.5));
        glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(+0.5));

        // Right
        glColor3f(1.0f, 0.5f, 0.0f);     // Orange
        glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
        glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(+0.5));
        glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(-0.5));

        // Back
        glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
        glVertex3f( GLfloat(xCoor-0.5), GLfloat(yCoor-1), GLfloat(-0.5));
        glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(-0.5));

        // Left
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
        glVertex3f( GLfloat(xCoor-0.5), GLfloat(yCoor-1), GLfloat(-0.5));
        glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(+0.5));
    glEnd();   // Done drawing the pyramid

    glBegin(GL_QUADS);
        // Bottom
        glColor3f(1.0f,0.0f,0.0f);       // Red
        glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(-0.5));
        glVertex3f( GLfloat(xCoor-0.5), GLfloat(yCoor-1), GLfloat(-0.5));
        glVertex3f( GLfloat(xCoor-0.5), GLfloat(yCoor-1), GLfloat(+0.5));
        glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(+0.5));
    glEnd();
}

void GLWidget3D::drawFixedSupport(double xCoor, double yCoor)
{

}


/*
void GLWidget3D::rotateGL()
{

}

void GLWidget3D::clearGL(){

}
*/


