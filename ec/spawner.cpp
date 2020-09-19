#include "components.cpp"

#define PLAYER_IMAGE "res/player.png"
#define TORPEDO_IMAGE "res/torpedo.png"

#define PLAYER_WIDTH 600
#define PLAYER_HEIGHT 100


static Entity* spawnPlayer( StateBasedGame* g, GameState* state, Engine* e)
{
    Entity* entity = new Entity();
    SDL_Surface* screen = state->getScreen();

    PositionSubsystem* coords = new PositionSubsystem();
    coords->x = (screen->w - PLAYER_WIDTH)/2;
    coords->y = screen->h - PLAYER_HEIGHT;
    entity->addSubsystem( g, state, e, coords ); //StateBasedGame* g, GameState* state, Engine* e, Subsystem* s

    DrawableSubsystem* renderer = new DrawableSubsystem( PLAYER_IMAGE, screen );
    entity->addSubsystem( g, state, e, renderer );

    ControllableSubsystem* controller = new ControllableSubsystem();
    entity->addSubsystem( g, state, e, controller);

    return entity;
}

static Entity* spawnTorpedo( StateBasedGame* g, GameState* state, Engine* e)
{
    Entity* entity = new Entity();
    SDL_Surface* screen = state->getScreen();

    PositionSubsystem* coords = new PositionSubsystem();
    coords->x = screen->w/2;
    coords->y = screen->h;
    entity->addSubsystem( g, state, e, coords ); //StateBasedGame* g, GameState* state, Engine* e, Subsystem* s

    DrawableSubsystem* renderer = new DrawableSubsystem( TORPEDO_IMAGE, screen );
    entity->addSubsystem( g, state, e, renderer );

    MovableSubsystem* moveMe = new MovableSubsystem();
    moveMe->speed.x = 0;
    moveMe->speed.y = -1;
    entity->addSubsystem( g, state, e, moveMe );

    return entity;
}
