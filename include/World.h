#ifndef WORLD_H
#define WORLD_H

#include <SDL.h>
#include <vector>
#include <string>

class World {
public:
    World(SDL_Renderer* renderer, int defaultBlock);
    ~World();

    void render(SDL_Rect camera);
    void printWorld();

    const int WIDTH = 100;
    const int HEIGHT = 54;
    const int TILE_SIZE = 32;
    std::vector<std::vector<int>> blocks;
    SDL_Rect camera; // Add this to access in Player

private:
    SDL_Renderer* mRenderer;
    std::vector<std::string> texturePaths;
    std::vector<SDL_Texture*> textures;
};

#endif