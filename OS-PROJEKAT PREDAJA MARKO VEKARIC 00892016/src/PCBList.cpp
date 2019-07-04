/*
 * PCBList.cpp
 *
 *  Created on: Aug 8, 2018
 *      Author: OS1
 */

#include "PCBList.h"
#include "PCB.h"
#include <iostream.h>
#include "Thread.h"
#include <dos.h>
#include "Global.h"
PCBList::PCBList() {
	// TODO Auto-generated constructor stub
	prvi=0;
	posl=0;
}

PCBList::~PCBList() {
	Elem* tek=prvi;
	while(prvi!=0){
		tek=prvi;prvi=prvi->sled;delete tek;
	}
	prvi=0;
	posl=0;

}

int PCBList::del(int idd){
	asm cli;
	Elem *tek=prvi;
	Elem* pret=0;
	int found=0;
	while(tek!=0){
		if (tek->data->id==idd){found =1;break;}
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


PCB* PCBList::get(int idd){
	Elem* tek = prvi;
	while (tek != 0) {
		if (tek->data->id == idd) return tek->data;
		tek=tek->sled;
	}
	return 0;
}


int PCBList::ins(PCB* pcb){
	asm cli;
	Elem* novi=new Elem(pcb);
	if (posl!=0){
		posl->sled=novi;
		posl=novi;
	}
	else{
		prvi=novi;
		posl=novi;
	}
	asm sti;
	return pcb->id;
}

void PCBList::write(){
	asm cli;
	Elem* tek=prvi;
	while(tek!=0){
		cout<<tek->data->id<<endl;
		tek=tek->sled;
	}
	asm sti;
}

PCB* PCBList::firstDel(){
	asm cli;//dobra
	if (prvi!=0){
	Elem* tek=prvi;
	PCB* ret=tek->data;
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

void PCBList::waitAll(){
	    asm cli;
	    cout<<"Zapoceto cekanje ostalih";
	    asm sti;
		Elem* tek=prvi;
		while(tek!=0){
			if (tek->data->id!=Global::running->id){
			tek->data->myThread->waitToComplete();
			}
			tek=tek->sled;
		}

		asm cli;
	    cout<<"Zavrseno cekanje ostalih";
	    asm sti;
}


