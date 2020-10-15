#ifndef SPAWNER
#define SPAWNER

#include "components.cpp"

#define PLAYER_IMAGE "res/player.png"
#define TORPEDO_IMAGE "res/torpedo.png"
#define BOAT_IMAGE "res/boat.png"
#define CARGO_IMAGE "res/cargo.png"
#define BATTLESHIP_IMAGE "res/battleship.png"

#define PLAYER_WIDTH 600
#define PLAYER_HEIGHT 100

#define BOAT_WIDTH 150
#define BOAT_HEIGHT 50
#define BASIC_BOAT_SPEED 2

#define CARGO_WIDTH 120
#define CARGO_HEIGHT 30
#define BASIC_CARGO_SPEED 3

#define BATTLESHIP_WIDTH 150
#define BATTLESHIP_HEIGHT 63
#define BASIC_BATTLESHIP_SPEED 5

#define TORPEDO_WIDTH 33
#define TORPEDO_HEIGHT 100

#define BOAT_COST 1
#define CARGO_COST 2
#define BATTLESHIP_COST 3

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
    collider->hitbox = {0, 0, TORPEDO_WIDTH, TORPEDO_WIDTH};
    entity->addSubsystem( g, state, e, collider);

    return entity;
}

static Entity* spawnBoat( StateBasedGame* g, GameState* state, Engine* e, int* pointsCounter, int* killsCounter, int level)
{
    Entity* entity = new Entity();
    SDL_Surface* screen = state->getScreen();

    PositionSubsystem* coords = new PositionSubsystem();
    coords->x = 0;
    coords->y = BOAT_HEIGHT+140;
    entity->addSubsystem( g, state, e, coords ); //StateBasedGame* g, GameState* state, Engine* e, Subsystem* s

    DrawableSubsystem* renderer = new DrawableSubsystem( BOAT_IMAGE, screen );
    entity->addSubsystem( g, state, e, renderer );

    PointsSubsystem* valuable = new PointsSubsystem(BOAT_COST, killsCounter, pointsCounter);
    entity->addSubsystem( g, state, e, valuable);

    MovableSubsystem* moveMe = new MovableSubsystem();
    moveMe->speed = {BASIC_BOAT_SPEED + (level * BASIC_BOAT_SPEED/10), 0};//с каждым уровнем, скорость лодки растет на 20%
    entity->addSubsystem( g, state, e, moveMe );

    CollidableSubsystem* collider = new CollidableSubsystem();
    collider->hitbox = {0, BOAT_HEIGHT/2, BOAT_WIDTH, BOAT_HEIGHT/2};
    entity->addSubsystem( g, state, e, collider);

    SelfDestroyableSubsystem* killMe = new SelfDestroyableSubsystem();
    killMe->bounds={0,0,screen->w-BOAT_WIDTH/2, screen->h};
    entity->addSubsystem( g, state, e, killMe );

    return entity;
}

static Entity* spawnCargo( StateBasedGame* g, GameState* state, Engine* e, int* pointsCounter,  int* killsCounter, int level)
{
    Entity* entity = new Entity();
    SDL_Surface* screen = state->getScreen();

    PositionSubsystem* coords = new PositionSubsystem();
    coords->x = 0;
    coords->y = CARGO_HEIGHT+140;
    entity->addSubsystem( g, state, e, coords ); //StateBasedGame* g, GameState* state, Engine* e, Subsystem* s

    DrawableSubsystem* renderer = new DrawableSubsystem( CARGO_IMAGE, screen );
    entity->addSubsystem( g, state, e, renderer );

    PointsSubsystem* valuable = new PointsSubsystem(CARGO_COST, killsCounter, pointsCounter);
    entity->addSubsystem( g, state, e, valuable);

    MovableSubsystem* moveMe = new MovableSubsystem();
    moveMe->speed = {BASIC_CARGO_SPEED + (level*BASIC_CARGO_SPEED/10), 0};//скорость торговорго корабля растет на 30%
    entity->addSubsystem( g, state, e, moveMe );

    CollidableSubsystem* collider = new CollidableSubsystem();
    collider->hitbox = {0, 0, CARGO_WIDTH, CARGO_HEIGHT};
    entity->addSubsystem( g, state, e, collider);

    SelfDestroyableSubsystem* killMe = new SelfDestroyableSubsystem();
    killMe->bounds={0,0,screen->w-CARGO_WIDTH/2, screen->h};
    entity->addSubsystem( g, state, e, killMe );

    return entity;
}

static Entity* spawnBattleship( StateBasedGame* g, GameState* state, Engine* e, int* pointsCounter, int* killsCounter, int level)
{
    Entity* entity = new Entity();
    SDL_Surface* screen = state->getScreen();

    PositionSubsystem* coords = new PositionSubsystem();
    coords->x = 0;
    coords->y = BATTLESHIP_HEIGHT+45;
    entity->addSubsystem( g, state, e, coords ); //StateBasedGame* g, GameState* state, Engine* e, Subsystem* s

    DrawableSubsystem* renderer = new DrawableSubsystem( BATTLESHIP_IMAGE, screen );
    entity->addSubsystem( g, state, e, renderer );

    PointsSubsystem* counter = new PointsSubsystem(BATTLESHIP_COST, killsCounter, pointsCounter);
    entity->addSubsystem( g, state, e, counter);

    MovableSubsystem* moveMe = new MovableSubsystem();
    moveMe->speed = {BASIC_BATTLESHIP_SPEED + (level*BASIC_BATTLESHIP_SPEED/10), 0};//скорость боевого корабля растет на 50% за уровень
    entity->addSubsystem( g, state, e, moveMe );

    CollidableSubsystem* collider = new CollidableSubsystem();
    collider->hitbox = {0, BATTLESHIP_HEIGHT/2, BATTLESHIP_WIDTH, BATTLESHIP_HEIGHT/2};
    entity->addSubsystem( g, state, e, collider);

    SelfDestroyableSubsystem* killMe = new SelfDestroyableSubsystem();
    killMe->bounds={0,0,screen->w-BATTLESHIP_WIDTH/2, screen->h};
    entity->addSubsystem( g, state, e, killMe );

    return entity;
}

#endif // SPAWNER
