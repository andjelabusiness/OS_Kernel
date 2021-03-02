
#ifndef H_PCB_H_
#define H_PCB_H_

#include "thread.h"

class KernelSem;

class PCB{
public:
	PCB(StackSize stackSize,Time timeSlice, Thread* myThread);
    virtual ~PCB();
    virtual void start();
    virtual void waitToComplete();
    ID getId() const;

private:
    friend class Kernel;
    friend class KernelSem;
    friend class KernelEv;
    friend class PCBList;

    StackSize stackSize;
    unsigned int ss,bp,sp;
    unsigned* stack;
    void initStartingStack();



    Time timeSlice;
    Time timeToWait;

    Thread* myThread;
    enum State {NEW, COMPLETED, READY, BLOCKED};
    State state;
    KernelSem* waitToCompleteSem;


    ID id;

    int waitResult;
    KernelSem* mySem;
};


#endif /* H_PCB_H_ */
