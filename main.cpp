#include <thread>
#include <chrono>
#include "machine.h"
#include <iostream>
#include <fstream>


#define TICKS_PER_FRAME (300/60)
#define TICKS_PER_FLASH (50*TICKS_PER_FRAME)

const int W = 230, H = 520;
SDL_Window *window = nullptr;
Uint32 windowFlags = 0;
const char* windowTitle = "Slot Machine";

SDL_Renderer *renderer = nullptr;
Uint32 rendererFlags  = SDL_RENDERER_ACCELERATED;

TTF_Font *font = nullptr;

double lastWin;
int lastTime = 0, lastFlash = 0;
int timerStart = 0;
int spinTime;

bool stop = false;
bool ready = true;
bool played = true;
bool flashWinners = false;

std::ifstream input;
std::ofstream output;

Machine m;
std::vector<Slot> winners;


void CreateWindowAndRenderer(){
    window = SDL_CreateWindow(windowTitle, 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
    W, H, 
    windowFlags);
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);
}
void Init(){
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    font = TTF_OpenFont("C:/WINDOWS/FONTS/BAUHS93.TTF", 50);
    CreateWindowAndRenderer();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}
void Quit(){
    SDL_DestroyWindow(window);
    SDL_Quit();
    double balance = m.GetBalance();
    output.open("stats.txt");
    output<<balance;
    output.close();
}
bool EventProcessor(SDL_Event event){
    if(SDL_PollEvent(&event)){
            if(event.type==SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:{
                        return false;
                    }
                    case SDLK_SPACE:{
                        if(ready){
                            played = false;
                            stop = false;
                            m.Build(3, 0, 0);
                            timerStart = SDL_GetTicks64();
                            spinTime = rand()%1000+300;
                            ready = false;
                        }
                        break;
                    }
                    case SDLK_DOWN:{
                        m.SetBet(m.GetBet()-0.1);
                        break;
                    }
                    case SDLK_UP:{
                        m.SetBet(m.GetBet()+0.1);
                        break;
                    }
                    default:{
                        break;
                    }

            }
            if(event.type == SDL_QUIT){
                return false;
            }
        }
        
    }
    return true;
}
void RenderFrame(){
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if(!played){
        m.Spin(stop);
    }
    m.DrawMachine(renderer, font);
    if(!winners.empty()&&ready){
        if(flashWinners){
            SDL_Color color{100,255,200,130};
            m.ColorSlots(color ,winners, renderer);
        }
        std::string winStr = std::to_string(lastWin);
        winStr = winStr.substr(0, winStr.size()-4);
        char* winChar = &winStr[0];
        SDL_Surface *surf  = TTF_RenderText_Solid(font, winChar, {255,255,255,255});
        SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect rect{0, H/2-50, W, 50};
        SDL_RenderCopy(renderer, text, NULL, &rect);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(text);
    }
    
    SDL_RenderPresent(renderer);
    
}
void InitMachine(){
    double balance;
    input.open("stats.txt");
    input>>balance;
    input.close();
    m.Build(3, 0, 0);
    m.SetBalance(balance);
    m.SetBet(20.0);
    std::cout<<m.GetBalance()<<"\n";
    m.LoadTextures(renderer);
}
void Run(){
    Init();
    InitMachine();
    SDL_Event event;
    while(EventProcessor(event)){
        while(SDL_GetTicks64()-lastTime<TICKS_PER_FRAME){
            SDL_Delay(1);
        }
        
        if(lastTime-timerStart >=spinTime){
            stop = true;
        }
        if(lastTime-lastFlash >=TICKS_PER_FLASH/2){
            flashWinners = !flashWinners;
            lastFlash = lastTime;
        }
        RenderFrame();
        if(!m.IsRunning()&&stop&&!played){
            winners = m.DidWin(&lastWin);
            played = true; 
            ready = true;
        }
       
        lastTime = SDL_GetTicks64();
        
    }
    Quit();
}
int main(){
    Run();
    return EXIT_SUCCESS;
}