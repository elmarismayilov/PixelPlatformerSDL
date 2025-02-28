#include "Inventory.h"
#include <iostream>

bool Inventory::addItem(int itemID, int quantity)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (slots[i][j].itemID == itemID)
            {
                slots[i][j].amount += quantity;
                return true;
            }
        }
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (slots[i][j].isEmpty())
            {
                slots[i][j] = Slot(itemID, quantity);
                return true;
            }
        }
    }
    return false;
}

bool Inventory::removeItem(int itemID, int quantity)
{
    // Early return if invalid parameters
    if (itemID <= 0 || quantity <= 0) return false;

    // Since we only have one row, we can optimize the search
    for (int j = 0; j < cols; j++)
    {
        if (slots[0][j].itemID == itemID)
        {
            if (slots[0][j].amount >= quantity)
            {
                slots[0][j].amount -= quantity;
                if (slots[0][j].amount <= 0)
                {
                    // Reset to empty slot
                    slots[0][j] = Slot();
                }
                return true;
            }
        }
    }
    return false;
}

void Inventory::printInventory()
{
    std::cout << "Inventory: " << std::endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << slots[i][j].itemID << " : " << slots[i][j].amount << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "END OF INVENTORY -------------------";
}