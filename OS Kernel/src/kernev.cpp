#include "kernev.h"
#include "kernel.h"
#include "ivtentry.h"
#include "pcb.h"
#include "schedule.h"

KernelEv::KernelEv(IVTNo ivtNo): ivtNo(ivtNo), isInitialized(0), ivtEntry(Kernel::getIVTEntryObject(ivtNo)), myPCB(Kernel::running), state(READY) {

	if (ivtEntry){
		if (ivtEntry->kernelEv==0){
			ivtEntry->kernelEv= this;
			isInitialized=1;
		}

	}
}

KernelEv::~KernelEv(){

	if (isInitialized){
		ivtEntry->kernelEv=0;

		if (state== BLOCKED){
			 signal();
			}
	}

}

void KernelEv::wait(){

	if (!isInitialized){
		return;
	}
	if (myPCB!= Kernel::running){
		return;
	}

	if (state== SIGNALLED){
		state=READY;
	}
	else if(state== READY){
		state= BLOCKED;
		Kernel::running->state= PCB::BLOCKED;
		dispatch();
	}
}

void KernelEv::signal(){

	if (!isInitialized){
			return;
	}

	if (state== BLOCKED){
		state= READY;
		myPCB->state= PCB::READY;
        Scheduler::put(myPCB);
	}
	else if( state==READY){
		state= SIGNALLED;
	}

}


