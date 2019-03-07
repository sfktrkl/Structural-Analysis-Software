#include "glwidget3d.h"
#include "mainwindow.h"
#include "member.h"

GLWidget3D::GLWidget3D(QWidget *parent) : QGLWidget (parent)
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
    timer.start(15);

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
}
void GLWidget3D::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    // rotation of camera around structure mid point which is xmax/2
    glTranslatef(GLfloat(xMax/2), 0, 0);
    glRotatef(GLfloat(this->CameraAngle), 0, 1, 0);
    this->CameraAngle = this->CameraAngle - 0.5;
    glTranslatef(GLfloat(-xMax/2), 0, 0);

    // check every member to take material dimensions and coordinates
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
        // gets offset according to height of the member for nodes to draw supports
        MainWindow::nodes[MainWindow::members[i].memberNodes[0].getNodeId()].setOffsetOpenGL(MainWindow::members[i].memberMaterial.height()/2);
        MainWindow::nodes[MainWindow::members[i].memberNodes[1].getNodeId()].setOffsetOpenGL(MainWindow::members[i].memberMaterial.height()/2);
        drawMember(xCoor,yCoor,xCoor2,yCoor2,height,width,cos,sin);
    }
    // check every node to find support type and coordinates
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
        drawNode(xCoor,yCoor);
    }

    glLoadIdentity();
    glTranslatef(GLfloat(-xMax/2), 0, -GLfloat(xMax*2));

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
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(width));

        // Bottom face
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(width));

        // Front face
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(width));

        // Back face
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(-width));

        // Left face
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor-heightSin), GLfloat(yCoor+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor+heightSin), GLfloat(yCoor-heightCos),  GLfloat(width));

        // Right face
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(width));
        glVertex3f( GLfloat(xCoor2-heightSin), GLfloat(yCoor2+heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(-width));
        glVertex3f( GLfloat(xCoor2+heightSin), GLfloat(yCoor2-heightCos),  GLfloat(width));
        glEnd();  // End of drawing color-cube
}

void GLWidget3D::drawPyramid(double xCoor, double yCoor,int rotation)
{
    if (rotation == 0){

        glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
            // Front
            glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
            glVertex3f( GLfloat(xCoor-0.5), GLfloat(yCoor-1), GLfloat(+0.5));
            glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(+0.5));

            // Right
            glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
            glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(+0.5));
            glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(-0.5));

            // Back
            glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
            glVertex3f( GLfloat(xCoor-0.5), GLfloat(yCoor-1), GLfloat(-0.5));
            glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(-0.5));

            // Left
            glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
            glVertex3f( GLfloat(xCoor-0.5), GLfloat(yCoor-1), GLfloat(-0.5));
            glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(+0.5));
        glEnd();   // Done drawing the pyramid

        glBegin(GL_QUADS);
            // Bottom
            glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(-0.5));
            glVertex3f( GLfloat(xCoor-0.5), GLfloat(yCoor-1), GLfloat(-0.5));
            glVertex3f( GLfloat(xCoor-0.5), GLfloat(yCoor-1), GLfloat(+0.5));
            glVertex3f( GLfloat(xCoor+0.5), GLfloat(yCoor-1), GLfloat(+0.5));
        glEnd();
    }
    else{
        glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
            // Front
            glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor-0.5), GLfloat(+0.5));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor+0.5), GLfloat(+0.5));

            // Top
            glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor+0.5), GLfloat(+0.5));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor+0.5), GLfloat(-0.5));

            // Back
            glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor-0.5), GLfloat(-0.5));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor+0.5), GLfloat(-0.5));

            // Bottom
            glVertex3f( GLfloat(xCoor), GLfloat(yCoor), GLfloat(0));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor-0.5), GLfloat(+0.5));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor-0.5), GLfloat(-0.5));
        glEnd();   // Done drawing the pyramid

        glBegin(GL_QUADS);
            // Right
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor+0.5), GLfloat(+0.5));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor+0.5), GLfloat(-0.5));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor-0.5), GLfloat(-0.5));
            glVertex3f( GLfloat(xCoor+1), GLfloat(yCoor-0.5), GLfloat(+0.5));
        glEnd();

    }

}

// drawing members using GL_QUADS
// http://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_examples.html
void GLWidget3D::drawMember(double xCoor,double yCoor,double xCoor2,double yCoor2,double height,double width,double cos,double sin)
{
    glColor3f(1.0f,0.0f,0.0f);       // Red
    drawCuboid(xCoor,yCoor,xCoor2,yCoor2,height/2*cos,height/2*sin,width/2);
}

