#ifndef OPENGL3D_H
#define OPENGL3D_H

#include <QDialog>
#include "saslibrary_global.h"

namespace Ui {
class OpenGL3D;
}

class OpenGL3D : public QDialog
{
    Q_OBJECT
    MAKE_TESTABLE(OpenGL3D);
public:
    explicit OpenGL3D(QWidget *parent = nullptr);
    ~OpenGL3D();

private slots:

private:
    Ui::OpenGL3D *ui;
};

#endif // OPENGL3D_H
