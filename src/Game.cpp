#include "Game.h"
#include <iostream>

Game::Game() : mRunning(false), mWindow(nullptr), mRenderer(nullptr), mPreviousTicks(0), mPlayer(nullptr), world(nullptr)
{

}

Game::~Game()
{
    shutdown();
}

bool Game::initialize(int width, int height, const char *title)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    mWindow = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN
        );
    if (!mWindow)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    // Initialize
    mPlayer = new Player(mRenderer);
    world = new World(mRenderer, 0);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr << "SDL_Image could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    mRunning = true;
    mPreviousTicks = SDL_GetTicks();

    return true;
}

void Game::run()
{
    while (mRunning)
    {
        handleEvents();

        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - mPreviousTicks) / 1000.0f;
        mPreviousTicks = currentTicks;

        update(deltaTime);
        render();
    }
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            mRunning = false;
        }

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    mRunning = false;
                    break;
                case SDLK_a:
                    keys.left = true;
                    break;
                case SDLK_d:
                    keys.right = true;
                    break;
                case SDLK_p:
                    keys.helperP = true;
                    break;
                case SDLK_SPACE:
                    keys.space = true;
                    break;
                case SDLK_w:
                    world->printWorld();
                    break;
                default:
                    break;
            }
        }

        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_a:
                    keys.left = false;
                    break;
                case SDLK_d:
                    keys.right = false;
                    break;
                case SDLK_p:
                    keys.helperP = false;
                    break;
                case SDLK_SPACE:
                    keys.space = false;
                    break;
                default:
                    break;
            }
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    keys.mouseLeft = true;
                    break;
                default:
                    break;
            }
        }

        if (event.type == SDL_MOUSEBUTTONUP)
        {
            switch (event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    keys.mouseLeft = false;
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::update(float deltaTime)
{
    //Game Logic
    mPlayer->update(deltaTime,keys, *world);
}

void Game::render()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    // Render game objects here
    world->render();
    mPlayer->render();


    SDL_RenderPresent(mRenderer);
}

void Game::shutdown()
{
    delete mPlayer;
    mPlayer = nullptr;
    delete world;
    world = nullptr;
    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }

    if (mWindow)
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}