#ifndef LOADS_H
#define LOADS_H

#include <vector>

class Loads
{
public:
    Loads();
    Loads(std::vector<double> Load);
    void setLoads(std::vector<double> Load);
    std::vector<double> get(){return this->vecLoads;}

private:
    std::vector<double> vecLoads;
    unsigned int count = 2;
};


class Forces : public Loads{
    friend class Loads;
private:
    unsigned int count = 3;
};

#endif // LOADS_H
