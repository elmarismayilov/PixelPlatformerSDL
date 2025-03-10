#include "Player.h"

Player::Player(SDL_Renderer* renderer, int invRows, int invCols) : mRenderer(renderer), inventory(invRows,invCols), inventoryUI(renderer, 1.0f), currentSlot(0)
{
    // Player Texture
    mTexture = IMG_LoadTexture(mRenderer, "../assets/images/characters/player.png");
    if (mTexture == nullptr)
    {
        std::cerr << "Error loading player texture" << std::endl;
    }

    mDestRect.x = mPosition.x;
    mDestRect.y = mPosition.y;
    mDestRect.w = mSIZE;
    mDestRect.h = mSIZE;

    inventory.addItem(1,30);
    inventory.addItem(2,30);
}

Player::~Player()
{
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;
}

void Player::render(SDL_Rect camera)
{
    SDL_Rect renderRect = {mDestRect.x - camera.x, mDestRect.y - camera.y, mDestRect.w, mDestRect.h};
    SDL_RenderCopy(mRenderer, mTexture, NULL, &renderRect);

    int w,h;
    SDL_GetRendererOutputSize(mRenderer, &w, &h);

    inventoryUI.render(inventory,currentSlot,w,h);
}

void Player::update(float deltaTime, KeyPressed keys, World& world) {
    mVelocityX = 0;
    currentSlot = keys.inventorySlot;
    if (keys.left && !keys.right) mVelocityX = -movementSpeed;
    if (keys.right && !keys.left) mVelocityX = movementSpeed;
    if (keys.helperP) {
        inventory.printInventory();
    }
    if (keys.mouseLeft)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int xPos = (mouseX + world.camera.x) / world.TILE_SIZE;
        int yPos = (mouseY + world.camera.y) / world.TILE_SIZE;
        std::cout << "Mouse position: (" << xPos << ", " << yPos << ")" << std::endl;

        if (currentSlot == 0)
        {
            std::cout << "Attempting to break block at (" << xPos / world.TILE_SIZE << ", " << yPos / world.TILE_SIZE <<
                    ")" << std::endl;
            bool broken = breakBlock(world, xPos, yPos);
            if (broken)
            {
                std::cout << "Block successfully broken" << std::endl;
            }
        }
        else if (currentSlot > 0 && currentSlot <= inventory.cols)
        {
            int slotIndex = currentSlot - 1;

            std::cout << "Trying to place from slot index: " << slotIndex << std::endl;
            std::cout << "Item ID: " << inventory.slots[0][slotIndex].itemID << std::endl;
            std::cout << "Amount: " << inventory.slots[0][slotIndex].amount << std::endl;

            if (!inventory.slots[0][slotIndex].isEmpty())
            {
                int itemID = inventory.slots[0][slotIndex].itemID;

                std::cout << "Attempting to place block with ID " << itemID << " at ("
                        << xPos / world.TILE_SIZE << ", " << yPos / world.TILE_SIZE << ")" << std::endl;

                bool placed = putBlock(world, xPos, yPos, itemID);
                if (placed)
                {
                    std::cout << "Block successfully placed" << std::endl;
                }
            }
        }
        else
        {
            std::cout << "Selected slot is empty" << std::endl;
        }
    }

    if (keys.space && onGround) {
        mVelocityYFloat = -jumpStrength;
        onGround = false;
    }

    mVelocityYFloat += gravity * deltaTime;

    handleCollisions(world, deltaTime);

    mMoveAccumulatorX += mVelocityX * deltaTime;
    int deltaX = static_cast<int>(mMoveAccumulatorX);
    mMoveAccumulatorX -= deltaX;
    mPosition.x += deltaX;

    mMoveAccumulatorY += mVelocityYFloat * deltaTime;
    int deltaY = static_cast<int>(mMoveAccumulatorY);
    mMoveAccumulatorY -= deltaY;
    mPosition.y += deltaY;

    if (mPosition.x <= 0) {
        mPosition.x = 0;
        if (mVelocityX < 0) {
            mVelocityX = 0;
            mMoveAccumulatorX = 0;
        }
    }
    if (mPosition.y <= 0) {
        mPosition.y = 0;
        if (mVelocityYFloat < 0) {
            mVelocityYFloat = 0;
            mMoveAccumulatorY = 0;
        }
    }
    if (mPosition.x >= world.TILE_SIZE * world.WIDTH - mSIZE) {
        mPosition.x = world.TILE_SIZE * world.WIDTH - mSIZE;
        if (mVelocityX > 0) {
            mVelocityX = 0;
            mMoveAccumulatorX = 0;
        }
    }
    if (mPosition.y >= world.TILE_SIZE * world.HEIGHT - mSIZE) {
        mPosition.y = world.TILE_SIZE * world.HEIGHT - mSIZE;
        if (mVelocityYFloat > 0) {
            mVelocityYFloat = 0;
            mMoveAccumulatorY = 0;
            onGround = true;
        }
    }

    mDestRect.x = mPosition.x;
    mDestRect.y = mPosition.y;
}

