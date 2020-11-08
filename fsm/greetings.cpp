#ifndef GREETINGS
#define GREETINGS

#include "states.h"
#include "FiniteStateMachine.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "../ui/label.h"
#include "../ui/text_field.h"

using namespace std;

class Greetings : public GameState
{
    SDL_Event event;
    SDL_Surface* screen;
    Label* game_name;
    Label* dev_name;

public:
    void init( SDL_Surface* display, StateBasedGame* g )
    {
        screen = display;
        game_name = new Label("МОРСКОЙ БОЙ");
        dev_name = new Label("Денис Левихин, И594");

        game_name->alignment = Label::Align::CENTER;
        dev_name->alignment = Label::Align::CENTER;

        game_name->bounds.w = 300;
        game_name->setFont("res/CharisSILR.ttf", 24);
        game_name->bounds.x = (screen->w - game_name->bounds.w)/2;
        game_name->bounds.y = screen->h/2 - 50;

        dev_name->bounds.w = 300;
        dev_name->bounds.x = (screen->w - dev_name->bounds.w)/2;
        dev_name->bounds.y = screen->h/2;
    }

    const int waitAbit = 60;
    int timer = 0;
    void update( StateBasedGame* g, int delta )
    {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) g->exit();
        }
        if(timer < waitAbit) timer++;
        else g->switchState(states::login);
    }

    void render( SDL_Surface* display )
    {
        game_name->render(display);
        dev_name->render(display);
    }

    SDL_Event* pollEvent( void )
    {
        return &event;
    }

    SDL_Surface* getScreen()
    {
        return screen;
    }

    ~Greetings()
    {
        delete game_name;
        delete dev_name;
    }
};

#endif // GREETINGS

