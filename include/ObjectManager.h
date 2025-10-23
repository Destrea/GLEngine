//Object Manager and Entity Component System

#ifndef OBJMGR
#define OBJMGR

#include "Common.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cstdint>

using namespace glm;

//CRC-32B hashing Function
uint32_t crc32(std::string input, uint32_t polynomial = 0xEDB88320)
{
    const char *data = input.c_str();
    size_t len = input.length();
    uint32_t crc = -1;
    for(size_t ii = 0; ii < len; ++ii)
    {
        //
        crc ^= uint32_t(data[ii]);
        for(size_t jj = 0; jj < 8; ++jj)
        {
            uint32_t mask = (crc & 1) ? ~(crc & 1) + 1 : 0;
            crc = (crc >> 1) ^ (polynomial & mask);
        }
    }

    //Returning the bitwise complement of the crc value
    crc = ~crc;
    return crc;
}



//Entity Component System Functions

//Base Component class
struct Component
{
    int ID;

};

//Base Entity class
class Entity
{
    mat4 Transform;
    std::string Tag;

    std::vector<Component*> Components;

};

//Mesh/Model
struct Mesh : public Component
{
    //Separate Transform?

    //Material/shader

};
//Camera

//RigidBody

//etc



//Object Node
class Object
{
public:
    std::string name;
    Entity data;
    //CRC-32B hashed object ID identifier
    uint32_t ID;
    std::vector<Object*> children;

    Object(std::string name, Entity data)
    {
        this->name = name;
        this->data = data;
        this->ID = crc32(name);
    }

};

//Object Manager tree
    //Objects are just stored as "hashes", to determine parent/child relationships.
    //The data is stored separately in a hash table, for effective lookup using the hashed value.



#endif
