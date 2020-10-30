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

public:
    void (*onClick)(StateBasedGame*, SDL_Event*);  // функция-обработчик клика

    /// можно создать кнопку, не устанавливая функцию, которую она будет выполнять
    Button()
    {
        text_color = {0,0,0};
        font = TTF_OpenFont( "res/CharisSILR.ttf", 22 );
        textsf = TTF_RenderUTF8_Solid(font, text, text_color);
    }

    /// инициализация кнопки функцией, которую она выполняет
    Button( void (*callback)(StateBasedGame* e, SDL_Event* g), char* myText )
    {
        text = myText;
        onClick = callback;
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
        //If mouse event happened
        if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
        {
            //Get mouse position
            int x, y;
            SDL_GetMouseState( &x, &y );

            //Check if mouse is in button
            bool inside = true;

            //Mouse is left of the button
            if( x < bounds.x )
            {
                inside = false;
            }
            //Mouse is right of the button
            else if( x > bounds.x + bounds.w )
            {
                inside = false;
            }
            //Mouse above the button
            else if( y < bounds.y )
            {
                inside = false;
            }
            //Mouse below the button
            else if( y > bounds.y + bounds.h )
            {
                inside = false;
            }

            //Mouse is outside button
            if( !inside )
            {
                color = backup_color;
            }
            //Mouse is inside button
            else
            {
                //Set mouse over sprite
                switch( e->type )
                {
                    case SDL_MOUSEMOTION:
                        color = ACTIVE_COLOR;
                    break;

                    case SDL_MOUSEBUTTONDOWN:
                         color = CLICK_COLOR;
                         onClick(g, e);
                    break;
                    case SDL_MOUSEBUTTONUP:
                         color = CLICK_COLOR;
                         onClick(g, e);
                    break;
                }
            }
        }
    }

    void setColor(SDL_Color& new_color)
    {
        backup_color = new_color;
    }

    void setCallback (void (*callback)(StateBasedGame* e, SDL_Event* g))
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
