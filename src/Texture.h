#ifndef TEXTURE_H
#define TEXTURE_H

#include "include/glad/glad.h"

class Texture2D
{
public:
    // texture object ID, used for all texture operations
    unsigned int ID;
    //texture image dimensions
    unsigned int Width, Height;
    //texture format
    unsigned int Internal_Format;   //Format of texture obj
    unsigned int Image_Format;      //format of image loaded
    //texture configuration settings
    unsigned int Wrap_S;    // S-axis texture wrap
    unsigned int Wrap_T;    // T-axis texture wrap
    unsigned int Filter_Min;// filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max;// filtering mode if texture pixels > screen pixels
    //constructor
    Texture2D();
    // generates texture from image data
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    // binding texture to current active GL_TEXTURE_2D texture obj
    void Bind() const;
};

#endif
