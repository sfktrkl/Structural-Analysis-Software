#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "glwidget.h"
#include "memberdata.h"
#include "structuredata.h"
#include "forcedata.h"
#include "library.h"

#include <QtCore>
#include <QStandardPaths>
#include <QtMath>
#include <QListWidgetItem>
#include <QListWidget>
#include <QString>

#include <vector>
#include <string>
#include <sstream>
#include "saslibrary_global.h"
#include "sasstaticlibrary_global.h"
#include "opengl3d.h"

std::vector<Node> MainWindow::nodes;
std::vector<Member> MainWindow::members;
std::vector<std::vector<double>> MainWindow::KGlobalMatrix;
std::vector<std::vector<double>> MainWindow::FGlobalMatrix;
std::vector<std::vector<double>> MainWindow::Deflections;
bool MainWindow::solved = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createNode_released()
{
    if (ui->openGLWidget->getActivated()){
        double dbXCoordinate = QString(ui->xCoordinate->text()).toDouble();
        double dbYCoordinate = QString(ui->yCoordinate->text()).toDouble();

        bool bXFixity = ui->xFixity->isChecked();
        double dbXStiffness = QString(ui->xStiffness->text()).toDouble();
        bool bYFixity = ui->yFixity->isChecked();
        double dbYStiffness = QString(ui->yStiffness->text()).toDouble();
        bool bZFixity = ui->zFixity->isChecked();
        double dbZStiffness = QString(ui->zStiffness->text()).toDouble();

        double dbXForce = QString(ui->xForce->text()).toDouble();
        double dbYForce = QString(ui->yForce->text()).toDouble();
        double dbZForce = QString(ui->zForce->text()).toDouble();

        nodes.push_back(Node(bXFixity,bYFixity,bZFixity,dbXStiffness,dbYStiffness,dbZStiffness,dbXCoordinate,dbYCoordinate,dbXForce,dbYForce,dbZForce));

        ui->nodeList->addItem(QString::number(Node::numOfNodes-1));
        ui->xCoordinate->selectAll();
        ui->xCoordinate->setFocus();

        drawNodes(bXFixity,bYFixity,bZFixity,int(dbXStiffness),int (dbYStiffness),int(dbZStiffness),dbXCoordinate,dbYCoordinate);
    }
    else{
        messageBox("First Create Grid to Draw!");
    }

}

void MainWindow::on_createMember_released()
{
    unsigned int uiNode1 = QString(ui->mNode1->text()).toUInt();
    unsigned int uiNode2 = QString(ui->mNode2->text()).toUInt();

    double xCoor = qFabs(nodes[uiNode1].GetXCoordinate()-nodes[uiNode2].GetXCoordinate());
    double yCoor = qFabs(nodes[uiNode1].GetYCoordinate()-nodes[uiNode2].GetYCoordinate());
    double dbLength = sqrt(pow(xCoor,2)+pow(yCoor,2));

    double dbCos = (nodes[uiNode2].GetXCoordinate()-nodes[uiNode1].GetXCoordinate())/dbLength;
    double dbSin = (nodes[uiNode2].GetYCoordinate()-nodes[uiNode1].GetYCoordinate())/dbLength;

    double dbE = QString(ui->mElastic->text()).toDouble();
    double dbHeight = QString(ui->mHeight->text()).toDouble();
    double dbWidth = QString(ui->mWidth->text()).toDouble();

    members.push_back(Member(nodes[uiNode1],nodes[uiNode2],dbE,dbHeight,dbWidth,dbLength,dbCos,dbSin));

    ui->memberList->addItem(QString::number(Member::numOfMembers-1));
    ui->mNode1->selectAll();
    ui->mNode1->setFocus();

    ui->openGLWidget->LineDraw(nodes[uiNode1].GetXCoordinate(),nodes[uiNode1].GetYCoordinate(),nodes[uiNode2].GetXCoordinate(),nodes[uiNode2].GetYCoordinate());

}

