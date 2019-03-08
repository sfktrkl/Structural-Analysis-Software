#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>

#include <QMainWindow>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMessageBox>
#include <QString>

// essentials.cpp
std::vector<double> stringToVector(std::string sData);
std::string vectorToString(std::vector<double> vecData);
std::string boolVectorToString(std::vector<bool> vecData);
void messageBox(std::vector<bool> vecSData);
void messageBox(std::vector<double> vecSData);
void messageBox(std::string sData);
void messageBox(double dbData);

// mat.cpp
std::vector<std::vector<double>> transpose(std::vector<std::vector<double>> matrix);
std::vector<std::vector<double>> multiplication(std::vector<std::vector<double>> matrix1,std::vector<std::vector<double>> matrix2);
std::vector<std::vector<double>> matrixInverse(std::vector<std::vector<double>> inputMatrix);
std::vector<std::vector<double>> gaussJordan(std::vector<std::vector<double>> KMatrix,std::vector<std::vector<double>> FMatrix);

// calculations.cpp
std::vector<std::vector<double>> CreateLocalMatrix(double dbElasticModulus,double dbInertia,double dbLength,double dbArea);
std::vector<std::vector<double>> CreateRotationMatrix(double dbCos,double dbSin);
double avoidPrecision(double given);

#endif // LIBRARY_H
