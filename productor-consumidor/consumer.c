#include "consumer.h"

/*--------
  GLOBAL
--------*/

static volatile sig_atomic_t keep_running = 1;

void handle_exit(int signal) {
  keep_running = 0;
};

/*--------------------------
       MAIN CONSUMIDOR
--------------------------*/
int main(int argc, char *argv[]) {
  struct sigaction act;
  act.sa_handler = handle_exit;
  sigaction(SIGINT, &act, NULL);

  char shm_name[25]; //Nombre del segmento de memoria compartida.
  struct buffer_t * buff,* shm_buffer; //Puntero para inicializar el buffer y puntero para mapear la memoria compartida usando mmap.
  int fd; //File descriptor de la memoria compartida
  int * array_size;
  size_t size;
  int consumer_id,msg_index,stop_by_key,n_read_msg;
  double waiting_time;
  struct message_t * msg;
  char * eptr;

  if (argc >= 2) {
    for (int i=1;i<argc;i++) {
      if (strcmp(argv[i],"-n") == 0) { //Option Buffer Name
        if (argv[++i] == NULL){
          printf("Invalid buffer name. Please input a valid buffer name.\n");
          exit(0);
        }
        strcpy(shm_name, argv[i]);
        
      } else if (strcmp(argv[i],"-t") == 0) { //Waiting time in seconds
        if (argv[++i] == NULL || (waiting_time = strtod(argv[i],&eptr)) == 0){
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
  stop_by_key = 0; //Flag to stop consumer if key match PID % 5
  n_read_msg = 0; //
  //Accesing consumer count
  sem_wait(&shm_buffer->sem_consumer); //lock consumer sem
  //Critical Region
  consumer_id = shm_buffer->n_consumers++;
  printf("Starting consumer: %d.\n\n",consumer_id);
  print_buffer_status(shm_buffer);
  //End of Critical Region
  sem_post(&shm_buffer->sem_consumer); //unlock consumer sem

  usleep((int)rand_expo(waiting_time)); //Wait

  while (!shm_buffer->end && !stop_by_key && keep_running) //Do while not end
  {
    //Accesing the buffer
    sem_wait(&shm_buffer->sem_buffer); //Lock the buffer sem
    //Critical Region
    if (!isEmpty(shm_buffer)) {
      //Read message from buffer
      print_buffer_status(shm_buffer);
      msg = get_msg(shm_buffer,&msg_index);
      printf("Message read correctly from buffer index: %d.\n\n",msg_index);
      print_message(msg,msg_index);
      if ((getpid() % 5) == msg->key ) stop_by_key = 1;
      free_message(msg);
      n_read_msg++;
    } else {
      //Buffer is empty.
      print_buffer_status(shm_buffer);
      printf("Buffer empty. Not messages available.\n\n");
    }    
    //End of Critical Region
    sem_post(&shm_buffer->sem_buffer); //Unlock the buffer sem
    usleep((int)rand_expo(waiting_time)); //Wait
  }

  if (!keep_running) {
    printf("Consumer stopped by user\n");
  } else if (shm_buffer->end) {
    printf("Consumer stopped by finisher\n");
  } else  {
    printf("Consumer stopped by message key == PID %% 5\n");
  }
  //End consumer
  printf("Exiting consumer: %d.\n",consumer_id);
  printf("Read Messages: %d\n\n",n_read_msg);
  //Accesing consumer count
  sem_wait(&shm_buffer->sem_consumer); //lock consumer sem
  //Critical Region
  shm_buffer->n_consumers--;
  print_buffer_status(shm_buffer);
  //End of Critical Region
  sem_post(&shm_buffer->sem_consumer); //unlock consumer sem
  


  return 1;
}
