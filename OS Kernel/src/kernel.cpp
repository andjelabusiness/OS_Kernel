#include "kernel.h"
#include "schedule.h"
#include "pcb.h"
#include "kernsem.h"
#include "pcblist.h"
#include "idle.h"


void tick();

PCB* Kernel::running=0;
PCB* Kernel::idle=0;
PCBList* Kernel::allPcbList=0;
Thread* Kernel::mainThread=0;
IdleThread* Kernel::idleThread=0;
pInterrupt Kernel::systemTimerInt=0;
Time Kernel::runningTicks=0;
ID Kernel::nextThreadId=1;
Kernel::Node* Kernel::timeWaitingFirst= 0;
IVTEntry* Kernel::ivtEntryObjects[256];

static unsigned int tss,tbp,tsp;

void interrupt Kernel::dispatchInt(){

#ifndef BCC_BLOCK_IGNORE
	asm{
		mov tss,ss;
		mov tbp,bp;
		mov tsp,sp;
	}
#endif
	running->ss=tss;
	running->bp=tbp;
	running->sp=tsp;

	if(running->state== PCB::READY && running!=idle){
	      Scheduler::put(running);
	}
	running=Scheduler::get();
	if (running==0){
		running=idle;
	}


	tss=running->ss;
	tbp=running->bp;
	tsp=running->sp;
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov ss,tss;
		mov bp,tbp;
		mov sp,tsp;
	}
#endif

	runningTicks=0;

}

void Kernel::wrapper(Thread* thread){
	if (thread) thread->run();

    LOCKINTR();
	int semVal=running->waitToCompleteSem->val();
	if (semVal<0){
		running->waitToCompleteSem->signal(-semVal);
	}


	running->state= PCB::COMPLETED;
	UNLOCKINTR();

	dispatch();


}

void Kernel::initialize(){

  allPcbList= new PCBList();
  mainThread=new Thread(0);
  mainThread->myPCB->state= PCB::READY;
  running=mainThread->myPCB;
  idleThread= new IdleThread();
  idle= idleThread->myPCB;
  idleThread->start();
  LOCKINTR();
  systemTimerInt= GETVECT(8);
  SETVECT(8, &timerInt);
  UNLOCKINTR();
}

void Kernel::stop(){

	LOCKINTR();
	SETVECT(8, systemTimerInt );
	UNLOCKINTR();
	idleThread->myPCB->state= PCB::COMPLETED;
	delete idleThread;
	mainThread->myPCB->state= PCB::COMPLETED;
	delete mainThread;
	delete allPcbList;
}

ID Kernel::getNextThreadId(){

	return nextThreadId++;
}

Thread* Kernel::getThreadById(ID id){

	PCB* pcb= allPcbList->find(id);
	if( pcb) {
		return pcb->myThread;
	}
	else {
		return 0;
	}
}

void Kernel::addPcbToList(PCB* pcb){

	allPcbList->addLast(pcb);
}

void Kernel::removePcbFromList(PCB* pcb){

	allPcbList->remove(pcb);
}

void Kernel::addTimeToWaitPcb(PCB* pcb, Time timeToWait){

	if( pcb!=0 && timeToWait>0){
          pcb->timeToWait= timeToWait;
          Node* node= new Node();
          node->next= 0;
          node->pcb= pcb;

          Node* prevNode=0;
          Node* curNode= timeWaitingFirst;

          while( curNode!=0 && curNode->pcb->timeToWait < node->pcb->timeToWait){
        	  node->pcb->timeToWait -= curNode->pcb->timeToWait;
        	  prevNode= curNode;
        	  curNode= curNode->next;
          }

          if (curNode!=0){

        	  curNode->pcb->timeToWait -= node->pcb->timeToWait;
        	  node->next= curNode;
          }

          if (prevNode!= 0){
        	  prevNode->next= node;
          }
          else {
        	  timeWaitingFirst= node;
          }


	}

}

void Kernel::removeTimeToWaitPcb(PCB* pcb){

	Node* prevNode=0;
	Node* curNode= timeWaitingFirst;

	while ( curNode!=0 && pcb!= curNode->pcb){
		prevNode=curNode;
		curNode= curNode->next;
	}

	if (curNode!=0 ){

		if (prevNode!= 0){
			prevNode->next= curNode->next;
		}
		else {
			timeWaitingFirst= curNode->next;
		}

		if (curNode->next !=0){
			curNode->next->pcb->timeToWait += curNode->pcb->timeToWait;
		}

		delete curNode;

	}

}


void Kernel::updateWaitingThreads(){


	if (timeWaitingFirst!=0 && timeWaitingFirst->pcb->timeToWait>0){
        --timeWaitingFirst->pcb->timeToWait;
	}


	while (timeWaitingFirst!=0 && timeWaitingFirst->pcb->timeToWait==0) {
        PCB* pcb=timeWaitingFirst->pcb;
        Node* oldNode= timeWaitingFirst;
        timeWaitingFirst= timeWaitingFirst->next;
        oldNode->next = 0;
        delete oldNode;
        pcb->state= PCB::READY;
        Scheduler::put(pcb);
        pcb->waitResult = 0;
        pcb->mySem->remove(pcb);
        pcb->mySem=0;


	}

}


IVTEntry* Kernel::getIVTEntryObject(IVTNo ivtNo){

	LOCKINTR();
    IVTEntry* ivtEntry= ivtEntryObjects[ivtNo];
    UNLOCKINTR();
    return ivtEntry;
}

void Kernel::setIVTEntryObject(IVTNo ivtNo, IVTEntry* IVTEntry){

	LOCKINTR();
	ivtEntryObjects[ivtNo]= IVTEntry;
	UNLOCKINTR();
}

void interrupt Kernel::timerInt(...){


     if (systemTimerInt){
    	 (*systemTimerInt)();
     }


     tick();


     updateWaitingThreads();


    if(running->timeSlice > 0){

        if( ++runningTicks >= running->timeSlice ){

        	dispatch();
        }
    }

}
