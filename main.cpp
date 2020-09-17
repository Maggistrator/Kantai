#include<SDL.h>
#include<SDL_ttf.h>
int main(int argc, char*argv[])
{
    SDL_Surface*screen;
    SDL_Event event;
    TTF_Font *fnt = NULL;
    SDL_Color text_color;
    SDL_Rect dest;
    SDL_Surface *text_surface = NULL;
    /* Строка "Привет, Мир!" в кодировке utf8 */
    char hello_world[]="Hello world!";
    if (SDL_Init(SDL_INIT_VIDEO))return 1;
    if (TTF_Init()) return 1;
    if(!(screen=SDL_SetVideoMode(640,480,32,SDL_ANYFORMAT)))
    {
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    if(!(fnt = TTF_OpenFont("CharisSILR.ttf", 29)))
    {
        TTF_Quit();
        SDL_Quit();
        return 3;
    }
    dest.x = 200;
    dest.y = 100;
    dest.w = 1;
    dest.h = 1;
    text_color.r = 0;
    text_color.g = 0;
    text_color.b = 255;
    if(text_surface = TTF_RenderUTF8_Solid(fnt, hello_world, text_color))
    {
        SDL_BlitSurface(text_surface, NULL, screen, &dest);
        SDL_FreeSurface(text_surface);
        text_surface = NULL;
    }
    SDL_Flip(screen);
    while(SDL_WaitEvent(&event))
    {
        if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&event.key.keysym.sym == SDLK_ESCAPE))
        {
            TTF_CloseFont(fnt);
            TTF_Quit();
            SDL_Quit();
            return 0;
        }
    }TTF_CloseFont(fnt);TTF_Quit();SDL_Quit();return 2;
}
