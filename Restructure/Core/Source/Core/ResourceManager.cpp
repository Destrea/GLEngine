#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include "stb_image.h"
#include <glad/glad.h>

using namespace Renderer;
using namespace Core;

std::map<std::string, Texture2D> Core::ResourceManager::Textures;
std::map<std::string, Shader> Core::ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, std::string name)
{
    //needs error handling
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return Shaders[name];
}

Shader ResourceManager::ReloadShader(unsigned int ID, const char *vShaderFile, const char *fShaderFile, std::string name)
{
    //Create the new reloaded shader
    Shader newShader = loadShaderFromFile(vShaderFile, fShaderFile);

    //Error handling
    if(newShader.errors)
    {
        //error message output here
        return Shaders[name];
    }

    //Replacing the old shader and returning
    Shaders[name] = newShader;
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char *file, bool alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    for(auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    for(auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}


Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile)
{
    std::string vertexCode;
    std::string fragmentCode;
    Shader shader;
    try
    {
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        //Reading the buffers contents into the string streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        //Close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        //convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
        shader.errors = true;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    shader.Compile(vShaderCode, fShaderCode);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
{
    //Texture obj creation
    Texture2D texture;
    if(alpha)       //Alpha channel format handling
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    //Load image format
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    //Generate the texture
    texture.Generate(width, height, data);
    //free img data for cleanup
    stbi_image_free(data);
    return texture;
}


