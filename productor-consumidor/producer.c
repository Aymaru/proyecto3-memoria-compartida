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
  int producer_id,key,waiting_time,index; //Producer info
  time_t date;

  if (argc >= 2) {
    for (int i=1;i<argc;i++) {
      if (strcmp(argv[i],"-n") == 0) { //Option Buffer Name
        if (argv[++i] == NULL){
          printf("Invalid buffer name. Please input a valid buffer name.\n");
          exit(0);
        }
        strcpy(shm_name, argv[i]);
        
      } else if (strcmp(argv[i],"-t") == 0) { //Waiting time in seconds
        if (argv[++i] == NULL || (waiting_time = atoi(argv[i])) == 0){
          printf("Invalid waiting time. Please input a valid waiting time.\n");
          exit(0);
        }
        
      } else { //default
        printf("Invalid Option. Use: ./producer.o -n [Nombre del buffer] -t [Waiting time].\n");
        exit(0);
      };
    };
  } else {
    printf("Invalid option. Use: ./creator.o -s [Cantidad de Mensajes] -n [Nombre del buffer].\n");
    exit(0);
  };

  if ((fd = shm_open (shm_name, O_RDWR, 0)) == -1) { 
    printf("Error opening shared-memory file descriptor.\n");
    exit(1);
  };

  array_size = mmap(0,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Get the buffer size to mmap the buffer
  size = *array_size;
  munmap(array_size, sizeof(size_t));
  
  shm_buffer = mmap(0, sizeof(struct buffer_t) + (sizeof(struct message_t) * size),
                    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Mapping the shared-memory buffer

  //Accesing producers count
  sem_wait(&shm_buffer->sem_producer);
  //Critical Region
  producer_id = shm_buffer->n_producers++;
  printf("Starting producer: %d.\n\n",producer_id);
  print_buffer_status(shm_buffer);
  //End of Critical Region
  sem_post(&shm_buffer->sem_producer);

  sleep(waiting_time); //Wait

  while (!shm_buffer->end) //Do while not end
  {
    //Accesing the buffer
    sem_wait(&shm_buffer->sem_buffer); //Lock the buffer sem
    //Critical Region
    if (!isFull(shm_buffer)) {
      key = 2; //Generar key aleatoria entre 0 y 5
      insert_msg(shm_buffer,producer_id,key);
      printf("Message inserted in buffer correctly.\n");
      print_buffer_status(shm_buffer);
    } else {
      printf("Buffer full. Message not inserted.\n");
    }    

    //End of Critical Region
    sem_post(&shm_buffer->sem_buffer); //Unlock the buffer sem
    sleep(waiting_time); //Wait
  }
  //End producer
  printf("Exiting producer: %d.\n\n",producer_id);
  //Accesing producers count
  sem_wait(&shm_buffer->sem_producer);
  //Critical Region
  shm_buffer->n_producers--;
  print_buffer_status(shm_buffer);
  //End of Critical Region
  sem_post(&shm_buffer->sem_producer);
  


  return 1;
}
