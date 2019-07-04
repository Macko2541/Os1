/*
 * IdleT.h
 *
 *  Created on: Aug 8, 2018
 *      Author: OS1
 */

#ifndef IDLET_H_
#define IDLET_H_
#include "thread.h"

class IdleT:public Thread{
public:
	IdleT();
	virtual ~IdleT();
	virtual void run();
	volatile int vo;
};

#endif /* IDLET_H_ */
