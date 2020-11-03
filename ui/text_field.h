#ifndef TEXT_FIELD
#define TEXT_FIELD
#include <SDL.h>
#include <cstring>
#include "abstract_element.h"

class TextField : public Label
{
    SDL_Color color = { 255, 255, 255 };

    public:
    TextField(){
        text = new char[256];
        setFont("res/CharisSILR.ttf", 18);
        text_color = {0, 0, 0};
    }

    void render(SDL_Surface* s)
    {
        SDL_FillRect(s, &bounds, SDL_MapRGB(s->format, color.r, color.g, color.b));
        Label::render(s);
    }

    void update( SDL_Event& event )
    {
        switch (event.type) {
            case SDL_KEYDOWN:
                printf("The %s key was pressed!\n", SDL_GetKeyName(event.key.keysym.sym));
                strcat(text, SDL_GetKeyName(event.key.keysym.sym));
                SDL_FreeSurface(textsf);
                textsf = TTF_RenderUTF8_Blended( font, text, text_color );
                break;
            case SDL_QUIT:
                exit(0);
        }
    }

    ~TextField(){
        delete text;
    }

};
#endif // TEXT_FIELD
