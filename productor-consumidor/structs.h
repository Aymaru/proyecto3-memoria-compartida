#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


struct Message {
    int id_producer,key;
    time_t date;//String de tama;o por definir para representar la fecha
};

struct Node {
    struct id;
    struct Message * msg;
    struct Node * next; 
};

struct Buffer {
    struct Node * first;
    struct Node * current;  
};

int init_buffer (int size);

int isEmpty();
int length();   
void insertFirst(int id_producer, time_t date, int key);
struct Node * deleteFirst();
void printList();

void print_msg(struct Message * msg);



#endif