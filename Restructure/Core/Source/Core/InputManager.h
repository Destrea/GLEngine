//Input manager
#pragma once

//#include "Common.h"
#include <GLFW/glfw3.h>
#include "Renderer/Camera.h"
#include "Layer.h"
#include <memory>

namespace Core
{
    class InputManager
    {
    public:
        InputManager();
        ~InputManager();

        bool get_cursor();
        void set_cursor(bool val);
        void processInput(GLFWwindow *window, std::shared_ptr<Camera> p_Camera, float deltaTime);


        bool cursor_locked = false;

        static InputManager& Get();

    private:

    };

}

