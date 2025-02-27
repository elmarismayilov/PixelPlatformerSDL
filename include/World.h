#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include <SDL.h>
#include <string>

class World {
public:
    World(SDL_Renderer *renderer,int defaultBlock);
    ~World();
    void render();
    void printWorld();
    const int TILE_SIZE = 32;
    const int WIDTH = 40;
    const int HEIGHT = 23;
    std::vector<std::vector<int>> blocks;
private:
    SDL_Renderer* mRenderer;
    std::vector<SDL_Texture*> textures;
    std::vector<std::string> texturePaths;
};



#endif //WORLD_H