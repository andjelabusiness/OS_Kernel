
#ifndef H_KERNEV_H_
#define H_KERNEV_H_

typedef unsigned char IVTNo;
class IVTEntry;
class PCB;

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	virtual ~KernelEv();
	virtual void wait();
	virtual void signal();

private:
	IVTNo ivtNo;
	int isInitialized;
	IVTEntry* ivtEntry;
	PCB* myPCB;

	enum State {BLOCKED, READY, SIGNALLED};
	State state;

};



#endif /* H_KERNEV_H_ */
