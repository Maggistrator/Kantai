#ifndef HIGHSCORES
#define HIGHSCORES

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <vector>
#include <string.h>

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

class Highscores : public GameState
{

public:
    class RecordUI
    {
        Record* record;
        Label *playerNameLabel, *points;
        int x = 0, y = 0;

        SDL_Surface* screen;

    public:

        RecordUI (Record* record, SDL_Surface* s)
        {
            this->record = record;

            playerNameLabel = new Label(record->name);

            char* points_text = new char[10];
            sprintf(points_text, "%d", record->score);
            points = new Label(points_text);
            screen = s;
        }

        /** метка отрисовывается на любой поверхности, не только на экране;
          * например - на к-л панели, потому поверхность передается в параметрах
        */
        void render(SDL_Surface* s)
        {
            playerNameLabel->render(s);
            points->render(s);
        }



        void setX(int x)
        {
            this->x = x;
            playerNameLabel->bounds.x = x;
            alignToCenterX(points);
        }

        void setY(int y)
        {
            this->y = y;
            playerNameLabel->bounds.y = y;
            points->bounds.y = y;
        }

        int getX()
        {
            return x;
        }
        int getY()
        {
            return y;
        }

        ~RecordUI()
        {
            delete playerNameLabel;
            delete points;
        }
    };

private:
    SDL_Event event;
    SDL_Surface* screen;

    Label* title, *subtitle1, *subtitle2, *subtitle3, *subtitle4;

    vector<RecordUI*> currentSessionList, bestHighscoresList;
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
        int current_session_reccount = 0, overall_records_count = 0;

        for(Record* record: current_session.highscores) {
            if(record != nullptr) {
                current_session_reccount++;
                RecordUI* ui = new RecordUI(record, getScreen());
                ui->setX(LEFT_BORDER);
                ui->setY(65 + (OFFSET/2) * current_session_reccount);
                currentSessionList.push_back(ui);
            }
        }

        current_session.loadOverallHighscores();
        for(Record* record: current_session.overall_highscores) {
            if(record != nullptr) {
                current_session_reccount++;
                RecordUI* ui = new RecordUI(record, getScreen());
                ui->setX(LEFT_BORDER);
                ui->setY(95 + (OFFSET/2) * current_session_reccount);
                currentSessionList.push_back(ui);
            }
        }
    }

    void update( StateBasedGame* g, int delta )
    {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) g->exit();
            else if((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
                g->switchState(states::main_menu);
        }
    }

    void render( SDL_Surface* display )
    {
        title->render(display);
        subtitle1->render(display);
        subtitle2->render(display);
        subtitle3->render(display);
        subtitle4->render(display);
        for(RecordUI* ui: currentSessionList) ui->render(display);
        for(RecordUI* ui: bestHighscoresList) ui->render(display);
    }

    void leave()
    {

        for(RecordUI* ui: currentSessionList) delete ui;
        for(RecordUI* ui: bestHighscoresList) delete ui;

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
        for(RecordUI* ui: currentSessionList) delete ui;
        for(RecordUI* ui: bestHighscoresList) delete ui;
    }
};

#endif // HIGHSCORES


