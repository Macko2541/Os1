/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 17, 2018
 *      Author: OS1
 */

#include "IVTEntry.h"
IVTEntry* IVTEntry::allEntries[256];
#include <dos.h>
#include "KernelEv.h"

IVTEntry::IVTEntry(IVTNo ulaz, pInterrupt newRoutine) {
	oldInt=0;
	entry=ulaz;
	myEvent=0;
	allEntries[ulaz]=this;
#ifndef BCC_BLOCK_IGNORE
	oldInt=getvect(ulaz);
	setvect(ulaz,newRoutine);
#endif
	// TODO Auto-generated constructor stub

}

IVTEntry::~IVTEntry() {
	allEntries[entry]=0;
#ifndef BCC_BLOCK_IGNORE
	setvect(entry,oldInt);
#endif
	myEvent=0;
	oldInt=0;
	// TODO Auto-generated destructor stub
}



void IVTEntry::callOldInt(){
	(*oldInt)();
}

void IVTEntry::setEvent(KernelEv* ke){
	// razmisli o zakljucavanju sekcija ovde
	myEvent=ke;
}

void IVTEntry::clearEvent(){
	// razmisli o zakljucavanju sekcija ovde
	myEvent=0;
}

void IVTEntry::signal(){
	if (myEvent!=0){
		myEvent->signal();
	}
}

