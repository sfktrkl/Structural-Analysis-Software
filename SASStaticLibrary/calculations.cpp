#include "library.h"
#include <QtMath>
#include <vector>

std::vector<std::vector<double>> CreateLocalMatrix(double dbElasticModulus,double dbInertia,double dbLength,double dbArea){
    std::vector<std::vector<double>> LocalMatrix{
        {dbElasticModulus*dbArea/dbLength,0,0,-dbElasticModulus*dbArea/dbLength,0,0},
        {0,12*dbElasticModulus*dbInertia/pow(dbLength,3),6*dbElasticModulus*dbInertia/pow(dbLength,2),0,-12*dbElasticModulus*dbInertia/pow(dbLength,3),6*dbElasticModulus*dbInertia/pow(dbLength,2)},
        {0,6*dbElasticModulus*dbInertia/pow(dbLength,2),4*dbElasticModulus*dbInertia/pow(dbLength,1),0,-6*dbElasticModulus*dbInertia/pow(dbLength,2),2*dbElasticModulus*dbInertia/pow(dbLength,1)},
        {-dbElasticModulus*dbArea/dbLength,0,0,dbElasticModulus*dbArea/dbLength,0,0},
        {0,-12*dbElasticModulus*dbInertia/pow(dbLength,3),-6*dbElasticModulus*dbInertia/pow(dbLength,2),0,12*dbElasticModulus*dbInertia/pow(dbLength,3),-6*dbElasticModulus*dbInertia/pow(dbLength,2)},
        {0,6*dbElasticModulus*dbInertia/pow(dbLength,2),2*dbElasticModulus*dbInertia/pow(dbLength,1),0,-6*dbElasticModulus*dbInertia/pow(dbLength,2),4*dbElasticModulus*dbInertia/pow(dbLength,1)},
    };
    return LocalMatrix;
}

std::vector<std::vector<double>> CreateRotationMatrix(double dbCos,double dbSin){
    std::vector<std::vector<double>> RotationMatrix{
        {dbCos,dbSin,0,0,0,0},
        {-dbSin,dbCos,0,0,0,0},
        {0,0,1,0,0,0},
        {0,0,0,dbCos,dbSin,0},
        {0,0,0,-dbSin,dbCos,0},
        {0,0,0,0,0,1},
    };
    return RotationMatrix;
}