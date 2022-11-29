#include <iostream>
#include <string.h>
#include "Game/Game.h"
#include <SDL2/SDL.h>
#include "components.hpp"

void Game::boxRenderSystem(SDL_Renderer *renderer)
{
    const auto view = reg.view<BoxComponent, TransformComponent>();
    for (const auto e : view)
    {
        auto box = view.get<BoxComponent>(e);
        auto position = view.get<TransformComponent>(e);
        SDL_Rect rect;
        rect.x = position.x;
        rect.y = position.y;
        rect.w = box.w;
        rect.h = box.h;
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Game::handleEventSystem()
{
    SDL_Event event;
    auto &speed1 = reg.get<VelocityComponent>(player1);
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    int move = 10;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }


        if (event.type == SDL_KEYDOWN)
        {
            std::cout << "KEY PRESSED" << std::endl;

            if (state[SDL_SCANCODE_A])
            {
                speed1.vel_x = -move;
            }
            else if (state[SDL_SCANCODE_D])
            {
                speed1.vel_x = move;
            }

            if (state[SDL_SCANCODE_W])
            {
                speed1.vel_y = -move;
            }
            else if (state[SDL_SCANCODE_S])
            {
                speed1.vel_y = move;
            }
        }
        if (event.type == SDL_KEYUP)
        {
            std::cout << "KEY RELEASED" << std::endl;

            int key = event.key.keysym.sym;
            if (key == SDLK_a)
            {
                speed1.vel_x = 0;
            }
            else if (key == SDLK_d)
            {
                speed1.vel_x = 0;
            }

            if (key == SDLK_w)
            {
                speed1.vel_y = 0;
            }
            else if (key == SDLK_s)
            {
                speed1.vel_y = 0;
            }
        }
    }
}

void Game::movementSystem()
{
    const auto view = reg.view<TransformComponent, VelocityComponent>();
    for (const auto e : view)
    {
        auto speed = view.get<VelocityComponent>(e);
        auto &position = view.get<TransformComponent>(e);

        position.x += speed.vel_x;
        position.y += speed.vel_y;
    }
}

void Game::limitSystem()
{
    //y axis
    if (reg.get<TransformComponent>(player1).y <= 0)
    {
        reg.get<TransformComponent>(player1).y = 0;
    }
    if (reg.get<TransformComponent>(player1).y >= world.h - reg.get<BoxComponent>(player1).h)
    {
        reg.get<TransformComponent>(player1).y = world.h - reg.get<BoxComponent>(player1).h;
    }

    //x axis
    if (reg.get<TransformComponent>(player1).x <= 0)
    {
        reg.get<TransformComponent>(player1).x = 0;
    }
    if (reg.get<TransformComponent>(player1).x >= world.w - reg.get<BoxComponent>(player1).w)
    {
        reg.get<TransformComponent>(player1).x = world.w - reg.get<BoxComponent>(player1).w;
    }
}

void Game::ballMovementSystem()
{

    reg.get<TransformComponent>(ball).y += reg.get<VelocityComponent>(ball).vel_y * dT;
    if ((reg.get<TransformComponent>(ball).y < 0) || (reg.get<TransformComponent>(ball).y > (window_height - reg.get<BoxComponent>(ball).h)))
    {
        reg.get<VelocityComponent>(ball).vel_y *= -1.05f;
    }

    reg.get<TransformComponent>(ball).x += reg.get<VelocityComponent>(ball).vel_x * dT;
    if (reg.get<VelocityComponent>(ball).vel_x < 0)
    {
        if ((reg.get<TransformComponent>(ball).y + reg.get<BoxComponent>(ball).h >= reg.get<TransformComponent>(player1).y) && (reg.get<TransformComponent>(ball).y <= reg.get<TransformComponent>(player1).y + reg.get<BoxComponent>(player1).h))
        {
            if (reg.get<TransformComponent>(ball).x <= 0 + reg.get<BoxComponent>(player1).w)
            {
                reg.get<VelocityComponent>(ball).vel_x *= -1.05f;
            }
        }
        if (reg.get<TransformComponent>(ball).x <= 0)
        {
            std::cout << "Player 2 Won" << std::endl;
            isRunning = false;
        }
    }

    else if (reg.get<VelocityComponent>(ball).vel_x > 0)
    {
        if ((reg.get<TransformComponent>(ball).y + reg.get<BoxComponent>(ball).h >= reg.get<TransformComponent>(player2).y) && (reg.get<TransformComponent>(ball).y <= reg.get<TransformComponent>(player2).y + reg.get<BoxComponent>(player2).h))
        {
            if (reg.get<TransformComponent>(ball).x >= (window_width - reg.get<BoxComponent>(ball).w - reg.get<BoxComponent>(player2).w))
            {
                reg.get<VelocityComponent>(ball).vel_x *= -1.05f;
            }
        }
        if (reg.get<TransformComponent>(ball).x >= window_width - reg.get<BoxComponent>(ball).w)
        {
            std::cout << "Player 1 Won" << std::endl;
            isRunning = false;
        }
    }
}




