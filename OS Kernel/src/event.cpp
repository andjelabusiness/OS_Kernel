#include "event.h"
#include "kernel.h"
#include "kernev.h"

Event::Event (IVTNo ivtNo){

	LOCKINTR();
    myImpl= new KernelEv(ivtNo);
	UNLOCKINTR();
}

Event::~Event (){

	LOCKINTR();
    delete myImpl;
	UNLOCKINTR();
}

void Event::wait (){

	LOCKINTR();
    myImpl->wait();
	UNLOCKINTR();
}

void Event::signal (){

	LOCKINTR();
    myImpl->signal();
	UNLOCKINTR();
}

