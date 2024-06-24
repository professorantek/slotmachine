
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
    rect->x = x + 65*scale;
    rect->y = y + 400*scale;
    std::string balanceStr = std::to_string(balance);
    balanceStr = balanceStr.substr(0, balanceStr.size()-4);
    char* balanceChar = &balanceStr[0];  
    SDL_Surface *balanceSurf = TTF_RenderText_Solid(f, balanceChar, {255,255,255,255});
    SDL_Texture *balanceText = SDL_CreateTextureFromSurface(r, balanceSurf);
    SDL_RenderCopy(r, balanceText, NULL, rect);
    SDL_FreeSurface(balanceSurf);
    SDL_DestroyTexture(balanceText);


    rect->h = 50*scale;
    rect->w = 200*scale;
    rect->x = x + 15*scale;
    rect->y = y + 10*scale;
    std::string betStr = "Bet:" + std::to_string(bet);
    betStr = betStr.substr(0, betStr.size()-4);
    char* betChar = &betStr[0];
    SDL_Surface *betSurf = TTF_RenderText_Solid(f, betChar, {255,255,255,255});
    SDL_Texture *betText = SDL_CreateTextureFromSurface(r, betSurf);
    SDL_RenderCopy(r, betText, NULL, rect);
    SDL_FreeSurface(betSurf);
    SDL_DestroyTexture(betText);
    delete rect;
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
void Machine::ColorSlots(SDL_Color c, std::vector<Slot> v, SDL_Renderer *r){
    for(Slot s: v){
        SDL_Rect rect{s.x*scale, s.y*scale, SLOTW*scale, SLOTH*scale};
        SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(r, &rect);
    }
    SDL_RenderPresent(r);
}
std::vector<Slot> Machine::DidWin(double *amount){
    std::vector<Slot> empty, winners;
    if(running){
        return empty;
    }
    
    std::vector<std::vector<Slot>> t;
    for(Column *c: columns){
        t.push_back(c->WinningSlots());
    }
    *amount = 0;
    if(t[0][0].type==t[1][0].type&&t[0][0].type==t[2][0].type){
        *amount = bet*pow(2, t[0][0].type);
        winners = {t[0][0], t[1][0], t[2][0]};
        winners.push_back(t[0][0]);
        winners.push_back(t[1][0]);
        winners.push_back(t[2][0]);
    }
    if(t[0][1].type==t[1][1].type&&t[0][1].type==t[2][1].type){
        if(*amount!=0)*amount *= bet*pow(2, t[0][1].type);
        else *amount = bet*pow(2, t[0][1].type);
        winners.push_back(t[0][1]);
        winners.push_back(t[1][1]);
        winners.push_back(t[2][1]);
    }
    if(t[0][2].type==t[1][2].type&&t[0][2].type==t[2][2].type){
        if(*amount!=0)*amount *= bet*pow(2, t[0][2].type);
        else *amount = bet*pow(2, t[0][2].type);
        winners.push_back(t[0][2]);
        winners.push_back(t[1][2]);
        winners.push_back(t[2][2]);
    }
    balance += *amount;
    return winners;
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
