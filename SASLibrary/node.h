#ifndef NODE_H
#define NODE_H

#include <vector>
#include "saslibrary_global.h"
#include "loads.h"

class SASLIBRARYSHARED_EXPORT Node
{

    MAKE_TESTABLE(Node);

public:

    Node(bool bXFixity, bool bYFixity, bool bZFixity,double dbXStiffness,double dbYStiffness,double dbZStiffness, double dbXCoordinate, double dbYCoordinate, double dbXForce, double dbYForce, double dbZForce);

    void SetNodeCoordinates(double dbXCoordinate, double dbYCoordinate);
    void SetNodeFixity(bool bXFixity, bool bYFixity, bool bZFixity);
    void SetNodeForces(double dbXForce, double dbYForce, double dbZForce);
    void setOffsetOpenGL(double dbOffset){this->dbOffset = dbOffset;}
    std::vector<bool> GetNodeFixity(){return this->bFixity;}
    std::vector<double> GetNodeCoordinates(){return this->dbCoordinates;}
    std::vector<unsigned int> GetDofNumbers(){return this->vecDofNumbers;}
    std::vector<double> GetStiffness(){return this->vecStiffness;}
    double GetXCoordinate(){return this->dbCoordinates[0];}
    double GetYCoordinate(){return this->dbCoordinates[1];}
    double GetOffset(){return this->dbOffset;}
    std::vector<double> GetNodePropertiesForXML();
    std::vector<bool> GetNodeFixitiesForXML();
    unsigned int getNodeId(){return nodeId;}

    std::vector<double> supportReactions;

    static unsigned int numOfNodes;
    static unsigned int numOfDofs;

    Forces nodeForces;

private:

    std::vector<bool> bFixity;
    std::vector<double> vecStiffness;
    std::vector<double> dbCoordinates;

    double dbOffset;

    std::vector<unsigned int> vecDofNumbers;

    unsigned int nodeId;

};



#endif // NODE_H
