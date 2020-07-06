#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>


struct message_t {
    int id_producer,key;
    time_t date;//String de tama;o por definir para representar la fecha
};

struct node_t {
    int id;
    struct message_t * msg;
    struct node_t * next; 
};

struct buffer_t {
    struct node_t * first;
    struct node_t * current;  
    sem_t sem_producer, sem_consumer,sem_buffer;
    int size, n_producers, n_consumers, end;
};

struct buffer_t * init_buffer (int size); //Define la cantidad de mensajes que tiene el buffer
struct node_t * init_node (int id);
struct message_t * init_msg (int id_producer, int key);

int isEmpty(struct buffer_t * buff); //Devuelve 1 si no hay mensajes en el buffer

int length(); //Es necesario? puede ser la cantidad de mensajes que permite el buffer, o la cantidad de mensajes que tiene actualmente el buffer
int insert_msg(struct buffer_t * buff, struct message_t * new_msg); //Agrega un msj al buffer, devuelve la posicion del buffer donde se dejo el mensaje, -1 en caso de error
struct node_t * get_msg(struct buffer_t * buff);

// void insertFirst(int id_producer, time_t date, int key);
// struct Node * deleteFirst();
// void printList();

// void print_msg(struct message_t * msg);



#endif