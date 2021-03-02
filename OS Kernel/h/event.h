
#ifndef H_EVENT_H_
#define H_EVENT_H_

#define PREPAREENTRY(ivtNo, systemIntFlag) \
   extern IVTEntry ivtEntry##ivtNo; \
   void interrupt newInt##ivtNo(...) { \
	   ivtEntry##ivtNo.signal(systemIntFlag); \
	   dispatch(); \
   } \
   IVTEntry ivtEntry##ivtNo(ivtNo, &newInt##ivtNo);

#include "ivtentry.h"

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
   Event (IVTNo ivtNo);
   ~Event ();
   void wait ();
protected:
   friend class KernelEv;
   void signal(); // can call KernelEv
private:
   KernelEv* myImpl;

};


#endif /* H_EVENT_H_ */
