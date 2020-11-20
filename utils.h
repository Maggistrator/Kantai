#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL_ttf.h>

#define FRAMES_PER_SECOND 30

using namespace std;
    /// Вспомогательная функция загрузки PNG-изображений с прозрачностью
    static SDL_Surface* loadOptimisedSurface( std::string path , SDL_Surface* gScreenSurface )
    {
        SDL_Surface* optimizedSurface = NULL;
        SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
        if( loadedSurface == nullptr )
            printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        else {
            optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
            if( optimizedSurface == NULL )
            {
                printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            } else {
                Uint32 black = SDL_MapRGB( optimizedSurface->format , 0, 0, 0);
                SDL_SetColorKey( optimizedSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL , black );
            }
             SDL_FreeSurface( loadedSurface );
        }
        return optimizedSurface;
    }

    struct DeltaCounter
    {
        uint32_t delta = 0;
        uint32_t last_tick_time = 0;

        void tick()
        {
            uint32_t tick_time = SDL_GetTicks();
            delta = tick_time - last_tick_time;
            last_tick_time = tick_time;
        }
    };




class Timer
{
    private:
    int startTicks;
    int pausedTicks;
    int delta;

    bool paused;
    bool started;

    public:

Timer()
{
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void start()
{
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
}

void  stop()
{
    started = false;
    paused = false;
}

void  pause()
{
    if( ( started == true ) && ( paused == false ) )
    {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void  unpause()
{
    if( paused == true )
    {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

int  get_ticks()
{
    if( started == true )
    {
        if( paused == true )
        {
            return pausedTicks;
        }
        else
        {
            return SDL_GetTicks() - startTicks;
        }
    }
    return 0;
}

bool  is_started()
{
    return started;
}

bool  is_paused()
{
    return paused;
}
};

#endif // UTILS_H
