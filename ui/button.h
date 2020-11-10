#ifndef BUTTON
#define BUTTON

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_draw.h>
#include "../fsm/FiniteStateMachine.h"
#include "abstract_element.h"

#define ACTIVE_COLOR { 0, 255, 0 }
#define CLICK_COLOR { 0, 0, 255 }

///Класс кнопки. Обрабатывает клик, выполняет скормленную ему функцию реакции на него.
class Button : public AbstractUIElement
{
SDL_Color color = { 255, 255, 255 };
SDL_Color backup_color = color;
void* caller;


public:
    void (*onClick)(StateBasedGame*, SDL_Event*, void* );  // функция-обработчик клика

    /// можно создать кнопку, не устанавливая функцию, которую она будет выполнять
    Button(char* myText, void* caller=0)
    {
        this->caller = caller;
        text = myText;
        text_color = {0,0,0};
        font = TTF_OpenFont( "res/CharisSILR.ttf", 22 );
        textsf = TTF_RenderUTF8_Solid(font, text, text_color);
    }

    /// инициализация кнопки функцией, которую она выполняет
    Button( void (*callback)(StateBasedGame* e, SDL_Event* g, void*), char* myText, void* caller = 0)
    {
        text = myText;
        onClick = callback;
        this->caller = caller;
        text_color = {0,0,0};
        font = TTF_OpenFont( "res/CharisSILR.ttf", 22 );
        textsf = TTF_RenderUTF8_Solid(font, text, text_color);
    }

    void render(SDL_Surface* s)
    {
        SDL_FillRect(s, &bounds, SDL_MapRGB(s->format, color.r, color.g, color.b));
        if(textsf) {
            //textpos = bounds;
            alignText();
            SDL_BlitSurface(textsf, nullptr, s, &textpos);
        }
    }

    void update(StateBasedGame* g, SDL_Event* e)
    {
        if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
        {
            int x, y;
            SDL_GetMouseState( &x, &y );

            bool inside = true;

            if( x < bounds.x )
            {
                inside = false;
            }
            else if( x > bounds.x + bounds.w )
            {
                inside = false;
            }
            else if( y < bounds.y )
            {
                inside = false;
            }
            else if( y > bounds.y + bounds.h )
            {
                inside = false;
            }

            if( !inside )
            {
                color = backup_color;
            }
            else
            {
                switch( e->type )
                {
                    case SDL_MOUSEMOTION:
                        color = ACTIVE_COLOR;
                    break;

                    case SDL_MOUSEBUTTONDOWN:
                         color = CLICK_COLOR;
                         onClick(g, e, caller);
                    break;
                    case SDL_MOUSEBUTTONUP:
                         color = CLICK_COLOR;
                         onClick(g, e, caller);
                    break;
                }
            }
        }
    }

    void setColor(SDL_Color& new_color)
    {
        backup_color = new_color;
    }

    void setCallback (void (*callback)(StateBasedGame* e, SDL_Event* g, void*))
    {
        this->onClick = callback;
    }

    ~Button()
    {
        TTF_CloseFont(font);
        SDL_FreeSurface(textsf);
    }
};

#endif // BUTTON
