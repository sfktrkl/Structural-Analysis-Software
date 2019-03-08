#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QtXml/QtXml>
#include <QtCore>
#include <QStandardPaths>
#include <QtMath>
#include <QListWidgetItem>
#include <QListWidget>
#include <QString>
#include "library.h"

// empties the member node and global matrices
void MainWindow::clearAll()
{
    members.clear();
    nodes.clear();
    Member::numOfMembers = 0;
    Node::numOfNodes = 0;
    Node::numOfDofs = 1;
    KGlobalMatrix.clear();
    FGlobalMatrix.clear();
    Deflections.clear();
    solved = 0;

}
// creates global matrices and solves given problem
void MainWindow::solveStructure()
{
    // create structure global stiffness matrix
    std::vector<std::vector<double>> tempKMatrix(Node::numOfDofs-1, std::vector<double>(Node::numOfDofs-1));

    for (unsigned int i = 0;i<Member::numOfMembers;i++){
        for (unsigned int j = 0;j<6;j++){
            for (unsigned int k = 0;k<6;k++){
                if (members[i].GetDofNumbers()[j] != 0 && members[i].GetDofNumbers()[k] != 0){
                    tempKMatrix[members[i].GetDofNumbers()[j]-1][members[i].GetDofNumbers()[k]-1]
                            = members[i].GetGlobalMatrix()[j][k] + tempKMatrix[members[i].GetDofNumbers()[j]-1][members[i].GetDofNumbers()[k]-1];
                }
            }
        }
    }

    for (unsigned int i = 0;i<Node::numOfNodes;i++){
        for (unsigned int j = 0;j<3;j++){
            if (nodes[i].GetDofNumbers()[j] != 0){
                tempKMatrix[nodes[i].GetDofNumbers()[j]-1][nodes[i].GetDofNumbers()[j]-1]
                        = tempKMatrix[nodes[i].GetDofNumbers()[j]-1][nodes[i].GetDofNumbers()[j]-1] + nodes[i].GetStiffness()[j];
            }
        }
    }

    KGlobalMatrix = tempKMatrix;

    // create structure force matrix
    std::vector<std::vector<double>> tempForceMatrix(Node::numOfDofs-1, std::vector<double>(1));

    for (unsigned int i = 0;i<Member::numOfMembers;i++){
        for (unsigned int j = 0;j<6;j++){
            if (members[i].GetDofNumbers()[j] != 0){
                tempForceMatrix[members[i].GetDofNumbers()[j]-1][0]
                        = -members[i].GetGlobalFEMMatrix()[j][0] + tempForceMatrix[members[i].GetDofNumbers()[j]-1][0];
            }
        }
    }

    for (unsigned int i = 0;i<Node::numOfNodes;i++){
        for (unsigned int j = 0;j<3;j++){
            if (nodes[i].GetDofNumbers()[j] != 0){
                tempForceMatrix[nodes[i].GetDofNumbers()[j]-1][0]
                        = tempForceMatrix[nodes[i].GetDofNumbers()[j]-1][0] + nodes[i].nodeForces.get()[j];
            }
        }
    }

    FGlobalMatrix = tempForceMatrix;

    Deflections = gaussJordan(KGlobalMatrix,FGlobalMatrix);
    solved = true;

    // create member internal forces matrices
    for (unsigned int i = 0;i<Member::numOfMembers;i++){
        members[i].memberLocalForces = calculateMemberLocalForces(i);
        members[i].memberGlobalForces = multiplication(matrixInverse(members[i].GetRotationMatrix()),members[i].memberLocalForces);

        // arranges signs according to sign convention
        std::vector<std::vector<double>> tempInternalForces(6,std::vector<double>(1));
        for (unsigned int j = 0;j<6;j++){
            if (j<3){
                tempInternalForces[j][0] = -1*members[i].memberLocalForces[j][0];
            }
            else{
                tempInternalForces[j][0] = members[i].memberLocalForces[j][0];
            }
        }

        members[i].memberInternalForces = tempInternalForces;
    }

    // creates support reactions matrices and adds support reactions from nodes
    for (unsigned int  i= 0;i<Node::numOfNodes;i++){
        nodes[i].supportReactions = {0,0,0};
        for (unsigned int j = 0 ; j<3;j++){
                nodes[i].supportReactions[j] = nodes[i].supportReactions[j] - nodes[i].nodeForces.get()[j];

        }
    }
    // adds from member forces
    for (unsigned int i = 0;i<Member::numOfMembers;i++){
        for (unsigned int j = 0;j<6;j++){
                if (j<3){
                    nodes[members[i].memberNodes[0].getNodeId()].supportReactions[j] = members[i].memberGlobalForces[j][0] + nodes[members[i].memberNodes[0].getNodeId()].supportReactions[j];
                }
                else{
                    nodes[members[i].memberNodes[1].getNodeId()].supportReactions[j-3] = members[i].memberGlobalForces[j][0] + nodes[members[i].memberNodes[1].getNodeId()].supportReactions[j-3];
                }
        }
    }


}

