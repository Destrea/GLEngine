//Input manager

#ifndef INPUTMGR_H
#define INPUTMGR_H

//#include "Common.h"
#include "Camera.h"
#include "game.h"
#include "Shader.h"

class InputManager
{
public:

    InputManager(const InputManager&) = delete;
    static InputManager& Get()
    {
        return s_Instance;
    }

    bool get_cursor();
    void set_cursor(bool val);
    void processInput(GLFWwindow *window, Camera* p_Camera, float deltaTime);


    bool cursor_locked = false;

private:

    InputManager() { }

    static InputManager s_Instance;
};

#endif
