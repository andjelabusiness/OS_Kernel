#include "pcb.h"
#include "kernel.h"
#include "schedule.h"
#include "kernsem.h"



PCB::PCB(StackSize stackSize,Time timeSlice, Thread* myThread)
     : stackSize(stackSize), timeSlice(timeSlice), timeToWait(0), myThread(myThread), ss(0), bp(0), sp(0) , stack(0), state(NEW) , id(Kernel::getNextThreadId()), waitToCompleteSem(0), waitResult(0) , mySem(0)
{
	if( stackSize>=65536UL){
		stackSize=65535UL;
	}

	Kernel::addPcbToList(this);
}


void PCB::start(){

	if(state==NEW){
	initStartingStack();
	waitToCompleteSem= new KernelSem(0);
	state=READY;
	if (this!= Kernel::idle){
	  Scheduler::put(this);
	 }
	}
}

void PCB::waitToComplete(){
    if ( state==NEW || state==COMPLETED){
    	return;
    }
    if (this==Kernel::running){
    	return;
    }
    waitToCompleteSem->wait(0);
}

ID PCB::getId() const {

	return id;
}

void PCB::initStartingStack(){

	StackSize sizeInInt= stackSize/ sizeof(unsigned);
	stack= new unsigned[sizeInInt];
	stack[sizeInInt-1]=SEGMENT(myThread);
	stack[sizeInInt-2]=OFFSET(myThread);
	stack[sizeInInt-5]=0x200;
	stack[sizeInInt-6]=SEGMENT(&Kernel::wrapper);
	stack[sizeInInt-7]=OFFSET(&Kernel::wrapper);


	ss=SEGMENT(stack + sizeInInt - 16);
	bp=OFFSET(stack + sizeInInt - 16);
	sp=bp;

}

PCB::~PCB(){
	Kernel::removePcbFromList(this);
	delete waitToCompleteSem;
    delete[] stack;
}

