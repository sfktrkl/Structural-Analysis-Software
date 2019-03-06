#include "member.h"
#include "node.h"
#include <math.h>
#include "library.h"
#include "mainwindow.h"
#include "material.h"

#include <QString>

unsigned int Member::numOfMembers = 0;

Member::Member(Node Node1,Node Node2,double dbE,double dbHeight,double dbWidth,double dbLength,double dbCos,double dbSin){
    this->memberNodes.push_back(Node1);
    this->memberNodes.push_back(Node2);
    this->dbLength = dbLength;

    this->memberMaterial.setMaterialProperties(dbE,dbHeight,dbWidth);
    this->dbCos = dbCos;
    this->dbSin = dbSin;

    this->memberId = numOfMembers;
    Member::numOfMembers++;

    this->mLocalMatrix = CreateLocalMatrix(this->memberMaterial.elasticModulus(),this->memberMaterial.inertia(),this->dbLength,this->memberMaterial.area());

    this->mRotationMatrix = CreateRotationMatrix(this->dbCos,this->dbSin);

    this->mGlobalMatrix = multiplication(multiplication(transpose(mRotationMatrix),mLocalMatrix),mRotationMatrix);

    for (unsigned int i = 0; i<3;i++){
    this->vecDofNumbers.push_back(this->memberNodes[0].GetDofNumbers()[i]);
    }
    for (unsigned int i = 0; i<3;i++){
    this->vecDofNumbers.push_back(this->memberNodes[1].GetDofNumbers()[i]);
    }

    this->localFixedEndMoments = {{0},{0},{0},{0},{0},{0}};
    this->globalFixedEndMoments = {{0},{0},{0},{0},{0},{0}};

}

void Member::setNodes(Node node1,Node node2){
    this->memberNodes.push_back(node1);
    this->memberNodes.push_back(node2);
}
// sets member properties to material class
void Member::SetMemberProperties(double dbE,double dbHeight,double dbWidth,double dbLength,double dbCos,double dbSin){

    this->dbLength = dbLength;
    this->dbCos = dbCos;
    this->dbSin = dbSin;
    this->memberMaterial.setMaterialProperties(dbE,dbHeight,dbWidth);
}
// gets member properties from material class
std::vector<double> Member::GetMemberProperties(){
    std::vector<double> vecProperties;
    vecProperties.push_back(this->memberMaterial.elasticModulus());
    vecProperties.push_back(this->memberMaterial.height());
    vecProperties.push_back(this->memberMaterial.width());
    return vecProperties;
}
// getting member properties for creating XML file
std::vector<double> Member::GetMemberPropertiesForXML(){
    std::vector<double> vecProperties;
    vecProperties.push_back(this->memberMaterial.elasticModulus());
    vecProperties.push_back(this->memberMaterial.height());
    vecProperties.push_back(this->memberMaterial.width());
    vecProperties.push_back(this->dbLength);
    vecProperties.push_back(this->dbCos);
    vecProperties.push_back(this->dbSin);
    return vecProperties;
}
// creates Fixed End Moment matrices according to given loads
void Member::SetLoads(double dbLoadNode1,double dbLoadNode2){
    std::vector<double> Load{dbLoadNode1 + this->memberLoads.get()[0],dbLoadNode2 + this->memberLoads.get()[1]};
    this->memberLoads.setLoads(Load);
    double difference = abs(this->memberLoads.get()[1]-this->memberLoads.get()[0]);
    double moment = abs(this->memberLoads.get()[0]*pow(this->dbLength,2)/12);
    double moment2 = difference*pow(this->dbLength,2)/20;
    double moment1 = difference*pow(this->dbLength,2)/30;
    double force = abs(this->memberLoads.get()[0]*this->dbLength/2);
    double force2 = difference*this->dbLength/3+moment2/this->dbLength-moment1/this->dbLength;
    double force1 = difference*this->dbLength/3/2-moment2/this->dbLength+moment1/this->dbLength;

    this->localFixedEndMoments = {{0},{force+force1},{moment+moment1},{0},{force+force2},{-(moment+moment2)}};
    this->globalFixedEndMoments = multiplication(matrixInverse(this->GetRotationMatrix()),this->localFixedEndMoments);

}

std::vector<double> Member::GetLoads(){
    std::vector<double> vecLoads{this->memberLoads.get()[0],this->memberLoads.get()[1]};
    return vecLoads;

}
