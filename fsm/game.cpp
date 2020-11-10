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

    SDL_Surface* textSurface;
    char* pointsLabel;
    TTF_Font *font = NULL;
    SDL_Color textColor = { 255, 255, 255 };
    SDL_Rect textPlaceholder = { 0, 50, 200, 50 };

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
        font = TTF_OpenFont( "res/CharisSILR.ttf", 28 );
        pointsLabel = new char[13];
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
        entityManager.update(g, this, delta);//StateBasedGame* g, GameState* state, int delta

        ControllableSubsystem* ss = (ControllableSubsystem*)player->getSubsystem(controllable);
        ld.torpedosSpawned = ss->torpedosSpawned;
        if(ld.torpedosSpawned > 10) nextLevel( g );
        if(ld.torpedosSpawned >= 10)
            if(next_level_cooldown <= 0) nextLevel( g );
            else next_level_cooldown--;
    }

    void render( SDL_Surface* display ) {
        SDL_BlitSurface( background, NULL, display, NULL );
        entityManager.render( display );
        sprintf(pointsLabel, "%s%d", "Points: ", ld.points);
        textSurface = TTF_RenderText_Solid( font, pointsLabel, textColor );
        SDL_BlitSurface( textSurface, nullptr, display, &textPlaceholder );
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

        current_session.current_player->scores.push_back(ld.points);

        for(int highscore: current_session.current_player->scores)
        {
            cout << "player: " << current_session.current_player->name << " points " << highscore << endl;
        }

        Statistics* s = new Statistics(ld.killed, ld.points);
        g->registerState( results, getScreen(), s );
        g->switchState ( results );

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
        TTF_CloseFont(font);
        SDL_FreeSurface( background );
    }
};


