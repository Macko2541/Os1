/*
 * kernel.h
 *
 *  Created on: Aug 20, 2018
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_
#include "thread.h"
#include "PCBList.h"
#include "PCB.h"
class KernelThread:public Thread{
public:
	KernelThread();
	virtual ~KernelThread();
protected:
	void run();
private:
	void _dispatch();
};

#endif /* KERNEL_H_ */
