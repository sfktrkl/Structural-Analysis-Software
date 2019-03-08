#ifndef MEMBER_H
#define MEMBER_H

#include <vector>
#include "saslibrary_global.h"
#include "material.h"
#include "loads.h"
#include "node.h"

class SASLIBRARYSHARED_EXPORT Member
{

    MAKE_TESTABLE(Member);

public:

    Member(Node Node1,Node Node2,double dbE,double dbHeight,double dbWidth,double dbLength,double dbCos,double dbSin);
    static unsigned int numOfMembers;
    void SetLoads(double dbLoadNode1,double dbLoadNode2);
    void SetMemberProperties(double dbE,double dbHeight,double dbWidth,double dbLength,double dbCos,double dbSin);
    void SetMemberLocalForces(std::vector<std::vector<double>> mLocalForces){this->memberLocalForces = mLocalForces;}
    void SetMemberGlobalForces(std::vector<std::vector<double>> mGlobalForces){this->memberGlobalForces = mGlobalForces;}
    void SetMemberInternalForces(std::vector<std::vector<double>> mInternalForces){this->memberInternalForces = mInternalForces;}

    std::vector<std::vector<double>> GetLocalMatrix(){return this->mLocalMatrix;}
    std::vector<std::vector<double>> GetRotationMatrix(){return this->mRotationMatrix;}
    std::vector<std::vector<double>> GetGlobalMatrix(){return this->mGlobalMatrix;}
    std::vector<std::vector<double>> GetLocalFEMMatrix(){return this->localFixedEndMoments;}
    std::vector<std::vector<double>> GetGlobalFEMMatrix(){return this->globalFixedEndMoments;}

    std::vector<unsigned int> GetDofNumbers(){return vecDofNumbers;}

    std::vector<double> GetMemberProperties();
    std::vector<double> GetMemberPropertiesForXML();
    double GetCos(){return this->dbCos;}
    double GetSin(){return this->dbSin;}
    double GetLenght(){return this->dbLength;}

    std::vector<std::vector<double>> memberLocalForces;
    std::vector<std::vector<double>> memberGlobalForces;
    std::vector<std::vector<double>> memberInternalForces;

    std::vector<double> GetLoads();

    std::vector<Node> memberNodes;
    void setNodes(Node node1,Node node2);

    Material memberMaterial;

    Loads memberLoads;

private:
    double dbLength;
    double dbCos;
    double dbSin;

    unsigned int memberId;

    std::vector<std::vector<double>> localFixedEndMoments;
    std::vector<std::vector<double>> globalFixedEndMoments;

    std::vector<std::vector<double>> mLocalMatrix;
    std::vector<std::vector<double>> mRotationMatrix;
    std::vector<std::vector<double>> mGlobalMatrix;



    std::vector<unsigned int> vecDofNumbers;

};

#endif // MEMBER_H
