/*
 * thread.cpp
 *
 *  Created on: Aug 5, 2018
 *      Author: OS1
 */


#include "Global.h"
#include <dos.h>
#include "PCB.h"
#include "thread.h"
#include "PCBList.h"
#include "PCBSl.h"
#include "SCHEDULE.H"
#include <iostream.h>


class PCBList;
ID Thread::idpos=-2; //-2 je main, -1 je idle, 0 je Kernel

unsigned structOff;
unsigned structSeg;
Thread::Thread(StackSize stackSize, Time timeSlice) {//5 za Thread()
	if (idpos==-2){
		myID=++idpos;
		new PCB(stackSize, timeSlice, this);
	}else{

		#ifndef BCC_BLOCK_IGNORE
			asm {	//cuvamo registre koji ce se menjati
				push cx
				push dx
				push bx //nije potrebno.
			}
		#endif

			sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
			myID=++idpos;
			newStruct.id=5;//5 za konstruktor Threada
			newStruct.idThread=myID;
			newStruct.stackSize=stackSize;
			newStruct.timeSlice=timeSlice;
			newStruct.myThread=this;
			   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
			asm cli;
		#ifndef BCC_BLOCK_IGNORE
			structOff=FP_OFF(&newStruct);
			structSeg=FP_SEG(&newStruct);
		#endif


			asm{
				mov cx,structSeg
				mov dx,structOff  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
			}


			asm sti;  // sa onim gore clijem.

			asm int 61h;

			asm{
				pop bx// vracamo sacuvane registre
				pop dx
				pop cx
			}
	}//kraj elsa
			/*
	;
	//smesti PCB u listu, U KONSTRUKTORU!.
	*/
}



Thread::~Thread() {


	#ifndef BCC_BLOCK_IGNORE
		asm {	//cuvamo registre koji ce se menjati
			push cx
			push dx
			push bx //nije potrebno.
		}
	#endif

		sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
		newStruct.id=4;
		newStruct.idThread=this->getId();//4 za destruktor Threada
		   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
		asm cli;
	#ifndef BCC_BLOCK_IGNORE
		structOff=FP_OFF(&newStruct);
		structSeg=FP_SEG(&newStruct);
	#endif


		asm{
			mov cx,structSeg
			mov dx,structOff  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
		}


		asm sti;  // sa onim gore clijem.

		asm int 61h;

		asm{
			pop bx// vracamo sacuvane registre
			pop dx
			pop cx
		}

	// obrisati PCB iz liste

    //waitToComplete() mozeda ce biti pozvan iz izvedene klase. Pa ovaj kod nije potreban.
		//STARI DESTRUKTOR
	/*PCB* p=Global::PCBs->get(this->myID);
	Global::PCBs->del(p->id);//-1 ako nije pronadjen 0 ako jeste.
	delete p;*/
		//STARI DESTRUKTOR
	// TODO Auto-generated destructor stub
}

ID Thread::getId(){
	return this->myID;
}


void Thread::waitToComplete(){

#ifndef BCC_BLOCK_IGNORE
	asm {	//cuvamo registre koji ce se menjati
		push cx
		push dx
		push bx
	}
#endif
	sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
	newStruct.id=1;  //1 za waitTocomplete sifra
	newStruct.idThread=this->getId();   // sifra threada u cijoj listi treba da se blokira
	newStruct.idRunning=Global::running->id;  //sifra running threada
	asm cli;   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
#ifndef BCC_BLOCK_IGNORE
	structOff=FP_OFF(&newStruct);
	structSeg=FP_SEG(&newStruct);
#endif


	asm{
		mov cx,structSeg
		mov dx,structOff  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
	}

	asm sti;   // sa onim gore clijem.
	asm int 61h;

	asm{
		pop bx// vracamo sacuvane registre
		pop dx
		pop cx
	}


	//STARA RUTINA WAITTOCOMPLETE//
/*	// trenutna running nit ceka zavrsetak niti ciju je waitToComplete metodu pozvala.
	if ((Global::PCBs->get(this->getId()))->started==0) return;
	if ((Global::PCBs->get(this->getId()))->finished==1) return;
	(Global::PCBs->get(this->getId()))->waitingPCBs->ins((PCB*)Global::running);
	Global::running->blocked=1;
	if (this!=0)
	Global::syncCout("Pozvano waitToComplete niti: ",this->myID);
	dispatch();
	//1. stavi runnig u listu blokiranih na niti za koju je pozvana
	//2. staviti mu status na blokiran.
	//3. promeniti kontekst*/

}



void Thread::start(){



#ifndef BCC_BLOCK_IGNORE
	asm {	//cuvamo registre koji ce se menjati
		push cx
		push dx
		push bx
	}
#endif

	sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
	newStruct.id=0; //0 za start
	newStruct.idThread=this->getId();
	asm cli;
#ifndef BCC_BLOCK_IGNORE
	structOff=FP_OFF(&newStruct);
	structSeg=FP_SEG(&newStruct);
#endif

	asm{
		mov cx,structSeg
		mov dx,structOff  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
	}

	asm sti; // sa onim gore clijem, razlog objasnjen iznad.
	asm int 61h;

	asm{
		pop bx// vracamo sacuvane registre
		pop dx
		pop cx
	}
	/*if (myPCB->started==0){
		myPCB->started=1;
		myPCB->mode=0;// obelezena nit kao spremna
		Scheduler::put(myPCB);
		Global::syncCout("Startovana nit: ",myID);
	} start pre preuredjivanja*/

}




void dispatch(){ // sinhrona promena konteksta
	asm cli;
#ifndef BCC_BLOCK_IGNORE
	asm {	//cuvamo registre koji ce se menjati
		push cx
		push dx
		push bx //nije potrebno.
	}
#endif

	sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
	newStruct.id=2;  //2 za dispatch()
	   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to

#ifndef BCC_BLOCK_IGNORE
	structOff=FP_OFF(&newStruct);
	structSeg=FP_SEG(&newStruct);
#endif


	asm{
		mov cx,structSeg
		mov dx,structOff  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
	}


	asm sti;  // sa onim gore clijem.
	asm int 61h;

	asm{
		pop bx// vracamo sacuvane registre
		pop dx
		pop cx
	}


	/* STARI DISPATCH*/
	/*asm cli;
	Global::explicitDispatch = 1;
	timer();
	asm sti;*/
}





void Thread::sleep(Time timeToSleep){
	asm cli;
#ifndef BCC_BLOCK_IGNORE
	asm {	//cuvamo registre koji ce se menjati
		push cx
		push dx
		push bx //nije potrebno.
	}
#endif

	sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
	newStruct.id=3;
	newStruct.sleepTime=timeToSleep;//3 za sleep
	   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
#ifndef BCC_BLOCK_IGNORE
	structOff=FP_OFF(&newStruct);
	structSeg=FP_SEG(&newStruct);
#endif


	asm{
		mov cx,structSeg
		mov dx,structOff  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
	}


	asm sti;  // sa onim gore clijem.
	asm int 61h;

	asm{
		pop bx// vracamo sacuvane registre
		pop dx
		pop cx
	}
/*
Global::running->moreToSleep=timeToSleep;
Global::running->sleepFlag=1;
Global::syncCout("Usla nit u spavanje: ",Global::running->id);
Global::SleepingPCBs->insert((PCB*)Global::running,timeToSleep);
dispatch();*/

}
