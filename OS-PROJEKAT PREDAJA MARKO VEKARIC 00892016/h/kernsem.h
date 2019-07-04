/*
 * kernsem.h
 *
 *  Created on: Aug 15, 2018
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_
#include "PCBList.h"
#include "semaphor.h"
typedef int ID;
class KernelSem{
public:
	KernelSem(int init, Semaphore* s);
	~KernelSem();
	int val()const;
	int wait(int toBlock);
	void signal();
	void block();
	void deblock();
	int value;
	PCBList* blockedThreads;
	ID id;
	Semaphore* mySem;
};




#endif /* KERNSEM_H_ */
