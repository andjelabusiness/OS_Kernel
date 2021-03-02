
#ifndef H_KERNSEM_H_
#define H_KERNSEM_H_

typedef unsigned int Time;
class PCBList;
class PCB;

class KernelSem{
public:
   KernelSem(int init);

   ~KernelSem();
   int wait(Time maxTimeToWait);
   int signal(int n);
   int val() const;
   int remove(PCB* pcb);

private:
   int v;
   PCBList* blockedList;
};



#endif /* H_KERNSEM_H_ */
