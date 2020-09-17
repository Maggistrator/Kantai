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

//TODO:протестить в бою эту заглушку, выглядит
//как костыль, но как иначе прервать цепочечное
//включение?
class Engine;

class Subsystem {

//TODO: добавить аргументы Engine и Game по мере создания
//		этих классов
public:
	virtual void update(Engine* e/*Game*/) = 0;
	virtual void render(SDL_Surface* s) = 0;
	virtual Aspect getAspect() = 0;

	virtual ~Subsystem();
};


#endif /* SUBSYSTEM_H_ */
