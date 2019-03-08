#ifndef MEMBERINTERNALFORCES_H
#define MEMBERINTERNALFORCES_H

#include <QDialog>
#include "mainwindow.h"
#include "library.h"
namespace Ui {
class MemberInternalForces;
}

class MemberInternalForces : public QDialog
{
    Q_OBJECT

public:
    explicit MemberInternalForces(QWidget *parent = nullptr);
    ~MemberInternalForces();

private slots:
    void on_memberList_itemClicked(QListWidgetItem *item);

    void on_refresh_released();

private:
    Ui::MemberInternalForces *ui;
};

#endif // MEMBERINTERNALFORCES_H
