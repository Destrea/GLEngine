#pragma once

#include "Renderer.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

using namespace std;

#define MAX_BONE_INFLUENCE 4


//TODO: Integrate the Texture.cpp/h files, and implement the necessary variables to get it working with the pos/normal/coords/tan/bitan

namespace Renderer
{

    struct Vertex {
        //Position
        glm::vec3 Position;
        //Normals
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        //Tangent
        glm::vec3 Tangent;
        //Bitangeng
        glm::vec3 Bitangent;
        //Bone indices which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct MeshTexture{
        unsigned int id;
        string type;
        string path;
    };


    class Mesh
    {
    public:
        //Mesh Data
        vector<Vertex>          vertices;
        vector<unsigned int>    indices;
        vector<MeshTexture>         textures;

        unsigned int VAO;

        //constructor
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<MeshTexture> textures)
        {
            //storing input data into the mesh we're constructing
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            //Then we can set up the vertex buffers and attribute pointers
            setupMesh();
        }

        //Render the Mesh
        void Draw(Shader &shader)
        {
            //bind appropriate textures
            unsigned int diffuseNr      = 1;
            unsigned int specularNr     = 1;
            unsigned int normalNr       = 1;
            unsigned int heightNr       = 1;

            for(unsigned int i = 0; i < textures.size(); i++)
            {
                //Activate the correct texture before binding attributes
                glActiveTexture(GL_TEXTURE0 + i);


                //retrieve texture number (the N in diffuse_textureN)
                string number;
                string name = textures[i].type;
                if(name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if(name == "texture_specular")
                    number = std::to_string(specularNr++);
                else if(name == "texture_normal")
                    number = std::to_string(normalNr++);
                else if(name == "texture_height")
                    number = std::to_string(heightNr++);

                //Now we'll set the sampler to the correct texture unit
                glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
                //and then bind the texture
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
            //Drawing the mesh
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            //Then we'll set everything back to defaults once configured
            glActiveTexture(GL_TEXTURE0);
        }
    private:
        // rendering data
        unsigned int VBO, EBO;

        //Initializes all the buffer objets/arrays
        void setupMesh()
        {
            //Create the buffers and arrays
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            //Loading data into the vertex buffers
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            //Setting vertex attribute pointers
            //vertex positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            //vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
            //vertex texCoords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

            //vertex tangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

            //vertex bitangent
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

            // ids
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
            // weights
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
            glBindVertexArray(0);
        }

    };
}
