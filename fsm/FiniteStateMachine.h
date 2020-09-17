#ifndef FINITESTATEMACHINE_H
#define FINITESTATEMACHINE_H

#include <SDL.h>
#include <list>
#include <iostream>

using namespace std;

class StateBasedGame;

class GameState {
    public:
    virtual void init( SDL_Surface* display, StateBasedGame* g ) = 0;

    virtual void update( StateBasedGame* g, int delta ) = 0;

    virtual void render( SDL_Surface* display ) = 0;

    virtual void exit( void ) = 0;
};

class StateBasedGame {
    // костыль вместо страшных плюсовых ассоциативных массивов
struct GameStateIndexer {
        int id;
        GameState* state;
    };

    int* exitPrivilegie = nullptr;
    list<GameStateIndexer*> states;
    GameStateIndexer* currentState = nullptr;

public:
    StateBasedGame( int* exitPrivilegie ){
        this->exitPrivilegie = exitPrivilegie;
    }

    void registerState( int id, SDL_Surface* display, GameState* state ) {
        GameStateIndexer* temp = new GameStateIndexer();
        temp->id = id;
        temp->state = state;
        states.push_back( temp );
        temp->state->init( display, this );
    }

    void switchState( int id )
    {
        for(GameStateIndexer* in : states) {
            if(in->id == id){
                currentState = in;
                return;
            }
        }
        cerr << "Переключение невозможно, состояния с индексом " << id << " не существует" << endl;
        cerr << "Доступны состояния: ";
        for (GameStateIndexer* in : states) cerr << in->id << " ";
        cerr << endl;
    }

    void update( int delta ) {
        if(currentState != nullptr)currentState->state->update( this, delta );
        else cerr << "Состояние не выбрано";
    };

    void render( SDL_Surface* display ) {
        if(currentState != nullptr) currentState->state->render( display );
        else cerr << "Состояние не выбрано";
    }

    void exit() {
        for (GameStateIndexer* in : states) in->state->exit();
        exitPrivilegie = 0;
    }
};

#endif // FINITESTATEMACHINE_H
