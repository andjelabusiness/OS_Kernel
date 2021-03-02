#include "thread.h"
#include "pcb.h"
#include "kernel.h"

Thread::Thread(StackSize stackSize, Time timeSlice){
	LOCKINTR();
	myPCB= new PCB(stackSize,timeSlice,this);
	UNLOCKINTR();
}


void Thread::start(){
	LOCKINTR();
	myPCB->start();
	UNLOCKINTR();
}

void Thread::waitToComplete(){
	LOCKINTR();
    myPCB->waitToComplete();
    UNLOCKINTR();
}


ID Thread::getRunningId(){

	LOCKINTR();
	ID id = Kernel::running->getId();
	UNLOCKINTR();
	return id;
}


ID Thread::getId(){

	LOCKINTR();
	ID id= myPCB->getId();
	UNLOCKINTR();
	return id;
}


Thread * Thread::getThreadById(ID id){

	LOCKINTR();
	Thread* thread= Kernel::getThreadById(id);
	UNLOCKINTR();
	return thread;

}

void dispatch(){

	LOCKINTR();
	Kernel::dispatchInt();
	UNLOCKINTR();
}


Thread::~Thread(){
	waitToComplete();
	LOCKINTR();
	delete myPCB;
	UNLOCKINTR();
}
