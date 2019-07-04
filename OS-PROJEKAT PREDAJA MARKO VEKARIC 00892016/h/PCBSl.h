/*
 * PCBSl.h
 *
 *  Created on: Aug 14, 2018
 *      Author: OS1
 */

#ifndef PCBSL_H_
#define PCBSL_H_
#include "PCB.h"

typedef struct ElemS {
	PCB* pcb;
	ElemS* sled;
	unsigned int time;

	ElemS(PCB* p, int timee) {
		pcb = p;
		sled = 0;
		time = timee;
	}
	~ElemS() {
		sled = 0;
		pcb=0;
		// jos nesto
	}

}ElemS;
class PCBSl {
public:
	PCBSl();
	PCBSl::~PCBSl();
	void insert(PCB*,int time);
	void ins(ElemS *e);
	void del(); //u ovome ce i u scheduler da se vracaju svi koji se izbrisu.
	void write();
private:
	ElemS *prvi;
	ElemS *posl;
};

#endif /* PCBSL_H_ */
