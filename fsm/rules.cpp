#ifndef RULES
#define RULES

#include "states.h"
#include "FiniteStateMachine.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "../ui/button.h"

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
    char *text = "I hate SDL";

    SDL_Surface* textsf;
    TTF_Font *font = nullptr;
    int_Rect offset;
    SDL_Rect textpos;
    SDL_Rect bounds = { 0, 0, 600, 400 };
    SDL_Color text_color = { 255, 255, 255 };

public:
    void init( SDL_Surface* display, StateBasedGame* g )
    {
        back_button = new Button(backButtonOnClick, "Back");

        font = TTF_OpenFont( "res/CharisSILR.ttf", 12 );
        textsf = TTF_RenderUTF8_Solid(font, text, text_color);
        static_assert(sizeof(wchar_t) == sizeof(Uint16));

        for(int i = 0; i < 5; i++) {
            back_button->bounds.x = (display->w - back_button->bounds.w)/2;
            back_button->bounds.y = display->h - back_button->bounds.h - 50;
        }
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
        if(!textsf) {
        } else {
            alignText();
            SDL_BlitSurface(textsf, nullptr, display, &textpos);
        }
    }

    void alignText()
    {
        TTF_SizeUTF8(font, text, &(offset.w), &(offset.h));
        offset.x = (bounds.w - offset.w)/2;
        offset.y = (bounds.h - offset.h)/2;
        textpos.x = bounds.x + offset.x;
        textpos.y = bounds.y + offset.y;
        textpos.w = offset.w;
        textpos.h = offset.h;
    }

    SDL_Event* pollEvent( void )
    {
        return &event;
    }

    SDL_Surface* getScreen()
    {
        return screen;
    }

    void exit( void )
    {}
};

#endif // MAIN_MENU

