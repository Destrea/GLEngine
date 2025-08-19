#ifndef GAME_H
#define GAME_H

#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};


class Game
{
public:
    GameState       State;
    bool            Keys[1024];
    unsigned int    Width, Height;
    // Constructor/Destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    //initialize game state (Loading shaders/textures/levels, etc)
    void Init(GLFWwindow* &My_Window);

    //Game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};

#endif
