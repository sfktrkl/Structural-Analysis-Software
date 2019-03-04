#ifndef NODE_H
#define NODE_H

#include <vector>
#include "saslibrary_global.h"

class SASLIBRARYSHARED_EXPORT Node
{

    MAKE_TESTABLE(Node);
    friend class TestCase;
public:

    Node(bool bXFixity, bool bYFixity, bool bZFixity,double dbXStiffness,double dbYStiffness,double dbZStiffness, double dbXCoordinate, double dbYCoordinate, double dbXForce, double dbYForce, double dbZForce);

    void SetNodeCoordinates(double dbXCoordinate, double dbYCoordinate);
    void SetNodeFixity(bool bXFixity, bool bYFixity, bool bZFixity);
    void SetNodeForces(double dbXForce, double dbYForce, double dbZForce);

    std::vector<bool> GetNodeFixity(){return this->bFixity;}
    std::vector<double> GetNodeCoordinates(){return this->dbCoordinates;}
    std::vector<double> GetNodeForces(){return this->dbForce;}
    std::vector<unsigned int> GetDofNumbers(){return this->vecDofNumbers;}
    std::vector<double> GetStiffness(){return this->vecStiffness;}
    double GetXCoordinate(){return this->dbCoordinates[0];}
    double GetYCoordinate(){return this->dbCoordinates[1];}
    std::vector<double> GetNodePropertiesForXML();
    std::vector<bool> GetNodeFixitiesForXML();

    static unsigned int numOfNodes;
    static unsigned int numOfDofs;

private:

    std::vector<bool> bFixity;
    std::vector<double> vecStiffness;
    std::vector<double> dbCoordinates;
    std::vector<double> dbForce{0,0,0};

    std::vector<unsigned int> vecDofNumbers;

    unsigned int nodeId;

};



#endif // NODE_H
