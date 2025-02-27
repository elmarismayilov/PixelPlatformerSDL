#include <iostream>
#include "Game.h"

int main(int argc, char* argv[])
{
    Game game;

    if (game.initialize(1280, 720,"Pixel Platformer"))
    {
        game.run();
    }

    game.shutdown();
    return 0;
}