#include "member.h"
#include "node.h"
#include <math.h>
#include "library.h"
#include "mainwindow.h"

#include <QString>

unsigned int Member::numOfMembers = 0;

Member::Member(unsigned int uiNode1,unsigned int uiNode2,double dbElasticModulus,double dbHeight,double dbWidth,double dbLength,double dbCos,double dbSin){
    this->vecNodes.push_back(uiNode1);
    this->vecNodes.push_back(uiNode2);
    this->dbLength = dbLength;

    this->dbElasticModulus = dbElasticModulus;
    this->dbHeight = dbHeight;
    this->dbWidth = dbWidth;
    this->dbArea = this->dbHeight * this->dbWidth;
    this->dbInertia = pow(this->dbHeight,3) * this->dbWidth / 12;
    this->dbCos = dbCos;
    this->dbSin = dbSin;

    this->memberId = numOfMembers;
    Member::numOfMembers++;

    this->mLocalMatrix = CreateLocalMatrix(this->dbElasticModulus,this->dbInertia,this->dbLength,this->dbArea);
    //messageBox(this->mLocalMatrix[1][1]);

    this->mRotationMatrix = CreateRotationMatrix(this->dbCos,this->dbSin);
    //messageBox(this->mRotationMatrix[0][0]);
    //messageBox(this->mRotationMatrix[0][1]);

    this->mGlobalMatrix = multiplication(multiplication(transpose(mRotationMatrix),mLocalMatrix),mRotationMatrix);
    //messageBox(this->mGlobalMatrix[0][0]);

    for (unsigned int i = 0; i<3;i++){
    this->vecDofNumbers.push_back(MainWindow::nodes[uiNode1].GetDofNumbers()[i]);
    }
    for (unsigned int i = 0; i<3;i++){
    this->vecDofNumbers.push_back(MainWindow::nodes[uiNode2].GetDofNumbers()[i]);
    }

    this->localFixedEndMoments = {{0},{0},{0},{0},{0},{0}};
    this->globalFixedEndMoments = {{0},{0},{0},{0},{0},{0}};

}

void Member::SetMemberNodes(unsigned int uiNode1,unsigned int uiNode2){
    this->vecNodes.clear();
    this->vecNodes.push_back(uiNode1);
    this->vecNodes.push_back(uiNode2);
}

void Member::SetMemberProperties(double dbElasticModulus,double dbHeight,double dbWidth,double dbLength,double dbCos,double dbSin){
    this->dbLength = dbLength;
    this->dbElasticModulus = dbElasticModulus;
    this->dbHeight = dbHeight;
    this->dbWidth = dbWidth;
    this->dbArea = this->dbHeight * this->dbWidth;
    this->dbInertia = pow(this->dbHeight,3) * this->dbWidth / 12;
    this->dbCos = dbCos;
    this->dbSin = dbSin;

    this->mLocalMatrix = CreateLocalMatrix(this->dbElasticModulus,this->dbInertia,this->dbLength,this->dbArea);
    this->mRotationMatrix = CreateRotationMatrix(this->dbCos,this->dbSin);
    this->mGlobalMatrix = multiplication(multiplication(transpose(mRotationMatrix),mLocalMatrix),mRotationMatrix);
}

std::vector<double> Member::GetMemberProperties(){
    std::vector<double> vecProperties;
    vecProperties.push_back(this->dbElasticModulus);
    vecProperties.push_back(this->dbHeight);
    vecProperties.push_back(this->dbWidth);
    return vecProperties;
}

std::vector<double> Member::GetMemberPropertiesForXML(){
    std::vector<double> vecProperties;
    vecProperties.push_back(this->dbElasticModulus);
    vecProperties.push_back(this->dbHeight);
    vecProperties.push_back(this->dbWidth);
    vecProperties.push_back(this->dbLength);
    vecProperties.push_back(this->dbCos);
    vecProperties.push_back(this->dbSin);
    return vecProperties;
}

void Member::SetLoads(double dbLoadNode1,double dbLoadNode2){

    this->dbLoadNode1 = dbLoadNode1 + this->dbLoadNode1;
    this->dbLoadNode2 = dbLoadNode2 + this->dbLoadNode2;
    double difference = abs(this->dbLoadNode2-this->dbLoadNode1);
    double moment = abs(this->dbLoadNode1*pow(this->dbLength,2)/12);
    double moment2 = difference*pow(this->dbLength,2)/20;
    double moment1 = difference*pow(this->dbLength,2)/30;
    double force = abs(this->dbLoadNode1*this->dbLength/2);
    double force2 = difference*this->dbLength/3+moment2/this->dbLength-moment1/this->dbLength;
    double force1 = difference*this->dbLength/3/2-moment2/this->dbLength+moment1/this->dbLength;

    this->localFixedEndMoments = {{0},{force+force1},{moment+moment1},{0},{force+force2},{-(moment+moment2)}};
    this->globalFixedEndMoments = multiplication(matrixInverse(this->GetRotationMatrix()),this->localFixedEndMoments);


}

std::vector<double> Member::GetLoads(){
    std::vector<double> vecLoads{this->dbLoadNode1,this->dbLoadNode2};
    return vecLoads;

}
