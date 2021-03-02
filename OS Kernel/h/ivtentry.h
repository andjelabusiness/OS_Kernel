
#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;
class KernelEv;

class IVTEntry{
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newInt);
	~IVTEntry();
	void signal(int systemIntFlag);

private:
	friend class KernelEv;

	IVTNo ivtNo;
	pInterrupt systemInt;
	KernelEv* kernelEv;

};



#endif /* H_IVTENTRY_H_ */