std::vector<std::vector<double> > MainWindow::calculateMemberLocalForces(unsigned int memberId)
{
    std::vector<std::vector<double>> MemberLocalForces(6,std::vector<double>(1));

    std::vector<std::vector<double>> tempDeflections(6,std::vector<double>(1));

    for (unsigned int i = 0;i<Member::numOfMembers;i++){
        for (unsigned int j = 0;j<6;j++){
            if (members[memberId].GetDofNumbers()[j] != 0){
                tempDeflections[j][0] = Deflections[members[memberId].GetDofNumbers()[j]-1][0];
            }
            else if (members[memberId].GetDofNumbers()[j] == 0){
                tempDeflections[j][0] = 0;
            }
        }
        tempDeflections = multiplication(members[memberId].GetRotationMatrix(),tempDeflections);
        MemberLocalForces = multiplication(members[memberId].GetLocalMatrix(),tempDeflections);
    }

    for (unsigned int i = 0;i<6;i++){
            MemberLocalForces[i][0] = members[memberId].GetLocalFEMMatrix()[i][0] + MemberLocalForces[i][0];
    }

    return MemberLocalForces;
}


std::vector<std::vector<double> > MainWindow::calculateMemberInternalForces(unsigned int memberId)
{
    std::vector<std::vector<double>> MemberLocalForces(6,std::vector<double>(1));

    std::vector<std::vector<double>> tempDeflections(6,std::vector<double>(1));

    for (unsigned int i = 0;i<Member::numOfMembers;i++){
        for (unsigned int j = 0;j<6;j++){
            if (members[memberId].GetDofNumbers()[j] != 0){
                tempDeflections[j][0] = Deflections[members[memberId].GetDofNumbers()[j]][0];
            }
            else{
                tempDeflections[j][0] = 0;
            }
        }
        tempDeflections = multiplication(members[memberId].GetRotationMatrix(),tempDeflections);
        MemberLocalForces = multiplication(members[memberId].GetLocalMatrix(),tempDeflections);
    }

    for (unsigned int i = 0;i<6;i++){
            MemberLocalForces[i][0] = members[memberId].GetLocalFEMMatrix()[i][0] + MemberLocalForces[i][0];
    }

    return MemberLocalForces;
}




