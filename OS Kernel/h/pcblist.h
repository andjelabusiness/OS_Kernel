
#ifndef H_PCBLIST_H_
#define H_PCBLIST_H_

class PCB;
typedef int ID;

class PCBList{
public:
	PCBList();
    virtual ~PCBList();

	virtual void addLast(PCB* pcb);
	virtual PCB* takeFirst();
    int remove(PCB* pcb);
	PCB* find(ID id);

private:
   struct Node{
	   Node* next;
	   PCB* pcb;
   };

   Node* last;
   Node* first;


};



#endif /* H_PCBLIST_H_ */
