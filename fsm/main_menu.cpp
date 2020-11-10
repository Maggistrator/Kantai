#ifndef MAIN_MENU
#define MAIN_MENU

#include "states.h"
#include "FiniteStateMachine.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "../ui/button.h"

#define b_newgame   buttons[0]
#define b_profiles  buttons[1]
#define b_highs     buttons[2]
#define b_rules     buttons[3]
#define b_exit      buttons[4]

#define BUTTON_OFFSET 5

using namespace std;

    static void gameOnClick(StateBasedGame* g , SDL_Event* e, GameState* owner)
    {
        g->switchState(states::game);
    }

    static void profilesOnClick(StateBasedGame* g , SDL_Event* e, GameState* owner)
    {
        g->switchState(states::game);
    }

    static void highsOnClick(StateBasedGame* g , SDL_Event* e, GameState* owner)
    {
        g->switchState(states::highscores);
    }

    static void rulesOnClick(StateBasedGame* g , SDL_Event* e, GameState* owner)
    {
        g->switchState(states::rules);
    }

    static void exitOnClick(StateBasedGame* g , SDL_Event* e, GameState* owner)
    {
        g->exit();
    }

class MainMenu : public GameState
{
    SDL_Event event;
    SDL_Surface* screen;
    Button* buttons[5];

public:

    void init( SDL_Surface* display, StateBasedGame* g )
    {
        screen = display;
        b_newgame = new Button(gameOnClick, "Новая игра");
        b_profiles = new Button(profilesOnClick, "Профили");
        b_highs = new Button(highsOnClick, "Рекорды");
        b_rules = new Button(rulesOnClick, "Правила");
        b_exit = new Button(exitOnClick, "Выход");

        for(int i = 0; i < 5; i++) {
            buttons[i]->bounds.x = (display->w - buttons[i]->bounds.w)/2;
            buttons[i]->bounds.y = (display->h - (buttons[i]->bounds.h + BUTTON_OFFSET) * 5)/2  + ((buttons[i]->bounds.h + BUTTON_OFFSET) * i);
        }
    }

    void update( StateBasedGame* g, int delta )
    {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) g->exit();
            for(int i = 0; i < 5; i++) buttons[i]->update(g, &event, this);
        }
    }

    void render( SDL_Surface* display )
    {
        for(int i = 0; i < 5; i++) buttons[i]->render(display);
    }

    SDL_Event* pollEvent( void )
    {
        return &event;
    }

    SDL_Surface* getScreen()
    {
        return screen;
    }

};

#endif // MAIN_MENU
