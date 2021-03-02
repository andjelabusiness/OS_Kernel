
#ifndef H_KERNEL_H_
#define H_KERNEL_H_

class Thread;
class PCB;
class PCBList;
class IdleThread;
class IVTEntry;

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;
typedef unsigned int Time;
typedef int ID;

#ifndef BCC_BLOCK_IGNORE
#include <dos.h>
#define SEGMENT(pointer) FP_SEG(pointer)
#define OFFSET(pointer) FP_OFF(pointer)
#define GETVECT(ivtNo) getvect(ivtNo)
#define SETVECT(ivtNo, pointer) setvect(ivtNo, pointer)
#define LOCKINTR() asm{ pushf; cli; }
#define UNLOCKINTR() asm{ popf; }
#else
#define SEGMENT(pointer) 0
#define OFFSET(pointer) 0
#define GETVECT(ivtNo) ((pInterrupt)0)
#define SETVECT(ivtNo, pointer)
#define LOCKINTR()
#define UNLOCKINTR()
#endif

class Kernel {

public:
	 static void interrupt dispatchInt();

	 static PCB* running;
	 static PCB* idle;

	 static void wrapper(Thread* thread);
	 static void initialize();
	 static void stop();
	 static ID getNextThreadId();
	 static Thread* getThreadById(ID id);
	 static void addPcbToList(PCB* pcb);
	 static void removePcbFromList(PCB* pcb);
	 static void addTimeToWaitPcb(PCB* pcb, Time timeToWait);
	 static void removeTimeToWaitPcb( PCB* pcb);
     static void  updateWaitingThreads();
	 static IVTEntry* getIVTEntryObject(IVTNo ivtNo);
	 static void setIVTEntryObject(IVTNo ivtNo, IVTEntry* IVTEntry);

private:
	 static PCBList* allPcbList;
	 static Thread* mainThread;
	 static IdleThread* idleThread;

	 static void interrupt timerInt(...);

	 static pInterrupt systemTimerInt;
	 static Time runningTicks;
	 static ID nextThreadId;

	 struct Node{
		 Node* next;
		 PCB* pcb;
	 };

	 static Node* timeWaitingFirst;


	 static IVTEntry* ivtEntryObjects[256];


	 Kernel() {}

};



#endif /* H_KERNEL_H_ */
