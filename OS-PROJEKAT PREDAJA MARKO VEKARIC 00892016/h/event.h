/*
 * event.h
 *
 *  Created on: Aug 17, 2018
 *      Author: OS1
 */

#ifndef _event_h_
#define _event_h_

#include "IVTEntry.h"

#include "Makro.h"
typedef int ID;
typedef unsigned char IVTNo;
class KernelEv;
class Event {
public:
	Event(IVTNo ivtNo);
	virtual ~Event();

	void wait();
	ID id;
protected:
	friend class KernelEv;
	void signal();
private:

	static int posId;
};

extern unsigned structSeg3;
extern unsigned structOff3;

#endif /* EVENT_H_ */
