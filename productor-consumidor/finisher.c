#include "finisher.h"

/*--------------------------
       MAIN FINALIZADOR
--------------------------*/
int main(int argc, char *argv[]) {
  struct sigaction act;
  act.sa_handler = SIG_IGN;
  sigaction(SIGINT, &act, NULL);

  char shm_name[25]; //Nombre del segmento de memoria compartida.
  struct timeval finish_time, start_time;
  struct buffer_t * buff,* shm_buffer; //Puntero para inicializar el buffer y puntero para mapear la memoria compartida usando mmap.
  int fd; //File descriptor de la memoria compartida
  int * array_size;
  int n_producers, n_consumers, total_consumers, total_producers, n_msg_received, n_msg_processed;
  size_t size;
  double execution_time;

  if (argc >= 2) {
    for (int i=1;i<argc;i++) {
      if (strcmp(argv[i],"-n") == 0) { //Option Buffer Name
        if (argv[++i] == NULL){
          printf("Invalid buffer name. Please input a valid buffer name.\n");
          exit(0);
        }
        strcpy(shm_name, argv[i]);
      } else { //default
        printf("Invalid Option. Use: ./finisher.o -n [Nombre del buffer].\n");
        exit(0);
      };
    };
  } else {
    printf("Invalid option. Use: ./creator.o -s [Cantidad de Mensajes] -n [Nombre del buffer].\n");
    exit(0);
  };

  gettimeofday(&start_time, NULL);    //Inicia timer.

  printf("--------------------------------\n");
  printf("      SHARED-MEMORY FINISHER\n");
  printf("--------------------------------\n");

  if ((fd = shm_open (shm_name, O_CREAT|O_EXCL, 0)) == -1) {
    int errsv = errno;
    if (errsv == EEXIST) {
      if ((fd = shm_open (shm_name, O_RDWR, 0)) == -1) {
        printf("Error opening shared-memory file descriptor.\n");
        exit(1);
      };
      //Si no hay un error, se abre la memoria compartida
    } else {
      printf("Error opening shared-memory file descriptor.\n");
      exit(1);
    };
  } else {
    close(fd);
    shm_unlink(shm_name);
    printf("Error shared-memory file doesn't exists.\n");
    exit(1);
  };
  printf("Shared-memory object opened.\n");

  array_size = mmap(0,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  size = *array_size;
  munmap(array_size, sizeof(size_t));

  shm_buffer = mmap(0, sizeof(struct buffer_t) + (sizeof(struct message_t) * size),
                    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  printf("Shared-memory object mapped to memory.\n");

  //Accesing shared-memory buffer
  sem_wait(&shm_buffer->sem_buffer); //Lock the buffer sem

  //Critical Region
  shm_buffer->end = 1;
  n_producers = shm_buffer->n_producers;
  n_consumers = shm_buffer->n_consumers;
  total_producers = shm_buffer->total_producers;
  total_consumers = shm_buffer->total_consumers;
  n_msg_received = shm_buffer->n_msg_received;
  n_msg_processed = shm_buffer->n_msg_processed;
  //End of critical Region
  sem_post(&shm_buffer->sem_buffer); //Unlock the buffer sem
  printf("\n ... Exiting producers and consumers ... \n");

  while(shm_buffer->n_producers != 0 || shm_buffer->n_consumers != 0) {
    printf(" ...\n");
    sleep(1); // espera 1 s para volver a revisar.
  }; //Espera que se cierren todos los productores y consumidores

  close(fd); //Close shared-memory file descriptor
  shm_unlink(shm_name); //Unlink shared-memory object

  gettimeofday(&finish_time, NULL);
  execution_time = (double)(finish_time.tv_usec - start_time.tv_usec)/1000000 +
                   (double)(finish_time.tv_sec - start_time.tv_sec);

  printf("\n--------------------------------\n");
  printf("         FINISHER STATS\n");
  printf("--------------------------------\n");

  printf("\nTotal pending messages: %d.\n", n_msg_received - n_msg_processed);
  printf("Total received messages: %d.\n", n_msg_received);
  printf("Total processed messages: %d.\n", n_msg_processed);
  printf("Total producers: %d.\n", total_producers);
  printf("Total consumers: %d.\n", total_consumers);
  printf("Closed producers: %d.\n", n_producers);
  printf("Closed consumers: %d.\n", n_consumers);

  printf("\nExecution time: %f seconds.\n", execution_time);

  exit(EXIT_SUCCESS);
}
