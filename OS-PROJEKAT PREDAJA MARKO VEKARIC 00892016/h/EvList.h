/*
 * EvList.h
 *
 *  Created on: Aug 26, 2018
 *      Author: OS1
 */

#ifndef EVLIST_H_
#define EVLIST_H_
class KernelEv;
typedef struct ElemEv{
		KernelEv* data;
		ElemEv* sled;
		ElemEv(KernelEv *p){
			data=p;
			sled=0;
		}
		~ElemEv(){
			data=0;
			sled=0;
		}
	}ElemEv;
class EvList {
public:
	EvList();
	virtual ~EvList();
	int del(int id);
	KernelEv* firstDel();
	KernelEv* get(int id);
	int ins(KernelEv* ke);
	ElemEv* prvi;
	ElemEv* posl;
	void write();

};

#endif /* EVLIST_H_ */
