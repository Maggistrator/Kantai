//INCLUDES--------------------
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>

#include "fsm/FiniteStateMachine.h"
#include "fsm/game.cpp"

//PREPROCESSOR DEFINES--------
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define STATE_GAME 1

using namespace std;

struct Clock
{
    uint32_t last_tick_time = 0;
    uint32_t delta = 0;

    void tick()
    {
        uint32_t tick_time = SDL_GetTicks();
        delta = tick_time - last_tick_time;
        last_tick_time = tick_time;
    }
};

int main(int argc, char* argv[])
{
    //----------------------------INITALISING LIBRARIES-----------------------------------//
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
        return 88;
    }

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        SDL_FreeSurface(screen);
        cerr << "Не удалось загрузить SDL_image! Причина: " << IMG_GetError() << endl;
    } else atexit (IMG_Quit);

    SDL_SetAlpha(screen, SDL_SRCALPHA , SDL_ALPHA_OPAQUE);
    //--------------------------------MAIN GAME CYCLE-------------------------------//
    int running = 1;
    Clock timer;

//    SDL_Flip(screen);
    StateBasedGame game( &running );

    //Game state
    Game* gamestate = new Game();
    game.registerState( STATE_GAME, screen, gamestate );//int id, SDL_Surface* display, GameState* state
    game.switchState( STATE_GAME );

    while ( running ){
        SDL_FillRect(screen, NULL, 0x000000);
        timer.tick();

        game.update( timer.delta );
        game.render( screen );

        SDL_Flip(screen);
    }

    SDL_FreeSurface(screen);

    return 0;
}