// edits area and inertia texts in Ui when height and width inputs are given
void MainWindow::on_mWidth_textEdited()
{
    if (ui->mHeight->text().isEmpty() != 1 && ui->mWidth->text().isEmpty() != 1){
        ui->mArea->setText(QString::number(ui->mHeight->text().toDouble()*ui->mWidth->text().toDouble()));
        ui->mInertia->setText(QString::number(pow(ui->mHeight->text().toDouble(),3)*ui->mWidth->text().toDouble()/12));
    }
}

void MainWindow::on_mHeight_textEdited()
{
    if (ui->mHeight->text().isEmpty() != 1 && ui->mWidth->text().isEmpty() != 1){
        ui->mArea->setText(QString::number(ui->mHeight->text().toDouble()*ui->mWidth->text().toDouble()));
        ui->mInertia->setText(QString::number(pow(ui->mHeight->text().toDouble(),3)*ui->mWidth->text().toDouble()/12));
    }
}
// edits length texts in Ui when node inputs are given
void MainWindow::on_mNode1_textEdited()
{
    if(ui->mNode1->text().isEmpty() != 1 && ui->mNode2->text().isEmpty() != 1){
        double xCoor = qFabs(nodes[ui->mNode1->text().toUInt()].GetXCoordinate()-nodes[ui->mNode2->text().toUInt()].GetXCoordinate());
        double yCoor = qFabs(nodes[ui->mNode1->text().toUInt()].GetYCoordinate()-nodes[ui->mNode2->text().toUInt()].GetYCoordinate());
        double dbLength = sqrt(pow(xCoor,2)+pow(yCoor,2));
        ui->mLength->setText(QString::number(dbLength));
    }
}

