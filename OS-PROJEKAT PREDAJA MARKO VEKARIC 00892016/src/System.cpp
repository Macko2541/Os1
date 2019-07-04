/*
 * System.cpp
 *
 *  Created on: Aug 5, 2018
 *      Author: OS1
 */
#include "System.h"
#include <iostream.h>
#include <dos.h>
#include <stdio.h>
#include "PCB.h"
#include "Global.h"
#include "PCBList.h"
#include "SCHEDULE.H"
#include "IdleT.h"
#include "PCBSl.h"
#include "SemList.h"
#include "kernel.h"
#include "thread.h"
#include "EvList.h"

unsigned oldTimerSEG;
unsigned oldTimerOFF;
unsigned tss;
unsigned tsp;
unsigned tbp;
void tick();
typedef int ID;
typedef void interrupt (*pInterrupt)(...);


void interrupt timer(){
	Global::counter--;
	tick();
	Global::SleepingPCBs->del();
	asm cli;


	/*Global::syncCout("Global counter: ",Global::counter);
	asm cli;*/
	if (Global::counter==0){// promena konteksta. UBACI I BP U PRICU
		if (Global::lockFlag==0){

			asm{
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			/*Global::syncCout("SP NITI KOJA SE STAVLJA U SCHEDULER: ",Global::running->sp);
			asm cli;
			Global::syncCout("SS NITI KOJA SE STAVLJA U SCHEDULER: ",Global::running->ss);
			asm cli;
			Global::syncCout("BP NITI KOJA SE STAVLJA U SCHEDULER: ",Global::running->bp);
			asm cli;*/

			(Global::running)->sp=tsp;
			(Global::running)->ss=tss;
			(Global::running)->bp=tbp;


			if (Global::running->finished==0 && Global::running->blocked==0 && Global::running->started==1 && Global::running->sleepFlag==0 && !(Global::running->id==0) && Global::running->semBlocked==0 && Global::running->eventBlocked==0){
				Scheduler::put((PCB*)Global::running);
			}
			//STAVI MU START na 1!

			/*if (Global::running->blocked==1 && Global::running->finished==0 && Global::running->sleepFlag==0 && !(Global::running->id==-1) && Global::running->semBlocked==0 && Global::running->eventBlocked==0){
				Global::running->mode=0; //nije running nego je spremna ali blokirana
				Global::BlockedPCBs->ins((PCB*)Global::running); // samo se ovde ubacuje u listu blokiranih, ovde su samo blokirane niti za wtc
				/*Global::syncCout("Ubacena nit u BlockedPCBs listu: ",Global::running->id);
				asm cli;
			}*/
			Global::running=Scheduler::get();





			if (Global::running==0){

				//U OVOME MOZE BITI GRESKA
			     // ako su sve niti blokirane ili spavaju.
				Global::running=Global::PCBs->get(Global::idle->getId());

			}


			tsp=(Global::running)->sp;
			tss=(Global::running)->ss;
			tbp=(Global::running)->bp;
			Global::counter=(Global::running)->timeSlice;
			asm{
				mov sp,tsp
				mov ss,tss
				mov bp,tbp
			}


		/*	Global::syncCout("Gotova rutnina tajmer niti : ",Global::running->id);
			asm cli;*/
		 // kraj lockflaga
		}
		else Global::explicitDispatch=1;

	}
	asm int 60h;
}


sysStruct* globalStruct=0;
unsigned tcx,tdx;
unsigned teksp, tekss, tekbp;

void interrupt sys_call(...){//int 61
	asm{
			mov teksp, sp //ove instrukcije moraju ici na pocetak jer ako ih stavimo u sredinu stek moze da se poremeti u medjuvremenu(pomocne operacije mozda zahtevaju stekovsko pushovanje i popovanje)
			mov tekss, ss
			mov tekbp, bp
		}

	asm{
		mov tcx, cx
		mov tdx, dx
	}
	/*cout<<tcx<<endl;
	cout<<tdx<<endl;*/
	globalStruct=(sysStruct*)MK_FP(tcx,tdx);


	Global::running->sp=teksp; //cuvanje konteksta stare niti
	Global::running->ss=tekss;
	Global::running->bp=tekbp;

	teksp=Global::runningKernelThread->sp; //davanje konteksta kernel niti
	tekss=Global::runningKernelThread->ss;
	tekbp=Global::runningKernelThread->bp;

	asm{
		mov sp,teksp
		mov ss,tekss
		mov bp,tekbp
	}							//namestimo ss sp i bp
								//cuvanje konteksta korisnicke niti i prebacivanje na kernel nit.
}

unsigned bsp,bss,bbp;
void interrupt sys_back(...){//int 63

	asm{
			mov bsp, sp
			mov bss, ss
			mov bbp, bp
		}
	Global::runningKernelThread->sp=bsp; //cuvanje konteksta kernel niti
	Global::runningKernelThread->ss=bss;
	Global::runningKernelThread->bp=bbp;


	if (Global::explicitDispatch==1){

				 if (Global::running->finished==0 && Global::running->blocked==0 && Global::running->started==1 && Global::running->sleepFlag==0 && !(Global::running->id==0) && Global::running->semBlocked==0 && Global::running->eventBlocked==0){
					Scheduler::put((PCB*)Global::running);   //vracamo nit i uzimamo drugu u koju cemo se vratiti ako slucajno nit nije blokirana.
				 }
						Global::running=Scheduler::get();

						if (Global::running==0){
						 // ako su sve niti blokirane ili spavaju.
						Global::running=Global::PCBs->get(Global::idle->getId());

						}
						Global::counter=(Global::running)->timeSlice;
						Global::explicitDispatch=0;
				}


	bsp=Global::running->sp; //davanje konteksta korisnickoj niti
	bss=Global::running->ss;
	bbp=Global::running->bp;

	asm{
			mov sp,bsp
			mov ss,bss
			mov bp,bbp
		}


}

pInterrupt sysc=&sys_call;
pInterrupt sysb=&sys_back;
pInterrupt oldInt61;
pInterrupt oldInt63;
void System::inic(){

#ifndef BCC_BLOCK_IGNORE
	asm{
			cli
			push es
			push ax

			mov ax,0   //  ; inicijalizuje rutinu za tajmer
			mov es,ax

			mov ax, word ptr es:0022h //; pamti staru rutinu
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax

			mov word ptr es:0022h, seg timer	 //postavlja
			mov word ptr es:0020h, offset timer //novu rutinu

			mov ax, oldTimerSEG	 //	postavlja staru rutinu
			mov word ptr es:0182h, ax //; na int 60h
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax

			pop ax
			pop es


		}



	oldInt61=getvect(0x61);
	oldInt63=getvect(0x63);
	setvect(0x61,sysc);
	setvect(0x63,sysb);

#endif

	Global::PCBs=new PCBList();
	Global::mainPCB=new PCB();//u konsturktoru ubacena u PCBs listu.
	Global::running=Global::mainPCB;
	Global::SleepingPCBs=new PCBSl();
	Global::Semaphores=new SemList();
	Global::Events=new EvList();
	Global::runningKT=new KernelThread();//u konsturktoru ubacena u PCBs listu.
	Global::runningKernelThread=Global::PCBs->get((Global::runningKT)->getId());
	Global::idle=new IdleT();//u konstruktoru ubacena u PCBs listu.
	asm sti;
}

void System::restore(){

	asm {
			cli
			push es
			push ax

			mov ax,0
			mov es,ax


			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop ax
			pop es
	}
	setvect(0x61,oldInt61);
	setvect(0x63,oldInt63);
	asm sti;
	/*delete Global::KT;*/
}

/*
    typedef void interrupt(*pInterrupt)(...);
    pInterrupt oldPrekid;
    void inic(){
  	asm cli;
  	oldPrekid=getvect(8);
  	setvect(8,timer);
  	setvect(60,oldPrekid);
  	asm sti;
  }

  void restore(){
  asm cli;
  setvect(8,oldPrekid);
  asm sti;
  }

*/





