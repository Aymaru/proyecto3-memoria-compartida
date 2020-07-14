#include "structs.h"

struct buffer_t * init_buffer(size_t array_size) {
    struct buffer_t * buff;
    buff = malloc(sizeof(struct buffer_t) + (sizeof(struct message_t) * array_size));
    if (buff == NULL) {
        //Manejar error de manera correcta...
        printf("Error allocating memory for buffer\n");
        exit(1);
    };
    sem_init(&(buff->sem_buffer), 1, 1); //semaforo del buffer, 1 = Es compartido entre procesos heavy weigth, 1 = valor inicial
    sem_init(&(buff->sem_producer), 1, 1); //semaforo del productor, 1 = Es compartido entre procesos heavy weigth, 1 = valor inicial
    sem_init(&(buff->sem_consumer), 1, 1); //semaforo del consumidor, 1 = Es compartido entre procesos heavy weigth, 1 = valor inicial
    buff->array_size = array_size;
    buff->n_msg_received = 0;
    buff->n_msg_processed = 0;
    buff->n_consumers = 0;
    buff->n_producers = 0;
    buff->end = 0; // flag to stop all producers and consumers
    for (size_t i = 0; i < array_size; ++i) {
        buff->messages[i].id_producer = -1;
        buff->messages[i].key = -1;
        time(&buff->messages[i].date);
    }
    return buff;
};

struct message_t * init_msg (int id_producer, int key, time_t time) {
    struct message_t * new_msg;
    new_msg = malloc(sizeof(struct message_t));
    if (new_msg == NULL) {
        //Manejar error de manera correcta...
        printf("Error allocating memory for message\n");
        exit(1);
    };
    new_msg->id_producer = id_producer;
    new_msg->key = key;
    new_msg->date = time;
    return new_msg;
};


void free_buffer(struct buffer_t * buff) {
    free(buff);
};

void free_message(struct message_t * msg) {
    free(msg);
};

struct message_t * insert_msg(struct buffer_t * buff, int id_producer, int key, int * index_msg) { //
    *index_msg = buff->n_msg_received % buff->array_size; //calculo el indice
    buff->messages[*index_msg].id_producer = id_producer;
    buff->messages[*index_msg].key = key;
    time(&buff->messages[*index_msg].date);
    buff->n_msg_received++;

    struct message_t * msg;
    msg = init_msg(id_producer, key, buff->messages[*index_msg].date);

    return msg;
};

struct message_t * get_msg(struct buffer_t * buff, int * index_msg) {
    struct message_t * msg;
    int id_producer, key;
    time_t time;

    int index;
    index = buff->n_msg_processed % buff->array_size;
    id_producer = buff->messages[index].id_producer;
    key = buff->messages[index].key;
    time = buff->messages[index].date;
    msg = init_msg(id_producer,key,time);
    *index_msg = index;

    buff->n_msg_processed++;
    return msg;
};

void print_buffer_status(struct buffer_t * buff) {
    printf("Estado del buffer en memoria compartida:\n\n");
    printf("Max cantidad de mensajes del buffer: %ld.\n", buff->array_size);
    printf("Cantidad de mensajes en el buffer: %d.\n", buff->n_msg_received - buff->n_msg_processed);
    printf("Cantidad de mensajes recibidos: %d.\n", buff->n_msg_received);
    printf("Cantidad de mensajes procesados: %d.\n", buff->n_msg_processed);
    printf("Cantidad de productores activos: %d.\n", buff->n_producers);
    printf("Cantidad de consumidores activos: %d.\n\n", buff->n_consumers);
};

void print_message(struct message_t * msg) {
    char buff[100];

    printf("MESSAGE INFORMATION\n");
    printf("\tProducer Id: %d\n",msg->id_producer);
    printf("\tRandom Key: %d\n",msg->key);

    strftime (buff, 100, "%Y-%m-%d %H:%M:%S.000", localtime (&msg->date));
    printf("\tTimestamp: %s\n", buff);

    printf("\n");
};

int isEmpty (struct buffer_t * buff) {
    if ((buff->n_msg_received - buff->n_msg_processed) == 0) {
        return 1;
    }
    return 0;
};

int isFull (struct buffer_t * buff) {
    if ((buff->n_msg_received - buff->n_msg_processed) == buff->array_size) {
        return 1;
    }
    return 0;
};

double rand_expo(double lambda) {
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return (-log(1- u) / lambda) * 1000000;
}
