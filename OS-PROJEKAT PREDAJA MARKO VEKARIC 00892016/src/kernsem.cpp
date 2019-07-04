/*
 * kernsem.cpp
 *
 *  Created on: Aug 15, 2018
 *      Author: OS1
 */
#include "kernsem.h"
#include "semaphor.h"
#include "Global.h"
#include <dos.h>
#include "PCB.h"
#include "SCHEDULE.H"
#include "thread.h"
#include "SemList.h"



KernelSem::KernelSem(int init, Semaphore* s){
	mySem=s;
	id=s->getId();
	value=init;
	blockedThreads=new PCBList();
	(Global::Semaphores)->ins(this);
};

KernelSem::~KernelSem(){
	Elem* tek=this->blockedThreads->prvi;    //ako se brise semafor odblokiraju se sve niti sa njega!
	while (this->blockedThreads->prvi!=0){
		tek=this->blockedThreads->prvi;this->blockedThreads->prvi=this->blockedThreads->prvi->sled;
		PCB* p=tek->data;
		p->semBlocked=0;
		Scheduler::put(p);
		delete tek;
	}
	this->blockedThreads->prvi=0;
	this->blockedThreads->posl=0;
	// ako obrisemo semafor a da ima blokiranih niti u njemu, prvo moramo da izbacimo niti iz blokirane liste.

}

int KernelSem::val()const{
	return value;

}


int KernelSem::wait(int toBlock){

	int ret=0;
	if (!toBlock && value<=0)
		ret=-1;
	else
		if (--value<0){
			ret=1;
			block();
		}

	return ret;

}

void KernelSem::signal(){


	if (value++<0)
		deblock();


}

void KernelSem::block(){
	Global::running->semBlocked=1;
	blockedThreads->ins((PCB*)Global::running);
	Global::explicitDispatch=1;
}

void KernelSem::deblock(){
	PCB* p=blockedThreads->firstDel();
	if (p!=0){
	p->semBlocked=0;
	Scheduler::put(p);
	}
}
