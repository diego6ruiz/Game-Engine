#include <iostream>
#include <cmath>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "entt.hpp"
#include "Scene/systems.hpp"
#include "Game.h"
#include "Scene/STexture.cpp"

entt::registry reg;

Game::Game(){
    FPS = 60;
    frameDuration = (1.0f / FPS)* 1000.0f;
    counter = 0;
    dT = 0;
}

Game::~Game(){

}

void Game::init(const char* title, int width, int height)
{
    std::cout << "Game Setting Up..." << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, 0,0, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1,0);
    isRunning = true;
}

void Game::setup()
{
    std::cout << "Game Setting Up..." << std::endl;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    background = new STexture(renderer, window);
    background->load("./assets/bg.png");

    world = World{4280, 4280};

    // ball = reg.create();
    // reg.emplace<Name>(ball, "Ball");
    // reg.emplace<BoxComponent>(ball, 10, 10);
    // reg.emplace<TransformComponent>(ball, 50, 250);
    // reg.emplace<VelocityComponent>(ball, 0.4f, 0.2f);

    player1 = reg.create();
    enemy = reg.create();
    // reg.emplace<Name>(player1, "Player1");
    // reg.emplace<BoxComponent>(player1, 30, window_height / 4);
    // reg.emplace<TransformComponent>(player1, 0, 250);
    // reg.emplace<VelocityComponent>(player1, 0, 0);

    createPlayerSystem(100, 100, "./assets/1.png", player1R);
    createPlayerSystem(2, 100, 100, "./assets/2.png", enemyR);  

    // player2 = reg.create();
    // reg.emplace<Name>(player2, "Player2");
    // reg.emplace<BoxComponent>(player2, 30, window_height / 4);
    // reg.emplace<TransformComponent>(player2, window_width - reg.get<BoxComponent>(player2).w, 250);
    // reg.emplace<VelocityComponent>(player2, 0, 0);
}

void Game::handleEvents()
{
    std::cout << "Game Handling Event..." << std::endl;
    handleEventSystem();
}

void Game::update()
{
    
    std::cout << "Game Updating..." << std::endl;
    limitSystem();
    
    //ballMovementSystem();
    camerFollowSystem();
    movementSystem();
    collisionHandler ();

}

void Game::render()
{
    std::cout << "Game Rendering..." << std::endl;
    SDL_SetRenderDrawColor(renderer, 50,50,100,1);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255,255,255,1);
    //SDL_RenderFillRect(renderer, &player1);
    //SDL_RenderFillRect(renderer, &player2);
    //SDL_RenderFillRect(renderer, &ball);

    //boxRenderSystem(renderer);
    drawBackgroundSystems();
    drawPlayerSystem(renderer);

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    std::cout << "Game Cleaning Up..." << std::endl;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Over..." << std::endl;
}

bool Game::running()
{
    return isRunning;
}



