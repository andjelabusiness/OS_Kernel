#include "semaphor.h"
#include "kernsem.h"
#include "kernel.h"

Semaphore::Semaphore(int init){
	LOCKINTR();
	myImpl= new KernelSem(init);
	UNLOCKINTR();
}

Semaphore::~Semaphore(){
	LOCKINTR();
	delete myImpl;
	UNLOCKINTR();
}

int Semaphore::wait(Time maxTimeToWait){
	LOCKINTR();
	int result= myImpl->wait(maxTimeToWait);
	UNLOCKINTR();
	return result;
}

int Semaphore::signal(int n){
	LOCKINTR();
	int result= myImpl->signal(n);
	UNLOCKINTR();
	return result;
}

int Semaphore::val() const {
	LOCKINTR();
	int result= myImpl->val();
	UNLOCKINTR();
	return result;
}
