#ifndef LOGIN
#define LOGIN

#include "states.h"
#include "FiniteStateMachine.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "../ui/button.h"
#include "../ui/label.h"
#include "../sessions.h"
#include "../ui/text_field.h"

using namespace std;

    static void loginOnClick(StateBasedGame* g , SDL_Event* e);

class Login : public GameState
{
    SDL_Event event;
    SDL_Surface* screen;
    Button* back_button;
    Label* text;
public:
    static TextField tf;

    void init( SDL_Surface* display, StateBasedGame* g )
    {
        screen = display;
        back_button = new Button(loginOnClick, "Продолжить");
        back_button->bounds.x = (display->w - back_button->bounds.w)/2;
        back_button->bounds.y = display->h - back_button->bounds.h - 50;

        text = new Label("Введите имя:");
        text->bounds.w = 300;
        text->bounds.x = (screen->w - text->bounds.w)/2;
        text->bounds.y = screen->h/2-50;

        tf.bounds.w = 300;
        tf.bounds.x = (screen->w - tf.bounds.w)/2;
        tf.bounds.y = screen->h/2;
    }

    void update( StateBasedGame* g, int delta )
    {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) g->exit();
            tf.update(event);
        }
        back_button->update(g, &event);
    }

    void render( SDL_Surface* display )
    {
        back_button->render(display);
        text->render(display);
        tf.render(display);
    }

    SDL_Event* pollEvent( void )
    {
        return &event;
    }

    SDL_Surface* getScreen()
    {
        return screen;
    }

    ~Login()
    {
        delete back_button;
        delete text;
    }
};

#ifndef LOGIN_ON_CLICK
#define LOGIN_ON_CLICK
    static void loginOnClick(StateBasedGame* g , SDL_Event* e)
    {
        Player* player = new Player((Login::tf).text);
        current_session.players.push_back(player);
        current_session.current_player = player;
        g->switchState(states::main_menu);
    }
#endif // LOGIN_ON_CLICK

#endif // LOGIN

