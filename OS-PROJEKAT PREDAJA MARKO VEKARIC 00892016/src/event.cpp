/*
 * event.cpp
 *
 *  Created on: Aug 17, 2018
 *      Author: OS1
 */

#include "event.h"
#include "KernelEv.h"
#include "Global.h"
#include "EvList.h"
#include <dos.h>

unsigned structSeg3;
unsigned structOff3;

int Event::posId=0;
Event::Event(IVTNo ivtNo) {

			#ifndef BCC_BLOCK_IGNORE
				asm {	//cuvamo registre koji ce se menjati
					push cx
					push dx
					push bx //nije potrebno.
				}
			#endif
				id=++posId;
				sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
				newStruct.id=14;//14 za konstruktor na eventu
				newStruct.idEve=this->id;
				newStruct.ivtNo=ivtNo;
				   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
				asm cli;
			#ifndef BCC_BLOCK_IGNORE
				structOff3=FP_OFF(&newStruct);
				structSeg3=FP_SEG(&newStruct);



				asm{
					mov cx,structSeg3
					mov dx,structOff3  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
				}


				asm sti;  // sa onim gore clijem.
				asm int 61h;

				asm{
					pop bx// vracamo sacuvane registre
					pop dx
					pop cx
				}
			#endif

	/*new KernelEv(ivtNo,id);*/// TODO Auto-generated constructor stub
}

Event::~Event() {

		#ifndef BCC_BLOCK_IGNORE
			asm {	//cuvamo registre koji ce se menjati
				push cx
				push dx
				push bx //nije potrebno.
			}
		#endif

			sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
			newStruct.id=13;//13 za destruktor na eventu
			newStruct.idEve=this->id;
			   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
			asm cli;
		#ifndef BCC_BLOCK_IGNORE
			structOff3=FP_OFF(&newStruct);
			structSeg3=FP_SEG(&newStruct);



			asm{
				mov cx,structSeg3
				mov dx,structOff3  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
			}


			asm sti;  // sa onim gore clijem.
			asm int 61h;

			asm{
				pop bx// vracamo sacuvane registre
				pop dx
				pop cx
			}
		#endif

	/*KernelEv* p=Global::Events->get(this->id);
	Global::Events->del(p->id);//-1 ako nije pronadjen 0 ako jeste.
	delete p;*/
}


void Event::wait(){

	#ifndef BCC_BLOCK_IGNORE
		asm {	//cuvamo registre koji ce se menjati
			push cx
			push dx
			push bx //nije potrebno.
		}
	#endif

		sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
		newStruct.id=11;//11 za wait na eventu
		newStruct.idEve=this->id;
		   // da jedna nit ne krene da upisuje ofset a onda druga nit utrci i promeni to
		asm cli;
	#ifndef BCC_BLOCK_IGNORE
		structOff3=FP_OFF(&newStruct);
		structSeg3=FP_SEG(&newStruct);



		asm{
			mov cx,structSeg3
			mov dx,structOff3  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
		}


		asm sti;  // sa onim gore clijem.
		asm int 61h;

		asm{
			pop bx// vracamo sacuvane registre
			pop dx
			pop cx
		}
	#endif


	/*Global::Events->get(this->id)->wait();*/

}


void Event::signal(){

	#ifndef BCC_BLOCK_IGNORE
		asm {	//cuvamo registre koji ce se menjati
			push cx
			push dx
			push bx //nije potrebno.
		}
	#endif

		sysStruct newStruct; //stvaramo lokalnu strukturu sa parametrima
		newStruct.id=12;//12 za signal na eventu
		newStruct.idEve=this->id;
		   // da jedna nit ne krene da upisuje ovset a onda druga nit utrci i promeni to
		asm cli;
	#ifndef BCC_BLOCK_IGNORE
		structOff3=FP_OFF(&newStruct);
		structSeg3=FP_SEG(&newStruct);



		asm{
			mov cx,structSeg3
			mov dx,structOff3  // dozvoljena je promena konteksta za razliku da imammo globalnu promeljivu jer se registri cuvaju.
		}


		asm sti;  // sa onim gore clijem.
		asm int 61h;

		asm{
			pop bx// vracamo sacuvane registre
			pop dx
			pop cx
		}
	#endif
	/*Global::Events->get(this->id)->signal();*/
}
