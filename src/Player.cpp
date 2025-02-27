#include "Player.h"


Player::Player(SDL_Renderer *renderer) : mRenderer(renderer)
{
    mTexture = IMG_LoadTexture(mRenderer, "../assets/images/characters/player.png");
    if (mTexture == nullptr)
    {
        std::cerr << "Error loading player texture" << std::endl;
    }

    mDestRect.x = mPosition.x;
    mDestRect.y = mPosition.y;
    mDestRect.w = mSIZE;
    mDestRect.h = mSIZE;
}

Player::~Player()
{
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;
}


void Player::render()
{
    SDL_RenderCopy(mRenderer, mTexture, NULL, &mDestRect);
}

void Player::update(float deltaTime, KeyPressed keys, World &world) {
    mVelocity.x = 0;

    if (keys.left && !keys.right) mVelocity.x = -movementSpeed;
    if (keys.right && !keys.left) mVelocity.x = movementSpeed;
    if (keys.helperP) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        std::cout << "Mouse x: " << mouseX << " Mouse y: " << mouseY << std::endl;
    }
    if (keys.mouseLeft)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int xPos = mouseX / world.TILE_SIZE;
        int yPos = mouseY / world.TILE_SIZE;

        std::cout << "Mouse position: (" << xPos << ", " << yPos << ")" <<
                std::endl;
        bool broken = breakBlock(world, xPos, yPos);
        if (broken)
        {
            std::cout << "Block successfully broken" << std::endl;
        }
    }

    if (keys.space && onGround) {
        mVelocityYFloat = -jumpStrength;
        onGround = false;
    }

    if (!onGround) {
        mVelocityYFloat += gravity * deltaTime;
    } else {
        mVelocityYFloat = 0;
        mMoveAccumulatorY = 0;
    }


    mMoveAccumulatorX += mVelocity.x * deltaTime;
    int deltaX = static_cast<int>(mMoveAccumulatorX);
    mMoveAccumulatorX -= deltaX;
    mPosition.x += deltaX;


    mMoveAccumulatorY += mVelocityYFloat * deltaTime;
    int deltaY = static_cast<int>(mMoveAccumulatorY);
    mMoveAccumulatorY -= deltaY;
    mPosition.y += deltaY;


    if (mPosition.y >= 550) {
        mPosition.y = 550;
        onGround = true;
    } else {
        onGround = false;
    }

    mDestRect.x = mPosition.x;
    mDestRect.y = mPosition.y;
}

bool Player::putBlock(World &world, int xPos, int yPos, int block)
{
    int x = xPos * world.TILE_SIZE + world.TILE_SIZE / 2;
    int y = yPos * world.TILE_SIZE + world.TILE_SIZE / 2;

    if (!(x <= world.WIDTH * world.TILE_SIZE && y <= world.HEIGHT * world.TILE_SIZE && x >= 0 && y >= 0))
    {
        std::cout << "Position out of bounds" << std::endl;
        return false;
    }

    int playerCenterX = mDestRect.x + mSIZE / 2;
    int playerCenterY = mDestRect.y + mSIZE / 2;
    int dx = playerCenterX - x;
    int dy = playerCenterY - y;

    if (dx*dx + dy*dy <= blockPutRange * blockPutRange)
    {
        if (xPos >= 0 && xPos < world.WIDTH && yPos >= 0 && yPos < world.HEIGHT && world.blocks[yPos][xPos] == 0)
        {
            world.blocks[yPos][xPos] = block;
            std::cout << "Block placed successfully at (" << xPos << ", " << yPos << ")" << std::endl;

            //TODO: Remove item from inventory
            return true;
        }
        std::cout << "Cannot place block - space occupied or invalid position" << std::endl;
        return false;

    }
    std::cout << "Block position too far away (distance: " << sqrt(dx * dx + dy * dy)
    << ", range: " << blockPutRange << ")" << std::endl;
    return false;
}

bool Player::breakBlock(World &world, int xPos, int yPos)
{
    int x = xPos * world.TILE_SIZE + world.TILE_SIZE / 2;
    int y = yPos * world.TILE_SIZE + world.TILE_SIZE / 2;

    if (!(x <= world.WIDTH * world.TILE_SIZE && y <= world.HEIGHT * world.TILE_SIZE && x >= 0 && y >= 0))
    {
        std::cout << "Position out of bounds" << std::endl;
        return false;
    }

    int playerCenterX = mDestRect.x + mSIZE / 2;
    int playerCenterY = mDestRect.y + mSIZE / 2;
    int dx = playerCenterX - x;
    int dy = playerCenterY - y;

    if (dx*dx + dy*dy <= blockPutRange * blockPutRange)
    {
        if (xPos >= 0 && xPos < world.WIDTH && yPos >= 0 && yPos < world.HEIGHT && world.blocks[yPos][xPos] != 3) //TODO: CHECK if not bedrock
        {
            int oldBlock = world.blocks[yPos][xPos];
            if (oldBlock == 0)
            {
                std::cout << "No block to break at (" << xPos << ", " << yPos << ")" << std::endl;
                return false;
            }
            world.blocks[yPos][xPos] = 0;
            std::cout << "Block broken successfully at (" << xPos << ", " << yPos << ")" << std::endl;

            //TODO: Add item to inventory
            return true;
        }
        std::cout << "Cannot break block - bedrock or invalid position" << std::endl;
        return false;

    }
    std::cout << "Block position too far away (distance: " << sqrt(dx * dx + dy * dy)
    << ", range: " << blockPutRange << ")" << std::endl;
    return false;
}