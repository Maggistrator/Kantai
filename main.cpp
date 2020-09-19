//INCLUDES--------------------
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>

#include "fsm/FiniteStateMachine.h"
#include "fsm/game.cpp"
#include "utils.h"

//PREPROCESSOR DEFINES--------
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_FPS 60
#define SCREEN_TICKS_PER_FRAME (1000 / SCREEN_FPS)

#define STATE_GAME 1

using namespace std;

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
    int fps = 0;
    uint32_t frame = 0;
    int running = 1;
    DeltaCounter dc;
    Timer fps_counter;

    //FPS label -------------
    SDL_Surface* textSurface;
    char* fpsLabel;
    TTF_Font *font = NULL;
    SDL_Rect place4fps = {0,50,200,50};
    SDL_Color textColor = { 0, 0, 0 };
    font = TTF_OpenFont( "res/CharisSILR.ttf", 28 );
    fpsLabel = new char[13];

    StateBasedGame game( &running );

    //Game state
    Game* gamestate = new Game();
    game.registerState( STATE_GAME, screen, gamestate );//int id, SDL_Surface* display, GameState* state
    game.switchState( STATE_GAME );

    while ( running ){
        fps_counter.start();
        dc.tick();
        frame++;
        fps = frame % FRAMES_PER_SECOND;

        SDL_FillRect(screen, NULL, 0x000000);
        game.update( dc.delta );
        game.render( screen );

        sprintf(fpsLabel, "%s%d", "FPS: ", fps);
        textSurface = TTF_RenderText_Solid( font, fpsLabel, textColor );
        SDL_BlitSurface( textSurface, nullptr, screen, &place4fps );
        //Sleep the remaining frame time
        if(fps_counter.get_ticks() < 1000 / FRAMES_PER_SECOND )
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps_counter.get_ticks() );

        SDL_Flip(screen);
    }

    SDL_FreeSurface(screen);

    return 0;
}
