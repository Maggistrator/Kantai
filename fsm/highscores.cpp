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

static bool print_ok = false;

static void clearButtonOnClick(StateBasedGame* g , SDL_Event* e, void* owner);

#ifndef BACK_BUTTON_CALLBACK
#define BACK_BUTTON_CALLBACK
static void backButtonOnClick(StateBasedGame* g , SDL_Event* e, void* owner)
{
    g->switchState(states::main_menu);
}
#endif

#ifndef PRINT_BUTTON_CALLBACK
#define PRINT_BUTTON_CALLBACK
static void printButtonOnClick(StateBasedGame* g , SDL_Event* e, void* owner)
{
    print_ok = true;
    current_session.printOverallHighscores();
}
#endif

class Highscores : public GameState
{

public:
    class RecordUI
    {
        Record* record;
        Label * playerNameLabel, *points;
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

    Label  *title, *subtitle1, *subtitle2, *subtitle3, *subtitle4, *ok;
    Button *b_back, *b_clear, *b_print;

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

        b_back = new Button(backButtonOnClick, "Назад", this);
        b_back->bounds.x = 0;
        b_back->bounds.y = screen->h - b_back->bounds.h;

        b_clear = new Button(clearButtonOnClick, "Очистить", this);
        b_clear->bounds.x = (screen->w - b_clear->bounds.w)/2;
        b_clear->bounds.y = screen->h - b_clear->bounds.h;

        b_print = new Button(printButtonOnClick, "Печать", this);
        b_print->bounds.x = screen->w - b_print->bounds.w;
        b_print->bounds.y = screen->h - b_print->bounds.h;


        SDL_Color green = {0, 255, 0};
        ok = new Label("OK!");
        ok->setTextColor(green);
        ok->bounds.x = b_print->bounds.x;
        ok->bounds.y = b_print->bounds.y - b_print->bounds.h;
        ok->alignment = Label::CENTER;
    }

    void enter(StateBasedGame* g)
    {
        int current_session_reccount = 0, overall_records_count = 0;

        for(Record* record: current_session.highscores) {
            if(record != nullptr) {
                cout << "curr elem " << record << endl;
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
                cout << "overall elem " << record << endl;
                overall_records_count++;
                RecordUI* ui = new RecordUI(record, getScreen());
                ui->setX(LEFT_BORDER);
                ui->setY(screen->h/2 - 15 + (OFFSET/2 * overall_records_count));
                bestHighscoresList.push_back(ui);
            }
        }
    }

    void update( StateBasedGame* g, int delta )
    {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) g->exit();
            else if((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
                g->switchState(states::main_menu);
            }
            b_back->update(g, &event);
            b_clear->update(g, &event);
            b_print->update(g, &event);
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
        b_back->render(display);
        b_clear->render(display);
        b_print->render(display);
        if(print_ok) ok->render(display);
    }

    void leave()
    {
        currentSessionList.clear();
        clearHighscores();
        print_ok = false;
    }

    void clearHighscores()
    {
        for(RecordUI* ui: bestHighscoresList) delete ui;
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
        delete b_back;
        delete b_clear;
        delete b_print;
    }
};

#endif // HIGHSCORES

#ifndef CLEAR_BUTTON_CALLBACK
#define CLEAR_BUTTON_CALLBACK
static void clearButtonOnClick(StateBasedGame* g , SDL_Event* e, void* owner)
{
    current_session.clearOverallHighscores();
    reinterpret_cast<Highscores*>(owner)->clearHighscores();
    current_session.writeOverallHighscores();
}
#endif

