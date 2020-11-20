/*
 *  Сущность - контейнер для подсистем. Сущностями являются
 *  игроки, противники, снаряды и так далее - всё, что может
 *  участвовать в игровом процессе исключаяя разве что
 *  декорации.
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <list>
#include "subsystem.h"
#include "aspect.h"

class Engine;

class Entity {

private:
	std::list<Subsystem*> subsystems;

public:

	virtual void update( StateBasedGame* g, GameState* state, Engine* e, int delta )
	{
		for(Subsystem* s: subsystems)
		{
			s->update(g, state, e, this, delta);
		}
	}

	virtual void render(SDL_Surface* sf)
	{
		for(Subsystem* s: subsystems)
		{
			s->render(sf);
		}
	}

	bool hasSubsystem(Aspect a)
	{
		for(Subsystem* s: subsystems)
		{
			if(a == s->getAspect()) return true;
		}
		return false;
	}

	void addSubsystem(StateBasedGame* g, GameState* state, Engine* e, Subsystem* s)
	{
	    s->init(g, state, e, this);
		subsystems.push_back(s);
	}

	Subsystem* getSubsystem(Aspect a)
	{
		for(Subsystem* s: subsystems)
		{
			if(a == s->getAspect()) return s;
		}
		return nullptr;
	}

	void removeSubsystem(Aspect a)
	{
		Subsystem* s = getSubsystem(a);
		if(s != nullptr) subsystems.remove(s);
	}

	~Entity()
	{
		for(Subsystem* s: subsystems)
		{
			s->~Subsystem();
		}
		subsystems.clear();
	}
};


#endif /* ENTITY_H_ */
