#ifndef BUTTON
#define BUTTON

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_draw.h>
#include "../fsm/FiniteStateMachine.h"

#define ACTIVE_COLOR { 0, 255, 0 }
#define CLICK_COLOR { 0, 0, 255 }

/*
    Прямоугольник с целочисленными координатами и размерами
    SDL_Rect зачем-то ограничен UInt16 в связи с чем практически неюзабелен с SDL_ttf
*/
struct int_Rect
{
    int x, y, w, h;
};

///Класс кнопки. Обрабатывает клик, выполняет скормленную ему функцию реакции на него.
class Button
{
//class StateBasedGame;
// Набор приватных полей обслуживающих вывод текста ----------
SDL_Surface* textsf;
TTF_Font *font = nullptr;
int_Rect offset;
SDL_Rect textpos;
//------------------------------------------------------------
SDL_Color color = { 255, 255, 255 };
SDL_Color backup_color = color;

public:
    void (*onClick)(StateBasedGame*, SDL_Event*);  // функция-обработчик клика
    SDL_Rect bounds = {0, 0, 200, 30};
    SDL_Color textColor = { 0, 0, 0 };
    char *text = nullptr;

    /// можно создать кнопку, не устанавливая функцию, которую она будет выполнять
    Button()
    {
        font = TTF_OpenFont( "res/CharisSILR.ttf", 28 );
    }

    /// инициализация кнопки функцией, которую она выполняет
    Button( void (*callback)(StateBasedGame* e, SDL_Event* g), char* myText )
    {
        text = myText;
        onClick = callback;
        Button();
    }

    void render(SDL_Surface* s)
    {
        //Draw_Rect(s, bounds.x, bounds.y, bounds.w, bounds.h, SDL_MapRGB(s->format, color.r, color.g, color.b));
        SDL_FillRect(s, &bounds, SDL_MapRGB(s->format, color.r, color.g, color.b));
        if(!text){
            textsf = TTF_RenderText_Solid( font, text, textColor );
            alignText();
            SDL_BlitSurface( textsf, nullptr, s, &textpos );
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
                        color = ACTIVE_COLOR;
                    break;
                }
            }
        }
    }

private:
    void alignText()
    {
        TTF_SizeUTF8(font, text, &(offset.w), &(offset.h));
        offset.x = (bounds.w - offset.w)/2;
        offset.y = bounds.y + (bounds.h - offset.h)/2;
        textpos.x = bounds.x + offset.x;
        textpos.y = bounds.y + offset.y;
        textpos.w = offset.w;
        textpos.h = offset.h;
    }

public:
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
