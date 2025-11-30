#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "Renderer/Texture.h"
#include "Core/ResourceManager.h"
// Map mesh class




//RENAME THIS SO THAT IT DOESNT CAUSE PROBLEMS WITH LOADING OTHER MESHES, SINCE THEY'RE HANDLED DIFFERENTLY FROM ONE ANOTHER
namespace Core
{
#if OLD
    class Vertex {
    public:
        Vertex(glm::vec3 Pos, glm::vec3 Norm = glm::vec3(0.0f)){
            Position = Pos;
            Normal = Norm;
        }

        float TexU;
        float TexV;
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords = glm::vec2(TexU, TexV);
        glm::vec3 Tangent;
        glm::vec3 Bitangent;


    };

#endif

    struct Vertex{
         glm::vec3 Position;
         glm::vec3 Normal;
         glm::vec2 TexCoords;
    };

    class MapMesh
    {
    public:

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices = {0, 1, 2};      //Each mesh is just one line of the brush, made of 3 vertices. So each one will always have the same indices.

        //Variables for each "Mesh" which makes up a single line of a Brush object
        Renderer::Texture2D texture;
        unsigned int VAO;


        //Initializes all of the vertex locations, and handles conversion to create the UV coordinates (TexCoords)
        MapMesh(glm::vec3 vert1In, glm::vec3 vert2In, glm::vec3 vert3In, std::string texNameIn, glm::vec4 uValsIn, glm::vec4 vValsIn, float rotationIn, float uScaleIn, float vScaleIn)
        {
            float gridUnit = 64.0f;

            Vertex v1; v1.Position = vert1In / gridUnit;
            Vertex v2; v2.Position = vert1In / gridUnit;
            Vertex v3; v3.Position = vert1In / gridUnit;


            //Vertex v1 = Vertex(vert1In /= gridUnit);
           // Vertex v2 = Vertex(vert2In /= gridUnit);
           // Vertex v3 = Vertex(vert3In /= gridUnit);
            //All three vertices that are declared for this mesh
            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            //Temporarily sets texNameIn to be "wall"
            std::string texName = "wall";


            //Grabs the already loaded texture from the resource ResourceManager
            //Core::ResourceManager::GetTexture("wall");
            texture = Core::ResourceManager::LoadTexture("Resources/Textures/wall.jpg",false,"wall");

            //UV coordinate values are stored in these. XYZ and W being the corresponding offset value.

            glm::vec3 axisU = glm::vec3(uValsIn.x, uValsIn.y, uValsIn.z) / uScaleIn;
            glm::vec3 axisV = glm::vec3(vValsIn.x, vValsIn.y, vValsIn.z) / vScaleIn;

            //UV handling. Sets up the Tex Coords for each vertex in vertices.
            for(Vertex& v : vertices)
            {
                float TexU;
                float TexV;
                TexU = v.Position.x * axisU.x + v.Position.y * axisU.y + v.Position.z * axisU.z;
                TexV = v.Position.x * axisV.x + v.Position.y * axisV.y + v.Position.z * axisV.z;
                TexU += uValsIn.w;
                TexV += vValsIn.w;
                TexU /= texture.Width;
                TexV /= texture.Height;
                v.TexCoords = glm::vec2(TexU, TexV);
            }
            //printf("\nDebug: %f,%f,%f", v1.Position.x, v1.Position.y, v1.Position.z);

            // VALVE 220 Format no longer uses Rotation. So it is currently left out, and commented out.
            //rotation = rotationIn;

            setupMesh();
        }



        float rotation;


        //Initialization, setting up the mesh using the given data within OpenGL requirements.

        void Draw(Renderer::Shader &shader)
        {
            printf("v1: %f\n", vertices[0].Position.x);
            glActiveTexture(GL_TEXTURE0);
            shader.setInteger("texture1",0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

    private:
        unsigned int VBO, EBO;

        void setupMesh()
        {

            //Handle texture generation

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            // vertex positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
            // vertex texture coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

            glBindVertexArray(0);

#if OTHER
            //Vertex Tangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Tangent));
            //Vertex Biangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Bitangent));
#endif


        }
    };



}
