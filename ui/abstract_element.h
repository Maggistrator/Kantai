#ifndef ABSTRACT_ELEMENT
#define ABSTRACT_ELEMENT

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_draw.h>
class AbstractUIElement {
protected:
/**
    Прямоугольник с целочисленными координатами и размерами
    SDL_Rect использует UInt16 в связи с чем несовместим с возвращаемыми значениями SDL_ttf
*/
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

    virtual void render(SDL_Surface* s) = 0;

    void setFont(const char* path2font, int size)
    {
        TTF_CloseFont(font);
        font = TTF_OpenFont( path2font, size );
        textsf = TTF_RenderUTF8_Blended( font, text, text_color );
    }

    void setTextColor(SDL_Color& c){
        this->text_color = c;
        textsf = TTF_RenderUTF8_Blended( font, text, text_color );
    }

protected:
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
};

#endif // ABSTRACT_ELEMENT