void Game::createPlayerSystem(int w, int h, std::string spriteName, SDL_Rect rect)
{
    reg.emplace<TransformComponent>(player1, (world.w / 2) - (w / 2), (world.h / 2) - (h / 2));
    reg.emplace<BoxComponent>(player1, w, h);
    SDL_Surface *surface = IMG_Load(spriteName.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
    reg.emplace<SpriteComponent>(player1, texture, rect);
    reg.emplace<VelocityComponent>(player1, 0, 0);

    camera.x = (world.w / 2) - (window_width / 2);
    camera.y = (world.h / 2) - (window_height / 2);
}

void Game::createPlayerSystem(int e, int w, int h, std::string spriteName, SDL_Rect rect)
{
    reg.emplace<TransformComponent>(enemy, (world.w / 2) - (w / 2) + 100, (world.h / 2) - (h / 2));
    reg.emplace<BoxComponent>(enemy, w, h);
    SDL_Surface *surface = IMG_Load(spriteName.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
    reg.emplace<SpriteComponent>(enemy, texture, rect);
    reg.emplace<VelocityComponent>(enemy, 0, 0);
}

void Game::drawPlayerSystem(SDL_Renderer *renderer)
{
    const auto view = reg.view<SpriteComponent, BoxComponent, TransformComponent>();
    for (const auto e : view)
    {
        auto sprite = view.get<SpriteComponent>(e);
        auto box = view.get<BoxComponent>(e);
        auto position = view.get<TransformComponent>(e);
        SDL_Rect rect;
        rect.x = position.x - camera.x;
        rect.y = position.y - camera.y;
        rect.w = box.w;
        rect.h = box.h;

        SDL_RenderCopy(renderer, sprite.texture, nullptr, &rect);
    }
}

void Game::drawBackgroundSystems()
{
    SDL_Rect clip = {camera.x, camera.y, window_width, window_height};
    background->render(0, 0, window_width, window_height, &clip, 0, nullptr, SDL_FLIP_NONE);
}

void Game::camerFollowSystem()
{
    const auto position = reg.get<TransformComponent>(player1);
    const auto rect = reg.get<BoxComponent>(player1);
    int cx = position.x - (window_width / 2) + (rect.w / 2);
    int cy = position.y - (window_height / 2) + (rect.h / 2);
    if (cx > 0 and cx < (world.w - window_width))
    {
        camera.x = cx;
    }
    if (cy > 0 and cy < (world.h - window_height))
    {
        camera.y = cy;
    }
}

void Game::collisionHandler()
{
    auto p1 = reg.get<SpriteComponent>(player1).box;
    auto tPos = reg.get<TransformComponent>(player1);
    auto e = reg.get<SpriteComponent>(enemy).box;
    auto ePos = reg.get<TransformComponent>(enemy);

    p1.x = tPos.x;
    p1.y = tPos.y;
    p1.w = reg.get<BoxComponent>(player1).w;
    p1.h = reg.get<BoxComponent>(player1).h;

    e.x = ePos.x;
    e.y = ePos.y;
    e.w = reg.get<BoxComponent>(enemy).w;
    e.h = reg.get<BoxComponent>(enemy).h;

    int speed = 60;

    if (SDL_HasIntersection(&p1, &e))
    {
        if (ePos.y >= tPos.y)
        {
            reg.get<TransformComponent>(enemy).y += speed;
        }
        if (ePos.y <= tPos.y)
        {
            reg.get<TransformComponent>(enemy).y -= speed;
        }
        if (ePos.x >= tPos.x)
        {
            reg.get<TransformComponent>(enemy).x += speed;
        }
        if (ePos.x <= tPos.x)
        {
            reg.get<TransformComponent>(enemy).x -= speed;
        }
    }
}   
