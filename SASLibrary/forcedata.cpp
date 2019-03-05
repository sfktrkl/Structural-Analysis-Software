#include "forcedata.h"
#include "ui_forcedata.h"
#include "mainwindow.h"
#include "library.h"

forceData::forceData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::forceData)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    for (unsigned int i = 0;i<MainWindow::members.size();i++){
        ui->memberList->addItem(QString::number(i));
    }

    for (unsigned int i = 0;i<MainWindow::nodes.size();i++){
        ui->nodeList->addItem(QString::number(i));
    }

}

forceData::~forceData()
{
    delete ui;
}

void forceData::on_memberList_itemClicked(QListWidgetItem *item)
{
    QString sData = item->text();
    unsigned int uiData = sData.toUInt();

    for (unsigned int i = 0; i<6;i++){
        for (unsigned int j = 0; j<6;j++){
            ui->rotationMatrix->setItem(int(i),int(j),new QTableWidgetItem(QString::number(MainWindow::members[uiData].GetRotationMatrix()[i][j])));
        }
    }

    for (unsigned int i = 0; i<6;i++){
        for (unsigned int j = 0; j<1;j++){
            ui->localMatrix->setItem(int(i),int(j),new QTableWidgetItem(QString::number(MainWindow::members[uiData].GetLocalFEMMatrix()[i][j])));
            ui->globalMatrix->setItem(int(i),int(j),new QTableWidgetItem(QString::number(MainWindow::members[uiData].GetGlobalFEMMatrix()[i][j])));
        }
    }

    ui->refresh->setEnabled(1);
    ui->refresh->setText("Refresh");
}


void forceData::on_refresh_released()
{
    on_memberList_itemClicked(ui->memberList->currentItem());
}

void forceData::on_nodeList_itemClicked(QListWidgetItem *item)
{
    QString sData = item->text();
    unsigned int uiData = sData.toUInt();

    for (unsigned int i = 0; i<3;i++){
            ui->nodeForces->setItem(int(i),int(0),new QTableWidgetItem(QString::number(MainWindow::nodes[uiData].nodeForces.get()[i])));
    }

    ui->refresh_2->setEnabled(1);
    ui->refresh_2->setText("Refresh");
}


void forceData::on_refresh_2_released()
{
    on_nodeList_itemClicked(ui->nodeList->currentItem());
}
