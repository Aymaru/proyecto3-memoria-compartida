#include "finisher.h"

/*--------
  GLOBAL
--------*/



/*--------------------------
       MAIN FINALIZADOR
--------------------------*/
int main(int argc, char *argv[]) {
  char shm_name[25]; //Nombre del segmento de memoria compartida.
  struct buffer_t * buff,* shm_buffer; //Puntero para inicializar el buffer y puntero para mapear la memoria compartida usando mmap.
  int fd; //File descriptor de la memoria compartida
  int * array_size;
  size_t size;

  if (argc >= 2) {
    for (int i=1;i<argc;i++) {
      if (strcmp(argv[i],"-n") == 0) { //Option Buffer Name
        if (argv[++i] == NULL){
          printf("Invalid buffer name. Please input a valid buffer name.\n");
          exit(0);
        }
        strcpy(shm_name, argv[i]);
        //printf("Buffer name: %s\n", shm_name);
      } else { //default
        printf("Invalid Option. Use: ./finisher.o -n [Nombre del buffer].\n");
        exit(0);
      };
    };
  } else {
    printf("Invalid option. Use: ./creator.o -s [Cantidad de Mensajes] -n [Nombre del buffer].\n");
    exit(0);
  };

  /* Open shared memory object */
  struct message_t * tmp_msg;
  int index;
  printf("antes.\n");
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
  printf("despues de shm open.\n");
  
  array_size = mmap(0,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  size = *array_size;
  munmap(array_size, sizeof(size_t));
  
  shm_buffer = mmap(0, sizeof(struct buffer_t) + (sizeof(struct message_t) * size),
                    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
  sem_wait(&shm_buffer->sem_buffer); //Lock the buffer sem
  
  //Critical Region
  print_buffer_status(shm_buffer);
  shm_buffer->end = 1;
  tmp_msg = get_msg(shm_buffer,&index);
  print_message(tmp_msg,index);
  free(tmp_msg);
  //End of critical Region
  sem_post(&shm_buffer->sem_buffer); //Unlock the buffer sem
  
  while(shm_buffer->n_producers != 0 || shm_buffer->n_producers != 0) {
    sleep(5); // podria esperar 5s cada vez que revisa
    //Hay que ver como espera el profe las estadisticas del finisher.
  }; //Espera que se cierren todos los productores y consumidores
  close(fd); //Close shared-memory file descriptor
  shm_unlink(shm_name); //Unlink shared-memory object

  return 1;
}
