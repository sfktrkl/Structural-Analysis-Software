#include "node.h"
#include <vector>

unsigned int Node::numOfNodes = 0;
unsigned int Node::numOfDofs = 1;

Node::Node(bool bXFixity, bool bYFixity, bool bZFixity,double dbXStiffness,double dbYStiffness,double dbZStiffness, double dbXCoordinate, double dbYCoordinate, double dbXForce, double dbYForce, double dbZForce){

    this->bFixity.push_back(bXFixity);
    this->vecStiffness.push_back(dbXStiffness);
    if (bXFixity == false){
        this->vecDofNumbers.push_back(numOfDofs);
        numOfDofs++;
    }
    else {
        this->vecDofNumbers.push_back(0);
    }
    //
    this->bFixity.push_back(bYFixity);
    this->vecStiffness.push_back(dbYStiffness);
    if (bYFixity == false){
        this->vecDofNumbers.push_back(numOfDofs);
        numOfDofs++;
    }
    else {
        this->vecDofNumbers.push_back(0);
    }
    //
    this->bFixity.push_back(bZFixity);
    this->vecStiffness.push_back(dbZStiffness);
    if (bZFixity == false){
        this->vecDofNumbers.push_back(numOfDofs);
        numOfDofs++;
    }
    else {
        this->vecDofNumbers.push_back(0);
    }

    this->dbCoordinates.push_back(dbXCoordinate);
    this->dbCoordinates.push_back(dbYCoordinate);

    this->dbForce[0] = this->dbForce[0] + dbXForce;
    this->dbForce[1] = this->dbForce[1] + dbYForce;
    this->dbForce[2] = this->dbForce[2] + dbZForce;

    this->nodeId=numOfNodes;
    Node::numOfNodes++;
}


void Node::SetNodeCoordinates(double dbXCoordinate, double dbYCoordinate){
    this->dbCoordinates.clear();
    this->dbCoordinates.push_back(dbXCoordinate);
    this->dbCoordinates.push_back(dbYCoordinate);
}

void Node::SetNodeFixity(bool bXFixity, bool bYFixity, bool bZFixity){
    this->bFixity.clear();
    this->vecDofNumbers.clear();

    this->bFixity.push_back(bXFixity);
    if (bXFixity == false){
        this->vecDofNumbers.push_back(numOfDofs);
        numOfDofs++;
    }
    else {
        this->vecDofNumbers.push_back(0);
    }
    //
    this->bFixity.push_back(bYFixity);
    if (bYFixity == false){
        this->vecDofNumbers.push_back(numOfDofs);
        numOfDofs++;
    }
    else {
        this->vecDofNumbers.push_back(0);
    }
    //
    this->bFixity.push_back(bZFixity);
    if (bZFixity == false){
        this->vecDofNumbers.push_back(numOfDofs);
        numOfDofs++;
    }
    else {
        this->vecDofNumbers.push_back(0);
    }
}

void Node::SetNodeForces(double dbXForce, double dbYForce, double dbZForce){
    this->dbForce[0] = this->dbForce[0] + dbXForce;
    this->dbForce[1] = this->dbForce[1] + dbYForce;
    this->dbForce[2] = this->dbForce[2] + dbZForce;
}

std::vector<double> Node::GetNodePropertiesForXML(){
    std::vector<double> vecProperties;
    vecProperties.push_back(this->vecStiffness[0]);
    vecProperties.push_back(this->vecStiffness[1]);
    vecProperties.push_back(this->vecStiffness[2]);
    vecProperties.push_back(this->dbCoordinates[0]);
    vecProperties.push_back(this->dbCoordinates[1]);
    vecProperties.push_back(this->dbForce[0]);
    vecProperties.push_back(this->dbForce[1]);
    vecProperties.push_back(this->dbForce[2]);
    return vecProperties;
}

std::vector<bool> Node::GetNodeFixitiesForXML(){
    std::vector<bool> vecProperties;
    vecProperties.push_back(this->bFixity[0]);
    vecProperties.push_back(this->bFixity[1]);
    vecProperties.push_back(this->bFixity[2]);
    return vecProperties;
}
