#include "loads.h"

Loads::Loads(){
    for (unsigned int i = 0 ; i<count;i++){
        this->vecLoads.push_back(0);
    }
}
Loads::Loads(std::vector<double> Load){
    for (unsigned int i = 0 ; i<count;i++){
        this->vecLoads.push_back(Load[i]);
    }
}
// set loads according to node forces (3 forces) or distributed loads (for members, 2 load magnitude for end nodes)
void Loads::setLoads(std::vector<double> Load){
    for (unsigned int i = 0 ; i<count;i++){
        this->vecLoads[i] = Load[i];
    }
}
