#include "World.h"
#include <SDL_image.h>
#include <iostream>

World::World(SDL_Renderer* renderer, int defaultBlock) : mRenderer(renderer)
{
    texturePaths.push_back("../assets/images/blocks/0.png");
    texturePaths.push_back("../assets/images/blocks/1.png");
    texturePaths.push_back("../assets/images/blocks/2.png");
    texturePaths.push_back("../assets/images/blocks/3.png");
    for (int i = 0; i < texturePaths.size(); i++)
    {
        SDL_Texture* texture = IMG_LoadTexture(renderer, texturePaths[i].c_str());
        textures.push_back(texture);
    }

    blocks.resize(HEIGHT);
    for (int i = 0; i < HEIGHT; i++)
    {
        blocks[i].resize(WIDTH, defaultBlock);
    }

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == 18) blocks[i][j] = 1;
            if (i > 18) blocks[i][j] = 2;
            if (i >= HEIGHT -3 ) blocks[i][j] = 3;
        }
    }
}

World::~World()
{
    for (int i = 0; i < textures.size(); i++)
    {
        SDL_DestroyTexture(textures[i]);
        textures[i] = nullptr;
    }
}

void World::render(SDL_Rect camera)
{
    this->camera = camera; // Store camera for Player access
    SDL_Texture* texture;
    SDL_Rect rect;
    int startX = camera.x / TILE_SIZE;
    int endX = (camera.x + camera.w) / TILE_SIZE + 1;
    int startY = camera.y / TILE_SIZE;
    int endY = (camera.y + camera.h) / TILE_SIZE + 1;

    startX = std::max(0, startX);
    endX = std::min(WIDTH, endX);
    startY = std::max(0, startY);
    endY = std::min(HEIGHT, endY);

    for (int i = startY; i < endY; i++)
    {
        for (int j = startX; j < endX; j++)
        {
            texture = textures[blocks[i][j]];
            rect = {j * TILE_SIZE - camera.x, i * TILE_SIZE - camera.y, TILE_SIZE, TILE_SIZE};
            SDL_RenderCopy(mRenderer, texture, NULL, &rect);
        }
    }
}

void World::printWorld()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            std::cout << blocks[i][j];
        }
        std::cout << std::endl;
    }
}