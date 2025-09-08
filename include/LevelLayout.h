#ifndef LLOADER_H
#define LLOADER_H



#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "resource_manager.h"
#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include "Camera.h"
//#include "Common.h"
#include "Shader.h"
class wall {
public:

    int id;
    int sector;
    int portal; //Determines which sector the wall connects to. If set to 0, it displays as a normal wall.
    int numVert;
    int numTri;
    int numFace;
    float attribData[180];
};

class sector {
public:
    int id;
    int firstWall, nWalls;
    float zFloor, zCeil;
};

class level {
public:
    //This will input the requisite shader data, specify textures, and then render each "sector", going
    //wall-by-wall until the sector is built, before continuing on to the next sector in the list.
    void SetVars(Shader &shader);

    void Draw(Shader &shader, Camera &camera);
    //Loads the level using the specified txt file at the path designated.
    static int load_level(const char *path);

    //public Contructor
    level() {}
private:




};

#endif
