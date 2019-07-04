/*
 * kernel.cpp
 *
 *  Created on: Aug 20, 2018
 *      Author: OS1
 */

#include "kernel.h"
#include "Global.h"
#include "PCBList.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "IdleT.h"
#include "PCBSl.h"
#include "kernsem.h"
#include "SemList.h"
#include "iostream.h"
#include "EvList.h"
#include "KernelEv.h"


typedef unsigned int Time;
typedef unsigned long StackSize;

KernelThread::KernelThread():Thread(){



}

KernelThread::~KernelThread() {


}

void KernelThread::_dispatch(){

	Global::explicitDispatch=1;
}

extern sysStruct* globalStruct;

void KernelThread::run(){
	unsigned int callId;
    int threadId;
    int runningId;
    unsigned int tts;
    Time timeSlice;
    StackSize stackSize;
    Thread* myThread;
	while (Global::oneKernel){
		Global::lockPreemt(); /*bio asm cli;*/
		callId=globalStruct->id;

		switch(callId){
		case 0:{//start metod Threada
			        threadId=globalStruct->idThread;  //id threada koji cemo startovati
					PCB* pom;
					pom=Global::PCBs->get(threadId);
					if (pom->started==0){
					pom->started=1;
					Scheduler::put(pom);
				}
		break;
		}// end of start methid - case0

		case 1:{//waitToComplete metod Threada
				threadId=globalStruct->idThread;
				runningId=globalStruct->idRunning;
				// trenutna running nit ceka zavrsetak niti ciju je waitToComplete metodu pozvala.
				if ( ((Global::PCBs)->get(threadId))->started ==0) break;
				if ((Global::PCBs->get(threadId))->finished==1) break;
				((Global::PCBs->get(threadId))->waitingPCBs)->ins((PCB*)(Global::PCBs->get(runningId)));
				Global::running->blocked=1;
				Global::explicitDispatch=1;  //samo promeni explicitDispatch flag
				//1. stavi runnig u listu blokiranih na niti za koju je pozvana
				//2. staviti mu status na blokiran.
				//3. promeniti kontekst


			break;
		}//end of wait method - case1


		case 2:{ // za dispatch()
			Global::explicitDispatch=1;
			break;
		}

		case 3:{//za sleep()
			tts=globalStruct->sleepTime;
			Global::running->moreToSleep=tts;
			Global::running->sleepFlag=1;
			Global::SleepingPCBs->insert((PCB*)Global::running,tts);
			Global::explicitDispatch=1;
			break;
		}

		case 4:{//za destruktor Threada
			threadId=globalStruct->idThread;// za destrutkor Threada
			PCB* p=Global::PCBs->get(threadId);
			Global::PCBs->del(threadId);//-1 ako nije pronadjen 0 ako jeste.
			delete p;
			break;
		}

		case 5:{//za konstruktor Threada
			myThread=globalStruct->myThread;// za konstrutkor Threada
			timeSlice=globalStruct->timeSlice;
			stackSize=globalStruct->stackSize;
			new PCB(stackSize, timeSlice, myThread);//smesti PCB u listu, U KONSTRUKTORU!.
			break;
		}

		case 6:{ //za wait Sempahorea
			 // drugaciji lock
			// ovde treba drugacije zakljucavanje.
				int toBlock=globalStruct->toBlock;
				int idSem=globalStruct->idSem;
				KernelSem* k=Global::Semaphores->get(idSem);
				Global::running->retWait=k->wait(toBlock);	//povratna vrednost
			break;
		}

		case 7:{//za signal Semaphorea

			int idSem=globalStruct->idSem;
			KernelSem* k=Global::Semaphores->get(idSem);
			k->signal();
			break;
		}

		case 8:{//Za value semaphorea
			int idSem=globalStruct->idSem;
			KernelSem* k=Global::Semaphores->get(idSem);
			Global::running->retValue=k->value;
			break;
		}

		case 9:{//konstruktor semafora
			Semaphore *s=globalStruct->mySemaphore;
			int init=globalStruct->semInit;
			new KernelSem(init,s);
			break;
		}

		case 10:{ //destruktor semafora
			int idSem=globalStruct->idSem;
			KernelSem* p=Global::Semaphores->get(idSem);
			Global::Semaphores->del(p->id);//-1 ako nije pronadjen 0 ako jeste.
			delete p;

			break;
		}
		case 11:{//wait eventa
			int idEv=globalStruct->idEve;
			KernelEv* p=Global::Events->get(idEv);
			p->wait();

			break;
		}

		case 12:{//za signal Eventa
			int idEv=globalStruct->idEve;
			KernelEv* p=Global::Events->get(idEv);
			p->signal();

			break;
			}

		case 13:{//case 13 za destruktor Eventa
			int idEv=globalStruct->idEve;
			KernelEv* p=Global::Events->get(idEv);
			Global::Events->del(p->id);//-1 ako nije pronadjen 0 ako jeste.
			delete p;
			break;
		}

		case 14:{//case 14 za konstruktor Eventa
			IVTNo ivtNo=globalStruct->ivtNo;
			int idEve=globalStruct->idEve;
			new KernelEv(ivtNo,idEve);
		break;
		}




		};//end of switch





		Global::unlockPreemt(); /*bili asm sti;*/
	asm int 63h;/*poziv prekidne rutine za povratak na kernel nit. int 63;*/

	}//end of while

}//end of run
