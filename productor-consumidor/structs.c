#include "structs.h"


struct Buffer *buff;

int isEmpty() {
   return buff->first == NULL;
}

int length() {
   int length = 0;

   //if list is empty
   if(buff->first == NULL) {
      return 0;
   }

   buff->current = buff->first->next;

   while(buff->current != buff->first) {
      length++;
      buff->current = buff->current->next;   
   }
	
   return length;
}

//insert link at the first location
void insertFirst(int id_producer, time_t date, int key) {

   //create a link
   struct Node *link = (struct node*) malloc(sizeof(struct Node));
 
 
   if (isEmpty()) {
      buff->first = link;
      buff->first->next = buff->first;
   } else {
      //point it to old first node
      link->next = buff->first;
		
      //point first to new first node
      buff->first = link;
   }    
}

//delete first item
struct Node * deleteFirst(struct Buffer * buff) { //struct Buffer * buff ??

   //save reference to first link
   struct Node *tempLink = buff->first;
	
   if(buff->first->next == buff->first) {  
      buff->first = NULL;
      return tempLink;
   }     

   //mark next to first link as first 
   buff->first = buff->first->next;
	
   //return the deleted link
   return tempLink;
}
