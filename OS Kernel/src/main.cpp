#include "kernel.h"

int userMain(int argc,char* argv[]);


int main(int argc,char* argv[]){

	Kernel::initialize();
	int result=userMain(argc,argv);
	Kernel::stop();

	return result;
}



