#include "components.cpp"

#define PLAYER_IMAGE "res/player.png"
#define TORPEDO_IMAGE "res/torpedo.png"

#define BOAT_IMAGE "res/boat.png"

#define PLAYER_WIDTH 600
#define PLAYER_HEIGHT 100

#define BOAT_WIDTH 150
#define BOAT_HEIGHT 50

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
    moveMe->speed.y = -10;
    entity->addSubsystem( g, state, e, moveMe );

    SelfDestroyableSubsystem* killMe = new SelfDestroyableSubsystem();
    killMe->bounds.x = 0;
    killMe->bounds.y = 120;
    killMe->bounds.w = screen->w;
    killMe->bounds.h = screen->h;
    entity->addSubsystem( g, state, e, killMe );

    return entity;
}

static Entity* spawnBoat( StateBasedGame* g, GameState* state, Engine* e)
{
    Entity* entity = new Entity();
    SDL_Surface* screen = state->getScreen();

    PositionSubsystem* coords = new PositionSubsystem();
    coords->x = 0;
    coords->y = BOAT_HEIGHT+70;
    entity->addSubsystem( g, state, e, coords ); //StateBasedGame* g, GameState* state, Engine* e, Subsystem* s

    DrawableSubsystem* renderer = new DrawableSubsystem( BOAT_IMAGE, screen );
    entity->addSubsystem( g, state, e, renderer );

    MovableSubsystem* moveMe = new MovableSubsystem();
    moveMe->speed.x = 3;
    moveMe->speed.y = 0;
    entity->addSubsystem( g, state, e, moveMe );

    SelfDestroyableSubsystem* killMe = new SelfDestroyableSubsystem();
    killMe->bounds.x = 0;
    killMe->bounds.y = 0;
    killMe->bounds.w = screen->w;
    killMe->bounds.h = screen->h;
    entity->addSubsystem( g, state, e, killMe );

    return entity;
}
