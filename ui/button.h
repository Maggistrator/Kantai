#ifndef BUTTON
#define BUTTON

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_draw.h>
#include "../fsm/FiniteStateMachine.h"

#define ACTIVE_COLOR { 0, 255, 0 }
#define CLICK_COLOR { 0, 0, 255 }

/*
    ������������� � �������������� ������������ � ���������
    SDL_Rect �����-�� ��������� UInt16 � ����� � ��� ����������� ���������� � SDL_ttf
*/
struct int_Rect
{
    int x, y, w, h;
};

///����� ������. ������������ ����, ��������� ����������� ��� ������� ������� �� ����.
class Button
{
//class StateBasedGame;
// ����� ��������� ����� ������������� ����� ������ ----------
SDL_Surface* textsf;
TTF_Font *font = nullptr;
int_Rect offset;
SDL_Rect textpos;
//------------------------------------------------------------
SDL_Color color = { 255, 255, 255 };
SDL_Color backup_color = color;

public:
    void (*onClick)(StateBasedGame*, SDL_Event*);  // �������-���������� �����
    SDL_Rect bounds = { 0, 0, 200, 30 };
    SDL_Color text_color = { 0, 0, 0 };
    char *text = nullptr;

    /// ����� ������� ������, �� ������������ �������, ������� ��� ����� ���������
    Button()
    {
        font = TTF_OpenFont( "res/CharisSILR.ttf", 28 );
    }

    /// ������������� ������ ��������, ������� ��� ���������
    Button( void (*callback)(StateBasedGame* e, SDL_Event* g), char* myText )
    {
        text = myText;
        onClick = callback;
        font = TTF_OpenFont( "res/CharisSILR.ttf", 18 );
    }

    void render(SDL_Surface* s)
    {
        SDL_FillRect(s, &bounds, SDL_MapRGB(s->format, color.r, color.g, color.b));
        if(text != nullptr) std::cout << text << std::endl;
        if(!(textsf = TTF_RenderUTF8_Solid(font, text, text_color))) {
        } else {
            //textpos = bounds;
            alignText();
            SDL_BlitSurface(textsf,nullptr,s,&textpos);
            SDL_FreeSurface(textsf);
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
        offset.y = (bounds.h - offset.h)/2;
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
