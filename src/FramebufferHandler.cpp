//Framebuffer/Editor layer, for the engine.


#include "include/Common.h"

FramebuffManager FramebuffManager::s_Instance;


void FramebuffManager::init_framebuffer()
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1, &f_tex);
    glBindTexture(GL_TEXTURE_2D, f_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_w, tex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, f_tex,0);

    //Depth buffer attachment

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tex_w, tex_h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,rbo);
}


void FramebuffManager::rescale_framebuffer(float width, float height)
{
    glBindTexture(GL_TEXTURE_2D, f_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, f_tex, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

void FramebuffManager::fbo_shutdown()
{
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &f_tex);
    glDeleteRenderbuffers(1, &rbo);
}

GLuint FramebuffManager::get_fTex()
{
    return fbo;
}
GLuint FramebuffManager::get_fbo()
{
    return fbo;
}
GLuint FramebuffManager::get_rbo()
{
    return f_tex;
}


void FramebuffManager::set_resolution(int width, int height)
{
    tex_w = width;
    tex_h = height;
    resolution = glm::vec2(tex_w,tex_h);
}



glm::vec2 FramebuffManager::get_resolution()
{

    return resolution;
}



