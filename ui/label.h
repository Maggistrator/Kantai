#ifndef LABEL
#define LABEL

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_draw.h>

class Label
{
struct int_Rect {
    int x, y, w, h;
};

// Набор приватных полей обслуживающих вывод текста ----------
SDL_Surface* textsf;
TTF_Font *font = nullptr;
int_Rect offset;
SDL_Rect textpos;
//------------------------------------------------------------

public:
    enum Align{ LEFT, CENTER };
    SDL_Rect bounds = { 0, 0, 200, 30 };
    SDL_Color text_color = { 255, 255, 255 };
    Align alignment = Align::LEFT;
    char *text = nullptr;

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

    void setFont(const char* path2font, int size)
    {
        TTF_CloseFont(font);
        /*Это утечка памяти, но если я освобождаю поверхность, программа валится!*/
        //SDL_FreeSurface(textsf);//
        font = TTF_OpenFont( path2font, size );
        textsf = TTF_RenderUTF8_Blended( font, text, text_color );
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
    ~Label()
    {
        TTF_CloseFont(font);
        SDL_FreeSurface(textsf);
    }
};

#endif // LABEL

