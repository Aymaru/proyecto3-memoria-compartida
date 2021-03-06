#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <time.h>
#include <semaphore.h>
#include <math.h>


struct message_t {
    int id_producer,key;
    time_t date;//String de tama;o por definir para representar la fecha
};

struct buffer_t {
    size_t array_size;
    sem_t sem_producer, sem_consumer, sem_buffer;
    int n_msg_received , n_msg_processed, n_producers, n_consumers, total_producers, total_consumers, end;
    struct message_t messages[];
};

struct buffer_t * init_buffer (size_t array_size); //Reseva memoria para el buffer.
struct message_t * init_msg (int id_producer, int key, time_t time); //Reserva memoria para un mensaje. Para uso del consumidor.

int isEmpty(struct buffer_t * buff); //Devuelve 1 si no hay mensajes en el buffer
int isFull(struct buffer_t * buff);

void free_buffer(struct buffer_t * buff); //Libera la memoria reservada para la estructura de buffer
void free_message(struct message_t * msg); //

struct message_t * insert_msg(struct buffer_t * buff, int id_producer, int key, int * index_msg); //Agrega un msj al buffer
struct message_t * get_msg(struct buffer_t * buff, int * index_msg); //Devuelve el siguiente mensaje del buffer

void print_buffer_status(struct buffer_t * buff);
void print_message(struct message_t * msg);

double rand_expo(double lambda);
#endif