void MainWindow::on_mNode2_textEdited()
{
    if(ui->mNode1->text().isEmpty() != 1 && ui->mNode2->text().isEmpty() != 1){
        double xCoor = qFabs(nodes[ui->mNode1->text().toUInt()].GetXCoordinate()-nodes[ui->mNode2->text().toUInt()].GetXCoordinate());
        double yCoor = qFabs(nodes[ui->mNode1->text().toUInt()].GetYCoordinate()-nodes[ui->mNode2->text().toUInt()].GetYCoordinate());
        double dbLength = sqrt(pow(xCoor,2)+pow(yCoor,2));
        ui->mLength->setText(QString::number(dbLength));
    }
}
// takes node properties and gives output to Ui
void MainWindow::on_nodeList_itemClicked(QListWidgetItem *item)
{
    QString sData = item->text();
    unsigned int uiData = sData.toUInt();

    ui->xCoordinate->setText(QString::number(nodes[uiData].GetXCoordinate()));
    ui->yCoordinate->setText(QString::number(nodes[uiData].GetYCoordinate()));

    if (nodes[uiData].GetNodeFixity()[0] == true){
        ui->xFixity->setChecked(1);
    }
    else {
        ui->xFixity->setChecked(0);
    }

    if (nodes[uiData].GetNodeFixity()[1] == true){
        ui->yFixity->setChecked(1);
    }
    else {
        ui->yFixity->setChecked(0);
    }

    if (nodes[uiData].GetNodeFixity()[2] == true){
        ui->zFixity->setChecked(1);
    }
    else {
        ui->zFixity->setChecked(0);
    }

    ui->xForce->setText(QString::number(nodes[uiData].nodeForces.get()[0]));
    ui->yForce->setText(QString::number(nodes[uiData].nodeForces.get()[1]));
    ui->zForce->setText(QString::number(nodes[uiData].nodeForces.get()[2]));

    ui->changeNode->setEnabled(1);

}
// sets properties of nodes
void MainWindow::on_changeNode_released()
{
    QString sData = ui->nodeList->currentItem()->text();
    unsigned int uiData = sData.toUInt();

    nodes[uiData].SetNodeCoordinates(QString(ui->xCoordinate->text()).toDouble(),QString(ui->yCoordinate->text()).toDouble());
    nodes[uiData].SetNodeForces(QString(ui->xForce->text()).toDouble(),QString(ui->yForce->text()).toDouble(),QString(ui->zForce->text()).toDouble());
    nodes[uiData].SetNodeFixity(ui->xFixity->isChecked(),ui->yFixity->isChecked(),ui->zFixity->isChecked());

    ui->changeNode->setEnabled(0);
    ui->nodeList->clearSelection();
    ui->xCoordinate->setFocus();
}
// takes member properties and gives output to Ui
void MainWindow::on_memberList_itemClicked(QListWidgetItem *item)
{
    QString sData = item->text();
    unsigned int uiData = sData.toUInt();

    ui->mElastic->setText(QString::number(members[uiData].GetMemberProperties()[0]));
    ui->mHeight->setText(QString::number(members[uiData].GetMemberProperties()[1]));
    ui->mWidth->setText(QString::number(members[uiData].GetMemberProperties()[2]));

    ui->mNode1->setText(QString::number(members[uiData].memberNodes[0].getNodeId()));
    ui->mNode2->setText(QString::number(members[uiData].memberNodes[1].getNodeId()));

    ui->lNode1->setText(QString::number(members[uiData].memberLoads.get()[0]+members[uiData].memberMaterial.unitWeight()*members[uiData].memberMaterial.area()*members[uiData].GetCos()));
    ui->lNode2->setText(QString::number(members[uiData].memberLoads.get()[0]+members[uiData].memberMaterial.unitWeight()*members[uiData].memberMaterial.area()*members[uiData].GetCos()));

    ui->unitWeight->setText(QString::number(members[uiData].memberMaterial.unitWeight()));
    ui->changeMember->setEnabled(1);
    ui->addLoad->setEnabled(1);
    ui->addLoad->setText("Add/Change \nLoads");
}
// sets properties of members
void MainWindow::on_changeMember_released()
{
    QString sData = ui->memberList->currentItem()->text();
    unsigned int uiData = sData.toUInt();

    unsigned int uiNode1 = QString(ui->mNode1->text()).toUInt();
    unsigned int uiNode2 = QString(ui->mNode2->text()).toUInt();

    double dbE = QString(ui->mElastic->text()).toDouble();
    double dbHeight = QString(ui->mHeight->text()).toDouble();
    double dbWidth = QString(ui->mWidth->text()).toDouble();

    double xCoor = qFabs(nodes[uiNode1].GetXCoordinate()-nodes[uiNode2].GetXCoordinate());
    double yCoor = qFabs(nodes[uiNode1].GetYCoordinate()-nodes[uiNode2].GetYCoordinate());
    double dbLength = sqrt(pow(xCoor,2)+pow(yCoor,2));

    double dbCos = (nodes[uiNode2].GetXCoordinate()-nodes[uiNode1].GetXCoordinate())/dbLength;
    double dbSin = (nodes[uiNode2].GetYCoordinate()-nodes[uiNode1].GetYCoordinate())/dbLength;

    members[uiData].setNodes(nodes[uiNode1],nodes[uiNode2]);
    members[uiData].SetMemberProperties(dbE,dbHeight,dbWidth,dbLength,dbCos,dbSin);

    ui->changeMember->setEnabled(0);
    ui->memberList->clearSelection();
    ui->addLoad->setEnabled(0);
    ui->addLoad->setText("First Select \nMember");
    ui->mElastic->setFocus();
}
// activates button when text is edited
void MainWindow::on_unitWeight_textEdited()
{
    ui->SelfWeight->setEnabled(1);
    ui->SelfWeight->setText("Add/Change \nUnit Weight");
}

