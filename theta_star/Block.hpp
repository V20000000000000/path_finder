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
    int x;
    int y;
    int z;
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

    int getX() 
    {
        return x;
    }

    int getY() 
    {
        return y;
    }

    int getZ() 
    {
        return z;
    }

    void setWeight(int weight) 
    {
        this->weight = weight;
    }

    void setX(int x) 
    {
        this->x = x;
    }

    void setY(int y) 
    {
        this->y = y;
    }

    void setZ(int z) 
    {
        this->z = z;
    }
};

#endif
