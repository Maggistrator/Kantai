#ifndef TEXT_FIELD
#define TEXT_FIELD
#include <SDL.h>
#include <cstring>
#include "abstract_element.h"

class TextField : public Label
{
    std::string str;
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
        if( event.type == SDL_KEYDOWN )
        {
            std::string temp = str;

            if( str.length() <= 16 )
            {
                if( event.key.keysym.unicode == (Uint16)' ' )
                {
                    str += (char)event.key.keysym.unicode;
                }
                else if( ( event.key.keysym.unicode >= (Uint16)'0' ) && ( event.key.keysym.unicode <= (Uint16)'9' ) )
                {
                    str += (char)event.key.keysym.unicode;
                }
                else if( ( event.key.keysym.unicode >= (Uint16)'A' ) && ( event.key.keysym.unicode <= (Uint16)'Z' ) )
                {
                    str += (char)event.key.keysym.unicode;
                }
                else if( ( event.key.keysym.unicode >= (Uint16)'a' ) && ( event.key.keysym.unicode <= (Uint16)'z' ) )
                {
                    str += (char)event.key.keysym.unicode;
                }
            }

            if( ( event.key.keysym.sym == SDLK_BACKSPACE ) && ( str.length() != 0 ) )
            {
                str.erase( str.length() - 1 );
            }
            if( str != temp )
            {
                delete text;
                SDL_FreeSurface( textsf );
                text = (char*) str.c_str();
                textsf = TTF_RenderText_Solid( font, str.c_str(), text_color );
            }
        }
    }

    ~TextField(){
        delete text;
    }

};
#endif // TEXT_FIELD
