#include "column.h"
#include <algorithm>
#include <iostream>


Column* SetColumn(int n, int x, int y){
    Column *c = new Column;
    std::vector<Slot*> slots;
    for(int i = 0; i<n; i++){
        Slot *s = new Slot;
        s->x = x;
        s->y = y-(SLOTH*n/2)+SLOTH*i;
        s->type = rand()%TYPE_COUNT;
        slots.push_back(s);
    }
    c->Set(n, x, y , slots);

    return c;
}
void Column::Set(int n, int x, int y, std::vector<Slot*> s){
    count = n;
    cX = x;
    cY = y;
    slots = s;
}
std::vector <Slot*> Column::SetVisible(){
    std::vector<Slot*> v;
    for(int i = 0; i<count; i++){
        if(abs(slots[i]->y-cY)<=3*SLOTH){
            v.push_back(slots[i]);
        }
    }
    return v;
}
bool Column::NextSpin(bool trytostop){
    if(forcestop){
        return true;
    }
    bool CANSTOP = false;
    if(trytostop){
        speed -= rand()%5;
        if(speed<1){
            speed = 1;
        }
    }
    for(Slot *s: slots){
        s->y = (s->y+speed);
        if(s->y>cY+count/2*SLOTH){
            s->y = s->y-count*SLOTH; 
        }
        if(s->y==cY&&trytostop){
            CANSTOP = true;
            forcestop = true;
        } 
    }
    
    return CANSTOP;
}
Slot* Column::CenterSlot(){
    for(Slot *s : slots){
        if(s->y == cY){
            running = false;
            return s;
        }
    }
    return nullptr;
}

bool Column::IsRunning(){
    return running;
}