/*
 * SemList.cpp
 *
 *  Created on: Aug 15, 2018
 *      Author: OS1
 */
#include "SemList.h"
#include "kernsem.h"
#include <iostream.h>
#include <dos.h>
#include "Global.h"

SemList::SemList(){
	prvi=0;
	posl=0;
}


SemList::~SemList(){
	ElemSem* tek=prvi;
		while(prvi!=0){
			tek=prvi;prvi=prvi->sled;delete tek;
		}
		prvi=0;
		posl=0;

}


int SemList::del(int idsem){
		asm cli;
		ElemSem* tek=prvi;
		ElemSem* pret=0;
		int found=0;
		while(tek!=0){
			if (tek->sem->id==idsem){found=1;break;}
			pret=tek;
			tek=tek->sled;
		}
		if (found==0) {asm sti;return -1;}
		if (pret==0){
			pret=prvi;
			prvi=prvi->sled;
			delete pret;
			if (prvi==0)posl=0;
		}
		else{
		pret->sled=tek->sled;
		if (posl==tek)posl=pret;
		delete tek;
		}
		asm sti;
		return 0;

}


KernelSem* SemList::firstDel(){
	asm cli;
		if (prvi!=0){
		ElemSem* tek=prvi;
		KernelSem* ret=tek->sem;
		prvi=prvi->sled;
		if(prvi==0) posl=0;
		delete tek;
		asm sti;
		return ret;
		}else {asm sti;return 0;}

}


KernelSem* SemList::get(int semid){
		ElemSem* tek = prvi;
		while (tek != 0) {
			if (tek->sem->id == semid) return tek->sem;
			tek=tek->sled;
		}
		return 0;

}

int SemList::ins(KernelSem *sem){
	asm cli;
		ElemSem* novi=new ElemSem(sem);
		if (posl!=0){
			posl->sled=novi;
			posl=novi;
		}
		else{
			prvi=novi;
			posl=novi;
		}

		asm sti;
		return sem->id;

}

void SemList::write(){
		asm cli;
		ElemSem* tek=prvi;
		while(tek!=0){
			cout<<tek->sem->id<<endl;
			tek=tek->sled;
		}
		asm sti;

}
