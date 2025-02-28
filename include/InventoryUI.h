#ifndef INVENTORY_UI_H
#define INVENTORY_UI_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "Inventory.h"
#include <iostream>

class InventoryUI {
private:
    SDL_Renderer* mRenderer;
    TTF_Font* mFont;
    
    float slotSize;
    float padding;
    float uiScale;
    
    SDL_Texture* breakTexture;
    std::vector<SDL_Texture*> itemTextures;
    
    SDL_Color bgColor = {100, 100, 100, 200};
    SDL_Color outlineColor = {50, 50, 50, 255};
    SDL_Color selectedOutlineColor = {255, 255, 255, 255};
    SDL_Color textColor = {200, 200, 200, 255};
    SDL_Color amountTextColor = {255, 255, 255, 255};
    
    void loadTextures();
    void drawSlot(int slotIndex, bool isSelected, int itemID, int itemAmount, float xPos, float yPos);

public:
    InventoryUI(SDL_Renderer* renderer, float scale = 1.0f);
    ~InventoryUI();
    void render(const Inventory& inventory, int currentSlot, int screenWidth, int screenHeight);
};

#endif // INVENTORY_UI_H