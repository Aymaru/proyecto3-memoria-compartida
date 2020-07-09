#include "producer.h"

/*--------
  GLOBAL
--------*/

/*--------------------------
       MAIN PRODUCTOR
--------------------------*/
int main(int argc, char *argv[]) {
  char shm_name[25]; //Nombre del segmento de memoria compartida.
  struct buffer_t * buff,* shm_buffer; //Puntero para inicializar el buffer y puntero para mapear la memoria compartida usando mmap.
  int fd; //File descriptor de la memoria compartida
  int * array_size;
  size_t size;
  int producer_id,key,msg_index,n_sent_msg; //Producer info
  time_t date;
  double waiting_time;
  char * eptr;
  useconds_t useconds,total_useconds,waiting_useconds;

  srand(time(0));

  if (argc >= 2) {
    for (int i=1;i<argc;i++) {
      if (strcmp(argv[i],"-n") == 0) { //Option Buffer Name
        if (argv[++i] == NULL){
          printf("Invalid buffer name. Please input a valid buffer name.\n");
          exit(EXIT_FAILURE);
        }
        strcpy(shm_name, argv[i]);
        
      } else if (strcmp(argv[i],"-t") == 0) { //Waiting time in seconds
        if (argv[++i] == NULL || (waiting_time = strtod(argv[i],&eptr)) == 0){
          printf("Invalid waiting time. Please input a valid waiting time.\n");
          exit(EXIT_FAILURE);
        }
        
      } else { //default
        printf("Invalid Option. Use: ./producer.o -n [Nombre del buffer] -t [Waiting time].\n");
        exit(EXIT_FAILURE);
      };
    };
  } else {
    printf("Invalid option. Use: ./creator.o -s [Cantidad de Mensajes] -n [Nombre del buffer].\n");
    exit(EXIT_FAILURE);
  };

  if ((fd = shm_open (shm_name, O_RDWR, 0)) == -1) { 
    printf("Error opening shared-memory file descriptor.\n");
    exit(EXIT_FAILURE);
  };

  array_size = mmap(0,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Get the buffer size to mmap the buffer
  size = *array_size;
  munmap(array_size, sizeof(size_t));
  
  shm_buffer = mmap(0, sizeof(struct buffer_t) + (sizeof(struct message_t) * size),
                    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Mapping the shared-memory buffer

  n_sent_msg = 0; //
  waiting_useconds = 0;
  total_useconds = 0;

  //Accesing producers count
  sem_wait(&shm_buffer->sem_producer); //Lock producer sem
  //Critical Region
  producer_id = shm_buffer->n_producers++;
  printf("Starting producer: %d.\n\n",producer_id);
  print_buffer_status(shm_buffer);
  //End of Critical Region
  sem_post(&shm_buffer->sem_producer); //Unlock producer sem
  useconds = (int)rand_expo(waiting_time);
  waiting_useconds += useconds;
  printf("Waiting %d microseconds for first message.\n",useconds);
  usleep(useconds); //Wait

  while (!shm_buffer->end) //Do while not end
  {
    //Accesing the buffer
    sem_wait(&shm_buffer->sem_buffer); //Lock the buffer sem
    //Critical Region
    if (!isFull(shm_buffer)) {
      key = rand() % 5; //Generate random key
      msg_index = buff->n_msg_received % buff->array_size;
      insert_msg(shm_buffer,producer_id,key); //Insert msg into buffer
      printf("Message inserted in buffer correctly at indext: %d.\n",msg_index);
      print_buffer_status(shm_buffer);
      n_sent_msg++;
    } else {
      print_buffer_status(shm_buffer);
      printf("Buffer full. Message not inserted.\n");
    }    
    //End of Critical Region
    sem_post(&shm_buffer->sem_buffer); //Unlock the buffer sem
    useconds = (int)rand_expo(waiting_time);
    waiting_useconds += useconds;
    printf("Waiting %d microseconds for next message.\n",useconds);
    usleep(useconds); //Wait
  }
  //End producer
  printf("Exiting producer: %d.\n",producer_id);
  printf("Sent Messages: %d\n\n",n_sent_msg);
  //Accesing producers count
  sem_wait(&shm_buffer->sem_producer); //Lock producer sem
  //Critical Region
  shm_buffer->n_producers--;
  print_buffer_status(shm_buffer);
  //End of Critical Region
  sem_post(&shm_buffer->sem_producer); //Unlock producer sem
  
  exit(EXIT_SUCCESS);
}
