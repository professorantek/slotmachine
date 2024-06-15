#ifndef MACHINE_H
#define MACHINE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "column.h"


class Machine{
    private:
        int x,y;
        double scale = 1.0, balance, bet;
        bool running;
        std::vector<Column*> columns;
        SDL_Texture *machineTex;   
        std::vector<SDL_Texture*> slotTextures;
    public:
        bool IsRunning();
        void LoadTextures(SDL_Renderer *r);
        void Build(int n, int X, int Y);
        void DrawMachine(SDL_Renderer *r, TTF_Font *f);
        void Spin(bool trytostop);
        bool DidWin();
        void SetBalance(double b);
        double GetBalance();
        void SetBet(double b);
        double GetBet();

};
#endif