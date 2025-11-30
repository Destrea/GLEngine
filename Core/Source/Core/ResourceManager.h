#pragma once

#include <map>
#include <string>

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

using namespace Renderer;

namespace Core {

    class ResourceManager
    {
    public:
        ResourceManager();
        ~ResourceManager();

        static std::map<std::string, Shader> Shaders;
        static std::map<std::string, Texture2D> Textures;

        //Shader loading and generation
        static Shader LoadShader(const char *vShaderFile, const char *fShaderFile, std::string name);
        static Shader ReloadShader(unsigned int ID, const char *vShaderFile, const char *fShaderFile, std::string name);
        //Shader Retrieval
        static Shader GetShader(std::string name);

        //Texure Loading and generation
        static Texture2D LoadTexture(const char *file, bool alpha, std::string name);

        //Texture Retrieval
        static Texture2D GetTexture(std::string name);

        //Properly De-allocate all loaded resources
        static void Clear();

    private:
        static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile);

        static Texture2D loadTextureFromFile(const char *file, bool alpha);

    };
}
