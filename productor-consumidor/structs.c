#include "structs.h"

struct buffer_t * init_buffer(int size) {
    struct buffer_t * buff;
    buff = malloc(sizeof(struct buffer_t));
    if (buff == NULL) {
        //Manejar error de manera correcta...
        printf("Error allocating memory for buffer\n");
        exit(1);
    };
    buff->first = NULL;
    buff->current = NULL;
    sem_init(&(buff->sem_buffer), 1, 0); //semaforo del buffer, 1 = Es compartido entre procesos heavy weigth, 0 = valor inicial
    sem_init(&(buff->sem_producer), 1, 0); //semaforo del productor, 1 = Es compartido entre procesos heavy weigth, 0 = valor inicial
    sem_init(&(buff->sem_consumer), 1, 0); //semaforo del consumidor, 1 = Es compartido entre procesos heavy weigth, 0 = valor inicial
    buff->size = size;
    buff->n_consumers = 0;
    buff->n_producers = 0;
    buff->end = 0;
    return buff;
};

struct node_t * init_node (int id) {
    struct node_t * new_node;
    new_node = malloc(sizeof(struct node_t));
    if (new_node == NULL) {
        //Manejar error de manera correcta...
        printf("Error allocating memory for node\n");
        exit(1);
    };
    new_node->id = id;
    new_node->msg = NULL;
    new_node->next = NULL;
    return new_node;
};

struct message_t * init_msg (int id_producer, int key) {
    struct message_t * new_msg;
    new_msg = malloc(sizeof(struct message_t));
    if (new_msg == NULL) {
        //Manejar error de manera correcta...
        printf("Error allocating memory for message\n");
        exit(1);
    };
    new_msg->id_producer = id_producer;
    new_msg->key = key;
    time(&new_msg->date);
    return new_msg;
};

int isEmpty (struct buffer_t * buff) {
    struct node_t * node;
    
    if (buff->first->msg != NULL) {
        return 1;
    };
    // node = buff->first->next;
    // while (node != buff->first) {
    //     if (node->msg != NULL) {
    //         return 1;
    //     };
    //     node = node->next;
    // };
    node = buff->first;
    for (int i = 0;i<buff->size;i++){
        if (node->msg != NULL) return 1;
        node = node->next;
    }
    return 0;
};

int insert_msg(struct buffer_t * buff, struct message_t * new_msg) {
    
}

// int isEmpty() {
//    return buff->first == NULL;
// }

// int length() {
//    int length = 0;

//    //if list is empty
//    if(buff->first == NULL) {
//       return 0;
//    }

//    buff->current = buff->first->next;

//    while(buff->current != buff->first) {
//       length++;
//       buff->current = buff->current->next;   
//    }
	
//    return length;
// }

// //insert link at the first location
// void insertFirst(int id_producer, time_t date, int key) {

//    //create a link
//    struct Node *link = (struct node*) malloc(sizeof(struct Node));
 
 
//    if (isEmpty()) {
//       buff->first = link;
//       buff->first->next = buff->first;
//    } else {
//       //point it to old first node
//       link->next = buff->first;
		
//       //point first to new first node
//       buff->first = link;
//    }    
// }

// //delete first item
// struct Node * deleteFirst(struct Buffer * buff) { //struct Buffer * buff ??

//    //save reference to first link
//    struct Node *tempLink = buff->first;
	
//    if(buff->first->next == buff->first) {  
//       buff->first = NULL;
//       return tempLink;
//    }     

//    //mark next to first link as first 
//    buff->first = buff->first->next;
	
//    //return the deleted link
//    return tempLink;
// }