// sets unitweight of the members
void MainWindow::on_SelfWeight_released()
{
    double unitWeight = QString(ui->unitWeight->text()).toDouble();
    for (unsigned int i = 0;i<Member::numOfMembers;i++){
        members[i].memberMaterial.setUnitWeight(unitWeight);
    }

    for (unsigned int i = 0;i<Member::numOfMembers;i++){
        members[i].SetLoads(-members[i].memberMaterial.area()*unitWeight*members[i].GetCos(),-members[i].memberMaterial.area()*unitWeight*members[i].GetCos());
        nodes[members[i].memberNodes[0].getNodeId()].SetNodeForces(-members[i].GetLenght()*members[i].memberMaterial.area()*unitWeight*members[i].GetSin()*members[i].GetCos()
                                                            ,-members[i].GetLenght()*members[i].memberMaterial.area()*unitWeight*members[i].GetSin()*members[i].GetSin()
                                                            ,0);
    }

    ui->SelfWeight->setEnabled(0);
    ui->SelfWeight->setText("First Enter \nUnit Weight");
    ui->lNode1->setFocus();

}
// adding loads to members
void MainWindow::on_addLoad_released()
{
    double dbLoad1 = QString(ui->lNode1->text()).toDouble();
    double dbLoad2 = QString(ui->lNode2->text()).toDouble();

    unsigned int memberId = QString(ui->memberList->currentItem()->text()).toUInt();

    members[memberId].SetLoads(dbLoad1,dbLoad2);
    ui->addLoad->setEnabled(0);
    ui->addLoad->setText("First Select \nMember");
    ui->memberList->clearSelection();
    ui->changeMember->setEnabled(0);
    ui->lNode1->setFocus();

}
// displays memberData Ui
void MainWindow::on_actionMembers_triggered()
{
    memberData memberDataUi;
    memberDataUi.setModal(true);
    memberDataUi.exec();

}
// solves the problem and displays structureData Ui
void MainWindow::on_solve_released()
{
    solveStructure();
    structureData structureDataUi;
    structureDataUi.setModal(true);
    structureDataUi.exec();
}

void MainWindow::on_actionStructure_triggered()
{
    if (solved == true){
        structureData structureDataUi;
        structureDataUi.setModal(true);
        structureDataUi.exec();
    }
    else {
        messageBox("Solve First");
    }

}
// displays forceData Ui
void MainWindow::on_actionForces_triggered()
{
    forceData forceDataUi;
    forceDataUi.setModal(true);
    forceDataUi.exec();
}

void MainWindow::on_action3D_View_triggered()
{
    OpenGL3D openglWindow;
    openglWindow.setModal(true);
    openglWindow.exec();
}

// saves file to XML
void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr,"Save the file",QDir::homePath());
    WriteXML(fileName);
}
// reads file from XML
void MainWindow::on_actionOpen_triggered()
{

    QString fileName = QFileDialog::getOpenFileName(nullptr,"Open the file",QDir::homePath());
    OpenXML(fileName);

    ui->memberList->clear();
    ui->nodeList->clear();

    for (unsigned int i = 0;i < Node::numOfNodes;i++){
        ui->nodeList->addItem(QString::number(i));
    }

    for (unsigned int i = 0;i<Member::numOfMembers;i++){
        ui->memberList->addItem(QString::number(i));
    }

    ui->createGrid->setEnabled(0);
    ui->openGLWidget->setActivated();

}

// clears the memory and Ui to enter new problem
void MainWindow::on_actionNew_triggered()
{
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Warning", "Unsaved Data will be lost, do you want to save?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes){
        on_actionSave_triggered();
      }
      else {
            clearAll();
            ui->memberList->clear();
            ui->nodeList->clear();
            ui->openGLWidget->clearGL();
            ui->createGrid->setEnabled(1);
      }
}
//
void MainWindow::createGrid(int xGrid,int yGrid){
    ui->openGLWidget->setActivated();
    ui->openGLWidget->sizeUpdateGL(xGrid,yGrid);
    ui->openGLWidget->drawGrids();
    ui->createGrid->setEnabled(0);
}

void MainWindow::on_createGrid_released()
{
    int xGrid = QString(ui->gridX->text()).toInt();
    int yGrid = QString(ui->gridY->text()).toInt();
    createGrid(xGrid,yGrid);

}
// enables fixity lineEdits to enter springs if there is
void MainWindow::on_xFixity_stateChanged(int arg1)
{
    if (arg1 == 2){
        ui->xStiffness->setEnabled(0);
        ui->xStiffness->setText("0");
    }
    else{
        ui->xStiffness->setEnabled(1);
    }
}

