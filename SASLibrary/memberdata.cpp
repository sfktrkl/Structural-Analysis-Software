#include "memberdata.h"
#include "ui_memberdata.h"
#include "mainwindow.h"

memberData::memberData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::memberData)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    for (unsigned int i = 0;i<MainWindow::members.size();i++){
        ui->memberList->addItem(QString::number(i));
    }

}

memberData::~memberData()
{
    delete ui;
}
// takes member local, global and rotation matrices from member class
void memberData::on_memberList_itemClicked(QListWidgetItem *item)
{
    QString sData = item->text();
    unsigned int uiData = sData.toUInt();

    for (unsigned int i = 0; i<6;i++){
        for (unsigned int j = 0; j<6;j++){
            ui->localMatrix->setItem(int(i),int(j),new QTableWidgetItem(QString::number(MainWindow::members[uiData].GetLocalMatrix()[i][j])));
            ui->globalMatrix->setItem(int(i),int(j),new QTableWidgetItem(QString::number(MainWindow::members[uiData].GetGlobalMatrix()[i][j])));
            ui->rotationMatrix->setItem(int(i),int(j),new QTableWidgetItem(QString::number(MainWindow::members[uiData].GetRotationMatrix()[i][j])));
        }
    }

    ui->refresh->setEnabled(1);
    ui->refresh->setText("Refresh");

}

void memberData::on_refresh_released()
{
    on_memberList_itemClicked(ui->memberList->currentItem());
}
