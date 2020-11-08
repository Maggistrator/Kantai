#ifndef RULES
#define RULES

#include "states.h"
#include "FiniteStateMachine.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "../ui/button.h"
#include "../ui/label.h"

using namespace std;

static void backButtonOnClick(StateBasedGame* g , SDL_Event* e)
{
    g->switchState(states::main_menu);
}

class Rules : public GameState
{
    SDL_Event event;
    SDL_Surface* screen;
    Button* back_button;
    Label* text;

public:
    void init( SDL_Surface* display, StateBasedGame* g )
    {
        screen = display;
        back_button = new Button(backButtonOnClick, "Back");
        back_button->bounds.x = (display->w - back_button->bounds.w)/2;
        back_button->bounds.y = display->h - back_button->bounds.h - 50;

        text = new Label(" Ненавижу, блять, SDL ");
        text->bounds.w = 300;
        text->bounds.x = (screen->w - text->bounds.w)/2;
        text->bounds.y = screen->h/2;
    }

    void update( StateBasedGame* g, int delta )
    {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) g->exit();
        }
        back_button->update(g, &event);
    }

    void render( SDL_Surface* display )
    {
        back_button->render(display);
        text->render(display);
    }

    SDL_Event* pollEvent( void )
    {
        return &event;
    }

    SDL_Surface* getScreen()
    {
        return screen;
    }

    ~Rules()
    {
        delete back_button;
        delete text;
    }
};

#endif // RULES

