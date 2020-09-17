#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using namespace std;

int main(int argc, char* argv[])
{
    SDL_Surface* screen;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Не удалось загрузить SDL! Причина: " << SDL_GetError() << endl;
        return 1;
    } else atexit (SDL_Quit);

    if (TTF_Init()) {
        cerr << "Не удалось загрузить SDL_ttf! Причина: " << TTF_GetError() << endl;
        return 2;
    } else atexit (TTF_Quit);

    if(!(screen=SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_ANYFORMAT)))
    {
        cerr << "Не удалось создать окно! Причина: " << SDL_GetError() << endl;
        return 3;
    }

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        SDL_FreeSurface(screen);
        cerr << "Не удалось загрузить SDL_ttf! Причина: " << IMG_GetError() << endl;
    } else atexit (IMG_Quit);

//    SDL_Flip(screen);

    SDL_Delay(2000);
    SDL_FreeSurface(screen);

    return 0;
}
