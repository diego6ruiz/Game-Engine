#ifndef GAME
#define GAME

#include <SDL2/SDL.h>
#include "entt.hpp"
#include "Scene/STexture.cpp"

struct Camera
{
    int x;
    int y;
};

struct World
{
    int w;
    int h;
};

class Game {
  public:
    Game();
    ~Game();

    int startClock;
    int fps;

    __int64_t previous_time;
    __int64_t current_time;
    __int64_t dT;

    void init(const char* title, int width, int height);
    void setup();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running();

    entt::registry reg;

    void boxRenderSystem(SDL_Renderer *renderer);
    void createPlayerSystem(int w, int h, std::string spriteName, SDL_Rect rect);
    void createPlayerSystem(int enemy, int w, int h, std::string spriteName, SDL_Rect rect);
    //void createBallSystem(entt::entity player, int x, int y, int w, int h, float v_x, float v_y);
  
    void handleEventSystem();
    void movementSystem();
    void limitSystem();
    void ballMovementSystem();
    void collisionHandler ();
    
    
    
    //void collisionHandler();

    void drawPlayerSystem(SDL_Renderer *renderer);
    void drawBackgroundSystems();
    void camerFollowSystem();

  private:
    int counter;
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    
    int window_width, window_height;

    int player_speed = 30;
    float ball_x_speed = 0.4f; //0.4
    float ball_y_speed = 0.2f; //0.2

    entt::entity player1, player2, ball, enemy;
    SDL_Rect player1R, enemyR;

    // for frame management
    int FPS;
    Uint32 frameStartTimestamp;
    Uint32 frameEndTimestamp;
    float frameDuration;
    //float dT; // time in miliseconds since last frame

    Camera camera;
    World world;

    STexture *background;
};

#endif