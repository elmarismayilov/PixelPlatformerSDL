#ifndef INVENTORY_H
#define INVENTORY_H
#include <vector>

class Inventory {
public:
    struct Slot
    {
        int itemID;
        int amount;
        Slot(int id = 0, int amt = 0) : itemID(id), amount(amt) {}
        bool isEmpty() const { return itemID == 0 || amount <= 0; }
    };
    std::vector<std::vector<Slot>> slots;
    int rows, cols;
    Inventory(int r, int c) : rows(r), cols(c)
    {
        slots.resize(rows, std::vector<Slot>(cols));
    }
    bool addItem(int itemID, int quantity);
    bool removeItem(int itemID, int quantity);
    void printInventory();
};



#endif //INVENTORY_H