void MainWindow::on_yFixity_stateChanged(int arg1)
{
    if (arg1 == 2){
        ui->yStiffness->setEnabled(0);
        ui->yStiffness->setText("0");
    }
    else{
        ui->yStiffness->setEnabled(1);
    }
}

void MainWindow::on_zFixity_stateChanged(int arg1)
{
    if (arg1 == 2){
        ui->zStiffness->setEnabled(0);
        ui->zStiffness->setText("0");
    }
    else{
        ui->zStiffness->setEnabled(1);
    }
}

// drawing nodes according to the fixities to GL
void MainWindow::drawNodes(bool bXFixity,bool bYFixity,bool bZFixity,int iXStiffness,int iYStiffness,int iZStiffness,double dbXCoordinate,double dbYCoordinate){
    ui->openGLWidget->nodeDraw(dbXCoordinate,dbYCoordinate);
    if (bXFixity == true && bYFixity == true && bZFixity == false){
        ui->openGLWidget->pinSupport(dbXCoordinate,dbYCoordinate);
        if (iZStiffness != 0){
            ui->openGLWidget->zSpring(float(dbXCoordinate),float(dbYCoordinate),0.25,100);
        }
    }
    else if (bXFixity == true && bYFixity == true && bZFixity == true){
        ui->openGLWidget->fixedSupport(dbXCoordinate,dbYCoordinate);
    }
    else if (bXFixity == false && bYFixity == true && bZFixity == false){
        ui->openGLWidget->rollerSupport(dbXCoordinate,dbYCoordinate,0);
        if (iXStiffness != 0){
            ui->openGLWidget->xSpring(dbXCoordinate,dbYCoordinate);
        }
        if (iZStiffness != 0){
            ui->openGLWidget->zSpring(float(dbXCoordinate),float(dbYCoordinate),0.25,100);
        }
    }
    else if (bXFixity == true && bYFixity == false && bZFixity == false){
        ui->openGLWidget->rollerSupport(dbXCoordinate,dbYCoordinate,1);
        if (iYStiffness != 0){
            ui->openGLWidget->ySpring(dbXCoordinate,dbYCoordinate);
        }
        if (iZStiffness != 0){
            ui->openGLWidget->zSpring(float(dbXCoordinate),float(dbYCoordinate),0.25,100);
        }
    }
    else if (bXFixity == true && bYFixity == false && bZFixity == true){
        ui->openGLWidget->fixedRollerSupport(dbXCoordinate,dbYCoordinate,1);
        if (iYStiffness != 0){
            ui->openGLWidget->ySpring(dbXCoordinate,dbYCoordinate);
        }
    }
    else if (bXFixity == false && bYFixity == true && bZFixity == true){
        ui->openGLWidget->fixedRollerSupport(dbXCoordinate,dbYCoordinate,0);
        if (iXStiffness != 0){
            ui->openGLWidget->xSpring(dbXCoordinate,dbYCoordinate);
        }
    }
    else if (bXFixity == false && bYFixity == false && bZFixity == true){
        ui->openGLWidget->rotZeroSupport(dbXCoordinate,dbYCoordinate);
        if (iXStiffness != 0){
            ui->openGLWidget->xSpring(dbXCoordinate,dbYCoordinate);
        }
        if (iYStiffness != 0){
            ui->openGLWidget->ySpring(dbXCoordinate,dbYCoordinate);
        }
    }
    else if (bXFixity == false && bYFixity == false && bZFixity == false){
        if (iXStiffness != 0){
            ui->openGLWidget->xSpring(dbXCoordinate,dbYCoordinate);
        }
        if (iYStiffness != 0){
            ui->openGLWidget->ySpring(dbXCoordinate,dbYCoordinate);
        }
        if (iZStiffness != 0){
            ui->openGLWidget->zSpring(float(dbXCoordinate),float(dbYCoordinate),0.25,100);
        }
    }
}


