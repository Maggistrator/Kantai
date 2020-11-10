#ifndef HIGHSCORES
#define HIGHSCORES

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "states.h"
#include "FiniteStateMachine.h"
#include "../ui/label.h"
#include "../ui/button.h"
#include "../sessions.h"

#define LEFT_BORDER 10
#define OFFSET 30
#define alignToCenterX(who) ((who)->bounds.x = (screen->w - (who)->bounds.w)/2)
#define alignToCenterY(who) ((who)->bounds.y = (screen->h - (who)->bounds.y)/2)

using namespace std;

static void saveHighScore(StateBasedGame*, SDL_Event*, void*);

class Highscores : public GameState
{

public:
    class Record
    {
        Label *playerNameLabel, *points;
        Button *save_button;
        int x = 0, y = 0;
        GameState* owner;

        bool save;

        SDL_Surface* screen;

    public:
        char* playerName;
        int scores;

        Record (char* pn, int scores, SDL_Surface* s, GameState* owner, bool save)
        {
            playerName = pn;
            this->scores = scores;

            playerNameLabel = new Label(pn);

            char* points_text = new char[10];
            sprintf(points_text, "%d", scores);
            points = new Label(points_text);

            if(save)
            {
                save_button = new Button(saveHighScore, "Сохранить", this);
                save_button->bounds.w = 120;
                save_button->bounds.h = 15;
                save_button->setFont("res/CharisSILR.ttf", 12);
                save_button->alignment = Button::CENTER;
            }
            screen = s;
            this->save = save;
            this->owner = owner;
        }

        /** метка отрисовывается на любой поверхности, не только на экране;
          * например - на к-л панели, потому поверхность передается в параметрах
        */
        void render(SDL_Surface* s)
        {
            playerNameLabel->render(s);
            points->render(s);

            if(save) save_button->render(s);
        }

        void update(StateBasedGame* g, SDL_Event* e)
        {
            if(save) save_button->update(g, e);
        }

        void setX(int x)
        {
            this->x = x;
            playerNameLabel->bounds.x = x;
            alignToCenterX(points);
            if(save) save_button->bounds.x = screen->w - save_button->bounds.w - x;
        }

        void setY(int y)
        {
            this->y = y;
            playerNameLabel->bounds.y = y;
            points->bounds.y = y;
            if(save) save_button->bounds.y = y;
        }

        int getX()
        {
            return x;
        }
        int getY()
        {
            return y;
        }

        ~Record()
        {
            delete playerNameLabel;
            delete points;
            if(save)
            {
                delete save_button->text;
                delete save_button;
            }
        }
    };
private:
    SDL_Event event;
    SDL_Surface* screen;

    Label* title, *subtitle1, *subtitle2, *subtitle3, *subtitle4;

    list<Record*> currentSessionList, bestHighscoresList;
public:

    void init( SDL_Surface* display, StateBasedGame* g )
    {
        screen = display;
        title = new Label("РЕКОРДЫ");
        subtitle1 = new Label("За текущую сессию:");
        subtitle2 = new Label("За все время:");
        subtitle3 = new Label("Имя:");
        subtitle4 = new Label("Очков:");

        title->alignment = Label::Align::CENTER;

        title->bounds.w = 300;
        title->bounds.y = 5;
        //title->bounds.x = (screen->w - title->bounds.w)/2;
        alignToCenterX(title);
        title->setFont("res/CharisSILR.ttf", 24);

        subtitle1->bounds.w = 350;
        subtitle1->bounds.y = title->bounds.y + OFFSET;
        subtitle1->bounds.x = LEFT_BORDER;

        subtitle2->bounds.w = 300;
        subtitle2->bounds.y = title->bounds.y + OFFSET*2;
        subtitle2->bounds.x = LEFT_BORDER;
        alignToCenterY(subtitle2);

        subtitle3->bounds.y = title->bounds.y + OFFSET*2;
        subtitle3->bounds.x = LEFT_BORDER;

        subtitle4->bounds.w = 300;
        subtitle4->bounds.y = title->bounds.y + OFFSET*2;
        alignToCenterX(subtitle4);
    }

    void enter(StateBasedGame* g)
    {
        int records = 0;
        for(int highscore: current_session.current_player->scores)
        {
            records++;
            Record* rec = new Record(current_session.current_player->name, highscore, getScreen(), this, true);
            rec->setX(LEFT_BORDER);
            rec->setY(65 + (OFFSET/2) * records);
            currentSessionList.push_back(rec);
            cout << "record: " << rec << endl;
        }
        /* TODO:
         * Написать такой же двойной цикл на сохраненные рекорды
        */
    }

    void update( StateBasedGame* g, int delta )
    {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) g->exit();
            else if((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
                g->switchState(states::main_menu);
        }


        for(Record* rec: currentSessionList) rec->update(g, &event);
        for(Record* rec: bestHighscoresList) rec->update(g, &event);
    }

    void render( SDL_Surface* display )
    {
        title->render(display);
        subtitle1->render(display);
        subtitle2->render(display);
        subtitle3->render(display);
        subtitle4->render(display);
        for(Record* rec: currentSessionList) rec->render(display);
        for(Record* rec: bestHighscoresList) rec->render(display);
    }

    void leave()
    {
        for(Record* rec: currentSessionList) delete rec;
        for(Record* rec: bestHighscoresList) delete rec;

        currentSessionList.clear();
        bestHighscoresList.clear();
    }

    SDL_Event* pollEvent( void )
    {
        return &event;
    }

    SDL_Surface* getScreen()
    {
        return screen;
    }

    ~Highscores()
    {
        delete title;
        delete subtitle1;
        delete subtitle2;
        delete subtitle3;
        delete subtitle4;
        for(Record* rec: currentSessionList) delete rec;
        for(Record* rec: bestHighscoresList) delete rec;
    }
};

static void saveHighScore(StateBasedGame* g, SDL_Event* e, void* caller)
{
    Highscores::Record* record = reinterpret_cast < Highscores::Record* >(caller);

}

#endif // HIGHSCORES


