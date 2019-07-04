/*
 * semaphor.h
 *
 *  Created on: Aug 15, 2018
 *      Author: OS1
 */

#ifndef _semaphor_h_
#define _semaphor_h_


typedef int ID;
class Semaphore{
public:
	Semaphore(int init=1);
	virtual ~Semaphore();

	virtual int wait(int toBlock);
	virtual void signal();

	int val()const;



	int getId();
	ID idSem;
protected:

private:


	static int posId;
};

extern unsigned structSeg2;
extern unsigned structOff2;



#endif /* _semaphor_h_ */
