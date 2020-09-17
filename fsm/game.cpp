/*
    FINITE STATE MACHINE: Game
*/

//--------INCLUDES-----------------//
#include <string>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "../engine/engine.cpp"
#include "../engine/utils.h"
//--------RESOURCES----------------//
#define BACKGROUND_FILE_PATH "res/sea.png"



/// Состояние игрового процесса
class Game
{
    SDL_Surface* background;
    Engine entityManager;

    public:
    void init( SDL_Surface* display ) {
        background = loadSurface( BACKGROUND_FILE_PATH, display );
        if( background == NULL ) printf( "Failed to load PNG image!\n" );

        //load background*
        //init engine*

    }

    int update() {

    }

    void render( SDL_Surface* display ) {
        SDL_BlitSurface( background, NULL, display, NULL );
    }

    /// Вызывается как антипод init(), завершает игру
    void stop () {

    }
};


