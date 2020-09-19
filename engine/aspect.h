/*
 *  Аспект, к которому принадлежит подсистема - грубо говоря,
 *  её тип. Примеры:
 *  - Подсистема обнаружения столкновений (Аспект: collidable)
 *  - Подсистема управления с клавиатуры  (Аспект: controllable)
 *  - Подсистема обсчета движения тела 	  (Аспект: movable)
 *  и так далее.
 *  Позволяет во время исполнения отличать подсистемы друг от
 *  друга - т.е. это велосипед для того, чтобы не искать в
 *  С++ рефлешкн.
 *
 *  Created on: 24 авг. 2020 г.
 *      Author: Sova
 */

#ifndef ASPECT_H_
#define ASPECT_H_

enum Aspect {
    drawable,
    positioned,
	movable,
	controllable,
	selfdestroyable,
	collidable
	/*etc*/
};



#endif /* ASPECT_H_ */
