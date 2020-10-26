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
#include "fsm/states.h"

//PREPROCESSOR DEFINES--------
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_FPS 60
#define SCREEN_TICKS_PER_FRAME (1000 / SCREEN_FPS)


//#define DEBUG 1

using namespace std;

int main(int argc, char* argv[])
{
    //----------------------------INITALISING LIBRARIES-----------------------------------//
    SDL_Surface* screen;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "�� ������� ��������� SDL! �������: " << SDL_GetError() << endl;
        return 1;
    } else atexit (SDL_Quit);

    if (TTF_Init()) {
        cerr << "�� ������� ��������� SDL_ttf! �������: " << TTF_GetError() << endl;
        return 2;
    } else atexit (TTF_Quit);

    if(!(screen=SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_ANYFORMAT)))
    {
        cerr << "�� ������� ������� ����! �������: " << SDL_GetError() << endl;
        return 88;
    }

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        SDL_FreeSurface(screen);
        cerr << "�� ������� ��������� SDL_image! �������: " << IMG_GetError() << endl;
    } else atexit (IMG_Quit);

    SDL_SetAlpha(screen, SDL_SRCALPHA , SDL_ALPHA_OPAQUE);

    freopen("CON", "w", stdout); // redirects stdout
    freopen("CON", "w", stderr); // redirects stderr

    //--------------------------------MAIN GAME CYCLE-------------------------------//
    int fps = 0;
    uint32_t frame = 0;
    int running = 1;
    DeltaCounter dc;
    Timer fps_counter;

    #ifdef DEBUG
    //FPS label -------------
    SDL_Surface* textSurface;
    char* fpsLabel;
    TTF_Font *font = NULL;
    SDL_Rect place4fps = {0,50,200,50};
    SDL_Color textColor = { 0, 0, 0 };
    font = TTF_OpenFont( "res/CharisSILR.ttf", 28 );
    fpsLabel = new char[13];
    #endif // DEBUG

    StateBasedGame game( &running );

    //Game state
    Game* gamestate = new Game();
    game.registerState( states::game, screen, gamestate );//int id, SDL_Surface* display, GameState* state
    game.switchState( states::game );

    while ( running ){
        fps_counter.start();
        dc.tick();
        frame++;
        fps = frame % FRAMES_PER_SECOND;

        SDL_FillRect(screen, NULL, 0x000000);
        game.update( dc.delta );
        game.render( screen );

        #ifdef DEBUG
        sprintf(fpsLabel, "%s%d", "FPS: ", fps);
        textSurface = TTF_RenderText_Solid( font, fpsLabel, textColor );
        SDL_BlitSurface( textSurface, nullptr, screen, &place4fps );
        #endif // DEBUG

        //Sleep the remaining frame time
        if(fps_counter.get_ticks() < 1000 / FRAMES_PER_SECOND )
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps_counter.get_ticks() );

        SDL_Flip(screen);
    }

    SDL_FreeSurface(screen);

    return 0;
}
