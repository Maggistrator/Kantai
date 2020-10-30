#ifndef LABEL
#define LABEL

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_draw.h>
#include "abstract_element.h"

class Label : public AbstractUIElement
{

public:
    Label( char* myText )
    {
        text = myText;
        setFont("res/CharisSILR.ttf", 18);

        textsf = TTF_RenderUTF8_Blended( font, text, text_color );
    }

    void render(SDL_Surface* s)
    {
        #ifdef DEBUG
        Draw_Rect(s, bounds.x,bounds.y, bounds.w, bounds.h,
        SDL_MapRGB(s->format,  text_color.r, text_color.g, text_color.b));
        //Draw_Rect(s, &bounds, SDL_MapRGB(s->format, text_color.r, text_color.g, text_color.b));
        #endif // DEBUG

        if(textsf) {
            if( alignment == Align::CENTER ) alignText();
            else textpos = bounds;
            SDL_BlitSurface(textsf, nullptr, s, &textpos);
        }
    }

public:
    ~Label()
    {
        TTF_CloseFont(font);
        SDL_FreeSurface(textsf);
    }
};

#endif // LABEL

