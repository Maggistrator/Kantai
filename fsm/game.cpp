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
#include "../utils.h"
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
            cerr << "�� ������� ��������� SDL_image! �������: " << IMG_GetError() << endl;
        Entity* player = spawnPlayer(g, this, &entityManager);
        entityManager.addEntity(player);
    }

    void update( StateBasedGame* g, int delta ) {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) g->exit();
            else {
                Entity* player = entityManager.getAllPosessing(controllable).front();//����� �������� ����, ���������� ������� ������ ������� ������ ���������� �����������
                ControllableSubsystem* ss = (ControllableSubsystem*)player->getSubsystem(controllable);
                if(ss->torpedosSpawned % 10) //���� �� ���� �������� ����� ������� ������ - �������� ������� (� ��������, ������� �� �����)
                    clearLevel( player );
            }
        }

        spawnEnemies(g);
        entityManager.update(g, this, delta);//StateBasedGame* g, GameState* state, int delta
    }

    void render( SDL_Surface* display ) {
        SDL_BlitSurface( background, NULL, display, NULL );
        entityManager.render( display );
    }

    int spawnCooldown = 500;
    int spawnCounter = 0;
    void spawnEnemies(StateBasedGame* g){
        if(spawnCounter > 0)spawnCounter--;
        if(rand() % 10 == 0 && spawnCounter <= 0) {
            Entity* enemy = spawnBoat(g, this, &entityManager);
            entityManager.addEntity(enemy);
            spawnCounter = spawnCooldown;
        }
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


