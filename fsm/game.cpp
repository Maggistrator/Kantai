//--------INCLUDES-----------------//
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "../ec/spawner.cpp"
#include "FiniteStateMachine.h"
#include "../engine/engine.cpp"
#include "../engine/png_load.h"
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
        Entity* player = spawnPlayer(background, g, this, &entityManager);
        entityManager.addEntity(player);
    }

    void update(  StateBasedGame* g, int delta ) {
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        g->exit();

        entityManager.update(g, this, delta);//StateBasedGame* g, GameState* state, int delta
    }

    void render( SDL_Surface* display ) {
        SDL_BlitSurface( background, NULL, display, NULL );
        entityManager.render( display );
    }


    void exit(){
        SDL_FreeSurface( background );
    }
};


