/*
 * IdleT.cpp
 *
 *  Created on: Aug 8, 2018
 *      Author: OS1
 */

#include "IdleT.h"
#include "Global.h"
#include <iostream.h>
#include <dos.h>


IdleT::IdleT():Thread(5000,1) {
	vo=1;
}

IdleT::~IdleT() {
	// wait to Finish
	// TODO Auto-generated destructor stub
}

void IdleT::run(){  //nit koja radi kad su sve ostale blokirane
	while (vo){

		/*if (Global::explicitDispatch==1)dispatch();*///ionako je tajmer 1 pa se svakako radi dispatch stalno
	}
}
