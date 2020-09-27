#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "SDL/SDL_ttf.h"

#define FRAMES_PER_SECOND 30

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


///////////////////////////////////////////////////////////////////////////////////////////


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
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void  stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void  pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void  unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int  get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
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
