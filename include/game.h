#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include <GLFW/glfw3.h>

#include "LevelLayout.h"
#include "Camera.h"
#include "InputManager.h"
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
    void ProcessInput(GLFWwindow*, Camera* p_Cam);
    void Update(float dt);
    void Render();

    void calcDeltaTime();
    float get_deltaTime();
    void set_debug(bool var);
    bool get_debug();

private:

    //DeltaTime variables
    float deltaTime;
    float lastFrame;
    bool  debug_mode;
};

#endif
