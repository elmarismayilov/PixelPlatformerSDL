#include "Game.h"
#include <iostream>

Game::Game() : mRunning(false), mWindow(nullptr), mRenderer(nullptr), mPreviousTicks(0), mPlayer(nullptr), world(nullptr)
{
}

Game::~Game()
{
    shutdown();
}

bool Game::initialize(int width, int height, const char* title)
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

    mPlayer = new Player(mRenderer, 1, 3);
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

    // Initialize camera position
    camera.x = mPlayer->mPosition.x - camera.w / 2;
    camera.y = mPlayer->mPosition.y - camera.h / 2;

    return true;
}

void Game::run()
{
    while (mRunning)
    {
        handleEvents();

        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - mPreviousTicks) / 1000.0f;
        deltaTime = std::min(deltaTime, 0.1f);
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
                case SDLK_0:
                    keys.inventorySlot = 0;
                    break;
                case SDLK_1:
                    keys.inventorySlot = 1;
                    break;
                case SDLK_2:
                    keys.inventorySlot = 2;
                    break;
                case SDLK_3:
                    keys.inventorySlot = 3;
                    break;
                case SDLK_4:
                    keys.inventorySlot = 4;
                    break;
                case SDLK_5:
                    keys.inventorySlot = 5;
                    break;
                case SDLK_6:
                    keys.inventorySlot = 6;
                    break;
                case SDLK_7:
                    keys.inventorySlot = 7;
                    break;
                case SDLK_8:
                    keys.inventorySlot = 8;
                    break;
                case SDLK_9:
                    keys.inventorySlot = 9;
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
    mPlayer->update(deltaTime, keys, *world);

    // Camera smoothing
    float smoothFactorX = 0.1f;
    float smoothFactorY = 0.05f;
    float deadZoneY = 50.0f;

    float targetX = mPlayer->mPosition.x + mPlayer->mSIZE / 2 - camera.w / 2;
    float targetY = mPlayer->mPosition.y + mPlayer->mSIZE / 2 - camera.h / 2;

    // Smooth X movement
    camera.x += static_cast<int>((targetX - camera.x) * smoothFactorX);
    // Smooth Y movement only outside dead zone
    if (std::abs(targetY - camera.y) > deadZoneY) {
        camera.y += static_cast<int>((targetY - camera.y) * smoothFactorY);
    }

    // Clamp camera to world bounds
    camera.x = std::max(0, std::min(camera.x, world->WIDTH * world->TILE_SIZE - camera.w));
    camera.y = std::max(0, std::min(camera.y, world->HEIGHT * world->TILE_SIZE - camera.h));
}

void Game::render()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    // Apply camera offset
    world->render(camera);
    mPlayer->render(camera);

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