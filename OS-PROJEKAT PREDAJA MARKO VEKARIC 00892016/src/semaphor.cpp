/*
 * semaphor.cpp
 *
 *  Created on: Aug 15, 2018
 *      Author: OS1
 */
#include "semaphor.h"
#include "kernsem.h"
#include "Global.h"
#include "SemList.h"
#include "iostream.h"
#include <dos.h>
#include "PCB.h"

#include "kernsem.h"
int Semaphore::posId=0;
Semaphore::Semaphore(int init){

						#ifndef BCC_BLOCK_IGNORE
							asm {	//cuvamo registre koji ce se menjati
								push cx
								push dx
								push bx //nije potrebno.
							}
						#endif
							idSem=++posId;
							sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
							newStruct.semInit=init;
							newStruct.id=9;//9 za konstruktor
							newStruct.idSem=this->idSem;
							newStruct.mySemaphore=this;
							   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
							asm cli;
						#ifndef BCC_BLOCK_IGNORE
							structOff2=FP_OFF(&newStruct);
							structSeg2=FP_SEG(&newStruct);
						#endif


							asm{
								mov cx,structSeg2
								mov dx,structOff2  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
							}


							asm sti;  // sa onim gore clijem.
							asm int 61h;

							asm{
								pop bx// vracamo sacuvane registre
								pop dx
								pop cx
							}



	/*idSem=++posId;
	 new KernelSem(init,this);*/

}

int Semaphore::getId(){
	return idSem;
}


Semaphore::~Semaphore(){

						#ifndef BCC_BLOCK_IGNORE
							asm {	//cuvamo registre koji ce se menjati
								push cx
								push dx
								push bx //nije potrebno.
							}
						#endif

							sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
							newStruct.id=10;//10 za destruktor
							newStruct.idSem=this->idSem;
							   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
							asm cli;
						#ifndef BCC_BLOCK_IGNORE
							structOff2=FP_OFF(&newStruct);
							structSeg2=FP_SEG(&newStruct);
						#endif


							asm{
								mov cx,structSeg2
								mov dx,structOff2  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
							}


							asm sti;  // sa onim gore clijem.
							asm int 61h;

							asm{
								pop bx// vracamo sacuvane registre
								pop dx
								pop cx
							}



		/*KernelSem* p=Global::Semaphores->get(this->getId());
		Global::Semaphores->del(p->id);//-1 ako nije pronadjen 0 ako jeste.
		delete p;*/

}

unsigned structSeg2;
unsigned structOff2;

int Semaphore::wait(int toBlock){

#ifndef BCC_BLOCK_IGNORE
	asm {	//cuvamo registre koji ce se menjati
		push cx
		push dx
		push bx //nije potrebno.
	}
#endif

	sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
	newStruct.id=6;
	newStruct.toBlock=toBlock;//6 za wait
	newStruct.idSem=this->idSem;

	   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
	asm cli;
#ifndef BCC_BLOCK_IGNORE
	structOff2=FP_OFF(&newStruct);
	structSeg2=FP_SEG(&newStruct);



	asm{
		mov cx,structSeg2
		mov dx,structOff2  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
	}


	asm sti;  // sa onim gore clijem.
	asm int 61h;

	asm{
		pop bx// vracamo sacuvane registre
		pop dx
		pop cx
	}
#endif


return Global::running->retWait;

	/*return Global::Semaphores->get(this->getId())->wait(toBlock);*/
}


void Semaphore::signal(){

				#ifndef BCC_BLOCK_IGNORE
					asm {	//cuvamo registre koji ce se menjati
						push cx
						push dx
						push bx //nije potrebno.
					}
				#endif

					sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
					newStruct.id=7;//7 za signal
					newStruct.idSem=this->idSem;
					   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
					asm cli;
#ifndef BCC_BLOCK_IGNORE
					structOff2=FP_OFF(&newStruct);
					structSeg2=FP_SEG(&newStruct);
#endif
					//printf

					asm{
						mov cx,structSeg2
						mov dx,structOff2  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
					}


					asm sti;// sa onim gore clijem.

					asm int 61h;

					asm{
						pop bx// vracamo sacuvane registre
						pop dx
						pop cx
					}
	/*Global::Semaphores->get(this->getId())->signal();*/
}


int Semaphore::val()const{

					#ifndef BCC_BLOCK_IGNORE
						asm {	//cuvamo registre koji ce se menjati
							push cx
							push dx
							push bx //nije potrebno.
						}
					#endif

						sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
						newStruct.id=8;//8 za value
						newStruct.idSem=this->idSem;
						   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
						asm cli;
					#ifndef BCC_BLOCK_IGNORE
						structOff2=FP_OFF(&newStruct);
						structSeg2=FP_SEG(&newStruct);
					#endif


						asm{
							mov cx,structSeg2
							mov dx,structOff2  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
						}


						asm sti;  // sa onim gore clijem.
						asm int 61h;

						asm{
							pop bx// vracamo sacuvane registre
							pop dx
							pop cx
						}


	return Global::running->retValue;
	/* return Global::Semaphores->get( ((Semaphore*)this)->getId() )->val();  //mora cast zbog consta*/
}





