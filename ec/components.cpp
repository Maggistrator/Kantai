#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

#include "../engine/aspect.h"
#include "../engine/subsystem.h"
#include "../engine/png_load.h"
#include "../engine/entity.h"
#include "../engine/engine.cpp"

using namespace std;

//somehow neccesary prototype----------------------------------------------------//
static Entity* spawnTorpedo( StateBasedGame* g, GameState* state, Engine* e );   //
//-------------------------------------------------------------------------------//

class PositionSubsystem : public Subsystem
{
    public:
    int x=0, y=0;
    int speed;

    Aspect getAspect(){
        return positioned;
	}
};

class DrawableSubsystem : public Subsystem
{

    string source;
    SDL_Rect* bounds;
    SDL_Surface* image = nullptr;
    PositionSubsystem* position;

    public:
    DrawableSubsystem( string path, SDL_Surface* screen ) : source( path ) {
        image = loadOptimisedSurface( path, screen );
    }

    void init(StateBasedGame* g, GameState* state, Engine* e, Entity* owner){
        position = (PositionSubsystem*)owner->getSubsystem(positioned);
        bounds = new SDL_Rect();
        bounds->x = position->x;
        bounds->y = position->y;
        bounds->w = image->w;
        bounds->h = image->h;
    }

	void render(SDL_Surface* s){
        SDL_BlitSurface( image, nullptr, s, bounds );
	}

	void update(StateBasedGame* g, GameState* state, Engine* e, Entity* owner, int delta){
       bounds->x = position->x;
       bounds->y = position->y;
	}

	Aspect getAspect(){
        return drawable;
	}

	~DrawableSubsystem(){
        SDL_FreeSurface( image );
        delete bounds;
	}
};

class ControllableSubsystem : public Subsystem
{
    void update(StateBasedGame* g, GameState* state, Engine* e, Entity* owner, int delta){
        SDL_Event event = *(state->pollEvent());
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN){
            Entity* torpedo = spawnTorpedo(g, state, e);
            e->addEntity(torpedo);
        }
	}

	Aspect getAspect(){
        return controllable;
	}
};

class MovableSubsystem : public Subsystem
{
    PositionSubsystem* position;

    public:
    struct vector2d {
        int x, y;
    } speed = {0, 0};

    void init(StateBasedGame* g, GameState* state, Engine* e, Entity* owner){
        position = (PositionSubsystem*)owner->getSubsystem(positioned);
    }

    void update(StateBasedGame* g, GameState* state, Engine* e, Entity* owner, int delta){
        position->x += speed.x;
        position->y += speed.y;
	}

	Aspect getAspect(){
        return movable;
	}
};
