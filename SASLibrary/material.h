#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include "saslibrary_global.h"

class SASLIBRARYSHARED_EXPORT Material
{
    MAKE_TESTABLE(Material);

public:
    Material();
    Material(double dbElasticModulus,double dbHeight,double dbWidth);
    static unsigned int numOfMaterials;

    void setMaterialProperties(double dbElasticModulus,double dbHeight,double dbWidth);
    void setUnitWeight(double dbUnitWeight){this->dbUnitWeight = dbUnitWeight;}
    double height(){return dbHeight;}
    double elasticModulus(){return dbElasticModulus;}
    double width(){return dbWidth;}
    double inertia(){return dbInertia;}
    double area(){return dbArea;}
    double unitWeight(){return dbUnitWeight;}
    unsigned int materialID(){return materialId;}

private:
    double dbUnitWeight = 0;
    double dbElasticModulus;
    double dbHeight;
    double dbWidth;
    double dbArea;
    double dbInertia;
    unsigned int materialId;

};

#endif // MATERIAL_H
