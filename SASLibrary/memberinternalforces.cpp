#include "memberinternalforces.h"
#include "ui_memberinternalforces.h"


MemberInternalForces::MemberInternalForces(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MemberInternalForces)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    for (unsigned int i = 0;i<MainWindow::members.size();i++){
        ui->memberList->addItem(QString::number(i));
    }

}

MemberInternalForces::~MemberInternalForces()
{
    delete ui;
}

void MemberInternalForces::on_memberList_itemClicked(QListWidgetItem *item)
{
    QString sData = item->text();
    unsigned int uiData = sData.toUInt();

    for (unsigned int i = 0; i<6;i++){
            ui->localMatrix->setItem(int(i),int(0),new QTableWidgetItem(QString::number(avoidPrecision(MainWindow::members[uiData].memberLocalForces[i][0]))));
            ui->globalMatrix->setItem(int(i),int(0),new QTableWidgetItem(QString::number(avoidPrecision(MainWindow::members[uiData].memberGlobalForces[i][0]))));
            ui->internalMatrix->setItem(int(i),int(0),new QTableWidgetItem(QString::number(avoidPrecision(MainWindow::members[uiData].memberInternalForces[i][0]))));
    }

    ui->refresh->setEnabled(1);
    ui->refresh->setText("Refresh");
}

void MemberInternalForces::on_refresh_released()
{
    on_memberList_itemClicked(ui->memberList->currentItem());
}
