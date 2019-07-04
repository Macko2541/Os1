/*
 * main.cpp
 *
 *  Created on: Aug 5, 2018
 *      Author: OS1
 */
#include <iostream.h>
#include <dos.h>
#include "System.h"
#include "Global.h"
#include "PCBList.h"
int userMain(int argc, char* argv[]);
int main(int argc,char* argv[]){
	int rez=0;
	System::inic();
	rez=userMain(argc,argv);   // pazi kad kreiras nit koja se ne startuje onda main ne moze da se zavrsi zbog destruktora.       // pazi ako je timeslice 0 onda treba da ima neograniceno vreme.
	System::restore();
	cout<<"Hello world";
	return rez;
}
// zabrani prekide tamo gde se barata sa listama i to ovako:
/*asm pushf;
 * asm cli;
 *
 * asm popf;
 */


// STA AKO NIT POZOVE WAITTOCOMPLETE ZA SEBE???
//ZAKLJUAJ PRISTUPANJA LISTI!!!
//EVENT TREBA DA DEBLOKIRA NIT AKO SE UGASI


