
#include "machine.h"
#include <iostream>

void Machine::LoadTextures(SDL_Renderer *r){
    machineTex = IMG_LoadTexture(r, "textures/machine_tex.png");
    for(int i=0; i<TYPE_COUNT;i++){
        std::string path = "textures/slot_tex" + std::to_string(i) +".png";
        const char* p = path.c_str();
        SDL_Texture *tex =  IMG_LoadTexture(r, p);
        slotTextures.push_back(tex);       
    }
}
void Machine::Build(int n, int X, int Y){
    x = X;
    y = Y;
    columns.clear();
    for(int i = 0; i<n; i++){
        Column *c = SetColumn(100, x+20+(20+SLOTW)*i, y+220);
        columns.push_back(c);  
    }
    running = true;
    
}
void Machine::DrawMachine(SDL_Renderer *r, TTF_Font *f){
    SDL_Rect *rect = new SDL_Rect;
    for(Column *c: columns){
        for(Slot *s: c->SetVisible()){
            rect->h = SLOTH*scale;
            rect->w = SLOTW*scale;
            rect->x = s->x*scale;
            rect->y = s->y*scale;
            SDL_RenderCopy(r, slotTextures[s->type], NULL, rect);
            
        }
    }
    rect->h = 520*scale;
    rect->w = 230*scale;
    rect->x = x*scale;
    rect->y = y*scale;
    SDL_RenderCopy(r, machineTex, NULL, rect);
    rect->h = 50*scale;
    rect->w = 100*scale;
    rect->x = x + 65;
    rect->y = y + 400*scale;
    std::string balanceStr = std::to_string(balance);
    const char* str = balanceStr.c_str();
    SDL_Surface *temp = TTF_RenderText_Solid(f, str, {255,255,255,255});
    SDL_Texture *tex = SDL_CreateTextureFromSurface(r, temp);
    SDL_FreeSurface(temp);
    SDL_RenderCopy(r, tex, NULL, rect);
}
void Machine::Spin(bool trytostop){
    if(running){
        int ready = 0;
        for(Column *c: columns){
             if(c->NextSpin(trytostop)){
                ready++;
             }
        }
        if(ready == 3){
            running = false;
        }
    }
}
bool Machine::DidWin(){
    if(running){
        return false;
    }
    int t = -1;
    for(Column *c: columns){
        Slot *s = c->CenterSlot();
        
        if(s==nullptr){
            return false;
        }
        if(t ==-1 ){
            t = s->type;
            
        }
        if(t!=s->type){
            balance -= bet;
            return false;
        }
    }
    balance += bet*10;
    return true;
}
bool Machine::IsRunning(){
    return running;
}
void Machine::SetBalance(double b){
    balance = b;
}
double Machine::GetBalance(){
    return balance;
}
void Machine::SetBet(double b){
    if(b<0.1){
        bet = 0.1;
        return;
    }
    bet = b;
    
}
double Machine::GetBet(){
    return bet;
}
