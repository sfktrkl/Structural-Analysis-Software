#ifndef ESSENTIALS_H
#define ESSENTIALS_H

#include <vector>
#include <string>

class essentials
{
public:
    std::vector<double> stringToVector(std::string sData);
    std::string vectorToString(std::vector<double> vecData);
    std::string boolVectorToString(std::vector<bool> vecData);
    void messageBox(std::vector<bool> vecSData);
    void messageBox(std::vector<double> vecSData);
    void messageBox(std::string sData);
    void messageBox(double dbData);
};

#endif // ESSENTIALS_H
