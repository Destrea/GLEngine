#pragma once

namespace Renderer
{
    class Texture2D
    {
    public:
        //Texture Obj ID and width/height
        unsigned int ID;
        unsigned int Width, Height;
        //Texture formatting
        unsigned int Internal_Format;
        unsigned int Image_Format;
        //Texture config settings
        unsigned int Wrap_S;
        unsigned int Wrap_T;
        unsigned int Filter_Min;
        unsigned int Filter_Max;

        //Constructor
        Texture2D();

        //Texture generation from image data
        void Generate(unsigned int width, unsigned int height, unsigned char* data);

        //Binding texture to current active GL_TEXTURE_2D object
        void Bind() const;
    };
}
