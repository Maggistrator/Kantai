//--------INCLUDES-----------------//
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "FiniteStateMachine.h"
#include "../engine/engine.cpp"
//--------RESOURCES----------------//
#define BACKGROUND_FILE_PATH "res/sea.png"


class Game : public GameState
{
    SDL_Surface* background;
    Engine entityManager;
    SDL_Event event;

    public:
    void init( SDL_Surface* display, StateBasedGame* g ) {
        background = loadOptimisedSurface( BACKGROUND_FILE_PATH, display );
        if( background == NULL )
            cerr << "Не удалось загрузить SDL_image! Причина: " << IMG_GetError() << endl;
        //load background*
        //init engine*
    }

    void update(  StateBasedGame* g, int delta ) {
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        g->exit();
    }

    void render( SDL_Surface* display ) {
        SDL_BlitSurface( background, NULL, display, NULL );
    }

    /// Вспомогательная функция загрузки PNG-изображений
    SDL_Surface* loadOptimisedSurface( std::string path , SDL_Surface* gScreenSurface )
    {
        SDL_Surface* optimizedSurface = NULL;
        SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
        if( loadedSurface == NULL )
            printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        else {
            //Convert surface to screen format
            optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
            if( optimizedSurface == NULL )
            {
                printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }
            //Get rid of old loaded surface
            SDL_FreeSurface( loadedSurface );
        }
        return optimizedSurface;
    }


    void exit(){
        SDL_FreeSurface( background );
    }
};


