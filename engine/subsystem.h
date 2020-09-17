/*
 *  Интерфейс Подсистемы.
 *  Может содержать в себе логику, данные, или ссылки на другие
 *  подсистемы. Позволяет динамически добавлять и удалять
 *  функциональность из Сущностей, следуя принципу превосходства
 *  композиции над наследованием.
 *	Сущность может содержать не более одной подсистемы с
 *	одним и тем же аспектом.
 *
 *  Created on: 24 авг. 2020 г.
 *      Author: Sova
 */

#ifndef SUBSYSTEM_H_
#define SUBSYSTEM_H_

#include <SDL.h>
#include "aspect.h"
#include "../fsm/FiniteStateMachine.h"

class Engine;
class Entity;

class Subsystem {

public:
    // эти функции не объявлены истинно виртуальными, так как не все подсистемы обязаны определять их в полном обьеме
    virtual void init(StateBasedGame* g, GameState* state, Engine* e, Entity* owner){}
	virtual void render(SDL_Surface* s){}
	virtual void update(StateBasedGame* g, GameState* state, Engine* e, Entity* owner, int delta){}
	// любая подсистема имеет аспект
	virtual Aspect getAspect() = 0;

	virtual ~Subsystem(){}
};

#endif /* SUBSYSTEM_H_ */
