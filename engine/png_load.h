#ifndef PNG_LOAD
#define PNG_LOAD

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;
    /// Вспомогательная функция загрузки PNG-изображений
    static SDL_Surface* loadOptimisedSurface( std::string path , SDL_Surface* gScreenSurface )
    {
        SDL_Surface* optimizedSurface = NULL;
        SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
        if( loadedSurface == nullptr )
            printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        else {
            //Convert surface to screen format
            optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
            if( optimizedSurface == NULL )
            {
                printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            } else {
                Uint32 black = SDL_MapRGB( optimizedSurface->format , 0, 0, 0);
                SDL_SetColorKey( optimizedSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL , black );
            }
            //Get rid of old loaded surface
            SDL_FreeSurface( loadedSurface );
        }
        return optimizedSurface;
    }

#endif // PNG_LOAD
