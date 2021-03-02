#include "ivtentry.h"
#include "kernel.h"
#include "kernev.h"
#include "thread.h"

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newInt) : ivtNo(ivtNo), systemInt(0), kernelEv(0){

	LOCKINTR();
	Kernel::setIVTEntryObject( ivtNo, this);
	systemInt= GETVECT(ivtNo);
	SETVECT(ivtNo, newInt);
	UNLOCKINTR();
}


IVTEntry::~IVTEntry(){

	LOCKINTR();
	SETVECT(ivtNo, systemInt);
	if (systemInt!=0){
		(*systemInt)();
	}
	Kernel::setIVTEntryObject( ivtNo, 0);
	UNLOCKINTR();
}

void IVTEntry::signal(int systemIntFlag){

	if (systemIntFlag!= 0 && systemInt!=0){
		(*systemInt)();
	}

	if (kernelEv!= 0){
		kernelEv->signal();
	}


}
