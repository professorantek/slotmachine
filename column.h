#ifndef COLUMN_H
#define COLUMN_H

#include "slot.h"
#include <random>
#include <vector>
class Column{
    private:
        int cX, cY, count, speed = 40;
        std::vector<Slot*> slots;
        bool running, forcestop = false;
    public:
        
        void Set(int n, int x, int y, std::vector<Slot*> s);
        std::vector<Slot*> SetVisible();
        std::vector<Slot> WinningSlots();
        bool NextSpin(bool trytostop);
        bool IsRunning();    

};
Column* SetColumn(int n, int x, int y);
bool compareByY(const Slot &a, const Slot &b);

#endif