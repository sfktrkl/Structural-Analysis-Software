#ifndef MEMBER_H
#define MEMBER_H

#include <vector>
#include "saslibrary_global.h"

class SASLIBRARYSHARED_EXPORT Member
{

    MAKE_TESTABLE(Member);

public:

    Member(unsigned int uiNode1,unsigned int uiNode2,double dbElasticModulus,double dbHeight,double dbWidth,double dbLength,double dbCos,double dbSin);

    static unsigned int numOfMembers;

    void SetMemberNodes(unsigned int uiNode1,unsigned int uiNode2);
    void SetMemberProperties(double dbElasticModulus,double dbHeight,double dbWidth,double dbLength,double dbCos,double dbSin);
    void SetUnitWeight(double dbUnitWeight){this->dbUnitWeight = dbUnitWeight;}
    void SetLoads(double dbLoadNode1,double dbLoadNode2);

    std::vector<std::vector<double>> GetLocalMatrix(){return this->mLocalMatrix;}
    std::vector<std::vector<double>> GetRotationMatrix(){return this->mRotationMatrix;}
    std::vector<std::vector<double>> GetGlobalMatrix(){return this->mGlobalMatrix;}
    std::vector<std::vector<double>> GetLocalFEMMatrix(){return this->localFixedEndMoments;}
    std::vector<std::vector<double>> GetGlobalFEMMatrix(){return this->globalFixedEndMoments;}
    double getLoadNode1(){return dbLoadNode1;}
    double getLoadNode2(){return dbLoadNode2;}
    std::vector<unsigned int> GetDofNumbers(){return vecDofNumbers;}

    std::vector<unsigned int> GetMemberNodes(){return vecNodes;}
    std::vector<double> GetMemberProperties();
    std::vector<double> GetMemberPropertiesForXML();
    double GetArea(){return this->dbArea;}
    double GetUnitWeight(){return this->dbUnitWeight;}
    double GetCos(){return this->dbCos;}
    double GetSin(){return this->dbSin;}
    double GetLenght(){return this->dbLength;}
    std::vector<double> GetLoads();


private:
    std::vector<unsigned int> vecNodes;

    double dbElasticModulus;
    double dbHeight;
    double dbWidth;
    double dbArea;
    double dbInertia;
    double dbLength;
    double dbCos;
    double dbSin;

    unsigned int memberId;

    double dbUnitWeight = 0;
    double dbLoadNode1 = 0;
    double dbLoadNode2 = 0;

    std::vector<std::vector<double>> localFixedEndMoments;
    std::vector<std::vector<double>> globalFixedEndMoments;

    std::vector<std::vector<double>> mLocalMatrix;
    std::vector<std::vector<double>> mRotationMatrix;
    std::vector<std::vector<double>> mGlobalMatrix;

    std::vector<unsigned int> vecDofNumbers;

};

#endif // MEMBER_H
