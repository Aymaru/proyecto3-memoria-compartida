#include "finisher.h"

/*--------
  GLOBAL
--------*/



/*--------------------------
       MAIN FINALIZADOR
--------------------------*/
int main(int argc, char *argv[]) {
  struct sigaction act;
  act.sa_handler = SIG_IGN;
  sigaction(SIGINT, &act, NULL);


  char shm_name[25]; //Nombre del segmento de memoria compartida.
  struct buffer_t * buff,* shm_buffer; //Puntero para inicializar el buffer y puntero para mapear la memoria compartida usando mmap.
  int fd; //File descriptor de la memoria compartida
  int * array_size;
  size_t size;
  int n_producers, n_consumers;

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
  
  array_size = mmap(0,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  size = *array_size;
  munmap(array_size, sizeof(size_t));
  
  shm_buffer = mmap(0, sizeof(struct buffer_t) + (sizeof(struct message_t) * size),
                    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  //Accesing shared-memory buffer  
  sem_wait(&shm_buffer->sem_buffer); //Lock the buffer sem

  print_buffer_status(shm_buffer);
  //Critical Region
  shm_buffer->end = 1;
  n_producers = shm_buffer->n_producers;
  n_consumers = shm_buffer->n_consumers;
  //End of critical Region
  sem_post(&shm_buffer->sem_buffer); //Unlock the buffer sem
  printf("\n ... Exiting producers and consumers ... \n");
  while(shm_buffer->n_producers != 0 || shm_buffer->n_consumers != 0) {
    printf(".");
    sleep(1); // espera 1 s para volver a revisar.
  }; //Espera que se cierren todos los productores y consumidores
  printf("\nSTATS\n\n");
  printf("Productores cerrados: %d.\n", n_producers);
  printf("Consumidores cerrados: %d.\n", n_consumers);
  close(fd); //Close shared-memory file descriptor
  shm_unlink(shm_name); //Unlink shared-memory object

  return 1;
}
