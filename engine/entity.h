/*
 *  Сущность - контейнер для подсистем. Сущностями являются
 *  игроки, противники, снаряды и так далее - всё, что может
 *  участвовать в игровом процессе исключаяя разве что
 *  декорации.
 *
 *  Created on: 26 авг. 2020 г.
 *      Author: sova
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <list>
#include "subsystem.h"
#include "aspect.h"

class Engine; //почему это выглядит как костыль?!

class Entity {

private:
	std::list<Subsystem*> subsystems;

public:

	virtual void update( StateBasedGame* g, GameState* state, Engine* e, int delta )
	{
		for(Subsystem* s: subsystems)
		{
			s->update(g, state, e, this, delta); //StateBasedGame* g, GameState* state, Engine* e, Entity* owner, int delta
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
			//cout<<"searching for aspect "<<a<<", found aspect "<<s->getAspect()<<" in subsystem " << s <<endl;
		}
		return false;
	}

	void addSubsystem(StateBasedGame* g, GameState* state, Engine* e, Subsystem* s)
	{
	    s->init(g, state, e, this);//StateBasedGame* g, GameState* state, Engine* e, Entity* owner
		subsystems.push_back(s);
	}

	//TODO: придумать оптимизацию, чтобы последовательный вызов
	//hasSubsystem() и getSubsystem() не итерировались по списку
	//подсистем дважды. Избавитьсьтя от hasSubsystem()?
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
