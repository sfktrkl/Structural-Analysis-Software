#include "opengl3d.h"
#include "ui_opengl3d.h"

OpenGL3D::OpenGL3D(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenGL3D)
{
    ui->setupUi(this);
}

OpenGL3D::~OpenGL3D()
{
    delete ui;
}
