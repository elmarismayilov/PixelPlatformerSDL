#pragma once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Player.h"
#include "World.h"

class Game {
public:
    Game();
    ~Game();

    bool initialize(int width, int height, const char* title);
    void run();
    void shutdown();
private:
    void handleEvents();
    void update(float deltaTime);
    void render();

    bool mRunning;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mPreviousTicks;
    Player* mPlayer;
    KeyPressed keys;
    World* world;

    SDL_Rect camera = {0, 0, 1280, 720};
};



#endif //GAME_H