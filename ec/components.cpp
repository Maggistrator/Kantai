#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <SDL_ttf.h>

#include "../engine/aspect.h"
#include "../engine/subsystem.h"
#include "../utils.h"
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


///��������� ����������, � ������� �� ��������� - ��� ��������������, � ������ ������ ������� ������
class ControllableSubsystem : public Subsystem
{
    //Timer------------------------------------
    int cooldown = 500;
    unsigned int lastTime = 0, currentTime;
    //----------------------------------------
    SDL_Surface* textSurface;
    char* torpedosLabel;
    TTF_Font *font = NULL;
    SDL_Color textColor = { 0, 0, 0 };

    public:
    int torpedosSpawned = 0;

    void init(StateBasedGame* g, GameState* state, Engine* e, Entity* owner){
        font = TTF_OpenFont( "res/CharisSILR.ttf", 28 );
        torpedosLabel = new char[13];
    }

    void update(StateBasedGame* g, GameState* state, Engine* e, Entity* owner, int delta){
        SDL_Event event = *(state->pollEvent());
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && (currentTime > lastTime + cooldown)){
            Entity* torpedo = spawnTorpedo(g, state, e);
            e->addEntity(torpedo);
            torpedosSpawned++;
            lastTime = currentTime;
        }
        currentTime = SDL_GetTicks();
	}

	void render(SDL_Surface* s){
	    sprintf(torpedosLabel, "%s%d", "Torpedos: ", torpedosSpawned);
        textSurface = TTF_RenderText_Solid( font, torpedosLabel, textColor );
        SDL_BlitSurface( textSurface, nullptr, s, nullptr );
	}

	Aspect getAspect(){
        return controllable;
	}

	~ControllableSubsystem(){
	    delete torpedosLabel;
	    SDL_FreeSurface(textSurface);
        TTF_CloseFont( font );
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

class SelfDestroyableSubsystem : public Subsystem
{
    PositionSubsystem* position;

    public:
    SDL_Rect bounds = {0,0,0,0};

    void init(StateBasedGame* g, GameState* state, Engine* e, Entity* owner){
        position = (PositionSubsystem*)owner->getSubsystem(positioned);
    }

    void update(StateBasedGame* g, GameState* state, Engine* e, Entity* owner, int delta){
        if(position->x > bounds.x  ||
           position->x < -bounds.w ||
           position->y > bounds.y  ||
           position->y < -bounds.h) e->killEntity( owner );
	}

	Aspect getAspect(){
        return selfdestroyable;
	}
};
