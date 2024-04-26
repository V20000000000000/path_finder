#ifndef Block_HPP
#define Block_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Block 
{
private:
    int id;
    double x;
    double y;
    double z;
    int weight;
public:
    Block(int id, int width, int height) : id(id){}
    Block(){}
    int getId() 
    {
        return id;
    }

    int getWeight() 
    {
        return weight;
    }

    double getX() 
    {
        return x;
    }

    double getY() 
    {
        return y;
    }

    double getZ() 
    {
        return z;
    }

    void setWeight(int weight) 
    {
        this->weight = weight;
    }

    void setX(double x) 
    {
        this->x = x;
    }

    void setY(double y) 
    {
        this->y = y;
    }

    void setZ(double z) 
    {
        this->z = z;
    }
};

#endif
