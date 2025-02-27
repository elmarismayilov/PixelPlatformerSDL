#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>
#include <World.h>

struct KeyPressed
{
    bool left = false;
    bool right = false;
    bool space = false;
    bool helperP = false;
    bool mouseLeft = false;
    bool mouseRight = false;
};
class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();
    void render();
    void update(float deltaTime, KeyPressed keys, World &world);
    bool putBlock(World &world, int x, int y, int block);
    bool breakBlock(World &world, int xPos, int yPos);
private:
    SDL_Renderer* mRenderer;
    SDL_Texture* mTexture;
    SDL_Rect mDestRect;
    const int mSIZE = 24;
    SDL_Point mPosition = {0,0};
    SDL_Point mVelocity = {0,0};
    int movementSpeed = 100;
    const int gravity = 320;
    float mVelocityYFloat = 0.0f;
    float mMoveAccumulatorX = 0.0f;
    float mMoveAccumulatorY = 0.0f;
    int jumpStrength = 200;
    bool onGround = false;
    int blockPutRange = 64;
};



#endif //PLAYER_H