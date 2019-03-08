#include "structuredata.h"
#include "ui_structuredata.h"

structureData::structureData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::structureData)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    on_refresh_released();

}

structureData::~structureData()
{
    delete ui;
}

void structureData::on_refresh_released()
{
    ui->stiffnessMatrix->setColumnCount(int(Node::numOfDofs-1));
    ui->stiffnessMatrix->setRowCount(int(Node::numOfDofs-1));

    if (Node::numOfDofs-1 != 0){
        for (unsigned int i = 0; i<Node::numOfDofs-1;i++){
            for (unsigned int j = 0; j<Node::numOfDofs-1;j++){
                ui->stiffnessMatrix->setItem(int(i),int(j),new QTableWidgetItem(QString::number(avoidPrecision(MainWindow::KGlobalMatrix[i][j]))));
            }
        }
    }

    ui->forceMatrix->setRowCount(int(Node::numOfDofs-1));

    if (Node::numOfDofs-1 != 0){
        for (unsigned int i = 0; i<Node::numOfDofs-1;i++){
                ui->forceMatrix->setItem(int(i),int(0),new QTableWidgetItem(QString::number(avoidPrecision(MainWindow::FGlobalMatrix[i][0]))));
        }
    }

    ui->deflectionMatrix->setRowCount(int(Node::numOfDofs-1));

    if (Node::numOfDofs-1 != 0){
        for (unsigned int i = 0; i<Node::numOfDofs-1;i++){
                ui->deflectionMatrix->setItem(int(i),int(0),new QTableWidgetItem(QString::number(avoidPrecision(MainWindow::Deflections[i][0]))));
        }
    }

}