// creates XML file according to member and node classes
void MainWindow::WriteXML(QString fileName){
    //Write XML
    MainWindow mainW;
    QDomDocument document;

    //Make root element
    QDomElement root = document.createElement("Class");

    //Add to document
    document.appendChild(root);

    QDomElement node = document.createElement("Nodes");
    root.appendChild(node);
    QDomElement member = document.createElement("Members");
    root.appendChild(member);
    QDomElement opengl = document.createElement("OpenGL");
    root.appendChild(opengl);

    QDomElement openglelement = document.createElement("Grid");
    openglelement.setAttribute("xGrid",mainW.ui->gridX->text());
    openglelement.setAttribute("yGrid",mainW.ui->gridY->text());
    opengl.appendChild(openglelement);

    //Add elements
    for(unsigned int i = 0;i < Node::numOfNodes ;i++){
        QDomElement nodeelement = document.createElement("Node");
        nodeelement.setAttribute("ID", QString::number(i));

        bool x = MainWindow::nodes[i].GetNodeFixitiesForXML()[0];
        QString text = x ? "true" : "false";
        nodeelement.setAttribute("X_Direction_Fixity", text);
        x = MainWindow::nodes[i].GetNodeFixitiesForXML()[1];
        text = x ? "true" : "false";
        nodeelement.setAttribute("Y_Direction_Fixity", text);
        x = MainWindow::nodes[i].GetNodeFixitiesForXML()[2];
        text = x ? "true" : "false";
        nodeelement.setAttribute("Z_Direction_Fixity", text);

        nodeelement.setAttribute("X_Stiffness", QString::number(MainWindow::nodes[i].GetNodePropertiesForXML()[0]));
        nodeelement.setAttribute("Y_Stiffness", QString::number(MainWindow::nodes[i].GetNodePropertiesForXML()[1]));
        nodeelement.setAttribute("Z_Stiffness", QString::number(MainWindow::nodes[i].GetNodePropertiesForXML()[2]));

        nodeelement.setAttribute("X_Coordinate", QString::number(MainWindow::nodes[i].GetNodePropertiesForXML()[3]));
        nodeelement.setAttribute("Y_Coordinate", QString::number(MainWindow::nodes[i].GetNodePropertiesForXML()[4]));

        nodeelement.setAttribute("X_Force", QString::number(MainWindow::nodes[i].GetNodePropertiesForXML()[5]));
        nodeelement.setAttribute("Y_Force", QString::number(MainWindow::nodes[i].GetNodePropertiesForXML()[6]));
        nodeelement.setAttribute("Z_Force", QString::number(MainWindow::nodes[i].GetNodePropertiesForXML()[7]));

        node.appendChild(nodeelement);
    }

    for(unsigned int i = 0;i < Member::numOfMembers ;i++){
        QDomElement memberelement = document.createElement("Member");
        memberelement.setAttribute("ID", QString::number(i));
        memberelement.setAttribute("First_Node", QString::number(MainWindow::members[i].memberNodes[0].getNodeId()));
        memberelement.setAttribute("Second_Node", QString::number(MainWindow::members[i].memberNodes[1].getNodeId()));
        memberelement.setAttribute("Elastic_Modulus", QString::number(MainWindow::members[i].memberMaterial.elasticModulus()));
        memberelement.setAttribute("Height", QString::number(MainWindow::members[i].memberMaterial.height()));
        memberelement.setAttribute("Width", QString::number(MainWindow::members[i].memberMaterial.width()));
        memberelement.setAttribute("Length", QString::number(MainWindow::members[i].GetMemberPropertiesForXML()[3]));
        memberelement.setAttribute("Cos", QString::number(MainWindow::members[i].GetMemberPropertiesForXML()[4]));
        memberelement.setAttribute("Sin", QString::number(MainWindow::members[i].GetMemberPropertiesForXML()[5]));
        memberelement.setAttribute("Load_Node1", QString::number(MainWindow::members[i].GetLoads()[0]));
        memberelement.setAttribute("Load_Node2", QString::number(MainWindow::members[i].GetLoads()[1]));
        memberelement.setAttribute("Unit_Weight", QString::number(MainWindow::members[i].memberMaterial.unitWeight()));
        member.appendChild(memberelement);
    }

    //Write to file
    QFile file(fileName+".xml");
    if(!file.open(QIODevice::WriteOnly |QIODevice::Text)){
        messageBox("Failed to open file");
    }
    else {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
    }
}
// parsing of XML file
void MainWindow::OpenXML(QString fileName){

    clearAll();
    MainWindow mainW;
    mainW.ui->openGLWidget->clearGL();

    QDomDocument document;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        messageBox("Failed to open file");
    }
    else {
        if(!document.setContent(&file)){
            messageBox("Failed to load file");
        }
        file.close();
    }

    QDomElement root = document.firstChildElement();

    QDomNodeList gridslist = root.elementsByTagName("OpenGL");
    QDomNode gridsnode = gridslist.at(0);
    QDomElement gridselement = gridsnode.toElement();
    QDomNodeList gridlist = gridselement.elementsByTagName("Grid");
    QDomNode gridnode = gridlist.at(0);
    QDomElement gridelement = gridnode.toElement();
    mainW.createGrid(QString(gridelement.attribute("xGrid")).toInt(),QString(gridelement.attribute("yGrid")).toInt());

    QDomNodeList nodeslist = root.elementsByTagName("Nodes");
    QDomNode nodesnode = nodeslist.at(0);
    QDomElement nodeselement = nodesnode.toElement();
    QDomNodeList nodelist = nodeselement.elementsByTagName("Node");

    for (int i = 0; i < nodelist.count();i++){
        QDomNode nodenode = nodelist.at(i);
        if(nodenode.isElement()){
            QDomElement nodeelement = nodenode.toElement();

            double dbXCoordinate = QString(nodeelement.attribute("X_Coordinate")).toDouble();
            double dbYCoordinate = QString(nodeelement.attribute("Y_Coordinate")).toDouble();
            bool bXFixity;
            if (QString(nodeelement.attribute("X_Direction_Fixity")).toStdString() == "true"){
                bXFixity = true;
            }
            else {
                bXFixity = false;
            }
            double dbXStiffness = QString(nodeelement.attribute("X_Stiffness")).toDouble();
            bool bYFixity;
            if (QString(nodeelement.attribute("Y_Direction_Fixity")).toStdString() == "true"){
                bYFixity = true;
            }
            else {
                bYFixity = false;
            }
            double dbYStiffness = QString(nodeelement.attribute("Y_Stiffness")).toDouble();
            bool bZFixity;
            if (QString(nodeelement.attribute("Z_Direction_Fixity")).toStdString() == "true"){
                bZFixity = true;
            }
            else {
                bZFixity = false;
            }
            double dbZStiffness = QString(nodeelement.attribute("Z_Stiffness")).toDouble();

            double dbXForce = QString(nodeelement.attribute("X_Force")).toDouble();
            double dbYForce = QString(nodeelement.attribute("Y_Force")).toDouble();
            double dbZForce = QString(nodeelement.attribute("Z_Force")).toDouble();
            MainWindow::nodes.push_back(Node(bXFixity,bYFixity,bZFixity,dbXStiffness,dbYStiffness,dbZStiffness,dbXCoordinate,dbYCoordinate,dbXForce,dbYForce,dbZForce));

            mainW.drawNodes(bXFixity,bYFixity,bZFixity,int(dbXStiffness),int (dbYStiffness),int(dbZStiffness),dbXCoordinate,dbYCoordinate);

        }
    }

    QDomNodeList memberslist = root.elementsByTagName("Members");
    QDomNode membersnode = memberslist.at(0);
    QDomElement memberselement = membersnode.toElement();
    QDomNodeList memberlist = memberselement.elementsByTagName("Member");
    for (int i = 0; i < memberlist.count();i++){
        QDomNode membernode = memberlist.at(i);
        if(membernode.isElement()){
            QDomElement memberelement = membernode.toElement();

            unsigned int uiNode1 = QString(memberelement.attribute("First_Node")).toUInt();
            unsigned int uiNode2 = QString(memberelement.attribute("Second_Node")).toUInt();

            double dbE = double(QString(memberelement.attribute("Elastic_Modulus")).toDouble());
            double dbHeight = double(QString(memberelement.attribute("Height")).toDouble());
            double dbWidth = double(QString(memberelement.attribute("Width")).toDouble());
            double dbLength = double(QString(memberelement.attribute("Length")).toDouble());

            double dbCos = QString(memberelement.attribute("Cos")).toDouble();
            double dbSin = QString(memberelement.attribute("Sin")).toDouble();

            unsigned int memberId = QString(memberelement.attribute("ID")).toUInt();

            double dbLoad1 = double(QString(memberelement.attribute("Load_Node1")).toDouble());
            double dbLoad2 = double(QString(memberelement.attribute("Load_Node2")).toDouble());

            double unitWeight = double(QString(memberelement.attribute("Unit_Weight")).toDouble());

            MainWindow::members.push_back(Member(nodes[uiNode1],nodes[uiNode2],dbE,dbHeight,dbWidth,dbLength,dbCos,dbSin));
            MainWindow::members[memberId].memberMaterial.setUnitWeight(unitWeight);
            MainWindow::members[memberId].SetLoads(dbLoad1,dbLoad2);

            mainW.ui->openGLWidget->LineDraw(nodes[uiNode1].GetXCoordinate(),nodes[uiNode1].GetYCoordinate(),nodes[uiNode2].GetXCoordinate(),nodes[uiNode2].GetYCoordinate());

        }
    }


}
