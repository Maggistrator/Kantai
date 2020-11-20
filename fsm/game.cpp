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
#include "../sessions.h"
#include "finish_game.cpp"
#include "states.h"
#include "../ui/label.h"
//--------RESOURCES----------------//
#define BACKGROUND_FILE_PATH "res/sea.png"

using namespace std;

class Game : public GameState
{
    SDL_Event event;
    SDL_Surface* display;
    SDL_Surface* background;
    Engine entityManager;
    friend class Statistics;

    Label* pointsLabel;

    struct levelData {
        int level = 1;
        int points = 0;
        int torpedosSpawned = 0;
        int killed = 0;
    } ld;
    int spawnCooldown = 120;
    int spawnCounter = 0;
    Entity* player = nullptr;

    int next_level_cooldown = 60;
    void init( SDL_Surface* display, StateBasedGame* g ) {
        srand (time(NULL));
        this->display = display;
        background = loadOptimisedSurface( BACKGROUND_FILE_PATH, display );
        pointsLabel = new Label("Points: 0");
        pointsLabel->setFont("res/CharisSILR.ttf", 28 );
        pointsLabel->bounds.y = 50;
        player = spawnPlayer(g, this, &entityManager);
        entityManager.addEntity(player);
    }

    void update( StateBasedGame* g, int delta ) {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) g->exit();
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
                clearLevel( g );
                g->switchState(states::main_menu);
            }
        }

        spawnEnemies(g);
        entityManager.update(g, this, delta);

        ControllableSubsystem* ss = (ControllableSubsystem*)player->getSubsystem(controllable);
        ld.torpedosSpawned = ss->torpedosSpawned;
        if(ld.torpedosSpawned >= 10)
            if(next_level_cooldown <= 0) nextLevel( g );
            else next_level_cooldown--;
    }

    void render( SDL_Surface* display ) {
        SDL_BlitSurface( background, NULL, display, NULL );
        entityManager.render( display );
        char * newtext = new char[32];
        sprintf(newtext, "%s%d", "Points: ", ld.points);
        delete pointsLabel->text;
        pointsLabel->setText(newtext);
        pointsLabel->render(display);
    }

    void spawnEnemies(StateBasedGame* g){
        if(spawnCounter > 0)spawnCounter--;
        if(rand() % 2 == 0 && spawnCounter <= 0 && next_level_cooldown == 60) {
            Entity* enemy;
            switch (rand() % 3 + 1)
            {
                case 1:
                enemy = spawnBoat(g, this, &entityManager, &ld.points, &(ld.killed), ld.level);
                break;
                case 2:
                enemy = spawnCargo(g, this, &entityManager, &ld.points, &(ld.killed), ld.level);
                break;
                case 3:
                enemy = spawnBattleship(g, this, &entityManager, &ld.points, &(ld.killed), ld.level);
                break;
            }
            entityManager.addEntity(enemy);
            spawnCounter = spawnCooldown;
        }
    }

    void clearLevel( StateBasedGame* g ){
        list<Entity*> extraEntities = entityManager.getAllPosessing(positioned);
        for(Entity* e: extraEntities) entityManager.killEntity(e);

        next_level_cooldown = 60;
        ld.torpedosSpawned = 0;
        ld.killed = 0;
        ld.points = 0;

        player = spawnPlayer(g, this, &entityManager);
        entityManager.addEntity(player);
    }

    void nextLevel( StateBasedGame* g ){
        if(ld.killed == 10) ld.level++;
        else ld.level = 1;

        Statistics* s = new Statistics(ld.killed, ld.points);
        g->registerState( results, getScreen(), s );
        g->switchState ( results );

        current_session.addRecord(current_session.current_player, ld.points);

        clearLevel( g );
    }

    SDL_Event* pollEvent( void ) {
        return &event;
    }

    SDL_Surface* getScreen() {
        return display;
    }

    ~Game(){
        delete pointsLabel;
        SDL_FreeSurface( background );
    }
};


