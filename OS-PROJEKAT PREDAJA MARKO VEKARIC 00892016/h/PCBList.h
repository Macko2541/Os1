/*
 * PCBList.h
 *
 *  Created on: Aug 8, 2018
 *      Author: OS1
 */

#ifndef PCBLIST_H_
#define PCBLIST_H_
class PCB;
typedef struct Elem{
		PCB* data;
		Elem* sled;
		Elem(PCB *p){
			data=p;
			sled=0;
		}
		~Elem(){
			data=0;
			sled=0;
		}
	}Elem;
class PCBList {
public:

	PCBList();
	virtual ~PCBList();
	int del(int id);
	PCB* firstDel();
	PCB* get(int id);
	int ins(PCB* pcb);
	Elem* prvi;
	Elem* posl;
	void write();
	void waitAll();
};

#endif /* PCBLIST_H_ */
