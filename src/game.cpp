#include "../include/game.h"



Game::Game(unsigned int width, unsigned int height) : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{

}

void Game::Init(GLFWwindow* &My_Window)
{


}

void Game::Update(float dt)
{

}

void Game::ProcessInput(GLFWwindow* window, Camera* p_Cam)
{
    InputManager::Get().processInput(window, p_Cam, deltaTime);
}

void Game::Render()
{

}


void Game::calcDeltaTime()
{
    //Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

float Game::get_deltaTime()
{
    return deltaTime;
}
