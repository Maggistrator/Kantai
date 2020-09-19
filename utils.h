#ifndef UTILS_H
#define UTILS_H

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

    struct Clock
    {
        uint32_t last_tick_time = 0;
        uint32_t delta = 0;

        void tick()
        {
            uint32_t tick_time = SDL_GetTicks();
            delta = tick_time - last_tick_time;
            last_tick_time = tick_time;
        }
    };

#endif // UTILS_H
