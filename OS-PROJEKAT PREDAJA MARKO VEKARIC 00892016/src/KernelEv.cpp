/*
 * KernelEv.cpp
 *
 *  Created on: Aug 17, 2018
 *      Author: OS1
 */

#include "KernelEv.h"
#include "Global.h"
#include "thread.h"
#include "IVTEntry.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "EvList.h"
KernelEv::KernelEv(IVTNo ivtNo, int id) {
	this->id=id;
	creator=(PCB*)Global::running;
	blockedThread=0;
	value=0;
	IVTEntry::allEntries[ivtNo]->setEvent(this);
	 ulaz=ivtNo;
	Global::Events->ins(this);
	// napisi ovde nesto

}

KernelEv::~KernelEv() {
	creator=0;
	blockedThread=0;
	IVTEntry::allEntries[ulaz]->clearEvent();// napisi ovde nesto
}


void KernelEv::wait(){
	if (Global::running==creator){
		if (value==0){
			blockedThread=creator;
			blockedThread->eventBlocked=1;
			Global::explicitDispatch=1;//blokiraj nit
		}
		else
		if (value==1) value=0;
	}

}

void KernelEv::signal(){

	if (blockedThread==0) value=1;
	else {
		 value=0;
		 blockedThread->eventBlocked=0;
		 Scheduler::put(blockedThread);
		 blockedThread=0;
		 //odbolkirati nit i postaviti pokazivac na 0
	}

}

