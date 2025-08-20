#ifndef GAME_H
#define GAME_H

#include "include/Common.h"


#include "LevelLayout.h"
#include "include/Camera.h"

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


private:

    //DeltaTime variables
    float deltaTime;
    float lastFrame;

};

#endif
