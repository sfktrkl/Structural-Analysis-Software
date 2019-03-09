#include "glwidget.h"
#include "math.h"
#include "library.h"
#include "mainwindow.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget (parent)
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
    timer.start(100);
}

// initializing gl to widget
void GLWidget::initializeGL(){
    glClearColor(GLfloat(0.2),GLfloat(0.2),GLfloat(0.2),GLfloat(1));
    glClear(GL_COLOR_BUFFER_BIT);
}
void GLWidget::paintGL(){
    if (activated == true){
        glClear(GL_COLOR_BUFFER_BIT);
        drawGrids();
        // check every member to take material dimensions and coordinates
        double xCoor,yCoor,xCoor2,yCoor2;
        for (unsigned int i = 0;i < Member::numOfMembers;i++){
            xCoor = MainWindow::members[i].memberNodes[0].GetXCoordinate();
            yCoor = MainWindow::members[i].memberNodes[0].GetYCoordinate();
            xCoor2 = MainWindow::members[i].memberNodes[1].GetXCoordinate();
            yCoor2 = MainWindow::members[i].memberNodes[1].GetYCoordinate();
            LineDraw(xCoor,yCoor,xCoor2,yCoor2);
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
            checkNode(bXFixity,  bYFixity,  bZFixity,  dbXStiffness,  dbYStiffness,  dbZStiffness, xCoor, yCoor);
            nodeDraw(xCoor,yCoor);
        }
    }
}