void GLWidget3D::checkNode(bool bXFixity, bool bYFixity, bool bZFixity, double dbXStiffness, double dbYStiffness, double dbZStiffness,double xCoor,double yCoor,double dbOffset)
{
    // for every type of support condition draw support
    if (bXFixity == true && bYFixity == true && bZFixity == false){
        drawPinSupport(xCoor,yCoor-dbOffset);
        if (QString::number(dbZStiffness) != "0"){
            drawZSpring(float(xCoor),float(yCoor),float(-dbOffset));
        }
    }
    else if (bXFixity == true && bYFixity == true && bZFixity == true){
        drawFixedSupport(xCoor,yCoor-dbOffset*2);
    }
    else if (bXFixity == true && bYFixity == false && bZFixity == true){
        drawFixedRollerSupport(xCoor,yCoor,1);
        if (QString::number(dbYStiffness) != "0"){
            drawYSpring(float(xCoor),float(yCoor-dbOffset),0);
        }
    }
    else if (bXFixity == true && bYFixity == false && bZFixity == false){
        drawRollerSupport(xCoor,yCoor,yCoor-dbOffset*2,1);
        if (QString::number(dbYStiffness) != "0"){
            drawYSpring(float(xCoor),float(yCoor-dbOffset),0);
        }
        if (QString::number(dbZStiffness) != "0"){
            drawZSpring(float(xCoor),float(yCoor),float(-dbOffset));
        }
    }
    else if (bXFixity == false && bYFixity == true && bZFixity == true){
        drawFixedRollerSupport(xCoor,yCoor-dbOffset*2,0);
        if (QString::number(dbXStiffness) != "0"){
            drawXSpring(float(xCoor),float(yCoor),0);
        }
    }
    else if (bXFixity == false && bYFixity == true && bZFixity == false){
        drawRollerSupport(xCoor,yCoor-dbOffset,yCoor-dbOffset*2,0);
        if (QString::number(dbXStiffness) != "0"){
            drawXSpring(float(xCoor),float(yCoor),0);
        }
        if (QString::number(dbZStiffness) != "0"){
            drawZSpring(float(xCoor),float(yCoor),float(-dbOffset));
        }
    }
    else if (bXFixity == false && bYFixity == false && bZFixity == true){
        drawRotZeroSupport(xCoor-dbOffset,yCoor);
        if (QString::number(dbXStiffness) != "0"){
            drawXSpring(float(xCoor),float(yCoor),0);
        }
        if (QString::number(dbYStiffness) != "0"){
            drawYSpring(float(xCoor),float(yCoor-dbOffset),0);
        }
    }
    else if (bXFixity == false && bYFixity == false && bZFixity == false){
        drawNode(xCoor,yCoor);
        if (QString::number(dbXStiffness) != "0"){
            drawXSpring(float(xCoor),float(yCoor),0);
        }
        if (QString::number(dbYStiffness) != "0"){
            drawYSpring(float(xCoor),float(yCoor-dbOffset),0);
        }
        if (QString::number(dbZStiffness) != "0"){
            drawZSpring(float(xCoor),float(yCoor),float(-dbOffset));
        }
    }

}

void GLWidget3D::drawPinSupport(double xCoor, double yCoor)
{
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    drawPyramid(xCoor,yCoor,0);
}

void GLWidget3D::drawFixedSupport(double xCoor, double yCoor)
{
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    drawCuboid(xCoor-0.5,yCoor,xCoor+0.5,yCoor,0.5,0,1);
}

void GLWidget3D::drawFixedRollerSupport(double xCoor, double yCoor, int rotation)
{
    if (rotation == 0){
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        drawCuboid(xCoor-0.5,yCoor,xCoor+0.5,yCoor,0.5,0,1);

        drawCuboid(xCoor-0.5,yCoor-0.8,xCoor+0.5,yCoor-0.8,0.1,0,1);
    }
    else{
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        drawCuboid(xCoor,yCoor,xCoor+0.5,yCoor,0.5,0,1);

        drawCuboid(xCoor+0.8,yCoor,xCoor+1,yCoor,0.5,0,1);
    }

}

void GLWidget3D::drawRollerSupport(double xCoor, double yCoor,double yCoor2, int rotation)
{
    if (rotation == 0){
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        drawPyramid(xCoor,yCoor,0);

        drawCuboid(xCoor-0.5,yCoor2-0.8,xCoor+0.5,yCoor2-0.8,0.1,0,0.5);
    }
    else{
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        drawPyramid(xCoor,yCoor,1);

        drawCuboid(xCoor+1.3,yCoor,xCoor+1.5,yCoor,0.5,0,0.5);
    }
}

void GLWidget3D::drawRotZeroSupport(double xCoor, double yCoor)
{
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    drawCuboid(xCoor,yCoor,xCoor+0.5,yCoor,0.75,0,0.75);
}

void GLWidget3D::drawNode(double xCoor, double yCoor)
{
    glColor3f(1.0f, 1.0f, 1.0f);     // White
    drawCuboid(xCoor-0.2,yCoor,xCoor+0.2,yCoor,0.4,0,0.4);
}

