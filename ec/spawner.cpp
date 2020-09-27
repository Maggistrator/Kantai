#include "components.cpp"

#define PLAYER_IMAGE "res/player.png"
#define TORPEDO_IMAGE "res/torpedo.png"

#define BOAT_IMAGE "res/boat.png"

#define PLAYER_WIDTH 600
#define PLAYER_HEIGHT 100

#define BOAT_WIDTH 150
#define BOAT_HEIGHT 50

#define TORPEDO_WIDTH 33
#define TORPEDO_HEIGHT 100

#define BOATS_COST 1

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
    coords->y = screen->h-TORPEDO_HEIGHT;
    entity->addSubsystem( g, state, e, coords ); //StateBasedGame* g, GameState* state, Engine* e, Subsystem* s

    DrawableSubsystem* renderer = new DrawableSubsystem( TORPEDO_IMAGE, screen );
    entity->addSubsystem( g, state, e, renderer );

    MovableSubsystem* moveMe = new MovableSubsystem();
    moveMe->speed = {0, -10};
    entity->addSubsystem( g, state, e, moveMe );

    SelfDestroyableSubsystem* killMe = new SelfDestroyableSubsystem();
    killMe->bounds = {0, 120, screen->w, screen->h};
    entity->addSubsystem( g, state, e, killMe );

    CollidableSubsystem* collider = new CollidableSubsystem();
    collider->hitbox = {0, 0, TORPEDO_WIDTH, TORPEDO_HEIGHT};
    entity->addSubsystem( g, state, e, collider);

    return entity;
}

static Entity* spawnBoat( StateBasedGame* g, GameState* state, Engine* e, int* pointsCounter )
{
    Entity* entity = new Entity();
    SDL_Surface* screen = state->getScreen();

    PositionSubsystem* coords = new PositionSubsystem();
    coords->x = 0;
    coords->y = BOAT_HEIGHT+90;
    entity->addSubsystem( g, state, e, coords ); //StateBasedGame* g, GameState* state, Engine* e, Subsystem* s

    DrawableSubsystem* renderer = new DrawableSubsystem( BOAT_IMAGE, screen );
    entity->addSubsystem( g, state, e, renderer );

    MovableSubsystem* moveMe = new MovableSubsystem();
    moveMe->speed = {3, 0};
    entity->addSubsystem( g, state, e, moveMe );

    SelfDestroyableSubsystem* killMe = new SelfDestroyableSubsystem();
    killMe->bounds={0,0,screen->w-BOAT_WIDTH/2, screen->h};
    entity->addSubsystem( g, state, e, killMe );

    CollidableSubsystem* collider = new CollidableSubsystem();
    collider->hitbox = {0, 0, BOAT_WIDTH, BOAT_HEIGHT};
    entity->addSubsystem( g, state, e, collider);

    PointsSubsystem* valuable = new PointsSubsystem(BOATS_COST, pointsCounter);
    entity->addSubsystem( g, state, e, valuable);

    return entity;
}
