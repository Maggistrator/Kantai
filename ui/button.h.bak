#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_draw.h>

struct int_Rect
{
    int x, y, w, h;
};

class Button
{
class StateBasedGame;
SDL_Surface* textsf;
TTF_Font *font = nullptr;
int_Rect offset;
SDL_Rect textpos;

public:
    void (onClick*)(StateBasedGame*, SDL_Event*);
    SDL_Rect bounds;
    SDL_Color color = { 255, 255, 255 };
    SDL_Color textColor = { 0, 0, 0 };
    const char text[128];

    Button()
    {
        font = TTF_OpenFont( "res/CharisSILR.ttf", 28 );
    }

    void render(SDL_Surface* s)
    {
        textsf = TTF_RenderText_Solid( font, text, textColor );
        alignText();
        //Draw_Rect(s, bounds.x, bounds.y, bounds.w, bounds.h, SDL_MapRGB(s->format, color.r, color.g, color.b));
        SDL_FillRect(s, &bounds, color);
        SDL_BlitSurface( textsf, nullptr, s, &textpos );
    }

    void update(StateBasedGame* g, SDL_Event* e)
    {}
в
private:
    void alignText()
    {
        TTF_SizeUTF8(font, &text, &(offset.w), &(offset.h);
        offset.x = (bounds.w - offset.w)/2;
        offset.y = bounds.y + (bounds.h - offset.h)/2;
        textpos.x = bounds.x + offset.x;
        textpos.y = bounds.y + offset.y;
        testpos.w = offset.w;
        testpos.h = offset.h;
    }

    void handleEvent( SDL_Event* e )
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
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
            }
            //Mouse is inside button
            else
            {
                //Set mouse over sprite
                switch( e->type )
                {
                    case SDL_MOUSEMOTION:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                    break;

                    case SDL_MOUSEBUTTONDOWN:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                    break;

                    case SDL_MOUSEBUTTONUP:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                    break;
                }
            }
        }
    }

    bool checkCollision( SDL_Rect a, SDL_Rect b )
    {
        bool c1 = a.x < (b.x + b.w); // самая левая точка одного прямоугольника не может быть правее самой правой точки другого
        bool c2 = (a.x + a.w) > b.x; // верно и обратное
        bool c3 = a.y < (b.y + b.h); // верхняя точка одного прямоугольника не может быть ниже нижней точки другого
        bool c4 = (a.y + a.h) > b.y; // и, соответственно, в обратном порядке это тоже так
        return ( c1 && c2 && c3 && c4 );
    }

public:
    ~Button()
    {
        TTF_CloseFont(font);
        SDL_FreeSurface(textsf);
    }
};
