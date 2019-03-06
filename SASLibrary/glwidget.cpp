#include "glwidget.h"
#include "math.h"
#include "library.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget (parent)
{
    //connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
    //timer.start(4);
}

// initializing gl to widget
void GLWidget::initializeGL(){
    glClearColor(GLfloat(0.2),GLfloat(0.2),GLfloat(0.2),GLfloat(1));
    glClear(GL_COLOR_BUFFER_BIT);
}
void GLWidget::paintGL(){
}
//  arranges size of GL according to aspect ratio
void GLWidget::resizeGL(int w,int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,w,h);
    glOrtho(-2*aspectratio,10*aspectratio,-2*aspectratio,5*aspectratio,1,-1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    swapBuffers();
}
// clearing GL
void GLWidget::clearGL(){
    glClearColor(GLfloat(0.2),GLfloat(0.2),GLfloat(0.2),GLfloat(1));
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    swapBuffers();
}
// drawes lines according to member properties
void GLWidget::LineDraw(double xCoor1,double yCoor1,double xCoor2,double yCoor2){
    swapBuffers();
    glColor3f(1,0,0);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor2),GLfloat(yCoor2));
    glEnd();
    swapBuffers();
}
// draws points so node coordinates
void GLWidget::nodeDraw(double xCoor1,double yCoor1){
    swapBuffers();
    glColor3f(1,1,1);
    glBegin(GL_POLYGON);
        glVertex2f(GLfloat(xCoor1-0.05),GLfloat(yCoor1-0.05));
        glVertex2f(GLfloat(xCoor1+0.05),GLfloat(yCoor1-0.05));
        glVertex2f(GLfloat(xCoor1+0.05),GLfloat(yCoor1+0.05));
        glVertex2f(GLfloat(xCoor1-0.05),GLfloat(yCoor1+0.05));
    glEnd();
    swapBuffers();
}
// creates pin support visuals to GL
void GLWidget::pinSupport(double xCoor1,double yCoor1){
    swapBuffers();
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
    swapBuffers();
}
// creates roller support visuals to GL
void GLWidget::rollerSupport(double xCoor1,double yCoor1,int rotation){
    if (rotation == 0){
        swapBuffers();
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
        swapBuffers();
    }
    else{
        swapBuffers();
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
        swapBuffers();
    }
}
// creates fixed support visuals to GL
void GLWidget::fixedSupport(double xCoor1,double yCoor1){
    swapBuffers();
    glColor3f(0,1,0);
    glLineWidth(1);
    glBegin(GL_POLYGON);
        glVertex2f(GLfloat(xCoor1-0.25),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor1+0.25),GLfloat(yCoor1));
        glVertex2f(GLfloat(xCoor1+0.25),GLfloat(yCoor1-0.3));
        glVertex2f(GLfloat(xCoor1-0.25),GLfloat(yCoor1-0.3));
    glEnd();
    swapBuffers();

}
// creates fixed support visuals to GL according to orientation of support
void GLWidget::fixedRollerSupport(double xCoor1,double yCoor1,int rotation){
    if (rotation == 0){
        swapBuffers();
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
        swapBuffers();

    }
    else{
        swapBuffers();
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
        swapBuffers();

    }
}
// creates rotation fixed support (rotation fixed only) visuals to GL
void GLWidget::rotZeroSupport(double xCoor1,double yCoor1){
    swapBuffers();
    glColor3f(0,1,0);
    glLineWidth(1);
    glBegin(GL_POLYGON);
        glVertex2f(GLfloat(xCoor1-0.15),GLfloat(yCoor1-0.15));
        glVertex2f(GLfloat(xCoor1+0.15),GLfloat(yCoor1-0.15));
        glVertex2f(GLfloat(xCoor1+0.15),GLfloat(yCoor1+0.15));
        glVertex2f(GLfloat(xCoor1-0.15),GLfloat(yCoor1+0.15));
    glEnd();
    swapBuffers();
}
// creates simple spring with orientation of X axis
void GLWidget::xSpring(double xCoor1,double yCoor1){
    swapBuffers();
    glColor3f(0,1,0);
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
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.6),GLfloat(yCoor1-0.2));
        glVertex2f(GLfloat(xCoor1-0.6),GLfloat(yCoor1+0.2));
    glEnd();
    swapBuffers();

}
// creates simple spring with orientation of Y axis
void GLWidget::ySpring(double xCoor1,double yCoor1){
    swapBuffers();
    glColor3f(0,1,0);
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
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor1-0.15),GLfloat(yCoor1-0.6));
        glVertex2f(GLfloat(xCoor1+0.15),GLfloat(yCoor1-0.6));
    glEnd();
    swapBuffers();

}
// creates simple spring with orientation of Z axis
void GLWidget::zSpring(float xCoor1, float yCoor1, float r, int num_segments){
    swapBuffers();
    double xCoor = double(xCoor1);
    double yCoor = double(yCoor1);
    glColor3f(0,1,0);
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
    glBegin(GL_LINES);
        glVertex2f(GLfloat(xCoor+0.4),GLfloat(yCoor-0.4));
        glVertex2f(GLfloat(xCoor+0.4),GLfloat(yCoor+0.4));
    glEnd();
    swapBuffers();
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
    swapBuffers();
}
// draws grids
void GLWidget::drawGrids(){
    swapBuffers();
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
    swapBuffers();

}

