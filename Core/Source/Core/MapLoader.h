#pragma once

#include "MapMesh.h"
#include "Core/ResourceManager.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"

#include <vector>
#include <string>
#include <memory>
#include <stdio.h>
// .Map file loading

//Map files use the Quake2 (Valve 220) format
//These maps can be created using Trenchbroom

namespace Core
{
    class Brush
    {
    public:

        Brush(int id)
        {
            ID = id;
        }

        unsigned int ID;
        unsigned int size;
        std::vector<MapMesh> meshes;

        void AddMesh(MapMesh newMesh)
        {
            meshes.push_back(newMesh);
        }
    private:
        //Each brush is made of a set of meshes.
        //Each "Line" of the brush in the file is one mesh




    };

    //contains a list of each individual "Brush" object
    //Each brush object contains a vector of it's mesh components
    class MapScene
    {
    public:

        MapScene(const char* path, std::string name)
        {
            //Sets the map's name to what is specified, and calls LoadMap to parse the .map file's text
            Name = name;
            LoadMap(path);
        }


        std::string ID;
        std::string Name;
        std::string MapVersion;
        std::string ClassName;
        std::vector<Brush> brushes;

        void DrawMap(Renderer::Shader &shader)
        {
            //printf("Draw1\n");
            for(Brush& b : brushes)
            {
                //printf("++Draw2\n");
                for(MapMesh& m : b.meshes)
                {
                    //printf("------Draw3\n");
                    m.Draw(shader);
                }
            }
        }

    private:


        //For later :)
        //std::vector<Entity> entities;


        //Parses the data of the given file at the path.
        void LoadMap(const char* path)
        {
            FILE *f = fopen(path, "r"); //File descriptor
            if(!f)
            {
                printf("ERROR:: Map file not loaded properly!\n");
                //return -1;
            }
            int lineCounter = 1;
            int depth = 0;
            int brushCounter = 0;
            char line[1024], buff[64];
            while(fgets(line, sizeof(line), f))
            {


                const char *p = line;

                //Skip Spaces and tabs
                while (isspace(*p))
                {
                    p++;
                }
                if(!*p || (*p == '/'))
                {
                    //Skip lines for comments
                    continue;
                }
                else if(!*p || (*p == '"'))
                {
                    //This is going to be where we handle the different entity key/value pairs
                    continue;
                }
                else if(*p == '{')
                {
                    //Increase the "depth".
                    depth++;
                    if(depth == 2)
                    {
                        Brush newBrush(brushCounter);
                        brushes.push_back(newBrush);

                    }
                    continue;
                }
                else if(*p == '(')
                {

                    //If we run into an open parenthesis, then we read the whole line into a mesh
                    if(!*p  || *p == '#')
                    {
                        fgets(line, sizeof(line), f);
                    }
                    glm::vec3 v1, v2, v3;
                    float   v1x, v1y, v1z,
                    v2x, v2y, v2z,
                    v3x, v3y, v3z;

                    char texture[12];
                    float uValsx, uValsy, uValsz, uValsw;
                    float vValsx, vValsy, vValsz, vValsw;

                    std::string texName;
                    glm::vec4 uVals, vVals;
                    int rotation, uScale, vScale;
                    if(sscanf(line,
                        "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %s [ %f %f %f %f ] [ %f %f %f %f ] %d %d %d",
                              &v1x, &v1y, &v1z, &v2x, &v2y, &v2z, &v3x, &v3y, &v3z, texture, &uValsx, &uValsy, &uValsz, &uValsw, &vValsx, &vValsy, &vValsz, &vValsw, &rotation, &uScale, &vScale) < 21)
                    {
                        printf("\n------------\nERROR:: Mesh input contains less values than expected!\n");
                        //printf("%s", line);
                        //printf("( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %s [ %f %f %f %f ] [ %f %f %f %f ] %d %d %d",
                               //v1x, v1y, v1z, v2x, v2y, v2z, v3x, v3y, v3z, texture, uValsx, uValsy, uValsz, uValsw, vValsx, vValsy, vValsz, vValsw, rotation, uScale, vScale);
                    }
                    else
                    {


                        //printf("\nSuccess!");
                        //printf("%s\n", line);
                        //setup the mesh with the discovered data, and add it to our current brush's vector of meshes
                        texName = texture;
                        v1 = glm::vec3(v1x,v1y,v1z);
                        v2 = glm::vec3(v2x,v2y,v2z);
                        v3 = glm::vec3(v3x,v3y,v3z);
                        uVals = glm::vec4(uValsx, uValsy, uValsz, uValsw);
                        vVals = glm::vec4(vValsx, vValsy, vValsz, vValsw);
                        MapMesh curLine(v1, v2, v3, texName, uVals, vVals, rotation, uScale, vScale);
                        //printf("\n brushCounter: %i\n", brushCounter);
                        //printf("Current Brush ID: %i\n", this->brushes[brushCounter].ID);
                        brushes[brushCounter].AddMesh(curLine);

                    }

                }
                else if(*p == '}' && depth > 1)
                {
                    //Once we reach the end of a brush's section, we increment, to indicate that we should create a new brush
                    //And decrement the depth counter, to indicate that we're back in the main "entity" section of the mapping file.
                    brushCounter++;
                    depth--;
                }

                lineCounter++;

            }

            printf("Map loading finished!\n");

        }
    };


}

