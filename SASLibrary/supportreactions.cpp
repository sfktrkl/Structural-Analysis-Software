#include "supportreactions.h"
#include "ui_supportreactions.h"

supportReactions::supportReactions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::supportReactions)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    for (unsigned int i = 0;i<MainWindow::nodes.size();i++){
        ui->nodeList->addItem(QString::number(i));
    }
}

supportReactions::~supportReactions()
{
    delete ui;
}

void supportReactions::on_refresh_released()
{
    on_nodeList_itemClicked(ui->nodeList->currentItem());
}

void supportReactions::on_nodeList_itemClicked(QListWidgetItem *item)
{
    QString sData = item->text();
    unsigned int uiData = sData.toUInt();

    for (unsigned int i = 0; i<3;i++){
            ui->nodeForces->setItem(int(i),int(0),new QTableWidgetItem(QString::number(avoidPrecision(MainWindow::nodes[uiData].supportReactions[i]))));
    }

    ui->refresh->setEnabled(1);
    ui->refresh->setText("Refresh");
}
