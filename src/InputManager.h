//Input manager

#ifndef INPUTMGR_H
#define INPUTMGR_H

#include "include/Common.h"
#include "include/Camera.h"
#include "game.h"

class InputManager
{
public:

    InputManager(const InputManager&) = delete;
    bool get_cursor();
    void set_cursor(bool val);
    void processInput(GLFWwindow *window, Camera* p_Camera, float deltaTime);

    static InputManager& Get()
    {
        return s_Instance;
    }

private:
    InputManager() { }
    bool cursor_locked = false;
    static InputManager s_Instance;
};

#endif