bool Player::putBlock(World& world, int xPos, int yPos, int block)
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

            inventory.removeItem(block, 1);
            return true;
        }
        std::cout << "Cannot place block - space occupied or invalid position" << std::endl;
        return false;
    }
    std::cout << "Block position too far away (distance: " << sqrt(dx * dx + dy * dy)
    << ", range: " << blockPutRange << ")" << std::endl;
    return false;
}

bool Player::breakBlock(World& world, int xPos, int yPos)
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
        if (xPos >= 0 && xPos < world.WIDTH && yPos >= 0 && yPos < world.HEIGHT && world.blocks[yPos][xPos] != 3)
        {
            int oldBlock = world.blocks[yPos][xPos];
            if (oldBlock == 0)
            {
                std::cout << "No block to break at (" << xPos << ", " << yPos << ")" << std::endl;
                return false;
            }
            world.blocks[yPos][xPos] = 0;
            std::cout << "Block broken successfully at (" << xPos << ", " << yPos << ")" << std::endl;

            inventory.addItem(oldBlock, 1);
            return true;
        }
        std::cout << "Cannot break block - bedrock or invalid position" << std::endl;
        return false;
    }
    std::cout << "Block position too far away (distance: " << sqrt(dx * dx + dy * dy)
    << ", range: " << blockPutRange << ")" << std::endl;
    return false;
}

bool Player::isSolidBlock(World& world, int xPos, int yPos)
{
    return world.blocks[yPos][xPos] != 0;
}

void Player::handleCollisions(World& world, float deltaTime)
{
    int leftTile = mPosition.x / world.TILE_SIZE;
    int rightTile = (mPosition.x + mSIZE - 1) / world.TILE_SIZE;
    int topTile = mPosition.y / world.TILE_SIZE;
    int bottomTile = (mPosition.y + mSIZE - 1) / world.TILE_SIZE;

    leftTile = std::max(0, leftTile);
    rightTile = std::min(world.WIDTH - 1, rightTile);
    topTile = std::max(0, topTile);
    bottomTile = std::min(world.HEIGHT - 1, bottomTile);

    onGround = false;

    int checkGroundTile = (mPosition.y + mSIZE + 1) / world.TILE_SIZE;
    if (checkGroundTile < world.HEIGHT)
    {
        for (int x = leftTile; x <= rightTile; x++)
        {
            if (x >= 0 && x < world.WIDTH && isSolidBlock(world, x, checkGroundTile))
            {
                onGround = true;
                break;
            }
        }
    }

    if (mVelocityYFloat > 0)
    {
        float nextY = mPosition.y + mVelocityYFloat * deltaTime;
        int nextBottomTile = static_cast<int>((nextY + mSIZE) / world.TILE_SIZE);

        if (nextBottomTile < world.HEIGHT)
        {
            for (int x = leftTile; x <= rightTile; x++)
            {
                if (nextBottomTile >= 0 && nextBottomTile < world.HEIGHT &&
                    x >= 0 && x < world.WIDTH && isSolidBlock(world, x, nextBottomTile))
                {
                    mVelocityYFloat = 0;
                    mPosition.y = nextBottomTile * world.TILE_SIZE - mSIZE;
                    mMoveAccumulatorY = 0;
                    onGround = true;
                    break;
                }
            }
        }
    }

    if (mVelocityYFloat < 0)
    {
        float nextY = mPosition.y + mVelocityYFloat * deltaTime;
        int nextTopTile = static_cast<int>(nextY / world.TILE_SIZE);

        if (nextTopTile >= 0)
        {
            for (int x = leftTile; x <= rightTile; x++)
            {
                if (nextTopTile >= 0 && nextTopTile < world.HEIGHT &&
                    x >= 0 && x < world.WIDTH && isSolidBlock(world, x, nextTopTile))
                {
                    mVelocityYFloat = 0;
                    mPosition.y = (nextTopTile + 1) * world.TILE_SIZE;
                    mMoveAccumulatorY = 0;
                    break;
                }
            }
        }
    }

    if (mVelocityX != 0)
    {
        float nextX = mPosition.x + mVelocityX * deltaTime;
        int nextLeftTile = static_cast<int>(nextX / world.TILE_SIZE);
        int nextRightTile = static_cast<int>((nextX + mSIZE - 1) / world.TILE_SIZE);

        if (mVelocityX < 0 && nextLeftTile >= 0)
        {
            for (int y = topTile; y <= bottomTile; y++)
            {
                if (nextLeftTile >= 0 && nextLeftTile < world.WIDTH &&
                    y >= 0 && y < world.HEIGHT && isSolidBlock(world, nextLeftTile, y))
                {
                    mVelocityX = 0;
                    mPosition.x = (nextLeftTile + 1) * world.TILE_SIZE;
                    mMoveAccumulatorX = 0;
                    break;
                }
            }
        }

        if (mVelocityX > 0 && nextRightTile < world.WIDTH)
        {
            for (int y = topTile; y <= bottomTile; y++)
            {
                if (nextRightTile >= 0 && nextRightTile < world.WIDTH &&
                    y >= 0 && y < world.HEIGHT && isSolidBlock(world, nextRightTile, y))
                {
                    mVelocityX = 0;
                    mPosition.x = nextRightTile * world.TILE_SIZE - mSIZE;
                    mMoveAccumulatorX = 0;
                    break;
                }
            }
        }
    }
}