/*
 * SemList.h
 *
 *  Created on: Aug 15, 2018
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_

class KernelSem;

typedef struct ElemSem{
	KernelSem* sem;
	ElemSem* sled;
	ElemSem(KernelSem *s){
		sem=s;
		sled=0;
	}
	~ElemSem(){
		sem=0;
		sled=0;
	}
}ElemSem;

class SemList{
public:
	SemList();
	virtual ~SemList();
	int del(int idsem);
	KernelSem* firstDel();
	KernelSem* get(int idsem);
	int ins(KernelSem *sem);
	ElemSem* prvi;
	ElemSem* posl;
	void write();

};




#endif /* SEMLIST_H_ */
