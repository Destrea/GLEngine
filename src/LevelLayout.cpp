//Level Layout

#include "../include/LevelLayout.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

static std::map<int, wall> Walls;


unsigned int VBO, VAO, EBO;
unsigned int tex1;
wall curWall = Walls[0];
unsigned int attSize = 0;
void level::SetVars(Shader &shader)
{
    int len = Walls.size();

    for(int i = 0; i <= len; i++)
    {

        wall curWall = Walls[0];

        glGenTextures(1, &tex1);
        glBindTexture(GL_TEXTURE_2D, tex1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
        unsigned char *data = stbi_load("resources/Textures/Debugempty.png", &width, &height, &nrChannels, 0);
        if(data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);

        shader.setInteger("ourTexture", 0);


        //Texture2D missingTex = ResourceManager::LoadTexture("src/Textures/Debugempty.png", true, "debug");


        //Divvy our the data for the Vertices
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);


        //VBO set up,  for the vertices
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, attSize * sizeof(float), curWall.attribData, GL_STATIC_DRAW);


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        shader.Use();


    }
}


void level::Draw(Shader &shader, Camera &camera)
{

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    shader.setMatrix4("projection",projection);
    shader.setMatrix4("view",view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    shader.setMatrix4("model",model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1);

    shader.Use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, attSize);

}

void printData()
{
    for(int id = 0; id < Walls.size(); id++)
    {
        //std::cout << "attribData print: ";
        for(int i = 0; i < 180; i++)
        {
            std::cout << " " << Walls[0].attribData[i] << " ";
        }
    }
}




int level::load_level(const char *path)
{
    FILE *f = fopen(path, "r");
    if(!f)
    {
        //std::cout << "ERROR::level File not loaded!" << std::endl;
        return -1;
    }

    int retval = 0;
    enum {
        SCAN_SECTOR,
        SCAN_WALL,
        NONE
    } loadState;


    wall curWall;

    loadState = SCAN_SECTOR;
    char line[1024], buff[64];
    while(fgets(line, sizeof(line), f))
    {
        //std::cout << "Repeating..." << std::endl;
        const char *p = line;   //Pointer to our current line location
        while (isspace(*p)){     //Ignore whitespace
            p++;
        }

        if(!*p || *p == '#')
        {
            continue;
        }
        else if(*p == '[')
        {

            strncpy(buff, p+1, sizeof(buff));
            char *section = strtok(buff, "]");
            if(!section)
            {
                retval = -2; break;
            }
            if(!strcmp(section, "SECTOR"))
            {
                loadState = SCAN_SECTOR;
            }
            else if(!strcmp(section, "WALL"))
            {
                loadState = SCAN_WALL;
            }
        }
        else
        {
            switch(loadState)
            {

                case SCAN_WALL: {


                    //storage
                    wall curWall;

                    if(!*p  || *p == '#')
                    {
                        fgets(line, sizeof(line), f);
                    }
                    //Info line
                    if(sscanf(
                        line,
                        "%d %d %d %d %d %d",
                        &curWall.id, &curWall.sector, &curWall.portal, &curWall.numVert, &curWall.numTri, &curWall.numFace)
                        < 6)
                    {
                        retval = -4; break;
                    }

                    //repeat until we get num vertices
                    int totalVert = curWall.numVert * curWall.numTri * curWall.numFace;
                    for(int i = 0; i < totalVert; i++)
                    {
                        fgets(line, sizeof(line), f);

                        //Ignores whitespace
                        if(line[0] == '#' || isspace(line[0]))
                        {
                            fgets(line, sizeof(line), f);
                        }

                        const char *l = line;

                        //Stores the vertex and texCoord attributes into the attribData value of the current "wall" object, for use by openGL to draw all the vertices, and to pass the texCoords to the fragment shader.
                        sscanf(l, "%f %f %f %f %f", &curWall.attribData[(i * 5)+0], &curWall.attribData[(i * 5)+1], &curWall.attribData[(i * 5)+2], &curWall.attribData[(i * 5) + 3], &curWall.attribData[(i * 5)+4]);
                        attSize += 5;
                    }

                    Walls[curWall.id] = curWall;
                    //printData();
                }; break;

                case SCAN_SECTOR: {
                    //Add the wall components to a sector here:
                    std::cout << "You shouldn't see this" << std::endl;
                }

            }
        }



    }




    //std::cout << "\nMade it to the end of LoadLevel" << std::endl;
    return 0;
}




