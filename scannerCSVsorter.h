#ifndef SCANNERCSVSORTER_H   /* Include guard */
#define SCANNERCSVSORTER_H

struct Node {
	struct Node * next;
	char line[500];
	int soi;
	char value[500];
};


void Msort(struct Node** headRef);



struct Node* SortedMerge(struct Node* a, struct Node* b); 
void FrontBackSplit(struct Node* source, struct Node** frontRef, struct Node** backRef); 



void printList(struct Node * p);
struct Node* freeptr(struct Node* ptr);
#endif
