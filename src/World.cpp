#include "World.h"

#include <SDL_image.h>
#include <iostream>

World::World(SDL_Renderer *renderer,int defaultBlock) : mRenderer(renderer)
{
    // Add textures to texturePaths
    texturePaths.push_back("../assets/images/blocks/0.png");
    texturePaths.push_back("../assets/images/blocks/1.png");
    texturePaths.push_back("../assets/images/blocks/2.png");
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
            if (i == 18)
            {
                blocks[i][j] = 1;
            }
            if (i > 18)
            {
                blocks[i][j] = 2;
            }
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


void World::render()
{
    SDL_Texture* texture;
    SDL_Rect rect;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            texture = textures[blocks[i][j]];
            rect = {j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE};
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