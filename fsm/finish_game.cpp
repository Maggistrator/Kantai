#ifndef STATISTICS_STATE
#define STATISTICS_STATE

#include "states.h"
#include "FiniteStateMachine.h"
#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

class Statistics : public GameState
{

    int m_kills, m_points;

    SDL_Event event;
    SDL_Surface* screen;

    SDL_Surface *killsSurface, *pointsSurface, *noticeSurface;
    char *killsText, *pointsText, *noticeText = "To continue press ENTER, for returning to main menu press ESC";
    TTF_Font *font = NULL;
    SDL_Color textColor = { 255, 255, 255 };
    SDL_Rect pointsPlaceholder = {360, 250, 200, 20};
    SDL_Rect killsPlaceholder =  {370, 270, 200, 20};
    SDL_Rect noticePlaceholder = {10, 90, 200, 20};


    public:
    Statistics (int kills, int points): m_kills(kills), m_points(points) {}

    void init( SDL_Surface* display, StateBasedGame* g )
    {
        screen = display;
        font = TTF_OpenFont( "res/CharisSILR.ttf", 24 );
        if( font == NULL )
            cerr << "Не удалось загрузить шрифт CharisSILR! Причина: " << TTF_GetError() << endl;
        pointsText = new char[32];
        killsText  = new char[32];
        sprintf(killsText, "%s%d", "Kills: ", m_kills);
        sprintf(pointsText, "%s%d", "Points: ", m_points);
        noticePlaceholder.y = display->h - 50;
    }

    int cooldown = 100;
    void update( StateBasedGame* g, int delta )
    {
        //if(cooldown <=0){
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) g->exit();
            else {
                if((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN))
                {
                    g->switchState(states::game);
                    g->switchState( game );
                    g->unregisterState( results );
                }
                else if((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
                    g->switchState(states::main_menu);
            }
        }
        //}else cooldown--;
    }

    void render( SDL_Surface* display )
    {
        //отрисовка метки очков
        pointsSurface = TTF_RenderText_Solid( font, pointsText, textColor );
        SDL_BlitSurface( pointsSurface, nullptr, display, &pointsPlaceholder );
        //отрисовка метки потопленных противников
        killsSurface = TTF_RenderText_Solid( font, killsText, textColor );
        SDL_BlitSurface( killsSurface, nullptr, display, &killsPlaceholder );
        //отрисовка метки поясняющей управление
        noticeSurface = TTF_RenderText_Solid( font, noticeText, textColor );
        SDL_BlitSurface( noticeSurface, nullptr, display, &noticePlaceholder );
    }

    SDL_Event* pollEvent( void )
    {
        return &event;
    }

    SDL_Surface* getScreen()
    {
        return screen;
    }

    void exit( void )
    {
        delete killsText;
        delete pointsText;
        TTF_CloseFont(font);
        SDL_FreeSurface(killsSurface);
        SDL_FreeSurface(pointsSurface);
    }
};

#endif // STATISTICS_STATE
