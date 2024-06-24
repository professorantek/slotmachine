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
        int type = TYPE_COUNT-1;
        int chance = pow(2, TYPE_COUNT/2+1);
        while(rand()%chance!=0&&chance!=1&&type>0){
            type--;
            if(type%2){
                chance/=2;   
            }   
        }
        s->type = type;
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
bool compareByY(const Slot &a, const Slot &b){
    return a.y>b.y;
}
std::vector<Slot> Column::WinningSlots(){
    std::vector<Slot> wS;
    for(Slot *s : slots){
        if(abs(s->y-cY)<=(SLOTH)){
            wS.push_back(*s);
        }
    }
    if(wS.size()==3){
        running = false;
        std::sort(wS.begin(), wS.end(), compareByY);
    }  
    return wS;
}

bool Column::IsRunning(){
    return running;
}