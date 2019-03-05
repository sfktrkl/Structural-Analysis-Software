#include "material.h"
#include "math.h"

unsigned int Material::numOfMaterials = 0;

Material::Material()
{
    this->dbElasticModulus = 1;
    this->dbHeight = 1;
    this->dbWidth = 1;
    this->dbArea = this->dbHeight * this->dbWidth;
    this->dbInertia = pow(this->dbHeight,3) * this->dbWidth / 12;

    this->materialId = numOfMaterials;
    Material::numOfMaterials++;

}

Material::Material(double dbElasticModulus,double dbHeight,double dbWidth)
{
    this->dbElasticModulus = dbElasticModulus;
    this->dbHeight = dbHeight;
    this->dbWidth = dbWidth;
    this->dbArea = this->dbHeight * this->dbWidth;
    this->dbInertia = pow(this->dbHeight,3) * this->dbWidth / 12;

    this->materialId = numOfMaterials;
    Material::numOfMaterials++;

}

void Material::setMaterialProperties(double dbElasticModulus,double dbHeight,double dbWidth){
    this->dbElasticModulus = dbElasticModulus;
    this->dbHeight = dbHeight;
    this->dbWidth = dbWidth;
    this->dbArea = this->dbHeight * this->dbWidth;
    this->dbInertia = pow(this->dbHeight,3) * this->dbWidth / 12;

    this->materialId = numOfMaterials;
    Material::numOfMaterials++;
}

