
#ifndef H_IDLE_H_
#define H_IDLE_H_

#include "thread.h"

class IdleThread: public Thread {

public:
	void run();
};

#endif /* H_IDLE_H_ */
