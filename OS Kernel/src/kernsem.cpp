#include "kernsem.h"
#include "pcblist.h"
#include "kernel.h"
#include "pcb.h"
#include "schedule.h"

KernelSem::KernelSem(int init){

	if(init>0){
		v=init;
	}
	else{
		v=0;
	}

	blockedList= new PCBList();
}

KernelSem::~KernelSem(){
	if( v<0 ){
		signal(-v);
	}
	delete blockedList;

}

int KernelSem::wait(Time maxTimeToWait){

    if( --v < 0 ){

    	Kernel::running->state= PCB::BLOCKED;
    	blockedList->addLast(Kernel::running);
    	Kernel::running->mySem=this;
    	if( maxTimeToWait > 0){
    		Kernel::addTimeToWaitPcb(Kernel::running, maxTimeToWait);
    	}
    	dispatch();
    }
    else{
    	Kernel::running->waitResult=1;
    }
	return Kernel::running->waitResult;
}

int KernelSem::signal(int n){
	if (n<0){
		return n;
	}

	if(n==0){
		if(v++ < 0){
			PCB* pcb= blockedList->takeFirst();
			if (pcb){

				pcb->state= PCB::READY;
				Scheduler::put(pcb);
				Kernel::removeTimeToWaitPcb(pcb);
				pcb->waitResult=1;
				pcb->mySem=0;
			}
		}
		return 0;
	}


	int result=0;
	for (int i=0;i<n;i++){
		if (v<0){
			result++;
		}
	    signal(0);
	}
	return result;
}

int KernelSem::val() const{
	return v;
}

int KernelSem::remove(PCB* pcb){

	int result = blockedList->remove(pcb);
	v += result;
	return result;
}
