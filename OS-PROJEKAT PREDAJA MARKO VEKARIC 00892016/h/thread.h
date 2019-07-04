/*
 * thread.h
 *
 *  Created on: Aug 5, 2018
 *      Author: OS1
 */

#ifndef _thread_h_
#define _thread_h_
#include "Global.h"
#include "intLock.h"

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;     // Da li ovo moze da se prebaci u CPP fajl?
typedef unsigned int Time;// time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;
void dispatch();
class PCB;

class Thread {
public:

	void start();
	void waitToComplete();
	virtual ~Thread();

	static void sleep(Time timeToSleep);

	ID getId();
protected:
	friend class PCB;
	Thread(StackSize stackSize=defaultStackSize, Time timeSlice=defaultTimeSlice);
	virtual void run(){}
	Thread(int a,int b,int c);

private:
	static ID idpos;
	ID myID;
	static void wrapper(Thread* running);

};



#endif /* _thread_h_ */
