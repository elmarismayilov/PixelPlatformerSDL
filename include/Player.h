#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include "World.h"
#include <iostream>
#include "Inventory.h"

struct Vector2 {
    int x, y;
};

struct KeyPressed
{
    bool left = false;
    bool right = false;
    bool space = false;
    bool helperP = false;
    bool mouseLeft = false;
};

class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();

    void update(float deltaTime, KeyPressed keys, World& world);
    void render(SDL_Rect camera);
    bool putBlock(World& world, int xPos, int yPos, int block);
    bool breakBlock(World& world, int xPos, int yPos);

    Vector2 mPosition = {900, 300}; // Initial position
    const int mSIZE = 24;
private:
    bool isSolidBlock(World& world, int xPos, int yPos);
    void handleCollisions(World& world, float deltaTime);

    SDL_Renderer* mRenderer;
    SDL_Texture* mTexture;
    SDL_Rect mDestRect;

    float mVelocityX = 0.0f;
    float mVelocityYFloat = 0.0f;
    float mMoveAccumulatorX = 0.0f;
    float mMoveAccumulatorY = 0.0f;
    const float movementSpeed = 100.0f;
    const float jumpStrength = 300.0f;
    const float gravity = 500.0f;
    const float blockPutRange = 100.0f;
    bool onGround = false;
    Inventory inventory;
    int currentSlot;
    int invRows;
    int invCols;
};

#endif