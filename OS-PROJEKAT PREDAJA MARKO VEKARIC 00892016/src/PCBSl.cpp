/*
 * PCBSl.cpp
 *
 *  Created on: Aug 14, 2018
 *      Author: OS1
 */

#include "PCBSl.h"
#include "Global.h"
#include <dos.h>
#include "SCHEDULER.H"

PCBSl::PCBSl() {
	{ prvi = 0; posl = 0; }// TODO Auto-generated constructor stub

}

PCBSl::~PCBSl() {
	ElemS* tek=prvi;
		while(prvi!=0){
			tek=prvi;prvi=prvi->sled;delete tek;
		}
		prvi=0;
		posl=0;
}


void PCBSl::insert(PCB* pcb,int time){
	asm cli;
	ElemS *e=new ElemS(pcb,time);
	this->ins(e);
	asm sti;
}

void PCBSl::ins(ElemS *e){
	asm cli;
	if (e->time <= 0){asm sti;return;}
		if (prvi == 0) {
			prvi = e;
			posl = prvi;
		}
		else {
			ElemS* pret = 0;
			ElemS* tek = prvi;
			while (1) {

				if (e->time > tek->time) {
					e->time -= tek->time;
					pret = tek;
					tek = tek->sled;
					if (tek == 0) {
						pret->sled = e;
						posl = e;
						break;
					}
				}
				else if (e->time == tek->time) {
					e->time = 0;
					e->sled = tek->sled;
					tek->sled = e;
					if (posl == tek)posl = e;
					break;
				}
				else if (e->time < tek->time) {
					if (pret==0){
						e->sled = tek;
						prvi = e;
					}
					else {
						pret->sled = e;
						e->sled = tek;
					}
					tek->time -= e->time;
					break;
				}
			}
			asm sti;
		}

}


void PCBSl::del(){ //posle ovoga u prekidnoj ruti ni mora cli;
	if (prvi == 0) {
		Global::sleepDispatch=0;
		return;
		}
		prvi->time -= 1;

		if (prvi->time == 0) {
			ElemS *tek = prvi;
			while (1) {
				if (prvi->time == 0) {
					tek = prvi;
					prvi = prvi->sled;
					tek->pcb->moreToSleep=0;
					tek->pcb->sleepFlag=0;
					if (tek->pcb->blocked==0)
					Scheduler::put(tek->pcb);
					/*Global::syncCout("Izasla nit iz sleepa: ",tek->pcb->id);*/
					//UBACI PCB U SCHEDULER NAZAD
					delete tek;
					if (prvi == 0) { posl = 0; break; }
				}
				else break;
			}
			Global::sleepDispatch=1;
		}
		else Global::sleepDispatch=0;
} //u ovome ce i u scheduler da se vracaju svi koji se izbrisu.


void PCBSl::write(){
	ElemS* tek = prvi;
		while (tek != 0) {
			Global::syncCout("PCB timetosleep: ",tek->time);
			tek = tek->sled;
		}

}



