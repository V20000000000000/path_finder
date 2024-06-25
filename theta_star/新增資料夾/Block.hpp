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
    double weight;
public:
    Block(int id) : id(id){}
    Block() : id(-1), x(0), y(0), z(0){}
    Block(double x, double y, double z) : id(id), x(x), y(y), z(z) {}
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

    void setWeight(double weight) 
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
