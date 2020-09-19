//--------INCLUDES-----------------//
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>
#include "../engine/aspect.h"
#include "../ec/spawner.cpp"
#include "FiniteStateMachine.h"
#include "../engine/engine.cpp"
#include "../engine/png_load.h"
//--------RESOURCES----------------//
#define BACKGROUND_FILE_PATH "res/sea.png"


class Game : public GameState
{
    int level = 1;
    SDL_Event event;
    SDL_Surface* display;
    SDL_Surface* background;
    Engine entityManager;

    public:
    void init( SDL_Surface* display, StateBasedGame* g ) {
        srand (time(NULL));
        this->display = display;
        background = loadOptimisedSurface( BACKGROUND_FILE_PATH, display );
        if( background == NULL )
            cerr << "Ќе удалось загрузить SDL_image! ѕричина: " << IMG_GetError() << endl;
        Entity* player = spawnPlayer(g, this, &entityManager);
        entityManager.addEntity(player);
    }

    void update( StateBasedGame* g, int delta ) {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) g->exit();
            else {
                Entity* player = entityManager.getAllPosessing(controllable).front();//игрок заведомо один, достаточно вызвать первый элемент списка полученных компонентов
                ControllableSubsystem* ss = (ControllableSubsystem*)player->getSubsystem(controllable);
                if(ss->torpedosSpawned % 10) //если на круг выпущено более дес€тка торпед - очистить уровень (и возможно, перейти на новый)
                    clearLevel( player );
            }
        }


        entityManager.update(g, this, delta);//StateBasedGame* g, GameState* state, int delta
    }

    void render( SDL_Surface* display ) {
        SDL_BlitSurface( background, NULL, display, NULL );
        entityManager.render( display );
    }

    void enemySpawner(){
        //iSecret = rand() % 10 + 1;
    }

    void clearLevel( Entity* player ){
        //for (Entity)
    }

    void nextLevel(){}

    SDL_Event* pollEvent( void ) {
        return &event;
    }

    SDL_Surface* getScreen() {
        return display;
    }

    void exit(){
        SDL_FreeSurface( background );
    }
};


