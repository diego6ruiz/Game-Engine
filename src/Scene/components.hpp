#ifndef COMPONENTS
#define COMPONENTS

#include <iostream>
#include <SDL2/SDL.h>
#include <string.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

struct TagComponent {
  std::string tag;
};

struct Name
{
    std::string name;
};

struct BoxComponent
{
    int w, h;
};

struct TransformComponent
{
    int x, y;
};

struct VelocityComponent
{
    float vel_x, vel_y;
};

struct SpriteComponent
{
    SDL_Texture *texture;
    SDL_Rect box;
};


#endif