/*
 * KernelEv.h
 *
 *  Created on: Aug 17, 2018
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

class PCB;
// mozda treba pokazivac na event koji ga je stvorio
typedef unsigned char IVTNo;
typedef int ID;
class KernelEv {
public:
	KernelEv(IVTNo ivtNo, int id);
	virtual ~KernelEv();
	void wait();
	void signal();
	ID id;
private:
	PCB* creator;
	PCB* blockedThread;
	int value;
	IVTNo ulaz;

};

#endif /* KERNELEV_H_ */
