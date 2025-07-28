#ifndef MESH_H
#define MESH_H

#include "include/glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"

#include <string>
#include <vector>

using namespace std;

#define MAX_BONE_INFLUENCE 4


//TODO: Integrate the Texture.cpp/h files, and implement the necessary variables to get it working with the pos/normal/coords/tan/bitan



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

struct Texture{
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
    vector<Texture>         textures;

    unsigned int VAO;

    //constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        //storing input data into the mesh we're constructing
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        //Then we can set up the vertex buffers and attribute pointers
        setupMesh();
    }

    //Render the Mesh
    void    Draw(Shader &shader);
private:
    // rendering data
    unsigned int VBO, EBO;

    //Initializes all the buffer objets/arrays
    void setupMesh();

};
#endif