// drawing line line strips
// http://www.informit.com/articles/article.aspx?p=328646&seqNum=6
void GLWidget3D::drawXSpring(float xCoor, float yCoor,float zCoor)
{
    glColor3f(1.0f, 0.5f, 0.0f);    //Orange
    float yCoor2,zCoor2;
    // Call only once for all remaining points
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    for(float angle = 0.0f; angle <= (2.0f*3.14f)*5.0f; angle += 0.15f)
      {
      yCoor2 = 0.5f*float(sin(double(angle)));
      zCoor2 = 0.5f*float(cos(double(angle)));
      // Specify the point and move the z value up a little
      glVertex3f(xCoor, yCoor2+yCoor, zCoor2+zCoor);
      xCoor -= 0.005f;
      }
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    drawCuboid(double(xCoor)-0.1,double(yCoor),double(xCoor),double(yCoor),0.5,0,0.5);
}

void GLWidget3D::drawYSpring(float xCoor, float yCoor,float zCoor)
{
    glColor3f(1.0f, 0.5f, 0.0f);    //Orange
    float xCoor2,zCoor2;
    // Call only once for all remaining points
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    for(float angle = 0.0f; angle <= (2.0f*3.14f)*5.0f; angle += 0.15f)
      {
      xCoor2 = 0.5f*float(sin(double(angle)));
      zCoor2 = 0.5f*float(cos(double(angle)));
      // Specify the point and move the z value up a little
      glVertex3f(xCoor2+xCoor, yCoor, zCoor2+zCoor);
      yCoor -= 0.005f;
      }
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    drawCuboid(double(xCoor)-0.5,double(yCoor),double(xCoor)+0.5,double(yCoor),0.1,0,0.5);
}

void GLWidget3D::drawZSpring(float xCoor, float yCoor,float zCoor)
{
    glColor3f(1.0f, 0.5f, 0.0f);    //Orange
    float xCoor2,yCoor2;
    // Call only once for all remaining points
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    for(float angle = 0.0f; angle <= (2.0f*3.14f)*5.0f; angle += 0.15f)
      {
      xCoor2 = 1.0f*float(sin(double(angle)));
      yCoor2 = 1.0f*float(cos(double(angle)));
      // Specify the point and move the z value up a little
      glVertex3f(xCoor2+xCoor, yCoor2+yCoor, zCoor);
      zCoor -= 0.005f;
      }
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
        // Top face
        glVertex3f( xCoor+1.0f, yCoor+1.0f,  zCoor+0.2f);
        glVertex3f( xCoor-1.0f, yCoor+1.0f,  zCoor+0.2f);
        glVertex3f( xCoor-1.0f, yCoor+1.0f,  zCoor);
        glVertex3f( xCoor+1.0f, yCoor+1.0f,  zCoor);

        // Bottom face
        glVertex3f( xCoor+1.0f, yCoor-1.0f,  zCoor+0.2f);
        glVertex3f( xCoor-1.0f, yCoor-1.0f,  zCoor+0.2f);
        glVertex3f( xCoor-1.0f, yCoor-1.0f,  zCoor);
        glVertex3f( xCoor+1.0f, yCoor-1.0f,  zCoor);

        // Front face
        glVertex3f( xCoor+1.0f, yCoor+1.0f,  zCoor);
        glVertex3f( xCoor-1.0f, yCoor+1.0f,  zCoor);
        glVertex3f( xCoor-1.0f, yCoor-1.0f,  zCoor);
        glVertex3f( xCoor+1.0f, yCoor-1.0f,  zCoor);

        // Back face
        glVertex3f( xCoor+1.0f, yCoor+1.0f,  zCoor+0.2f);
        glVertex3f( xCoor-1.0f, yCoor+1.0f,  zCoor+0.2f);
        glVertex3f( xCoor-1.0f, yCoor-1.0f,  zCoor+0.2f);
        glVertex3f( xCoor+1.0f, yCoor-1.0f,  zCoor+0.2f);

        // Left face
        glVertex3f( xCoor+1.0f, yCoor+1.0f,  zCoor+0.2f);
        glVertex3f( xCoor+1.0f, yCoor+1.0f,  zCoor);
        glVertex3f( xCoor+1.0f, yCoor-1.0f,  zCoor);
        glVertex3f( xCoor+1.0f, yCoor-1.0f,  zCoor+0.2f);

        // Right face
        glVertex3f( xCoor-1.0f, yCoor+1.0f,  zCoor+0.2f);
        glVertex3f( xCoor-1.0f, yCoor+1.0f,  zCoor);
        glVertex3f( xCoor-1.0f, yCoor-1.0f,  zCoor);
        glVertex3f( xCoor-1.0f, yCoor-1.0f,  zCoor+0.2f);
        glEnd();  // End of drawing color-cube

}


/*
void GLWidget3D::rotateGL()
{

}

void GLWidget3D::clearGL(){

}
*/


