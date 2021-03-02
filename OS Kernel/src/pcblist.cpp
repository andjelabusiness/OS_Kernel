#include "pcblist.h"
#include "pcb.h"

PCBList::PCBList(): first(0), last(0) {}

void PCBList::addLast(PCB* pcb){

	Node* newNode= new Node();
	newNode->next=0;
	newNode->pcb=pcb;

	if( last==0){

		first=newNode;
		last=newNode;
	}
	else{

		last->next=newNode;
		last= newNode;
	}
}

PCB* PCBList::takeFirst(){

	if( first==0){

		return 0;
	}
	else {
		Node* node = first;
		first=first->next;
		if( first==0){
			last=0;
		}
		node->next=0;
		PCB* pcb=node->pcb;
		delete node;
		return pcb;
	}
}

int PCBList::remove(PCB* pcb){

	Node* prevNode=0;
	Node* curNode= first;

	while( curNode!=0 ){
		if(pcb== curNode->pcb){

			Node* oldNode= curNode;
			curNode=curNode->next;
			if (prevNode!=0){
				prevNode->next= curNode;
			}
			else {
				first= curNode;
			}
			if (oldNode==last){
				last=prevNode;
			}
			delete oldNode;
			return 1;
		}
		else {
			prevNode= curNode;
			curNode= curNode->next;
		}
	}

	return 0;
}

PCB* PCBList::find(ID id){

	Node* curNode=first;

	while (curNode!=0 ){
        if( id== curNode->pcb->id){
             return curNode->pcb;
        }
        else {
        	curNode= curNode->next;
        }
	}

    return 0;
}

PCBList::~PCBList(){

	while(first){
		Node* node = first;
		first=first->next;
	    node->next=0;
	    delete node;
	}

}




