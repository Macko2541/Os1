/*
 * PCB.h
 *
 *  Created on: Aug 5, 2018
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_
typedef unsigned long StackSize;
typedef unsigned int Time;
class PCBList;
class Thread;
class PCB {
public:
	PCB(StackSize stackSize, Time timeSlice, Thread *myThr);
	PCB();
	virtual ~PCB();
	unsigned *stack;
	unsigned ss; // stek segment
	unsigned sp; // stek offset
	unsigned bp;
	unsigned timeSlice;
	// mode: 0 za spremna ili blokirana, 1 za running;
	StackSize stackSize;
	Thread* myThread;
	PCBList* waitingPCBs;// promenljiva tipa liste za blokirane niti zbog ove niti.
	volatile int finished;
	volatile int blocked;// verovatno ne treba jer blokirana nit nikada nece biti running
	int id;
	volatile int started;
	volatile unsigned int sleepFlag; // 1 ako spava 0 ako ne
	volatile int moreToSleep;// more to sleep
	volatile int semBlocked;
	volatile int eventBlocked;
	volatile int retValue;
	volatile int retWait;

};

#endif /* PCB_H_ */
