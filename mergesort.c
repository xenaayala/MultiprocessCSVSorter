#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "simpleCSVsorter.h"

void Msort(struct Node** headRef){

	
	struct Node* head = *headRef;
	struct Node* n1;
	struct Node* n2; 

	
	if((head==NULL)|| (head->next== NULL)){
		return; 
	} 
	 	
	FrontBackSplit(head, &n1, &n2);

	
	Msort(&n1);
	Msort(&n2);

	
	
	*headRef= SortedMerge(n1,n2);
	
	
	
}


struct Node* SortedMerge(struct Node* n1, struct Node* n2){
	
	struct Node* answer= NULL;

	if (n1==NULL){
		return n2;
	}
	else if (n2==NULL){
		return n1;
	}

	//COMPARE DATA HERE
	if (n1->soi == 1){
		int n1val = atoi(n1->value);
		int n2val = atoi(n2->value);
		if (n1val<= n2val){

			answer=n1;
			answer->next= SortedMerge(n1->next, n2);
		}
		else 
		{
			answer=n2;
			answer->next=SortedMerge(n1, n2->next);
		}
	
		
		}else{
			
			if(strcmp(n1->value, n2->value)<=0){
				answer=n1;
				answer->next = SortedMerge(n1->next, n2);
			}else{
				answer=n2;
				answer->next=SortedMerge(n1, n2->next);
			}
		}
		
		

		return(answer);
	
	
}



void FrontBackSplit(struct Node* source, struct Node** frontRef, struct Node** backRef) 
{ 
        struct Node* fast; 
        struct Node* slow; 
    slow = source; 
    fast = source->next; 
  
   
    while (fast != NULL) 
    { 
    fast = fast->next; 
    if (fast != NULL) 
    { 
        slow = slow->next; 
        fast = fast->next; 
    } 
    } 
  
  
    *frontRef = source; 
    *backRef = slow->next; 
    slow->next = NULL; 
}
