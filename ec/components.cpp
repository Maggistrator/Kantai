#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

#include "../engine/aspect.h"
#include "../engine/subsystem.h"
#include "../engine/png_load.h"
#include "../engine/entity.h"

using namespace std;

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
