#include "InventoryUI.h"

InventoryUI::InventoryUI(SDL_Renderer* renderer, float scale) :
    mRenderer(renderer),
    slotSize(64.0f * scale),
    padding(8.0f * scale),
    uiScale(scale)
{
    // Initialize TTF if not already initialized
    if (!TTF_WasInit() && TTF_Init() == -1) {
        std::cerr << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
    }

    // Load font
    mFont = TTF_OpenFont("../assets/fonts/ARIAL.TTF", static_cast<int>(18 * uiScale));
    if (!mFont) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
    }

    loadTextures();
}

InventoryUI::~InventoryUI() {
    // Clean up textures
    if (breakTexture) {
        SDL_DestroyTexture(breakTexture);
    }

    for (auto texture : itemTextures) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    // Close font
    if (mFont) {
        TTF_CloseFont(mFont);
    }
}

void InventoryUI::loadTextures() {
    // Resize to accommodate all your item types
    itemTextures.resize(4, nullptr); // Adjust based on how many different items you have

    // Load textures for different item types
    SDL_Surface* surface = IMG_Load("../assets/images/blocks/1.png");
    if (surface) {
        itemTextures[1] = SDL_CreateTextureFromSurface(mRenderer, surface);
        SDL_FreeSurface(surface);
    } else {
        std::cerr << "Failed to load grass texture: " << IMG_GetError() << std::endl;
    }

    surface = IMG_Load("../assets/images/blocks/2.png");
    if (surface) {
        itemTextures[2] = SDL_CreateTextureFromSurface(mRenderer, surface);
        SDL_FreeSurface(surface);
    } else {
        std::cerr << "Failed to load stone texture: " << IMG_GetError() << std::endl;
    }

    surface = IMG_Load("../assets/images/blocks/3.png");
    if (surface) {
        itemTextures[3] = SDL_CreateTextureFromSurface(mRenderer, surface);
        SDL_FreeSurface(surface);
    } else {
        std::cerr << "Failed to load dirt texture: " << IMG_GetError() << std::endl;
    }

    // Load break tool texture
    surface = IMG_Load("../assets/images/blocks/0.png");
    if (surface) {
        breakTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
        SDL_FreeSurface(surface);
    } else {
        std::cerr << "Failed to load break tool texture: " << IMG_GetError() << std::endl;
    }
}

void InventoryUI::render(const Inventory& inventory, int currentSlot, int screenWidth, int screenHeight) {

    // Calculate starting position (centered at bottom of screen)
    float totalWidth = (inventory.cols + 1) * (slotSize + padding); // +1 for break tool
    float startX = (screenWidth - totalWidth) / 2.0f;
    float y = screenHeight - slotSize - padding * 3;


    // Draw break tool (slot 0)
    drawSlot(0, currentSlot == 0, 0, 0, startX, y);

    // Draw inventory slots
    for (int i = 0; i < inventory.cols; i++) {
        float x = startX + (i + 1) * (slotSize + padding);
        drawSlot(i + 1, currentSlot == i + 1,
                 inventory.slots[0][i].itemID,
                 inventory.slots[0][i].amount, x, y);
    }
}

void InventoryUI::drawSlot(int slotIndex, bool isSelected, int itemID, int itemAmount, float xPos, float yPos) {
    // Draw slot background
    SDL_Rect slotRect = {
        static_cast<int>(xPos),
        static_cast<int>(yPos),
        static_cast<int>(slotSize),
        static_cast<int>(slotSize)
    };

    // Set background color with alpha (semi-transparent)
    SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(mRenderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(mRenderer, &slotRect);

    // Draw outline
    SDL_Color& outlineClr = isSelected ? selectedOutlineColor : outlineColor;
    SDL_SetRenderDrawColor(mRenderer, outlineClr.r, outlineClr.g, outlineClr.b, outlineClr.a);

    // Draw thicker outline for selected slot
    int outlineThickness = isSelected ? 3 : 2;
    for (int i = 0; i < outlineThickness; i++) {
        SDL_Rect outlineRect = {
            static_cast<int>(xPos) + i,
            static_cast<int>(yPos) + i,
            static_cast<int>(slotSize) - i * 2,
            static_cast<int>(slotSize) - i * 2
        };
        SDL_RenderDrawRect(mRenderer, &outlineRect);
    }

    // Draw item icon if not empty
    if (slotIndex == 0) {
        // Draw break tool
        if (breakTexture) {
            int textureWidth, textureHeight;
            SDL_QueryTexture(breakTexture, NULL, NULL, &textureWidth, &textureHeight);

            float scale = (slotSize - 16.0f) / textureWidth;

            SDL_Rect destRect = {
                static_cast<int>(xPos + (slotSize - textureWidth * scale) / 2.0f),
                static_cast<int>(yPos + (slotSize - textureHeight * scale) / 2.0f),
                static_cast<int>(textureWidth * scale),
                static_cast<int>(textureHeight * scale)
            };

            SDL_RenderCopy(mRenderer, breakTexture, NULL, &destRect);
        }
    }
    else if (itemID > 0 && itemAmount > 0 && itemID < static_cast<int>(itemTextures.size()) && itemTextures[itemID]) {
        // Draw item
        int textureWidth, textureHeight;
        SDL_QueryTexture(itemTextures[itemID], NULL, NULL, &textureWidth, &textureHeight);

        float scale = (slotSize - 16.0f) / textureWidth;

        SDL_Rect destRect = {
            static_cast<int>(xPos + (slotSize - textureWidth * scale) / 2.0f),
            static_cast<int>(yPos + (slotSize - textureHeight * scale) / 2.0f),
            static_cast<int>(textureWidth * scale),
            static_cast<int>(textureHeight * scale)
        };

        SDL_RenderCopy(mRenderer, itemTextures[itemID], NULL, &destRect);

        // Draw item amount if more than 1
        if (itemAmount > 1 && mFont) {
            SDL_Surface* textSurface = TTF_RenderText_Solid(
                mFont,
                std::to_string(itemAmount).c_str(),
                amountTextColor
            );

            if (textSurface) {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);

                if (textTexture) {
                    SDL_Rect textRect = {
                        static_cast<int>(xPos + slotSize - textSurface->w - 5),
                        static_cast<int>(yPos + slotSize - textSurface->h - 5),
                        textSurface->w,
                        textSurface->h
                    };

                    // Draw text with black outline for better visibility
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            if (dx != 0 || dy != 0) {
                                SDL_Rect outlineRect = {
                                    textRect.x + dx,
                                    textRect.y + dy,
                                    textRect.w,
                                    textRect.h
                                };
                                SDL_SetTextureColorMod(textTexture, 0, 0, 0);
                                SDL_RenderCopy(mRenderer, textTexture, NULL, &outlineRect);
                            }
                        }
                    }

                    // Draw the actual text
                    SDL_SetTextureColorMod(textTexture, 255, 255, 255);
                    SDL_RenderCopy(mRenderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);
                }

                SDL_FreeSurface(textSurface);
            }
        }
    }

    // Draw slot number
    if (mFont) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(
            mFont,
            std::to_string(slotIndex).c_str(),
            textColor
        );

        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);

            if (textTexture) {
                SDL_Rect textRect = {
                    static_cast<int>(xPos + 5),
                    static_cast<int>(yPos + 5),
                    textSurface->w,
                    textSurface->h
                };

                SDL_RenderCopy(mRenderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }

            SDL_FreeSurface(textSurface);
        }
    }
}