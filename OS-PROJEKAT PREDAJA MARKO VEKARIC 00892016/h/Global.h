/*
 * Global.h
 *
 *  Created on: Aug 5, 2018
 *      Author: OS1
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_


class PCBList;
class IdleT;
class PCB;
class PCBSl;
class SemList;
class KernelThread;
class Thread;
class EvList;
class Semaphore;
typedef void interrupt (*pInterrupt)(...);
typedef unsigned int Time;
typedef unsigned long StackSize;
typedef unsigned char IVTNo;
typedef struct sysStruct{
	unsigned id;
	int idThread;
	int idRunning;
	Time sleepTime;
	StackSize stackSize;
	Time timeSlice;
	Thread * myThread;
	int toBlock;
	int idSem;
	int semInit;
	Semaphore *mySemaphore;
	int idEve;
	IVTNo ivtNo;
}sysStruct;//struktura za prenosenje argumenata sistemskog poziva

#define lock asm{\
	pushf\
	cli\
}

#define unlock asm popf;

class Global{
	public:
	static volatile int counter;
	static volatile PCB* running;
	static volatile int explicitDispatch; // ne zaboravi da je inicijalizujes na pocetku programa.
	static volatile unsigned int lockFlag;
	static volatile int lockCount;
	static volatile PCB* mainPCB;
	static int waitRetVal;
	static IdleT* idle;
	static void lockPreemt();
	static void unlockPreemt();
	static PCBList* PCBs;
	static volatile PCB* mainThread; // za sada nam ne treba.
	static void syncCout(char* str,int br);
	static volatile PCB* pcbe;
	static volatile int one;
	static PCBSl* SleepingPCBs;
	static volatile int sleepDispatch;
	static SemList* Semaphores;
	static volatile int oneKernel;
	static volatile PCB* runningKernelThread;// kernel pcb
	static KernelThread* runningKT;// kernel thread
	static volatile int kernelTrci;
	static EvList* Events;
	static int retValue;

};

int syncPrintf(const char *format, ...);

#endif /* GLOBAL_H_ */
