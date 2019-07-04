/*
 * Global.cpp
 *
 *  Created on: Aug 5, 2018
 *      Author: OS1
 */

#include "Global.h"
#include "PCBList.h"
#include <dos.h>
#include <iostream.h>
#include <STDARG.H>
#include <STDIO.H>
#include "intLock.h"
#include "kernel.h"
volatile int Global::counter=20;
volatile PCB* Global::running=0;
volatile int Global::explicitDispatch=0;
volatile unsigned int Global::lockFlag=0;
volatile int Global::lockCount=0;
IdleT* Global::idle=0;
PCBList* Global::PCBs=0;
IdleT* idle=0;//timeslice joj je 1 i while(1)
volatile PCB* Global::mainPCB=0;
volatile PCB* Global::pcbe=0;
volatile int Global::one=1;
PCBSl* Global::SleepingPCBs=0;
volatile int Global::sleepDispatch=0;
SemList* Global::Semaphores=0;
volatile int Global::oneKernel=1;
volatile PCB* Global::runningKernelThread=0;// namestiti da pokazuje na pcb u konstruktoru kernel threda.
KernelThread* Global::runningKT=0;
int Global::waitRetVal=0;
volatile int Global::kernelTrci=0;
EvList* Global::Events=0;
int Global::retValue=0;
void Global::lockPreemt(){
	lockCount++;
	if (lockCount>=1)lockFlag=1;
}


void Global::unlockPreemt(){
	lockCount--;
	if (lockCount<0)lockCount=0;
	if (lockCount==0)lockFlag=0;
}


void Global::syncCout(char* str,int br){
	asm cli;
	cout<<str<<br<<endl;
	asm sti;

}


int syncPrintf(const char *format, ...)
{int res;
va_list args;
intLock;
va_start(args, format);
res = vprintf(format, args);
va_end(args);
intUnlock;
return res;
}


