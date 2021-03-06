#ifndef ENGINE_CPP
#define ENGINE_CPP

/*
 * Entity-Component движок, концентрирующий Сущности,
 * концентрирующие подсистемы. Занимается коммуникацией
 * сущностей между собой на правах контейнера верхнего
 * уровня, и отрисовкой.
 *
 * Не в хэдере как остальные классы движка, так как не является
 * служебным, не переопределяется и не включается в другие
 * классы как донор функциональности или данных - это
 * самодостаточный модуль, требующий только вызовов.
 */

#include <list>
#include "entity.h"
#include <SDL.h>
#include <iostream>
using namespace std;
class Engine
{
	std::list<Entity*> entities;
	std::list<Entity*> removable;

public:
	void update(StateBasedGame* g, GameState* state, int delta )
	{

	    for(int i = 0; i < removable.size(); i++){
            Entity* toDelete = removable.front();
            entities.remove(toDelete);
            removable.remove(toDelete);
            #ifdef DEBUG
            cout << "removable size:"<<removable.size()<<endl;
            cout << "deleting:"<<toDelete<<endl;
            #endif // DEBUG
            delete toDelete;
	    }
		for(Entity* e: entities) e->update(g, state, this, delta );
	}

	void render(SDL_Surface* s)
	{
		for(Entity* e: entities) e->render(s);
	}

	std::list<Entity*> getAllPosessing(Aspect a)
	{
		std::list<Entity*> posessing;

		for(Entity* e: entities)
		{
			if(e->hasSubsystem(a)) posessing.push_back(e);
		}
		return posessing;
	}

	void addEntity(Entity* e)
	{
		entities.push_back(e);
	}

	void killEntity(Entity* e)
	{
	    removable.push_back(e);
	}

	~Engine()
	{
		for(Entity* e: entities) delete e;
	}

};


#endif // ENGINE_CPP
