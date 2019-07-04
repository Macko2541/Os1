/*
 * IVTEntry.h
 *
 *  Created on: Aug 17, 2018
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "Global.h"
class KernelEv;
typedef unsigned char IVTNo;
class IVTEntry {
public:
	IVTEntry(IVTNo ulaz, pInterrupt newRoutine);
	virtual ~IVTEntry();
	pInterrupt oldInt;
	IVTNo entry;
	KernelEv* myEvent;
	static IVTEntry* allEntries[];

	void callOldInt();
	void setEvent(KernelEv* ke);
	void clearEvent();
	void signal();
};



#endif /* IVTENTRY_H_ */
