#include "library.h"

#include <vector>
#include <string>
#include <sstream>

#include <QString>
#include <QMessageBox>

std::vector<double> stringToVector(std::string sData){
    std::vector<double> vecData;
    std::stringstream ss(sData);
    std::string sIndivStr;
    char cSpace = ' ';

    while(getline(ss, sIndivStr, cSpace)){
        vecData.push_back(std::stod(sIndivStr));
    }

    return vecData;

}

std::string vectorToString(std::vector<double> vecData){
    std::string sData;

    for(unsigned int i = 0; i<vecData.size() ;i++){
        sData.append(std::to_string(vecData[i]));
        sData += ' ';
    }

    return sData;

}

std::string boolVectorToString(std::vector<bool> vecData){
    std::string sData;

    for(unsigned int i = 0;vecData.size();i++){
        if (vecData[i] == true){
            sData.append("true ");
        }
        else {
            sData.append("false ");
        }
    }

    return sData;
}
// messageBoxes for simple debug
void messageBox(std::vector<bool> vecSData){
    std::string message = boolVectorToString(vecSData);
    QString qMessage = QString::fromStdString(message);

    QMessageBox msgBox;
    msgBox.setText(qMessage);
    msgBox.exec();
}

void messageBox(std::vector<double> vecSData){
    std::string message = vectorToString(vecSData);
    QString qMessage = QString::fromStdString(message);

    QMessageBox msgBox;
    msgBox.setText(qMessage);
    msgBox.exec();
}
void messageBox(std::string sData){
    QString qMessage = QString::fromStdString(sData);

    QMessageBox msgBox;
    msgBox.setText(qMessage);
    msgBox.exec();
}
void messageBox(double dbData){
    QString qMessage = QString::number(dbData);

    QMessageBox msgBox;
    msgBox.setText(qMessage);
    msgBox.exec();
}


