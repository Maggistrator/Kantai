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
 *
 *  Created on: 26 авг. 2020 г.
 *      Author: sova
 */

#include <list>
#include "entity.h"
#include <SDL.h>

class Engine
{
	std::list<Entity*> entities;

public:
	void update(StateBasedGame* g, GameState* state, int delta )
	{
		for(Entity* e: entities) e->update(g, state, this, delta );//StateBasedGame* g, GameState* state, Engine* e, int delta
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
		entities.remove(e);
		delete e;
	}

	~Engine()
	{
		for(Entity* e: entities) delete e;
	}

};


