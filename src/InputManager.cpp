
#include "include/Common.h"


InputManager InputManager::s_Instance;

void InputManager::set_cursor(bool val)
{
    cursor_locked = val;
}

bool InputManager::get_cursor()
{
    return cursor_locked;
}

void InputManager::processInput(GLFWwindow* window, Camera* p_Camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && get_cursor() == true)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        set_cursor(false);
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        p_Camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        p_Camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        p_Camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        p_Camera->ProcessKeyboard(RIGHT, deltaTime);
}
