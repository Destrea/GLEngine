#ifndef FBO_H
#define FBO_H

#include "Common.h"
#include "glad/glad.h"
//Handles the framebuffer for the engine's "editor" display.

class FramebuffManager
{
public:
    //Singleton "redefinition" safety, and Get() function.
    FramebuffManager(const FramebuffManager&) = delete;
    static FramebuffManager& Get()
    {
        return s_Instance;
    }


    //Editor space size -- Defaults to 1280 x 720, but can be resized.
    int tex_w = 1280;
    int tex_h = 720;

    //Framebuffer and depth buffer variables, along with the framebuffer texture
    GLuint f_tex;
    GLuint fbo;
    GLuint rbo;

    void init_framebuffer();                            //Initializes the framebuffer and attaches the depth buffer to it
    void rescale_framebuffer(float width, float height);    //Resizes the fbo as needed.
    void fbo_shutdown();                                    //Correctly deletes and "shuts down" the framebuffer operations

    //Each does as they say and returns the vales
    GLuint get_fTex();
    GLuint get_fbo();
    GLuint get_rbo();




private:
    //Placeholders, May delete later.
    void set_fbo();
    void set_rbo();
    void set_fTex();

    //Constructor, for the singleton
    FramebuffManager() { }
    //Singleton instance declaration
    static FramebuffManager s_Instance;

};



#endif
