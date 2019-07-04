/*
 * EvList.cpp
 *
 *  Created on: Aug 26, 2018
 *      Author: OS1
 */

#include "EvList.h"
#include "KernelEv.h"
#include "Global.h"
#include "iostream.h"
EvList::EvList() {
	// TODO Auto-generated constructor stub
	prvi=0;
	posl=0;

}

EvList::~EvList() {
	ElemEv* tek=prvi;
		while(prvi!=0){
			tek=prvi;prvi=prvi->sled;delete tek;
		}
		prvi=0;
		posl=0;
	// TODO Auto-generated destructor stub
}




int EvList::del(int idd){
	asm cli;
	ElemEv *tek=prvi;
	ElemEv* pret=0;
	int found=0;
	while(tek!=0){
		if (tek->data->id==idd){found =1;break;}
		pret=tek;
		tek=tek->sled;
	}
	if (found==0) {Global::syncCout("Nije pronadjen taj event",0);asm sti;return -1;}
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


KernelEv* EvList::get(int idd){
	ElemEv* tek = prvi;
	while (tek != 0) {
		if (tek->data->id == idd) return tek->data;
		tek=tek->sled;
	}
	return 0;
}


int EvList::ins(KernelEv* ke){
	asm cli;
	ElemEv* novi=new ElemEv(ke);
	if (posl!=0){
		posl->sled=novi;
		posl=novi;
	}
	else{
		prvi=novi;
		posl=novi;
	}
	asm sti;
	return ke->id;
}

void EvList::write(){
	asm cli;
	ElemEv* tek=prvi;
	while(tek!=0){
		cout<<tek->data->id<<endl;
		tek=tek->sled;
	}
	asm sti;
}

KernelEv* EvList::firstDel(){
	asm cli;//dobra
	if (prvi!=0){
	ElemEv* tek=prvi;
	KernelEv* ret=tek->data;
	prvi=prvi->sled;
	if(prvi==0) posl=0;
	delete tek;
	asm sti;
	return ret;
	}else{
		asm sti;
		return 0;
	}
}


