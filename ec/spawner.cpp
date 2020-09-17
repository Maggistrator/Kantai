#include "components.cpp"

#define PLAYER_IMAGE "res/player.png"
#define PLAYER_WIDTH 600
#define PLAYER_HEIGHT 100


static Entity* spawnPlayer( SDL_Surface* screen, StateBasedGame* g, GameState* state, Engine* e){
    Entity* entity = new Entity();

    PositionSubsystem* coords = new PositionSubsystem();
    coords->x = (screen->w - PLAYER_WIDTH)/2;
    coords->y = screen->h - PLAYER_HEIGHT;
    entity->addSubsystem( g, state, e, coords ); //StateBasedGame* g, GameState* state, Engine* e, Subsystem* s

    DrawableSubsystem* renderer = new DrawableSubsystem( PLAYER_IMAGE, screen );
    entity->addSubsystem( g, state, e, renderer );

    return entity;
}
