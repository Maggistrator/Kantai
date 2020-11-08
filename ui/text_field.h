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
            //Keep a copy of the current version of the string
            std::string temp = str;

            //If the string less than maximum size
            if( str.length() <= 16 )
            {
                //If the key is a space
                if( event.key.keysym.unicode == (Uint16)' ' )
                {
                    //Append the character
                    str += (char)event.key.keysym.unicode;
                }
                //If the key is a number
                else if( ( event.key.keysym.unicode >= (Uint16)'0' ) && ( event.key.keysym.unicode <= (Uint16)'9' ) )
                {
                    //Append the character
                    str += (char)event.key.keysym.unicode;
                }
                //If the key is a uppercase letter
                else if( ( event.key.keysym.unicode >= (Uint16)'A' ) && ( event.key.keysym.unicode <= (Uint16)'Z' ) )
                {
                    //Append the character
                    str += (char)event.key.keysym.unicode;
                }
                //If the key is a lowercase letter
                else if( ( event.key.keysym.unicode >= (Uint16)'a' ) && ( event.key.keysym.unicode <= (Uint16)'z' ) )
                {
                    //Append the character
                    str += (char)event.key.keysym.unicode;
                }
            }

            //If backspace was pressed and the string isn't blank
            if( ( event.key.keysym.sym == SDLK_BACKSPACE ) && ( str.length() != 0 ) )
            {
                //Remove a character from the end
                str.erase( str.length() - 1 );
            }

            //If the string was changed
            if( str != temp )
            {
                //Free the old surface
                SDL_FreeSurface( textsf );
                textsf = TTF_RenderText_Solid( font, str.c_str(), text_color );
            }
        }
    }

    ~TextField(){
        delete text;
    }

};
#endif // TEXT_FIELD