//  arranges size of GL according to aspect ratio
void GLWidget::resizeGL(int w,int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2*aspectratio,10*aspectratio,-2*aspectratio,5*aspectratio,1,-1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// drawing nodes according to the fixities to GL
void GLWidget::checkNode(bool bXFixity,bool bYFixity,bool bZFixity,double dbXStiffness,double dbYStiffness,double dbZStiffness,double dbXCoordinate,double dbYCoordinate){
    nodeDraw(dbXCoordinate,dbYCoordinate);
    if (bXFixity == true && bYFixity == true && bZFixity == false){
        pinSupport(dbXCoordinate,dbYCoordinate);
        if (QString::number(dbZStiffness) != "0"){
            zSpring(float(dbXCoordinate),float(dbYCoordinate),0.25,100);
        }
    }
    else if (bXFixity == true && bYFixity == true && bZFixity == true){
        fixedSupport(dbXCoordinate,dbYCoordinate);
    }
    else if (bXFixity == false && bYFixity == true && bZFixity == false){
        rollerSupport(dbXCoordinate,dbYCoordinate,0);
        if (QString::number(dbXStiffness) != "0"){
            xSpring(dbXCoordinate,dbYCoordinate);
        }
        if (QString::number(dbZStiffness) != "0"){
            zSpring(float(dbXCoordinate),float(dbYCoordinate),0.25,100);
        }
    }
    else if (bXFixity == true && bYFixity == false && bZFixity == false){
        rollerSupport(dbXCoordinate,dbYCoordinate,1);
        if (QString::number(dbYStiffness) != "0"){
            ySpring(dbXCoordinate,dbYCoordinate);
        }
        if (QString::number(dbZStiffness) != "0"){
            zSpring(float(dbXCoordinate),float(dbYCoordinate),0.25,100);
        }
    }
    else if (bXFixity == true && bYFixity == false && bZFixity == true){
        fixedRollerSupport(dbXCoordinate,dbYCoordinate,1);
        if (QString::number(dbYStiffness) != "0"){
            ySpring(dbXCoordinate,dbYCoordinate);
        }
    }
    else if (bXFixity == false && bYFixity == true && bZFixity == true){
        fixedRollerSupport(dbXCoordinate,dbYCoordinate,0);
        if (QString::number(dbXStiffness) != "0"){
            xSpring(dbXCoordinate,dbYCoordinate);
        }
    }
    else if (bXFixity == false && bYFixity == false && bZFixity == true){
        rotZeroSupport(dbXCoordinate,dbYCoordinate);
        if (QString::number(dbXStiffness) != "0"){
            xSpring(dbXCoordinate,dbYCoordinate);
        }
        if (QString::number(dbYStiffness) != "0"){
            ySpring(dbXCoordinate,dbYCoordinate);
        }
    }
    else if (bXFixity == false && bYFixity == false && bZFixity == false){
        if (QString::number(dbXStiffness) != "0"){
            xSpring(dbXCoordinate,dbYCoordinate);
        }
        if (QString::number(dbYStiffness) != "0"){
            ySpring(dbXCoordinate,dbYCoordinate);
        }
        if (QString::number(dbZStiffness) != "0"){
            zSpring(float(dbXCoordinate),float(dbYCoordinate),0.25,100);
        }
    }
}



// clearing GL
void GLWidget::clearGL(){
    glClearColor(GLfloat(0.2),GLfloat(0.2),GLfloat(0.2),GLfloat(1));
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
}
// drawes lines according to member properties
void GLWidget::LineDraw(double xCoor1,double yCoor1,double xCoor2,double yCoor2){
    glColor3f(1,0,0);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor2),GLfloat(yCoor2));
    glEnd();
}
// draws points so node coordinates
void GLWidget::nodeDraw(double xCoor1,double yCoor1){
    glColor3f(1,1,1);
    glBegin(GL_POLYGON);
        glVertex2f(GLfloat(xCoor1-0.05),GLfloat(yCoor1-0.05));
        glVertex2f(GLfloat(xCoor1+0.05),GLfloat(yCoor1-0.05));
        glVertex2f(GLfloat(xCoor1+0.05),GLfloat(yCoor1+0.05));
        glVertex2f(GLfloat(xCoor1-0.05),GLfloat(yCoor1+0.05));
    glEnd();
}
// creates pin support visuals to GL
void GLWidget::pinSupport(double xCoor1,double yCoor1){
    glColor3f(0,1,0);
    glLineWidth(1);
    glBegin(GL_TRIANGLES);
        glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor1+0.25),GLfloat(yCoor1-0.5));
        glVertex2f(GLfloat(xCoor1-0.25),GLfloat(yCoor1-0.5));
    glEnd();
    glColor3f(1,1,1);
    glBegin(GL_POLYGON);
        glVertex2f(GLfloat(xCoor1-0.05),GLfloat(yCoor1-0.05));
        glVertex2f(GLfloat(xCoor1+0.05),GLfloat(yCoor1-0.05));
        glVertex2f(GLfloat(xCoor1+0.05),GLfloat(yCoor1+0.05));
        glVertex2f(GLfloat(xCoor1-0.05),GLfloat(yCoor1+0.05));
    glEnd();
}
// creates roller support visuals to GL
void GLWidget::rollerSupport(double xCoor1,double yCoor1,int rotation){
    if (rotation == 0){
        glColor3f(0,1,0);
        glLineWidth(1);
        glBegin(GL_TRIANGLES);
            glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1));
            glVertex2f(GLfloat(xCoor1+0.25),GLfloat(yCoor1-0.5));
            glVertex2f(GLfloat(xCoor1-0.25),GLfloat(yCoor1-0.5));
        glEnd();
        glLineWidth(1);
        glBegin(GL_LINES);
            glVertex2f(GLfloat(xCoor1+0.25),GLfloat(yCoor1-0.75));
            glVertex2f(GLfloat(xCoor1-0.25),GLfloat(yCoor1-0.75));
        glEnd();
    }
    else{
        glColor3f(0,1,0);
        glLineWidth(1);
        glBegin(GL_TRIANGLES);
            glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1));
            glVertex2f(GLfloat(xCoor1+0.5),GLfloat(yCoor1-0.25));
            glVertex2f(GLfloat(xCoor1+0.5),GLfloat(yCoor1+0.25));
        glEnd();
        glLineWidth(1);
        glBegin(GL_LINES);
            glVertex2f(GLfloat(xCoor1+0.75),GLfloat(yCoor1-0.25));
            glVertex2f(GLfloat(xCoor1+0.75),GLfloat(yCoor1+0.25));
        glEnd();
    }
}
// creates fixed support visuals to GL
void GLWidget::fixedSupport(double xCoor1,double yCoor1){
    glColor3f(0,1,0);
    glLineWidth(1);
    glBegin(GL_POLYGON);
        glVertex2f(GLfloat(xCoor1-0.25),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor1+0.25),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor1+0.25),GLfloat(yCoor1-0.3));
        glVertex2f(GLfloat(xCoor1-0.25),GLfloat(yCoor1-0.3));
    glEnd();

}
// creates fixed support visuals to GL according to orientation of support
void GLWidget::fixedRollerSupport(double xCoor1,double yCoor1,int rotation){
    if (rotation == 0){
        glColor3f(0,1,0);
        glLineWidth(1);
        glBegin(GL_POLYGON);
            glVertex2f(GLfloat(xCoor1-0.25),GLfloat(yCoor1));
            glVertex2f(GLfloat(xCoor1+0.25),GLfloat(yCoor1));
            glVertex2f(GLfloat(xCoor1+0.25),GLfloat(yCoor1-0.3));
            glVertex2f(GLfloat(xCoor1-0.25),GLfloat(yCoor1-0.3));
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(GLfloat(xCoor1-0.25),GLfloat(yCoor1-0.5));
            glVertex2f(GLfloat(xCoor1+0.25),GLfloat(yCoor1-0.5));
        glEnd();

    }
    else{
        glColor3f(0,1,0);
        glLineWidth(1);
        glBegin(GL_POLYGON);
            glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1-0.25));
            glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1+0.25));
            glVertex2f(GLfloat(xCoor1+0.3),GLfloat(yCoor1+0.25));
            glVertex2f(GLfloat(xCoor1+0.3),GLfloat(yCoor1-0.25));
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(GLfloat(xCoor1+0.5),GLfloat(yCoor1-0.25));
            glVertex2f(GLfloat(xCoor1+0.5),GLfloat(yCoor1+0.25));
        glEnd();

    }
}
// creates rotation fixed support (rotation fixed only) visuals to GL
void GLWidget::rotZeroSupport(double xCoor1,double yCoor1){
    glColor3f(0,0,1);
    glLineWidth(1);
    glBegin(GL_POLYGON);
        glVertex2f(GLfloat(xCoor1-0.15),GLfloat(yCoor1-0.15));
        glVertex2f(GLfloat(xCoor1+0.15),GLfloat(yCoor1-0.15));
        glVertex2f(GLfloat(xCoor1+0.15),GLfloat(yCoor1+0.15));
        glVertex2f(GLfloat(xCoor1-0.15),GLfloat(yCoor1+0.15));
    glEnd();
}
// creates simple spring with orientation of X axis
void GLWidget::xSpring(double xCoor1,double yCoor1){
    glColor3f(1.0f, 0.5f, 0.0f);    //Orange
    glLineWidth(1);
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor1-0.1),GLfloat(yCoor1));
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.1),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor1-0.2),GLfloat(yCoor1-0.1));
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.2),GLfloat(yCoor1-0.1));
        glVertex2f(GLfloat(xCoor1-0.3),GLfloat(yCoor1+0.1));
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.3),GLfloat(yCoor1+0.1));
        glVertex2f(GLfloat(xCoor1-0.4),GLfloat(yCoor1-0.1));
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.4),GLfloat(yCoor1-0.1));
        glVertex2f(GLfloat(xCoor1-0.5),GLfloat(yCoor1));
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.5),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor1-0.6),GLfloat(yCoor1));
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.6),GLfloat(yCoor1-0.2));
        glVertex2f(GLfloat(xCoor1-0.6),GLfloat(yCoor1+0.2));
    glEnd();

}
// creates simple spring with orientation of Y axis
void GLWidget::ySpring(double xCoor1,double yCoor1){
    glColor3f(1.0f, 0.5f, 0.0f);    //Orange
    glLineWidth(1);
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1-0.1));
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1-0.1));
        glVertex2f(GLfloat(xCoor1-0.1),GLfloat(yCoor1-0.2));
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.1),GLfloat(yCoor1-0.2));
        glVertex2f(GLfloat(xCoor1+0.1),GLfloat(yCoor1-0.3));
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1+0.1),GLfloat(yCoor1-0.3));
        glVertex2f(GLfloat(xCoor1-0.1),GLfloat(yCoor1-0.4));
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.1),GLfloat(yCoor1-0.4));
        glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1-0.5));
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1-0.5));
        glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1-0.6));
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.15),GLfloat(yCoor1-0.6));
        glVertex2f(GLfloat(xCoor1+0.15),GLfloat(yCoor1-0.6));
    glEnd();

}
// creates simple spring with orientation of Z axis
void GLWidget::zSpring(float xCoor1, float yCoor1, float r, int num_segments){
    double xCoor = double(xCoor1);
    double yCoor = double(yCoor1);
    glColor3f(1.0f, 0.5f, 0.0f);    //Orange
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        glVertex2f(x + xCoor1, y + yCoor1);//output vertex
    }
    glEnd();
    r = r/2;
    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        glVertex2f(x + xCoor1, y + yCoor1);//output vertex
    }
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor),GLfloat(yCoor));
        glVertex2f(GLfloat(xCoor+0.4),GLfloat(yCoor));
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor+0.4),GLfloat(yCoor-0.4));
        glVertex2f(GLfloat(xCoor+0.4),GLfloat(yCoor+0.4));
    glEnd();
}
// updadets the GL axes according to inputs
void GLWidget::sizeUpdateGL(int maxW,int maxH){
    this->maxW = maxW;
    this->maxH = maxH;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2*aspectratio,maxW*aspectratio,-2*aspectratio,maxH*aspectratio,1,-1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// draws grids
void GLWidget::drawGrids(){
    glColor3f(GLfloat(0.1),GLfloat(0.1),GLfloat(0.1));
    glLineWidth(0.5);
    for (int i = 0; i<maxW*3;i++){
        if (i == 3){
            glLineWidth(5);
        }
        else if ((i+2) % 5 == 0){
            glLineWidth(2);
        }
        else{
            glLineWidth(0.5);
        }
        glBegin(GL_LINES);
            glVertex2f(GLfloat(-3+i),GLfloat(-3));
            glVertex2f(GLfloat(-3+i),GLfloat(maxH*2));
        glEnd();
    }
    for (int i = 0; i<maxH*3;i++){
        if (i == 3){
            glLineWidth(5);
        }
        else if ((i+2) % 5 == 0){
            glLineWidth(2);
        }
        else{
            glLineWidth(0.5);
        }
        glBegin(GL_LINES);
            glVertex2f(GLfloat(-3),GLfloat(-3+i));
            glVertex2f(GLfloat(maxW*2),GLfloat(-3+i));
        glEnd();
    }

}

