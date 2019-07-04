/*
 * PCB.cpp
 *
 *  Created on: Aug 5, 2018
 *      Author: OS1
 */
#include <iostream.h>
#include "PCB.h"
#include <dos.h>
#include "thread.h"
#include "PCBList.h"
#include "Global.h"
#include "SCHEDULE.H"


PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThr) {
	if(stackSize>65536) stackSize=65536;
	stackSize/=sizeof(unsigned);
	this->stackSize=stackSize;
	stack=new unsigned[stackSize];
	myThread=myThr;
	id=myThread->myID;
	this->timeSlice=timeSlice;
	finished=0;
	blocked=0;
	started=0;
	semBlocked=0;
	eventBlocked=0;
	retValue=0;
	retWait=0;
	waitingPCBs=new PCBList();
	moreToSleep=0;
	sleepFlag=0;
    #ifndef BCC_BLOCK_IGNORE
	stack[stackSize-1]=FP_SEG(myThr);
	stack[stackSize-2]=FP_OFF(myThr);
	stack[stackSize-5]=0x200; // -3 i-4 su za callback funckije.
	stack[stackSize-6]=FP_SEG(Thread::wrapper);
	stack[stackSize-7]=FP_OFF(Thread::wrapper);
	this->ss=FP_SEG(stack+stackSize-16);
	this->sp=FP_OFF(stack+stackSize-16);
    #endif
	// registri na -8 do -15 i bp na -16.(Njegova pocetna vrednsost je nebitna).
	bp=sp;
	Global::PCBs->ins(this);//UBACENA NIT U PCBs listu

}

PCB::PCB(){
 myThread=0;
 stackSize=defaultStackSize/sizeof(unsigned); // mozda velicina nekog sistemskog steka
 finished=0;
 blocked=0;
 ss=0;
 id=-2;
 sp=0;
 bp=0;
 semBlocked=0;
 eventBlocked=0;
 stack=0;
 started=1;
 moreToSleep=0;
 sleepFlag=0;
 waitingPCBs=new PCBList();
 timeSlice=1; //nit maina je startovana cim je kreireamo u inicu
 Global::PCBs->ins(this);//UBACENA NIT U PCBs listu
 Global::counter=timeSlice;
}

PCB::~PCB() {
	/*delete myThread;*/// TODO Auto-generated destructor stub
}

void Thread::wrapper(Thread* running){

		running->run();  //moguca greska zbog GLOBAL::running umesto running
		while(1){
			Global::pcbe=Global::running->waitingPCBs->firstDel();
			if (Global::pcbe==0) break;
			Global::pcbe->blocked=0;
			if (Global::pcbe->started==1)Scheduler::put((PCB*)Global::pcbe);
		}// probudimo sve niti koje su cekale ovu nit da zavrsi i stavimo u scheduler.Prvo ih obelezimo kao spremne.
		// obelezimo da je ta nit zavrsila posao da je ne bismo opet stavljali u scheduler.
		// izbacivanje iz liste;
		Global::running->finished=1;
		/*Global::PCBs->del(Global::running->id); brisacemo u destruktoru pcb niti i izbacivati ga iz ove liste.*/
		dispatch();// mora dispatch na kraju jer nismo definisali callback adresu i nikada ne smemo da se vratimo iz ove funkcije.

}

