
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

//#include "include/glad/glad.h"
//#include "Common.h"
#include "Shader.h"
#include "Texture.h"

class ResourceManager
{
public:
    //Resource Storage
    static std::map<std::string, Shader>     Shaders;
    static std::map<std::string, Texture2D>  Textures;

    //Shader loading and Generating, using the designated source code
    static Shader   LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    // retrieves a stored shader
    static Shader   GetShader(std::string name);
    //Texture loading and generating, from a file
    static Texture2D LoadTexture(const char *file, bool alpha, std::string name);
    // retrieves a stored texture
    static Texture2D GetTexture(std::string name);



    // properly de-allocate all loaded resources
    static void     Clear();


private:
    //Private constructor
    ResourceManager() { }

    //loads and generates a shader from a file
    static Shader   loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile);
    // loads a single texture from file
    static Texture2D loadTextureFromFile(const char *file, bool alpha);

};

#endif
